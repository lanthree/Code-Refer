#ifndef __LOG_H__
#define __LOG_H__

#include <string>

enum class LOG_LEVEL {
	ERROR, WARN, INFO, DEBUG
};

class Log
{
public:
	Log(const std::string& pathname, long long maxsize_eachfile);

	Print(const std::string& file

private:
	std::string pathname_;
	long long maxsize_eachfile_;

};

#endif
