#ifndef _CM_NET_H_
#define _CM_NET_H_



#include <unistd.h> //close
#include <ifaddrs.h> //getifaddrs ifaddrs freeifaddrs
#include <arpa/inet.h> //inet_addr
#define NO_BLOCK 0 //其他框架会使用到
/*
#ifdef __cplusplus
extern "C" {
#endif 
*/

/*
 *
 * struct in_addr {
 *     in_addr_t s_addr; //32位网络序
 *  };
 *
 *  struct sockaddr_in{
 *      short int sin_family;     
 *      unsigned short int sin_port;     
 *      struct in_add sin_addr;     
 *      unsigned char sin_zero[8];     
 *  };  
 */

/*
 * char * 转换为in_addr
 * 支持ip字符串（xxx.xxx.xxx.xxx）和 hostname
 */
struct in_addr *atoaddr(const char *sAddress);

/*
 * char * 转换为端口
 * 支持端口数字字符串（“8080”）和 getservbyname
 */
int atoport(const char *sService, const char *sProto);

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
int CreateSocketRaw(int iSockType, int *pListener, uint32_t dwListenIp, int iPort, int iIsNeedBlock);

int CreateSocket(const char *sSockType, int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);

int CreateTcpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);
int CreateUdpSocket(int *pListener, const char *sListenIp, const char *sPort, int iIsNeedBlock);

int CreateTcpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock);
int CreateUdpSocketEx(int *pListener, const char *sListenIp, int iPort, int iIsNeedBlock);

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
int CreateClientSocketRaw(int iSockType, int *piSocket, uint32_t dwServerIp, int iPort, int iIsNeedBlock);

int CreateClientSocket(const char *sSockType, int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);

int CreateTcpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);
int CreateTcpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock);

int CreateUdpClientSocket(int *piSocket, const char *sServerIp, const char *sPort, int iIsNeedBlock);
int CreateUdpClientSocketEx(int *piSocket, const char *sServerIp, int iPort, int iIsNeedBlock);

/*
 * 判断dwip是否为内网
 */ 
int IsInnerIp(in_addr_t dwIp);

/*
 * 根据网卡名字获取ip
 * 参数说明：
 *      sInterfaceName：网卡名字（如“eth1”、“en1”）
 *      pIp：网卡ip（网络序）出参
 * 返回值：
 *      pIp是否内网（1是）
 */
int GetIpByName(const char *sInterfaceName, uint32_t *pIp);
//int GetLocalIp(const char *sInterfaceName, uint32_t *pIp)

/*
 * 设置fd为非阻塞
 */
int SetNBLock(int iSock);

/*
 * 根据ip、port生成sockaddr_in
 */
struct sockaddr_in * CreateAddr(const char *sIp, const char *sPort, const char *sSockType);
struct sockaddr_in * CreateAddrEx(const char *sIp, int iPort, const char *sSockType);

/*
#ifdef __cplusplus
}
#endif 
*/



#endif
