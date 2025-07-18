#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dicion_api.h>
#include <parse_api.h>
#include <input.h>
#include <infix_generator.h>
#include <config.h>

int g_iDifficulty = 0;

int main() {
    char szInput[MAX_WORD_LEN];
    char szDifficulty[8];
    char *pszRsl;
    char szInfixBuffer[8];
    PSTRUCT_WORDLIST pstWordList;
    
    while ( TRUE ) {
        memset(szInput, 0, sizeof(szInput));
        memset(szInfixBuffer, 0, sizeof(szInfixBuffer));    
        memset(szDifficulty, 0, sizeof(szDifficulty));

        if (!g_iDifficulty) {  // Catch difficulty
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
            
            g_iDifficulty = szDifficulty[0] - '0';
        }
        
        vInfixGeneratorBase(szInfixBuffer);
        
        do { 
            // vClearTerminal();
            printf("Words with: (%s)\n\t", szInfixBuffer);    
            char *pszDyn = cCatchInput();
            if (pszDyn) {
                strncpy(szInput, pszDyn, sizeof(szInput)-1);
                szInput[sizeof(szInput)-1] = '\0';
                free(pszDyn);
            }
        } while ( strstr(szInput, szInfixBuffer) == NULL );

        pszRsl = pszDICION_BrowseInfix(szInput);
        if (pszRsl == NULL) 
            return 0;
            
        pstWordList = pstParseApi(pszRsl);
        vParseWordList(pstWordList);
        free(pszRsl);
        vFreeList(pstWordList);
    }

    return 0;
}