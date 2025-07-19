#include <curl_api.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>

// #define DIC_API_URL  "https://api.dicionario-aberto.net"
// #define WORD_ENDPOINT "word"
// #define INFIX_ENDPOINT "infix"

// int bDICION_BrowseWord(char *pszWord) {    
//     char *pszFullEndPoint;
//     char szRsl[_MAX_RSL_BUFFER];
    
//     pszFullEndPoint = (char *)malloc((strlen(pszWord) + strlen(WORD_ENDPOINT) + 2));
//     if ( pszFullEndPoint == NULL )
//         return FALSE; 

//     memset(pszFullEndPoint, 0, (strlen(pszWord) + strlen(WORD_ENDPOINT) + 2));
//     memset(szRsl,0,sizeof(szRsl));
//     sprintf(pszFullEndPoint, "%s/%s", WORD_ENDPOINT, pszWord);
    
//     if ( iCurlReq(DIC_API_URL, pszFullEndPoint, "GET", NULL, 0, szRsl) < 0 ) 
//         return FALSE;

//     if ( !strcmp(szRsl, "[]") )
//         return FALSE;

//     return TRUE;
// }

// char *pszDICION_BrowseInfix(char *pszInfix) {    
//     char *pszRsl;
//     char *pszFullEndPoint;
//     char szRsl[_MAX_RSL_BUFFER];
    
//     pszFullEndPoint = (char *)malloc((strlen(pszInfix) + strlen(INFIX_ENDPOINT) + 2));
//     if ( pszFullEndPoint == NULL )
//         return NULL; 

//     memset(pszFullEndPoint, 0, (strlen(pszInfix) + strlen(INFIX_ENDPOINT) + 2));
//     memset(szRsl, 0, sizeof(szRsl));
//     sprintf(pszFullEndPoint, "%s/%s", INFIX_ENDPOINT, pszInfix);
    
//     if ( iCurlReq(DIC_API_URL, pszFullEndPoint, "GET", NULL, 0, szRsl) < 0 ) {
//         free(pszFullEndPoint);
//         return NULL;
//     }
//     if ( !strcmp(szRsl, "[]") ) {
//         free(pszFullEndPoint);
//         return NULL;
//     }
//     pszRsl = (char *)malloc(strlen(szRsl) + 2 );

//     memset(pszRsl, 0, strlen(szRsl)+2);

//     strcpy(pszRsl, szRsl);
//     free(pszFullEndPoint);
//     printf("%s\n", szRsl);
//     return pszRsl;
// }