#ifndef IMAGE_DETAIL_INFO_H
#define IMAGE_DETAIL_INFO_H

#include<string>
#include "multiport_common.h"
using namespace std;

#define UNKNOWN_FORMAT -1
#define UNKNOWN_TYPE -1

class ImageDetailInfo
{
public:
	ImageDetailInfo();
	~ImageDetailInfo(){};

	const AnsiString& GetImagePrefixName(void) const {return imagePrefixName;}
	const int GetBromFormat(void) const {return brom_format;}
	const int GetEbootFormat(void) const {return eboot_format;}
	const int GetImageType(void) const {return img_type;}
	const AnsiString GetImagePath(void) const{return mImagePath[0];};
	const AnsiString GetImageIniPath(void) const{return mImageIniPath[0];}
	const _BOOL GetImageChecked(void) const{return mImageChecked[0];}

	void SetImagePrefixName(AnsiString imageName);
	void SetBromFormat(int bromFormat);
	void SetEbootFormat(int ebootFormat);
	void SetImageType(int imageType);
	void SetImagePath(AnsiString imagePath);
	void SetImageIniPath(AnsiString imageIniPath);
	void SetImageChecked(_BOOL imageChecked);

private:
	AnsiString imagePrefixName;
	int brom_format;
	int eboot_format;
	int img_type;

	AnsiString mImagePath[MAX_SUPPORT_LOAD_NUM];
	AnsiString mImageIniPath[MAX_SUPPORT_LOAD_NUM];
	_BOOL mImageChecked[MAX_SUPPORT_LOAD_NUM];
};

#endif