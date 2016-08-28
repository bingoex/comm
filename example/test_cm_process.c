#include <stdio.h>
#include <unistd.h>
#include "cm_process.h"

int main()
{

	WritePid("/tmp/test_cm_process.pid");

	DaemonInit();

	sleep(15);
	//ps aux | grep test_cm_process


	return 0;
}
