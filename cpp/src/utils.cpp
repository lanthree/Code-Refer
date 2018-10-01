#include "utils.h"

#include <time.h>

std::string lan_tools::utils::get_Ymd()
{
	static time_t pre = 0;
	static char date[16];

	time_t now = time(NULL);
	if (now - pre >= 1) {
		struct tm t;
		localtime_r(&now, &t);
		strftime(date, sizeof(date), "%Y-%m-%d", &t);
	}
	pre = now;
	return date;
}

std::string lan_tools::utils::get_YmdHMS()
{
	static time_t pre = 0;
	static char date[32];

	time_t now = time(NULL);
	if (now - pre >= 1) {
		struct tm t;
		localtime_r(&now, &t);
		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &t);
	}
	pre = now;
	return date;
}
