#include <stdio.h>
#include <stdlib.h> //strtol
#include <unistd.h> //close
#include <fcntl.h> //F_GETFL O_NONBLOCK O_NDELAY F_SETFL
#include <errno.h> //EINPROGRESS
#include <string.h> //strcmp memset
#include <ifaddrs.h> //getifaddrs ifaddrs freeifaddrs
#include <netdb.h> //gethostbyname hostent getservbyname servent
#include <arpa/inet.h> //inet_addr

#include "cm_net.h"
#include "cm_type.h"

#define NO_BLOCK 0
#define NEED_BLOCK 1
#define MAX_LISTENING_NUM 5000

/*
 * char * 转换为in_addr
 * 支持ip字符串（xxx.xxx.xxx.xxx）和 hostname
 */
struct in_addr *atoaddr(const char *sAddress)
{
	static struct in_addr stSaddr;
	struct hostent *pHost;

	//如果 sAddress == XXX.XXX.XXX.XXX
	stSaddr.s_addr = inet_addr(sAddress);
	if (stSaddr.s_addr != -1)
		return &stSaddr;

	pHost = gethostbyname(sAddress);
	if (pHost != NULL)
		return (struct in_addr *) *(pHost->h_addr_list);

	return NULL;
}


/*
 * char * 转换为端口
 * 支持端口数字字符串（“8080”）和 getservbyname
 */
int atoport(const char *sService, const char *sProto)
{
	int iPort;
	CM_INT32 lPort;
	
	struct servent *pstServ;
	char *sErrpos;

	pstServ = getservbyname(sService, sProto);
	if (pstServ != NULL) {
		iPort = pstServ->s_port;
	} else {
		lPort = strtol(sService, &sErrpos, 0);
		if (sErrpos[0] != 0 || lPort < 1 || lPort > 65535)
			return -1;

		iPort = htons(lPort);
	}

	return iPort;
}

/*
 * 设置fd为非阻塞
 */
int SetNBLock(int iSock)
{
	int iFlags;
	iFlags = fcntl(iSock, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	iFlags |= O_NDELAY;
	fcntl(iSock, F_SETFL, iFlags);
	return 0;
}

/*
 * 创建socket并监听(如果为tcp协议)
 * 参数说明：
 *      iSockType：SOCK_DGRAM/SOCK_STREAM
 *      pListener: 监听socket，返回值
 *      dwListenIp：ip网络序
 *      iPort：端口网络序
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
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

/*
 * 创建socket并监听(如果为tcp协议)
 * 参数说明：
 *      sSockType：tcp/udp字符串
 *      pListener: 监听socket，返回值
 *      sListenIp：ip字符串
 *      sPort：端口字符串
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
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

/*
 * 创建tcp socket并监听
 * 参数说明：
 *      pListener: 监听socket，返回值
 *      sListenIp：ip字符串
 *      sPort：端口字符串
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
int CreateTcpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock)
{
	return CreateSocket("tcp", pListener, sListenIp, sPort, iIsNeedBlock);
}

/*
 * 创建udp socket
 * 参数说明：
 *      pListener: 监听socket，返回值
 *      sListenIp：ip字符串
 *      sPort：端口字符串
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
int CreateUdpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock)
{
	return CreateSocket("udp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateTcpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	return CreateSocket("tcp", pListener, sListenIp, sPort, iIsNeedBlock);
}

int CreateUdpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock)
{
	static char sPort[8];
	snprintf(sPort, sizeof(sPort) - 1, "%d", iPort);
	return CreateSocket("udp", pListener, sListenIp, sPort, iIsNeedBlock);
}

/*
 * 创建发包socket (如果tcp协议则conn)
 * 参数说明：
 *      iSockType：SOCK_DGRAM/SOCK_STREAM
 *      piSocket: 发包socketfd，返回值
 *      dwServerIp：服务器ip（网络序）
 *      iPort：服务器端口
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
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
	}

	if(NULL != piSocket)
		*piSocket = iSocket;

	if (iSockType == SOCK_STREAM) {
		if (connect(iSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
			if (!iIsNeedBlock && errno != EINPROGRESS) {
				perror("client connect failed");
				close(iSocket);
				return -9;
			}
		}
	}

	return 0;
}

/*
 * 创建发包socket (如果tcp协议则conn)
 * 参数说明：
 *      sSockType：tcp/udp
 *      piSocket: 发包socketfd，返回值
 *      sServerIp：服务器ip
 *      sPort：服务器端口
 *      iIsNeedBlock：是否阻塞(1是)
 *  返回值：
 *      0：成功
 *      非0:失败
 */
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
		return  CreateClientSocketRaw(SOCK_DGRAM, piSocket, pstAddr->s_addr, iPort, iIsNeedBlock);
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
	return CreateClientSocket("udp", piSocket, sServerIp, sPort, iIsNeedBlock);
}

/*
 * 判断dwip是否为内网
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

    if (pIp != NULL) *pIp = dwIp;

	return IsInnerIp((in_addr_t)dwIp);
}
*/

/*
 * 根据网卡名字获取ip
 * 参数说明：
 *      sInterfaceName：网卡名字（如“eth1”、“en1”）
 *      pIp：网卡ip（网络序）出参
 * 返回值：
 *      pIp是否内网（1是）
 */
int GetIpByName(const char *sInterfaceName, uint32_t *pIp)
{
	struct sockaddr_in *sin = NULL;
	struct ifaddrs *ifa = NULL, *ifList;

	if (getifaddrs(&ifList) < 0) return -1;

	for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next) {
		if(ifa->ifa_addr->sa_family == AF_INET) {
			printf("\n>>> interfaceName: %s\n", ifa->ifa_name);

			sin = (struct sockaddr_in *)ifa->ifa_addr;
			printf(">>> ipAddress: %s\n", inet_ntoa(sin->sin_addr));

			sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
			printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));

			sin = (struct sockaddr_in *)ifa->ifa_netmask;
			printf(">>> subnetMask: %s\n", inet_ntoa(sin->sin_addr));

			if ((sInterfaceName == NULL && !strcmp(ifa->ifa_name, "en1"))
					|| (sInterfaceName != NULL  && !strcmp(ifa->ifa_name, sInterfaceName))) {
                if (pIp != NULL)
                    *pIp  = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;

				return IsInnerIp((in_addr_t)*pIp);
			}
		}
	}

	freeifaddrs(ifList);

	return -2;
}

/*
 * 根据ip、port生成sockaddr_in
 */
struct sockaddr_in * CreateAddr(const char *sIp, const char *sPort, const char *sSockType)
{
	static struct sockaddr_in address;
	struct in_addr *pstAddr = NULL;
	int iPort = 0;

	if (sIp == NULL)
		return NULL;

	pstAddr = atoaddr(sIp);
	iPort = atoport(sPort, sSockType);

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

	return CreateAddr(sIp, sPort, sSockType);
}
