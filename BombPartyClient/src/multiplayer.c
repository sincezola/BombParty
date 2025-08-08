#include <room.h>
#include <input.h>

int iRunMultiplayerMode() {

  while ( TRUE ){
    int iAction = 0;
    
    vDrawRoom();

    if ( !(iAction = iReadMultiplayerAction()) ) continue;

    if ( iAction == ACTION_CREATE ){
      iNewPlayerRoom();
    }
    else if ( iAction == ACTION_JOIN ){
      iJoinRoom();
    }
  }
  
  return 0;
}