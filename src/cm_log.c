#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include<sys/time.h>

#include "cm_time.h"
#include "cm_log.h"

#define FILE_SUFFIX_LEN 10

static int ShiftFiles(LogFile *pstLogFile)
{
	int iRet = 0;
	int i = 0;
	struct stat stStat;
	char sOldFileName[MAX_LOG_FILE_NAME_LEN], sNewFileName[MAX_LOG_FILE_NAME_LEN];

	if ((iRet = stat(pstLogFile->sLogFileName, &stStat)) < 0) {
		printf("stat fail iret(%d) errno %d\n", iRet, errno);
		return -1;
	}

	printf("file size %llu lMaxFileSize %d lLogLineCount %d lMaxLineCount %d\n", 
			stStat.st_size, pstLogFile->lMaxFileSize, pstLogFile->lLogLineCount, pstLogFile->lMaxLineCount);

	switch (pstLogFile->iShiftType) {
		case LOG_SHIFT_BY_SIZE://0
			if (stStat.st_size < pstLogFile->lMaxFileSize) { //don't need shift so that return
				return 0;
			}
			//show shift file
			break;
		case LOG_SHIFT_BY_COUNT:
			if (pstLogFile->lLogLineCount < pstLogFile->lMaxLineCount)
				return 0;
			pstLogFile->lLogLineCount = 0;
			break;
	}

	//run here means that should shift file  
	//printf("iMaxLogNum %d\n", pstLogFile->iMaxLogNum);
	for (i = pstLogFile->iMaxLogNum - 2; i >= 0; i--) {
		//printf("shifting i %d\n", i);
		if (i == 0)
			snprintf(sOldFileName, sizeof(sOldFileName),  "%s.log", pstLogFile->sBaseFileName);
		else
			snprintf(sOldFileName, sizeof(sOldFileName), "%s%d.log", pstLogFile->sBaseFileName, i);


		if (!access(sOldFileName, F_OK)) {
			snprintf(sNewFileName, sizeof(sNewFileName), "%s%d.log", pstLogFile->sBaseFileName, i + 1);

			//printf("sOldFileName %s --> sNewFileName %s\n", sOldFileName, sNewFileName);

			if (rename(sOldFileName, sNewFileName) < 0) {
				printf("rename false errno(%d)\n", errno);
				return -2;
			}
		} else {
			printf("access(%s) false errno(%d)\n", sOldFileName, errno);
		}

	}

	pstLogFile->lLastShiftTime = time(NULL);
	return 0;
}


int InitLogFile(LogFile * pstLogFile, const char *sBaseFileName, int iShiftType, int iMaxLogNum, CM_INT32 lMaxSize)
{
	memset(pstLogFile, 0, sizeof(LogFile));
	strncat(pstLogFile->sLogFileName, sBaseFileName, sizeof(pstLogFile->sLogFileName) - FILE_SUFFIX_LEN);
	strncat(pstLogFile->sLogFileName, ".log", FILE_SUFFIX_LEN);

	printf("IninLogFile logfile name %s---\n", pstLogFile->sLogFileName);

	strncat(pstLogFile->sBaseFileName, sBaseFileName, sizeof(pstLogFile->sBaseFileName) - 1);

	printf("IninLogFile base name %s---\n", pstLogFile->sBaseFileName);

	pstLogFile->iShiftType = iShiftType;

	//the number of log file 
	pstLogFile->iMaxLogNum = iMaxLogNum;

	//the max size of one log file
	pstLogFile->lMaxFileSize = lMaxSize;

	//the number of line in one log file
	pstLogFile->lMaxLineCount = lMaxSize;
	pstLogFile->lLogLineCount = 0;
	pstLogFile->lLastShiftTime = time(NULL);

	printf("iShiftType %d iMaxLogNum %d lMaxFileSize %d lMaxLineCount %d\n", iShiftType, iMaxLogNum, lMaxSize, lMaxSize);

	return ShiftFiles(pstLogFile);
}


int Log(LogFile *pstLogFile, int iLogFormat, const char *sFormat, ...)
{
	//printf("Log bein\n");
	va_list ap;
	struct timeval stLogTv;

	if ((pstLogFile->pLogFile = fopen(pstLogFile->sLogFileName, "a+")) ==NULL) 
		return -1;

	va_start(ap, sFormat);

	//print time
	switch(iLogFormat) {
		case LOG_FORMAT_TYPE_NOTIME:
		case LOG_FORMAT_TYPE_TIME:
			gettimeofday(&stLogTv, NULL);
			fprintf(pstLogFile->pLogFile, "[%s.%.6u]", DateTimeToStr(stLogTv.tv_sec), (unsigned int)stLogTv.tv_usec);

			//printf("%s\n", DateTimeToStr(stLogTv.tv_sec));
			break;
	}

	//print content
	vfprintf(pstLogFile->pLogFile, sFormat, ap);

	//print \n
	fprintf(pstLogFile->pLogFile, "\n");
	va_end(ap);

	pstLogFile->lLogLineCount++;
	fclose(pstLogFile->pLogFile);

	//printf("Log end\n");

	return ShiftFiles(pstLogFile);
}
