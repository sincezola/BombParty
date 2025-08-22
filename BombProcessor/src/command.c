#include <string.h>
#include <command.h>
#include <routes_api.h>
#include <curl_api.h>
#include <sys_interface.h>
#include <trace.h>
#include <stdlib.h>

#ifdef LINUX
#include <sys/socket.h>
#endif


void vSendMsg(int iSock, char *pszMsg, int iLen) {
  send(iSock, pszMsg, iLen, 0);
}
void vSendBye(int iSock) {
  vSendMsg(iSock, "BYE\n", strlen("BYE\n"));
}
/** 
*  iRslSts RESULT_OK: 
*   "OK|BYTES|lBytes\n",
*  iRslSts RESULT_ERR: 
*   "ERR|BYTES|lBytes\n",
*  
*  OK_MSG  "OK"
*  ERR_MSG "ERR"
*  sprintf(szMsg, "OK|BYTES|%ld\n", strlen(szChildResponse));
*  send(iSocketClient, szMsg, strlen(szMsg), 0);
*  send(iSocketClient, szChildResponse, strlen(szChildResponse), 0); 
*/
void vSendMessageBytes(int iSock, char *pszMessage, long lBytes, int iRslSts) {
  char szMsg[_MAX_PATH];

  if ( lBytes <= 0 )
    return ;

  memset(szMsg, 0, sizeof(szMsg));

  sprintf(szMsg, "%s|%s|%ld\n", 
    (iRslSts == RESULT_OK) ? OK_MSG : ERR_MSG,
    BYTES_MSG,
    lBytes
  );

  vSendMsg(iSock, szMsg, strlen(szMsg));
  vTraceVarArgsFn("BytesMsg[%s]", szMsg);
  vSendMsg(iSock, pszMessage, lBytes);
  vTraceVarArgsFn("FinalMsg[%s]", pszMessage);
}


/**
* CMD|CMD_CREATE_ROOM|playername|roomname|roomcapacity|dificultylevel|{password}
* CMD|001|"since gordo"|"Name"|4|1|
*/
int iCMD_CreateRoom(char **ppszArgs, int iSocketClient) {
  char *pTok;
  char szRsl[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szFullEndpoint[512];
  char szURL[2048];
  char szMsg[258];
  char szPayload[1024];
  char *pszTitle = "createroom.txt";
  STRUCT_ROOM stRoom;

  memset(&stRoom, 0, sizeof(STRUCT_ROOM));
  memset(szPayload, 0, sizeof(szPayload));
  memset(szFullEndpoint, 0, sizeof(szFullEndpoint));
  memset(szMsg, 0, sizeof(szMsg));
  memset(szRsl, 0, sizeof(szRsl));
  memset(szURL, 0, sizeof(szURL));
  
  /** Player Name */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(stRoom.szPlayerName, sizeof(stRoom.szPlayerName), "%s", pTok);
  
  /** Room Name */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(stRoom.szRoomName, sizeof(stRoom.szRoomName), "%s", pTok);
  
  /** Room Capacity */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( pTok != NULL ) 
    stRoom.iRoomCapacity = atoi(pTok);

  /** Difficulty Level */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( pTok != NULL ) 
    stRoom.iDifficultyLevel = atoi(pTok);
  
  /** Password */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(stRoom.szPassword, sizeof(stRoom.szPassword), "%s", pTok);

  if ( !bStrIsEmpty(stRoom.szPassword) ){
    sprintf(szMsg, ",\n\t\"room_password\":\"%s\"\n}", stRoom.szPassword);
  }
  else{
    sprintf(szMsg, "%s", "\n}");
  }

  snprintf(szPayload, sizeof(szPayload),
    "{\n"
    "\t\"player_name\": \"%s\",\n"
    "\t\"room_name\": \"%s\",\n"
    "\t\"room_level\": %d,\n"
    "\t\"room_capacity\": %d"
    "%s",
      stRoom.szPlayerName, 
      stRoom.szRoomName, 
      stRoom.iDifficultyLevel,
      stRoom.iRoomCapacity,
      szMsg
  );

  vTraceVarArgsFn("Msg:[%s]", szPayload);

  strcpy(szFullEndpoint, CREATE_ROOM_PATH); 
  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 

  iCurlReq(szURL, szFullEndpoint, "POST", szPayload, strlen(szPayload), szRsl);

  vTraceVarArgsFn("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|CREATE_ROOM\n", strlen("ERR|CREATE_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");
  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);
  //** Salvar PlayerID no Client */

  return 0;
}

/**
* CMD|CMD_JOIN_ROOM|playername|room_id|
* CMD|002|
*/
int iCMD_JoinRoom(char **ppszArgs, int iSocketClient) {
  int  iRoomId = 0;
  char *pTok;
  char szPlayerName[128];
  char szURL[2048];
  char szRsl[_MAX_RSL_BUFFER];
  char szPayload[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szFullEndpoint[512];
  char *pszTitle = "joinroom.txt";

  memset(szPlayerName, 0, sizeof(szPlayerName));
  memset(szChildResponse, 0, sizeof(szChildResponse));
  memset(szPayload, 0, sizeof(szPayload));
  memset(szRsl, 0, sizeof(szRsl));
  memset(szURL, 0, sizeof(szURL));

  /** Player Name */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(szPlayerName, sizeof(szPlayerName), "%s", pTok);
  
  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iRoomId = atoi(pTok);
  
  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 
  
  strcpy(szFullEndpoint, JOIN_ROOM_PATH);
  sprintf(szPayload, 
    "{\n"
    "\t\"room_key\": %d,\n"
    "\t\"player_name\": \"%s\"\n"
    "}",
      iRoomId,
      szPlayerName
  );

  vTraceVarArgsFn("szPayload:[%s]", szPayload);

  iCurlReq(szURL, szFullEndpoint, METHOD_POST, szPayload, strlen(szPayload), szRsl);

  vTraceVarArgsFn("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|JOIN_ROOM\n", strlen("ERR|JOIN_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");
  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);

  //** Salvar PlayerID no Client */
  return 0;
}

/**
* CMD|CMD_DELETE_ROOM|room_id|
* CMD|003|
*/
int iCMD_DeleteRoom(char **ppszArgs, int iSocketClient) {
  int iRoomId = 0;
  char *pTok;
  char szFullEndpoint[512];
  char szURL[2048];
  char szPayload[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szRsl[_MAX_RSL_BUFFER];
  char *pszTitle = "deleteroom.txt";

  memset(szURL, 0, sizeof(szURL));
  memset(szFullEndpoint, 0, sizeof(szFullEndpoint));
  memset(szPayload, 0, sizeof(szPayload));
  memset(szRsl, 0, sizeof(szRsl));

  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iRoomId = atoi(pTok);

  snprintf(szPayload, sizeof(szPayload),
    "{\n"
    "\t\"room_id\": %d\n"
    "}",
     iRoomId
  );
  
  sprintf(szFullEndpoint, "%s/%d", DELETE_ROOM_PATH, iRoomId);

  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 

  iCurlReq(szURL, szFullEndpoint, METHOD_DELETE, szPayload, strlen(szPayload), szRsl);
  
  vTraceVarArgsFn("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|DELETE_ROOM\n", strlen("ERR|DELETE_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");

  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);

  return 0;
}

/**
* CMD|CMD_LEAVE_ROOM|player_id
* CMD|004|1
*/
int iCMD_LeaveRoom(char **ppszArgs, int iSocketClient) {
  int iPlayerId = 0;
  char *pTok;
  char szFullEndpoint[512];
  char szURL[2048];
  char szPayload[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szRsl[_MAX_RSL_BUFFER];
  char *pszTitle = "leaveroom.txt";

  memset(szURL, 0, sizeof(szURL));
  memset(szFullEndpoint, 0, sizeof(szFullEndpoint));
  memset(szPayload, 0, sizeof(szPayload));
  memset(szRsl, 0, sizeof(szRsl));

  /** Player Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iPlayerId = atoi(pTok);

  sprintf(szFullEndpoint, "%s/%d", LEAVE_ROOM_PATH, iPlayerId);

  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 

  iCurlReq(szURL, szFullEndpoint, METHOD_POST, NULL, 0, szRsl);
  
  vTraceVarArgsFn("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|LEAVE_ROOM\n", strlen("ERR|LEAVE_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");

  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);

  return 0;
}

/**
 * Before game starts:
 *  Difficulty Level
 *  Room Capacity
 * After game starts:
 *  Room Type - (1)- Created, (2) In game - (3) - Closed
 *
 * CMD|CMD_PATCH_ROOM|room_id|[dificultylevel|-]|[roomcapacity|-]|{Roomtype|}
 * CMD|005|2|[3|-]|[4|-]|{S|}
 */
int iCMD_PatchRoom(char **ppszArgs, int iSocketClient) {
  STRUCT_ROOM stRoom;
  char *pTok;  
  char szURL[2048];
  char szPayload[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szRsl[_MAX_RSL_BUFFER];
  char *pszTitle = "patchroom.txt";

  memset(szURL, 0, sizeof(szURL));
  memset(szPayload, 0, sizeof(szPayload));
  memset(szChildResponse, 0, sizeof(szChildResponse));
  memset(szRsl, 0, sizeof(szRsl));
  memset(&stRoom, 0, sizeof(STRUCT_ROOM));

  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    stRoom.iRoomId = atoi(pTok);
  
  /** Difficulty Level */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) && *pTok != '-' ) 
    stRoom.iDifficultyLevel = atoi(pTok);
  
  /** Room Capacity */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) && *pTok != '-' ) 
    stRoom.iRoomCapacity = atoi(pTok);

  /** Room Type */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    stRoom.iRoomType = atoi(pTok);
  
  /**JSON - room_id Comum para ambos */
  snprintf(szPayload, sizeof(szPayload), 
  "{\n"
    "\t\"room_id\": %d,\n",
    stRoom.iRoomId
  );

  if ( stRoom.iRoomType ) {
    /** Room status change */
    char szRoomType[16];
    snprintf(szRoomType, sizeof(szRoomType), "%d", stRoom.iRoomType);
    strcat(szPayload, "\t\"status_type\": ");
    strcat(szPayload, szRoomType);
    strcat(szPayload, "\n");
  }
  else{
    /** Room setup change */
    char szRoomLvl[16];
    char szRoomCapacity[16];
    snprintf(szRoomLvl, sizeof(szRoomLvl), "%d", stRoom.iDifficultyLevel);
    snprintf(szRoomCapacity, sizeof(szRoomCapacity), "%d", stRoom.iRoomCapacity);
    strcat(szPayload, "\t\"room_level\": ");
    strcat(szPayload, szRoomLvl);
    strcat(szPayload, ",\n");
    strcat(szPayload, "\t\"room_capacity\": ");
    strcat(szPayload, szRoomCapacity);
    strcat(szPayload, "\n");
  } 
  /**JSON - '}' Comum para ambos */
  strcat(szPayload, "}");

  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 

  iCurlReq(szURL, PATCH_ROOM_PATH, METHOD_PATCH, szPayload, strlen(szPayload), szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|PATCH_ROOM\n", strlen("ERR|PATCH_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");

  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);

  return 0;
}
/**
 * Before game starts:
 *  Difficulty Level
 *  Room Capacity
 * After game starts:
 *  query_parameter - A - All , S - Status, I - Id
 *  
 * CMD|CMD_GET_ROOM|query_parameter|{parameter_value|}
 * CMD|006|A
 * CMD|006|S|1
 * CMD|006|I|13
 *
 */
int iCMD_GetRoom(char **ppszArgs, int iSocketClient) {
  int iPRM_Value = 0;
  char cQueryPRM = 0;
  char *pTok;
  char *pszEndpoint;
  char szURL[2048];
  char szFullEndpoint[1024];
  char szRsl[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char *pszTitle = "getroom.txt";

  memset(szURL, 0, sizeof(szURL));
  memset(szFullEndpoint, 0, sizeof(szFullEndpoint));
  memset(szChildResponse, 0, sizeof(szChildResponse));
  memset(szRsl, 0, sizeof(szRsl));
  /** Query parameter */
  pszEndpoint = GET_ALL_ROOM_PATH;
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    cQueryPRM = *pTok;

  if ( cQueryPRM == 'I' || cQueryPRM == 'S' ){
    pszEndpoint = GET_ROOM_BY_ID_PATH;
    if ( cQueryPRM == 'S') pszEndpoint = GET_ROOM_BY_STS_PATH;

    /** Parameter Value */
    pTok = strtok_r(NULL, "|", ppszArgs);
    if ( !bStrIsEmpty(pTok) ) 
      iPRM_Value = atoi(pTok);
    
    if ( !iPRM_Value )
      return -2;

    sprintf(szFullEndpoint, "%s/%d", pszEndpoint, iPRM_Value);
  }
  else if ( cQueryPRM == 'A' ) {
    sprintf(szFullEndpoint, "%s", pszEndpoint);
  }
  else {
    return -1;
  }  

  if ( !bStrIsEmpty(API_HOST_PORT) )
    snprintf(szURL, sizeof(szURL), "%s:%s/%s", pszAPI_URL_ADDRESS, API_HOST_PORT, BASE_PATH);
  else
    snprintf(szURL, sizeof(szURL), "%s/%s", pszAPI_URL_ADDRESS, BASE_PATH); 

  iCurlReq(szURL, szFullEndpoint, "GET", NULL, 0, szRsl);

  vTraceVarArgsFn("Return from backend:[%s]", szRsl);

  
  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse), pszTitle) < 0 ){
    vSendMsg(iSocketClient, "ERR|GET_ROOM\n", strlen("ERR|GET_ROOM\n"));
    vSendBye(iSocketClient);
    return -1;
  }
  strcat(szChildResponse, "\n");
  vSendMessageBytes(iSocketClient, szChildResponse, strlen(szChildResponse), RESULT_OK);

  return 0;
}

int iJSON_ExternalParse(char *pszJSON, char *pszRsl, int iRslSz, char *pszTitle){
  FILE *pfInput;
  char szCmd[1024];

  memset(szCmd, 0, sizeof(szCmd));

  if ( (pfInput = fopen(pszTitle, "w")) == NULL )
    return -1;
  
  fprintf(pfInput, "%s", pszJSON);
  fclose(pfInput);

  sprintf(szCmd, "node %s %s < %s", JSON_PARSER_TITLE, pszTitle, pszTitle);
  vTraceVarArgsFn("Command[%s]", szCmd);
  if ( bRunCmd(szCmd, pszRsl, iRslSz) == FALSE )
    return -1;

  vTraceVarArgsFn("Rsl[%s]", pszRsl);
  return 0;
}