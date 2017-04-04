#ifndef _CM_SHM_H_
#define _CM_SHM_H_



/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

void *GetReadOnlyShm(int iKey);

/*
 * 获取通过iKey iSize iFlag(可选IPC_CREAT、0666)获取共享内存, 并attach
 * 如果iSize != 0，那么获取的共享内存大小必须等于iSize，否则失败
 * 如果iSize == 0，那么不检查iSize
 */
void *GetShm(int iKey, int iSize, int iFlag);

/*
 * 1、尝试Get\Attach方式获取共享内存
 * 2、如果不存在，且iFlag with IPC_CREAT，则新建共享内存
 */ 
int GetShmEx(void **pShm, int iKey, int iSize, int iFlag);

/*
 * 1、尝试获取共享内存，如果成功，则检查大小，如果大小变化了则删除老共享内存，并重新生成
 * 2、如果不存在则新建一个
 * 3、线程安全
 */ 
int GetShmFrocely(void **pstShm, int iKey, int iSize);

/*
 * 获取共享内存大小
 */
int GetShmSize(int iKey);

/*
 * 删除共享内存
 */
int DelShm(int iKey);

/*
 * 按参数获取／新建共享内存，并attach
 */
char *CM_GetShm(int iKey, int iSize, int iFlag);

/*
 * 1、尝试获取共享内存
 * 2、如果不存在则新建一个
 */
int CM_GetShm2(void **pstShm, int iKey, size_t iSize, int iFlag);

/*
#ifdef __cplusplus
}
#endif 
*/



#endif

