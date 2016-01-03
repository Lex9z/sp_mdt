#include "ImagesFormat.h"
#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

#define STR_UNKNOWN "unknown"

ImagesFormat::ImagesFormat():mImagesIni(NULL), mImageNameList(NULL)
{

}

ImagesFormat::~ImagesFormat()
{
	if (NULL != mImageNameList)
	{
		delete mImageNameList;
		mImageNameList = NULL; 
	}

	if (NULL != mImagesIni)
	{
		delete mImagesIni;
		mImagesIni = NULL;
	}

	std::map<string, ImageDetailInfo*>::iterator mapIterator = mImageForamtMap.begin();
	for(; mapIterator != mImageForamtMap.end(); mapIterator++)
	{
		delete mapIterator->second;
		mapIterator->second = NULL;
	}
	
}

void ImagesFormat::GetAllImages()
{
	AnsiString iniFileNameWithPath = NULL;
	AnsiString asPath = NULL;

	if (getPathFromStr(Application->ExeName,asPath))
	{
		GC_TRACE("<ImagesFormat> GetAllImages(): the tool path is %s", asPath);
		iniFileNameWithPath = asPath + "ImageInfo.ini";
		if (!Check_FileExist(iniFileNameWithPath))
		{
			GC_TRACE("<ImagesFormat> GetAllImages(): Can not find Images ini file. ");
			return ;
		}
	}

	try{
		mImagesIni = new TIniFile(iniFileNameWithPath.c_str());
		//ImageDetailInfo *imageInfo = NULL;
		if(NULL == mImageNameList)
		{
			mImageNameList = new TStringList();
			//Get all sections' name.
			mImagesIni->ReadSections(mImageNameList);
			int count = mImageNameList->Count;

			for(int i=0; i<count; i++)
			{
				TStringList* mImageSectionList = new TStringList();
				
				mImagesIni->ReadSection(mImageNameList->Strings[i], mImageSectionList);

				ImageDetailInfo *imageInfo = new ImageDetailInfo();

				if (mImageSectionList->Count != 4)
				{
					GC_TRACE("<ImagesFormat> GetAllImages(): The section key number is wrong! ");
					return;
				}
				AnsiString imageName = mImagesIni->ReadString(mImageNameList->Strings[i], mImageSectionList->Strings[0], STR_UNKNOWN);

				imageInfo->SetImagePrefixName(imageName);
				GC_TRACE("<ImagesFormat> GetAllImages(): The image name is %s .", imageName.c_str());

				std::string read_str = mImagesIni->ReadString(mImageNameList->Strings[i], mImageSectionList->Strings[1], STR_UNKNOWN).c_str();
				GC_TRACE("<ImagesFormat> GetAllImages(): The image brom format is %s .", read_str);
				imageInfo->SetBromFormat(atoi(read_str.c_str()));

				read_str = mImagesIni->ReadString(mImageNameList->Strings[i], mImageSectionList->Strings[2], STR_UNKNOWN).c_str();
				GC_TRACE("<ImagesFormat> GetAllImages(): The image eboot format is %s .", read_str);
				imageInfo->SetEbootFormat(atoi(read_str.c_str()));

				read_str = mImagesIni->ReadString(mImageNameList->Strings[i], mImageSectionList->Strings[3], STR_UNKNOWN).c_str();
				GC_TRACE("<ImagesFormat> GetAllImages(): The image type is %s .", read_str);
				imageInfo->SetImageType(atoi(read_str.c_str()));

				mImageForamtMap.insert(map<string, ImageDetailInfo*> :: value_type(mImageNameList->Strings[i].c_str(), imageInfo));

				if (NULL != mImageSectionList)
				{
					delete mImageSectionList;
					mImageSectionList = NULL; 
				}

			}

		} 
	} catch (const std::exception& ex) {
		GC_TRACE("<ImagesFormat> GetAllImages: Read image_info failed, reason(%s).", ex.what());
		return ;
	}


}

map<string, ImageDetailInfo*>* ImagesFormat::GetImageForamtMap(void )
{
	if(0 == mImageForamtMap.size()) 
		GetAllImages();		
	return &mImageForamtMap;
}