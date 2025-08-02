#include <string.h>
#include <stdlib.h>
#include <parse_messages.h>
#include <trace.h>
#include <curl_api.h>
#include <routes_api.h>
#include <command.h>
#include <sys_interface.h>

/**
 * @brief Faz o parse de um comando do cliente no formato CMD|ID|ARG1|ARG2...
 * @param pszBuffer Mensagem recebida
 * @param piCmdId Retorna o ID do comando
 * @param ppszArgs Retorna um array de argumentos (strings)
 * @param piArgCount Retorna o número de argumentos
 * @return int 0 se OK, -1 se inválido
 */
int iParseCommand(char *pszBuffer, int *piCmdId, int iSock) {
  int iRsl = 0;
  char *pTok;
  char *pszSavePtr;

  // Primeiro token: deve ser "CMD"
  pTok = strtok_r(pszBuffer, "|", &pszSavePtr);
  if (!pTok || strcmp(pTok, "CMD") != 0)
    return -1;

  // Segundo token: ID
  pTok = strtok_r(NULL, "|", &pszSavePtr);
  if (!pTok)
    return -1;

  *piCmdId = atoi(pTok);
  switch( *piCmdId ) {
    case CMD_CREATE_ROOM:
      iRsl = iCMD_CreateRoom(&pszSavePtr, iSock);
      break;
    case CMD_JOIN_ROOM:
      iRsl = iCMD_JoinRoom(&pszSavePtr, iSock);
      break;
    case CMD_DELETE_ROOM:
      iRsl = iCMD_DeleteRoom(&pszSavePtr, iSock);
      break;
    case CMD_LEAVE_ROOM:
      iRsl = iCMD_LeaveRoom(&pszSavePtr, iSock);
      break;
    case CMD_PATCH_ROOM:
      iRsl = iCMD_PatchRoom(&pszSavePtr, iSock);
      break;
    case CMD_GET_ROOM:
      iRsl = iCMD_GetRoom(&pszSavePtr, iSock);
      break;
    default:
      iRsl = -1;
      break;
  }

  return iRsl;
}

void vProcessCommand(char *pszCmd, int iSock) {
  int iCmdId;

  if (iParseCommand(pszCmd, &iCmdId, iSock) != 0) {
    vTraceVarArgs("Comando inválido: %s", pszCmd);
    return;
  }
  return;
}