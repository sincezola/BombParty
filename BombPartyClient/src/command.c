#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET tSocket;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
typedef int tSocket;
#endif

int giSocketClient = -1;

int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam) {
  char szCommand[MAX_MSG];
  char szResponse[MAX_MSG];
  int iBytes;

  // Monta o comando
  snprintf(szCommand, sizeof(szCommand), "CMD|%d|%s\n", iCmdId, pszParam);

  // Envia
  send(iSockClient, szCommand, strlen(szCommand), 0);

  // Espera resposta
  memset(szResponse, 0, sizeof(szResponse));
  iBytes = recv(iSockClient, szResponse, sizeof(szResponse) - 1, 0);

  if (iBytes > 0) {
    printf("Resposta do processor: %s\n", szResponse);
    return 0;
  }

  return -1;
}


int iConnectToProcessor(const char *pszIpAddr, int iPort) {
  struct sockaddr_in stServerAddr;

#ifdef _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("Erro ao iniciar Winsock.\n");
    return -1;
  }
#endif

  // Cria o socket
  giSocketClient = socket(AF_INET, SOCK_STREAM, 0);
  if (giSocketClient < 0) {
    perror("Erro ao criar socket");
    return -1;
  }

  // Define parâmetros do servidor
  memset(&stServerAddr, 0, sizeof(stServerAddr));
  stServerAddr.sin_family = AF_INET;
  stServerAddr.sin_port = htons(iPort);

  if (inet_pton(AF_INET, pszIpAddr, &stServerAddr.sin_addr) <= 0) {
    perror("Endereço IP inválido");
    return -1;
  }

  // Conecta
  if (connect(giSocketClient, (struct sockaddr *)&stServerAddr,
              sizeof(stServerAddr)) < 0) {
    perror("Erro ao conectar ao servidor");
#ifdef _WIN32
    closesocket(giSocketClient);
#else
    close(giSocketClient);
#endif
    return -1;
  }

  printf("Conectado ao Processor em %s:%d\n", pszIpAddr, iPort);
  return 0;
  
}
