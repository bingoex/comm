#ifndef _CM_PROCESS_H_
#define _CM_PROCESS_H_

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif 

void DaemonInit();
int WritePid(const char *sPidFilePath);

#ifdef __cplusplus
}
#endif 


#endif
