#ifndef _CM_SHM_H_
#define _CM_SHM_H_

#include <sys/ipc.h>
#include <sys/shm.h>


#ifdef __cplusplus
extern "C" {
#endif 

void * GetReadOnlyShm(int iKey, int iSize);


/*
 * get shm memory by iKey iSize iFlag(IPC_CREAT | 0666).
 * if the iSize != 0. then the shm size must equal iSize otherwize false.
 * if the iSize == 0. don't check the size
 */
void * GetShm(int iKey, int iSize, int iFlag);


/*
 * pShm point to the shm memory
 *
 * 1.try to get only
 * 2.if step 1 fail and iFlag with IPC_CREAT then new a shm memory and memset to zero
 *
 */ 
int GetShmEx(void **pShm, int iKey, int iSize, int iFlag);


/*
 * get shm .if the size change, then delete the old shm and new one with iSize
 *
 */ 
int GetShmFrocely(void **pstShm, int iKey, int iSize);
int GetShmSize(int iKey);
int DelShm(int iKey);


#ifdef __cplusplus
}
#endif 

#endif

