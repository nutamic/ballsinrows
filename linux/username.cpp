#include <QString>
#include <pwd.h>
#include <unistd.h>
QString getUserName(){
	passwd *pwd = getpwuid(getuid());
	return *pwd->pw_gecos ? pwd->pw_gecos : pwd->pw_name;
}
