#ifndef __NVRAM_BACKUP_MAP_H
#define __NVRAM_BACKUP_MAP_H

#include <string>
#include <list>
#include <map>

using std::string;
using std::list;
using std::map;
using std::pair;

class NvramBackupMap
{
public:
	NvramBackupMap(const string &iniFile = "NVRAM_BACKUP.INI");
	~NvramBackupMap();
	string GetNvramBackupFilterFloderNames() const;
	int GetFilterFloderCount() const;
    bool IsReadNvramBackupINISuccess() const;
    void UpdateNvramBackupINIStatus();
    void ResetNvramBackupMap();

private:
	void  ReadNvramBackupINI();
	void  MapNvramBackupFilterNames(const string& strFolderName);
    void  ComposeNvramBackupFilterNames();
	map<string, string> m_backupFilterFolderNamesMap;
    list<string> m_backupFilterFolderNamesList;
	string m_strIniFile;
	string m_strFolderNames;
	int m_nDelCount;
	bool m_bIsReadNvramBackupINISuccess;
	//Nvram Mapping folder Names
	static const string FOLDER_SML;
	static const string FOLDER_IMEI;
	static const string FOLDER_CALIBRAT;
	static const string FOLDER_CORE;
	static const string FOLDER_DATA;
	static const string FOLDER_APRDCL;
	static const string FOLDER_APRDEB;
};

#endif //__NVRAM_BACKUP_MAP_H
