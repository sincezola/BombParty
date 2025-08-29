#include <bombparty.h>
#include <command.h>
#include <singleplayer.h>
#include <multiplayer.h>
#include <room.h>
#include <input.h>
#include <trace.h>
#include <config.h>

int giDifficulty = 0;
int giArgc = 0;
char **gapszArgv = NULL;
char *gkpszProgramName = NULL;

int main(int argc, char *argv[]) { 
  int iRsl; 
  
  gkpszProgramName = argv[0];
  giArgc = argc;
  gapszArgv = argv;

  vInitLogs();

  if ( iInitSockets() < 0 )
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
