#include "NewAddedImages.h"

#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

NewAddedImages::NewAddedImages():mImagesIni(NULL), mImageScatterNameList(NULL), mImagePrefixNameList(NULL)
{

}

NewAddedImages::~NewAddedImages()
{
	if (NULL != mImagePrefixNameList)
	{
		delete mImagePrefixNameList;
		mImagePrefixNameList = NULL; 
	}

	if (NULL != mImageScatterNameList)
	{
		delete mImageScatterNameList;
		mImageScatterNameList = NULL;
	}

	if (NULL != mImagesIni)
	{
		delete mImagesIni;
		mImagesIni = NULL;
	}

	std::map<string, ImageDetailInfo*>::iterator mapIterator = mImageMap.begin();
	for(; mapIterator != mImageMap.end(); mapIterator++)
	{
		delete mapIterator->second;
		mapIterator->second = NULL;
	}
	
}

void NewAddedImages::GetNewImages()
{
	AnsiString iniFileNameWithPath = NULL;
	AnsiString asPath = NULL;

	if (getPathFromStr(Application->ExeName,asPath))
	{
		GC_TRACE("<NewAddedImages> GetNewImages(): the tool path is %s", asPath);
		iniFileNameWithPath = asPath + "Images.ini";
		if (!Check_FileExist(iniFileNameWithPath))
		{
			GC_TRACE("<NewAddedImages> GetNewImages(): Can not find Images ini file. ");
			return ;
		}
	}

        mImagesIni = new TIniFile(iniFileNameWithPath.c_str());
	if(NULL == mImageScatterNameList)
	{
		mImageScatterNameList = new TStringList();
		mImagesIni->ReadSection(IMAGE_SECTION_NAME, mImageScatterNameList);
	}

	if(NULL == mImagePrefixNameList)
	{
		int count = mImageScatterNameList->Count;
		if (count > 0 )
		{
			mImagePrefixNameList = new TStringList();

			for(int i = 0; i < count; i++)
			{
				AnsiString imagePrefixName = mImagesIni->ReadString(IMAGE_SECTION_NAME, mImageScatterNameList->Strings[i], NAME_UNKNOWN);
				mImagePrefixNameList->Add(imagePrefixName);
				//mImagePrefixNameList->Strings[i] = mImagesIni->ReadString(IMAGE_SECTION_NAME, mImageScatterNameList->Strings[i], NAME_UNKNOWN);
				GC_TRACE("<NewAddedImages> GetNewImages(): mImageScatterNameList->Strings[d%] is s% .", i, mImageScatterNameList->Strings[i]);
				GC_TRACE("<NewAddedImages> GetNewImages(): mImagePrefixNameList->Strings[d%] is s% .", i, imagePrefixName);
			}
		}
		
	}

	int count = mImageScatterNameList->Count;

	for(int j = 0; j < count; j++)
	{
		ImageDetailInfo *imageInfo = new ImageDetailInfo();

		imageInfo->SetImagePrefixName(mImagePrefixNameList->Strings[j]);
		string imageScatterName = mImageScatterNameList->Strings[j].c_str();

		mImageMap.insert(map<string, ImageDetailInfo*> :: value_type(mImageScatterNameList->Strings[j].c_str(), imageInfo));
	}
		
	return ;
}

map<string, ImageDetailInfo*>* NewAddedImages::GetImageMap(void )
{
	if(0 == mImageMap.size()) 
		GetNewImages();		
	return &mImageMap;
}
