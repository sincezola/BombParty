#include <stdio.h>
#include "terminal_utils.h"

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
  vSetCursorPosition(20, 1);  /** Input sempre na linha 20 */
}

void vGotoFeedbackPosition() {
  vSetCursorPosition(22, 1);  /** Feedback sempre na linha 22 */
}
void vGotoFeedbackErrorPosition() {
  vSetCursorPosition(14, 1);  /** Feedback de erro sempre na linha 14 */
}

void vGotoInputTextPosition() {
  vSetCursorPosition(20, 22); /** Linha 20, coluna após "Digite sua palavra: " */
}