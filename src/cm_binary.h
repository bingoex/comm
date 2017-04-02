#ifndef _CM_BINARY_H_
#define _CM_BINARY_H_



#include <stdint.h> 
#include "cm_type.h"

/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

/*
 * 参数piLen可以为空值，如果为空，函数将不会校验长度的合法值
 */
int AddChar(char **p, int *piLen, char cValue);
int GetChar(char **p, int *piLen, char *cValue);

/*
 * 16位
 * 超过1字节长度的值，统一存为网络序，取出时会转为本机序
 */
int AddWord(char **p, int *piLen, unsigned short shValue);
int GetWord(char **p, int *piLen, unsigned short *pshValue);

/*
 * 32位
 */ 
int AddDWord(char **p, int *piLen, unsigned CM_INT32 lValue);
int GetDWord(char **p, int *piLen, unsigned CM_INT32 *plValue);

/*
 * 64位
 */ 
int AddQWord(char **p, int *piLen, uint64_t qwValue);
int GetQWord(char **p, int *piLen, uint64_t *pqwValue);

/*
 * iBufLen上限为32位
 * 内容：BuffLen（网络序） ＋ Buff
 */
int AddBuffer(char **p, int *piLen, const char *pBuf, CM_INT32 iBufLen);
int GetBuffer(char **p, int *piLen, char *pBuf, unsigned CM_INT32 *piBufLen);

/*
 * 不拷贝buf，pBufPtr指向p中的buf
 */
int GetBufferPtr(char **p, int *piLen, char **pBufPtr, CM_INT32 *piBufLen);

/*
 * 只添加Buf,不添加长度
 * 内容：buf
 */
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

/*
 * 64位子节序转换宏，类似htonl\ntohl\htons\ntohs
 */
#define ntoh64 hton64

/*
#ifdef __cplusplus
}
#endif 
*/




#endif
