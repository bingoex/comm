#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "cm_net.h"
#include "cm_type.h"

struct in_addr *atoaddr(const char *sAddress)
{
	static struct in_addr stSaddr;
	struct hostent *pHost;

	//if sAddress == XXX.XXX.XXX.XXX
	stSaddr.s_addr = inet_addr(sAddress);
	if (stSaddr.s_addr != -1)
		return &stSaddr;

	printf("try gethostbyname\n");

	pHost = gethostbyname(sAddress);
	if (pHost != NULL)
		return (struct in_addr *) *(pHost->h_addr_list);

	return NULL;
}


int atoport(const char *sService, const char *sProto)
{
	int iPort;
	CM_INT32 lPort;
	
	struct servent *pstServ;
	char *sErrpos;

	pstServ = getservbyname(sService, sProto);
	if (pstServ != NULL) {
		iPort = pstServ->s_port;
		printf("getservbyname %d\n", ntohs(iPort));
	} else {
		lPort = strtol(sService, &sErrpos, 0);
		if (sErrpos[0] != 0 || lPort < 1 || lPort > 65535)
			return -1;

		iPort = htons(lPort);
		printf("strtol %d\n", lPort);
	}
	return iPort;
}

int SetNBLock(int iSock)
{
	int iFlags;
	iFlags = fcntl(iSock, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	iFlags |= O_NDELAY;
	fcntl(iSock, F_SETFL, iFlags);
	return 0;
}

#define MAX_LISTENING_NUM 5000
int CreateSocketRaw(int iSockType, int *pListener, uint32_t dwListenIp, int iPort, int iIsNeedBlock)
{
	struct sockaddr_in address;
	int iListenSocket;
	int iReuseAddr = 1;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = iPort;
	address.sin_addr.s_addr = dwListenIp;

	iListenSocket = socket(AF_INET, iSockType, 0);
	if (iListenSocket < 0) {
		perror("socket failed");
		return -7;
	}

	if (!iIsNeedBlock) {
		SetNBLock(iListenSocket);
		printf("SetNBLock\n");
	}

	if(NULL != pListener)
		*pListener = iListenSocket;

	//TODO
	setsockopt(iListenSocket, SOL_SOCKET, SO_REUSEADDR, &iReuseAddr, sizeof(iReuseAddr));

	if (bind(iListenSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("bind failed");
		return -9;
	}

	if (iSockType == SOCK_STREAM) {
		listen(iListenSocket, MAX_LISTENING_NUM);
	}

	return 0;
}

int CreateSocket(const char *sSockType, int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock)
{
	struct in_addr *pstAddr;
	int iPort = 0;

	if (sSockType == NULL || sListenIp == NULL || sPort == NULL)
		return -1;

	pstAddr = atoaddr(sListenIp);

	iPort = atoport(sPort, sSockType);

	if (strcmp(sSockType, "udp") == 0) {
		return  CreateSocketRaw(SOCK_DGRAM, pListener, pstAddr->s_addr, iPort, iIsNeedBlock);
	} else if (strcmp(sSockType, "tcp") == 0) {
		return  CreateSocketRaw(SOCK_STREAM, pListener, pstAddr->s_addr, iPort, iIsNeedBlock);
	}

	return -2;
}

int CreateTcpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock)
{
	return CreateSocket("tcp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateUdpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock)
{
	return CreateSocket("udp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateTcpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	//TODO
	printf("Serverport itoa %s--\n", sPort);
	return CreateSocket("tcp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateUdpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	//TODO
	printf("Serverport itoa %s--\n", sPort);
	return CreateSocket("udp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateClientSocketRaw(int iSockType, int *piSocket, uint32_t dwServerIp, int iPort, int iIsNeedBlock)
{
	struct sockaddr_in address;
	int iSocket;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = iPort;
	address.sin_addr.s_addr = dwServerIp;

	iSocket = socket(AF_INET, iSockType, 0);
	if (iSocket < 0) {
		perror("Client socket failed");
		return -8;
	}

	if (!iIsNeedBlock) {
		SetNBLock(iSocket);
		printf("Client SetNBLock\n");
	}

	if(NULL != piSocket)
		*piSocket = iSocket;

	if (connect(iSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
		if (!iIsNeedBlock && errno != EINPROGRESS) {
			perror("client connect failed");
			close(iSocket);
			return -9;
		}
	}

	return 0;
}

int CreateClientSocket(const char *sSockType, int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock)
{
	struct in_addr *pstAddr;
	int iPort = 0;

	if (sSockType == NULL || sServerIp == NULL || sPort == NULL)
		return -1;

	pstAddr = atoaddr(sServerIp);

	iPort = atoport(sPort, sSockType);

	if (strcmp(sSockType, "tcp") == 0) {
		return  CreateClientSocketRaw(SOCK_STREAM, piSocket, pstAddr->s_addr, iPort, iIsNeedBlock);
	} else {
		printf("error type %s\n", sSockType);
		return -3;
	}

	return -2;
}

int CreateTcpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock)
{
	return CreateClientSocket("tcp", piSocket, sServerIp, sPort, iIsNeedBlock);
}

int CreateTcpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	printf("client port itoa %s--\n", sPort);
	return CreateClientSocket("tcp", piSocket, sServerIp, sPort, iIsNeedBlock);
}

int CreateUdpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock)
{
	return CreateClientSocket("udp", piSocket, sServerIp, sPort, iIsNeedBlock);
}

int CreateUdpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	printf("client port itoa %s--\n", sPort);
	return CreateClientSocket("udp", piSocket, sServerIp, sPort, iIsNeedBlock);
}
/*
 * dwIp netword oder
 */ 
int IsInnerIp(in_addr_t dwIp)
{
	static int iIsInit = 0;
	static unsigned int dwInnerIp1 = 0;
	static unsigned int dwInnerIp2 = 0;
	static unsigned int dwInnerIp3 = 0;

	if (!iIsInit) {
		iIsInit = 1;
		dwInnerIp1 = ntohl(inet_addr("10.0.0.0"));
		dwInnerIp2 = ntohl(inet_addr("172.16.0.0"));
		dwInnerIp3 = ntohl(inet_addr("192.168.0.0"));
	}

	//channg to h
	dwIp = ntohl(dwIp);

	if (((dwIp & 0xFF000000) == dwInnerIp1)
			|| ((dwIp & 0xFFF00000) == dwInnerIp2)
			|| ((dwIp & 0xFFFF0000) == dwInnerIp3))
		return 1;

	return 0;
}

/*
 * just for old linux
int GetLocalIp(const char *sInterfaceName, uint32_t *pIp)
{
	struct ifreq if_data;
	uint32_t dwIp;
	int fd = 0;

	if (pIp == NULL)
		return -1;

	if ((fd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0) {
		perror("socket failed");
		return -2;
	}

	if (sInterfaceName == NULL)
		strncpy(if_data.ifr_name, "en1", sizeof(if_data.ifr_name));
	else 
		strncpy(if_data.ifr_name, sInterfaceName, sizeof(if_data.ifr_name));

	if (ioctl(fd, SIOCGIFADDR, &if_data) < 0) {
		perror("ioctl get ip address failed");
		return -3;
	}

	close(fd);

	memcpy((void *) &dwIp, (char *)&if_data.ifr_addr.sa_data + 2, sizeof(dwIp));

	*pIp = dwIp;

	return IsInnerIp((in_addr_t)dwIp);
}
*/

int GetIpByName(const char *sInterfaceName, uint32_t *pIp)
{
	struct sockaddr_in *sin = NULL;
	struct ifaddrs *ifa = NULL, *ifList;

	if (getifaddrs(&ifList) < 0) return -1;

	for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr->sa_family == AF_INET)
		{
			printf("\n>>> interfaceName: %s\n", ifa->ifa_name);

			sin = (struct sockaddr_in *)ifa->ifa_addr;
			printf(">>> ipAddress: %s\n", inet_ntoa(sin->sin_addr));

			sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
			printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));

			sin = (struct sockaddr_in *)ifa->ifa_netmask;
			printf(">>> subnetMask: %s\n", inet_ntoa(sin->sin_addr));

			if ((sInterfaceName == NULL && !strcmp(ifa->ifa_name, "en1"))
					|| (sInterfaceName != NULL  && !strcmp(ifa->ifa_name, sInterfaceName))) {
				*pIp  = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
				return IsInnerIp((in_addr_t)*pIp);
			}
		}
	}

	freeifaddrs(ifList);

	return -2;
}

struct sockaddr_in * CreateAddr(const char *sIp, const char *sPort, const char *sSockType)
{
	static struct sockaddr_in address;
	struct in_addr *pstAddr = NULL;
	int iPort = 0;

	if (sIp == NULL)
		return NULL;

	pstAddr = atoaddr(sIp);

	iPort = atoport(sPort, sSockType);

	printf("iPort %d", iPort);

	if(iPort < 0 || pstAddr == NULL)
		return NULL;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = iPort;
	address.sin_addr = *pstAddr;

	return &address;
}

struct sockaddr_in * CreateAddrEx(const char *sIp, int iPort, const char *sSockType)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	printf("CreateAddrEx port itoa %s--\n", sPort);

	return CreateAddr(sIp, sPort, sSockType);
}
