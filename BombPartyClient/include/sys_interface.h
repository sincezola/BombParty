#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H

  void vWaitChild();
  void vSetSigUsrHandler(void (*vActCallBack)(int));
  void vSendSig2Process(int iPID, int iSigType);
  int vSpawnTimerProcess(void (*vTimerFunc)(int), int iParentPID);
  void vKillBombProcess(int iPid); /** Encerra processo/thread da bomba */
  bool bStrIsEmpty(const char *kpszStr);
  int iDIR_MkDir(char *szDir);
  int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName,
                       char *szExt);
  int iDIR_IsDir(char *szDir);
  bool bOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode);
  bool bCloseFile(FILE **fppFile);
  bool bFileExist(const char *kpszFileName);
#endif
