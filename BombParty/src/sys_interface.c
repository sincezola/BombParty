#include <signal.h>
#include <string.h>


void vSendSig2Process(int iPID, int iSigType){
    kill(iPID, iSigType);
}

void vSetSigUsrHandler(struct sigaction *pstSig, void (*vActCallBack)(int)) {
    memset(pstSig, 0, sizeof(struct sigaction));
    pstSig->sa_handler = vActCallBack;
    sigemptyset(&pstSig->sa_mask);
    pstSig->sa_flags = 0;
    sigaction(SIGUSR1, pstSig, NULL);
}