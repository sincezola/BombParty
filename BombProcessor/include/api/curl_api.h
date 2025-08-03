#ifndef CURL_API_H
  #define CURL_API_H
  #define _MAX_RSL_BUFFER 904800
  #define METHOD_POST   "POST"
  #define METHOD_GET    "GET"
  #define METHOD_PATCH  "PATCH"
  #define METHOD_DELETE "DELETE"
  int iCurlReq(char *pszUrl, char *pszEndPoint, char *pszMethod, char *szPayload, int iPayloadLen, char *szRsl);
#endif