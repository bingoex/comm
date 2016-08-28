#include "cm_net.h"

struct in_addr *atoaddr(const char *sAddress)
{
	static struct in_addr stSaddr;
	struct hostent *pHost;

	//if sAddress == XXX.XXX.XXX.XXX
	stSaddr.s_addr = inet_addr(sAddress);
	if (stSaddr.s_addr != -1)
		return &stSaddr;

	pHost = gethostbyname(sAddress);
	if (pHost != NULL)
		return (struct in_addr *) *(pHost->h_addr_list);

	return NULL;
}
