#include <stdio.h>
#include <string.h>
#include <word_db.h>

int bSearchWordDb(char *pszWord) {
  char szSearch[1024];
  char szFilePath[_MAX_PATH];
  FILE *pfDb;

  sprintf(szFilePath, "%s/%s", PATH_TO_DB, WORD_DB_TITLE);
  if ((pfDb = fopen(szFilePath, "r")) == NULL) return FALSE;

  memset(szSearch, 0, sizeof(szSearch));
  while (fgets(szSearch, sizeof(szSearch), pfDb)) {
    strtok(szSearch, "\n");
    szSearch[strlen(szSearch) - 1] = 0;
    
    // printf("[%s] [%s]\n", szSearch, pszWord);

    if (!strcmp(szSearch, pszWord)) {
      fclose(pfDb);
      return TRUE;
    }

    memset(szSearch, 0, sizeof(szSearch));
  }

  fclose(pfDb);
  return FALSE;
}

void vGetWordFromDb(char *pszWord, int iWordSz, int iLine) {
  char szSearch[1024];
  char szFilePath[_MAX_PATH];
  int ii = 0;
  FILE *pfDb;

  sprintf(szFilePath, "%s/%s", PATH_TO_DB, WORD_DB_TITLE);
  if ((pfDb = fopen(szFilePath, "r")) == NULL) return ;

  memset(szSearch, 0, sizeof(szSearch));
  while (fgets(szSearch, sizeof(szSearch), pfDb) && ii++ < iLine );
  
  strtok(szSearch, "\n");
  sprintf(pszWord, "%.*s", iWordSz, szSearch);
  fclose(pfDb);
  return;
}