#include <string.h>

#include "cm_binary.h"

/*
 * if the piLen == NULL, then don't care the iLen
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
 * 32bit system :Word mean 16 bit
 * 64bit system :Word mean 32 bit
 * so use unsigned short
 * use network order
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
 * 32bit
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
 * 64bit
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
