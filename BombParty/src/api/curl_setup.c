#include <curl/curl.h>
#include <string.h>

size_t ulWriteCallback(void *vContents, size_t ulSize, size_t ulNMemb, void *vUserp) {
  size_t ulTotalSize = ulSize * ulNMemb;
  strcat(vUserp, vContents);
  return ulTotalSize;
}

int iCurlReq(char *pszUrl, char *pszEndPoint, char *pszMethod, char *szPayload, int iPayloadLen, char *szRsl) {
  CURL *curl;
  CURLcode curlRes;
  char szUrl[1024];

  sprintf(szUrl, "%s/%s", pszUrl, pszEndPoint);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, szUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ulWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, szRsl);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, pszMethod);
    if ( !strcmp(pszMethod, "POST") ){
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, iPayloadLen);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPayload);
    }
    curlRes = curl_easy_perform(curl);

    if (curlRes != CURLE_OK){
      return -1;
    }

    curl_easy_cleanup(curl);
  } else {
    return -1;
  }

  return 0;
}