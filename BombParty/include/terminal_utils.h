#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

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

#endif
