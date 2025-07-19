#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <input.h>
#include <config.h>
#include <parse_api.h>
#include <word_db.h>
#include <infix_generator.h>
#include <bombtimer.h>

int giDifficulty = 0;
int gbBombTimeout = FALSE;
void vTimerAction() {
  gbBombTimeout = TRUE;
  printf("Timer esgotado\n");
  return;
}

int main() {
  char szInput[MAX_WORD_LEN];
  char szDifficulty[8];
  char szInfixBuffer[8];
  int iPid;
  struct sigaction stSigAct;

  // Processo principal
  while ( TRUE )
  {
    memset(szInput, 0, sizeof(szInput));
    memset(szInfixBuffer, 0, sizeof(szInfixBuffer));
    memset(szDifficulty, 0, sizeof(szDifficulty));

    if (!giDifficulty) {
      do
      {
        // vClearTerminal();
        printf("[1] Easy\n[2] Medium\n[3] Hard\n");

        printf("\nWhich difficulty do you want? ");

        char *pszRet = fgets(szDifficulty, sizeof(szDifficulty), stdin);
        if (pszRet) {
          if (strchr(szDifficulty, '\n') == NULL)
          {
            vFlushInput(); // Avoid multiple questions (Line 28)
          }
        }
      } while (szDifficulty[0] != '1' && szDifficulty[0] != '2' && szDifficulty[0] != '3');

      giDifficulty = szDifficulty[0] - '0';
    }
    
    stSigAct.sa_handler = vTimerAction;
    sigaction(SIGUSR1, &stSigAct, NULL);

    if ( (iPid = fork()) == 0 ) {
      // Processo da bomba
      vHandleBombTimer();
      return 0;
    }

    vInfixGeneratorDb(szInfixBuffer, sizeof(szInfixBuffer));

    do {
      // vClearTerminal();
      printf("Words with: (%s)\n\t", szInfixBuffer);
      char *pszDyn = cCatchInput();
      if (pszDyn) {
        strncpy(szInput, pszDyn, sizeof(szInput) - 1);
        szInput[sizeof(szInput) - 1] = '\0';
        free(pszDyn);
      }
    } while (strstr(szInput, szInfixBuffer) == NULL);

    wait(NULL); // Catch child

    if (bSearchWordDb(szInput))
        printf("\nUHUUU ACERTO\n");
    else
        printf("\nAHHHH ERRO\n");
  }

  return 0;
}