#include <room.h>
#include <stdio.h>
#include <string.h>
#include <config.h>

char *pszDifficulty[]={
  "-",
  "Easy",
  "Medium",
  "Hard",
  NULL
};
/**
  Maximo de caracteres
    Nome: 36
    Complexidade: 16
    Capacidade: 14
    Status: 10
*/

/**
    __________________________________________________________________________
    |     Nome    |     Complexidade     |     Capacidade    |     Status    |
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
    |-------------|---------------------|-------------------|----------------|
*/


int iCalcPaddingOffset(int iFieldSize, int *iContentLen){
  int iEmptyLeftSize;

  if ( iFieldSize <= 0 || *iContentLen <= 0 )
    return -1;

  if ( iFieldSize < *iContentLen )
    return -2;

  if ( *iContentLen % 2 ) *iContentLen = *iContentLen + 1;

  iEmptyLeftSize = iFieldSize - *iContentLen;
  
  return (iEmptyLeftSize / 2);
}



void vDrawRooms() {
  char szLine[128];
  int iNameLen;
  int iNameOffset;
  int iDifficultyLen;
  int iDifficultyOffset;
  int iCapacityLen;
  int iCapacityOffset;
  int iStatusLen;
  int iStatusOffset;

  /** Separator Line */
  memset(szLine, 0, sizeof(szLine));
  szLine[0] = ' ';
  memset(&szLine[1], '_', ROOM_MAX_CHARS-2);
  szLine[ROOM_MAX_CHARS-1] = ' ';
  
  /** Title Line */
  iNameLen = strlen(ROOM_NAME);
  iNameOffset = iCalcPaddingOffset(NAME_FLD_SIZE, &iNameLen);

  iDifficultyLen = strlen(ROOM_DIFFICULTY);
  iDifficultyOffset = iCalcPaddingOffset(DIFFICULTY_FLD_SIZE, &iDifficultyLen);

  iCapacityLen = strlen(ROOM_CAPACITY);
  iCapacityOffset = iCalcPaddingOffset(CAPACITY_FLD_SIZE, &iCapacityLen);

  iStatusLen = strlen(ROOM_STATUS);
  iStatusOffset = iCalcPaddingOffset(STATUS_FLD_SIZE, &iStatusLen);

  printf("%s\n", szLine);
  printf(
"|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|%*.*s%-*.*s%*.*s|\n",
    iNameOffset,       iNameOffset,       " ", iNameLen,       iNameLen,       ROOM_NAME,       iNameOffset,       iNameOffset,       " ",
    iDifficultyOffset, iDifficultyOffset, " ", iDifficultyLen, iDifficultyLen, ROOM_DIFFICULTY, iDifficultyOffset, iDifficultyOffset, " ",
    iCapacityOffset,   iCapacityOffset,   " ", iCapacityLen,   iCapacityLen,   ROOM_CAPACITY,   iCapacityOffset,   iCapacityOffset,   " ",
    iStatusOffset,     iStatusOffset,     " ", iStatusLen,     iStatusLen,     ROOM_STATUS,     iStatusOffset,     iStatusOffset,     " "
  );

  /** TODO: Room Lines */

}