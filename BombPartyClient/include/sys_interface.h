#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H

  void vWaitChild();
  void vSetSigUsrHandler(void (*vActCallBack)(int));
  void vSendSig2Process(int iPID, int iSigType);
  int vSpawnTimerProcess(void (*vTimerFunc)(int), int iParentPID);
  void vKillBombProcess(int iPid); /** Encerra processo/thread da bomba */
  int bStrIsEmpty(const char *kpszStr);
#endif
