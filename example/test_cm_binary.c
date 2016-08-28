#include <stdio.h>
#include <memory.h>

#include "cm_binary.h"
#include "cm_debug.h"

#define BUF_LEN 100
int main()
{

	char sBuf[BUF_LEN];
	int iLen = sizeof(sBuf);
	char *pCur;

	signed short shValue = -1;
	signed short shValue2 = -2;
	uint16_t wValue = 2;
	uint16_t wValue2 = 64;

	CM_INT32 iValue = -2;
	uint32_t dwValue = 64;

	long long lValue = -2;
	uint64_t qwValue = 64;

	char sData1[] = "123456";
	char sData2[] = "7654321";


	memset(sBuf, 0, sizeof(sBuf));
	pCur = sBuf;
	iLen = sizeof(sBuf);

	//AddChar
	if (AddChar(&pCur, &iLen, 'A') < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddChar(&pCur, &iLen, 'D') < 0) {printf("error line(%d)", __LINE__); return -1;}

	//AddWord
	if (AddWord(&pCur, &iLen, shValue) <0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddWord(&pCur, &iLen, shValue2) <0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddWord(&pCur, &iLen, wValue) <0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddWord(&pCur, &iLen, wValue2) <0) {printf("error line(%d)", __LINE__); return -1;}


	//AddDWord
	if (AddDWord(&pCur, &iLen, iValue) <0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddDWord(&pCur, &iLen, dwValue) <0) {printf("error line(%d)", __LINE__); return -1;}

	//AddQWord
	if (AddQWord(&pCur, &iLen, lValue) <0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddQWord(&pCur, &iLen, qwValue) <0) {printf("error line(%d)", __LINE__); return -1;}

	//AddBuffer
	//sizeof(sData1) - 1   mean remove the '\0'
	if (AddBuffer(&pCur, &iLen, sData1, sizeof(sData1) - 1) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (AddBuffer(&pCur, &iLen, sData2, sizeof(sData2) - 1) < 0) {printf("error line(%d)", __LINE__); return -1;}
	

	printf("sBuf(%p) pCur(%p) iLen(%d) total size(%ld) BUF_LEN(%d)\n", sBuf, pCur, iLen, pCur - sBuf, BUF_LEN);
	printf("dump:\n%s\n", DumpPackage(sBuf, sizeof(sBuf)));
	printf("dump_RAW:\n%s\n", DumpHex(sBuf, sizeof(sBuf)));

	char cValue;
	pCur = sBuf;
	iLen = sizeof(sBuf);
	shValue = shValue2 = 0;
	wValue = wValue2 = 0;

	iValue = 0;
	dwValue = 0;

	lValue = 0;
	qwValue = 0;

	char sGetData1[100], sGetData2[100];
	CM_INT32 iDate1Len = sizeof(sGetData1), iDate2Len = sizeof(sGetData2);


	//GetChar
	if (GetChar(&pCur, &iLen, &cValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetChar : %c\n", cValue);
	if (GetChar(&pCur, &iLen, &cValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetChar : %c\n", cValue);

	//GetWord
	if (GetWord(&pCur, &iLen, (unsigned short *)&shValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetWord(&pCur, &iLen, (unsigned short *)&shValue2) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetWord(&pCur, &iLen, &wValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetWord(&pCur, &iLen, &wValue2) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetWord:shValue(%d) shValue2(%d) wValue(%u) wValue2(%u)\n", shValue, shValue2, wValue, wValue2);

	//GetDWord
	if (GetDWord(&pCur, &iLen, (unsigned CM_INT32 *)&iValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetDWord(&pCur, &iLen, &dwValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetDword:iValue(%d) dwValue(%u)\n", iValue, dwValue);

	//GetQWord
	if (GetQWord(&pCur, &iLen, (uint64_t *)&lValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetQWord(&pCur, &iLen, &qwValue) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetQWord:lValue(%lld) qwValue(%llu)\n", lValue, qwValue);

	//GetBuffer
	if (GetBuffer(&pCur, &iLen, sGetData1, (unsigned CM_INT32 *)&iDate1Len) < 0) {printf("error line(%d)", __LINE__); return -1;}
	if (GetBuffer(&pCur, &iLen, sGetData2, (unsigned CM_INT32 *)&iDate2Len) < 0) {printf("error line(%d)", __LINE__); return -1;}
	printf("GetBuffer:data1(%d):(%s) data2(%d):(%s)\n", iDate1Len, sGetData1, iDate2Len, sGetData2);

	//GetBufferPtr
	/*
	char *pBuffer = NULL;
	CM_INT32 iBufferLen = 0;
	if (GetBufferPtr(&pCur, &iLen, &pBuffer, &iBufferLen) < 0) {printf("error line(%d)", __LINE__); return -1;}
	*/

	printf("sBuf(%p) pCur(%p) iLen(%d) BUF_LEN(%d)\n", sBuf, pCur, iLen, BUF_LEN);
	printf("dump:\n%s\n", DumpPackage(sBuf, sizeof(sBuf)));
	printf("dump_RAW:\n%s\n", DumpHex(sBuf, sizeof(sBuf)));


	return 0;
}
