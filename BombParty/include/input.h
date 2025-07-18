#include <config.h>

#ifndef INPUT_H
  #define INPUT_H

  bool bIsOnlySpaces(const char *str);
  char *cCatchInput();

  void vClearTerminal();
  void vFlushInput();
#endif
