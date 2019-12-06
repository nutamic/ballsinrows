#include <QString>
#include <windows.h>
#define USER_NAME_LENGTH 260
QString getUserName(){
	wchar_t userName[USER_NAME_LENGTH];
	DWORD length = USER_NAME_LENGTH;
	GetUserName(userName, &length);
	return QString::fromWCharArray(userName);
}
