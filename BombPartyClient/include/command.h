#include <config.h>

#ifndef COMMAND_H
  #define COMMAND_H

  #define MAX_MSG 512
  #define _MAX_RSL_BUFFER 904800

  #define DEFAULT_SOCK_IP   "127.0.0.1"
  #define DEFAULT_SOCK_PORT 5050

  extern char gszSocketIP[256];
  extern int giSocketPort;
  extern int giSocketClient;

  typedef enum eCMDList {
    CMD_CREATE_ROOM = 1,
    CMD_JOIN_ROOM,
    CMD_DELETE_ROOM,
    CMD_LEAVE_ROOM,
    CMD_PATCH_ROOM,
    CMD_GET_ROOM
  } eCMDList;

  int iConnectToProcessor();
  int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam, char *pszRsl, int iRslSz);
  int iInitSockets(int iArgCt, char *paszArgv[]);
#endif