#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H
  void vSetSigUsrHandler(struct sigaction *pstSig, void (*vActCallBack)(int));
  void vSendSig2Process(int iPID, int iSigType);
#endif