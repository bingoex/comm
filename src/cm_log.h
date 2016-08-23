#ifndef _CM_LOG_H_
#define _CM_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#include "cm_type.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define MAX_LOG_FILE_NAME_LEN 100

typedef struct {
	FILE *pLogFile;
	char sBaseFileName[MAX_LOG_FILE_NAME_LEN];
	char sLogFileName[MAX_LOG_FILE_NAME_LEN];

	//0:by size  1:by LogFile Count  2:
	int iShiftType;
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

int InitLogFile(LogFile * pstLogFile, const char *sLogBaseName, int iShiftType, int iMaxLogNum, CM_INT32 lMaxSize);
int Log(LogFile *pstLogFile, int iLogFormat, const char *sFormat, ...);

#ifdef __cplusplus
}
#endif 


#endif
