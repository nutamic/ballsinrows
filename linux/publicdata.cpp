#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "appdata.h"
#define PUBLIC_DATA_MODE 0664
#ifndef PUBLIC_RECORDS_PATH
#define PUBLIC_RECORDS_PATH "/var/lib/ballsinrows/records.xml"
#endif
bool getPublicRecordsPath(QString &path){
	const mode_t prevMask = umask(0);
	const int fd = open(PUBLIC_RECORDS_PATH, O_CREAT | O_RDWR, PUBLIC_DATA_MODE);
	umask(prevMask);
	if(fd == -1) return false;
	close(fd);
	path = PUBLIC_RECORDS_PATH;
	return true;
}
