#include <player.h>

#ifndef ROOM_H
  #define ROOM_H

  #define ROOM_MAX_CHARS 81
  #define ROOM_NAME       "Nome"
  #define ROOM_DIFFICULTY "Complexidade"
  #define ROOM_CAPACITY   "Capacidade"
  #define ROOM_STATUS     "Status"
  #define NAME_FLD_SIZE       36
  #define DIFFICULTY_FLD_SIZE 16
  #define CAPACITY_FLD_SIZE   14
  #define STATUS_FLD_SIZE     10

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

  extern STRUCT_ROOM_LIST gstRoomList;

  void vDrawRooms();
  
#endif