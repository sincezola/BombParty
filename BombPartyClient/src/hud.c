#include <stdio.h>
#include <room.h>
#include <player.h>
#include <hud.h>
#include <terminal_utils.h>

void vDrawHud(PSTRUCT_ROOM pstCurrentRoom, PSTRUCT_PLAYER pstPlayer) {
  char szDivLine[HUD_MAX_CHAR];
  char szLine[1024];
  char szContents[1024];

  memset(szLine, 0, sizeof(szLine));
  memset(szDivLine, 0, sizeof(szDivLine));

  szDivLine[0] = ' ';
  memset(&szDivLine[1], '_', HUD_MAX_CHAR - 2);
  szLine[HUD_MAX_CHAR - 1] = ' ';
  vPrintLine(szLine, INSERT_NEW_LINE);

  memset(szContents, 0, sizeof(szContents));
  szContents[0] = '|';
  memset(&szContents[1], ' ', HUD_MAX_CHAR - 2);
  szContents[HUD_MAX_CHAR - 1] = '|';

  for ( ii = 0; ii < HUX_MAX_HEIGHT - 3; ii++ )  vPrintLine(szContents, INSERT_NEW_LINE);

  // sprintf(szContents, "| 1-> %s 2->%s%s",);
  vPrintLine(szLine, INSERT_NEW_LINE);

}