#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdio.h>
#include <string.h>

#include "cm_process.h"



void DaemonInit()
{
	int fd;

	signal(SIGALRM, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	if (fork()) 
		exit(0);

	// new a session
	if (setsid() == -1)
		exit(1);

	for (fd = 3; fd < NOFILE; fd++)
		close(fd);

	if (chdir("/") < 0)
		exit(1);

	umask(0);

	return;
}

int WritePid(const char *sPidFilePath)
{
	FILE *pPidFile;
	if ((pPidFile = fopen(sPidFilePath, "w")) == NULL)
		return -1;

	fprintf(pPidFile, "%d", getpid());

	fclose(pPidFile);

	return 0;
}

