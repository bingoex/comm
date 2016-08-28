#ifndef _CM_DEBUG_H_
#define _CM_DEBUG_H_



#ifdef __cplusplus
extern "C" {
#endif 

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
const char * DumpPackage(const void *pPkg, int iPkgLen);

/*
 * just print the raw hex data
 */
const char * DumpHex(const void *pMem, int iLen);
#ifdef __cplusplus
}
#endif 


#endif
