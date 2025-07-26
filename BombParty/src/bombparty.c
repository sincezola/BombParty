#include <bombtimer.h>
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

int giDifficulty = 0;
int gbBombTimeout = FALSE;

void vTimerAction(int iSig) {
  (void)iSig;
  gbBombTimeout = TRUE;
  return;
}

int main() {
  int iCh;
  int bRestart = FALSE;
  int iBombPid = -1; /** Guarda o processo/thread da bomba */
  char szInfix[8];
  char szInput[MAX_WORD_LEN];
  char szDifficulty[DIFICULTY_LEN];

  while (TRUE) {
    memset(szInput, 0, sizeof(szInput));
    memset(szInfix, 0, sizeof(szInfix));
    memset(szDifficulty, 0, sizeof(szDifficulty));

    if (!giDifficulty) {
      do {
        vClearTerminal();
        printf("\n Escolha sua a dificuldade :\n");
        printf("\t[E] Easy   (%d letras por palavra)\n", EASY_INFIX);
        printf("\t[M] Medium (%d letras por palavra)\n", MEDIUM_INFIX);
        printf("\t[H] Hard   (%d letras por palavra)\n", HARD_INFIX);
        printf("Dificuldade: ");

        if (fgets(szDifficulty, sizeof(szDifficulty), stdin)) {
          if (strchr(szDifficulty, '\n') == NULL)
            vFlushInput();
        }

        iCh = tolower(szDifficulty[0]);
      } while (iCh != 'e' && iCh != 'm' && iCh != 'h');

      giDifficulty = iSetDifficultyFromChar(iCh);
      printf("\n\n");
    }

    vSetSigUsrHandler(vTimerAction);
    iBombPid = vSpawnTimerProcess(vHandleBombTimer, 0); /** Cria a bomba */

    vInfixGeneratorDb(szInfix, sizeof(szInfix));
    do {
      char *pszUserInput;
      vClearTerminal();

      printf("\033[9;1H Encontre uma palavra que tenha: (%s)", szInfix);
      pszUserInput = cCatchInput();
      if (!strcmp(pszUserInput, TIMEOUT_STR)) {
        char szInput[_MAX_PATH];
        gbBombTimeout = FALSE;
        free(pszUserInput);

        vClearTerminal();
        printf("💥 A BOMBA EXPLODIU! Você perdeu esta rodada.\n");
        printf("Pressione enter para continuar...\n");
        fflush(stdout);

        if (fgets(szInput, sizeof(szInput), stdin) == NULL)
          break;

        vSleepSeconds(1); /** Espera para o jogador ver a mensagem */
        bRestart = TRUE;
        break;
      }
      if (pszUserInput) {
        strncpy(szInput, pszUserInput, sizeof(szInput) - 1);
        szInput[sizeof(szInput) - 1] = '\0';
        free(pszUserInput);
      }

      /** Novo: feedback se a palavra não contém a substring */
      if (strstr(szInput, szInfix) == NULL) {
        vGotoFeedbackErrorPosition();
        printf("A palavra digitada não contém a sequência exigida! Tente "
               "novamente.\n");
        fflush(stdout);
        vSleepSeconds(2);
      }

    } while (strstr(szInput, szInfix) == NULL);

    /** Mata o processo da bomba quando a rodada termina */
    vKillBombProcess(iBombPid);

    vWaitChild();

    if (bRestart) {
      giDifficulty = 0;
      bRestart = FALSE;
      continue;
    }

    vTrimSpaces(szInput);
    vGotoFeedbackErrorPosition();
    if (bSearchWordDb(szInput)) {
      printf("Correto!!\n");
      fflush(stdout);
      vSleepSeconds(2);
    } else {
      printf("Incorreto, tente novamente!\n");
      fflush(stdout);
    }
  }

  return 0;
}
