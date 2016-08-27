#include <stdio.h>

#include "cm_debug.h"
#define PKG_LEN (65535 * 4)
/*
 * printf the pPkg with hex
 *
 * example:
 *
 * printf("dump:\n%s\n", DumpPackage(sBuf, sizeof(sBuf)));
 *
 * dump:
 * 0000: 68 65 6C 6C - 6F 20 77 6F | 72 6C 64 21 - 21 21 20 20
 * 0010: 31 32 33 34 - 35 36 37 38 | 39 31 30 00 - 
 */
const char * DumpPackage(const void *pPkg, int iPkgLen)
{
	static char sBuf[PKG_LEN];
	char *pDst = sBuf;
	const char *pSrc = (const char *)pPkg;
	int i, inc;

	for (i = 0; i < iPkgLen; i++, pSrc++) {
		char const *sSep;
		char sAddr[20];
		//each word's separator
		switch (i%16) {
			case 3:
			case 11:
				sSep = " - ";
				break;
			case 7:
				sSep = " | ";
				break;
			case 15:
				sSep = "\n";
				break;
			default:
				sSep = " ";
				break;
		}

		//each line's fist Addr
		switch (i%16) {
			case 0:
				snprintf(sAddr, sizeof(sAddr), "%04hX: ", (unsigned short)i);
				break;
			default:
				sAddr[0] = '\0';
				break;
		}

		inc  = snprintf(pDst, sBuf + sizeof(sBuf) - pDst, "%s%02X%s", sAddr, (unsigned char) *pSrc, sSep);
		if (inc < 0)
			break;

		pDst += inc;
		if (pDst >= sBuf + sizeof(sBuf))
			break;
	}
	return sBuf;
}
