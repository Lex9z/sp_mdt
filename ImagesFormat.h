#ifndef IMAGES_FORMAT_H
#define IMAGES_FORMAT_H

#include <string>
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#include <map>
#include "GCTrace.h"
#include "ImageDetailInfo.h"

using namespace std;

class ImagesFormat
{
public:
	ImagesFormat();
	~ImagesFormat();

	void GetAllImages(void);

	map<string, ImageDetailInfo*>* GetImageForamtMap(void);


private:
	TIniFile* mImagesIni;
	TStringList* mImageNameList;
	
	std::map<string, ImageDetailInfo*> mImageForamtMap;

};

#endif