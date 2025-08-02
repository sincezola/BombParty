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
  
  #define JSON_PARSER_TITLE "node_scripts/json-parser.js"
  
  int iCMD_CreateRoom(char **ppszArgs, int iSocketClient);
  int iCMD_JoinRoom(char **ppszArgs, int iSocketClient);
  int iCMD_DeleteRoom(char **ppszArgs, int iSocketClient);
  int iCMD_LeaveRoom(char **ppszArgs, int iSocketClient);
  int iCMD_PatchRoom(char **ppszArgs, int iSocketClient);
  int iCMD_GetRoom(char **ppszArgs, int iSocketClient);
  int iJSON_ExternalParse(char *pszJSON, char *pszRsl, int iRslSz, char *pszTitle);

  #define RESULT_OK  1
  #define RESULT_ERR 2

  #define OK_MSG    "OK"
  #define ERR_MSG   "ERR"
  #define BYTES_MSG "BYTES"

#endif