#ifndef COMMAND_H
  #define COMMAND_H
  typedef struct STRUCT_ROOM {
    int  iRoomId;
    int  iRoomCapacity;
    int  iDifficultyLevel;
    int  iRoomType;
    char szPlayerName[128];
    char szRoomName[128];
    char szPassword[128];
  } STRUCT_ROOM;

  int iCMD_CreateRoom(char **ppszArgs);
  int iCMD_JoinRoom(char **ppszArgs);
  int iCMD_DeleteRoom(char **ppszArgs);
  int iCMD_LeaveRoom(char **ppszArgs);
  int iCMD_PatchRoom(char **ppszArgs);
  int iCMD_GetRoom(char **ppszArgs);
#endif