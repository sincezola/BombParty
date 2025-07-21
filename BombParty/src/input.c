#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <string.h>
#include <parse_api.h>
#include <bombparty.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
#endif

void vSleepSeconds(int seconds) {
#ifdef _WIN32
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
}

void vClearTerminal() {
#ifdef _WIN32
  system("cls");
#else
  int vRet = system("clear"); // <- avoid compilation warnings
  (void)vRet;
#endif
}

void vFlushInput() {
  int c; 
  while ((c = getchar()) != '\n' && c != EOF) {}
}

void vToLower(char *pszBuf) {
  for (char *p = pszBuf; *p; ++p)
  if (*p >= 'A' && *p <= 'Z')
    *p += 32; /* 'A'(65)+32 = 'a'(97) */
}

int bIsOnlySpaces(const char *str) {
  while (*str) {
    if (!((unsigned char)*str == ' '))
      return FALSE;
    str++;
  }
  return TRUE;
}

int iSetDifficultyFromChar(int iCh){
  int iDifficulty = EASY;
  if      ( iCh == 'e' ) iDifficulty = EASY;
  else if ( iCh == 'm' ) iDifficulty = MEDIUM;
  else if ( iCh == 'h' ) iDifficulty = HARD;
  return iDifficulty;
}

char *cCatchInput() {
  int iBufferLen;
  char *pszBuffer = (char *)malloc(MAX_WORD_LEN);

  memset(pszBuffer, 0, MAX_WORD_LEN);
  while ( TRUE ) {
    /* Nao retirar o \n */
    printf("\033[16;1H Digite sua palavra: \n");
    if (!fgets(pszBuffer, MAX_WORD_LEN, stdin)) {
      if ( gbBombTimeout ){
        sprintf(pszBuffer, "%s", TIMEOUT_STR);
        return pszBuffer;
      }
      free(pszBuffer);
      return NULL;
    }

    if ( gbBombTimeout ){
      sprintf(pszBuffer, "%s", TIMEOUT_STR);
      return pszBuffer;
    }

    iBufferLen = strlen(pszBuffer);
    if ( iBufferLen < 1 ){
      printf("Você deve digitar uma palavra!\n");
      vSleepSeconds(1.5f);
      /* vClearTerminal(); */
      continue;
    }
    
    strtok(pszBuffer, "\n"); 
    pszBuffer[iBufferLen - 1] = '\0';
    
    if (pszBuffer[0] != '\0' && !bIsOnlySpaces(pszBuffer))
      break;

    printf("Você deve digitar uma palavra!\n");
    vSleepSeconds(1.5f);
    /* vClearTerminal(); */
  }

  vToLower(pszBuffer); /* Changes buff to lowercase */
  vRemoveWordAccents(pszBuffer); /* Remove buff accents */

  return pszBuffer;
}