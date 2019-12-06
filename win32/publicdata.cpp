#include <QString>
#include <aclapi.h>
#include <shlobj.h>
constexpr wchar_t pathBallsInRows[] = L"\\BallsInRows";
constexpr wchar_t pathRecords[] = L"\\records.xml";
bool createPublicDataDir(wchar_t *path);
bool getPublicRecordsPath(QString &path){
	wchar_t wpath[MAX_PATH];
	if(SHGetFolderPath(nullptr, CSIDL_COMMON_APPDATA, nullptr, CSIDL_FLAG_CREATE, wpath)) return false;
	if(wcscat_s(wpath, pathBallsInRows)) return false;
	const DWORD attributes = GetFileAttributes(wpath);
	if(!(attributes & FILE_ATTRIBUTE_DIRECTORY)) return false;
	if(attributes == INVALID_FILE_ATTRIBUTES){
		if(!createPublicDataDir(wpath)) return false;
	}
	if(wcscat_s(wpath, pathRecords)) return false;
	path = QString::fromWCharArray(wpath);
	return true;
}
bool createPublicDataDir(wchar_t *path){
	SID sid;
	SID_IDENTIFIER_AUTHORITY world = SECURITY_WORLD_SID_AUTHORITY;
	if(!InitializeSid(&sid, &world, 1)) return false;
	*GetSidSubAuthority(&sid, 0) = SECURITY_WORLD_RID;
	EXPLICIT_ACCESS access;
	access.grfAccessPermissions = GENERIC_ALL;
	access.grfAccessMode = SET_ACCESS;
	access.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	access.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
	access.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	access.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	access.Trustee.ptstrName = LPWSTR(&sid);
	PACL pacl;
	if(SetEntriesInAcl(1, &access, nullptr, &pacl)) return false;
	SECURITY_DESCRIPTOR sd;
	bool success = InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	if(success) success = SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);
	if(success){
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof sa;
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;
		success = CreateDirectory(path, &sa);
	}
	LocalFree(pacl);
	return success;
}
