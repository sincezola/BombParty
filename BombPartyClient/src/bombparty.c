#include <bombparty.h>
#include <command.h>
#include <singleplayer.h>
#include <multiplayer.h>
#include <room.h>
#include <input.h>
#include <trace.h>

int giDifficulty = 0;
const char *gkpszProgramName;

int main(int argc, char *argv[]) { 
  int iRsl; 
  
  gkpszProgramName = argv[0];

  if ( iInitSockets(argc, argv) < 0 )
    return -1;

  vInitLogs();
  
  switch ( iReadGameMode() ) {
    case SINGLEPLAYER:
      iRsl = iRunSingleplayerMode();
      break;
    case MULTIPLAYER:
      iRsl = iRunMultiplayerMode();
      break;
    
    default:
      iRsl = -1;
      break;
  }

  return iRsl;
}
