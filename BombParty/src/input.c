#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <string.h>
#include <parse_api.h>

#ifdef _WIN32      // Sleep includes
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

bool bIsOnlySpaces(const char *str) {
  while (*str) {
    if (!((unsigned char)*str == ' '))
      return FALSE;
    str++;
  }
  return TRUE;
}

char *cCatchInput() {
  char *buffer = (char *)malloc(MAX_WORD_LEN);
  memset(buffer, 0, MAX_WORD_LEN);

  while (1) {
    printf("Digite sua palavra: ");
    if (!fgets(buffer, MAX_WORD_LEN, stdin)) {
      free(buffer);
      return NULL;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
      buffer[len - 1] = '\0';

    if (buffer[0] != '\0' && !bIsOnlySpaces(buffer))
      break;

    printf("Você deve digitar uma palavra!\n");
    vSleepSeconds(1.5f);
    vClearTerminal();
  }

  return buffer;
}