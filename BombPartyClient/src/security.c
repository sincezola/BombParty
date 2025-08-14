#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef struct STRUCT_SHA1_CTX{
  uint32_t ui32H[5];
  uint64_t ui64Len;   
  uint8_t  ui8Buf[64];
  int      iBufferUsed;
} STRUCT_SHA1_CTX;

/* ---------------- Base32 decode (A-Z2-7) ---------------- */
int iBase32(int iCh) {
  if (iCh >= 'A' && iCh <= 'Z') return iCh - 'A';
  if (iCh >= 'a' && iCh <= 'z') return iCh - 'a';
  if (iCh >= '2' && iCh <= '7') return 26 + (iCh - '2');
  return -1;
}

int iBase32Decode(const char *pszIn, uint8_t *ui8Out, int *iOutLen) {
  int iLen = strlen(pszIn);
  uint32_t ui32Buffer = 0; 
  int iBitsLeft = 0;
  int iPos = 0;
  int ii;

  for (ii = 0; ii < iLen; ii++) {
    int iV;
    if (pszIn[ii] == '=') break;
    iV = iBase32((unsigned char)pszIn[ii]);
    if (iV < 0) continue;
    ui32Buffer = (ui32Buffer << 5) | (uint32_t)iV;
    iBitsLeft += 5;
    if (iBitsLeft >= 8) {
      iBitsLeft -= 8;
      if (iPos >= *iOutLen) return -1;
      ui8Out[iPos++] = (uint8_t)((ui32Buffer >> iBitsLeft) & 0xFF);
    }
  }
  *iOutLen = iPos;
  return 0;
}


static uint32_t uiROL32(uint32_t ui32X, int iN){
   return (ui32X<<iN) | (ui32X>>(32-iN)); 
}

static void vSHA1Init(STRUCT_SHA1_CTX *stSHA1Ctx){
  stSHA1Ctx->ui32H[0]=0x67452301; 
  stSHA1Ctx->ui32H[1]=0xEFCDAB89; 
  stSHA1Ctx->ui32H[2]=0x98BADCFE;
  stSHA1Ctx->ui32H[3]=0x10325476; 
  stSHA1Ctx->ui32H[4]=0xC3D2E1F0; 
  stSHA1Ctx->ui64Len=0; 
  stSHA1Ctx->iBufferUsed=0;
}

static void vSHA1Compress(STRUCT_SHA1_CTX *stSHA1Ctx, const uint8_t ui8Block[64]){
  uint32_t ui32W[80];
  int ii;
  uint32_t ui32A;
  uint32_t ui32B;
  uint32_t ui32CCC;
  uint32_t ui32D;
  uint32_t ui32E;

  for (ii=0;ii<16;ii++){
    ui32W[ii] = (ui8Block[4*ii]<<24) | (ui8Block[4*ii+1]<<16) | (ui8Block[4*ii+2]<<8) | ui8Block[4*ii+3];
  }
  for (ii=16;ii<80;ii++){
    ui32W[ii] = uiROL32(ui32W[ii-3]^ui32W[ii-8]^ui32W[ii-14]^ui32W[ii-16],1);
  }
  ui32A=stSHA1Ctx->ui32H[0],
  ui32B=stSHA1Ctx->ui32H[1],
  ui32CCC=stSHA1Ctx->ui32H[2],
  ui32D=stSHA1Ctx->ui32H[3],
  ui32E=stSHA1Ctx->ui32H[4];
  for (ii=0;ii<80;ii++){
    uint32_t ui32F;
    uint32_t ui32K;
    uint32_t ui32Temp;
    if (ii<20){ 
      ui32F=(ui32B&ui32CCC)|((~ui32B)&ui32D);
      ui32K=0x5A827999; 
    }
    else if (ii<40){ 
      ui32F=ui32B^ui32CCC^ui32D; 
      ui32K=0x6ED9EBA1;
    }
    else if (ii<60){ 
      ui32F=(ui32B&ui32CCC)|(ui32B&ui32D)|(ui32CCC&ui32D);
      ui32K=0x8F1BBCDC;
    }
    else { 
      ui32K=ui32B^ui32CCC^ui32D;
      ui32K=0xCA62C1D6;
    }
    ui32Temp = uiROL32(ui32A,5)+ui32F+ui32E+ui32K+ui32W[ii];
    ui32E = ui32D;
    ui32D = ui32CCC; 
    ui32CCC = uiROL32(ui32B,30);
    ui32B = ui32A; 
    ui32A = ui32Temp;
  }
  stSHA1Ctx->ui32H[0]+=ui32A; 
  stSHA1Ctx->ui32H[1]+=ui32B; 
  stSHA1Ctx->ui32H[2]+=ui32CCC;
  stSHA1Ctx->ui32H[3]+=ui32D;
  stSHA1Ctx->ui32H[4]+=ui32E;
}

static void vSHA1Update(STRUCT_SHA1_CTX *stC, const uint8_t *ui8Data, int iLen){
  stC->ui64Len += (uint64_t)iLen*8;
  while (iLen>0){
    int iToCopy = 64 - stC->iBufferUsed;
    if (iToCopy>iLen) iToCopy=iLen;

    memcpy(stC->ui8Buf + stC->iBufferUsed, ui8Data, iToCopy);
    stC->iBufferUsed += iToCopy;
    ui8Data += iToCopy; 
    iLen -= iToCopy;
    if (stC->iBufferUsed==64){ 
      vSHA1Compress(stC,stC->ui8Buf);
      stC->iBufferUsed=0;
    }
  }
}

static void vSHA1Final(STRUCT_SHA1_CTX *stC, uint8_t ui8Out[20]){
  int ii;
  stC->ui8Buf[stC->iBufferUsed++] = 0x80;
  if (stC->iBufferUsed > 56){
    while (stC->iBufferUsed<64) 
      stC->ui8Buf[stC->iBufferUsed++]=0x00;

    vSHA1Compress(stC,stC->ui8Buf);
    stC->iBufferUsed=0;
  }
  while (stC->iBufferUsed<56) 
    stC->ui8Buf[stC->iBufferUsed++]=0x00;
  for (ii=7;ii>=0;ii--)
   stC->ui8Buf[stC->iBufferUsed++] = (uint8_t)((stC->ui64Len>>(ii*8))&0xFF);
  vSHA1Compress(stC,stC->ui8Buf);

  for (ii=0;ii<5;ii++){
    ui8Out[4*ii+0]=(uint8_t)((stC->ui32H[ii]>>24)&0xFF);
    ui8Out[4*ii+1]=(uint8_t)((stC->ui32H[ii]>>16)&0xFF);
    ui8Out[4*ii+2]=(uint8_t)((stC->ui32H[ii]>> 8)&0xFF);
    ui8Out[4*ii+3]=(uint8_t)((stC->ui32H[ii]>> 0)&0xFF);
  }
}

/* ---------------- HMAC-SHA1 ---------------- */
static void vHMAC_SHA1(const uint8_t *ui8Key, int iKeyLen, uint8_t *ui8Msg, int iMsgLen, uint8_t ui8Out[20]){
  uint8_t ui8KIpad[64];
  uint8_t ui8KOpad[64];
  uint8_t ui8Tk[20];
  STRUCT_SHA1_CTX stC;
  STRUCT_SHA1_CTX stT;
  uint8_t ui8Inner[20];
  int ii;

  if (iKeyLen > 64){ 
    vSHA1Init(&stT); 
    vSHA1Update(&stT, ui8Key, iKeyLen); 
    vSHA1Final(&stT, ui8Tk);
    ui8Key = ui8Tk; 
    iKeyLen = 20;
  }
  memset(ui8KIpad, 0x36, 64);
  memset(ui8KOpad, 0x5c, 64);
  for (ii=0;ii<iKeyLen;ii++){
    ui8KIpad[ii]^=ui8Key[ii]; 
    ui8KOpad[ii]^=ui8Key[ii];
  }

  vSHA1Init(&stC); 
  vSHA1Update(&stC, ui8KIpad, 64);
  vSHA1Update(&stC, ui8Msg, iMsgLen);
  vSHA1Final(&stC, ui8Inner);
  vSHA1Init(&stC); 
  vSHA1Update(&stC, ui8KOpad, 64);
  vSHA1Update(&stC, ui8Inner, 20); 
  vSHA1Final(&stC, ui8Out);
}

/* ---------------- HOTP/TOTP helpers ---------------- */
static uint32_t ui32HOTP_Truncate(const uint8_t ui8HMAC[20]){
  int iOffset = ui8HMAC[19] & 0x0F;
  uint32_t ui32BinCode = ((ui8HMAC[iOffset] & 0x7F) << 24) |
                      ((ui8HMAC[iOffset+1] & 0xFF) << 16) |
                      ((ui8HMAC[iOffset+2] & 0xFF) << 8) |
                      ((ui8HMAC[iOffset+3] & 0xFF));
  return ui32BinCode;
}

int iClampDigits(int iD){
  if (iD < 4) iD = 4;
  if (iD > 10) iD = 10;
  return iD;
}

static void vFormatOtp(uint32_t u32Code, int iDigits, char *pszOut){
  uint32_t u32Mod = 1;
  uint32_t u32V;
  int ii;

  for (ii=0;ii<iDigits;ii++)
    u32Mod *= 10;

  u32V = u32Mod % u32Mod;
  for ( ii = iDigits-1; ii>=0; ii-- ){
    pszOut[ii] = '0' + (u32V % 10);
    u32V /= 10;
  }

  pszOut[iDigits] = '\0';
}

/* ---------------- main (TOTP 30s, SHA1) ---------------- */
int iMakeOtpWith(char *pszKey, int iDigits){
  int ii;
  int iRemain;
  char szOtp[16];
  time_t   lNow;
  uint8_t  u8Key[128];
  uint8_t  u8Msg[8];
  uint8_t  u8Mac[20];
  uint32_t u32BinCode;
  uint64_t u64Counter = (uint64_t)(lNow / 30);

  if ( strlen(pszKey) <= 0 )
    return -1;

  iDigits = iClampDigits(pszKey);

  if ( iBase32Decode(pszKey, u8Key, sizeof(u8Key)) != 0 ){
    fprintf(stderr, "Erro: segredo Base32 invalido.\n");
    return -2;
  }

  for ( ii=7; ii>=0; ii--){
    u8Msg[ii] = (uint8_t)(u64Counter & 0xFF);
    u64Counter >>= 8;
  }

  vHMAC_SHA1(u8Key, sizeof(u8Key), u8Msg, 8, u8Mac);
  u32BinCode = ui32HOTP_Truncate(u8Mac);

  vFormatOtp(u32BinCode, iDigits, szOtp);
  printf("%s\n", szOtp);
  
  return 0;
}
