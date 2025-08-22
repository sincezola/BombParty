#include <trace.h>
#include <config.h>
#include <player.h>
#include <room.h>
#include <stdlib.h>
#include <string.h>

STRUCT_PLAYER_LIST gstPlayerList;

void vInitPlayerList() {
  gstPlayerList.pstFirst = NULL;
  gstPlayerList.pstLast = NULL;
}

void vAddPlayer2List(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_PLAYER *pstLast;
  if (pstPlayer == NULL)
    return;

  if (gstPlayerList.pstFirst == NULL) {
    gstPlayerList.pstFirst = pstPlayer;
    gstPlayerList.pstLast = pstPlayer;
  } else { /* Ja existe alguem na lista */
    pstLast = &gstPlayerList.pstLast;
    (*pstLast)->pstNext = pstPlayer;
    (*pstLast) = pstPlayer;
  }
}

PSTRUCT_PLAYER pstCreatePlayer(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_PLAYER pstNewPlayer = (PSTRUCT_PLAYER)malloc(sizeof(STRUCT_PLAYER));

  if (pstNewPlayer == NULL)
    return NULL;

  memset(pstNewPlayer, 0, sizeof(STRUCT_PLAYER));
  memcpy(pstNewPlayer, pstPlayer, sizeof(STRUCT_PLAYER));
  pstNewPlayer->pstNext = NULL;

  return pstNewPlayer;
}

int iNewPlayer(int iId, char *pszName) {
  STRUCT_PLAYER stPlayer;
  PSTRUCT_PLAYER pstPlayer;

  strcpy(stPlayer.szPlayerName, pszName);
  stPlayer.iPlayerId = iId;

  pstPlayer = pstCreatePlayer(&stPlayer);

  if (pstPlayer == NULL)
    return -1;

  vAddPlayer2List(pstPlayer);

  return 0;
}

void vLogPlayer(PSTRUCT_PLAYER pstPlayer) {
  PSTRUCT_ROOM pWrkRoom;
  char szRoom[256];
  memset(szRoom, 0, sizeof(szRoom));
  strcpy(szRoom, " ");
  if ( (pWrkRoom = pstFindPlayerRoom(pstPlayer)) != NULL ){
    sprintf(szRoom, " Sala=%s ", pWrkRoom->szRoomName);
  }

  vTraceVarArgs(
  "[ Player Id=%d Name=%s%s]",
    pstPlayer->iPlayerId,
    pstPlayer->szPlayerName,
    szRoom
  );
}

void vLogPlayerList() {
  PSTRUCT_PLAYER pWrk;
  PSTRUCT_ROOM pWrkRoom;
  for (pWrk = gstPlayerList.pstFirst; pWrk != NULL; pWrk = pWrk->pstNext) {
    char szRoom[256];
    memset(szRoom, 0, sizeof(szRoom));
    strcpy(szRoom, " ");
    if ( (pWrkRoom = pstFindPlayerRoom(pWrk)) != NULL ){
      sprintf(szRoom, " Sala=%s ", pWrkRoom->szRoomName);
    }

    vTraceVarArgs(
    "[ Player Id=%d Name=%s%s]",
      pWrk->iPlayerId,
      pWrk->szPlayerName,
      szRoom
    );
  }
}

void vClearPlayerList() {
  PSTRUCT_PLAYER pstCurrent;
  PSTRUCT_PLAYER pstNext;

  pstCurrent = gstPlayerList.pstFirst;

  while (pstCurrent != NULL) {
    pstNext = pstCurrent->pstNext;
    free(pstCurrent);
    pstCurrent = pstNext;
  }

  gstPlayerList.pstFirst = NULL;
  gstPlayerList.pstLast = NULL;
}

PSTRUCT_PLAYER pstFindPlayer(int iId) {
  PSTRUCT_PLAYER pstPlayer;

  for ( pstPlayer = gstPlayerList.pstFirst; pstPlayer != NULL; pstPlayer = pstPlayer->pstNext )
    if ( pstPlayer->iPlayerId == iId ) return pstPlayer;

  return NULL;
}
