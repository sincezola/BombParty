#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <windows.h>
  #include <mmsystem.h>
#else
  #include <unistd.h>
  #include <signal.h>
#endif

/* ========================================================================
 * Windows Implementation
 * ======================================================================== */
#ifdef _WIN32

static int giIsPlaying = 0;

int iAudioInit() {
  giIsPlaying = 0;
  return 0;
}

int iAudioPlayTheme(const char *kpszPath, int iLoop) {
  UINT uiFlags;
  int iOk;

  if (kpszPath == NULL)
    return -1;

  uiFlags = SND_FILENAME | SND_ASYNC;
  if (iLoop)
    uiFlags |= SND_LOOP;

  iOk = PlaySoundA(kpszPath, NULL, uiFlags) ? 0 : -1;
  if (iOk == 0)
    giIsPlaying = 1;

  return iOk;
}

void vAudioStop() {
  if (giIsPlaying) {
    PlaySound(NULL, 0, 0);
    giIsPlaying = 0;
  }
}

void vAudioShutdown() {
  vAudioStop();
}

#endif /* _WIN32 */


/* ========================================================================
 * Linux / Unix Implementation
 * ======================================================================== */
#ifndef _WIN32

static int giAudioPid = -1;

static bool bCmdExists(const char *kpszCmd) {
  char szBuf[256];
  FILE *pf;
  int iLen;

  if (kpszCmd == NULL)
    return FALSE;

  char szCheck[256];
  snprintf(szCheck, sizeof(szCheck), "command -v %s 2>/dev/null", kpszCmd);

  pf = popen(szCheck, "r");
  if (pf != NULL) {
    iLen = (int)fread(szBuf, 1, sizeof(szBuf) - 1, pf);
    pclose(pf);
    if (iLen > 0)
      return TRUE;
  }

  return FALSE;
}

static int iRunBgCmdReadPid(const char *kpszCmd) {
  FILE *pf;
  char szLine[64];
  int iPid;
  int iRead;

  pf = popen(kpszCmd, "r");
  if (pf == NULL)
    return -1;

  memset(szLine, 0, sizeof(szLine));
  iRead = fscanf(pf, "%63s", szLine);
  pclose(pf);

  if (iRead != 1)
    return -1;

  iPid = atoi(szLine);
  if (iPid <= 0)
    return -1;

  return iPid;
}

int iAudioInit() {
  giAudioPid = -1;
  return 0;
}

int iAudioPlayTheme(const char *kpszPath, int iLoop) {
  char szCmd[1024];
  int iPid;
  int iLen;

  if (kpszPath == NULL)
    return -1;

  /* Prioridade: ffplay -> aplay -> paplay */
  if (bCmdExists("ffplay")) {
    if (iLoop)
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"ffplay -nodisp -autoexit -loglevel quiet -loop 0 '%s' >/dev/null 2>&1 & echo $!\"",
                      kpszPath);
    else
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"ffplay -nodisp -autoexit -loglevel quiet '%s' >/dev/null 2>&1 & echo $!\"",
                      kpszPath);
  } else if (bCmdExists("aplay")) {
    if (iLoop)
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"while true; do aplay '%s' >/dev/null 2>&1; done & echo $!\"",
                      kpszPath);
    else
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"aplay '%s' >/dev/null 2>&1 & echo $!\"",
                      kpszPath);
  } else if (bCmdExists("paplay")) {
    if (iLoop)
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"while true; do paplay '%s' >/dev/null 2>&1; done & echo $!\"",
                      kpszPath);
    else
      iLen = snprintf(szCmd, sizeof(szCmd),
                      "sh -c \"paplay '%s' >/dev/null 2>&1 & echo $!\"",
                      kpszPath);
  } else {
    return -1; /* nenhum player encontrado */
  }

  if (iLen <= 0 || iLen >= (int)sizeof(szCmd))
    return -1;

  iPid = iRunBgCmdReadPid(szCmd);
  if (iPid <= 0)
    return -1;

  giAudioPid = iPid;
  return 0;
}

void vAudioStop() {
  int iPid;

  if (giAudioPid > 0) {
    iPid = giAudioPid;
    giAudioPid = -1;
    kill(iPid, SIGTERM);
  }
}

void vAudioShutdown() {
  vAudioStop();
}

#endif /* ! _WIN32 */
