#include "ImageDetailInfo.h"

ImageDetailInfo::ImageDetailInfo()
{

	this->brom_format = UNKNOWN_FORMAT;
	this->eboot_format = UNKNOWN_FORMAT;
	this->img_type = UNKNOWN_TYPE;
	this->imagePrefixName = "";
}

void ImageDetailInfo::SetBromFormat(int bromFormat)
{
	this->brom_format = bromFormat;
}

void ImageDetailInfo::SetEbootFormat(int ebootFormat)
{
	this->eboot_format = ebootFormat;
}

void ImageDetailInfo::SetImagePrefixName(AnsiString imageName)
{
	this->imagePrefixName = imageName;
}

void ImageDetailInfo::SetImageType(int imageType)
{
	this->img_type = imageType;
}

void ImageDetailInfo::SetImagePath(AnsiString imagePath)
{
	this->mImagePath[0] = imagePath;	
}

void ImageDetailInfo::SetImageIniPath(AnsiString imageIniPath)
{
	this->mImageIniPath[0] = imageIniPath;	
}

void ImageDetailInfo::SetImageChecked(_BOOL imageChecked)
{
	this->mImageChecked[0] = imageChecked;	
}