/**
 * @file bomb_processor.c
 * @brief Servidor de processamento das salas do jogo BombParty
 *
 * Este servidor escuta em uma porta TCP e cria um subprocesso/thread
 * para cada conexão de cliente (cada sala de jogo).
 */

#include <config.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <parse_messages.h>
#include <trace.h>

#ifdef _WIN32
  #include <process.h>
  #include <windows.h>
  #include <winsock2.h>
  #pragma comment(lib, "ws2_32.lib")
  typedef SOCKET tSocket;
#else /** LINUX */
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <signal.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>
  typedef int tSocket;
#endif

#define SERVER_PORT 5050
#define BACKLOG 10

int giServerPort = 0;
const char *gkpszProgramName;

/**
 * @brief Função que lida com a conexão de um cliente (sala).
 * @param pArg - ponteiro para o socket do cliente
 */
#ifdef _WIN32
unsigned __stdcall vHandleClient(void *pArg)
#else
void vHandleClient(void *pArg)
#endif
{
  char szBuffer[512];
  int iBytes;
  tSocket iClientSock = *((tSocket *)pArg);
  free(pArg);

  vTraceMsg("[INFO] Nova conexão iniciada.");

  while (TRUE) {
    memset(szBuffer, 0, sizeof(szBuffer));
    iBytes = recv(iClientSock, szBuffer, sizeof(szBuffer) - 1, 0);
    if (iBytes <= 0) {
      vTraceMsg("[INFO] Conexão encerrada.");
      break;
    }

    vTraceVarArgs("[CLIENTE] %s", szBuffer);
    vProcessCommand(szBuffer);
    send(iClientSock, "OK\n", 3, 0);
  }

#ifdef _WIN32
  closesocket(iClientSock);
  _endthreadex(0);
  return 0;
#else
  close(iClientSock);
  _exit(0);
#endif
}

/**
 * @brief Inicializa a biblioteca de sockets (Windows)
 */
void vInitSockets() {
#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    vTraceMsg("Erro ao iniciar Winsock.");
    exit(EXIT_FAILURE);
  }
#endif
}

/**
 * @brief Finaliza a biblioteca de sockets (Windows)
 */
void vCleanupSockets() {
#ifdef _WIN32
  WSACleanup();
#endif
}

/**
 * @brief Função principal do servidor
 */
int main(int argc, char *argv[]) {
  tSocket iServerSock, *pClientSock;
  struct sockaddr_in stServerAddr, stClientAddr;
  socklen_t iClientLen = sizeof(stClientAddr);

  gkpszProgramName = argv[0];

  vInitLogs();

  /** Setting non default server parameters
   * argv[1] = Server Port
   */
  giServerPort = SERVER_PORT;
  if (argc > 1) {
    int iSrvPort = atoi(argv[1]);
    if (iSrvPort < 0)
      return -1;
    
    giServerPort = iSrvPort;
  }
  
  vInitSockets();

  /** Creates Socket Server */
  iServerSock = socket(AF_INET, SOCK_STREAM, 0);
  if (iServerSock < 0) {
    perror("Erro ao criar socket");
    vCleanupSockets();
    return EXIT_FAILURE;
  }

  /** Set Server Port */
  memset(&stServerAddr, 0, sizeof(stServerAddr));
  stServerAddr.sin_family = AF_INET;
  stServerAddr.sin_addr.s_addr = INADDR_ANY;
  stServerAddr.sin_port = htons(giServerPort);

  /** Bind Socket Server */
  if (bind(iServerSock, (struct sockaddr *)&stServerAddr,
           sizeof(stServerAddr)) < 0) {
    perror("Erro ao fazer bind");
#ifdef _WIN32
    closesocket(iServerSock);
#else
    close(iServerSock);
#endif
    vCleanupSockets();
    return EXIT_FAILURE;
  }

  /** Listen */
  if (listen(iServerSock, BACKLOG) < 0) {
    perror("Erro ao escutar");
#ifdef _WIN32
    closesocket(iServerSock);
#else
    close(iServerSock);
#endif
    vCleanupSockets();
    return EXIT_FAILURE;
  }

  vTraceVarArgs("[INFO] Servidor escutando na porta %d...", giServerPort);

#ifndef _WIN32
  /** Avoid zombie proccess */
  signal(SIGCHLD, SIG_IGN); 
#endif

  while (TRUE) {
    pClientSock  = (tSocket *)malloc(sizeof(tSocket));
    *pClientSock = accept(iServerSock, (struct sockaddr *)&stClientAddr, &iClientLen);
    if (*pClientSock < 0) {
      perror("Erro ao aceitar conexão");
      free(pClientSock);
      continue;
    }

#ifdef _WIN32
    uintptr_t iThread = _beginthreadex(NULL, 0, vHandleClient, (void *)pClientSock, 0, NULL);
    if (iThread == 0) {
      vTraceMsg("Erro ao criar thread.");
      closesocket(*pClientSock);
      free(pClientSock);
    }
#else /** LINUX */
    pid_t iPID = fork();
    if (iPID == 0) {
      /** Child Process */
      close(iServerSock);
      vHandleClient((void *)pClientSock);
      exit(0);
    } 
    else if (iPID > 0) {
      close(*pClientSock);
      free(pClientSock);
    }
    else {
      perror("Erro no fork");
      close(*pClientSock);
      free(pClientSock);
    }
#endif
  }

#ifdef _WIN32
  closesocket(iServerSock);
#else
  close(iServerSock);
#endif
  vCleanupSockets();

  return 0;
}
