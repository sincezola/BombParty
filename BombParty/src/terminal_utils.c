#include <stdio.h>  
#include <terminal_utils.h>

void vSetCursorPosition(int iRow, int iCol) {
  printf("\033[%d;%dH", iRow, iCol);
  fflush(stdout);
}

void vClearScreen() {
  printf("\033[2J\033[1;1H");  /** Limpa a tela e volta para (1,1) */
  fflush(stdout);
}

void vGotoBombPosition() {
  vSetCursorPosition(3, 10);  /** Bomba sempre a partir da linha 3, coluna 10 */
}

void vGotoInputPosition() {
  vSetCursorPosition(13, 1);  /** Input sempre na linha 23 */
}

void vGotoFeedbackPosition() {
  vSetCursorPosition(13, 1);  /** Feedback sempre na linha 23 */
}
void vGotoFeedbackErrorPosition() {
  vSetCursorPosition(13, 1);  /** Feedback de erro sempre na linha 13 */
}

void vGotoInputTextPosition() {
  vSetCursorPosition(11, 21); /** Linha 11, coluna após "Digite sua palavra: " */
}

void vPrintColored(const char *pszText, int iColor) {
  printf("\033[%dm%s\033[0m", iColor, pszText);
}

void vClearLineFromCursor() {
  printf("\033[K");
  fflush(stdout);
}

