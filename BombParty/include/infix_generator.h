#ifndef INFIX_GENERATOR_H
  #define INFIX_GENERATOR_H

  int iRandomBetween(int min, int max, int iSeed);
  void vInfixGeneratorRandom(char *pszInfixBuffer);
  void vInfixGeneratorBase(char *pszInfixBuffer);
  void vInfixGeneratorDb(char *pszInfixBuffer, int iInfixBufferSize);
#endif