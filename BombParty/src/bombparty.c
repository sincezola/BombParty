#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <input.h>
#include <config.h>
#include <errno.h>
#include <ctype.h>
#include <parse_api.h>
#include <word_db.h>
#include <infix_generator.h>
#include <bombtimer.h>

int giDifficulty = 0;
int gbBombTimeout = FALSE;
void vTimerAction(int iSig) {
  int ii = iSig;
  ii++;
  gbBombTimeout = TRUE;
  return;
}

int main() {
  int iCh;
  char szInput[MAX_WORD_LEN];
  char szDifficulty[8];
  char szInfixBuffer[8];
  int iPid;
  struct sigaction stSigAct;
  int bRestart = FALSE;

  // Processo principal
  while ( TRUE )
  {
    memset(szInput, 0, sizeof(szInput));
    memset(szInfixBuffer, 0, sizeof(szInfixBuffer));
    memset(szDifficulty, 0, sizeof(szDifficulty));

    if (!giDifficulty) {
      do
      {
        vClearTerminal();
        
        printf("\nEscolha sua a dificuldade :\n");
        printf("\t[E] Easy   (%d letras por palavra)\n"  , EASY_INFIX);
        printf("\t[M] Medium (%d letras por palavra)\n"  , MEDIUM_INFIX);
        printf("\t[H] Hard   (%d letras por palavra)\n", HARD_INFIX);
        printf("  Resposta: ");
        char *pszRet = fgets(szDifficulty, sizeof(szDifficulty), stdin);
        if (pszRet) {
          if (strchr(szDifficulty, '\n') == NULL)
          {
            vFlushInput(); // Avoid multiple questions (Line 28)
          }
        }
        iCh = tolower(szDifficulty[0]);
      } while (iCh != 'e' && iCh != 'm' && iCh != 'h');
      printf("\n\n");
      giDifficulty = iSetDifficultyFromChar(iCh);
    }
    memset(&stSigAct, 0, sizeof(stSigAct));
    stSigAct.sa_handler = vTimerAction;    
    sigemptyset(&stSigAct.sa_mask);
    stSigAct.sa_flags = 0;
    sigaction(SIGUSR1, &stSigAct, NULL);

    if ( (iPid = fork()) == 0 ) {
      // Processo da bomba
      vHandleBombTimer();
      return 0;
    }

    vInfixGeneratorDb(szInfixBuffer, sizeof(szInfixBuffer));

    do {
      vClearTerminal();
      printf("Encontre uma palavra que tenha: (%s)\n", szInfixBuffer);
      char *pszDyn = cCatchInput();
      if ( !strcmp(pszDyn, TIMEOUT_STR) ){
        char szInput[_MAX_PATH];
        free(pszDyn);

        printf("\n\t PERDEU!!!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        fgets(szInput, sizeof(szInput), stdin);
        
        bRestart = TRUE;
        break;
      }
      if (pszDyn) {
        strncpy(szInput, pszDyn, sizeof(szInput) - 1);
        szInput[sizeof(szInput) - 1] = '\0';
        free(pszDyn);
      }
    } while (strstr(szInput, szInfixBuffer) == NULL);

    wait(NULL); // Catch child

    if ( bRestart ){
      giDifficulty = 0;
      bRestart = FALSE;
      continue;
    }

    if (bSearchWordDb(szInput))
        printf("\nUHUUU ACERTO\n");
    else
        printf("\nAHHHH ERRO\n");
  }

  return 0;
}