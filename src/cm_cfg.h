#ifndef _CM_CFG_H_
#define _CM_CFG_H_

#include "cm_type.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define CFG_STRING 1
#define CFG_INT 2
#define CFG_LONG 3
#define CFG_DOUBLE 4

int Comm_GetConfig(char * sConfigFullFileName, ...);


#ifdef __cplusplus
}
#endif 

#endif

