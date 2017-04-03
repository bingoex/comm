#include "cm_math.h"

int IsPrime(int iNum)
{
	int i = 0;
	for (i = 2; i <= iNum / 2; i++)
		if (0 == iNum % i)
			return 0;

	return 1;
}


int GenPrimeArray(int iMax, int iNum, int array[])
{
	int i = 0;
	for (; iMax > 0 && iNum > 0; iMax--) {
		if (IsPrime(iMax)) {
			array[i++] = iMax;
			iNum--;
		}
	}

	return 0;
}

/*
#include<stdio.h>
int main()
{
	printf("IsPrime(%d) %d\n", 5, IsPrime(5));
	printf("IsPrime(%d) %d\n", 6, IsPrime(6));
	int primes[10];
	GenPrimeArray(65535, 10, primes);
	for (int i = 0; i< 10; i++) {
		printf("primies[%d] = %d\n", i, primes[i]);
	}

	return 0;
}
*/
