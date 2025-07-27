#ifndef PARSE_MESSAGES_H
  #define PARSE_MESSAGES_H
  typedef enum eCMDList {
    CMD_CREATE_ROOM = 1,
    CMD_JOIN_ROOM,
    CMD_DELETE_ROOM,
    CMD_LEAVE_ROOM,
    CMD_PATCH_ROOM,
    CMD_GET_ROOM
  } eCMDList;

   typedef enum eDifficultyList {
    DIFFICULTY_EASY = 1,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
  } eDifficultyList;

  void vHandleCreateRoom(char **ppszArgs, int iArgCount);
  void vHandleJoinRoom(char **ppszArgs, int iArgCount);
  void vHandleDeleteRoom(char **ppszArgs, int iArgCount);
  void vHandleDisconnect();
  int iParseCommand(char *pszBuffer, int *piCmdId);
  void vProcessCommand(char *pszCmd);
#endif
