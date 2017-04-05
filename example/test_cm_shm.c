#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h> //O_RDWR O_CREAT

#include "cm_shm.h"

#define SHM_ID 0x88888

#define PERSION_ARRAY_SIZE 10

struct Person {
	int iAge;
	int iSex;
};

struct Person g_astPersons[PERSION_ARRAY_SIZE];

int main(int argc, char * argv[])
{
	printf("argc %d\n", argc);
	struct Person *sShm = NULL;
	int iRet = 0;

	if (argc == 1) {
		//chang the shm
		iRet =GetShmEx((void **)&sShm, SHM_ID, sizeof(g_astPersons), IPC_CREAT | 0666);
		printf("GetShmEx ret(%d)\n", iRet);

		srand((unsigned)time(NULL));
		for (int i = 0; i < PERSION_ARRAY_SIZE; i++) {
			sShm[i].iAge = (1+(int) (90.0*rand()/(RAND_MAX+1.0)));
			((struct Person *)sShm)[i].iSex = i%2 ? 11:12;
		}

	} else if (argc == 2) {

		//get readonly
		iRet = GetShmFrocely((void **)&sShm, SHM_ID, sizeof(g_astPersons));
		printf("GetShmFrocely ret(%d)\n", iRet);

		for (int i = 0; i < PERSION_ARRAY_SIZE; i++) {
			printf("%d age(%d), sex(%d)\n", i, sShm[i].iAge, sShm[i].iSex);
		}

		//Get size
		printf("shm size %d Byte\n", GetShmSize(SHM_ID));
	} else if (argc ==3) {

		//delete shm
		iRet = DelShm(SHM_ID);
		printf("DelShm ret(%d)\n", iRet);
	}

	//sleep(5);
	return 0;
}
