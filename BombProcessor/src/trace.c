/**
 * trace.c
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 */

#include <stdlib.h>
#include <string.h>
#include <trace.h>
#include <sys_interface.h>

#ifdef LINUX
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#else
#include <windows.h>
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif

char gszTraceFile[_MAX_PATH + _MAX_PATH];
int giDebugLevel = 0;
char gszConfFile[_MAX_PATH];

int giNoNL = FALSE;

#ifdef _WIN32

int gettimeofday(struct timeval *tp, void *tzp) {
    FILETIME ft;
    ULARGE_INTEGER li;
    unsigned __int64 t;
    
    if ( tzp == NULL ){}

    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    t = li.QuadPart - 116444736000000000ULL; // converte de 1601 para 1970
    tp->tv_sec = (long)(t / 10000000ULL);
    tp->tv_usec = (long)((t % 10000000ULL) / 10);
    return 0;
}
#endif

void vTraceMsgNoNL(char *szMsg) {
  giNoNL = TRUE;
  vTraceMsg(szMsg);
  giNoNL = FALSE;
} /* vTraceMsgNoNL */

void vTraceMsg(char *szMsg) {
  FILE *pfLog;
  char szDateTimeNow_us[128];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDateTimeNow_us, 0, sizeof(szDateTimeNow_us));

  snprintf(szDateTimeNow_us, sizeof(szDateTimeNow_us),
           "[%02d/%02d/%04d %02d:%02d:%02d.%03ld] ", (int)st_tm_Now->tm_mday,
           (int)st_tm_Now->tm_mon + 1, (int)st_tm_Now->tm_year + 1900,
           (int)st_tm_Now->tm_hour, (int)st_tm_Now->tm_min,
           (int)st_tm_Now->tm_sec, (long)tv.tv_usec / 1000);

  if ((pfLog = fopen(gszTraceFile, "a+")) == NULL){
      return;
  }
  if (giNoNL == TRUE)
    fprintf(pfLog, "%s", szMsg);
  else
    fprintf(pfLog, "%s%s\n", szDateTimeNow_us, szMsg);

  fclose(pfLog);
  pfLog = NULL;
} /* vTraceMsg */

void vTracePid(char *szMsg, int iMsgLen) {
  char *pszMyMsg = NULL;
  int iNewMsgLen = iMsgLen + 16;
  int iPid;

  iPid = getpid();

  if ((pszMyMsg = (char *)malloc(iNewMsgLen)) == NULL)
    return;

  memset(pszMyMsg, 0x00, iNewMsgLen);
  sprintf(pszMyMsg, "%d %s", iPid, szMsg);

  vTraceMsg(pszMyMsg);

  free(pszMyMsg);
  pszMyMsg = NULL;
} /* vTracePid */

void _vTraceVarArgsFn(char *pszModuleName, const int kiLine, const char *kpszFunctionName,
                    const char *kpszFmt, ...) {
  va_list args;
  FILE *pfLog = NULL;
  char szPath[_MAX_PATH + _MAX_PATH + 8];
  char szName[_MAX_PATH];
  char szExt[_MAX_PATH];
  char szDbg[2048];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDbg, 0x00, sizeof(szDbg));
  memset(szPath, 0x00, sizeof(szPath));
  memset(szName, 0x00, sizeof(szName));
  memset(szExt, 0x00, sizeof(szExt));

  iDIR_SplitFilename(gszTraceFile, szPath, szName, szExt);

  snprintf(szPath, sizeof(szPath), "%s/log", ROOT_PATH_FROM_BIN);

  if (!iDIR_IsDir(szPath)) {
    if (!iDIR_MkDir(szPath)) {
      fprintf(stderr, "E: Impossible create dir %s!\n"
              "%s\n",
              szPath, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
  sprintf(gszTraceFile, "%s/%s%s",szPath,szName,szExt);
  if ((pfLog = fopen(gszTraceFile, "a+")) == NULL) {
    fprintf(stderr, "E: Impossible create or open file %s!\n"
            "%s\n",
            gszTraceFile, strerror(errno));
    exit(EXIT_FAILURE);
  }

  va_start(args, kpszFmt);

  iDIR_SplitFilename(pszModuleName, szPath, szName, szExt);
  snprintf(szDbg, sizeof(szDbg), "[%02d/%02d/%02d %02d:%02d:%02d.%03ld]<%s%s:%d>(%s) - ",
           (int)st_tm_Now->tm_mday, (int)st_tm_Now->tm_mon + 1,
           (int)st_tm_Now->tm_year - 100, (int)st_tm_Now->tm_hour,
           (int)st_tm_Now->tm_min, (int)st_tm_Now->tm_sec,
           (long)tv.tv_usec / 1000, szName, szExt, kiLine, kpszFunctionName);

  strcat(szDbg, kpszFmt);
  strcat(szDbg, "\n");
  vfprintf(pfLog, szDbg, args);

  va_end(args);

  fclose(pfLog);
  pfLog = NULL;
} /* _vTraceVarArgs */

void _vTraceVarArgs(const char *kpszModuleName, const int kiLine,
                    const char *kpszFmt, ...) {
  va_list args;
  FILE *pfLog = NULL;
  char szPath[_MAX_PATH + _MAX_PATH + 8];
  char szName[_MAX_PATH];
  char szExt[_MAX_PATH];
  char szDbg[2048];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDbg, 0x00, sizeof(szDbg));
  memset(szPath, 0x00, sizeof(szPath));
  memset(szName, 0x00, sizeof(szName));
  memset(szExt, 0x00, sizeof(szExt));

  iDIR_SplitFilename(gszTraceFile, szPath, szName, szExt);

  snprintf(szPath, sizeof(szPath), "%s/log", ROOT_PATH_FROM_BIN);

  if (!iDIR_IsDir(szPath)) {
    if (!iDIR_MkDir(szPath)) {
      fprintf(stderr, "E: Impossible create dir %s!\n"
              "%s\n",
              szPath, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
  sprintf(gszTraceFile, "%s/%s%s",szPath,szName,szExt);
  if ((pfLog = fopen(gszTraceFile, "a+")) == NULL) {
    fprintf(stderr, "E: Impossible create or open file %s!\n"
            "%s\n",
            gszTraceFile, strerror(errno));
    exit(EXIT_FAILURE);
  }

  va_start(args, kpszFmt);

  snprintf(szDbg, sizeof(szDbg), "[%02d/%02d/%04d %02d:%02d:%02d.%03ld] %s:%d ",
           (int)st_tm_Now->tm_mday, (int)st_tm_Now->tm_mon + 1,
           (int)st_tm_Now->tm_year + 1900, (int)st_tm_Now->tm_hour,
           (int)st_tm_Now->tm_min, (int)st_tm_Now->tm_sec,
           (long)tv.tv_usec / 1000, kpszModuleName, kiLine);

  strcat(szDbg, kpszFmt);
  strcat(szDbg, "\n");
  vfprintf(pfLog, szDbg, args);

  va_end(args);

  fclose(pfLog);
  pfLog = NULL;
} /* _vTraceVarArgs */

int iGetDebugLevel(const char *kpszConfFile) {
  FILE *fpConfFile = NULL;
  int iDebugLevel = 0;
  char szLine[256];
  int iLineLen = sizeof(szLine);
  int bFoundDebugLevel = FALSE;
  char *pTok = NULL;
  int ii = 0;
  char szLevel[2] = "";

  memset(szLine, 0, iLineLen);
  memset(szLevel, 0, sizeof(szLevel));

  if ((fpConfFile = fopen(kpszConfFile, "r")) == NULL) {
    fprintf(stderr, "E: %s %s", kpszConfFile, strerror(errno));

    return 0;
  }

  /**
   * bacagine - 2023-11-10 - Reading the .conf file
   */
  while (fgets(szLine, iLineLen, fpConfFile) != NULL) {
    /**
     * Ignore commented lines
     */
    if (strstr(szLine, "#"))
      continue;

    pTok = strtok(szLine, " = ");

    if (strstr(szLine, "DEBUG_LEVEL")) {
      bFoundDebugLevel = TRUE;

      /**
       * bacagine - 2023-11-10 - Getting the value after '='
       * symbom and converting form string to char.
       */
      while (pTok != NULL) {
        if (ii == 1) {
          /**
           * bacagine - 2023-11-10 - Verify if have value after '=' symbol.
           */
          if (bStrIsEmpty(pTok)) {
            fprintf(stderr, "E: DEBUG_LEVEL is empty in file %s!\n",
                    kpszConfFile);

            fclose(fpConfFile);
            fpConfFile = NULL;

            return 0;
          }

          snprintf(szLevel, sizeof(szLevel), "%s", pTok);
          iDebugLevel = atoi(szLevel);

          break;
        }

        pTok = strtok(NULL, " = ");

        ii++;
      } /* while pTok */

      break;
    } else {
      continue;
    }
  } /* while fgets */

  /**
   * If don't found the DEBUG_LEVEL
   * variable in file print a error
   * message to user
   */
  if (bFoundDebugLevel == FALSE) {
    fprintf(stderr, "E: Not found variable DEBUG_LEVEL in file %s!\n",
            kpszConfFile);

    fclose(fpConfFile);
    fpConfFile = NULL;

    return 0;
  }

  /**
   * If DEBUG_LEVEL in file is incorret
   */
  if (iDebugLevel < 0 || iDebugLevel > 9) {
    fprintf(stderr, "E: Invalid value of log level in file %s!\n",
            kpszConfFile);

    fclose(fpConfFile);
    fpConfFile = NULL;

    return 0;
  }

  fclose(fpConfFile);
  fpConfFile = NULL;

  return iDebugLevel;
} /* iGetDebugLevel */

void vSetLogFile(void) {
  sprintf(gszTraceFile, "%s", "bombprocessor.log");
} /* vSetLogFile */

void vInitLogs(void) {
  memset(gszTraceFile, 0, sizeof(gszTraceFile));

  vSetLogFile();

} /* vInitLogs */
