#include <bombparty.h>
#include <command.h>
#include <room.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys_interface.h>
#include <trace.h>
#include <config.h>

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
void vSendBye(int iSock) { vSendMsg(iSock, "BYE\n", strlen("BYE\n")); }

char *pszNext_token(char *input) { return strtok(input, "|"); }

int iNext_int(char *pszInput) {
  char *tok = strtok(pszInput, "|");
  return tok ? atoi(tok) : 0;
}

int iNext_line(char *pszInput) {
  char *tok = strtok(pszInput, "\n");

  return tok ? *(++tok) : 0;
}

/*
  99|01|1|1|SALA RENATO|null|4|1
  99|02|1|created
  99|03|1|easy
  99|04|1|RENATO
  99|01|2|1|sala de since|null|3|2
  99|02|1|created
  99|03|2|medium
  99|04|3|since
  99|04|4|since
  99|01|3|1|SALA RENATO|null|4|1
  99|02|1|created
  99|03|1|easy
  99|04|5|RENATO
**/
int iParseResult(char *pszInput, unsigned long ulInputLen, PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM pstWrkRoom;
  PSTRUCT_ROOM pstFirstRoom;
  char *pszData;
  char *pLine;
  char *pszSavePtrLine;
  int iRsl = 0;

  pszData = malloc(ulInputLen + 1);
  if (!pszData)
    return -1;

  memset(pszData, 0, ulInputLen + 1);
  strcpy(pszData, pszInput);

  pLine = strtok_r(pszData, "\n", &pszSavePtrLine);
  pstWrkRoom = (PSTRUCT_ROOM)malloc(sizeof(STRUCT_ROOM));

  if (!pstWrkRoom) {
    free(pszData);
    return -1;
  }

  memset(pstWrkRoom, 0, sizeof(STRUCT_ROOM));
  pstWrkRoom->pstNextRoom = NULL;
  pstFirstRoom = pstWrkRoom;

  while (pLine != NULL) {
    if (strncmp(pLine + 3, "01", 2) == 0) {
      char *pszSavePtrTok;
      char *pTok;

      vTraceVarArgsFn("REGISTRO 01 ENCONTRADO!");

      if (pstFirstRoom->iRoomId > 0) {
        pstWrkRoom->pstNextRoom = (PSTRUCT_ROOM)malloc(sizeof(STRUCT_ROOM));
        if (!pstWrkRoom->pstNextRoom) {
          free(pszData);
          return -1;
        }

        pstWrkRoom = pstWrkRoom->pstNextRoom;
        memset(pstWrkRoom, 0, sizeof(STRUCT_ROOM));
        pstWrkRoom->pstNextRoom = NULL;
      }

      pTok = strtok_r(pLine, "|", &pszSavePtrTok); // 99 - ignore
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);  // 01 - ignore

      if (bStrIsEmpty(pTok)) {
        free(pszData);
        free(pstWrkRoom);
        return 0;
      }

      // iRoomId
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok)
        pstWrkRoom->iRoomId = atoi(pTok);

      // iRoomStatus
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok)
        pstWrkRoom->iRoomStatus = atoi(pTok);

      // szRoomName
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok)
        strcpy(pstWrkRoom->szRoomName, pTok);

      // szRoomPassword
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok) {
        /** Ajustar json_parse.js colocar |-1 na ausencia */
        if (strcasecmp(pTok, "null") == 0)
          memset(pstWrkRoom->szRoomPassword, 0,
                 sizeof(pstWrkRoom->szRoomPassword));
        else
          strcpy(pstWrkRoom->szRoomPassword, pTok);
      }

      // iRoomCapacity
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok)
        pstWrkRoom->iRoomCapacity = atoi(pTok);

      // iRoomDifficulty
      pTok = strtok_r(NULL, "|", &pszSavePtrTok);
      if (pTok)
        pstWrkRoom->iRoomDifficulty = atoi(pTok);

      vAddRoom2List(pstWrkRoom);
    }

    vLogRoom(pstWrkRoom);

    pLine = strtok_r(NULL, "\n", &pszSavePtrLine);

    if ( (iRsl = iParsePlayer(pszSavePtrLine, strlen(pszSavePtrLine), pstWrkRoom, pszData, ulInputLen + 1)) < 0)
      return -1;
    
    vTraceVarArgsFn("iParsePlayer Rsl=%d", iRsl);

    if ( iRsl == 1 ){
      if ( pstRoom != NULL && pstWrkRoom != NULL )
        memcpy(pstRoom, pstWrkRoom, sizeof(STRUCT_ROOM));
      break;
    }

    pLine = pszData;
  }

  free(pszData);
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
int iParsePlayer(char *pszInput, unsigned long ulInputLen,
                 PSTRUCT_ROOM pstRoom, char *pszOut, int iOutLen) {
  PSTRUCT_PLAYER pstWrkPlayer = NULL;
  PSTRUCT_PLAYER pstFirstPlayer = NULL;
  char *pszData = malloc(ulInputLen + 1);
  char *pLine;
  int iRole;

  if (!pszData)
    return -1;

  strcpy(pszData, pszInput);
  pLine = pszData;
  while (pLine != NULL) {
    char *pTok;
    if ( bStrIsEmpty(pLine) ){
      free(pszData);
      return 1;
    }
    if (memcmp(pLine + 3, "01", 2) == 0){
      memset(pszOut, 0, iOutLen);
      sprintf(pszOut, "%s", pLine);
      free(pszData);
      return 0;
    }
    if (strncmp(pLine + 3, "04", 2) == 0) {
      vTraceVarArgsFn("Registro 04 encontrado");
      if (pstWrkPlayer != NULL) {
        pstWrkPlayer->pstNext = (PSTRUCT_PLAYER)malloc(sizeof(STRUCT_PLAYER));
        if (!pstWrkPlayer->pstNext) {
          free(pszData);
          return -1;
        }
        pstWrkPlayer = pstWrkPlayer->pstNext;
      } else {
        pstWrkPlayer = (PSTRUCT_PLAYER)malloc(sizeof(STRUCT_PLAYER));
        pstFirstPlayer = pstWrkPlayer;
      }
      memset(pstWrkPlayer, 0, sizeof(STRUCT_PLAYER));
      pstWrkPlayer->pstNext = NULL;

      pTok = strtok_r(NULL, "|", &pLine);
      pTok = strtok_r(NULL, "|", &pLine);
      pTok = strtok_r(NULL, "|", &pLine);
      if (!bStrIsEmpty(pTok))
        pstWrkPlayer->iPlayerId = atoi(pTok);

      pTok = strtok_r(NULL, "\n", &pLine);
      if (!bStrIsEmpty(pTok)) {
        strcpy(pstWrkPlayer->szPlayerName, pTok);
        strtok(pstWrkPlayer->szPlayerName, "\n");
      }
    }
    else {
      strtok_r(NULL, "\n", &pLine);
    }


    iRole = ROLE_GUEST;
    if (pstFirstPlayer == pstWrkPlayer)
      iRole = ROLE_OWNER;

    vAddPlayer2List(pstWrkPlayer);
    iAddPlayer2Room(iRole, pstWrkPlayer, pstRoom);
  }

  vTraceVarArgsFn("Fim");

  free(pszData);
  return 1;
}

// OK|BYTES|XXX
int iParseRcvMsg(char *pszMsg, int iMsgLen) {
  if (iMsgLen <= 0)
    return -1;

  if (memcmp(pszMsg, OK_MSG, 2))
    return -2;

  if (memcmp(&pszMsg[3], BYTES_MSG, strlen(BYTES_MSG)))
    return -3;

  return atoi(&pszMsg[3 + strlen(BYTES_MSG) + 1]);
}

int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam,
                            char *pszRsl, int iRslSz) {
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
  if (iBytes <= 0)
    return -1;

  if ((iBytesToRead = iParseRcvMsg(pszRsl, iBytes)) < 0)
    return -2;

  memset(pszRsl, 0, iRslSz);
  iBytes = recv(iSockClient, pszRsl, iRslSz - 1, 0);

  if (iBytes <= 0)
    return -1;

  vTraceVarArgsFn("rsl = [%s]", pszRsl);

  vTraceVarArgsFn("bytes expected = [%d], Rcvd=[%d]", iBytesToRead, iBytes);

  vSendBye(iSockClient);

  vCloseConnection();

  return 0;
}


int iInitSockets(void) {
  memset(gszSocketIP, 0, sizeof(gszSocketIP));
  strcpy(gszSocketIP, DEFAULT_SOCK_IP);

  if (giArgc > 1) {
    strcpy(gszSocketIP, gapszArgv[1]);
  }

  giSocketPort = DEFAULT_SOCK_PORT;

  if (giArgc > 2) {
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

  return iConnectToProcessor();
}


void vCloseConnection() {
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
  if (connect(giSocketClient, (struct sockaddr *)&stServerAddr,
              sizeof(stServerAddr)) < 0) {
    vTraceVarArgsFn("Erro ao conectar ao servidor");
#ifdef _WIN32
    closesocket(giSocketClient);
#else
    close(giSocketClient);
#endif
    return -1;
  }

  vTraceVarArgsFn("Conectado ao Processor em %s:%d\n", gszSocketIP,
                  giSocketPort);
  return 0;
}
