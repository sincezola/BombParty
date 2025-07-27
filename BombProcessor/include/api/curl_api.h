#ifndef CURL_API_H
  #define CURL_API_H
  #define _MAX_RSL_BUFFER 904800
  int iCurlReq(char *pszUrl, char *pszEndPoint, char *pszMethod, char *szPayload, int iPayloadLen, char *szRsl);
#endif