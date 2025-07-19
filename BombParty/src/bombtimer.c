#include <stdio.h>
#include <input.h>
#include <unistd.h>
#include <bombtimer.h>
#include <signal.h>

int iSetTimeFromDifficulty() {
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
  int iSeconds = iSetTimeFromDifficulty();
  
  while (iSeconds > 0) {
    printf("Tempo: %d\n", iSeconds--);
    fflush(stdout);
    vSleepSeconds(1);
  }
  
  kill(getppid(), SIGUSR1);

  return;
}