#include <iostream>
#include "log.h"

int main()
{
	int cnt = 0;
	while (cnt < 20) {
		DEBUG("++++++++++++++%d", cnt++);
		ERROR("--------------%d", cnt++);
	}

}
