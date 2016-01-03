#include "ProtectRegionInfo.h"

ProtectRegionInfo::ProtectRegionInfo()
{

	this->backup_file_name ="";
	this->chksum_file_name = "";
	this->imagePrefixName = "";
}
void ImageDetailInfo::SetImagePrefixName(AnsiString imageName)
{
	this->imagePrefixName = imageName;
}
void ImageDetailInfo::SetBackupFileName(AnsiString backupFileName)
{
	this->backup_file_name = backupFileName;	
}

void ImageDetailInfo::SetChksumFileName(AnsiString chksumFileName)
{
	this->chksum_file_name = chksumFileName;	
}
