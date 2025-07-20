#ifndef INFIX_GENERATOR_H
  #define INFIX_GENERATOR_H

  #define EASY_INFIX   2
  #define MEDIUM_INFIX 3
  #define HARD_INFIX   4
  int iRandomBetween(int min, int max, int iSeed);
  void vInfixGeneratorRandom(char *pszInfixBuffer);
  void vInfixGeneratorBase(char *pszInfixBuffer);
  void vInfixGeneratorDb(char *pszInfixBuffer, int iInfixBufferSize);
#endif