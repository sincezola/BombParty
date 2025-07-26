#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <input.h>
#include <parse_api.h>
#include <bombparty.h>
#include <terminal_utils.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

/**
 * @brief Remove espaços no início e no fim da string.
 * @param pszStr - string a ser tratada
 */
void vTrimSpaces(char *pszStr) {
  char *pszStart = pszStr;
  char *pszEnd;

  // Remove espaços no início
  while (*pszStart == ' ') {
    pszStart++;
  }

  // Move string para o início se necessário
  if (pszStart != pszStr) {
    memmove(pszStr, pszStart, strlen(pszStart) + 1);
  }

  // Remove espaços no final
  pszEnd = pszStr + strlen(pszStr) - 1;
  while (pszEnd >= pszStr && *pszEnd == ' ') {
    *pszEnd = '\0';
    pszEnd--;
  }
}


/**
 * @brief Captura um único caractere do teclado, sem eco e sem buffer
 * @return int - Código ASCII do caractere lido
 */
int iPortableGetchar()
{
#ifdef _WIN32
  return _getch();
#else
  struct termios stOldt, stNewt;
  int iCh;
  tcgetattr(STDIN_FILENO, &stOldt);
  stNewt = stOldt;
  stNewt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &stNewt);
  iCh = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &stOldt);
  return iCh;
#endif
}

/**
 * @brief Pausa a execução por um número de segundos (cross-platform)
 * @param iSeconds - tempo em segundos
 */
void vSleepSeconds(int iSeconds)
{
#ifdef _WIN32
  Sleep(iSeconds * 1000);
#else
  sleep(iSeconds);
#endif
}

/**
 * @brief Limpa o terminal (cross-platform)
 */
void vClearTerminal()
{
#ifdef _WIN32
  system("cls");
#else
  int iRet = system("clear");
  (void)iRet;
#endif
}

/**
 * @brief Limpa buffer de entrada do teclado
 */
void vFlushInput()
{
#ifdef _WIN32
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  FlushConsoleInputBuffer(hIn);
#else
  int iC;
  while ((iC = getchar()) != '\n' && iC != EOF)
  {
  }
#endif
}

/**
 * @brief Converte uma string para minúsculas
 * @param pszBuf - string a ser convertida
 */
void vToLower(char *pszBuf)
{
  for (char *p = pszBuf; *p; ++p)
    if (*p >= 'A' && *p <= 'Z')
      *p += 32; /* 'A'(65)+32 = 'a'(97) */
}

/**
 * @brief Verifica se a string contém apenas espaços
 * @param pszStr - string a ser verificada
 * @return int - TRUE se for só espaços, FALSE caso contrário
 */
int bIsOnlySpaces(const char *pszStr)
{
  while (*pszStr)
  {
    if (!((unsigned char)*pszStr == ' '))
      return FALSE;
    pszStr++;
  }
  return TRUE;
}

/**
 * @brief Converte um caractere em nível de dificuldade
 * @param iCh - caractere (e/m/h)
 * @return int - nível de dificuldade (EASY/MEDIUM/HARD)
 */
int iSetDifficultyFromChar(int iCh)
{
  int iDifficulty = EASY;
  if (iCh == 'e')
    iDifficulty = EASY;
  else if (iCh == 'm')
    iDifficulty = MEDIUM;
  else if (iCh == 'h')
    iDifficulty = HARD;
  return iDifficulty;
}

/**
 * @brief Captura input do usuário caractere a caractere, com redesenho contínuo
 * @return char* - ponteiro para a string digitada (precisa ser liberado com free)
 */
/**
 * @brief Captura input do usuário caractere a caractere, com redesenho contínuo
 * @return char* - ponteiro para a string digitada (precisa ser liberado com free)
 */
char *cCatchInput()
{
  char *pszBuffer = (char *)malloc(MAX_WORD_LEN);
  int iBufferLen = 0;
  memset(pszBuffer, 0, MAX_WORD_LEN);

  while (TRUE)
  {
    /** Redesenha linha de input */
    vGotoInputPosition();
    printf("Digite sua palavra: %s", pszBuffer);
    vClearLineFromCursor();
    fflush(stdout);

    /** Captura um único caractere */
    int iCh = iPortableGetchar();

    /** Verifica se a bomba explodiu */
    if (gbBombTimeout)
    {
      sprintf(pszBuffer, "%s", TIMEOUT_STR);
      return pszBuffer;
    }

    if (iCh == '\n' || iCh == '\r')
    { /** Enter */
      if (iBufferLen > 0 && !bIsOnlySpaces(pszBuffer))
        break; /** Input válido */
      vGotoFeedbackPosition();
      printf("Você deve digitar uma palavra!\n");
      fflush(stdout);
      vSleepSeconds(1);
      continue;
    }
    else if (iCh == 127 || iCh == 8)
    { /** Backspace */
      if (iBufferLen > 0)
      {
        iBufferLen--;
        pszBuffer[iBufferLen] = '\0';
      }
    }
    else if (isprint(iCh) && iBufferLen < MAX_WORD_LEN - 1)
    { /** Caractere imprimível */
      pszBuffer[iBufferLen++] = (char)iCh;
      pszBuffer[iBufferLen] = '\0';
    }
  }

  /** Converte para minúsculo e remove acentos */
  vToLower(pszBuffer);
  vRemoveWordAccents(pszBuffer);

  return pszBuffer;
}