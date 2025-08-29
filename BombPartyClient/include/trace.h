/**
 * trace.h
 * 
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 * 
 */

#ifndef _TRACE_H_
  #define _TRACE_H_

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/ 
  #include <errno.h>
  #include <stdarg.h>
  #include <config.h>
  #include <stdio.h>

  #ifdef _WIN32
    /* Winsock2 deve vir antes de windows.h e antes de time.h */
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <process.h>
    #include <time.h>
  #elif defined(LINUX)
    #include <unistd.h>
    #include <sys/time.h>
    #include <time.h>
  #else
    #include <time.h>
  #endif
  
/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/
  #define DEBUG_MSGS      giDebugLevel > 0
  #define DEBUG_MORE_MSGS giDebugLevel > 8

  #define vTraceVarArgs(FORMAT, ...) _vTraceVarArgs(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__)
  #define vTraceVarArgsFn(FORMAT, ...) _vTraceVarArgsFn(__FILE__, __LINE__, __func__, FORMAT, ##__VA_ARGS__)
  #define vTraceBegin() vTraceVarArgs("%s - begin", __func__)
  #define vTraceEnd() vTraceVarArgs("%s - end", __func__)

/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern char gszTraceFile[_MAX_PATH+_MAX_PATH];
  extern int giDebugLevel;
  extern char gszConfFile[_MAX_PATH];

/******************************************************************************
 *                                                                            *
 *                                 Prototypes                                 *
 *                                                                            *
 ******************************************************************************/ 

  int iGetDebugLevel( const char *kpszConfFile );

  void vSetLogFile( void );

  void vTraceMsg( char *szMsg );
  void vTracePid( char *szMsg, int iMsgLen );
  void vTraceMsgNoNL( char *szMsg );
  void vInitLogs( void );

  void _vTraceVarArgs( const char *kpszModuleName,
                       const int kiLine,
                       const char *kpszFmt, ... );
  void _vTraceVarArgsFn( char *pszModuleName,
                       const int kiLine,
                       const char *kpszFmt,
                       const char *kpszFunctionName, ... );

  #define TOKEN_MISMATCH -1
  #define ROOT_PATH_FROM_BIN ".."

  extern char *szTokenName[];
#endif /* _TRACE_H */
