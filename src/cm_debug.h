#ifndef _CM_DEBUG_H_
#define _CM_DEBUG_H_



/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

/*
 * 以16进制格式化输出buf的内容
 * 注意！
 *	1、因使用static变量，导致线程不安全
 *	2、不要在同一个输出函数如printf中调用该函数两次
 *			printf("dump:\n%s\n%s", DumpPackage(sBuf, sizeof(sBuf)), DumpPackage(sBuf, sizeof(sBuf)));// 错误演示
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
 * 无格式化
 */
const char * DumpHex(const void *pMem, int iLen);
/*
#ifdef __cplusplus
}
#endif 
*/



#endif
