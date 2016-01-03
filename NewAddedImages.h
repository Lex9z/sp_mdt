#ifndef NEW_ADDED_IMAGES_H
#define NEW_ADDED_IMAGES_H

#include <string>
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#include <map>
#include "GCTrace.h"
#include "ImageDetailInfo.h"

using namespace std;

#define IMAGE_SECTION_NAME "AddedImages"
#define NAME_UNKNOWN "unknown"

class NewAddedImages
{
public:
	NewAddedImages();
	~NewAddedImages();

	//void ReadImagesFromIni(void);

	void GetNewImages(void);

	map<string, ImageDetailInfo*>* GetImageMap(void);

private:
	TIniFile* mImagesIni;
	TStringList* mImageScatterNameList;
	TStringList* mImagePrefixNameList;
	
	std::map<string, ImageDetailInfo*> mImageMap;


};

#endif