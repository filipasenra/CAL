/*
 * Factorial.cpp
 */

#include "Factorial.h"

int factorialRecurs(int n)
{
	int a = 0;

	if (n <= 2)
		a = 2;
	else
	{
		a = n*factorialRecurs(n-1);
	}

	return a;
}

int factorialDinam(int n)
{
	int a = 1;

	while (n > 1)
	{
		a*=n;
		n--;
	}

	return a;
}
