#ifndef _PROCNUM_CONTROL_H_
#define _PROCNUM_CONTROL_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>	
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

// crol the process num by System V's  semaphore


/* 
 * call this func when process start
 * return 0 success, not 0 false
 */
int procnum_control_on_start(key_t semKey, int iProcNum);

/*
 * call this func when process exit
 */
int procnum_control_on_exit(key_t semkey);

/*
 * get the value 
 * How many processes can be started
 *
 */
int procnum_control_get_value(key_t semkey, int* pValue);


#endif
