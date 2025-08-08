#include <bomb_timer.h>
#include <config.h>
#include <ctype.h>
#include <errno.h>
#include <infix_generator.h>
#include <input.h>
#include <parse_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys_interface.h>
#include <terminal_utils.h>
#include <time.h>
#include <word_db.h>
#include <singleplayer.h>
#include <bombparty.h>


int iRunSingleplayerMode(){
  int bRestart = FALSE;
  int bFoundWord = FALSE;
  int iBombPid = -1; /** Guarda o processo/thread da bomba */
  char szInfix[8];
  char szInput[MAX_WORD_LEN];
  char szLastWrong[MAX_WORD_LEN] = ""; /** Guarda a última palavra incorreta */
  int bHasError = FALSE;

  while (TRUE) {
    memset(szInput, 0, sizeof(szInput));
    memset(szInfix, 0, sizeof(szInfix));

    if (!giDifficulty) {
      vReadRoomDifficulty(&giDifficulty);
    }

    vSetSigUsrHandler(vTimerAction);
    iBombPid = vSpawnTimerProcess(vHandleBombTimer, 0); /** Cria a bomba */

    vInfixGeneratorDb(szInfix, sizeof(szInfix));
    do {
      char *pszUserInput;
      vClearTerminal();

      /** Linha do desafio */
      vSetCursorPosition(9, 1);
      printf("Encontre uma palavra que tenha: (%s)", szInfix);
      if (bHasError) {
        printf(" ");
        vPrintColored(szLastWrong, 31); /** Vermelho */
      }
      fflush(stdout);

      /** Captura input */
      pszUserInput = cCatchInput();
      if (!strcmp(pszUserInput, TIMEOUT_STR)) {
        gbBombTimeout = FALSE;
        free(pszUserInput);

        vClearTerminal();
        vGotoBombPosition();
        printf("!! A BOMBA EXPLODIU !! Você perdeu esta rodada.\n");
        fflush(stdout);
        vSleepSeconds(2);
        bRestart = TRUE;
        break;
      }
      if (pszUserInput) {
        strncpy(szInput, pszUserInput, sizeof(szInput) - 1);
        szInput[sizeof(szInput) - 1] = '\0';
        free(pszUserInput);
      }

      /** Feedback se a palavra não contém a substring */
      if (strstr(szInput, szInfix) == NULL) {
        strncpy(szLastWrong, szInput, MAX_WORD_LEN - 1);
        szLastWrong[MAX_WORD_LEN - 1] = '\0';
        bHasError = TRUE;
        vGotoFeedbackErrorPosition();
        printf("A palavra digitada não contém a sequência exigida! Tente novamente.\n");
        fflush(stdout);
        vSleepSeconds(2);
      }

      /** Remove espaços antes de validar */
      vTrimSpaces(szInput);

      /** Verifica no banco */
      vGotoFeedbackPosition();
      if (bSearchWordDb(szInput)) {
        printf("Correto!!\n");
        fflush(stdout);
        vSleepSeconds(2);
        bHasError = FALSE;
        bFoundWord = TRUE;
        bRestart = TRUE;
        szLastWrong[0] = '\0';
      } else {
        printf("Incorreto, tente novamente!\n");
        fflush(stdout);
        strncpy(szLastWrong, szInput, MAX_WORD_LEN - 1);
        szLastWrong[MAX_WORD_LEN - 1] = '\0';
        bHasError = TRUE;
        vSleepSeconds(2);
      }
    } while (!bFoundWord);

    /** Mata o processo da bomba quando a rodada termina */
    vKillBombProcess(iBombPid);
    vWaitChild();

    if (bRestart) {
      giDifficulty = 0;
      bFoundWord = FALSE;
      bRestart = FALSE;
      bHasError = FALSE;
      szLastWrong[0] = '\0';
      continue;
    }
  }

  return 0;
}