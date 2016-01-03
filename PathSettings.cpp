#include"PathSettings.h"
#include<windows.h>
#include <shlobj.h>
#include <cstdio>

PathSettings::PathSettings():m_Path("C:\\")
{

}

PathSettings::~PathSettings()
{

}

const string& PathSettings::getPath(void) const
{
	
	return m_Path;

}

void PathSettings::setUserDataPath(void)
{
	char szUserPath[MAX_PATH];
	if ( SHGetSpecialFolderPath(NULL, szUserPath, CSIDL_COMMON_APPDATA, 0) ) {
		m_Path = std::string(szUserPath) + "\\SP_MDT_Logs";
	}
}

string PathSettings::getTimestamp() const {
	char infoTimestamp[_MAX_FNAME] = { 0 };
	SYSTEMTIME timestamp;
	GetLocalTime(&timestamp);
	sprintf(infoTimestamp,
			"%02d-%02d-%02d-%02d-%02d-%02d",
			timestamp.wMonth, timestamp.wDay, timestamp.wYear, timestamp.wHour,
			timestamp.wMinute, timestamp.wSecond);
	return string(infoTimestamp);
}