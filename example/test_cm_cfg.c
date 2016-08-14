#include <stdio.h>
#include "cm_cfg.h"
#include <string.h>
#include <stdlib.h>

#define DEFAULT_CONF_FILE "test_cm_cfg.conf"

int main(int argc, char * argv[])
{
	char sConfFilePath[80];
	strncpy(sConfFilePath, DEFAULT_CONF_FILE, sizeof(sConfFilePath));

	char sLogFilePath[100];
	int iLogLevel = 0;
	char sPidFilePath[100];

	/*
	 * ConfFliePath,
	 * name, tpe, val, defaultVal, [String Size]
	 * NULL
	 */
	int iRet = Comm_GetConfig(sConfFilePath,
			"LOGFILE", CFG_STRING, sLogFilePath, "/data/log/test_cfg.log", sizeof(sLogFilePath),
			"LOG_LEVEL", CFG_INT, &iLogLevel, 3,
			"PIDFILE", CFG_STRING, sPidFilePath, "/tmp/test_cfg.pid", sizeof(sPidFilePath),
			(void *)NULL);

	printf("LOGFILE %s\nLOG_LEVEL %d\nPIDFILE %s\n", sLogFilePath, iLogLevel, sPidFilePath);

	return 0;
}
