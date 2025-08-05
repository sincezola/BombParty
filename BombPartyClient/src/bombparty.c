#include <bombparty.h>
#include <singleplayer.h>
#include <multiplayer.h>

int giDifficulty = 0;

int main() { 
  int iRsl; 
  
  switch ( iReadGameMode() ) {
    case SINGLEPLAYER:
      iRsl = iRunSingleplayerMode();
      break;
    case MULTIPLAYER:
      iRsl = iRunMultiplayerMode();
      break;
    
    default:
      break;
  }

  return iRsl;
}
