#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parse_messages.h>
#include <trace.h>

void vHandleCreateRoom(char **ppszArgs, int iArgCount) {
  if (iArgCount < 3) { vTraceMsg("Erro: argumentos insuficientes"); return; }
  vTraceVarArgs("Criar sala: Jogador=%s, Dificuldade=%s, Capacidade=%s",
          ppszArgs[0], ppszArgs[1], ppszArgs[2]);
}

void vHandleJoinRoom(char **ppszArgs, int iArgCount) {
  if (iArgCount < 2) { vTraceMsg("Erro: argumentos insuficientes"); return; }
  vTraceVarArgs("Entrar sala: Jogador=%s, SalaID=%s", ppszArgs[0], ppszArgs[1]);
}

void vHandleDeleteRoom(char **ppszArgs, int iArgCount) {
  if (iArgCount < 1) { vTraceMsg("Erro: argumentos insuficientes"); return; }
  vTraceVarArgs("Deletar sala: SalaID=%s", ppszArgs[0]);
}

void vHandleDisconnect() {
  vTraceMsg("Cliente desconectado.");
}

/**
 * @brief Faz o parse de um comando do cliente no formato CMD|ID|ARG1|ARG2...
 * @param pszBuffer Mensagem recebida
 * @param piCmdId Retorna o ID do comando
 * @param ppszArgs Retorna um array de argumentos (strings)
 * @param piArgCount Retorna o número de argumentos
 * @return int 0 se OK, -1 se inválido
 */
int iParseCommand(char *pszBuffer, int *piCmdId, char ***ppszArgs, int *piArgCount) {
  char *pTok;
  int iCount = 0;
  char *pszSavePtr;
  char **ppszTmpArgs = NULL;

  // Primeiro token: deve ser "CMD"
  pTok = strtok_r(pszBuffer, "|", &pszSavePtr);
  if (!pTok || strcmp(pTok, "CMD") != 0)
    return -1;

  // Segundo token: ID
  pTok = strtok_r(NULL, "|", &pszSavePtr);
  if (!pTok)
    return -1;
  *piCmdId = atoi(pTok);

  // Demais tokens: argumentos
  while ((pTok = strtok_r(NULL, "|", &pszSavePtr)) != NULL) 
  {
    ppszTmpArgs = realloc(ppszTmpArgs, sizeof(char*) * (iCount + 1));
    ppszTmpArgs[iCount] = strdup(pTok);
    iCount++;
  }

  *ppszArgs = ppszTmpArgs;
  *piArgCount = iCount;
  return 0;
}

void vProcessCommand(char *pszCmd) {
  int iCmdId;
  int iArgCount = 0;
  int ii;
  char **ppszArgs = NULL;

  if (iParseCommand(pszCmd, &iCmdId, &ppszArgs, &iArgCount) != 0) {
    vTraceVarArgs("Comando inválido: %s", pszCmd);
    return;
  }

  switch (iCmdId) {
    case CMD_CREATE_ROOM: vHandleCreateRoom(ppszArgs, iArgCount); break;
    case CMD_JOIN_ROOM:   vHandleJoinRoom(ppszArgs, iArgCount); break;
    case CMD_DELETE_ROOM: vHandleDeleteRoom(ppszArgs, iArgCount); break;
    case CMD_DISCONNECT:  vHandleDisconnect(); break;
    default: vTraceVarArgs("Comando desconhecido: %d", iCmdId);
  }

  for (ii = 0; ii < iArgCount; ii++)
    free(ppszArgs[ii]);

  free(ppszArgs);
}