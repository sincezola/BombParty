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
 * @return int 0 se OK, -1 se inválido, 1 fechar conexao
 */
int iParseCommand(char *pszBuffer, int *piCmdId, int iSock) {
  int iRsl = 0;
  char *pTok;
  char *pszSavePtr;

  pTok = strtok_r(pszBuffer, "|", &pszSavePtr);
  if (!pTok || memcmp(pTok, "BYE", 3) == 0)
    return 1;

  // Primeiro token: se não é bye deve ser "CMD"
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
