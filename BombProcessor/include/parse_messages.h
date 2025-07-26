#ifndef PARSE_MESSAGES_H
  #define PARSE_MESSAGES_H
  typedef enum eCMDList {
    CMD_CREATE_ROOM = 1,
    CMD_JOIN_ROOM,
    CMD_DELETE_ROOM,
    CMD_DISCONNECT
  } eCMDList;

  void vHandleCreateRoom(char **ppszArgs, int iArgCount);
  void vHandleJoinRoom(char **ppszArgs, int iArgCount);
  void vHandleDeleteRoom(char **ppszArgs, int iArgCount);
  void vHandleDisconnect();
  int iParseCommand(char *pszBuffer, int *piCmdId, char ***ppszArgs, int *piArgCount);
  void vProcessCommand(char *pszCmd);
#endif
