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

Log::Log(const std::string& pathname, size_t maxsize_eachfile)
	: pathname_(pathname),
	  maxsize_eachfile_(maxsize_eachfile) {

	std::lock_guard<std::mutex> guard(mutex_);

	if (pathname_2_logfile_.count(pathname_)) {
		return;
	}

	pathname_2_logfile_.insert(
		std::make_pair(pathname_,
			std::make_shared<LogFile>()
		)
	);

	pathname_2_logfile_[pathname_]->fs_.open(
		pathname_ + pathname_2_logfile_[pathname_]->get_fileappend(), 
		std::ios::out|std::ios::app);
}

Log::~Log() {}


int Log::print(const std::string& file, int line, LOG_LEVEL level,
	const char *fmt, ...)
{
	std::lock_guard<std::mutex> guard(
		pathname_2_logfile_[pathname_]->mutex_
	);

	roll_file();

	pathname_2_logfile_[pathname_]->fs_ << "\e[1;34m[" << utils::get_YmdHMS() << "] \e[m";

	switch (level) {
		case LOG_LEVEL::DEBUG:
			pathname_2_logfile_[pathname_]->fs_ << "DEBUG ";
			break;
		case LOG_LEVEL::ERROR:
			pathname_2_logfile_[pathname_]->fs_ << "\e[1;31mERROR ";
			break;
	}

	static char buf[10240];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	pathname_2_logfile_[pathname_]->fs_ <<  buf << std::endl;
}

void Log::roll_file()
{
	auto& logfile = pathname_2_logfile_[pathname_];

	while(true) {	

		// can be cached in LogFile ?
		logfile->fs_.seekp(0, logfile->fs_.end);
		size_t filesize = logfile->fs_.tellp();

		if (filesize > maxsize_eachfile_*1024*1024) {
			// file over size
			logfile->fs_.close();
			++ logfile->cut_cnt;

			logfile->fs_.open(
				pathname_ + logfile->get_fileappend(),
				std::ios::out|std::ios::app);

		} else if (logfile->last_write_Ymd != utils::get_Ymd()) {
			// new day
			logfile->fs_.close();
			logfile->last_write_Ymd = utils::get_Ymd();
			logfile->cut_cnt = 0;

			logfile->fs_.open(
				pathname_ + logfile->get_fileappend(),
				std::ios::out|std::ios::app);
		} else {
			return;
		}
	}
}


Log& lan_tools::get_syslog()
{
	static Log syslog("../log/sys.log", 4);
	return syslog;
}
