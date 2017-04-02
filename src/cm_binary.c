#include <string.h> //memcpy
#include <arpa/inet.h> //ntohl

#include "cm_binary.h"

/*
 * 参数piLen可以为空值，如果为空，函数将不会校验长度的合法值
 */
int GetChar(char **p, int *piLen, char *cValue)
{
	int iLen;
	char *pCur;

	pCur = *p;
	if (piLen) 
		iLen = *piLen;
	else
		iLen = sizeof(char);

	if (iLen < (signed int)sizeof(char))
		return -1;

	*cValue = *pCur;

	//forward move 
	pCur += sizeof(char);
	iLen -= sizeof(char);

	*p = pCur;
	if (piLen) *piLen = iLen;

	return 0;
}

int AddChar(char **p, int *piLen, char cValue)
{
	int iLenLeft = sizeof(cValue) + 1;
	char *pCur;

	pCur = *p;
	if (piLen)
		iLenLeft = *piLen;

	if (iLenLeft < sizeof(cValue))
		return -1;

	*pCur = cValue;
	
	pCur += sizeof(cValue);
	iLenLeft -= sizeof(cValue);

	*p = pCur;

	if (piLen) *piLen = iLenLeft;

	return 0;
}

/*
 * 16位
 * 超过1字节长度的值，统一存为网络序，取出时会转为本机序
 */
int AddWord(char **p, int *piLen, unsigned short shValue)
{
	int iLenLeft = sizeof(unsigned short) + 1;
	char *pCur;
	unsigned short shNValue;

	pCur = *p;
	if (piLen) 
		iLenLeft = *piLen;

	if (iLenLeft < (signed int)sizeof(shValue))
		return -1;

	shNValue = htons(shValue);
	memcpy(pCur, &shNValue, sizeof(shNValue));

	pCur += sizeof(shValue);
	iLenLeft -= sizeof(shValue);

	*p = pCur;
	if (piLen)
		*piLen = iLenLeft;

	return 0;
}

int GetWord(char **p, int *piLen, unsigned short *pshValue)
{
	unsigned short shNValue;
	int iLen;
	char *pCur;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else
		iLen = sizeof(shNValue);

	if (iLen < (signed int)sizeof(shNValue))
		return -1;

	memcpy(&shNValue, pCur, sizeof(shNValue));
	*pshValue = ntohs(shNValue);

	pCur += sizeof(shNValue);
	iLen -= sizeof(shNValue);

	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}


/*
 * 32位
 */ 
int AddDWord(char **p, int *piLen, unsigned CM_INT32 lValue)
{
	int iLenLeft = sizeof(unsigned CM_INT32) + 1;
	char *pCur;
	unsigned CM_INT32 lNValue;

	pCur = *p;
	if (piLen)
		iLenLeft = *piLen;

	if (iLenLeft < (signed int)sizeof(lValue))
		return -1;

	lNValue = htonl(lValue);
	memcpy(pCur, &lNValue, sizeof(lNValue));

	pCur += sizeof(lValue);
	iLenLeft -= sizeof(lValue);

	*p = pCur;
	if (piLen)
		*piLen = iLenLeft;

	return 0;
}

int GetDWord(char **p, int *piLen, unsigned CM_INT32 *plValue)
{
	unsigned CM_INT32 lNValue;
	int iLen;
	char *pCur;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else
		iLen = sizeof(CM_INT32);

	if(iLen < (signed int)sizeof(lNValue))
		return -1;

	memcpy(&lNValue, pCur, sizeof(lNValue));
	*plValue = ntohl(lNValue);

	pCur += sizeof(lNValue);
	iLen -= sizeof(lNValue);

	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}


/*
 * 64位
 */ 
int AddQWord(char **p, int *piLen, uint64_t qwValue)
{
	int iLenLeft = sizeof(uint64_t) + 1;
	char *pCur;
	uint64_t qwNValue;

	pCur = *p;
	if (piLen)
		iLenLeft = *piLen;

	if (iLenLeft < (signed int)sizeof(qwValue))
		return -1;

	qwNValue = hton64(qwValue);
	memcpy(pCur, &qwNValue, sizeof(qwNValue));

	pCur += sizeof(qwValue);
	iLenLeft -= sizeof(qwValue);

	*p = pCur;
	if (piLen)
		*piLen = iLenLeft;

	return 0;
}

int GetQWord(char **p, int *piLen, uint64_t *pqwValue)
{
	uint64_t qwNValue;
	int iLen;
	char *pCur;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else
		iLen = sizeof(CM_INT32);

	if(iLen < (signed int)sizeof(qwNValue))
		return -1;

	memcpy(&qwNValue, pCur, sizeof(qwNValue));
	*pqwValue = ntoh64(qwNValue);

	pCur += sizeof(qwNValue);
	iLen -= sizeof(qwNValue);

	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}

/*
 * iBufLen上限为32位
 * 内容：BuffLen（网络序） ＋ Buff
 */
#define MAX_BUFFER_LEN (1024 * 1024)
int AddBuffer(char **p, int *piLen, const char *pBuf, CM_INT32 iBufLen)
{
	int iLen = MAX_BUFFER_LEN;
	char *pCur;
	
	//add the len
	if (AddDWord(p, piLen, iBufLen) < 0)
		return -1;

	pCur = *p;
	if (piLen)
		iLen = *piLen;

	if (iLen < iBufLen)
		return -2;

	//add the really buffer data
	memcpy(pCur, pBuf, iBufLen);
	pCur += iBufLen;
	iLen -= iBufLen;

	*p = pCur;

	if (piLen)
		*piLen = iLen;

	return 0;
}

/*
 * piBufLen max bigger then actually buffer data len
 */ 
int GetBuffer(char **p, int *piLen, char *pBuf, unsigned CM_INT32 *piBufLen)
{
	int iLen;
	char *pCur;
	unsigned CM_INT32 iCopyLen, iGetLen;
	unsigned CM_INT32 iBufLen;

	iBufLen = *piBufLen;

	if (iBufLen <= 0)
		return -1;

	//get the buffer len
	if (GetDWord(p, piLen, &iGetLen) < 0)
		return -2;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else 
		iLen = iBufLen;

	if (iGetLen > iLen)
		return -3;

	if (iBufLen >= iGetLen)
		iCopyLen = iGetLen; // general the iBufLen bigger then iGetLen
	else
		return -5;
		//iCopyLen = iBufLen - 1;

	memcpy(pBuf, pCur, iCopyLen);
	//*(pBuf + iCopyLen) = 0;

	pCur += iCopyLen;
	iLen -= iCopyLen;
	
	*p = pCur;
	*piBufLen = iCopyLen;
	if (piLen)
		*piLen = iLen;

	return 0;
}

/*
 * 不拷贝buf，pBufPtr指向p中的buf
 */
int GetBufferPtr(char **p, int *piLen, char **pBufPtr, CM_INT32 *piBufLen)
{
	unsigned CM_INT32 iGetLen;
	int iLen;
	char *pCur;

	if (GetDWord(p, piLen, &iGetLen) < 0)
		return -1;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else 
		iLen = iGetLen;

	if (iGetLen > iLen)
		return -2;

	if (pBufPtr)
		*pBufPtr = pCur;

	if (piBufLen)
		*piBufLen = iGetLen;

	pCur += iGetLen;
	iLen -= iGetLen;

	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}


/*
 * 只添加Buf,不添加长度
 * 内容：buf
 */
int AddBufferNoLen(char **p, int *piLen, const char *pBuf, CM_INT32 iBufLen)
{
	int iLen = MAX_BUFFER_LEN;
	char *pCur;

	pCur = *p;

	if (piLen)
		iLen = *piLen;
	
	if (iLen < iBufLen)
		return -1;

	memcpy(pCur, pBuf, iBufLen);
	
	pCur += iBufLen;
	iLen -= iBufLen;

	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}


int GetBufferNoLen(char **p, int *piLen, char *pBuf, CM_INT32 iBufLen)
{
	int iLen;
	char *pCur;

	if (iBufLen <= 0)
		return -1;

	pCur = *p;
	if (piLen)
		iLen = *piLen;
	else 
		return -2;

	if (iBufLen > iLen)
		return -3;

	memcpy(pBuf, pCur, iBufLen);

	pCur += iBufLen;
	iLen -= iBufLen;
	
	*p = pCur;
	if (piLen)
		*piLen = iLen;

	return 0;
}

/*
#include<stdio.h> //printf
#include "cm_debug.h"
int main() 
{
	char buf[1024];
	int len = sizeof(buf);
	char c;
	char sKeyBuf[512] = {'A', 'B', 'C'};
	uint32_t iKeyLen = sizeof(sKeyBuf) - 1;
	uint32_t dwLen;

	char *p = buf;

	// 封包
	if (AddChar(&p, &len, 'D')) { printf("AddChar error\n"); return -1; }
	if (AddDWord(&p, &len, 65538)) { printf("AddDWord error\n"); return -1; }
	if (AddBuffer(&p, &len, sKeyBuf, iKeyLen)) { printf("AddBuffer error\n"); return -1; }
	printf("remainlen %d size %ld iKeyLen %u \n", len, p - buf, iKeyLen);
	printf("===================================================================\n");


	// 解包
	p = buf;
	len = sizeof(buf);
	memset(sKeyBuf, 0, sizeof(sKeyBuf));
	if (GetChar(&p, &len, &c)) { printf("GetChar error\n"); return -1; }
	if (GetDWord(&p, &len, &dwLen)) { printf("GetDWord error\n"); return -1; }
	if (GetBuffer(&p, &len, sKeyBuf, &iKeyLen)) { printf("GetBuffer error\n"); return -1; }
	printf("remainlen %d size %ld\n", len, p - buf);
	printf("c %c\n", c);
	printf("dwLen %u\n", dwLen);
	printf("iKeyLen %d sKeyBuf\n%s\n", iKeyLen, DumpPackage(sKeyBuf, iKeyLen));
}
*/
