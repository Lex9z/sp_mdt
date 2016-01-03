#include "BackupConfig.h"
#include "FlashDownLoad.h"
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#include "GCTrace.h"
#include "form_MultiLoad.h"
#include <algorithm>    // std::transform

#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

BackupConfig::BackupConfig(const std::string &backupFolder)
 :backup_folder_(backupFolder){

}
BackupConfig::~BackupConfig()
{}
std::string BackupConfig::Get_BackupINI_path(const std::string& scatter_file)
{
        std::string Tmp_load_path;
	int dpos = scatter_file.find_last_of('\\');
	if (dpos != std::string::npos) {
	    Tmp_load_path = scatter_file.substr(0, dpos);
	}
	return Tmp_load_path;
}
int BackupConfig::InitBackupInfoTable()
{
    //AnsiString filePath = ExtractFilePath( Application->ExeName ) + "\\protected_region.ini";
	AnsiString filePath=AnsiString(Get_BackupINI_path(Application->ExeName.c_str()).c_str())+"\\protected_region.ini";
    TIniFile *file = new TIniFile(filePath);

    TStrings *sections = new TStringList;
    file->ReadSections(sections);

    string section;
    string partition;
    int ret = S_DONE;

    PART_INFO partiInfo;
    memset(&partiInfo, 0, sizeof(PART_INFO));

   /* NandLayoutParameter nandPara(g_pCurNode->m_dl_handle, g_ft_handle);

   if(!nandPara.is_target_layout_map_ready())
   {
          LOGE("Get PMT fail.");
	  ret = FT_UPGRADE_GET_PMT_FAIL;
   }else{
  */

    BackupItem backupItem;

    for(int i = 0; i < sections->Count; i++)
    {
        GC_TRACE("section%d, name(%s)", i, sections->Strings[i].c_str());

        section = string(sections->Strings[i].c_str());

        TStrings *backupInfo = new TStringList;
        file->ReadSection(sections->Strings[i], backupInfo);

        for(int j = 0; j < backupInfo->Count; j++)
        {
            if(backupInfo->Strings[j] == "backup_file_name")
            {
                backupItem.set_file_path(backup_folder_ + "\\" +file->ReadString(sections->Strings[i], backupInfo->Strings[j], "").c_str());
            }
            else if(backupInfo->Strings[j] == "chksum_file_name")
            {
               backupItem.set_chksum_file(backup_folder_ + "\\" +file->ReadString(sections->Strings[i], backupInfo->Strings[j], "").c_str());
            }
        }

        backupItem.set_length(partiInfo.image_length);
        backupItem.set_start_addr(partiInfo.begin_addr);
        backupItem.set_partition_name(section);

        backup_map_.insert(pair<std::string, BackupItem>(section, backupItem));
        backup_partitions_.push_back(section);
    }

    return ret;
}

std::list<std::string> BackupConfig:: backup_partitions()
{
  std::list<std::string> backup_partitions_tmp;
	//Attention: allocate space for dest vector before std::copy
	backup_partitions_tmp.clear();
	backup_partitions_tmp.resize(backup_partitions_.size());
	std::copy(backup_partitions_.begin(), backup_partitions_.end(), backup_partitions_tmp.begin());
	return backup_partitions_tmp;
}


std::map<std::string, BackupItem> BackupConfig:: backup_map()
{
       return backup_map_;
}

bool BackupConfig::QueryBackupItemByPartitionName(std::string partition_name, BackupItem *backup_item)
{
    bool is_found = false;
	vector<string> vctDstTemp;
    InitVectorForFindPartName(partition_name, vctDstTemp);
    std::map<std::string, BackupItem>::iterator it = backup_map_.begin();
    for(; it!= backup_map_.end(); it++)
    {   
        if(FindPartName(it->first, vctDstTemp) == _TRUE)
        {
            backup_item->set_chksum_file(it->second.chksum_file());
            backup_item->set_file_path(it->second.file_path());
            backup_item->set_length(it->second.length());
            backup_item->set_partition_name(it->second.partition_name());
            backup_item->set_start_addr(it->second.start_addr());
            is_found = true;
            break;
        }
        /*if(it->first == partition_name){
            backup_item->set_chksum_file(it->second.chksum_file());
            backup_item->set_file_path(it->second.file_path());
            backup_item->set_length(it->second.length());
            backup_item->set_partition_name(it->second.partition_name());
            backup_item->set_start_addr(it->second.start_addr());
            is_found = true;
            break;
        }*/
    }

    return is_found;
}

void BackupConfig::InitVectorForFindPartName(string strName, vector<string> &vctDstTemp )
{
    if( strName.compare(SCATTER_FAT_NAME) == 0 ||strName.compare("FAT") == 0)  //for FAT
	{
	    vctDstTemp.push_back("FAT");
		vctDstTemp.push_back(SCATTER_FAT_NAME);
	}
	else if(strName.compare(SCATTER_PRODUCT_INFO_NAME) == 0 || strName.compare("PRO_INFO") == 0)// For
	{
	    vctDstTemp.push_back(SCATTER_PRODUCT_INFO_NAME);
		vctDstTemp.push_back("PRO_INFO");
	}
	else if(strName.compare(SCATTER_PROTECTF_NAME) == 0 || strName.compare("PROTECT_F") == 0)
	{
	    vctDstTemp.push_back("PROTECT_F");
		vctDstTemp.push_back(SCATTER_PROTECTF_NAME);
	}
	else if(strName.compare(SCATTER_PROTECTS_NAME) == 0 || strName.compare("PROTECT_S") == 0)
	{
	    vctDstTemp.push_back("PROTECT_S");
		vctDstTemp.push_back(SCATTER_PROTECTS_NAME);
	}
	else if(strName.compare(SCATTER_NVRAM_NAME1) == 0 || strName.compare(SCATTER_NVRAM_NAME2) == 0)
	{
	    vctDstTemp.push_back(SCATTER_NVRAM_NAME1);
		vctDstTemp.push_back(SCATTER_NVRAM_NAME2);
	}
	else if(strName.compare(SCATTER_BMTPOOL_NAME) == 0 || strName.compare("BMTPOOL") == 0)
	{
	    vctDstTemp.push_back(SCATTER_BMTPOOL_NAME);
		vctDstTemp.push_back("BMTPOOL");
	}
	else
	{
	    vctDstTemp.push_back(strName); 
	}
	
}

bool BackupConfig::FindPartName(string strSrc, vector<string> vctDstStr )
{
    string strSrcTemp = strSrc;
    string strDstTemp;
    std::vector<string>::iterator it;

    //transfer the string to lower case first
    transform(strSrcTemp.begin(),strSrcTemp.end(),strSrcTemp.begin(),tolower);
    for(it = vctDstStr.begin(); it != vctDstStr.end(); it++)
    {
        strDstTemp = *it;
        transform(strDstTemp.begin(),strDstTemp.end(),strDstTemp.begin(),tolower);
		if(strSrcTemp.compare(strDstTemp) == 0)
		{
	       // GC_TRACE("[COM%d] FindPartName(): strSrc (%s), strDstTemp(%s), success!.", m_sComPortSetting.com.number, strSrcTemp.c_str(),strDstTemp.c_str());
			break;
		}
	}
   
    if(it != vctDstStr.end())
    {
       return _TRUE;
    }else{
       return _FALSE;
    }
}




