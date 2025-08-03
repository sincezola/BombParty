#include <stdio.h>
#include <input.h>
#include <unistd.h>
#include <bomb_timer.h>
#include <signal.h>
#include <fcntl.h>
#include <sys_interface.h>
#include <terminal_utils.h>

int gbBombTimeout = FALSE;

void vDrawBomb(int iTimeout) {
  vGotoBombPosition();
  vSetCursorPosition(2, 10);  printf("           ░  \n");
  vSetCursorPosition(3, 10);  printf("       ░░░░  \n");
  vSetCursorPosition(4, 10);  printf("      ░       \n");
  vSetCursorPosition(5, 10);  printf(" ░░░░░░░░░░  \n");
  vSetCursorPosition(6, 10);  printf(" ░░░░%02d░░░░\n", iTimeout);
  vSetCursorPosition(7, 10);  printf(" ░░░░░░░░░░  \n");
  
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

/** Callback do timer (usado no processo/thread da bomba) */
void vTimerAction(int iSig) {
  (void)iSig;
  gbBombTimeout = TRUE;
  return;
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