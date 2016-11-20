#include "cm_procnum_control.h"

int procnum_control_on_start(key_t semkey, int iProcNum)
{
	int semid;
	struct sembuf sbuf;

	/* Get semaphore ID associated with this key. */
	if((semid=semget(semkey, 0, S_IRUSR|S_IWUSR)) < 0) {
		/* Semaphore does not exist - Create. */
		if((semid=semget(semkey, 1, IPC_CREAT | IPC_EXCL | S_IRUSR |
						S_IWUSR | S_IRGRP | S_IROTH)) >= 0) {
			/* Initialize the semaphore. */
			memset(&sbuf,0,sizeof(sbuf));

			sbuf.sem_num = 0;
			sbuf.sem_op = iProcNum;  
			sbuf.sem_flg = 0;


			if(semop(semid, (struct sembuf*)(&sbuf), 1) < 0) {
				perror("semget IPC_CREAT SUCC semop");
				return -__LINE__;
			}
		}
		else if (errno == EEXIST) {
			if((semid=semget(semkey, 0, S_IRUSR|S_IWUSR)) < 0) {
				perror("semget IPC_CREAT EEXIST semget again");
				return -__LINE__;
			}
		}
		else {
			perror("semget IPC_CREAT");
			return -__LINE__;
		}
	}

	//try to lock
	memset(&sbuf,0,sizeof(sbuf));
	sbuf.sem_num = 0;
	sbuf.sem_op = -1;
	sbuf.sem_flg = SEM_UNDO|IPC_NOWAIT;

	if(semop(semid, &sbuf, 1) < 0) {
		perror("semop");
		return -__LINE__;
	}

	return 0;
}

int procnum_control_on_exit(key_t semkey)
{
	int semid;
	struct sembuf sbuf;

	/* Get semaphore ID associated with this key. */
	if((semid=semget(semkey, 0, S_IRUSR|S_IWUSR)) < 0) {
		perror("semget");
		return -__LINE__;
	}

	//release the sem lock
	memset(&sbuf,0,sizeof(sbuf));
	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = SEM_UNDO|IPC_NOWAIT;

	if(semop(semid,&sbuf, 1) < 0) {
		perror("semop");
		return -__LINE__;
	}

	return 0;
}

int procnum_control_get_value(key_t semkey, int* pValue)
{
	int semid;

	/* Get semaphore ID associated with this key. */
	if((semid=semget(semkey, 0, S_IRUSR|S_IWUSR)) < 0) {
		perror("semget");
		return -__LINE__;
	}

    *pValue = semctl(semid, 0, GETVAL);
    return 0;
}

