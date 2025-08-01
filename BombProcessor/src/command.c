#include <string.h>
#include <command.h>
#include <routes_api.h>
#include <curl_api.h>
#include <sys_interface.h>
#include <trace.h>
#include <stdlib.h>
#include <sys/socket.h>
/**
* CMD|CMD_CREATE_ROOM|playername|roomname|roomcapacity|dificultylevel|{password}
* CMD|001|"Name"|4|1|
*/

int iCMD_CreateRoom(char **ppszArgs) {
  char *pTok;
  STRUCT_ROOM stRoom;
  char *pszTitle = "createroom.txt";
  char szRsl[_MAX_RSL_BUFFER];
  char szUrl[80];
  char szMsg[80];

  memset(&stRoom, 0, sizeof(STRUCT_ROOM));
  
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
  char szPayload[1024];
  snprintf(szPayload, sizeof(szPayload),
    "{ \"player_name\": \"%s\", \"room_name\": \"%s\", \"room_capacity\": %d, \"difficulty_level\": %d, \"password\": \"%s\" }",
    stRoom.szPlayerName, stRoom.szRoomName, stRoom.iRoomCapacity, stRoom.iDifficultyLevel, stRoom.szPassword);

  sprintf(szURL, "%s:%s/%s", API_HOST_ADDRESS, API_HOST_PORT, BASE_PATH);
  iCurlReq(szURL, szFullEndpoint, "POST", NULL, 0, szRsl);

  vTraceVarArgs("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse)) < 0 )
    return -1;

  strcat(szChildResponse, "\n");
  sprintf(szMsg, "OK|BYTES|%ld\n", strlen(szChildResponse));
  send(iSocketClient, szMsg, strlen(szMsg), 0);
  send(iSocketClient, szChildResponse, strlen(szChildResponse), 0);
  send(iSocketClient, "BYE\n", strlen("BYE\n"), 0);

  return 0;
}

/**
* CMD|CMD_JOIN_ROOM|playername|room_id|
* CMD|002|
*/
int iCMD_JoinRoom(char **ppszArgs) {
  int  iRoomId = 0;
  char *pTok;
  char szPlayerName[128];

  /** Player Name */
  memset(szPlayerName, 0, sizeof(szPlayerName));
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(szPlayerName, sizeof(szPlayerName), "%s", pTok);
  
  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iRoomId = atoi(pTok);
  
  // iCurlReq();

  return 0;
}

/**
* CMD|CMD_DELETE_ROOM|room_id|
* CMD|003|
*/
int iCMD_DeleteRoom(char **ppszArgs) {
  int iRoomId = 0;
  char *pTok;

  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iRoomId = atoi(pTok);

  // iCurlReq();

  return 0;
}

/**
* CMD|CMD_LEAVE_ROOM|player_name|room_id
* CMD|004|
*/
int iCMD_LeaveRoom(char **ppszArgs) {
  char *pTok;
  char szPlayerName[128];
  int iRoomId = 0;

  /** Player Name */
  memset(szPlayerName, 0, sizeof(szPlayerName));
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(szPlayerName, sizeof(szPlayerName), "%s", pTok);
  
  /** Room Id */
  pTok = strtok_r(NULL, "|", ppszArgs);
  if ( !bStrIsEmpty(pTok) ) 
    iRoomId = atoi(pTok);
  
  // iCurlReq();

  return 0;
}

/**
 * Before game starts:
 *  Difficulty Level
 *  Room Capacity
 * After game starts:
 *  Room Type - S - StartGame,  C - Close
 *
 * CMD|CMD_PATCH_ROOM|room_id|[dificultylevel|-]|[roomcapacity|-]|{Roomtype|}
 * CMD|005|2|[3|-]|[4|-]|{S|}
 */
int iCMD_PatchRoom(char **ppszArgs) {
  STRUCT_ROOM stRoom;
  char *pTok;

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
  
  if ( stRoom.iRoomType ) {
    /** Room status change */
  }
  else{
    /** Room setup change */
  }

  // iCurlReq();

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
  char *pTok;
  char cQueryPRM = 0;
  int iPRM_Value = 0;
  char szURL[1024];
  char szFullEndpoint[1024];
  char *pszEndpoint;
  char szRsl[_MAX_RSL_BUFFER];
  char szChildResponse[_MAX_RSL_BUFFER];
  char szMsg[128];
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

  sprintf(szURL, "%s:%s/%s", API_HOST_ADDRESS, API_HOST_PORT, BASE_PATH);
  iCurlReq(szURL, szFullEndpoint, "GET", NULL, 0, szRsl);

  vTraceVarArgs("Return from backend:[%s]", szRsl);

  if ( iJSON_ExternalParse(szRsl, szChildResponse, sizeof(szChildResponse)) < 0 )
    return -1;

  strcat(szChildResponse, "\n");
  sprintf(szMsg, "OK|BYTES|%ld\n", strlen(szChildResponse));
  send(iSocketClient, szMsg, strlen(szMsg), 0);
  send(iSocketClient, szChildResponse, strlen(szChildResponse), 0);
  send(iSocketClient, "BYE\n", strlen("BYE\n"), 0);

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

  sprintf(szCmd, "node %s < %s", JSON_PARSER_TITLE, pszTitle);
  if ( bRunCmd(szCmd, pszRsl, iRslSz) == FALSE )
    return -1;

  return 0;
}