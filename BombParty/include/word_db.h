#include <config.h>

#ifndef WORD_DB_H
  #define WORD_DB_H
  #define WORD_DB_TITLE "word_db.txt"
  
  int bSearchWordDb(char *pszWord);
  void vGetWordFromDb(char *pszWord, int iWordSz, int iLine);
#endif