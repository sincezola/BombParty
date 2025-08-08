#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trace.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET tSocket;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
typedef int tSocket;
#endif

char gszSocketIP[256];
int giSocketPort = -1;
int giSocketClient = -1;

int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam, char *pszRsl, int iRslSz) {
  char szCommand[MAX_MSG];
  int iBytes;

  // Monta o comando
  snprintf(szCommand, sizeof(szCommand), "CMD|%d|%s\n", iCmdId, pszParam);

  // Envia
  send(iSockClient, szCommand, strlen(szCommand), 0);

  // Espera resposta
  memset(pszRsl, 0, iRslSz);
  iBytes = recv(iSockClient, pszRsl, sizeof(pszRsl) - 1, 0);

  if (iBytes <= 0) {
    return -1;
  }

  return 0;
}

int iInitSockets(int iArgCt, char *paszArgv[]) {
  memset(gszSocketIP, 0, sizeof(gszSocketIP));
  strcpy(gszSocketIP, DEFAULT_SOCK_IP);
  if (iArgCt > 1) {
    strcpy(gszSocketIP, paszArgv[1]);
  } 
  giSocketPort = DEFAULT_SOCK_PORT;
  if (iArgCt > 2) {
    int iSrvPort = atoi(paszArgv[2]);
    if (iSrvPort > 0)
      giSocketPort = iSrvPort;
  }

#ifdef _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    vTraceVarArgsFn("Erro ao iniciar Winsock.\n");
    return -1;
  }
#endif
  return 0;
}
int iConnectToProcessor() {
  struct sockaddr_in stServerAddr;

  giSocketClient = socket(AF_INET, SOCK_STREAM, 0);
  if (giSocketClient < 0) {
    vTraceVarArgsFn("Erro ao criar socket");
    return -1;
  }

  memset(&stServerAddr, 0, sizeof(stServerAddr));
  stServerAddr.sin_family = AF_INET;
  stServerAddr.sin_port = htons(giSocketPort);

  if (inet_pton(AF_INET, gszSocketIP, &stServerAddr.sin_addr) <= 0) {
    vTraceVarArgsFn("Endereço IP inválido");
    return -1;
  }

  // Conecta
  if (connect(giSocketClient, (struct sockaddr *)&stServerAddr, sizeof(stServerAddr)) < 0) {
    vTraceVarArgsFn("Erro ao conectar ao servidor");
#ifdef _WIN32
    closesocket(giSocketClient);
#else
    close(giSocketClient);
#endif
    return -1;
  }

  vTraceVarArgsFn("Conectado ao Processor em %s:%d\n", gszSocketIP, giSocketPort);
  return 0;
  
}


