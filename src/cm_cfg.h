#ifndef _CM_CFG_H_
#define _CM_CFG_H_



/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

#define CFG_STRING 1
#define CFG_INT 2
#define CFG_LONG 3
#define CFG_DOUBLE 4

/*
 * 配置文件读取
 * 支持string、int、long、double类型
 *
 * 参数说明：
 * ConfFliePath,
 * name, tpe, val, defaultVal, [val String Size], 
 * NULL 
 * int iRet = Comm_GetConfig(sConfFilePath,
			"LOGFILE", CFG_STRING, sLogFilePath, "/data/log/test_cfg.log", sizeof(sLogFilePath),
			"LOG_LEVEL", CFG_INT, &iLogLevel, 3,
			"PIDFILE", CFG_STRING, sPidFilePath, "/tmp/test_cfg.pid", sizeof(sPidFilePath),
			(void *)NULL);
 */
int Comm_GetConfig(char * sConfigFullFileName, ...);

/*
#ifdef __cplusplus
}
#endif 
*/



#endif

