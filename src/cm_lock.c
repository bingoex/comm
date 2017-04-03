#include <unistd.h> //usleep
#include <sys/sem.h> //sem*
#include <errno.h> //EEXIST

#include "cm_lock.h"

#define SEM_INIT_MAX_TRIES 10

/*
 * iTimeOut 单位毫秒
 */
static int CM_SemLock(int iSemKey, int iTimeOut)
{
    union CmSemun arg;
    struct semid_ds stSemInfo;
    struct sembuf stSemBuf;
    int iLockId = 0;
    int iRet = 0;

    //retry lock interval: 10ms
    int iMaxRetry = (iTimeOut+9)/10;
    int iRetry = 0;
    int i = 0;

    // 资源个数初始化1
    // IPC_EXCL 已存在则产生错误, IPC_CREAT 不存在则创建
    iLockId = semget(iSemKey, 1, IPC_CREAT|IPC_EXCL|0666);
    if(iLockId >= 0) {
        // 新创建
        arg.val = 1;
        semctl(iLockId, 0, SETVAL, arg);
    }
    else if(errno == EEXIST) {
        // 已存在则单纯的get, 并获取信号量状态信息
        iLockId = semget(iSemKey, 1, 0666);
        arg.buf = &stSemInfo;
        // TODO
        for(i = 0; i < SEM_INIT_MAX_TRIES; i++) {
            semctl(iLockId, 0, IPC_STAT, arg); 
            if(arg.buf->sem_otime != 0) {
                goto RETRY_LOCK;
            }
            usleep(1000*10); //10ms
        }

        return -__LINE__;
    }
    else {
        return -__LINE__;
    }
    
RETRY_LOCK:
    stSemBuf.sem_num = 0;
    stSemBuf.sem_op = -1; 
    stSemBuf.sem_flg = SEM_UNDO|IPC_NOWAIT;

    iRet = semop(iLockId, &stSemBuf, 1); 
    if(iRet < 0) {
        if(iRetry < iMaxRetry) {
            iRetry++;
            usleep(10*1000); //10ms
            goto RETRY_LOCK;
        }
        else { //wait timeout 
            return -__LINE__;
        }
    }

    return 0;
}

int CM_SemUnLock(int iSemKey)
{
    int iRet = 0;
    struct sembuf stSemBuf;
    int iLockId = 0;

    iLockId = semget(iSemKey, 1, 0);
    if(iLockId == -1) {
        return -__LINE__;
    } 
    
    stSemBuf.sem_num = 0;
    stSemBuf.sem_op = 1;
    stSemBuf.sem_flg = SEM_UNDO|IPC_NOWAIT;
    iRet = semop(iLockId, &stSemBuf, 1);
    if(iRet < 0) {
        return -__LINE__;
    }

    return 0;
}

int CM_SemLockNoWait(int iSemKey)
{
    return CM_SemLock(iSemKey, 0);
}

int CM_SemLockWait(int iSemKey, int iTimeOut)
{
    return CM_SemLock(iSemKey, iTimeOut);
}


/*
int main()
{
	int key = 0x12345, iRet;
	int count = 10;
	int value = 0;
	for (int i = 0; i < count; i++) {
		iRet = CM_SemLockWait(key, 200);
		if (iRet) {
			printf("CM_SemLockWait fail %d errno %d\n", iRet, errno);
			continue;
		}

		value++;

		iRet = CM_SemUnLock(key);
		if (iRet) {
			printf("CM_SemUnLock fail %d errno %d\n", iRet, errno);
			continue;
		}

		printf("success value %d\n", value);
	}

	return 0;
}
*/
		
		
