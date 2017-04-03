#include <stdio.h>

#include "cm_log.h"



int main(int argc, char * argv[])
{
	LogFile stLogFile;

	InitLogFile(&stLogFile, "/tmp/log/test_cm_log", LOG_SHIFT_BY_SIZE, 5, 10000);
	
	Log(&stLogFile, LOG_FORMAT_TYPE_TIME, "hehehe %d", 1);

    return 0;
}
