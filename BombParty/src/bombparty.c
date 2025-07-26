#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <input.h>
#include <config.h>
#include <errno.h>
#include <ctype.h>
#include <parse_api.h>
#include <word_db.h>
#include <infix_generator.h>
#include <bombtimer.h>
#include <sys_interface.h>

int giDifficulty = 0;
int gbBombTimeout = FALSE;

void vTimerAction(int iSig) {
    (void)iSig;
    gbBombTimeout = TRUE;
    return;
}

int main() {
    int iCh;
    int bRestart = FALSE;
    char szInfix[8];
    char szInput[MAX_WORD_LEN];
    char szDifficulty[DIFICULTY_LEN];

    /** Difficulty choice */
    while ( TRUE ) {
        memset(szInput, 0, sizeof(szInput));
        memset(szInfix, 0, sizeof(szInfix));
        memset(szDifficulty, 0, sizeof(szDifficulty));

        if (!giDifficulty) {
            do {
                printf("\nEscolha sua a dificuldade :\n");
                printf("\t[E] Easy   (%d letras por palavra)\n", EASY_INFIX);
                printf("\t[M] Medium (%d letras por palavra)\n", MEDIUM_INFIX);
                printf("\t[H] Hard   (%d letras por palavra)\n", HARD_INFIX);
                printf("  Resposta: ");

                if ( fgets(szDifficulty, sizeof(szDifficulty), stdin) ) {
                    /** 
                     * Flushes anything greater than sizeof(szDifficulty)
                     * to avoid overlapping readout
                     */
                    if ( strchr(szDifficulty, '\n') == NULL ) vFlushInput(); 
                }
                
                iCh = tolower(szDifficulty[0]);
            } while (iCh != 'e' && iCh != 'm' && iCh != 'h');
            
            giDifficulty = iSetDifficultyFromChar(iCh);
            printf("\n\n");
        }

        /** Configura o handler cross-platform */
        vSetSigUsrHandler(vTimerAction);

        /** Cria processo/thread temporizador */
        vSpawnTimerProcess(vHandleBombTimer, 0);

        vInfixGeneratorDb(szInfix, sizeof(szInfix));
        do {
            char *pszUserInput;
            vClearTerminal();

            printf("\n\n");
            printf("\033[15;1H Encontre uma palavra que tenha: (%s)\n", szInfix);
            pszUserInput = cCatchInput();
            if ( !strcmp(pszUserInput, TIMEOUT_STR) ){
                char szInput[_MAX_PATH];
                
                gbBombTimeout = FALSE;
                free(pszUserInput);

                printf("\n\tA BOMBA EXPLODIU!!!\n");
                printf("Pressione enter para continuar...\n");

                if (fgets(szInput, sizeof(szInput), stdin) == NULL) break;
                
                bRestart = TRUE;
                break;
            }
            if (pszUserInput) {
                strncpy(szInput, pszUserInput, sizeof(szInput) - 1);
                szInput[sizeof(szInput) - 1] = '\0';
                free(pszUserInput);
            }
        } while (strstr(szInput, szInfix) == NULL);

        /** Aguarda processo filho/thread terminar */
        vWaitChild();

        if ( bRestart ){
            giDifficulty = 0;
            bRestart = FALSE;
            continue;
        }

        if (bSearchWordDb(szInput))
            printf("\nUHUUU ACERTO\n");
        else
            printf("\nAHHHH ERRO\n");
    }

    return 0;
}
