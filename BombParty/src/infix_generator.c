#include <time.h>
#include <infix_generator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordbase.h>
#include <config.h>

int iRandomBetween(int min, int max, int iSeed) {
  srand(time(NULL) * iSeed);

  return rand() % (max - min + 1) + min;
}
void vInfixGeneratorBase(char *pszInfixBuffer){
  if (!g_iDifficulty) return;

  int iRand;
  int ii;
  char *pTok = NULL;
  
  srand(time(NULL));
  iRand = rand() % 60;
  for (ii = 0; ii < iRand; ii++ ){
    pTok = pszGetNextWord();
  }
  // if ( pTok != NULL )
  //   strcpy(pszInfixBuffer, pTok);
  // else 
  //   strcpy(pszInfixBuffer, pszGetFirstWord());
  iRand = iRandomBetween(1, strlen(pTok-4), 5);
  pTok += iRand;
  sprintf(pszInfixBuffer, "%4.4s", pTok);

  if ( g_iDifficulty == 1 )
    sprintf(pszInfixBuffer, "%3.3s", pTok);
}
void vInfixGeneratorRandom(char *pszInfixBuffer) {
  if ( pszInfixBuffer != NULL)
    return ;
  return;
  // if (g_iDifficulty) {
  //   int i = 1;

  //   sprintf(pszInfixBuffer, "%c%c%c%c", iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i)); 
    
  //   if (g_iDifficulty == 1)
  //     sprintf(pszInfixBuffer, "%c%c%c", iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i), iRandomBetween(97, 122, ++i)); 
  // }
}