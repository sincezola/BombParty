#include <stdio.h>
#include <input.h>
#include <unistd.h>
#include <bombtimer.h>
#include <signal.h>
#include <fcntl.h>
#include <sys_interface.h>
#include <terminal_utils.h>

void vDrawBomb(int iTimeout) {
  vGotoBombPosition();
  printf( "           ░░  \n");
  printf( "      ░░░░░░░  \n");
  printf( "      ░░       \n");
  printf( " ░░░░░░░░░░░░  \n");
  printf( " ░░░░░░░░░░░░  \n");
  printf( " ░░░░░%02d░░░░░\n", iTimeout);
  printf( " ░░░░░░░░░░░░  \n");
  printf( " ░░░░░░░░░░░░  \n");
  vGotoInputTextPosition();  /** <- reposiciona cursor para o input */
  fflush(stdout);
}


int iGetDifficultyTimeout() {
  switch ( giDifficulty ) {
    case HARD:
      return HARD_TIMEOUT;
    case MEDIUM:
      return MEDIUM_TIMEOUT;
    case EASY:
    default  :
      return EASY_TIMEOUT;
  }
  return EASY_TIMEOUT;
}

void vHandleBombTimer(int iParentPID) {
  int iTimeout = iGetDifficultyTimeout();
  
  while (iTimeout > 0) {
    vDrawBomb(iTimeout--);
    vSleepSeconds(1);
  }
  
  vSendSig2Process(iParentPID, SIGUSR1);

  return;
}