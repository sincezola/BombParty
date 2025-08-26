#include <room.h>
#include <input.h>
#include <terminal_utils.h>
#include <hud.h>
#include <player.h>

int iRunMultiplayerMode() {
  while ( TRUE ) {
    int iAction = 0;
    
    #ifdef FAKE
      vTraceVarArgsFn("vDrawRooms Dummy Rooms");
      vCreateDummyRooms();
      vDrawFakeHud();
      return 0;
    #endif
  
    vDrawRooms();

    if ( gpstCurrentRoom != NULL && gpstCurrentPlayer != NULL ){
      vDrawHud(gpstCurrentRoom, gpstCurrentPlayer);
      if ( gpstCurrentRoom != NULL && gpstCurrentPlayer != NULL )
        continue;
    }

    if ( !(iAction = iReadMultiplayerAction()) ) continue;

    if ( iAction == ACTION_CREATE ) {
      if ( iNewPlayerRoom() < 0 ) {
        vPrintLine("Erro ao criar sala", INSERT_NEW_LINE);
        return -1; 
      }
    }
    else if ( iAction == ACTION_JOIN ) { 
      iJoinRoom();
    }    
    else if ( iAction == ACTION_REFRESH_LIST ) { 
      vDrawRooms();
    }
  }
  
  return 0;
}