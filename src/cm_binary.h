#ifndef _CM_BINARY_H_
#define _CM_BINARY_H_

#include <stdint.h> 
#include <arpa/inet.h>
#include "cm_type.h"

#ifdef __cplusplus
extern "C" {
#endif 

int AddChar(char **p, int *piLen, char cValue);
int GetChar(char **p, int *piLen, char *cValue);

int AddWord(char **p, int *piLen, unsigned short shValue);
int GetWord(char **p, int *piLen, unsigned short *pshValue);

int AddDWord(char **p, int *piLen, unsigned CM_INT32 lValue);
int GetDWord(char **p, int *piLen, unsigned CM_INT32 *plValue);

int AddQWord(char **p, int *piLen, uint64_t qwValue);
int GetQWord(char **p, int *piLen, uint64_t *pqwValue);

int AddBuffer(char **p, int *piLen, const char *pBuf, CM_INT32 iBufLen);
int GetBuffer(char **p, int *piLen, char *pBuf, unsigned CM_INT32 *piBufLen);

int GetBufferPtr(char **p, int *piLen, char **pBufPtr, CM_INT32 *piBufLen);

int AddBufferNoLen(char **p, int *piLen, const char *pBuf, CM_INT32 iBufLen);
int GetBufferNoLen(char **p, int *piLen, char *pBuf, CM_INT32 iBufLen);

static inline uint64_t hton64(uint64_t qwVal)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	return qwVal;
#else
	return bswap_64(qwVal);
#endif
}

#define ntoh64 hton64

#ifdef __cplusplus
}
#endif 

#endif
