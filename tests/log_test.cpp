#include <iostream>
#include "log.h"

int main()
{
	for (int i = 0; i < 100; ++i)
		DEBUG("%d", i);

	for (int i = 0; i < 100; ++i)
		ERROR("%d", i);
}
