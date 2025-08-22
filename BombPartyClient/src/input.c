#include <room.h>
#include <player.h>
#include <hud.h>
#include <bombparty.h>
#include <ctype.h>
#include <input.h>
#include <parse_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <terminal_utils.h>
#include <infix_generator.h>
#include <sys_interface.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
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
int iPortableGetchar() {
#ifdef _WIN32
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD record;
  DWORD read;

  while (1) {
    ReadConsoleInput(hStdin, &record, 1, &read);

    if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
      WORD vkCode = record.Event.KeyEvent.wVirtualKeyCode;

      // Debug para Virtual Key Code
      // vPrintLine("[DEBUG - WIN] VK = %d\n", vkCode);
      fflush(stdout);

      switch (vkCode) {
        case VK_UP:    return -1;
        case VK_DOWN:  return -2;
        case VK_RIGHT: return -3;
        case VK_LEFT:  return -4;
        case VK_RETURN: return '\n';
        default:
          return record.Event.KeyEvent.uChar.AsciiChar;
      }
    }
  }
#else
  struct termios stOldt, stNewt;
  int iCh, iSecond = -1, iThird = -1;

  tcgetattr(STDIN_FILENO, &stOldt);
  stNewt = stOldt;
  stNewt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &stNewt);

  iCh = getchar();

  if (iCh == 27) { // ESC
    iSecond = getchar();
    if (iSecond == '[') {
      iThird = getchar();

      // vPrintLine("[DEBUG - LIN] Sequence: ESC (%d), [ (%d), Code (%d)\n", iCh, iSecond, iThird);
      tcsetattr(STDIN_FILENO, TCSANOW, &stOldt);

      switch (iThird) {
        case 'A': return -1; // ↑
        case 'B': return -2; // ↓
        case 'C': return -3; // →
        case 'D': return -4; // ←
        default:
          return 0;
      }
    }
  } else {
    // vPrintLine("[DEBUG - LIN] Key = %d (%c)\n", iCh, (iCh >= 32 && iCh <= 126) ? iCh : '?');
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &stOldt);
  return iCh;
#endif
}


/**
 * @brief Pausa a execução por um número de segundos (cross-platform)
 * @param iSeconds - tempo em segundos
 */
void vSleepSeconds(int iSeconds) {
#ifdef _WIN32
  Sleep(iSeconds * 1000);
#else
  sleep(iSeconds);
#endif
}

/**
 * @brief Limpa o terminal (cross-platform)
 */
void vClearTerminal() {
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
void vFlushInput() {
#ifdef _WIN32
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  FlushConsoleInputBuffer(hIn);
#else
  int iC;
  while ((iC = getchar()) != '\n' && iC != EOF) {
  }
#endif
}

/**
 * @brief Converte uma string para minúsculas
 * @param pszBuf - string a ser convertida
 */
void vToLower(char *pszBuf) {
  for (char *p = pszBuf; *p; ++p)
    if (*p >= 'A' && *p <= 'Z')
      *p += 32; /* 'A'(65)+32 = 'a'(97) */
}

/**
 * @brief Verifica se a string contém apenas espaços
 * @param pszStr - string a ser verificada
 * @return bool - TRUE se for só espaços, FALSE caso contrário
 */
bool bIsOnlySpaces(const char *pszStr) {
  while (*pszStr) {
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
int iSetDifficultyFromChar(int iCh) {
  int iDifficulty = EASY;
  if (iCh == 'e')
    iDifficulty = EASY;
  else if (iCh == 'm')
    iDifficulty = MEDIUM;
  else if (iCh == 'h')
    iDifficulty = HARD;
  return iDifficulty;
}

void vReadPlayerName(char *pszName, int iNameSz) {
  do {
    vClearTerminal();
    vPrintLine("\n Escolha seu nome :", INSERT_NEW_LINE);
    vPrintLine("Nome: ", NO_NEW_LINE);

    memset(pszName, 0, iNameSz);
    if (fgets(pszName, iNameSz, stdin)) {
      if (strchr(pszName, '\n') == NULL)
        vFlushInput();
    }
  } while ( bStrIsEmpty(pszName) );
  
  strtok(pszName, "\n");

  return;
}

int iReadEditable() {
  int iRsl;
  int iCh;
  char szBuffer[128];

  char szLine[1024];
  do {
    vClearTerminal();
    vPrintLine("\n Escolha sua o que deseja editar :", INSERT_NEW_LINE);
    sprintf(szLine, 
    "\t[C] Capacidade\n"
    "\t[D] Dificuldade\n"
    "\t[A] Ambos");
    vPrintLine(szLine, INSERT_NEW_LINE);
    vPrintLine("Opcao: ", NO_NEW_LINE);

    memset(szBuffer, 0, sizeof(szBuffer));
    if (fgets(szBuffer, sizeof(szBuffer), stdin)) {
      if (strchr(szBuffer, '\n') == NULL)
        vFlushInput();
    }

    iCh = tolower(szBuffer[0]);
  } while (iCh != 'c' && iCh != 'd' && iCh != 'a');

  if ( iCh == 'c' )
    iRsl = EDITABLE_OPT_CAPACITY;
  else if ( iCh == 'd' )
    iRsl = EDITABLE_OPT_DIFFICULTY;
  else 
    iRsl = EDITABLE_OPT_BOTH;

  return iRsl;
}

void vReadRoomName(char *pszName, int iNameSz) {
  do {
    vClearTerminal();
    vPrintLine("\n Escolha o nome da sala :", INSERT_NEW_LINE);
    vPrintLine("Nome: ", NO_NEW_LINE);

    memset(pszName, 0, iNameSz);
    if (fgets(pszName, iNameSz, stdin)) {
      if (strchr(pszName, '\n') == NULL)
          vFlushInput();
    }
  } while ( bStrIsEmpty(pszName) );

  strtok(pszName, "\n");
  
  return;
}

int iReadOptions() {
  return fgetc(stdin);
}

void vReadRoomDifficulty(int *iDifficulty) {
  char szBuffer[128];
  int iCh;
  char szLine[1024];

  do {
    vClearTerminal();
    vPrintLine("\n Escolha sua a dificuldade :", INSERT_NEW_LINE);
    sprintf(szLine, 
    "\t[E] Easy   (%d letras por palavra)\n"
    "\t[M] Medium (%d letras por palavra)\n"
    "\t[H] Hard   (%d letras por palavra)",
    EASY_INFIX, MEDIUM_INFIX, HARD_INFIX);
    vPrintLine(szLine, INSERT_NEW_LINE);
    vPrintLine("Dificuldade: ", NO_NEW_LINE);

    memset(szBuffer, 0, sizeof(szBuffer));
    if (fgets(szBuffer, sizeof(szBuffer), stdin)) {
      if (strchr(szBuffer, '\n') == NULL)
        vFlushInput();
    }

    iCh = tolower(szBuffer[0]);
  } while (iCh != 'e' && iCh != 'm' && iCh != 'h');

  *iDifficulty = iSetDifficultyFromChar(iCh);
  vPrintLine("\n", INSERT_NEW_LINE);
  
  return;
}

void vReadRoomPlayerPassword(char *pszBuff, int iBuffLen) {
  vClearTerminal();
  vPrintLine("Insira a senha: ", INSERT_NEW_LINE);
  memset(pszBuff, 0, iBuffLen);

  if (fgets(pszBuff, iBuffLen, stdin) == NULL) return;
}

void vReadRoomPassword(char *pszPassword, int iPasswdSz) {
  char szBuffer[128];
  int iCh;

  memset(pszPassword, 0, iPasswdSz);
  do {
    vClearTerminal();
    vPrintLine("\n Deseja criar uma senha para sala?\n", INSERT_NEW_LINE);
    vPrintLine("\t [S] - Sim | [N] - Nao\n", INSERT_NEW_LINE);
    vPrintLine("Criar senha? ", NO_NEW_LINE);

    memset(szBuffer, 0, sizeof(szBuffer));
    if (fgets(szBuffer, sizeof(szBuffer), stdin)) {
      if (strchr(szBuffer, '\n') == NULL)
        vFlushInput();
    }
    iCh = tolower(szBuffer[0]);
  } while (iCh != 's' && iCh != 'n');
  
  if ( iCh == 'n' ){
    return;
  }

  do {
    vClearTerminal();
    vPrintLine("\n Defina a senha.", INSERT_NEW_LINE);
    vPrintLine("Senha: ", NO_NEW_LINE);

    memset(szBuffer, 0, sizeof(szBuffer));
    if (fgets(szBuffer, sizeof(szBuffer), stdin)) {
      if (strchr(szBuffer, '\n') == NULL)
        vFlushInput();
    }
    strtok(szBuffer, "\n");

  } while ( bStrIsEmpty(szBuffer) );
  
  snprintf(pszPassword, iPasswdSz, "%s", szBuffer);

  strtok(pszPassword, "\n");

  return;
}

void vReadRoomCapacity(int *iCapacity) {
  char szBuffer[128];
  do {
    vClearTerminal();
    vPrintLine("\n Escolha a quantidade maxima de jogadores :", INSERT_NEW_LINE);
    vPrintLine("\t Minimo - 2\t Maximo 5", INSERT_NEW_LINE);
    vPrintLine("Capacidade da sala: ", NO_NEW_LINE);

    memset(szBuffer, 0, sizeof(szBuffer));
    if (fgets(szBuffer, sizeof(szBuffer), stdin)) {
      if (strchr(szBuffer, '\n') == NULL)
        vFlushInput();
    }

  } while ( atoi(szBuffer) < 2 || atoi(szBuffer) > 5);
  
  *iCapacity = atoi(szBuffer);
  vPrintLine("\n", NO_NEW_LINE);
  
  return;
}

int iReadGameMode() {
  char szMode[128];
  int  iCh;
  do{
    vClearTerminal();
    vPrintLine("\n Escolha o modo de jogo :", INSERT_NEW_LINE);
    vPrintLine("\t[S] Singleplayer (Um jogador local)", INSERT_NEW_LINE);
    vPrintLine("\t[M] Multiplayer  (Mais jogadores Online)", INSERT_NEW_LINE);
    vPrintLine("Modo: ", NO_NEW_LINE);

    memset(szMode, 0, sizeof(szMode));
    if (fgets(szMode, sizeof(szMode), stdin)) {
      if (strchr(szMode, '\n') == NULL)
          vFlushInput();
      }
    iCh = tolower(szMode[0]);
  } while (iCh != 's' && iCh != 'm');
  
  return (iCh == 's') ? SINGLEPLAYER : MULTIPLAYER;
}

int iReadMultiplayerAction() {
  char szMode[128];
  int  iCh;
  
  vPrintLine("\n Escolha uma acao :", INSERT_NEW_LINE);
  vPrintLine("\t[E] Entrar em uma sala ", INSERT_NEW_LINE);
  vPrintLine("\t[C] Criar  uma sala", INSERT_NEW_LINE);
  vPrintLine("\n\t[A] Atualizar lista de salas", INSERT_NEW_LINE);
  vPrintLine("Opcao: ", NO_NEW_LINE);

  memset(szMode, 0, sizeof(szMode));
  if (fgets(szMode, sizeof(szMode), stdin)) {
    if (strchr(szMode, '\n') == NULL)
        vFlushInput();
    }
 
  iCh = tolower(szMode[0]);
  
  if ( iCh != 'e' && iCh != 'c' && iCh != 'a')
    iCh = 0;

  return iCh;
}
/**
 * @brief Captura input do usuário caractere a caractere, com redesenho contínuo
 * @return char* - ponteiro para a string digitada (precisa ser liberado com
 * free)
 */
/**
 * @brief Captura input do usuário caractere a caractere, com redesenho contínuo
 * @return char* - ponteiro para a string digitada (precisa ser liberado com
 * free)
 */
char *cCatchInput() {
  char *pszBuffer = (char *)malloc(MAX_WORD_LEN);
  int iBufferLen = 0;
  memset(pszBuffer, 0, MAX_WORD_LEN);

  while (TRUE) {
    /** Redesenha linha de input */
    vGotoInputPosition();
    // vPrintLine("Digite sua palavra: %s", pszBuffer);
    vClearLineFromCursor();
    fflush(stdout);

    /** Captura um único caractere */
    int iCh = iPortableGetchar();

    /** Verifica se a bomba explodiu */
    if (gbBombTimeout) {
      // vPrintLine(pszBuffer, "%s", TIMEOUT_STR);
      return pszBuffer;
    }

    if (iCh == '\n' || iCh == '\r') { /** Enter */
      if (iBufferLen > 0 && !bIsOnlySpaces(pszBuffer))
        break; /** Input válido */
      vGotoFeedbackPosition();
      vPrintLine("Você deve digitar uma palavra!", INSERT_NEW_LINE);
      fflush(stdout);
      vSleepSeconds(1);
      continue;
    } else if (iCh == 127 || iCh == 8) { /** Backspace */
      if (iBufferLen > 0) {
        iBufferLen--;
        pszBuffer[iBufferLen] = '\0';
      }
    } else if (isprint(iCh) &&
               iBufferLen < MAX_WORD_LEN - 1) { /** Caractere imprimível */
      pszBuffer[iBufferLen++] = (char)iCh;
      pszBuffer[iBufferLen] = '\0';
    }
  }

  /** Converte para minúsculo e remove acentos */
  vToLower(pszBuffer);
  vRemoveWordAccents(pszBuffer);

  return pszBuffer;
}