#ifndef _CM_PROCNUM_CONTROL_H_
#define _CM_PROCNUM_CONTROL_H_



/* 
 * 进程启动时调用
 * 参数说明：
 *  iProcNum：最大进程启动数
 * 返回码：
 * return 0 success, not 0 false
 */
int procnum_control_on_start(key_t semKey, int iProcNum);

/*
 * 进程结束时调用
 */
int procnum_control_on_exit(key_t semkey);

/*
 * 获取进程启动数量
 */
int procnum_control_get_value(key_t semkey, int* pValue);



#endif
