#ifndef _CM_PROCESS_H_
#define _CM_PROCESS_H_



/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

/*
 * 创建Daemon进程
 */
void DaemonInit();

/*
 * 写进程pid到相应的文件
 */
int WritePid(const char *sPidFilePath);

/*
#ifdef __cplusplus
}
#endif 
*/



#endif
