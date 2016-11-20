#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#include "cm_lock.h"

#define SEM_INIT_MAX_TRIES 10
static int CM_SemLock(int iSemKey, int iWaitFlag, int iTimeOut)
{
    union CmSemun arg;
    struct semid_ds stSemInfo;
    int iLockId = 0;
    struct sembuf stSemBuf;
    int iRet = 0;
    //retry lock interval: 10ms
    int iMaxRetry = (iTimeOut+9)/10;
    int iRetry = 0;
    int i = 0;

    iLockId = semget(iSemKey, 1, IPC_CREAT|IPC_EXCL|0666);
    if(iLockId >= 0) {
        arg.val = 1;
        semctl(iLockId, 0, SETVAL, arg);
    }
    else if(errno == EEXIST) {
        iLockId = semget(iSemKey, 1, 0666);
        arg.buf = &stSemInfo;
        for(i = 0; i < SEM_INIT_MAX_TRIES; i++) {
            semctl(iLockId, 0, IPC_STAT, arg); 
            if(arg.buf->sem_otime != 0) {
                goto RETRY_LOCK;
            }
            usleep(1000*10); 
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
            usleep(10*1000);
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
    return CM_SemLock(iSemKey, 0, 0);
}

int CM_SemLockWait(int iSemKey, int iTimeOut)
{
    return CM_SemLock(iSemKey, 1, iTimeOut);
}

