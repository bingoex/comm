#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>

#include "cm_shm.h"

#define ERR_MSG_LEN 100

void * GetReadOnlyShm(int iKey)
{
	int iShmId;
	void *sShm;

	static char sErrMsg[ERR_MSG_LEN];

	if ((iShmId = shmget(iKey, 0, 0)) < 0) {
		snprintf(sErrMsg, sizeof(sErrMsg), "shmget fail key(%d)", iKey);
		perror(sErrMsg);
		return NULL;
	}

	//read only map:SHM_RDONLY
	if ((sShm = shmat(iShmId, NULL, SHM_RDONLY)) == (char *)-1) {
		snprintf(sErrMsg, sizeof(sErrMsg), "shmat fail key(%d)", iKey);
		perror(sErrMsg);
		return NULL;
	}

	return sShm;
}

/*
 * get shm memory by iKey iSize iFlag.
 * if the iSize != 0. then the shm size must equal iSize otherwize false.
 * if the iSize == 0. don't check the size
 */
void * GetShm(int iKey, int iSize, int iFlag)
{
	int iShmId;
	void *sShm;
	struct shmid_ds stShmDs;

	static char sErrMsg[ERR_MSG_LEN];

	if ((iShmId = shmget(iKey, iSize, iFlag)) < 0) {
		snprintf(sErrMsg, sizeof(sErrMsg), "shmget fail(%d) key(%d) size(%d) iFlag(%d) error(%d)",
				iShmId, iKey, iSize, iFlag, errno);
		perror(sErrMsg);
		return NULL;
	}

	if ((sShm = shmat(iShmId, NULL, 0)) == (char *)-1) {
		snprintf(sErrMsg, sizeof(sErrMsg), "shmat fail key(%d) size(%d)", iKey, iSize);
		perror(sErrMsg);
		return NULL;
	}
	printf("sShm %p\n", sShm);

	if (shmctl(iShmId, IPC_STAT, &stShmDs) == 0) {
		if (iSize != 0 && stShmDs.shm_segsz != iSize) {
			snprintf(sErrMsg, sizeof(sErrMsg), "shmclt fail key(%d) stShmDs.shm_segsz(%lu) != iSize(%d)", 
					iKey, stShmDs.shm_segsz, iSize);
			perror(sErrMsg);
			return NULL;
		}
	} else {
		snprintf(sErrMsg, sizeof(sErrMsg), "shmclt fail key(%d)", iSize);
		perror(sErrMsg);
		return NULL;
	}

	return sShm;
}

/*
 * pShm point to the shm memory
 *
 * 1.try to get only
 * 2.if step 1 fail then new a shm memory and memset to zero
 *
 */ 
int GetShmEx(void **pShm, int iKey, int iSize, int iFlag)
{
	void *sShm;
	if (!(sShm = GetShm(iKey, iSize, iFlag & (~IPC_CREAT)))) {
		if (!(iFlag & IPC_CREAT))
			return -1;

		if (!(sShm = GetShm(iKey, iSize, iFlag)))
			return -2;

		memset(sShm, 0, iSize);
	}

	*pShm = sShm;
	return 0;
}


static int _GetShmAndForcely(void **pstShm, int iKey, int iSize)
{
	int iShmId;
	void *sShm;
	struct shmid_ds stShmDs;


	if ((iShmId = shmget(iKey, 0, 0)) >= 0) {
		//check the size
		if ((sShm = shmat(iShmId, NULL, 0)) == (char *)-1)
			return -4;

		if (shmctl(iShmId, IPC_STAT, &stShmDs) < 0) 
			return -5;

		if (stShmDs.shm_segsz == iSize) {
			*pstShm = sShm;
			return 0;
		}

		//if the size change, then delete the old shm
		if (shmdt(sShm) < 0)
			return -7;

		if (shmctl(iShmId, IPC_RMID, NULL) < 0)
			return -8;

		printf("iKey(%d) size change, from(%lu) to(%d)\n", iKey, stShmDs.shm_segsz, iSize);
	}

	//new a shm
	if ((iShmId = shmget(iKey, iSize, IPC_CREAT | 0666)) < 0)
		return -1;

	if ((sShm = shmat(iShmId, NULL, 0)) == (char *)-1)
		return -2;

	memset(sShm, 0, iSize);
	*pstShm = sShm;
	return 0;
}


#define LOCK_FILE_NAME_LEN 100

/*
 * get shm .if the size change, then delete the old shm
 *
 */ 
int GetShmFrocely(void **pstShm, int iKey, int iSize)
{
	char sLockFileName[LOCK_FILE_NAME_LEN];
	snprintf(sLockFileName, sizeof(sLockFileName), "/tmp/get_shm_forely_flock.%d", iKey);
	printf("lock file name(%s)--\n", sLockFileName);
	
	int fd = open(sLockFileName, O_RDWR | O_CREAT, 0666);
	if (-1 == fd) {
		perror("open lock file failed");
		return -11;
	}
	
	if (lockf(fd, F_LOCK, 0)) {
		perror("lockf lock failed");
		close(fd);
		return -12;
	}

	int iRet = _GetShmAndForcely(pstShm, iKey, iSize);

	if (lockf(fd, F_ULOCK, 0)) {
		perror("lockf unlock failed");
		close(fd);
		return -13;
	}
	close(fd);
	
	return iRet;
}


int DelShm(int iKey)
{
	int iShmId =0;

	if ((iShmId = shmget(iKey, 0, 0)) >= 0) {

		if (shmctl(iShmId, IPC_RMID, NULL) < 0) {
			perror("RM failse");
			return -2;
		}
	}

	return 0;
}

int GetShmSize(int iKey) 
{
	int iShmId;
	struct shmid_ds stShmDs;

	if ((iShmId = shmget(iKey, 0, 0)) < 0) 
		return -1;

	if (shmctl(iShmId, IPC_STAT, &stShmDs) < 0) 
		return -2;

	return (int)stShmDs.shm_segsz;
}


char* CM_GetShm(int iKey, int iSize, int iFlag)
{
    int iShmID = 0;
    char* sShm = NULL;

    if ((iShmID = shmget(iKey, iSize, iFlag)) < 0) {
        return NULL;
    }

    if ((sShm = (char*) shmat(iShmID, NULL, 0)) == (char *) - 1) {
        return NULL;
    }

    return sShm;
}

int CM_GetShm2(void **pstShm, int iKey, size_t iSize, int iFlag)
{
    char *sShm;
    if( 0 == iKey ) {
        return -__LINE__;
    }

	//尝试attach
    if(!(sShm = CM_GetShm(iKey, iSize, iFlag & (~IPC_CREAT)))) {
        if(!(iFlag & IPC_CREAT))
            return -__LINE__;
		//创建新的
        if(!(sShm = CM_GetShm(iKey, iSize, iFlag)))
            return -__LINE__;

		//创建新的 需清0
        memset(sShm, 0, iSize);
        *pstShm = sShm;
        return 1;
    }

    *pstShm = sShm;
    return 0;
}
