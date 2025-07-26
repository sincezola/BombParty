#include <string.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#else
    #include <signal.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

void vSendSig2Process(int iPID, int iSigType){
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
    /** Creates Windows Thread */ 
    _beginthread((_beginthread_proc_type)vTimerFunc, 0, (void*)(intptr_t)iParentPID);
    return 1; /** returns fake PID */
#else
    int pid = fork();
    if (pid == 0) {
        if (iParentPID == 0) iParentPID = getppid();
        vTimerFunc(iParentPID);
        _exit(0);
    }
    return pid;
#endif
}


void vWaitChild() {
#ifdef _WIN32
    Sleep(100); /** Single wait for threads */
#else
    wait(NULL);
#endif
}
