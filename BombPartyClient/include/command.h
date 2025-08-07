#ifndef COMMAND_H
  #define COMMAND_H

  #define MAX_MSG 512

  extern int giSocketClient;

  typedef enum eCMDList {
    CMD_CREATE_ROOM = 1,
    CMD_JOIN_ROOM,
    CMD_DELETE_ROOM,
    CMD_LEAVE_ROOM,
    CMD_PATCH_ROOM,
    CMD_GET_ROOM
  } eCMDList;

  int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam);
#endif