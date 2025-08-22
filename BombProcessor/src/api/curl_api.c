  #include <curl/curl.h>
#include <string.h>
#include <sys_interface.h>
#include <trace.h>

size_t ulWriteCallback(void *vContents, size_t ulSize, size_t ulNMemb, void *vUserp) {
  size_t ulTotalSize = ulSize * ulNMemb;
  strncat((char *)vUserp, (char *)vContents, ulTotalSize);
  return ulTotalSize;
}

int iCurlReq(char *pszUrl, char *pszEndPoint, char *pszMethod, char *szPayload, int iPayloadLen, char *szRsl) {
  CURL *curl;
  CURLcode curlRes;
  struct curl_slist *headers = NULL;
  char szUrl[2048];
  int bUseSSL = FALSE;

  if ( bStrIsEmpty(pszUrl) )
    return -3;

  if ( !strcmp(pszMethod, "PATCH") && iPayloadLen <= 0 )
    return -4;

  memset(szUrl, 0, sizeof(szUrl));
  snprintf(szUrl, sizeof(szUrl), "%s/%s", pszUrl, pszEndPoint);

  if (strstr(szUrl, "https:") != NULL) {
    bUseSSL = TRUE;
  }

  vTraceVarArgsFn("URL [%s]\n", szUrl);


  curl_global_init(CURL_GLOBAL_ALL); 

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, szUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ulWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, szRsl);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30000);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    if (bUseSSL) {
      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    }

    if ( iPayloadLen > 0 ) { /** PATCH OR POST */
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, iPayloadLen);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPayload);
    }
    if (!strcmp(pszMethod, "POST")) {
      if ( iPayloadLen <= 0 )
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

      curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else if (!strcmp(pszMethod, "GET")) {
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    } else if (!strcmp(pszMethod, "DELETE") || !strcmp(pszMethod, "PATCH")) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, pszMethod);
    }

    curlRes = curl_easy_perform(curl);

    if (curlRes != CURLE_OK) {
      vTraceVarArgsFn("Error CURL=[%s]\n", curl_easy_strerror(curlRes));
      curl_easy_cleanup(curl);
      return -1;
    }

    curl_easy_cleanup(curl);
  } else {
    return -2;
  }

  vTraceVarArgsFn("Retorno [%s]\n", szRsl);
  return 0;
}
