/*
 * NvramBackupMap.cpp
 *
 *  Created on: 2010-12-21
 *      Author: MTK80762
 */
#include <vcl.h>
#pragma hdrstop

#pragma warn -8004
#include <stdio.h>
#include <IniFiles.hpp>
#include "NvramBackupMap.h"
#include "SharedPtr.h"


const string NvramBackupMap::FOLDER_CALIBRAT("CALIBRAT");
const string NvramBackupMap::FOLDER_SML("SML");
const string NvramBackupMap::FOLDER_CORE("CORE");
const string NvramBackupMap::FOLDER_DATA("DATA");
const string NvramBackupMap::FOLDER_IMEI("IMEI");
const string NvramBackupMap::FOLDER_APRDCL("APRDCL");
const string NvramBackupMap::FOLDER_APRDEB("APRDEB");

NvramBackupMap::NvramBackupMap(const string& iniFile)
        : m_strIniFile(iniFile)
{
	m_nDelCount = 0;
	m_bIsReadNvramBackupINISuccess = false;
}

NvramBackupMap::~NvramBackupMap()
{
}

void NvramBackupMap::ResetNvramBackupMap()
{
	m_backupFilterFolderNamesMap.clear();
    m_backupFilterFolderNamesList.clear();
	m_strIniFile = "";
	m_strFolderNames = "";
	m_nDelCount = 0;
	m_bIsReadNvramBackupINISuccess = false;
}

string NvramBackupMap::GetNvramBackupFilterFloderNames(void) const
{
	return m_strFolderNames;
}

int NvramBackupMap::GetFilterFloderCount(void) const
{
	return m_nDelCount;
}

bool NvramBackupMap::IsReadNvramBackupINISuccess(void) const
{
	return m_bIsReadNvramBackupINISuccess;
}

void  NvramBackupMap::MapNvramBackupFilterNames(const string& strFolderName)
{
	if(strFolderName == FOLDER_SML )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_SML, "IMPORTNT"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_SML]);
	}
	else if( strFolderName == FOLDER_IMEI )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_IMEI, "NVD_IMEI"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_IMEI]);
	}
	else if( strFolderName == FOLDER_CALIBRAT )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_CALIBRAT, "CALIBRAT"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_CALIBRAT]);
	}
	else if( strFolderName == FOLDER_CORE )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_CORE, "NVD_CORE"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_CORE]);
	}
	else if( strFolderName == FOLDER_DATA )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_DATA, "NVD_DATA"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_DATA]);
	}
	else if( strFolderName == FOLDER_APRDCL )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_APRDCL, "APRDCL"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_APRDCL]);
	}
	else if( strFolderName == FOLDER_APRDEB )
	{
		m_backupFilterFolderNamesMap.insert(pair<string, string>(FOLDER_APRDEB, "APRDEB"));
		m_backupFilterFolderNamesList.push_back(m_backupFilterFolderNamesMap[FOLDER_APRDEB]);
	}
}

void NvramBackupMap::UpdateNvramBackupINIStatus(void)
{
	ReadNvramBackupINI();
	ComposeNvramBackupFilterNames();
}

void  NvramBackupMap::ComposeNvramBackupFilterNames(void)
{
    for(list<string>::const_iterator it = m_backupFilterFolderNamesList.begin();
    		it != m_backupFilterFolderNamesList.end(); it++)
    {
    	m_strFolderNames += *it;
    	m_strFolderNames += ";";
    }
}

void NvramBackupMap::ReadNvramBackupINI(void)
{
	char szCurPath[MAX_PATH] = "";
	string strCurPath = "";
    string strFolderName = "";

    int nDelCount = 0;
	GetModuleFileName(NULL, szCurPath, MAX_PATH);
    strCurPath = szCurPath;
	strCurPath = strCurPath.substr(0, strCurPath.rfind('\\'));
	strCurPath = strCurPath + "\\" + m_strIniFile;

	SharedPtr<TIniFile> ini(new TIniFile(strCurPath.c_str()));

    nDelCount = ini->ReadInteger( "Backup/Restore File Name-DELETE", "folder_count", 0);
    if(0 == nDelCount)
    {
        m_nDelCount = nDelCount;
        m_strFolderNames = "";
        m_bIsReadNvramBackupINISuccess = true;
    }
    else if(nDelCount > 0)
    {
    	m_nDelCount = nDelCount;
    	for(int i = 0; i < nDelCount; i++)
        {
    		AnsiString folderName = "folder_name"+IntToStr(i+1);
    		strFolderName =
    				ini->ReadString( "Backup/Restore File Name-DELETE", folderName.c_str(), "").c_str();
    		MapNvramBackupFilterNames(strFolderName);
        }
    	m_bIsReadNvramBackupINISuccess = true;
    }
    else
    {
    	m_bIsReadNvramBackupINISuccess = false;
    }
}
