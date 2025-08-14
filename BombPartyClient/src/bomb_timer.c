#include <stdio.h>
#include <input.h>
#include <unistd.h>
#include <bomb_timer.h>
#include <signal.h>
#include <fcntl.h>
#include <sys_interface.h>
#include <terminal_utils.h>
#ifdef _WIN32
  #include <windows.h>
#endif
int gbBombTimeout = FALSE;

void vDrawBomb(int iTimeout) {
  vGotoBombPosition();
  vSetCursorPosition(2, 10);  vPrintLine("           ░  ", INSERT_NEW_LINE);
  vSetCursorPosition(3, 10);  vPrintLine("       ░░░░  " , INSERT_NEW_LINE);
  vSetCursorPosition(4, 10);  vPrintLine("      ░       ", INSERT_NEW_LINE);
  vSetCursorPosition(5, 10);  vPrintLine(" ░░░░░░░░░░  " , INSERT_NEW_LINE);
  // vSetCursorPosition(6, 10);  vPrintLine(" ░░░░%02d░░░░" , INSERT_NEW_LINE iTimeout);
  vSetCursorPosition(7, 10);  vPrintLine(" ░░░░░░░░░░  " , INSERT_NEW_LINE);
  
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
  #ifndef _WIN32
    vSendSig2Process(iParentPID, SIGUSR1);
  #else
    vSendSig2Process(iParentPID, 0);
  #endif
  return;
}