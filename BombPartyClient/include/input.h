#include <config.h>

#ifndef INPUT_H
  #define INPUT_H
  #define TIMEOUT_STR "XXXXTIMEOUTXXX"
  int bIsOnlySpaces(const char *str);
  char *cCatchInput();
  void vSleepSeconds(int iSeconds);
  void vClearTerminal();
  void vFlushInput();
  int iSetDifficultyFromChar(int iCh);
  void vTrimSpaces(char *pszStr);
#endif
