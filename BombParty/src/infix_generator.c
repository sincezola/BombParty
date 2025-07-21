#include <time.h>
#include <infix_generator.h>
#include <parse_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#include <word_db.h>


int iRandomBetween(int min, int max, int iSeed) {
  srand(time(NULL) * iSeed);

  return rand() % (max - min + 1) + min;
}

void vInfixGeneratorDb(char *pszInfixBuffer, int iInfixBufferSize) {
  int iRand;
  char szWord2Infix[256];
  int iInfixMaxSize;
  
  srand(time(NULL));
  iRand = rand() % 200000;

  memset(szWord2Infix,0,sizeof(szWord2Infix));
  vGetWordFromDb(szWord2Infix, sizeof(szWord2Infix), iRand);

  iInfixMaxSize = strlen(szWord2Infix) - HARD_INFIX; // Hard
  if ( giDifficulty == EASY )
    iInfixMaxSize = strlen(szWord2Infix) - EASY_INFIX;
  else if ( giDifficulty == MEDIUM )
    iInfixMaxSize = strlen(szWord2Infix) - MEDIUM_INFIX;

  if ( iInfixMaxSize <= 0 ) {
    sprintf(pszInfixBuffer, "%.*s", (int)iInfixBufferSize, szWord2Infix);
    return;
  }

  iRand = iRandomBetween(1, strlen(szWord2Infix) - iInfixMaxSize, 5);
  if ( strlen(szWord2Infix) > MEDIUM_INFIX ) {
    sprintf(pszInfixBuffer, "%.*s", (int)(strlen(szWord2Infix) - iInfixMaxSize), &szWord2Infix[iRand]);
  }
}

/** 
 * Entre if 0 e endif era usado para Api dicionario aberto. 
 * descontinuado... 
 * */
#if 0 
void vInfixGeneratorRandom(char *pszInfixBuffer) {
  if ( pszInfixBuffer != NULL)
    return ;
  
  return;
}

void vInfixGeneratorBase(char *pszInfixBuffer) {
  if (!giDifficulty) return;

  int iRand;
  int ii;
  char *pTok = NULL;
  
  srand(time(NULL));
  iRand = rand() % 60;
  for (ii = 0; ii < iRand; ii++ ) {
    pTok = pszGetNextWord();
  }
  // if ( pTok != NULL )
  //   strcpy(pszInfixBuffer, pTok);
  // else 
  //   strcpy(pszInfixBuffer, pszGetFirstWord());
  iRand = iRandomBetween(1, strlen(pTok-4), 5);
  pTok += iRand;
  sprintf(pszInfixBuffer, "%4.4s", pTok);

  if ( giDifficulty == 1 )
    // sprintf(pszInfixBuffer, "%3.3s", pTok);

    strcpy(pszInfixBuffer, "odo");
}
  if (giDifficulty) {
    int i = 1;

    sprintf(pszInfixBuffer, "%c%c%c%c", iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i)); 
    
    if (giDifficulty == 1)
      sprintf(pszInfixBuffer, "%c%c%c", iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i)); 
  }

#endif