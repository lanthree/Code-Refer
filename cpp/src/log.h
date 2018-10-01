#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <mutex>
#include <map>
#include <sstream>
#include <fstream>
#include <memory>

#define DEBUG(fmt, ...)\
do {\
	lan_tools::get_syslog().print(__FILE__, __LINE__, lan_tools::LOG_LEVEL::DEBUG, fmt, ##__VA_ARGS__);\
} while(0)


#define ERROR(fmt, ...)\
do {\
	lan_tools::get_syslog().print(__FILE__, __LINE__, lan_tools::LOG_LEVEL::ERROR, fmt, ##__VA_ARGS__);\
} while(0)

namespace lan_tools {

enum class LOG_LEVEL {
	ERROR, WARN, INFO, DEBUG
};

struct LogFile {

	LogFile();

	std::string get_fileappend();

	std::fstream fs_;
	std::mutex mutex_;

	std::string last_write_Ymd;
	int cut_cnt;
};

// You should not init Log obj in thread.
class Log
{
public:
	Log(const std::string& pathname, size_t maxsize_eachfile); // MB
	~Log();

	int print(const std::string& file, int line, LOG_LEVEL level,
		const char *fmt, ...);

private:
	void roll_file();

private:
	std::string pathname_;
	size_t maxsize_eachfile_;

	static std::mutex mutex_;
	static std::map<std::string, std::shared_ptr<LogFile>> pathname_2_logfile_;
};

Log& get_syslog();

}

#endif
