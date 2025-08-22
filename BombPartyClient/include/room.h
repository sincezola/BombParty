#include <player.h>
#include <config.h>

#ifndef ROOM_H
  #define ROOM_H

  #define ROOM_NAME       "Nome"
  #define ROOM_DIFFICULTY "Complexidade"
  #define ROOM_CAPACITY   "Capacidade"
  #define ROOM_STATUS     "Status"
  #define ROOM_PASSWORD   "Password"
  #define NAME_FLD_SIZE       36
  #define DIFFICULTY_FLD_SIZE 16
  #define CAPACITY_FLD_SIZE   14
  #define STATUS_FLD_SIZE     10
  #define PASSWORD_FLD_SIZE   12
  #define ROOM_MAX_CHARS      NAME_FLD_SIZE+DIFFICULTY_FLD_SIZE+CAPACITY_FLD_SIZE+STATUS_FLD_SIZE+PASSWORD_FLD_SIZE
  #define STATUS_NOT_READY    0
  #define STATUS_READY        1
  #define PASSWORD_YES          'S'
  #define PASSWORD_NO           'N'
  #define MAX_EDITABLE_OPTIONS   3
  #define EDITABLE_OPT_CAPACITY   1
  #define EDITABLE_OPT_DIFFICULTY 2
  #define EDITABLE_OPT_BOTH       3
  
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

  int iGetReadyStatus(PSTRUCT_PLAYER pstPlayer);
  int iDestroyRoom(int iRoomId);
  int iLeaveRoom(int iPlayerId);
  int iNewRoom(int iId, char *pszName, int iStatus, int iCapacity, int iDifficulty);
  int iNewPlayerRoom();
  int iCalcPaddingOffset(int iFieldSize, int *iContentLen);
  int iRoomPlayerCt(PSTRUCT_ROOM pstRoom);
  int iRoomReadyCt(PSTRUCT_ROOM pstRoom);
  int iFindPlayerRole(PSTRUCT_PLAYER pstPlayer);
  int iEditRoom(PSTRUCT_ROOM pstRoom);
  int iJoinRoom();
  int iGetTotalRoomCt();
  int iGetServerRoom();
  int iAddPlayer2Room(int iRole, PSTRUCT_PLAYER pstPlayer, PSTRUCT_ROOM pstRoom);
  int iTogglePlayerRdyStatus(PSTRUCT_PLAYER pstPlayer);
  int iSelectPlayer(PSTRUCT_ROOM pstRoom);
  void vDrawRooms();
  void vInitServerGame(PSTRUCT_ROOM pstRoom);
  void vInitRoomList();
  void vClearRoomList();
  void vCreateDummyRooms();
  void vLogRoomList();
  void vLogRoom(PSTRUCT_ROOM pstRoom);
  void vAddRoom2List(PSTRUCT_ROOM pstRoom);
  
  PSTRUCT_ROOM pstSelectRoomFromList();
  PSTRUCT_ROOM pstFindRoom(int iId);
  PSTRUCT_ROOM pstCreateRoom(PSTRUCT_ROOM pstRoom);
  PSTRUCT_ROOM pstFindPlayerRoom(PSTRUCT_PLAYER pstPlayer);
  PSTRUCT_ROOM_ROLES pstFindPlayerRole(PSTRUCT_PLAYER pstPlayer);
#endif