#include <sys/sem.h>
#include <errno.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// union semun is defined by including <sys/sem.h> 
#else

union CmSemun
{
    int val;                  /* value for SETVAL */
    struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;    /* array for GETALL, SETALL */
    /* Linux specific part: */
    struct seminfo *__buf;    /* buffer for IPC_INFO */
};
#endif
extern int errno;
int CM_SemLockNoWait(int iSemKey);
int CM_SemLockWait(int iSemKey, int iTimeOut);
int CM_SemUnLock(int iSemKey);
