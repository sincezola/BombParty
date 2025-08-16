#include <command.h>
#include <config.h>
#include <player.h>
#include <room.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <input.h>
#include <sys_interface.h>
#include <trace.h>
#include <terminal_utils.h> 

char *pszDifficulty[] = {"-", "Easy", "Medium", "Hard", NULL};

char *pszStatus[] = {"-", "Created", "In game", "Closed", NULL};

char *pszRoles[] = {"-", "Owner", "Guest", "Spectator", NULL};

STRUCT_ROOM_LIST gstRoomList;

/**
  Maximo de caracteres
    Nome: 36
    Complexidade: 16
    Capacidade: 14
    Status: 10
    __________________________________________________________________________
    |     Nome    |     Complexidade     |     Capacidade    |     Status    |
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
*/

int iCalcPaddingOffset(int iFieldSize, int *iContentLen) {
  int iEmptyLeftSize;

  if (iFieldSize <= 0 || *iContentLen <= 0)
    return -1;

  if (iFieldSize < *iContentLen)
    return -2;

  if (*iContentLen % 2)
    *iContentLen = *iContentLen + 1;

  iEmptyLeftSize = iFieldSize - *iContentLen;

  return (iEmptyLeftSize / 2);
}

PSTRUCT_ROOM pstSelectRoomFromList() {
  char szLine[1024];
  int iSelected = 0;
  int iKey = 0;
  int iRoomCt = 0;
  int ii;
  PSTRUCT_ROOM pstRoom;
  PSTRUCT_ROOM pstWrkRoom;

  if ( (iRoomCt = iGetTotalRoomCt()) == 0 ){
    vTraceVarArgsFn("No room to select..");
    return NULL;
  }
   
  while (TRUE) {
    int iIndex = 0;
    pstWrkRoom = gstRoomList.pstFirst;

    vClearTerminal();
    vPrintLine("Use as setas /\\ e \\/ para navegar, ENTER para selecionar:\n", INSERT_NEW_LINE);

    for (iIndex = 0; pstWrkRoom != NULL; pstWrkRoom = pstWrkRoom->pstNextRoom, iIndex++) {
      char szRoomCapacity[32];
      char *pszStatusStr = pszStatus[pstWrkRoom->iRoomStatus];
      char *pszDiffStr = pszDifficulty[pstWrkRoom->iRoomDifficulty];

      sprintf(szRoomCapacity, "%d/%d", iRoomPlayerCt(pstWrkRoom), pstWrkRoom->iRoomCapacity);

      if (iIndex == iSelected) {
        memset(szLine, 0, sizeof(szLine));
        sprintf(szLine,
      "%-16s | %-8s | %-8s | %-8s",
          pstWrkRoom->szRoomName, pszDiffStr, szRoomCapacity, pszStatusStr
        );

        vPrintHighlitedLine(szLine, INSERT_NEW_LINE);
      } else {
        memset(szLine, 0, sizeof(szLine));
        sprintf(szLine,
      "%-16s | %-8s | %-8s | %-8s",
          pstWrkRoom->szRoomName, pszDiffStr, szRoomCapacity, pszStatusStr
        );
        vPrintLine(szLine, INSERT_NEW_LINE);
      }
    }

    iKey = iPortableGetchar();
    if (iKey == -1) {      
      iSelected = (iSelected - 1 + iRoomCt) % iRoomCt;
    } 
    else if (iKey == -2) {
      iSelected = (iSelected + 1) % iRoomCt;
    } 
    else if (iKey == '\n' || iKey == '\r') {
      pstRoom = gstRoomList.pstFirst;
      for (ii = 0; ii < iSelected && pstRoom != NULL; ii++)
        pstRoom = pstRoom->pstNextRoom;
      return pstRoom;
    }
  }

  return NULL;
}

void vDrawRooms() {
  int iNameLen;
  int iNameOffset;
  int iDifficultyLen;
  int iDifficultyOffset;
  int iCapacityLen;
  int iCapacityOffset;
  int iStatusLen;
  int iStatusOffset;
  char szDivLine[128];
  char szLine[1024];
  PSTRUCT_ROOM pstRoom;

  #ifdef FAKE
    vTraceVarArgsFn("vDrawRooms Dummy Rooms");
    vCreateDummyRooms();
  #else
    iGetServerRoom();
  #endif
  vClearTerminal();
  /** Separator Line */
  memset(szLine, 0, sizeof(szLine));
  memset(szDivLine, 0, sizeof(szDivLine));
  szDivLine[0] = ' ';
  memset(&szDivLine[1], '_', ROOM_MAX_CHARS - 2);
  szLine[ROOM_MAX_CHARS - 1] = ' ';

  /** Title Line */
  iNameLen = strlen(ROOM_NAME);
  iNameOffset = iCalcPaddingOffset(NAME_FLD_SIZE, &iNameLen);

  iDifficultyLen = strlen(ROOM_DIFFICULTY);
  iDifficultyOffset = iCalcPaddingOffset(DIFFICULTY_FLD_SIZE, &iDifficultyLen);

  iCapacityLen = strlen(ROOM_CAPACITY);
  iCapacityOffset = iCalcPaddingOffset(CAPACITY_FLD_SIZE, &iCapacityLen);

  iStatusLen = strlen(ROOM_STATUS);
  iStatusOffset = iCalcPaddingOffset(STATUS_FLD_SIZE, &iStatusLen);

  vPrintLine(szDivLine, INSERT_NEW_LINE);
  memset(szLine, 0, sizeof(szLine));
  sprintf(szLine,
"|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
    iNameOffset, iNameOffset, " ", iNameLen, iNameLen, ROOM_NAME, iNameOffset, iNameOffset, " ", 
    iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, ROOM_DIFFICULTY, iDifficultyOffset, iDifficultyOffset, " ",
    iCapacityOffset, iCapacityOffset, " ", iCapacityLen, iCapacityLen, ROOM_CAPACITY, iCapacityOffset, iCapacityOffset, " ",
    iStatusOffset, iStatusOffset, " ", iStatusLen, iStatusLen, ROOM_STATUS, iStatusOffset, iStatusOffset, " "
  );
  vPrintLine(szLine, INSERT_NEW_LINE);

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    /** Room Line */
    char *pszRoomName = pstRoom->szRoomName;
    char *pszRoomDifficulty = pszDifficulty[pstRoom->iRoomDifficulty];
    char szRoomCapacity[64];
    char *pszRoomStatus = pszStatus[pstRoom->iRoomStatus];

    memset(szRoomCapacity, 0, sizeof(szRoomCapacity));
    sprintf(szRoomCapacity, "%d/%d", iRoomPlayerCt(pstRoom), pstRoom->iRoomCapacity);

    iNameLen = strlen(pszRoomName);
    iNameOffset = iCalcPaddingOffset(NAME_FLD_SIZE, &iNameLen);

    iDifficultyLen = strlen(pszRoomDifficulty);
    iDifficultyOffset = iCalcPaddingOffset(DIFFICULTY_FLD_SIZE, &iDifficultyLen);

    iCapacityLen = strlen(szRoomCapacity);
    iCapacityOffset = iCalcPaddingOffset(CAPACITY_FLD_SIZE, &iCapacityLen);

    iStatusLen = strlen(pszRoomStatus);
    iStatusOffset = iCalcPaddingOffset(STATUS_FLD_SIZE, &iStatusLen);

    memset(szLine, 0, sizeof(szLine));
    sprintf(szLine,
  "|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
      iNameOffset, iNameOffset, " ", iNameLen, iNameLen, pszRoomName, iNameOffset, iNameOffset, " ",
      iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, pszRoomDifficulty, iDifficultyOffset, iDifficultyOffset, " ",
      iCapacityOffset, iCapacityOffset, " ", iCapacityLen, iCapacityLen, szRoomCapacity, iCapacityOffset, iCapacityOffset, " ",
      iStatusOffset, iStatusOffset, " ", iStatusLen, iStatusLen, pszRoomStatus, iStatusOffset, iStatusOffset, " "
    );
    vPrintLine(szLine, INSERT_NEW_LINE);
  }
  vPrintLine(szDivLine, INSERT_NEW_LINE);
}

void vInitRoomList() {
  gstRoomList.pstFirst = NULL;
  gstRoomList.pstLast = NULL;
}

void vAddRoom2List(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM *pstLast;
  if (pstRoom == NULL)
    return;

  if (gstRoomList.pstFirst == NULL) {
    gstRoomList.pstFirst = pstRoom;
    gstRoomList.pstLast = pstRoom;
  } else { /* Ja existe alguem na lista */
    pstLast = &gstRoomList.pstLast;
    (*pstLast)->pstNextRoom = pstRoom;
    (*pstLast) = pstRoom;
  }
}

PSTRUCT_ROOM pstCreateRoom(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM pstNewRoom = (PSTRUCT_ROOM)malloc(sizeof(STRUCT_ROOM));

  if (pstNewRoom == NULL)
    return NULL;

  memset(pstNewRoom, 0, sizeof(STRUCT_ROOM));
  memcpy(pstNewRoom, pstRoom, sizeof(STRUCT_ROOM));
  pstNewRoom->pstNextRoom = NULL;
  pstNewRoom->pstNextRole = NULL;
  return pstNewRoom;
}

int iAddPlayer2Room(int iRole, PSTRUCT_PLAYER pstPlayer, PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM_ROLES pstRoomRole;
  PSTRUCT_ROOM_ROLES pTmp;
  
  if (pstPlayer == NULL || pstRoom == NULL)
      return -1;

  pstRoomRole = malloc(sizeof(STRUCT_ROOM_ROLES));
  if (pstRoomRole == NULL)
      return -2; // Erro: malloc falhou

  memset(pstRoomRole, 0, sizeof(STRUCT_ROOM_ROLES));
  pstRoomRole->iPlayerRole = iRole;
  pstRoomRole->pstPlayer = pstPlayer;
  pstRoomRole->pstNext = NULL;

  if (pstRoom->pstNextRole == NULL) {
      pstRoom->pstNextRole = pstRoomRole;
  } else {
      pTmp = pstRoom->pstNextRole;
      while (pTmp->pstNext != NULL)
          pTmp = pTmp->pstNext;
      pTmp->pstNext = pstRoomRole;
  }

  vTraceVarArgs("Player %s adicionado à sala %s com role %d", pstPlayer->szPlayerName, pstRoom->szRoomName, iRole);

  return 0;
}


int iRoomPlayerCt(PSTRUCT_ROOM pstRoom) {
  int iPlayerCt = 0;
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  
  for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext)
    iPlayerCt++;
  
  return iPlayerCt;
}

int iGetServerRoom() {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  PSTRUCT_ROOM pstRoom = NULL;
  PSTRUCT_PLAYER pstPlayer = NULL;
  STRUCT_ROOM stRoom;
  STRUCT_PLAYER stPlayer;
  
  memset(szParams, 0, sizeof(szParams));
  strncpy(szParams, "A", sizeof(szParams));
#ifndef FAKE
  if (iSendCommandToProcessor(giSocketClient, CMD_GET_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao pegar salas do servidor. Prms[%s] Rsl=%s", szParams, szRsl);
    
    return -1;  
  }
#endif
  vInitRoomList(); 
  vInitPlayerList();

  memset(&stRoom, 0, sizeof(STRUCT_ROOM));
  memset(&stPlayer, 0, sizeof(STRUCT_PLAYER));

  if ( iParseCreateRoom(szRsl, sizeof(szRsl), &stRoom) ) 
    return -1;
    
  pstRoom = pstCreateRoom(&stRoom);
  vAddRoom2List(pstRoom);

  if (pstRoom == NULL)
    return -1;

  vTraceVarArgs("SZ RSL!!: %s", szRsl);
  if ( iParsePlayer(szRsl, sizeof(szRsl), &stPlayer) < 0 )
    return -1; 
  
  pstPlayer = pstCreatePlayer(&stPlayer);

  vLogPlayerList();

  vAddPlayer2List(pstPlayer);
  iAddPlayer2Room(ROLE_OWNER, pstPlayer, pstRoom);
  
  for (pstPlayer = pstPlayer->pstNext; pstPlayer != NULL; pstPlayer = pstPlayer->pstNext){
    vAddPlayer2List(pstPlayer);
    iAddPlayer2Room(ROLE_GUEST, pstPlayer, pstRoom);
  }
  
  return 0;
}

void vLogRoom(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM pWrk;

  if (pstRoom == NULL) return;

  pWrk = pstRoom;

  vTraceVarArgs(
    "[Room Id: %d\n"
    "Room Status: %d\n"
    "Room Capacity: %d\n"
    "Room Difficulty: %d\n"
    "Room Password: %s\n"
    "Room Name: %s\n]",
    pWrk->iRoomId,
    pWrk->iRoomStatus,
    pWrk->iRoomCapacity,
    pWrk->iRoomDifficulty,
    (pWrk->szRoomPassword[0] != 0x00) ? pWrk->szRoomPassword : "XXX",
    pWrk->szRoomName
  );
}

void vLogRoomList() {
  PSTRUCT_ROOM pWrk;

  for (pWrk = gstRoomList.pstFirst; pWrk != NULL; pWrk = pWrk->pstNextRoom) {
    vTraceVarArgs(
      "[Room Id: %d\n"
      " Room Status: %d\n"
      " Room Capacity: %d\n"
      " Room Difficulty: %d\n"
      " Room Password: %s\n"
      " Room Name: %s\n]",
      pWrk->iRoomId,
      pWrk->iRoomStatus,
      pWrk->iRoomCapacity,
      pWrk->iRoomDifficulty,
      (pWrk->szRoomPassword[0] != '\0') ? pWrk->szRoomPassword : "XXX",
      pWrk->szRoomName
    );

    PSTRUCT_ROOM_ROLES pRole = pWrk->pstNextRole;
    while (pRole != NULL) {
      vTraceVarArgs(
        "  [Player Role: %d, Player Name: %s]",
        pRole->iPlayerRole,
        pRole->pstPlayer ? pRole->pstPlayer->szPlayerName : "Unknown"
      );
      pRole = pRole->pstNext;
    }
  }
}

/**
CMD|CMD_CREATE_ROOM|playername|roomname|roomcapacity|dificultylevel|{password}
*/
PSTRUCT_ROOM pstCreateServerRoom(char *pszRoomName, char *pszPlayerName, int iRoomCapacity, int iDifficultyLevel, char *pszPassword) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  PSTRUCT_ROOM pstRoom = NULL;
  STRUCT_ROOM stRoom;
  PSTRUCT_PLAYER pstPlayer = NULL;
  STRUCT_PLAYER stPlayer;
  /** TODO: Toggle comment on parse */
  
  memset(szParams, 0, sizeof(szParams));
  snprintf(szParams, sizeof(szParams), "%s|%s|%d|%d", pszPlayerName, pszRoomName, iRoomCapacity, iDifficultyLevel);
  if (!bStrIsEmpty(pszPassword)) {
    strcat(szParams, "|");
    strcat(szParams, pszPassword);
  }
#ifndef FAKE
  if (iSendCommandToProcessor(giSocketClient, CMD_CREATE_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao criar sala no servidor. Prms[%s] Rsl=%s", szParams, szRsl);
    
    return NULL;  
  }
#else
  memset(szRsl,0,sizeof(szRsl));
  sprintf(szRsl,
"%s",  
    "99|01|3|1|Sala de Renatinho|null|5|2\n"
    "99|02|1|created\n"
    "99|03|2|medium\n"
    "99|04|11|Renatinho\n"
  );
#endif

  /** TODO: Parse Rsl  **/

  memset(&stRoom, 0, sizeof(STRUCT_ROOM));
  memset(&stPlayer, 0, sizeof(STRUCT_PLAYER));
  if ( iParseCreateRoom(szRsl, sizeof(szRsl), &stRoom) ) 
    return NULL;
    
  pstRoom = pstCreateRoom(&stRoom);
  if (pstRoom == NULL)
    return NULL;
  
  if ( iParsePlayer(szRsl, sizeof(szRsl), &stPlayer) < 0 )
    return NULL; 

  pstPlayer = pstCreatePlayer(&stPlayer);
  vAddPlayer2List(pstPlayer);
  iAddPlayer2Room(ROLE_OWNER, pstPlayer, pstRoom);
  for (pstPlayer = pstPlayer->pstNext; pstPlayer != NULL; pstPlayer = pstPlayer->pstNext){
    vAddPlayer2List(pstPlayer);
    iAddPlayer2Room(ROLE_GUEST, pstPlayer, pstRoom);
  }

  return pstRoom;
}

int iNewPlayerRoom() {
  int iDifficulty = 0;
  int iCapacity = 0;
  char szPlayerName[128];
  char szRoomName[128];
  char szPassword[128];
  PSTRUCT_ROOM pstRoom;

  vReadPlayerName(szPlayerName, sizeof(szPlayerName));
  vReadRoomName(szRoomName, sizeof(szRoomName));
  vReadRoomDifficulty(&iDifficulty);
  vReadRoomCapacity(&iCapacity);
  vReadRoomPassword(szPassword, sizeof(szPassword));
  
  vTraceVarArgsFn(
"PName=[%s] RName=[%s] Diff=[%d] Cap=[%d] Pw=[%s]",
    szRoomName, szPlayerName, iCapacity, iDifficulty, szPassword
  );  
  
  pstRoom = pstCreateServerRoom(szRoomName, szPlayerName, iCapacity, iDifficulty, szPassword);
  
  if (pstRoom == NULL)
    return -1;

  vAddRoom2List(pstRoom);
  
  return 0;
}

int iNewRoom(int iId, char *pszName, int iStatus, int iCapacity, int iDifficulty) {
  STRUCT_ROOM stRoom;
  PSTRUCT_ROOM pstRoom;

  strcpy(stRoom.szRoomName, pszName);
  stRoom.iRoomId = iId;
  stRoom.iRoomCapacity = iCapacity;
  stRoom.iRoomStatus = iStatus;
  stRoom.iRoomDifficulty = iDifficulty;

  pstRoom = pstCreateRoom(&stRoom);

  if (pstRoom == NULL)
    return -1;

  vAddRoom2List(pstRoom);

  return 0;
}

/* CMD|CMD_JOIN_ROOM|playername|room_id| */
int iJoinRoom() {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  char szPlayerName[128];
  PSTRUCT_ROOM pstRoom;
  
  if ( (pstRoom = pstSelectRoomFromList()) == NULL ) {
    vPrintLine("\t Nenhuma sala selecionada.", INSERT_NEW_LINE);
    return -1;
  }

  vReadPlayerName(szPlayerName, sizeof(szPlayerName));

  memset(szParams,0,sizeof(szParams));
  snprintf(szParams, sizeof(szParams), "%s|%d\n", szPlayerName, pstRoom->iRoomId);

  if (iSendCommandToProcessor(giSocketClient, CMD_JOIN_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao entrar em sala no servidor. Prms[%s] Rsl=%s", szParams, szRsl);
    return -1;
  }
  /** TODO: parse rsl */
  /*
    iNewPlayer(iPlayerId, pszPlayerName);
    iAddPlayer2Room(ROLE_GUEST, pstFindPlayer(iPlayerId), pstRoom);
  */

  return 0;
}

void vClearRoomRoleList(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM_ROLES pstCurrent;
  PSTRUCT_ROOM_ROLES pstLast;

  pstCurrent = pstRoom->pstNextRole;

  while (pstCurrent != NULL) {
    pstLast = pstCurrent;
    pstCurrent = pstCurrent->pstNext;
    free(pstLast);
  }
}

void vClearRoomList() {
  PSTRUCT_ROOM pstCurrent;
  PSTRUCT_ROOM pstLast;

  pstCurrent = gstRoomList.pstFirst;

  while (pstCurrent != NULL) {
    vClearRoomRoleList(pstCurrent);
    pstLast = pstCurrent;
    pstCurrent = pstCurrent->pstNextRoom;
    free(pstLast);
  }

  gstRoomList.pstFirst = NULL;
  gstRoomList.pstLast = NULL;
}

PSTRUCT_ROOM pstFindRoom(int iId) {
  PSTRUCT_ROOM pstRoom;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom)
    if (pstRoom->iRoomId == iId)
      return pstRoom;

  return NULL;
}

PSTRUCT_ROOM pstFindRoomByPlayer(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM pstRoom;
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext) {
      if (pstRoomRoles->pstPlayer == pstPlayer)
        return pstRoom;
    }
  }

  return NULL;
}

int iFindPlayerRole(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM pstRoom;
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext) {
      if (pstRoomRoles->pstPlayer == pstPlayer)
        return pstRoomRoles->iPlayerRole;
    }
  }

  return -1;
}

void vCreateDummyRooms() {
  vInitRoomList();
  vInitPlayerList();

  iNewPlayer(1, "owner1");
  iNewRoom(101, "Sala1", ROOM_CREATED, 5, EASY);
  iAddPlayer2Room(ROLE_OWNER, pstFindPlayer(1), pstFindRoom(101));

  iNewPlayer(2, "owner2");
  iNewRoom(102, "Sala2", ROOM_CREATED, 4, MEDIUM);
  iAddPlayer2Room(ROLE_OWNER, pstFindPlayer(2), pstFindRoom(102));

  iNewPlayer(3, "owner3");
  iNewPlayer(4, "player1");
  iNewRoom(103, "Sala3", ROOM_CREATED, 5, HARD);
  iAddPlayer2Room(ROLE_OWNER, pstFindPlayer(3), pstFindRoom(103));
  iAddPlayer2Room(ROLE_GUEST, pstFindPlayer(4), pstFindRoom(103));
}

int iGetTotalRoomCt() {
  int iRoomCt = 0;
  PSTRUCT_ROOM pstRoom;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom)
    iRoomCt++;

  return iRoomCt;
}