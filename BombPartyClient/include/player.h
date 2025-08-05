#ifndef PLAYER_H
  #define PLAYER_H

  typedef struct STRUCT_PLAYER {
    int iPlayerId;
    char szPlayerName[128];
    struct STRUCT_PLAYER *pstNext;
  } STRUCT_PLAYER, *PSTRUCT_PLAYER;

  typedef struct STRUCT_PLAYER_LIST {
    PSTRUCT_PLAYER pstFirst;
    PSTRUCT_PLAYER pstLast;
  } STRUCT_PLAYER_LIST, *PSTRUCT_PLAYER_LIST;

  extern STRUCT_PLAYER_LIST gstPlayerList;
#endif