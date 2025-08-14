#include <stdio.h>
#include <config.h>
#include <string.h>
#include <sys_interface.h>

#ifdef _WIN32
  #include <stdint.h> /** Para intptr_t */
  #include <windows.h>
  static HANDLE ghBombThread = NULL; /** Handle global para thread da bomba */
#else
  #include <libgen.h>
  #include <signal.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/stat.h>
  #include <unistd.h>
  static int giBombPid = -1; /** PID global do processo da bomba */
#endif

#ifdef _WIN32
  /** Wrapper para CreateThread */
  DWORD WINAPI ThreadWrapper(LPVOID pArg) {
    void (**args)(int) = (void (**)(int))pArg;
    void (*func)(int) = args[0];
    int param = (int)(intptr_t)args[1];
    func(param);
    free(pArg);
    return 0;
  }

  /** Wrapper para SetConsoleCtrlHandler */
  static BOOL WINAPI CtrlHandler(DWORD dwCtrlType) {
    (void)dwCtrlType;
    return TRUE; /** Apenas consome o evento */
  }
#endif

void vSendSig2Process(int iPID, int iSigType) {
#ifdef _WIN32
  (void)iSigType; /** Não usado no Windows */
  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, iPID);
  if (hProcess) {
    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
  }
#else
  kill(iPID, iSigType);
#endif
}

void vSetSigUsrHandler(void (*vActCallBack)(int)) {
#ifdef _WIN32
  (void)vActCallBack; /** Não usado, mas podemos armazenar se necessário */
  SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
  struct sigaction stSig;
  memset(&stSig, 0, sizeof(struct sigaction));
  stSig.sa_handler = vActCallBack;
  sigemptyset(&stSig.sa_mask);
  stSig.sa_flags = 0;
  sigaction(SIGUSR1, &stSig, NULL);
#endif
}

int vSpawnTimerProcess(void (*vTimerFunc)(int), int iParentPID) {
#ifdef _WIN32
  void **pArgs = malloc(2 * sizeof(void *));
  pArgs[0] = (void *)vTimerFunc;
  pArgs[1] = (void *)(intptr_t)iParentPID;
  ghBombThread = CreateThread(NULL, 0, ThreadWrapper, pArgs, 0, NULL);
  return 1; /** ID fake */
#else
  int pid = fork();
  if (pid == 0) {
    if (iParentPID == 0)
      iParentPID = getppid();
    vTimerFunc(iParentPID);
    _exit(0);
  }
  giBombPid = pid;
  return pid;
#endif
}

/**
 * @brief Encerra processo/thread da bomba
 */
void vKillBombProcess(int iPid) {
#ifdef _WIN32
  (void)iPid; /** Ignorado no Windows */
  if (ghBombThread) {
    TerminateThread(ghBombThread, 0);
    CloseHandle(ghBombThread);
    ghBombThread = NULL;
  }
#else
  if (iPid > 0) {
    kill(iPid, SIGKILL);
    giBombPid = -1;
  }
#endif
}

void vWaitChild() {
#ifdef _WIN32
  Sleep(100);
#else
  wait(NULL);
#endif
}
#ifdef _WIN32
  // Return 0 if error
  int iDIR_MkDir(char *szDir) { 
    return CreateDirectory(szDir, NULL); 
  }
  int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName, char *szExt) {
    char szDrive[_MAX_DRIVE];
    char szDir[_MAX_DIR];
    _splitpath(szFilename, szDrive, szDir, szName, szExt);
    strcpy(szPath, szDrive);
    strcat(szPath, szDir);
    return 0;
  }
  int iDIR_IsDir(char *szDir) {
    HANDLE hArquivo;
    WIN32_FIND_DATA wfdArquivo;
    hArquivo = FindFirstFile(szDir, &wfdArquivo);
    if (hArquivo == INVALID_HANDLE_VALUE)
      return 0;
    FindClose(hArquivo);
    if (wfdArquivo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return 1;
    return 0;
  }
#else /** LINUX */
/*
 * Is directory: 1
 * Exists, not directory: 0
 * Does not exist: -1
 * **/
int iDIR_IsDir(char *szDir) {
  struct stat stStat;
  if (stat(szDir, &stStat) != 0)
    return 0;
  if (S_ISDIR(stStat.st_mode))
    return 1;
  return 0;
}
int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName,
                       char *szExt) {
  char szWrk[_MAX_PATH];
  char szBase[_MAX_PATH];
  char *pszBase;
  int ii;
  strcpy(szWrk, szFilename);
  pszBase = dirname(szWrk);
  if (pszBase == NULL) {
    szPath[0] = 0;
  } else {
    strcpy(szPath, pszBase);
  }
  strcpy(szWrk, szFilename);
  pszBase = basename(szWrk);
  strcpy(szBase, pszBase);
  for (ii = strlen(szBase) - 1; ii && szBase[ii] != '.'; ii--) {
    if (szBase[ii] == '\n' || szBase[ii] == '\r')
      szBase[ii] = 0;
  }
  if (ii > 0) { // found dot
    strcpy(szExt, &szBase[ii]);
    szBase[ii] = 0;
    strcpy(szName, szBase);
  } else {
    strcpy(szName, szBase);
    *szExt = 0;
  }

  return 0;
}
// Return 0 if error
int iDIR_MkDir(char *szDir) // linux
{
  int iRsl;
  // if (DEBUG_LEVEL)
  // vTracePid("iDIR_MkDir(%s)", strlen(szDir));
  iRsl = mkdir(szDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |
                          S_IXGRP | S_IROTH);
  return (iRsl == 0);
}
#endif

bool bOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode) {
  if ((*fppFile = fopen(kpszFileName, kpszMode)) == NULL) {
    return FALSE;
  }
  return TRUE;
} /* bOpenFile */

bool bCloseFile(FILE **fppFile) {
  if (*fppFile != NULL) {
    fclose(*fppFile);
    *fppFile = NULL;

    return TRUE;
  }
  return FALSE;
} /* bCloseFile */

bool bFileExist(const char *kpszFileName) {
  FILE *fpFile = NULL;

  if (!bOpenFile(&fpFile, kpszFileName, "r")) {
    return FALSE;
  }

  bCloseFile(&fpFile);

  return TRUE;
} /* bFileExist */

int bRunCmd(char *pszCmd, char *pszRsl, int iRslSz) {
  FILE *pfPopen;
  char szLine[1024];
  int iCurrLen = 0;

  if ((pfPopen = popen(pszCmd, "r")) == NULL)
    return FALSE;

  while (fgets(szLine, sizeof(szLine), pfPopen)) {
    if (((int)strlen(szLine) + iCurrLen) > iRslSz) {
      pclose(pfPopen);
      return FALSE;
    }
    sprintf(&pszRsl[iCurrLen], "%s", szLine);
    iCurrLen += strlen(szLine);
  }

  pclose(pfPopen);
  if (bStrIsEmpty(pszRsl))
    return FALSE;

  return TRUE;
}

bool bStrIsEmpty(const char *kpszStr) {
  if (kpszStr == NULL || !strcmp(kpszStr, "") || !strcmp(kpszStr, "\n")) {
    return TRUE;
  }

  return FALSE;
} /* bStrIsEmpty */
