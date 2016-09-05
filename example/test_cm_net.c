#include <stdio.h>

#include "cm_process.h"
#include "cm_net.h"

int main()
{
	//atoaddr
	struct in_addr * stAddr = atoaddr("127.0.0.1");
	printf("in_addr %u %u\n", stAddr->s_addr, ntohl(stAddr->s_addr));

	stAddr = atoaddr("www.baidu.com");
	printf("in_addr %u %u\n", stAddr->s_addr, ntohl(stAddr->s_addr));

	//atoport
	int iPort = 0;
	iPort = atoport("mysql", "tcp");
	printf("iPort %d\n", iPort);
	iPort = atoport("8080", "udp");
	printf("iPort %d\n", iPort);

	uint32_t dwIp = 0;
	int iRet = 0;
	iRet = GetIpByName("en0", &dwIp);
	printf("GetLocalIp ret %d dwip %u %u\n", iRet, dwIp, ntohl(dwIp));
	

	//CreateSocketRaw
	DaemonInit();
	int iListenSock = 0;
	iRet = CreateSocketRaw(SOCK_DGRAM, &iListenSock, htonl(INADDR_ANY), iPort, 1);
	printf("iRet %d iListenSock %d\n", iRet, iListenSock);

	//CreateSocket
	iRet = CreateSocket("tcp", &iListenSock, "192.168.1.110", "8081", 0);
	printf("iRet %d iListenSock %d\n", iRet, iListenSock);

	sleep(20);






	return 0;
}
