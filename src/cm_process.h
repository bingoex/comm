#ifndef _CM_PROCESS_H_
#define _CM_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif 

void DaemonInit();
int WritePid(const char *sPidFilePath);

#ifdef __cplusplus
}
#endif 


#endif
