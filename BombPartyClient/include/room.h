#ifndef ROOM_H
  #define ROOM_H

  typedef struct STRUCT_ROOM_ROLES {
    int iPlayerRole;
    STRUCT_PLAYER *pstPlayer;
    STRUCT_ROOM_ROLES *pstNext;
  } STRUCT_ROOM_ROLES, PSTRUCT_ROOM_ROLES;

  typedef struct STRUCT_ROOM {
    int  iRoomId;
    int  iRoomStatus;
    int  iRoomCapacity;
    int  iRoomDifficulty;
    char szRoomName[128];
    PSTRUCT_ROOM pstNext;
    STRUCT_ROOM_ROLES pstRoles;
  } STRUCT_ROOM, *PSTRUCT_ROOM;

  typedef struct STRUCT_ROOM_LIST{
    STRUCT_ROOM_LIST *pstFirst;
    STRUCT_ROOM_LIST *pstLast;
  } STRUCT_ROOM_LIST;

  extern STRUCT_ROOM_LIST *pstRoomList;
#endif