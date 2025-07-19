#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dicion_api.h>
#include <parse_api.h>
#include <input.h>
#include <infix_generator.h>
#include <config.h>

int giDifficulty = 0;

int main() {
    char szInput[MAX_WORD_LEN];
    char szDifficulty[8];
    char szInfixBuffer[8];
    // char *pszRsl;
    // PSTRUCT_WORDLIST pstWordList;
    
    while ( TRUE ) {
        memset(szInput, 0, sizeof(szInput));
        memset(szInfixBuffer, 0, sizeof(szInfixBuffer));    
        memset(szDifficulty, 0, sizeof(szDifficulty));

        if (!giDifficulty) {  // Catch difficulty
            do {
                // vClearTerminal();
                printf("[1] Easy\n[2] Medium\n[3] Hard\n");
                
                printf("\nWhich difficulty do you want? ");
                
                char *pszRet = fgets(szDifficulty, sizeof(szDifficulty), stdin);
                if (pszRet) {
                    if (strchr(szDifficulty, '\n') == NULL) {
                        vFlushInput();  // Avoid multiple questions (Line 28)
                    }
                }
            } while (szDifficulty[0] != '1' && szDifficulty[0] != '2' && szDifficulty[0] != '3');
            
            giDifficulty = szDifficulty[0] - '0';
        }
        
        vInfixGeneratorDb(szInfixBuffer, sizeof(szInfixBuffer));
        
        do { 
            // vClearTerminal();
            printf("Words with: (%s)\n\t", szInfixBuffer);    
            char *pszDyn = cCatchInput();
            if (pszDyn) {
                strncpy(szInput, pszDyn, sizeof(szInput) -1 );
                szInput[sizeof(szInput)-1] = '\0';
                free(pszDyn);
            }
        } while ( strstr(szInput, szInfixBuffer) == NULL );

        // pszRsl = pszDICION_BrowseInfix(szInfixBuffer);
        // if (pszRsl == NULL) {
        //     printf("pszRsl == NULL!!!!\n");
        //     return 0;
        // }

        // pstWordList = pstParseApi(pszRsl);
        
        // vParseWordList(pstWordList);
        
        // vOutputRawWordList(pstWordList);

        printf("INPUT: %s\n", szInput);

        printf("INPUT: %s\n", szInput);

        if (bSearchWordDb(szInput)) {
            printf("\nUHUUU ACERTO\n");
        } else {
            printf("\nAHHHH ERRO\n");
        }

        // vFreeList(pstWordList);
    }

    return 0;
}