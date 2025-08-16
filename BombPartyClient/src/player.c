#include <trace.h>
#include <config.h>
#include <player.h>
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
  vTraceVarArgs(
    "[Player Id: %d\n"
    "Player Name: %s\n]",
    pstPlayer->iPlayerId,
    pstPlayer->szPlayerName
  );
}

void vLogPlayerList() {
  PSTRUCT_PLAYER pWrk;

  for (pWrk = gstPlayerList.pstFirst; pWrk != NULL; pWrk = pWrk->pstNext) {
    vTraceVarArgs(
      "[Player Id: %d\n"
      "Player Name: %s\n]",
      pWrk->iPlayerId,
      pWrk->szPlayerName
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
