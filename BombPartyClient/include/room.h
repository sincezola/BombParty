#include <player.h>

#ifndef ROOM_H
  #define ROOM_H

  typedef struct STRUCT_ROOM_ROLES {
    int iPlayerRole;
    PSTRUCT_PLAYER pstPlayer;
    struct STRUCT_ROOM_ROLES *pstNext;
  } STRUCT_ROOM_ROLES, PSTRUCT_ROOM_ROLES;

  typedef struct STRUCT_ROOM {
    int  iRoomId;
    int  iRoomStatus;
    int  iRoomCapacity;
    int  iRoomDifficulty;
    char szRoomName[128];
    struct STRUCT_ROOM *pstNext;
    PSTRUCT_ROOM_ROLES pstRoles;
  } STRUCT_ROOM, *PSTRUCT_ROOM;

  typedef struct STRUCT_ROOM_LIST{
    PSTRUCT_ROOM pstFirst;
    PSTRUCT_ROOM pstLast;
  } STRUCT_ROOM_LIST, *PSTRUCT_ROOM_LIST;

  extern PSTRUCT_ROOM_LIST pstRoomList;

  void vDrawRooms();
#endif