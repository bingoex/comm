#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "cm_cfg.h"

#define MAX_CONFIG_LEN 4096

static int _get_name(char * desc, char * src)
{
	char * pDesc = desc, * pSrc = src;
	int iWordCnt = 0, iSpace = 0;

	while (iWordCnt != 2 && *src != '\0') {
		switch (*pSrc) {
			case ' ':
			case '\t':
			case '\0':
			case '\n':
				iSpace = 1;
				pSrc++;
				break;
			default:
				//the second or first word
				if (iSpace || pSrc == src) {
					iWordCnt++;
					iSpace = 0;
				}

				//WordCnt == 2 mean has both name and val
				if (iWordCnt == 2) break;
				*pDesc++ = *pSrc++;
		}
	}
	*pDesc = '\0';
	strcpy(src, pSrc);
	return 0;
}

static void _InitDefault(va_list ap)
{
	char *sVal, *sDefault;;
	int *piVal, iDefault;
	double *pdVal, dDefault;
	CM_INT32 *plVal, lDefault;
	
	char *sName;
	int iType;
	CM_INT32 lSize;

	sName = va_arg(ap, char *);
	while (NULL != sName) {
		iType = va_arg(ap, int);
		switch (iType) {
			case CFG_STRING:
				sVal = va_arg(ap, char *);
				sDefault = va_arg(ap, char *);
				lSize = va_arg(ap, CM_INT32);
				//char default[] = "abcd"; size = sizeof(default) == 5 ; so 
				strncpy(sVal, sDefault, (int)(lSize - 1));
				sVal[lSize - 1] = '\0';
				break;
			case CFG_INT:
				piVal = va_arg(ap, int *);
				iDefault = va_arg(ap, int);
				*piVal = iDefault;
				break;
			case CFG_DOUBLE:
				pdVal = va_arg(ap, double *);
				dDefault = va_arg(ap, double);
				*pdVal = dDefault;
				break;
			case CFG_LONG:
				plVal = va_arg(ap, CM_INT32 *);
				lDefault = va_arg(ap, CM_INT32);
				*plVal = lDefault;
				break;
		}
		sName = va_arg(ap, char *);
	}
}

static int _SetVal(va_list ap, char *sCfgName, char *sCfgVal)
{
	char *sName = NULL;
	char *sVal = NULL, *sDefault = NULL;
	int *piVal = NULL, iDefault;
	double *pdVal = NULL, dDefault;
	CM_INT32 *plVal = NULL, lDefault;
	
	int iType; 
	CM_INT32 lSize = 0;

	char sLine[MAX_CONFIG_LEN];

	/*
	 * ensure the sCfgVal don't have blank space
	 */
	strcpy(sLine, sCfgVal);
	_get_name(sCfgVal, sLine);

	sName = va_arg(ap, char *);
	while (NULL != sName) {
		iType = va_arg(ap, int);
		switch (iType) {
			case CFG_STRING:
				sVal = va_arg(ap, char *);
				sDefault = va_arg(ap, char *);
				lSize = va_arg(ap, CM_INT32);
				break;
			case CFG_INT:
				piVal = va_arg(ap, int *);
				iDefault = va_arg(ap, int);
				break;
			case CFG_DOUBLE:
				pdVal = va_arg(ap, double *);
				dDefault = va_arg(ap, double);
				break;
			case CFG_LONG:
				plVal = va_arg(ap, CM_INT32 *);
				lDefault = va_arg(ap, CM_INT32);
				*plVal = lDefault;
				break;
		}

		//check whether is CfgName !!
		if (strcmp(sName, sCfgName) == 0) {
			switch (iType) {
				case CFG_STRING:
					strncpy(sVal, sCfgVal, (int)(lSize - 1));
					sVal[lSize - 1] = '\0';
					break;
				case CFG_LONG:
					*piVal = strtoul(sCfgVal, NULL, 10);
					break;
				case CFG_INT:
					*piVal = atoi(sCfgVal);
					break;
				case CFG_DOUBLE:
					*pdVal = atof(sCfgVal);
					break;
			}

			return 0;
		}
		sName = va_arg(ap, char *);
	}

	return -1;
}


static int _GetNameAndVal(char * sLine, char * sName, char * sVal)
{
	_get_name(sName, sLine);
	strcpy(sVal, sLine);

	if ('#' == sName[0])
		return -1;

	return 0;
}

int Comm_GetConfig(char * sConfigFileFullPath, ...)
{
	FILE * pstFile = NULL;
	va_list ap;
	char sLine[MAX_CONFIG_LEN], sName[MAX_CONFIG_LEN], sVal[MAX_CONFIG_LEN];
	
	va_start(ap, sConfigFileFullPath);
	//init Default value
	_InitDefault(ap);
	va_end(ap);

	if (NULL == (pstFile = fopen(sConfigFileFullPath, "r"))) {
		return -1;
	}

	while (true) {
		if (NULL == fgets(sLine, sizeof(sLine), pstFile)) {
			break;
		}

		if (feof(pstFile)) {
			break;
		}

		if (0 == _GetNameAndVal(sLine, sName, sVal)) {
			va_start(ap, sConfigFileFullPath);
			_SetVal(ap, sName, sVal);
			va_end(ap);
		}
	}

	fclose(pstFile);
	return 0;
}
