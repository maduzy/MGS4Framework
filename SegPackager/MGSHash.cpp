#include <iostream>
#include "MGSHash.h"

int MGSHash::GetIDFromString(const char *str)
{
	int hash = 0;
	for (; *str; str++)
	{
		int v1 = hash >> 19;
		int v0 = hash << 5;
		hash = ((v0 | v1) + *str) & 0xFFFFFF;
	}
	if (hash) return hash;
	return 1;
}