#ifndef _CM_NET_H_
#define _CM_NET_H_

#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <net/ethernet.h>
#include <net/if.h>       /* for ifconf */  
#include <netinet/in.h>       /* for sockaddr_in */  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <sys/ioctl.h>  
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif 

struct in_addr *atoaddr(const char *sAddress);
int atoport(const char *sService, const char *sProto);

int CreateSocketRaw(int iSockType, int *pListener, uint32_t dwListenIp, int iPort);

int CreateSocket(const char *sSockType, int *pListener, const char *sListenIp, const char *sPort);

int CreateTcpSocket(int *pListener, const char *sListenIp, const char *sPort);
int CreateUdpSocket(int *pListener, const char *sListenIp, const char *sPort);

//int GetLocalIp(const char *sInterfaceName, uint32_t *pIp);
int IsInnerIp(in_addr_t dwIp);
int GetIpByName(const char *sInterfaceName, uint32_t *pIp);

#ifdef __cplusplus
}
#endif 


#endif
