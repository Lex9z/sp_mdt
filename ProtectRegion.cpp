#include "ProtectRegion.h"
#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif


ProtectRegion::ProtectRegion():mProtectRegionIni(NULL),mProtectRegionNameList(NULL)
{

}

ProtectRegion::~ProtectRegion()
{
	if (NULL != mProtectRegionNameList)
	{
		delete mProtectRegionNameList;
		mProtectRegionNameList = NULL; 
	}

	if (NULL != mProtectRegionIni)
	{
		delete mProtectRegionIni;
		mProtectRegionIni = NULL;
	}

	std::map<string, ProtectRegionInfo*>::iterator mapIterator = mProtectRegionMap.begin();
	for(; mapIterator != mProtectRegionMap.end(); mapIterator++)
	{
		delete mapIterator->second;
		mapIterator->second = NULL;
	}
	
}


void ProtectRegion::ReadProtectRegion(void)
{
	AnsiString iniFileNameWithPath = NULL;
	AnsiString asPath = NULL;

	if (getPathFromStr(Application->ExeName,asPath))
	{
		GC_TRACE("<ProtectRegion> ReadProtectRegion(): the tool path is %s", asPath);
		iniFileNameWithPath = asPath + "Protected_region.ini";
		if (!Check_FileExist(iniFileNameWithPath))
		{
			GC_TRACE("<ProtectRegion> ReadProtectRegion(): Can not find protected_region ini file. ");
			return;
		}
	}
try{
		mProtectRegionIni = new TIniFile(iniFileNameWithPath.c_str());
		if(NULL == mProtectRegionNameList)
		{
			mProtectRegionNameList = new TStringList();
			//Get all sections' name.
			mProtectRegionIni->ReadSections(mProtectRegionNameList);
			int count= mProtectRegionNameList->Count;

			for(int i=0; i<count; i++)
			{
				TStringList* mProtectRegionSectionList = new TStringList();
				
				mProtectRegionIni->ReadSection(mProtectRegionNameList->Strings[i], mProtectRegionSectionList);

                            ProtectRegionInfo *protectInfo = new ProtectRegionInfo()();
							
				if (mProtectRegionSectionList->Count != 3)
				{
					GC_TRACE("<ProtectRegion> ReadProtectRegion(): The section key number is wrong! ");
					return;
				}
				
				AnsiString imageName = mProtectRegionIni->ReadString(mProtectRegionNameList->Strings[i], mProtectRegionSectionList->Strings[0], STR_UNKNOWN);
                            protectInfo->SetImagePrefixName(imageName);
				GC_TRACE("<ProtectRegion> ReadProtectRegion(): The image name is %s .", imageName.c_str());

				std::string read_str = mProtectRegionIni->ReadString(mProtectRegionNameList->Strings[i], mProtectRegionSectionList->Strings[1], STR_UNKNOWN).c_str();
				GC_TRACE("<ProtectRegion> ReadProtectRegion() the mBackupFileName is %s .", this->mBackupFileName);
				protectInfo->SetBackupFileName(atoi(read_str.c_str()));

				read_str = mProtectRegionIni->ReadString(mProtectRegionNameList->Strings[i], mProtectRegionSectionList->Strings[2], STR_UNKNOWN).c_str();
				GC_TRACE("<ProtectRegion> ReadProtectRegion() the mChksumFileName is %s .", this->mChksumFileName);
				protectInfo->SetChksumFileName(atoi(read_str.c_str()));

                            mProtectRegionMap.insert(map<string, ImageDetailInfo*> :: value_type(mProtectRegionNameList->Strings[i].c_str(), protectInfo));

				if (NULL != mProtectRegionSectionList)
				{
					delete mProtectRegionSectionList;
					mProtectRegionSectionList = NULL; 
				}

			}

		} 
	} catch (const std::exception& ex) {
		GC_TRACE("<ProtectRegion> ReadProtectRegion: Read Protect Region failed, reason(%s).", ex.what());
		return ;
	}


}

map<string, ProtectRegionInfo*>* ProtectRegion::GetProtectRegionInfoMap(void )
{
	if(0 == mProtectRegionMap.size()) 
		ReadProtectRegion();		
	return &mProtectRegionMap;
}

