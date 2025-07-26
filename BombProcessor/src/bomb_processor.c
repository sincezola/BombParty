/**
 * @file bomb_processor.c
 * @brief Servidor de processamento das salas do jogo BombParty
 *
 * Este servidor escuta em uma porta TCP e cria um subprocesso/thread
 * para cada conexão de cliente (cada sala de jogo).
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET tSocket;
#else
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
  tSocket iClientSock = *((tSocket *)pArg);
  free(pArg);

  char szBuffer[512];
  int iBytes;

  printf("[INFO] Nova conexão iniciada.\n");

  while (1) {
    memset(szBuffer, 0, sizeof(szBuffer));
    iBytes = recv(iClientSock, szBuffer, sizeof(szBuffer) - 1, 0);
    if (iBytes <= 0) {
      printf("[INFO] Conexão encerrada.\n");
      break;
    }

    printf("[CLIENTE] %s\n", szBuffer);

    // Resposta simples (eco por enquanto)
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
    printf("Erro ao iniciar Winsock.\n");
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
int main() {
  vInitSockets();

  tSocket iServerSock, *pClientSock;
  struct sockaddr_in stServerAddr, stClientAddr;
  socklen_t iClientLen = sizeof(stClientAddr);

  // Cria socket
  iServerSock = socket(AF_INET, SOCK_STREAM, 0);
  if (iServerSock < 0) {
    perror("Erro ao criar socket");
    vCleanupSockets();
    return EXIT_FAILURE;
  }

  // Prepara endereço
  memset(&stServerAddr, 0, sizeof(stServerAddr));
  stServerAddr.sin_family = AF_INET;
  stServerAddr.sin_addr.s_addr = INADDR_ANY;
  stServerAddr.sin_port = htons(SERVER_PORT);

  // Faz bind
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

  // Escuta
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

  printf("[INFO] Servidor escutando na porta %d...\n", SERVER_PORT);

#ifndef _WIN32
  signal(SIGCHLD, SIG_IGN); // Evita processos zumbis no Linux
#endif

  while (1) {
    pClientSock = (tSocket *)malloc(sizeof(tSocket));
    *pClientSock =
        accept(iServerSock, (struct sockaddr *)&stClientAddr, &iClientLen);
    if (*pClientSock < 0) {
      perror("Erro ao aceitar conexão");
      free(pClientSock);
      continue;
    }

#ifdef _WIN32
    uintptr_t iThread =
        _beginthreadex(NULL, 0, vHandleClient, (void *)pClientSock, 0, NULL);
    if (iThread == 0) {
      printf("Erro ao criar thread.\n");
      closesocket(*pClientSock);
      free(pClientSock);
    }
#else
    pid_t pid = fork();
    if (pid == 0) { // Processo filho
      close(iServerSock);
      vHandleClient((void *)pClientSock);
      exit(0);
    } else if (pid > 0) {
      close(*pClientSock);
      free(pClientSock);
    } else {
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
