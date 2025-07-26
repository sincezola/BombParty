#include <string.h>
#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  #include <process.h>
  static HANDLE ghBombThread = NULL; /** Handle global para thread da bomba */
#else
  #include <signal.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  static int giBombPid = -1; /** PID global do processo da bomba */
#endif

void vSendSig2Process(int iPID, int iSigType) {
#ifdef _WIN32
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
  SetConsoleCtrlHandler((PHANDLER_ROUTINE)vActCallBack, TRUE);
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
  ghBombThread = (HANDLE)_beginthread((_beginthread_proc_type)vTimerFunc, 0, (void*)(intptr_t)iParentPID);
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
