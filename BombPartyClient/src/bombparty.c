#include <bombparty.h>
#include <command.h>
#include <singleplayer.h>
#include <multiplayer.h>
#include <room.h>
#include <input.h>
#include <trace.h>

int giDifficulty = 0;
char *gkpszProgramName;
int *giArgc;
char **gapszArgv;

int main(int argc, char *argv[]) { 
  int iRsl; 
  
  gkpszProgramName = argv[0];
  giArgc = &argc;
  gapszArgv = argv;
  vInitLogs();

  if ( iInitSockets(argc, argv) < 0 )
    return -1;

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
