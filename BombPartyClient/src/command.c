#include <sys_interface.h>
#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trace.h>
#include <room.h>
#include <bombparty.h>

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

// room_key|room_status|room_name|password|room_capacity|room_level

// 99|01|3|1|Sala de Renatinho|null|5|2\n
// 99|02|1|created
// 99|03|2|medium
// 99|04|11|Renatinho

//  #define ROOM_REG_ID 99
//  #define SECTION_ROOM 1
//  #define SECTION_STATUS 2
//  #define SECTION_LEVEL  3
//  #define SECTION_PLAYER 4

void vSendMsg(int iSock, char *pszMsg, int iLen) {
  send(iSock, pszMsg, iLen, 0);
}
void vSendBye(int iSock) {
  vSendMsg(iSock, "BYE\n", strlen("BYE\n"));
}

char *pszNext_token(char *input) {
  return strtok(input, "|");
}

int iNext_int(char *pszInput) {
  char *tok = strtok(pszInput, "|");
  return tok ? atoi(tok) : 0;
}

int iNext_line(char *pszInput) {
  char *tok = strtok(pszInput, "\n");

  return tok ? *(++tok) : 0;
}

/*
  99|01|9|1|sala de since|null|3|2
  99|02|1|created
  99|03|2|medium
  99|04|9|since 
*/
int iParseCreateRoom(char *pszInput, unsigned long ulInputLen, PSTRUCT_ROOM pstRoom) {
  char *pszData;

  if ( !memcmp(pszInput, "ERR", 3) )
    return -1;

  pszData = (char *)malloc(ulInputLen);

  strcpy(pszData, pszInput);

  vTraceVarArgsFn("Parse Input[%s]", pszInput);
  
  iNext_int(pszData);
  iNext_int(NULL);

  pstRoom->iRoomId        = iNext_int(NULL);
  pstRoom->iRoomStatus    = iNext_int(NULL);

  strcpy(pstRoom->szRoomName, pszNext_token(NULL));
  
  char *pwd = pszNext_token(NULL);
  if (!bStrIsEmpty(pwd))
      strcpy(pstRoom->szRoomPassword, pwd);

  pstRoom->iRoomCapacity   = iNext_int(NULL);
  pstRoom->iRoomDifficulty = iNext_int(NULL);

  pstRoom->pstNextRole = NULL;
  pstRoom->pstNextRoom = NULL;

  printf("%s|%d|%d|%d|%d\n",  pstRoom->szRoomName,
                              pstRoom->iRoomStatus,
                              pstRoom->iRoomId,
                              pstRoom->iRoomDifficulty,
                              pstRoom->iRoomCapacity);

  return 0;
}

/*
  99|01|9|1|sala de since|null|3|2
  99|02|1|created
  99|03|2|medium
  99|04|9|since 
  99|04|9|since 
  99|04|9|since 
**/
int iParsePlayer(char *pszInput, unsigned long ulInputLen, PSTRUCT_PLAYER pstPlayer) {
  char *pszData = malloc(ulInputLen + 1);
  
  if (!pszData)
    return -1;

  strcpy(pszData, pszInput);

  char *pLine = strtok(pszData, "\n");
  PSTRUCT_PLAYER pstWrkPlayer = pstPlayer;
  int firstPlayer = 1;

  while (pLine != NULL) {
    if (strncmp(pLine + 3, "04", 2) == 0) {
      char *pTok;

      if (!firstPlayer) {
        pstWrkPlayer->pstNext = malloc(sizeof(STRUCT_PLAYER));
        if (!pstWrkPlayer->pstNext) {
          free(pszData);
          return -1;
        }
        pstWrkPlayer = pstWrkPlayer->pstNext;
        memset(pstWrkPlayer, 0, sizeof(STRUCT_PLAYER));
        pstWrkPlayer->pstNext = NULL;
      }
      firstPlayer = 0;

      pTok = strtok(pLine, "|");
      pTok = strtok(NULL, "|");
      pTok = strtok(NULL, "|");
      if (pTok)
        pstWrkPlayer->iPlayerId = atoi(pTok);

      pTok = strtok(NULL, "|");
      if (pTok)
        strcpy(pstWrkPlayer->szPlayerName, pTok);
    }

    pLine = strtok(NULL, "\n");
  }

  free(pszData);
  return 0;
}


// OK|BYTES|XXX
int iParseRcvMsg(char *pszMsg, int iMsgLen){
  if ( iMsgLen <= 0 )
    return -1; 

  if ( memcmp(pszMsg, OK_MSG, 2) )
    return -2;

  if ( memcmp(&pszMsg[3], BYTES_MSG, strlen(BYTES_MSG)) )
    return -3;

  
  return atoi(&pszMsg[3+strlen(BYTES_MSG)+1]);
}

int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam, char *pszRsl, int iRslSz) {
  char szCommand[MAX_MSG];
  int iBytes;
  int iBytesToRead = 0;

  /* Monta o comando */ 
  snprintf(szCommand, sizeof(szCommand), "CMD|%03d|%s\n", iCmdId, pszParam);

  vTraceVarArgsFn("comando = [%s]", szCommand);

  iInitSockets();
  
  /* Envia */ 
  send(iSockClient, szCommand, strlen(szCommand), 0);

  /* Espera resposta */ 
  memset(pszRsl, 0, iRslSz);
  iBytes = recv(iSockClient, pszRsl, iRslSz - 1, 0);
  
  vTraceVarArgsFn("rslBytes = [%s]", pszRsl);
  /* Resposta invalida */ 
  if (iBytes <= 0) return -1;

  if ((iBytesToRead = iParseRcvMsg(pszRsl, iBytes)) < 0 )
    return -2;
  
  memset(pszRsl, 0, iRslSz);
  iBytes = recv(iSockClient, pszRsl, iRslSz - 1, 0);
  
  if (iBytes <= 0) return -1;

  vTraceVarArgsFn("rsl = [%s]", pszRsl);

  vTraceVarArgsFn("bytes expected = [%d], Rcvd=[%d]", iBytesToRead, iBytes);
  
  vSendBye(iSockClient);
  
  vCloseConnection();
  
  return 0;
}

int iInitSockets() {
  memset(gszSocketIP, 0, sizeof(gszSocketIP));
  strcpy(gszSocketIP, DEFAULT_SOCK_IP);
  if (*giArgc > 1) {
    strcpy(gszSocketIP, gapszArgv[1]);
  } 
  giSocketPort = DEFAULT_SOCK_PORT;
  if (*giArgc > 2) {
    int iSrvPort = atoi(gapszArgv[2]);
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
  iConnectToProcessor();

  return 0;
}

void vCloseConnection(){
#ifdef _WIN32
  closesocket(giSocketClient);
#else
  close(giSocketClient);
#endif
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
