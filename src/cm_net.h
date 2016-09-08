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

#define NO_BLOCK 0
#define NEED_BLOCK 1

#ifdef __cplusplus
extern "C" {
#endif 

struct in_addr *atoaddr(const char *sAddress);
int atoport(const char *sService, const char *sProto);

int CreateSocketRaw(int iSockType, int *pListener, uint32_t dwListenIp, int iPort, int iIsNeedBlock);
int CreateSocket(const char *sSockType, int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);
int CreateTcpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);
int CreateUdpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);
int CreateTcpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock);
int CreateUdpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock);

int CreateClientSocketRaw(int iSockType, int *piSocket, uint32_t dwServerIp, int iPort, int iIsNeedBlock);
int CreateClientSocket(const char *sSockType, int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);
int CreateTcpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);
int CreateTcpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock);

int CreateUdpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);
int CreateUdpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock);

//int GetLocalIp(const char *sInterfaceName, uint32_t *pIp);
int IsInnerIp(in_addr_t dwIp);
int GetIpByName(const char *sInterfaceName, uint32_t *pIp);

int SetNBLock(int iSock);

struct sockaddr_in * CreateAddr(const char *sIp, const char *sPort, const char *sSockType);
struct sockaddr_in * CreateAddrEx(const char *sIp, int iPort, const char *sSockType);

#ifdef __cplusplus
}
#endif 


#endif
