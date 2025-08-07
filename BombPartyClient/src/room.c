#include <command.h>
#include <stdlib.h>
#include <room.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <player.h>

char *pszDifficulty[]={
  "-",
  "Easy",
  "Medium",
  "Hard",
  NULL
};

char *pszStatus[]={
  "-",
  "Created",
  "In game",
  "Closed",
  NULL
};

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


int iCalcPaddingOffset(int iFieldSize, int *iContentLen){
  int iEmptyLeftSize;

  if ( iFieldSize <= 0 || *iContentLen <= 0 )
    return -1;

  if ( iFieldSize < *iContentLen )
    return -2;

  if ( *iContentLen % 2 ) *iContentLen = *iContentLen + 1;

  iEmptyLeftSize = iFieldSize - *iContentLen;
  
  return (iEmptyLeftSize / 2);
}

void vDrawRoom() {
  int iNameLen;
  int iNameOffset;
  int iDifficultyLen;
  int iDifficultyOffset;
  int iCapacityLen;
  int iCapacityOffset;
  int iStatusLen;
  int iStatusOffset;
  char szLine[128];
  PSTRUCT_ROOM pstRoom;

  /** Separator Line */
  memset(szLine, 0, sizeof(szLine));
  szLine[0] = ' ';
  memset(&szLine[1], '_', ROOM_MAX_CHARS-2);
  szLine[ROOM_MAX_CHARS-1] = ' ';
  
  /** Title Line */
  iNameLen = strlen(ROOM_NAME);
  iNameOffset = iCalcPaddingOffset(NAME_FLD_SIZE, &iNameLen);

  iDifficultyLen = strlen(ROOM_DIFFICULTY);
  iDifficultyOffset = iCalcPaddingOffset(DIFFICULTY_FLD_SIZE, &iDifficultyLen);

  iCapacityLen = strlen(ROOM_CAPACITY);
  iCapacityOffset = iCalcPaddingOffset(CAPACITY_FLD_SIZE, &iCapacityLen);

  iStatusLen = strlen(ROOM_STATUS);
  iStatusOffset = iCalcPaddingOffset(STATUS_FLD_SIZE, &iStatusLen);

  printf("%s\n", szLine);
  printf(
"|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
    iNameOffset,       iNameOffset,       " ", iNameLen,       iNameLen,       ROOM_NAME,       iNameOffset,       iNameOffset,       " ",
    iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, ROOM_DIFFICULTY, iDifficultyOffset, iDifficultyOffset, " ",
    iCapacityOffset,   iCapacityOffset,   " ", iCapacityLen,   iCapacityLen,   ROOM_CAPACITY,   iCapacityOffset,   iCapacityOffset,   " ",
    iStatusOffset,     iStatusOffset,     " ", iStatusLen,     iStatusLen,     ROOM_STATUS,     iStatusOffset,     iStatusOffset,     " "
  );

  /** TODO: Room Lines */
  for ( pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom ){
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
    
    printf(
  "|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
      iNameOffset,       iNameOffset,       " ", iNameLen,       iNameLen,       ROOM_NAME,       iNameOffset,       iNameOffset,       " ",
      iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, ROOM_DIFFICULTY, iDifficultyOffset, iDifficultyOffset, " ",
      iCapacityOffset,   iCapacityOffset,   " ", iCapacityLen,   iCapacityLen,   ROOM_CAPACITY,   iCapacityOffset,   iCapacityOffset,   " ",
      iStatusOffset,     iStatusOffset,     " ", iStatusLen,     iStatusLen,     ROOM_STATUS,     iStatusOffset,     iStatusOffset,     " "
    );
  }

  printf("%s\n", szLine);  
}

void vInitRoomList(){
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

void vAddPlayer2Room(int iRole, PSTRUCT_PLAYER pstPlayer, PSTRUCT_ROOM pstRoom) {
  PSTRUCT_ROOM_ROLES pstRoomRole;
  PSTRUCT_ROOM_ROLES pstWrkRoles;
  
  if (pstPlayer == NULL || pstRoom == NULL) return;

  if ((pstRoomRole = (PSTRUCT_ROOM_ROLES)malloc(sizeof(STRUCT_ROOM_ROLES))) == NULL) return;

  memset(pstRoomRole, 0, sizeof(STRUCT_ROOM_ROLES));

  pstRoomRole->iPlayerRole = iRole;
  pstRoomRole->pstPlayer = pstPlayer;
  pstRoomRole->pstNext = NULL;

  pstWrkRoles = pstRoom->pstNextRole;
  if ( pstWrkRoles == NULL ) {
    pstRoom->pstNextRole = pstRoomRole;
    
    return;
  }

  for ( ; pstWrkRoles->pstNext != NULL; pstWrkRoles = pstWrkRoles->pstNext);

  pstWrkRoles->pstNext = pstRoomRole;
}

int iRoomPlayerCt(PSTRUCT_ROOM pstRoom){
  int iPlayerCt = 0;
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for ( pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom ) {
    for ( pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext )
      iPlayerCt++;
  }
  return iPlayerCt;
}


PSTRUCT_ROOM pstNewServerRoom(const char *pszRoomName, const char *pszPlayerName) {
    char szParams[256];
    char szResponse[512];
    PSTRUCT_ROOM pstRoom;
    STRUCT_ROOM stRoom;

    // Monta os parâmetros: <room_name>|<player_name>
    snprintf(szParams, sizeof(szParams), "%s|%s", pszRoomName, pszPlayerName);

    // Envia o comando CMD_CREATE_ROOM
    if (iSendCommandToProcessor(giSocketClient, CMD_CREATE_ROOM, szParams) != 0) {
        fprintf(stderr, "Erro ao criar sala no servidor.\n");
        return NULL;
    }

    pstRoom = pstCreateRoom(&stRoom);
    if (!pstRoom)
        return NULL;

    vAddRoom2List(pstRoom);
    return pstRoom;
}


int iNewPlayerRoom(){
  char szPlayerName[128];
  char szRoomName[128];
  PSTRUCT_ROOM pstRoom;

  vReadPlayerName(szPlayerName, sizeof(szPlayerName));
  vReadPlayerName(szRoomName, sizeof(szRoomName));
  
  pstRoom = pstNewServerRoom(szRoomName, szPlayerName);

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

  for ( pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom )
    if ( pstRoom->iRoomId == iId ) return pstRoom;

  return NULL;
}

PSTRUCT_ROOM pstFindRoomByPlayer(PSTRUCT_PLAYER pstPlayer){
  PSTRUCT_ROOM pstRoom;
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for ( pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom ) {
    for ( pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext ) {
      if ( pstRoomRoles->pstPlayer == pstPlayer )
        return pstRoom;
    }
  }

  return NULL;
}

int iFindPlayerRole(PSTRUCT_PLAYER pstPlayer){
  PSTRUCT_ROOM pstRoom;
  PSTRUCT_ROOM_ROLES pstRoomRoles;

  for ( pstRoom = gstRoomList.pstFirst; pstRoom != NULL; pstRoom = pstRoom->pstNextRoom ) {
    for ( pstRoomRoles = pstRoom->pstNextRole; pstRoomRoles != NULL; pstRoomRoles = pstRoomRoles->pstNext ) {
      if ( pstRoomRoles->pstPlayer == pstPlayer )
        return pstRoomRoles->iPlayerRole;
    }
  }

  return -1;
}
