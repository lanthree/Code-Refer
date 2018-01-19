#include "log.h"
#include "utils"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

using namespace lan_tools;

LogFile::

Log::Log(const std::string& pathname, long long maxsize_eachfile)
	: pathname_(pathname),
	  maxsize_eachfile_(maxsize_eachfile) {

	int fd = -1;

	if (pathname_ == "stdout") {
		fd = 1;
	} else if (pathname_ == "stderr") {
		fd = 2;
	} else {
		// 取当前
		pathname_.append(".").append(utils::get_yymmdd()).append(".0");

		if (access(pathname, F_OK | R_OK | W_OK) == 0) {
			fd = open(pathname, O_RDWR | O_APPEND, 0644);
		} else {
			fd = open(pathname, O_RDWR | O_APPEND | O_CREAT, 0644);
		}
	}

	if (fd == -1) {
		throw std::exception("log file open failed: " + strerror(errno));
	}

	pathname_2_logfile_.insert(
		std::make_pair(pathname_, Logfile(fd)));
}
