#include <stdio.h>
#include <time.h> //tm time

#include "cm_time.h"

#define STRING_BUFFER_LEN 50

char *DateTimeToStr(const time_t tTime)
{
	static char sBuff[STRING_BUFFER_LEN];
	struct tm stTm;
	
	stTm = *localtime(&tTime);

	if (stTm.tm_year > 50) {
		snprintf(sBuff, sizeof(sBuff), "%04d-%02d-%02d %02d:%02d:%02d",
				stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday,
				stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
	} else {
		snprintf(sBuff, sizeof(sBuff), "%04d-%02d-%02d %02d:%02d:%02d",
				stTm.tm_year + 2000, stTm.tm_mon + 1, stTm.tm_mday,
				stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
	}

	return sBuff;
}

/*
int main()
{
	time_t t = time(NULL);
	printf("now : %s\n", DateTimeToStr(t));
}
*/
