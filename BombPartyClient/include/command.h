#include <config.h>
#include <room.h>

#ifndef COMMAND_H
  #define COMMAND_H
  
  #define ROOM_REG_ID 99
  #define SECTION_ROOM 1
  #define SECTION_STATUS 2
  #define SECTION_LEVEL  3
  #define SECTION_PLAYER 4
  
  #define OK_MSG    "OK"
  #define ERR_MSG   "ERR"
  #define BYTES_MSG "BYTES"

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
  void vCloseConnection();
  int iSendCommandToProcessor(int iSockClient, int iCmdId, const char *pszParam, char *pszRsl, int iRslSz);
  int iInitSockets();
  int iParseResult(char *pszLayout, unsigned long ulInputLen);
  int iParsePlayer(char *pszInput, unsigned long ulInputLen, PSTRUCT_ROOM pstRoom, char *pszOut, int iOutLen);
#endif