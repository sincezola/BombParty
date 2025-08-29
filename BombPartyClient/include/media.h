#ifndef MEDIA_H
  #define MEDIA_H
  
  extern int giIsPlaying = 0;
  int iAudioInit();
  int iAudioPlayTheme(const char *kpszPath, int iLoop);
  void vAudioStop();
  void vAudioShutdown();

#endif