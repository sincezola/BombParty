#include <config.h>

#ifndef SYSINTERFACE_H
  #define SYSINTERFACE_H
/****************************************************************************** 
 *                                                                            *
 *               Compatibilidade Windows (MSVC vs MinGW/GCC)                  *
 *                                                                            *
 ******************************************************************************/

  #ifdef _WIN32
    #include <windows.h>

    /* Definições ausentes no MinGW */
    #ifndef _MAX_DRIVE
      #define _MAX_DRIVE 3
    #endif
    #ifndef _MAX_DIR
      #define _MAX_DIR 256
    #endif
    #ifndef _MAX_FNAME
      #define _MAX_FNAME 256
    #endif
    #ifndef _MAX_EXT
      #define _MAX_EXT 256
    #endif

    /* Emulação de _splitpath caso não exista */
    #ifndef HAVE__SPLITPATH
      static inline void _splitpath(const char *pszPath,
                                    char *pszDrive,
                                    char *pszDir,
                                    char *pszFname,
                                    char *pszExt) {
        const char *pszSlash, *pszDot;

        if (pszDrive) pszDrive[0] = '\0'; /* Ignora drive no MinGW */

        /* aceita tanto '\' quanto '/' */
        pszSlash = strrchr(pszPath, '\\');
        if (!pszSlash) pszSlash = strrchr(pszPath, '/');

        if (pszSlash) {
          if (pszDir) {
            size_t iLen = pszSlash - pszPath + 1;
            strncpy(pszDir, pszPath, iLen);
            pszDir[iLen] = '\0';
          }
          pszPath = pszSlash + 1;
        } else {
          if (pszDir) strcpy(pszDir, "");
        }

        pszDot = strrchr(pszPath, '.');
        if (pszDot) {
          if (pszExt) strcpy(pszExt, pszDot);
          if (pszFname) {
            size_t iLen = pszDot - pszPath;
            strncpy(pszFname, pszPath, iLen);
            pszFname[iLen] = '\0';
          }
        } else {
          if (pszExt) strcpy(pszExt, "");
          if (pszFname) strcpy(pszFname, pszPath);
        }
      }
      #define HAVE__SPLITPATH
    #endif /* HAVE__SPLITPATH */
  #endif /* _WIN32 */

/******************************************************************************
 *                                                                            *
 *                          Protótipos de funções                             *
 *                                                                            *
 ******************************************************************************/

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

#endif /* SYSINTERFACE_H */
