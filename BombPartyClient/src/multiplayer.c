#include <room.h>
#include <input.h>
#include <terminal_utils.h>

int iRunMultiplayerMode() {
  while ( TRUE ) {
    int iAction = 0;
    
    vDrawRoom();

    if ( !(iAction = iReadMultiplayerAction()) ) continue;

    if ( iAction == ACTION_CREATE ) {
      if ( iNewPlayerRoom() < 0 ){
        vPrintLine("Erro ao criar sala", INSERT_NEW_LINE);
        return -1; 
      }
    }
    else if ( iAction == ACTION_JOIN ) { 
      iJoinRoom();
    }
  }
  
  return 0;
}