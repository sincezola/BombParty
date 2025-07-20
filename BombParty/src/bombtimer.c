#include <stdio.h>
#include <input.h>
#include <unistd.h>
#include <bombtimer.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

void vDrawBomb(int iTimeout) {
  int iRow;
  int iCol;

  printf( "\033[3;10H           ░░  \n");
  printf( "\033[4;10H      ░░░░░░░  \n");
  printf( "\033[5;10H      ░░       \n");
  printf( "\033[6;10H ░░░░░░░░░░░░  \n");
  printf( "\033[7;10H ░░░░░░░░░░░░  \n");
  printf( "\033[8;10H ░░░░░%02d░░░░░\n", iTimeout);
  printf( "\033[9;10H ░░░░░░░░░░░░  \n");
  printf("\033[10;10H ░░░░░░░░░░░░  \n");
  
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

void vHandleBombTimer() {
  int iTimeout = iGetDifficultyTimeout();
  
  while (iTimeout > 0) {
    vDrawBomb(iTimeout--);
    vSleepSeconds(1);
  }
  
  kill(getppid(), SIGUSR1);

  return;
}