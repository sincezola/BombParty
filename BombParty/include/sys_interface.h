#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H

  void vWaitChild();
  void vSetSigUsrHandler(void (*vActCallBack)(int));
  void vSendSig2Process(int iPID, int iSigType);
  int vSpawnTimerProcess(void (*vTimerFunc)(int), int iParentPID);

#endif
