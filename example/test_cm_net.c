#include <stdio.h>
#include "cm_net.h"

int main()
{
	struct in_addr * stAddr = atoaddr("127.0.0.1");

	printf("in_addr %u %u\n", stAddr->s_addr, ntohl(stAddr->s_addr));

	return 0;
}
