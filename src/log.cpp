#include "log.h"
#include "utils.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include <iostream>

using namespace lan_tools;

LogFile::LogFile() : cut_cnt(0),
	  last_write_Ymd(utils::get_Ymd()) {}

std::string LogFile::get_fileappend()
{
	std::stringstream ss;
	ss << "." << last_write_Ymd << "." << cut_cnt;
	return ss.str();
}


std::mutex Log::mutex_;
std::map<std::string, std::shared_ptr<LogFile>> Log::pathname_2_logfile_;

Log::Log(const std::string& pathname, long long maxsize_eachfile)
	: pathname_(pathname),
	  maxsize_eachfile_(maxsize_eachfile) {

	std::lock_guard<std::mutex> guard(mutex_);

	if (pathname_2_logfile_.count(pathname_)) {
		return;
	}
	pathname_.append(".").append(utils::get_Ymd()).append(".0");

	pathname_2_logfile_.insert(
		std::make_pair(pathname_,
			std::make_shared<LogFile>()
		)
	);

	pathname_2_logfile_[pathname_]->fs_.open(
		pathname_, std::ios::out|std::ios::app);
}

Log::~Log() {}


int Log::print(const std::string& file, int line, LOG_LEVEL level,
	const char *fmt, ...)
{
	std::lock_guard<std::mutex> guard(
		pathname_2_logfile_[pathname_]->mutex_
	);

	//roll_file();

	static char buf[10240];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	pathname_2_logfile_[pathname_]->fs_ << "[" << utils::get_YmdHMS() << "] " 
		<<  buf << std::endl;
}

Log& lan_tools::get_syslog()
{
	static Log syslog("../log/sys.log", 4);
	return syslog;
}
