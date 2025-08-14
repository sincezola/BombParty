#include <stdio.h>

#ifndef CONFIG_H
  #define CONFIG_H

  extern int giDifficulty;

  // Bool definition
  typedef int bool;

  #define FALSE 0
  #define TRUE  1

  #define EASY   1
  #define MEDIUM 2
  #define HARD   3

  #define ROOM_CREATED 1
  #define ROOM_IN_GAME 2
  #define ROOM_CLOSED  3

  #define PATH_TO_DB    "db"
#ifndef _MAX_PATH
  #define _MAX_PATH     256
#endif
  #define MAX_WORD_LEN  128
  #define DIFICULTY_LEN   8 
#endif