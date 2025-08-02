#include <curl/curl.h>
#include <string.h>
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

  memset(szUrl, 0, sizeof(szUrl));
  snprintf(szUrl, sizeof(szUrl), "%s/%s", pszUrl, pszEndPoint);

  if (strstr(szUrl, "https:") != NULL) {
    bUseSSL = TRUE;
  }

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, szUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ulWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, szRsl);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if (bUseSSL) {
      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    }

    if (!strcmp(pszMethod, "POST")) {
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, iPayloadLen);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPayload);
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    } else if (!strcmp(pszMethod, "GET")) {
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }

    curlRes = curl_easy_perform(curl);

    if (curlRes != CURLE_OK) {
      vTraceVarArgs("Curl error: %s\n", curl_easy_strerror(curlRes));
      curl_easy_cleanup(curl);
      return -1;
    }

    curl_easy_cleanup(curl);
  } else {
    return -1;
  }

  vTraceVarArgs("Retorno [%s]\n", szRsl);
  return 0;
}
