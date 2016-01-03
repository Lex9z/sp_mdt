#include "CustomerSetup.h"
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif


CustomerSetup::CustomerSetup():mPreloaderPid("2000"),mDAHighSpeedPid("2001"),
mBromPid("0003"),mAndroidVid("0E8D"),mGUIDFlag(0)
{

}

CustomerSetup::~CustomerSetup()
{
}

const string& CustomerSetup::GetPreloaderPid(void) const
{
	return mPreloaderPid;
}

const string& CustomerSetup::GetDAHighSpeedPid(void) const
{
	return mDAHighSpeedPid;
}

const string& CustomerSetup::GetBromPid(void) const
{

	return mBromPid;
}

const string& CustomerSetup::GetAndroidVid(void) const
{
	return mAndroidVid;
}

const GUID& CustomerSetup::GetGUID(void) const
{
	if (CLASS_GUID == mGUIDFlag) 
	{
		return Class_Guid_ANDROID_USB2Ser;
	}
	else 
	{
		return Interface_Guid_ANDROID_USB2Ser;
	}

}

void CustomerSetup::ReadCustomerSettings(void)
{
	AnsiString iniFileNameWithPath = NULL;
	AnsiString asPath = NULL;

	if (getPathFromStr(Application->ExeName,asPath))
	{
		GC_TRACE("<CustomerSetup> ReadCustomerSettings(): the tool path is %s", asPath);
		iniFileNameWithPath = asPath + "Customer.ini";
		if (!Check_FileExist(iniFileNameWithPath))
		{
			GC_TRACE("<CustomerSetup> ReadCustomerSettings(): Can not find Customer ini file. ");
			return;
		}
	}

	TIniFile* customer_setup_ini_;
	TStringList* str_list = NULL;
	try {
		str_list = new TStringList;
		customer_setup_ini_ = new TIniFile(iniFileNameWithPath.c_str());
		customer_setup_ini_->ReadSection("IDConfig", str_list);
		
		if ( 4 != str_list->Count )
		{
			GC_TRACE("<CustomerSetup> ReadCustomerSettings() the config numbers is smaller than 4.");
			return;
		}
		
		this->mPreloaderPid = customer_setup_ini_->ReadString("IDConfig", str_list->Strings[0], "2000").c_str();
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() the mPreloaderPid is %s .", this->mPreloaderPid);
		this->mDAHighSpeedPid = customer_setup_ini_->ReadString("IDConfig", str_list->Strings[1], "2001").c_str();
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() the mDAHighSpeedPid is %s .", this->mDAHighSpeedPid);
		this->mBromPid = customer_setup_ini_->ReadString("IDConfig", str_list->Strings[2], "0003").c_str();
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() the mBromPid is %s .", this->mBromPid);
		this->mAndroidVid = customer_setup_ini_->ReadString("IDConfig", str_list->Strings[3], "0E8D").c_str();
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() the mAndroidVid is %s .", this->mAndroidVid);
		
		this->mGUIDFlag = customer_setup_ini_->ReadString("GUIDFlag", "flag", "0").ToInt();
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() the mGUIDFlag is %d .", this->mGUIDFlag);
		

	} catch (const std::exception& ex) {
		if (NULL != str_list) {
		    delete str_list;
			str_list = NULL;
		}
		if (NULL != customer_setup_ini_) {
		    delete customer_setup_ini_;
			customer_setup_ini_ = NULL;
		}
		GC_TRACE("<CustomerSetup> ReadCustomerSettings() failed, reason(%s).", ex.what());
	}

	if (NULL != str_list)
	{
	    delete str_list;
		str_list = NULL;
	}
	if (NULL != customer_setup_ini_) 
	{
	    delete customer_setup_ini_;
		customer_setup_ini_ = NULL;
	}
}