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
int iParseCommand(char *pszBuffer, int *piCmdId) {
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
      iRsl = iCMD_CreateRoom(&pszSavePtr);
      break;
    case CMD_JOIN_ROOM:
      iRsl = iCMD_JoinRoom(&pszSavePtr);
      break;
    case CMD_DELETE_ROOM:
      iRsl = iCMD_DeleteRoom(&pszSavePtr);
      break;
    case CMD_LEAVE_ROOM:
      iRsl = iCMD_LeaveRoom(&pszSavePtr);
      break;
    case CMD_PATCH_ROOM:
      iRsl = iCMD_PatchRoom(&pszSavePtr);
      break;
    case CMD_GET_ROOM:
      iRsl = iCMD_GetRoom(&pszSavePtr);
      break;
    default:
      iRsl = -1;
      break;
  }

  return iRsl;
}

void vProcessCommand(char *pszCmd) {
  int iCmdId;
  int iArgCount = 0;
  int ii;
  char **ppszArgs = NULL;

  if (iParseCommand(pszCmd, &iCmdId) != 0) {
    vTraceVarArgs("Comando inválido: %s", pszCmd);
    return;
  }

  for (ii = 0; ii < iArgCount; ii++)
    free(ppszArgs[ii]);

  free(ppszArgs);
}