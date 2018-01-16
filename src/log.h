#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <mutex>
#include <map>

#define DEBUG(fmt, ...)\
do {\
	lan_tools::sys_log.print(__FILE__, __LINE__, lan_tools::LOG_LEVEL::DEBUG, fmt, ##__VA_ARGS__);\
} while(0)


#define ERROR(fmt, ...)\
do {\
	lan_tools::sys_log.print(__FILE__, __LINE__, lan_tools::LOG_LEVEL::ERROR, fmt, ##__VA_ARGS__);\
} while(0)

namespace lan_tools {

enum class LOG_LEVEL {
	ERROR, WARN, INFO, DEBUG
};

// You should not init Log obj in thread.

struct LogFile{

	LogFile(int fd_);

	int fd_;
	std::mutex mutex_;

	std::string last_write_yymmdd;
	int cut_cnt;
};

class Log
{
public:
	Log(const std::string& pathname, long long maxsize_eachfile); // MB
	~Log();

	print(const std::string& file, int line, LOG_LEVEL level,
		const char *fmt, ...);

private:
	std::string pathname_;
	long long maxsize_eachfile_;

	static std::map<std::string, LogFile> pathname_2_logfile_;
};

static Log sys_log("../log/sys.log", 4);

}

#endif
