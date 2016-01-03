#include "EfuseSettings.h"
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

#define EFUSE_SETTINGS "EfuseSettings"

EfuseSettings::EfuseSettings():mEfuseEnable(0), mEfuseSettingsEnable(0), mEfuseLockEnable(0), mEfuseReadBackEnable(0)
{

}

EfuseSettings::~EfuseSettings()
{

}

void EfuseSettings::ReadEfuseSettings(void)
{
    AnsiString iniFileNameWithPath = NULL;
    AnsiString asPath = NULL;
	AnsiString as_str;

    if (getPathFromStr(Application->ExeName,asPath))
    {
        GC_TRACE("<EfuseSettings> ReadEfuseSettings(): the tool path is %s", asPath);
        iniFileNameWithPath = asPath + "Efuse.ini";
        if (!Check_FileExist(iniFileNameWithPath))
        {
            GC_TRACE("<EfuseSettings> ReadEfuseSettings(): Can not find Efuse.ini file. ");
            return;
        }
    }

    TIniFile *ini;
    try
    {
       ini = new TIniFile(iniFileNameWithPath);
       if(ini != NULL)
       {
           as_str  = ini->ReadString(EFUSE_SETTINGS, "Enable", "0");
           mEfuseEnable = atoi(as_str.c_str());
		   as_str  = ini->ReadString(EFUSE_SETTINGS, "SettingsEnable", "0");
           mEfuseSettingsEnable = atoi(as_str.c_str());
		   as_str  = ini->ReadString(EFUSE_SETTINGS, "LockEnable", "0");
           mEfuseLockEnable = atoi(as_str.c_str());
		   as_str  = ini->ReadString(EFUSE_SETTINGS, "ReadBackEnable", "0");
           mEfuseReadBackEnable = atoi(as_str.c_str());

		   //dump
		   GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseEnable = %d .", mEfuseEnable);
		   GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseSettingsEnable = %d .", mEfuseSettingsEnable);
		   GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseLockEnable = %d .", mEfuseLockEnable);
		   GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseReadBackEnable = %d .", mEfuseReadBackEnable);
		   
           delete ini;
       }
    }
    catch (...)
    {
        ShowMessage( "read setup file " + iniFileNameWithPath + " error");
        delete ini;
    }

/*
    TIniFile* efuseSettingsIni;
    TStringList* str_list = NULL;
    try {
		str_list = new TStringList;
		efuseSettingsIni = new TIniFile(iniFileNameWithPath.c_str());
		efuseSettingsIni->ReadSection(EFUSE_SETTINGS, str_list);

		if ( 4 > str_list->Count )
		{
			GC_TRACE("<EfuseSettings> ReadEfuseSettings() the config numbers is smaller than 4.");
			return;
		}

		this->mEfuseEnable = atoi(efuseSettingsIni->ReadString(EFUSE_SETTINGS, str_list->Strings[0], "0").c_str());
		GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseEnable is %d .", this->mEfuseEnable);
		this->mEfuseSettingsEnable = atoi(efuseSettingsIni->ReadString(EFUSE_SETTINGS, str_list->Strings[1], "0").c_str());
		GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseSettingsEnable is %d .", this->mEfuseSettingsEnable);
		this->mEfuseLockEnable = atoi(efuseSettingsIni->ReadString(EFUSE_SETTINGS, str_list->Strings[2], "0").c_str());
		GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseLockEnable is %d .", this->mEfuseLockEnable);
		this->mEfuseReadBackEnable = atoi(efuseSettingsIni->ReadString(EFUSE_SETTINGS, str_list->Strings[3], "0").c_str());
		GC_TRACE("<EfuseSettings> ReadEfuseSettings() the mEfuseReadBackEnable is %d .", this->mEfuseReadBackEnable);

    } catch (const std::exception& ex) {
        if (NULL != str_list) {
            delete str_list;
            str_list = NULL;
        }
        if (NULL != efuseSettingsIni) {
            delete efuseSettingsIni;
            efuseSettingsIni = NULL;
        }
        GC_TRACE("<EfuseSettings> ReadEfuseSettings() failed, reason(%s).", ex.what());
    }

    if (NULL != str_list)
    {
        delete str_list;
        str_list = NULL;
    }
    if (NULL != efuseSettingsIni)
    {
        delete efuseSettingsIni;
        efuseSettingsIni = NULL;
    }*/
}