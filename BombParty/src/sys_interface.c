#include <string.h>
#include <stdio.h>

#ifdef _WIN32
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP ou superior */
  #endif
  #include <windows.h>
  #include <stdint.h>  /** Para intptr_t */
  static HANDLE ghBombThread = NULL; /** Handle global para thread da bomba */

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

#else
  #include <signal.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  static int giBombPid = -1; /** PID global do processo da bomba */
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
    if (iParentPID == 0) iParentPID = getppid();
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
