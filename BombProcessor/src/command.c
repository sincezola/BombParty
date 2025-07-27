#include <string.h>
#include <command.h>

/**
* CMD|CMD_CREATE_ROOM|playername|roomname|roomcapacity|dificultylevel|{password}
* CMD|001|"Name"|4|1|
*/

int iCMD_CreateRoom(char **ppszArgs) {
  char *pTok;
  STRUCT_ROOM stRoom;

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
  if ( !bStrIsEmpty(pTok) ) 
    snprintf(stRoom.szPassword, sizeof(stRoom.szPassword), "%s", pTok);
  
  // iCurlReq();

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