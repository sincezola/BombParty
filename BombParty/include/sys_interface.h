#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H
  void vSetSigUsrHandler(struct sigaction *pstSig, void (*vActCallBack)(int));
#endif