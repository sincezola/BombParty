
#include <stdio.h>

#ifndef SYS_INTERFACE_H
  #define SYS_INTERFACE_H
  
  int iDIR_MkDir(char *szDir);
  int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName,
                       char *szExt);
  int iDIR_IsDir(char *szDir);
  int bOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode);
  int bCloseFile(FILE **fppFile);
  int bFileExist(const char *kpszFileName);
  int bStrIsEmpty(const char *kpszStr);
  int bRunCmd(char *pszCmd, char *pszRsl, int iRslSz);
#endif