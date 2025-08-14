#include <stdio.h>
#include <string.h>
#include <parse_api.h>
#include <stdlib.h>
#include <config.h>

void vRemoveWordAccents(char *pszInput) {
  const unsigned char *pucSrc = (const unsigned char *)pszInput;
  char *pszPtr = pszInput;

  while (*pucSrc) {
    if (*pucSrc == 0xC3) {
      pucSrc++;
      switch (*pucSrc) {
        case 0xA1: case 0xA0: case 0xA2: case 0xA3: *pszPtr = 'a'; break;
        case 0xA9: case 0xA8: case 0xAA: *pszPtr = 'e'; break;
        case 0xAD: case 0xAC: case 0xAE: *pszPtr = 'i'; break;
        case 0xB3: case 0xB2: case 0xB4: case 0xB5: *pszPtr = 'o'; break;
        case 0xBA: case 0xB9: case 0xBB: *pszPtr = 'u'; break;
        case 0xA7: *pszPtr = 'c'; break;
        default: *pszPtr = '?'; break;
      }
    } else {
      *pszPtr = *pucSrc;
    }
    pucSrc++;
    pszPtr++;
  }
  *pszPtr = '\0';
}

/** Parte antiga que funciona, o que eesta fora do if 0 ja tinha sido descontinuado
 *  se voltarmos a usar, usar somente entre if 0 e endif
*/
#if 0 

void vFreeList(PSTRUCT_WORDLIST pstList) {
  PSTRUCT_WORDLIST pstLastNode;
  PSTRUCT_WORDLIST pstCurrentNode;

  for (pstCurrentNode = pstList; pstCurrentNode != NULL; ) {
    pstLastNode = pstCurrentNode;
    pstCurrentNode = pstCurrentNode->pstNext;

    free(pstLastNode);
  }
}

void vOutputWordList(PSTRUCT_WORDLIST pstList) {
  PSTRUCT_WORDLIST pstCurrentNode;

  for (pstCurrentNode = pstList; pstCurrentNode != NULL; pstCurrentNode = pstCurrentNode->pstNext)
    vPrintLine("[%-30.30s] -> [%-30.30s]\n", pstCurrentNode->pszDefaultWord, pstCurrentNode->pszParsedWord);
}

void vOutputRawWordList(PSTRUCT_WORDLIST pstList) {
  PSTRUCT_WORDLIST pstCurrentNode;

  for (pstCurrentNode = pstList; pstCurrentNode != NULL; pstCurrentNode = pstCurrentNode->pstNext) {
    vPrintLine("%s\n", pstCurrentNode->pszParsedWord);
  }
}

PSTRUCT_WORDLIST pstParseApi(char *pszApiContent) {
  char *pszWordToken;
  char szWord[MAX_WORD_LEN];
  PSTRUCT_WORDLIST pstCurrNode = NULL;
  PSTRUCT_WORDLIST pstFirst = NULL;

  pszWordToken = strstr(pszApiContent, "\"word\":\"");

  while (pszWordToken != NULL) {
    char *pTok;

    pszWordToken += strlen("\"word\":\"");
    pTok = pszWordToken;
    
    if( (pTok = strchr(pTok, '"')) == NULL)
      return NULL;

    sprintf(szWord, "%.*s", (int)(pTok - pszWordToken), pszWordToken);

    /* Primeiro no? */
    if (pstFirst == NULL) {
      pstCurrNode = (PSTRUCT_WORDLIST)malloc(sizeof(STRUCT_WORDLIST));
      pstFirst = pstCurrNode;
    } else { /* Demais nos */
      pstCurrNode->pstNext = (PSTRUCT_WORDLIST)malloc(sizeof(STRUCT_WORDLIST));
      pstCurrNode = pstCurrNode->pstNext;
    }
    pstCurrNode->pstNext = NULL; /* Aterramos o final */

    memset(pstCurrNode, 0, sizeof(STRUCT_WORDLIST));
    strcpy(pstCurrNode->pszDefaultWord, szWord);

    pszWordToken = strstr(pszWordToken, "\"word\":\"");
  }

  free(pszApiContent);  // Dá fre no ponteiro de ApiResult
  return pstFirst;
}

#endif

// void vParseWordList(PSTRUCT_WORDLIST pstList) {
//   PSTRUCT_WORDLIST pstCurrentNode;
//   char *pszCrrWord;

//   for (pstCurrentNode = pstList; pstCurrentNode != NULL; pstCurrentNode = pstCurrentNode->pstNext) {
//     pszCrrWord = pszRemoveWordAccents(pstCurrentNode->pszDefaultWord);
//     strcpy(pstCurrentNode->pszParsedWord, pszCrrWord);
//     free(pszCrrWord);
//   }

//    vOutputWordList(pstList);

//   return;
// }

// int bIsCorrectWord(char *pszWord, PSTRUCT_WORDLIST pstWordList) {
//   PSTRUCT_WORDLIST pstPointer;
// 
//   for (pstPointer = pstWordList; pstPointer != NULL; pstPointer = pstPointer->pstNext) {
//      vPrintLine("\nPSZWORD: %s PSZPARSED WORD: %s", pszWord, pstPointer->pszParsedWord);
// 
//     if (strcmp(pszWord, pstPointer->pszParsedWord) == 0) return TRUE;
//   }
// 
//   return FALSE;
// }
