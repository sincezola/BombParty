#include <config.h>

#ifndef PARSEAPI_H
  #define PARSEAPI_H
  

  // typedef struct STRUCT_WORDLIST {
  //   char pszDefaultWord[MAX_WORD_LEN];
  //   char pszParsedWord[MAX_WORD_LEN];
  //   struct STRUCT_WORDLIST *pstNext;
  // } STRUCT_WORDLIST, *PSTRUCT_WORDLIST;

  // PSTRUCT_WORDLIST pstParseApi(char *pszApiContent);
  // void vParseWordList(PSTRUCT_WORDLIST pstList);
  // int bIsCorrectWord(char *pszWord, PSTRUCT_WORDLIST pstWordList);
  // void vFreeList(PSTRUCT_WORDLIST pstList);
  // void vOutputWordList(PSTRUCT_WORDLIST pstList);
  // void vOutputRawWordList(PSTRUCT_WORDLIST pstList);
  void vRemoveWordAccents(char *pszInput);
#endif