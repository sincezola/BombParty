#include <command.h>
#include <config.h>
#include <player.h>
#include <room.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <input.h>
#include <hud.h>
#include <sys_interface.h>
#include <trace.h>
#include <terminal_utils.h> 

char *pszDifficulty[] = {"-", "Easy", "Medium", "Hard", NULL};

char *pszStatus[] = {"-", "Created", "In game", "Closed", NULL};

char *pszRoles[] = {"-", "Owner", "Guest", "Spectator", NULL};

STRUCT_ROOM_LIST gstRoomList;
PSTRUCT_ROOM gpstCurrentRoom;

/**
  Maximo de caracteres
    Nome: 36
    Complexidade: 16
    Capacidade: 14
    Status: 10
    Password: 12
    __________________________________________________________________________________________
    |     Nome    |     Complexidade     |     Capacidade     |     Status     |   Password   |
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
    |-------------|----------------------|--------------------|----------------|
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

int iSelectPlayer(PSTRUCT_ROOM pstRoom) {
  char szLine[1024];
  int iSelected = 0;
  int iKey = 0;
  int iPlayerCt = 0;
  int ii;
  PSTRUCT_ROOM_ROLES pstWrkRole;

  if ( (iPlayerCt = iRoomPlayerCt(pstRoom)) == 0 ){
    vTraceVarArgsFn("No player to select..");
    return -1;
  }
   
  while (TRUE) {
    int iIndex = 0;

    vClearTerminal();
    vPrintLine("Use as setas /\\ e \\/ para navegar, ENTER para selecionar:\n", INSERT_NEW_LINE);

    for (iIndex = 0, pstWrkRole = pstRoom->pstNextRole; pstWrkRole != NULL; pstWrkRole = pstWrkRole->pstNext, iIndex++) {
      PSTRUCT_PLAYER pstWrkPlayer = pstWrkRole->pstPlayer; 
      memset(szLine, 0, sizeof(szLine));
      sprintf(szLine,
    "%-16s",
        pstWrkPlayer->szPlayerName
      );
      if (iIndex == iSelected) {
        vPrintHighlitedLine(szLine, INSERT_NEW_LINE);
      } else {
        vPrintLine(szLine, INSERT_NEW_LINE);
      }
    }

    iKey = iPortableGetchar();
    if (iKey == -1) {      
      iSelected = (iSelected - 1 + iPlayerCt) % iPlayerCt;
    } 
    else if (iKey == -2) {
      iSelected = (iSelected + 1) % iPlayerCt;
    } 
    else if (iKey == '\n' || iKey == '\r') {
      PSTRUCT_ROOM_ROLES pRole = pstRoom->pstNextRole;
      PSTRUCT_PLAYER pstPlayer =NULL;
      for (ii = 0; ii < iSelected && pRole != NULL; ii++){
        if ( pRole->pstPlayer == NULL )
          return -1;
        pstPlayer = pRole->pstPlayer;
        pRole = pRole->pstNext;
      }
      if ( pstPlayer != NULL )
        return pstPlayer->iPlayerId;
    }
  }

  return -1;
}

int iGetOptionCtFromChoice(int iChoice){
  switch(iChoice){
    case EDITABLE_OPT_CAPACITY:
      return MAX_CAPACITY;
    case EDITABLE_OPT_DIFFICULTY:
      return MAX_DIFFICULTY;
    case EDITABLE_OPT_BOTH:
      return EDITABLE_OPT_BOTH;
  }
  return iChoice;
}

int iChooseEditable(PSTRUCT_ROOM pstRoom) {
  char szLine[1024];
  int iOpt = iReadEditable();
  int iSelected = 0;

  while (TRUE) {
    int iIndex;
    int iMaxOpt = iGetOptionCtFromChoice(iOpt);
    
    if ( iMaxOpt == MAX_CAPACITY || iMaxOpt == EDITABLE_OPT_BOTH ){
      int iLeftEditable = iMaxOpt - pstRoom->iRoomCapacity + iRoomPlayerCt(pstRoom);
      for (iIndex = 0; iIndex < (iLeftEditable + iIndex) ; iIndex++) {
        memset(szLine, 0, sizeof(szLine));
        sprintf(szLine,
          "%d/%d", iRoomPlayerCt(pstRoom), (iLeftEditable+iIndex)
        );
        if (iIndex == iSelected) 
          vPrintHighlitedLine(szLine, INSERT_NEW_LINE);
        else
          vPrintLine(szLine, INSERT_NEW_LINE);
      }
    }
    if ( iMaxOpt == MAX_DIFFICULTY || iMaxOpt == EDITABLE_OPT_BOTH ){
    }
  }
  return 0;
}

int iEditRoom(PSTRUCT_ROOM pstRoom) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  
  memset(szParams, 0, sizeof(szParams));

  if (iSendCommandToProcessor(giSocketClient, CMD_PATCH_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao pegar salas do servidor. Prms[%s] Rsl=%s", szParams, szRsl);
    
    return -1;  
  }

  if ( iParseResult(szRsl, sizeof(szRsl), NULL) ) 
    return -1;
  
  return 0;
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
  int iPasswordLen;
  int iPasswordOffset;
  char szDivLine[128];
  char szLine[1024];
  PSTRUCT_ROOM pstRoom;

  iGetServerRoom();
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

  iPasswordLen = strlen(ROOM_PASSWORD);
  iPasswordOffset = iCalcPaddingOffset(PASSWORD_FLD_SIZE, &iPasswordLen);

  vPrintLine(szDivLine, INSERT_NEW_LINE);
  memset(szLine, 0, sizeof(szLine));
  sprintf(szLine,
"|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s\n",
    iNameOffset, iNameOffset, " ", iNameLen, iNameLen, ROOM_NAME, iNameOffset, iNameOffset, " ", 
    iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, ROOM_DIFFICULTY, iDifficultyOffset, iDifficultyOffset, " ",
    iCapacityOffset, iCapacityOffset, " ", iCapacityLen, iCapacityLen, ROOM_CAPACITY, iCapacityOffset, iCapacityOffset, " ",
    iStatusOffset, iStatusOffset, " ", iStatusLen, iStatusLen, ROOM_STATUS, iStatusOffset, iStatusOffset, " ",
    iPasswordOffset, iPasswordOffset, " ", iPasswordLen, iPasswordLen, ROOM_PASSWORD, iPasswordOffset, iPasswordOffset, " "
  );
  vPrintLine(szLine, INSERT_NEW_LINE);

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    /** Room Line */
    char *pszRoomName = pstRoom->szRoomName;
    char *pszRoomDifficulty = pszDifficulty[pstRoom->iRoomDifficulty];
    char szRoomCapacity[64];
    char *pszRoomStatus = pszStatus[pstRoom->iRoomStatus];
    char szPw[64];

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

    iPasswordLen = 1;
    iPasswordOffset = iCalcPaddingOffset(PASSWORD_FLD_SIZE, &iPasswordLen);
    
    memset(szPw, 0, sizeof(szPw));
    sprintf(szPw, "%c", PASSWORD_NO); 
    if ( !bStrIsEmpty(pstRoom->szRoomPassword) )
      sprintf(szPw, "%c", PASSWORD_YES); 

    memset(szLine, 0, sizeof(szLine));
    sprintf(szLine,
  "|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
      iNameOffset, iNameOffset, " ", iNameLen, iNameLen, pszRoomName, iNameOffset, iNameOffset, " ",
      iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, pszRoomDifficulty, iDifficultyOffset, iDifficultyOffset, " ",
      iCapacityOffset, iCapacityOffset, " ", iCapacityLen, iCapacityLen, szRoomCapacity, iCapacityOffset, iCapacityOffset, " ",
      iStatusOffset, iStatusOffset, " ", iStatusLen, iStatusLen, pszRoomStatus, iStatusOffset, iStatusOffset, " ",
      iPasswordOffset, iPasswordOffset, " ", iPasswordLen, iPasswordLen, szPw, iPasswordOffset, iPasswordOffset, " "
    );
    vPrintLine(szLine, INSERT_NEW_LINE);
  }
  vPrintLine(szDivLine, INSERT_NEW_LINE);
}

void vInitRoomList() {
  gstRoomList.pstFirst = NULL;
  gstRoomList.pstLast = NULL;
  gpstCurrentRoom = NULL;
}

void vAddRoom2List(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM *pstLast;
  if (pstRoom == NULL)
    return;

  if (gstRoomList.pstFirst == NULL) {
    gstRoomList.pstFirst = pstRoom;
    gstRoomList.pstLast = pstRoom;
    pstLast = &gstRoomList.pstLast;
  } else { /* Ja existe alguem na lista */
    pstLast = &gstRoomList.pstLast;
    (*pstLast)->pstNextRoom = pstRoom;
    pstLast = pstLast->pstNextRoom;
  }
  (*pstLast)->pstNextRoom = NULL;
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
  pstRoomRole->bReadyStatus = STATUS_NOT_READY;
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

  vTraceVarArgsFn("Player %s adicionado à sala %s com role %d", pstPlayer->szPlayerName, pstRoom->szRoomName, iRole);

  return 0;
}


int iRoomPlayerCt(PSTRUCT_ROOM pstRoom) {
  int iPlayerCt = 0;
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  
  for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext)
    iPlayerCt++;
  
  return iPlayerCt;
}

int iRoomReadyCt(PSTRUCT_ROOM pstRoom) {
  int iReadyCt = 0;
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  
  for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext)
    if (pstRoomRoles->bReadyStatus) iReadyCt++;
  
  return iReadyCt;
}


int iGetServerRoom() {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  STRUCT_PLAYER stPlayer;
  
  memset(szParams, 0, sizeof(szParams));
  strncpy(szParams, "A", sizeof(szParams));
#ifndef FAKE
  if (iSendCommandToProcessor(giSocketClient, CMD_GET_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao pegar salas do servidor. Prms[%s] Rsl=%s", szParams, szRsl);
    
    return -1;  
  }
#endif
  vClearAll();
  
  memset(&stPlayer, 0, sizeof(STRUCT_PLAYER));

  if ( iParseResult(szRsl, sizeof(szRsl), NULL) ) 
    return -1;

  vLogRoomList();
  vLogPlayerList();
  
  return 0;
}
void vLogRoomRoles(PSTRUCT_ROOM pstRoom){
  PSTRUCT_ROOM_ROLES pRole;

  pRole = pstRoom->pstNextRole;
  while (pRole != NULL) {
    vTraceVarArgsFn(
      "  [Player Role: %d, Player Name: %s]",
      pRole->iPlayerRole,
      pRole->pstPlayer ? pRole->pstPlayer->szPlayerName : "Unknown"
    );
    pRole = pRole->pstNext;
  }
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
  vLogRoomRoles(pWrk);
}


void vLogRoomList() {
  PSTRUCT_ROOM pWrk;

  for (pWrk = gstRoomList.pstFirst; pWrk != NULL; pWrk = pWrk->pstNextRoom) {
    vTraceVarArgsFn(
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
    vLogRoomRoles(pWrk);
  }
}

/**
CMD|CMD_CREATE_ROOM|playername|roomname|password|roomcapacity|dificultylevel|
*/
PSTRUCT_ROOM pstCreateServerRoom(char *pszRoomName, char *pszPlayerName, int iRoomCapacity, int iDifficultyLevel, char *pszPassword) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];
  PSTRUCT_ROOM pstRoom = NULL;
  STRUCT_ROOM stRoom;
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
  if ( iParseResult(szRsl, sizeof(szRsl), &stRoom) ) 
    return NULL;
    
  if ( (pstRoom = pstFindRoom(stRoom.iRoomId)) == NULL )
    return NULL;

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
  
  gpstCurrentRoom = pstRoom;
  if ( pstRoom == NULL || (gpstCurrentPlayer = pstFindRoomPlayerOwner(pstRoom)) == NULL )
    return -1;
  

  vAddRoom2List(pstRoom);
  
  vDrawHud(pstRoom, pstRoom->pstNextRole->pstPlayer);

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
  int iCount = 0;
  char szParams[256];
  char szRoomPw[128];
  char szRsl[_MAX_RSL_BUFFER];
  char szPlayerName[128];
  PSTRUCT_ROOM pstRoom;
  
  if ( (pstRoom = pstSelectRoomFromList()) == NULL ) {
    vPrintLine("\t Nenhuma sala selecionada.", INSERT_NEW_LINE);
    return -1;
  }

  memset(szRoomPw, 0, sizeof(szRoomPw));

  vReadPlayerName(szPlayerName, sizeof(szPlayerName));
  if ( !bStrIsEmpty(pstRoom->szRoomPassword) ) {
    do {
      vReadRoomPlayerPassword(szRoomPw, sizeof(szRoomPw));

      iCount++;
    } while (strcmp(pstRoom->szRoomPassword, szRoomPw) && iCount < 3);

    if (iCount >= 3) memset(szRoomPw, 0, sizeof(szRoomPw));
  }

  memset(szParams, 0, sizeof(szParams));
  snprintf(szParams, sizeof(szParams), "%s|%d", szPlayerName, pstRoom->iRoomId);

  if (!bStrIsEmpty(pstRoom->szRoomPassword)) {
    strcat(szParams, "|");
    strcat(szParams, szRoomPw);
  }

  strcat(szParams, "\n");

  if (iSendCommandToProcessor(giSocketClient, CMD_JOIN_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao entrar em sala no servidor. Prms[%s] Rsl[%s]", szParams, szRsl);
    return -1;
  }

  gpstCurrentRoom = pstRoom;
  if ( pstRoom == NULL || (gpstCurrentPlayer = pstFindRoomPlayerByName(pstRoom, szPlayerName)) == NULL )
    return -1;

  return 0;
}

int iLeaveRoom(int iPlayerId) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];

  memset(szParams, 0, sizeof(szParams));
  memset(szRsl, 0, sizeof(szRsl));

  snprintf(szParams, sizeof(szParams), "%d\n", iPlayerId);
  
  vTraceVarArgsFn("CMD_LEAVE_ROOM. Prms[%s]", szParams);

  if (iSendCommandToProcessor(giSocketClient, CMD_LEAVE_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao deletar da sala no servidor. Prms[%s] Rsl[%s]", szParams, szRsl);

    return -1;
  }

  vClearAll();

  return 0;
}

int iDestroyRoom(int iRoomId) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];

  memset(szParams, 0, sizeof(szParams));
  memset(szRsl, 0, sizeof(szRsl));

  snprintf(szParams, sizeof(szParams), "%d\n", iRoomId);

  if (iSendCommandToProcessor(giSocketClient, CMD_DELETE_ROOM, szParams, szRsl, sizeof(szRsl)) != 0) {
    vTraceVarArgsFn("Erro ao sair da sala no servidor. Prms[%s] Rsl[%s]", szParams, szRsl);

    return -1;
  }
  
  vClearAll();
  
  return 0;
}

void vClearAll() {
  vClearPlayerList();
  vClearRoomList();
  vInitRoomList();
  vInitPlayerList();
}

void vClearRoomRoleList(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM_ROLES pstCurrent;
  PSTRUCT_ROOM_ROLES pstLast;
  int  iRoom = 0;

  if ( pstRoom == NULL ) return;

  pstCurrent = pstRoom->pstNextRole;
  iRoom = pstRoom->iRoomId;

  vTraceVarArgsFn("vClearRoomRoleList - Clearing roles from Room=%d", iRoom);

  while (pstCurrent != NULL) {
    pstLast = pstCurrent;
    pstCurrent = pstCurrent->pstNext;
    free(pstLast);
  }
  
  vTraceVarArgsFn("vClearRoomRoleList - Roles of Room=%d were cleared", iRoom);
}

void vClearRoomList() {
  PSTRUCT_ROOM pstCurrent;
  PSTRUCT_ROOM pstLast;

  pstCurrent = gstRoomList.pstFirst;

  vTraceVarArgsFn("vClearRoomList Begin");

  while (pstCurrent != NULL) {
    vClearRoomRoleList(pstCurrent);
    pstLast = pstCurrent;
    pstCurrent = pstCurrent->pstNextRoom;
    free(pstLast);
  }

  gstRoomList.pstFirst = NULL;
  gstRoomList.pstLast = NULL;
  
  vTraceVarArgsFn("vClearRoomList OK");
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

PSTRUCT_ROOM_ROLES pstFindPlayerRole(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  PSTRUCT_ROOM pstRoom;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext) {
      if (pstRoomRoles->pstPlayer == pstPlayer)
        return pstRoomRoles;
    }
  }

  return NULL;
}

int iFindPlayerRole(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  if ( (pstRoomRoles = pstFindPlayerRole(pstPlayer)) == NULL )
    return -1;

  return pstRoomRoles->iPlayerRole;
}

PSTRUCT_ROOM pstFindPlayerRoom(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  PSTRUCT_ROOM pstRoom;

  for (pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom) {
    for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext) {
      if (pstRoomRoles->pstPlayer == pstPlayer)
        return pstRoom;
    }
  }

  return NULL;
}

void vInitServerGame(PSTRUCT_ROOM pstRoom) {
  char szParams[256];
  char szRsl[_MAX_RSL_BUFFER];

  memset(szParams, 0, sizeof(szParams));
  memset(szRsl, 0, sizeof(szRsl));

  snprintf(szParams, sizeof(szParams), "%d|%d|%d|%d\n", pstRoom->iRoomId, pstRoom->iRoomDifficulty, pstRoom->iRoomCapacity, ROOM_IN_GAME);

  if (iSendCommandToProcessor(giSocketClient, CMD_PATCH_ROOM, szParams, szRsl, sizeof(szRsl)) != 0)
    vTraceVarArgsFn("Erro ao sair da sala no servidor. Prms[%s] Rsl[%s]", szParams, szRsl);
  

  return;
}

int iTogglePlayerRdyStatus(PSTRUCT_PLAYER pstPlayer){
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  if ( (pstRoomRoles = pstFindPlayerRole(pstPlayer)) == NULL )
    return -1;

  pstRoomRoles->bReadyStatus = !pstRoomRoles->bReadyStatus;

  
  vTraceVarArgsFn("Alterado ready de player=[%s] para %s", pstPlayer->szPlayerName, pstRoomRoles->bReadyStatus ? "Pronto" : "Nao pronto");

  return 0;
}

int iGetReadyStatus(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  if ( (pstRoomRoles = pstFindPlayerRole(pstPlayer)) == NULL )
    return -1;
      
  return pstRoomRoles->bReadyStatus;
}

void vCreateDummyRooms() {
  vClearAll();

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

PSTRUCT_PLAYER pstFindRoomPlayerOwner(PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext){
    if ( pstRoomRoles->iPlayerRole == ROLE_OWNER )
      return pstRoomRoles->pstPlayer;
  }
  return NULL;
}

PSTRUCT_PLAYER pstFindRoomPlayerByName(PSTRUCT_ROOM pstRoom, char *pszName){
  PSTRUCT_ROOM_ROLES pstRoomRoles;
  PSTRUCT_PLAYER pstPlayer;
  
  if ( bStrIsEmpty(pszName) )
    return NULL;
  
  for (pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext){
    pstPlayer = pstRoomRoles->pstPlayer;
    if ( !strcmp(pstPlayer->szPlayerName, pszName) )
      return pstPlayer;
  }
  
  return NULL;
}
