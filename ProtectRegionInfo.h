#ifndef PROTECT_REGION_INFO_H
#define PROTECT_REGION_INFO_H
#include<string>
#include "multiport_common.h"
using namespace std;

//#define UNKNOWN_FORMAT -1
//#define UNKNOWN_TYPE -1

class ProtectRegionInfo
{
public:
	ProtectRegionInfo();
	~ProtectRegionInfo(){};

	const AnsiString& GetImagePrefixName(void) const {return imagePrefixName;}
	//const int GetBromFormat(void) const {return brom_format;}
	//const int GetEbootFormat(void) const {return eboot_format;}
	const AnsiString GetBackupFileName(void) const{return backup_file_name;};
	const AnsiString GetChksumFileName(void) const{return chksum_file_name;}
	//const _BOOL GetImageChecked(void) const{return mImageChecked[0];}

	void SetImagePrefixName(AnsiString imageName);
	void SetBackupFileName(AnsiString backupFileName);
	void SetChksumFileName(AnsiString chksumFileName);
	//void SetImagePath(AnsiString imagePath);
	//void SetImageIniPath(AnsiString imageIniPath);
private:
	AnsiString backup_file_name;
	AnsiString chksum_file_name;
	AnsiString imagePrefixName;

	//AnsiString mImagePath[MAX_SUPPORT_LOAD_NUM];
	//AnsiString mImageIniPath[MAX_SUPPORT_LOAD_NUM];
	//_BOOL mImageChecked[MAX_SUPPORT_LOAD_NUM];
};

#endif