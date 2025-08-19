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
  #define STATUS_NOT_READY    0
  #define STATUS_READY        1
  
  typedef enum eRoomRoles {
    ROLE_OWNER = 1,
    ROLE_GUEST,
    ROLE_SPECTATOR
  } eRoomRoles;

  typedef struct STRUCT_ROOM_ROLES {
    int iPlayerRole;
    bool bReadyStatus;
    PSTRUCT_PLAYER pstPlayer;
    struct STRUCT_ROOM_ROLES *pstNext;
  } STRUCT_ROOM_ROLES, *PSTRUCT_ROOM_ROLES;

  typedef struct STRUCT_ROOM {
    int  iRoomId;
    int  iRoomStatus;
    int  iRoomCapacity;
    int  iRoomDifficulty;
    char szRoomPassword[128];
    char szRoomName[128];
    struct STRUCT_ROOM *pstNextRoom;
    PSTRUCT_ROOM_ROLES pstNextRole;
  } STRUCT_ROOM, *PSTRUCT_ROOM;

  typedef struct STRUCT_ROOM_LIST{
    PSTRUCT_ROOM pstFirst;
    PSTRUCT_ROOM pstLast;
  } STRUCT_ROOM_LIST, *PSTRUCT_ROOM_LIST;

  extern STRUCT_ROOM_LIST gstRoomList;

  int iNewRoom(int iId, char *pszName, int iStatus, int iCapacity, int iDifficulty);
  int iNewPlayerRoom();
  int iCalcPaddingOffset(int iFieldSize, int *iContentLen);
  int iRoomPlayerCt(PSTRUCT_ROOM pstRoom);
  int iFindPlayerRole(PSTRUCT_PLAYER pstPlayer);
  int iJoinRoom();
  int iGetTotalRoomCt();
  int iGetServerRoom();
  int iAddPlayer2Room(int iRole, PSTRUCT_PLAYER pstPlayer, PSTRUCT_ROOM pstRoom);
  void vDrawRooms();
  void vInitRoomList();
  void vClearRoomList();
  void vCreateDummyRooms();
  void vLogRoomList();
  void vAddRoom2List(PSTRUCT_ROOM pstRoom);
  PSTRUCT_ROOM pstSelectRoomFromList();
  PSTRUCT_ROOM pstFindRoom(int iId);
  PSTRUCT_ROOM pstCreateRoom(PSTRUCT_ROOM pstRoom);
#endif