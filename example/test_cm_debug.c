#include <stdio.h>
#include "cm_debug.h"

int main()
{
	char sBuf[] = "hello world!!!  12345678910";

	printf("dump:\n%s\n", DumpPackage(sBuf, sizeof(sBuf)));

	printf("dump:\n%s\n", DumpHex(sBuf, sizeof(sBuf)));



	return 0;
}
