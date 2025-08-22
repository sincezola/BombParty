#include <input.h>
#include <stdio.h>
#include <room.h>
#include <player.h>
#include <hud.h>
#include <terminal_utils.h>
#include <string.h>

void vDrawHud(PSTRUCT_ROOM pstCurrentRoom, PSTRUCT_PLAYER pstPlayer) {
  char szDivLine[HUD_MAX_CHAR];
  char cOption;
  char szLine[1024];
  char szInput[1024];
  char szContents[1024];
  int ii;
  int iRole;

  memset(szLine, 0, sizeof(szLine));
  memset(szDivLine, 0, sizeof(szDivLine));
  
  szDivLine[0] = ' ';
  memset(&szDivLine[1], ' ', HUD_MAX_CHAR - 2);
  
  szDivLine[HUD_MAX_CHAR - 1] = ' ';

  vClearScreen();
  vPrintLine(szDivLine, INSERT_NEW_LINE);

  memset(szContents, 0, sizeof(szContents));
  szContents[0] = ' ';
  memset(&szContents[1], ' ', HUD_MAX_CHAR - 2);
  szContents[HUD_MAX_CHAR - 1] = ' ';

  for ( ii = 0; ii < HUX_MAX_HEIGHT - 3; ii++ )
    vPrintLine(szContents, INSERT_NEW_LINE);

  if ( (iRole = iFindPlayerRole(pstPlayer)) < 0 )
    return;
  
  if ( iRole == ROLE_OWNER ) {
    char szFieldReady[64];
    memset(szFieldReady, 0, sizeof(szFieldReady));
    strcpy(szFieldReady, "Aguardando Players");

    if ( iRoomReadyCt(pstCurrentRoom) == (pstCurrentRoom->iRoomCapacity - 1) )
      strcpy(szFieldReady, "Iniciar Jogo");

    sprintf(szContents, "  1 -> %s 2 -> Editar Sala 3 -> Remover Jogador 4 -> Destruir Sala\n", szFieldReady);
  }
  else {
    char szFieldReady[64];
    int bStatus = iGetReadyStatus(pstPlayer);
    memset(szFieldReady, 0, sizeof(szFieldReady));
    strcpy(szFieldReady, "Pronto");
    if ( bStatus == FALSE )
      strcpy(szFieldReady, "Nao pronto");

    sprintf(szContents, "  1-> %s 2-> Sair da sala\n", szFieldReady);
  }
  vPrintLine(szContents, NO_NEW_LINE);
  szDivLine[0] = ' ';
  szDivLine[strlen(szDivLine) - 1] = ' ';
  vPrintLine(szDivLine, INSERT_NEW_LINE);

  cOption = 99;
  if (iRole == ROLE_OWNER) {
    do {
      cOption = iReadOptions();
    } while (cOption != '1' && cOption != '2' && cOption != '3' && cOption != '4');
  }
  else if (iRole == ROLE_GUEST) {
    do {
      cOption = iReadOptions();
    } while (cOption != '1' && cOption != '2');
  }
  
  switch (cOption) {
    case '1':
      if (iRole == ROLE_GUEST) {
        if (iTogglePlayerRdyStatus(pstPlayer) < 0)
          return;
      }

      else if (iRole == ROLE_OWNER) {
        vInitServerGame(pstCurrentRoom);
      }

      else {
        // Spectator option 1
      }

      break;
    case '2':
      if (iRole == ROLE_GUEST) {
        iLeaveRoom(pstPlayer->iPlayerId);
      }

      else if (iRole == ROLE_OWNER) {
        iEditRoom(pstCurrentRoom);
      }
      
      break;
    case '3':
      int iPlayerId = iSelectPlayer(pstCurrentRoom);

      if (iPlayerId < 0) 
        return;

      iLeaveRoom(iPlayerId);
      break;
    case '4':
      iDestroyRoom(pstCurrentRoom->iRoomId);

      break;
    default:
      break;
  }
}