#include <config.h>

#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#define NO_NEW_LINE     0
#define INSERT_NEW_LINE 1
/**
 * @brief Move o cursor para uma posição específica no terminal.
 * @param iRow Linha (1-based)
 * @param iCol Coluna (1-based)
 */
void vSetCursorPosition(int iRow, int iCol);

/**
 * @brief Limpa toda a tela e move o cursor para o início.
 */
void vClearScreen();

/**
 * @brief Posição fixa para exibir a bomba.
 */
void vGotoBombPosition();

/**
 * @brief Posição fixa para exibir o input do jogador.
 */
void vGotoInputPosition();

/**
 * @brief Posição fixa para exibir mensagens de feedback.
 */
void vGotoFeedbackPosition();
/**
 * @brief Posição fixa do input (onde começa a digitação, após o label).
 */
void vGotoInputTextPosition();
/**
 * @brief Posição fixa para exibir mensagens de erro feedback.
 */
void vGotoFeedbackErrorPosition();

/**
 * @brief Imprime um texto colorido.
 * @param pszText Texto a imprimir
 * @param iColor Código ANSI da cor (ex: 31 = vermelho)
 */
void vPrintColored(const char *pszText, int iColor);
/**
 * @brief Limpa do cursor até o final da linha atual.
 */
void vClearLineFromCursor();
void vPrintLine(char *pszLine, bool bNewLine);
void vPrintHighlitedLine(char *pszLine, bool bNewLine);

#endif
