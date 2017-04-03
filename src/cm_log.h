#ifndef _CM_LOG_H_
#define _CM_LOG_H_



#include "cm_type.h"

/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

#define MAX_LOG_FILE_NAME_LEN 100

typedef struct {
	FILE *pLogFile;
	char sBaseFileName[MAX_LOG_FILE_NAME_LEN]; //日志全面 去掉“.log”
	char sLogFileName[MAX_LOG_FILE_NAME_LEN]; //日志文件全名

	//0:by size  1:by LogFile Count
	int iShiftType; //日志轮转方式
	int iMaxLogNum;

	CM_INT32 lMaxCount;

	CM_INT32 lMaxFileSize;

	CM_INT32 lMaxLineCount; 
	CM_INT32 lLogLineCount;

	CM_INT32 lLastShiftTime;
	
} LogFile;

#define LOG_SHIFT_BY_SIZE 0
#define LOG_SHIFT_BY_COUNT 1

#define LOG_FORMAT_TYPE_NOTIME 0
#define LOG_FORMAT_TYPE_TIME 1

/*
 *  1、初始化
 *  参数：
 *      /tmp/log/test_cm_log: 会自动添加log后缀然后生成文件 如 /tmp/log/test_cm_log.log
 *      LOG_SHIFT_BY_SIZE: 按照日志文件大小滚动
 *      5: 最多5个文件
 *      10000: 单个文件最大大小
 *  LogFile stLogFile;
 *  InitLogFile(&stLogFile, "/tmp/log/test_cm_log", LOG_SHIFT_BY_SIZE, 5, 10000); 
 *
 *  2、写日志
 *  Log(&stLogFile, LOG_FORMAT_TYPE_TIME, "hehehe %d", 1);
 */
int InitLogFile(LogFile * pstLogFile, const char *sLogBaseName, int iShiftType, int iMaxLogNum, CM_INT32 lMaxSize);

/*
 * 线程不安全
 */
int Log(LogFile *pstLogFile, int iLogFormat, const char *sFormat, ...);

/*
#ifdef __cplusplus
}
#endif 
*/



#endif
