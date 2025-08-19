#include <config.h>

#ifndef INPUT_H
  #define INPUT_H
  #define TIMEOUT_STR "XXXXTIMEOUTXXX"

  #define ACTION_CREATE       'c'
  #define ACTION_JOIN         'e'
  #define ACTION_REFRESH_LIST 'a'
  #define ACTION_DESTROY_ROOM 'd'

  char *cCatchInput();
  bool bIsOnlySpaces(const char *str);
  int iSetDifficultyFromChar(int iCh);
  int iReadGameMode();
  int iReadMultiplayerAction();
  int iPortableGetchar();
  void vSleepSeconds(int iSeconds);
  void vClearTerminal();
  void vFlushInput();
  void vTrimSpaces(char *pszStr);
  void vReadPlayerName(char *pszName, int iNameSz);
  void vReadRoomName(char *pszName, int iNameSz);
  void vReadRoomDifficulty(int *iDifficulty);
  void vReadRoomPassword(char *pszPassword, int iPasswdSz);
  void vReadRoomCapacity(int *iCapacity);
  void vReadRoomPlayerPassword(char *pszBuff, int iBuffLen);
#endif
