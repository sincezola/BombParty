#include <config.h>

#ifndef INPUT_H
  #define INPUT_H

  bool bIsOnlySpaces(const char *str);
  char *cCatchInput();
  void vSleepSeconds(int iSeconds);
  void vClearTerminal();
  void vFlushInput();
#endif
