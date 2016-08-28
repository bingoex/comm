#ifndef _CM_NET_H_
#define _CM_NET_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif 

struct in_addr *atoaddr(const char *sAddress);

#ifdef __cplusplus
}
#endif 


#endif
