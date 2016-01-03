/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   form_MultiLoad.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Multi-load form source file
 *
 * Author:
 * -------
 *  Andy Ueng (mtk00490)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime$
 * $Log$
 *
 * May 21 2008 mtk00490
 * [STP100001729] [Multiport download tool] Multiport download tool ver 3.0820.0
 *
 *
 * Feb 4 2008 mtk00490
 * [STP100001633] [Multiport download tool] Multiport download tool ver 3.0804.0
 *
 *
 * Dec 3 2007 mtk00490
 * [STP100001572] [Multiport download tool] Multiport download tool ver 3.0748.0
 *
 *
 * Nov 29 2006 mtk00490
 * [STP100001239] [Multiport download tool] Multiport download tool ver 3.1.02
 *
 *
 * Jun 21 2006 MTK00490
 * [STP100000939] Multiport download Tool_v2.9.1001
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

//---------------------------------------------------------------------------

#include <vcl.h>
#include <inifiles.hpp>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "file_utils.h"

#include "NewIniHandle.h"
#include "ImagesFormat.h"
#include "MD5.h"
#pragma hdrstop

// common
#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif

// form
#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#include "MultiPortFlashDownloadTool.h"
#endif

#include "form_MultiLoad.h"

#ifndef	_FLASHTOOL_HANDLE_H_
#include "flashtool_handle.h"
#endif


#include "formChecksumProgress.h"
#include "INIFileHandle.h"
typedef enum {
	 LV_BOOTLOADER = 0
	,LV_NFB_DL_ROM
	,LV_DL_ROM
	,LV_OTP
	,LV_FTCFG
	,LV_SEC_RO
	,LV_CUST_PARA
    ,LV_GPS_ROM
} E_LISTVIEW_ITEM_TYPE;

E_LISTVIEW_ITEM_TYPE g_LV_ItemType[] =
{
    LV_BOOTLOADER
    ,LV_NFB_DL_ROM
    ,LV_DL_ROM
    ,LV_OTP
    ,LV_FTCFG
    ,LV_SEC_RO
    ,LV_CUST_PARA
    ,LV_GPS_ROM
};
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMultiLoad *frmMultiLoad;
extern TMainForm *MainForm;
//extern struct DL_HANDLE	*g_DL_HANDLE;
extern struct DA_HANDLE	*g_DA_HANDLE;
extern DL_HANDLE_T *g_DL_HANDLE_SecroReworking;

const int Listview_Subitem_Num = 4;

#define DLCALL(a) {if((a)) {return -1;}}


//---------------------------------------------------------------------------
__fastcall TfrmMultiLoad::TfrmMultiLoad(TComponent* Owner)
        : TForm(Owner)
{
    m_lv[0] = lv1;
    m_lvPar[0] = lvPar1;

    m_lblScatFileName[0] = lblScatFileName1;
    m_dlgOpenScatter[0] = dlgOpenScatter1;
    m_dlgOpenROM[0] = dlgOpenROM1;

    m_xldr_ini_path[0] = "";
    m_eboot_ini_path[0] = "";

    m_uboot_ini_path[0] = "";
    m_Logo_ini_path[0]  = "";
	m_TEE1_ini_path[0]  = "";
	m_TEE2_ini_path[0]  = "";
    m_Boot_ini_path[0] = "";
    m_Secro_ini_path[0] = "";
    m_AndroidSystem_ini_path[0] = "";
    m_UserData_ini_path[0] = "";

    m_Custpack1_ini_path[0] = "";
    m_Custpack2_ini_path[0] = "";
    m_Custpack3_ini_path[0] = "";
    m_Custpack4_ini_path[0] = "";

	m_cache_ini_path[0]  = "";
	m_FAT_ini_path[0]  = "";


    b_xlder_checked[0] = _TRUE;
    b_eboot_checked[0] = _TRUE;

    _bootloader_type = BT_UNKONW;

    MakeImageAttrVector();
    //m_dl_vector.clear();
}

//--------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::FormCreate(TObject *Sender)
{
//    Display_ScatterAndLoad();
    m_chip_type = UNKNOWN_BBCHIP_TYPE;
    m_bPlatformChange = true;
    m_Storage_Type = HW_STORAGE_NAND;
    m_Load_End_Address = 0;
    iniFilePath = NULL;

  	imageMap = addedImages.GetImageMap();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//    Save_ScatterAndLoad(false);
}

bool TfrmMultiLoad::Update_Handle_List(unsigned int idx, DL_HANDLE_T ** pp_temp_dl_handle,
                                       DL_HANDLE_T ** pp_dl_handle, int &ret)
{
    return true;
}

bool TfrmMultiLoad::Display_FAT_Info(unsigned int idx, DL_HANDLE_T * p_temp_dl_handle)
{
#ifndef SERVICE_CENTER
    int iLD = m_asScatFileName[idx].LastDelimiter("\\:");

    AnsiString as_LV_FAT_index;
    m_asFATFileName[idx] = m_INIFileH.read_FAT( INI_FILE_NAME, Application->ExeName, idx+1 );
    as_LV_FAT_index = m_INIFileH.read_LV_FAT_index(INI_FILE_NAME, Application->ExeName, idx+1 );
    m_iLV_FAT_index[idx] = as_LV_FAT_index.ToInt();

    if( m_iLV_FAT_index[idx]>=1 && MainForm->Get_Type() == OP_FORMAT_AND_DOWNLOAD )
    {
        if (m_asFATFileName[idx].AnsiCompareIC("") != 0)
        {
            if(!Load_Rom(p_temp_dl_handle, m_iLV_FAT_index[idx], m_asFATFileName[idx]))
            {
                return false;
            }

            iLD = m_asFATFileName[idx].LastDelimiter("\\:");
            if ( iLD > 0)
            {
                m_asFATPath[idx] = m_asFATFileName[idx].SubString(1, iLD-1 );
            }
        }
    }
#endif

    return true;
}

bool TfrmMultiLoad::Auto_Load_Scatter_File(unsigned int idx, int &ret, DL_HANDLE_T * p_temp_dl_handle)
{

    if (MainForm->Get_AutoLoadResByScatFlag())
    {
        ret = DL_AutoLoadByScatRegionName(*p_temp_dl_handle, m_asScatFileName[idx].c_str(), NORMAL_ROM/*|RESOURCE_BIN|JUMPTABLE_BIN*/, _TRUE);
        if (ret != 0)
        {
            Application->MessageBox("Automatic load by scatter region name fail, please load images manually!", "Reminder", MB_OK);
            //Destroy_DLHandle(p_temp_dl_handle);
            //return false;
        }
    }

    return true;
}
void TfrmMultiLoad::LoadSecroScatter(AnsiString scatterfilePath)
{

	int iLD,ret;
	AnsiString Folder_path;
	AnsiString path;
	
try{
	m_SecroScatter_path = m_INIFileH.read_SecroScatter(INI_FILE_NAME, Application->ExeName );
    iLD = scatterfilePath.LastDelimiter("\\:");
	MainForm->SecroReworkRunningFlag = _FALSE;	 //init running flag
    if (iLD > 0)
    {
        ::WIN32_FIND_DATA FindFileData;     
        HANDLE hFind;
        Folder_path= scatterfilePath.SubString(1, iLD-1 );
        path = Folder_path + "\\Rework-Secro" + scatterfilePath.SubString(iLD, scatterfilePath.Length()-1);
        hFind = ::FindFirstFile(path.c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            MainForm->SecroReworkRunningFlag = _FALSE;			
            GC_TRACE("<Debug> LoadSecroScatter: Secro reworking scatter file invalid!!");
            Application->MessageBox("Secro reworking scatter file NOT FOUND!", "Warning", MB_OK);
            return ;
        }
	    else{
		    ::FindClose(hFind);
		    m_SecroScatter_path = path;
		}						  
	}
	else{
		return; //escape error message
	}
	        	    
    GC_TRACE("<Debug> LoadSecroScatter: Scatter file path=(%s)",m_SecroScatter_path.c_str());


    Destroy_DLHandle(g_DL_HANDLE_SecroReworking);
	g_DL_HANDLE_SecroReworking = new DL_HANDLE_T;
	DL_Create(g_DL_HANDLE_SecroReworking);
		
    ret=DL_LoadScatter(*g_DL_HANDLE_SecroReworking, m_SecroScatter_path.c_str(),NULL);
    if (ret)
    {
        if( S_FTHND_HANDLE_BUSY_NOW == ret )
        {
            ShowMessage("Program is busy! Can not reload scat file right now.");
        }
        else if( S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + m_SecroScatter_path + "\"!\n\nThe offset value of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + m_SecroScatter_path + "\"!\n\nThe region begin address of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER == ret )
        {
            ShowMessage("Failed to load scatter file \"" + m_SecroScatter_path + "\"!\n\nThe region begin address of one load region is not ascending sorting sequence!");
        }
        else
        {
            ShowMessage("Incorrect scatter file format!");
        }
        return;
    }

	ret = DL_AutoLoadRomImages(*g_DL_HANDLE_SecroReworking, m_SecroScatter_path.c_str());
    if(ret != S_DONE && ret != 1)
	{
	    MainForm->SecroReworkRunningFlag = _FALSE;
	    GC_TRACE("<MDLF> LoadSecroScatter(): Rom files auto load failed, ret(%d)", ret);
	    Application->MessageBox("Load Scatter File Fail", "FAIL", MB_OK);
	    return;
	}
	MainForm->SecroReworkRunningFlag = _TRUE;
    GetPlatformInfoAndBmtBlock(m_SecroScatter_path, g_DL_HANDLE_SecroReworking,FALSE);		
    m_INIFileH.write_SecroScatter(INI_FILE_NAME, Application->ExeName, m_SecroScatter_path);

  }
  catch(...)
  {
		//int a = 0;
    GC_TRACE("<MDLF> LoadSecroScatter() exception!");
  }

}
//==============================================================================
void TfrmMultiLoad::Display_LoadScatterAndBinFile(void)
{
    int ret; 
	int idx=0;
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
	DL_HANDLE_LIST_T *p_dl_handle_list = MainForm->Get_DlHandleList();
	m_asScatFileName[idx] = m_INIFileH.read_RomInfo(INI_FILE_NAME, Application->ExeName,"ScatterFile", "Scatter_File_Path");	
    b_SCATTER_checkEnable=m_INIFileH.read_SCATTER(INI_FILE_NAME, Application->ExeName);
	b_checksum_ver=m_INIFileH.read_Checksum_Ver(INI_FILE_NAME, Application->ExeName);
	if(!m_asScatFileName[idx].IsEmpty())
	{     	
        ret=DL_LoadScatter(*p_dl_handle, m_asScatFileName[idx].c_str(),NULL);
        if (ret)
        {
           if( S_FTHND_HANDLE_BUSY_NOW == ret )
           {
               ShowMessage("Program is busy! Can not reload scat file right now.");
           }
           else if( S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN == ret )
           {
               ShowMessage("Failed to load scatter file \"" + m_asScatFileName[idx] + "\"!\n\nThe offset value of one load region is not WORD alignment!");
           }
           else if( S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN == ret )
           {
               ShowMessage("Failed to load scatter file \"" + m_asScatFileName[idx] + "\"!\n\nThe region begin address of one load region is not WORD alignment!");
           }
           else if( S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER == ret )
           {
               ShowMessage("Failed to load scatter file \"" + m_asScatFileName[idx] + "\"!\n\nThe region begin address of one load region is not ascending sorting sequence!");
           }
           else
           {
               ShowMessage("Incorrect scatter file format!");
           }
                   return ;
        }
		
        ret = DL_AutoLoadRomImages(*p_dl_handle, m_asScatFileName[idx].c_str());
        if(ret != S_DONE && ret != 1)
        {
            GC_TRACE("<MDLF> Display_LoadScatterAndBin(): Rom files auto load failed, ret(%d)", ret);
            return;
        }
	       
        //Destroy_DLHandle(p_dl_handle);
        //p_dl_handle = p_temp_dl_handle;
        MainForm->Set_DlHandle(idx, p_dl_handle);
        ret = DL_AddHandleToList(*p_dl_handle_list, *p_dl_handle);
        if (ret != 0)
        {
            Application->MessageBox("Add handle to list fail", "FAIL", MB_OK);
            return;
        }

        AutoLoadSecroSignImg(p_dl_handle, m_asScatFileName[idx].c_str());
		
        SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);
        MainForm->SetDAChecksum(MainForm->GetChecksumEnable());
      
		GetPlatformInfoAndBmtBlock(m_asScatFileName[idx], p_dl_handle,TRUE);
		UpdateRawLVListAndSaveRomInfo(p_dl_handle,m_asScatFileName[idx]);
        SetScatterFileCaption(idx, m_asScatFileName[idx]);
        MainForm->SetDAChecksum(MainForm->GetChecksumEnable());
        RedrawTypeComBox();
        //RedrawLVParamList(idx, p_dl_handle);
        MainForm->SecroReworkingEnable = m_INIFileH.read_SecroReworkingSupport(INI_FILE_NAME, Application->ExeName);
        if(MainForm->SecroReworkingEnable)
        {
            GC_TRACE("<Debug> Display_LoadScatterAndBinFile: Do LoadSecroScatter-(SecroReworkingEnable)");
            LoadSecroScatter(m_asScatFileName[idx]);
        }
        else
        {
            GC_TRACE("<Debug> Display_LoadScatterAndBinFile: SecroReworkingEnable NOT true"); 
        }
	 }
	 else
	 {
              //Destroy_DLHandle(p_temp_dl_handle);
		return;
	 }	
	
}

//==============================================================================
/*this function is replaced by Display_LoadScatterAndBinFile now */
void TfrmMultiLoad::Display_ScatterAndLoad(void)
{
    int ret;
    DL_HANDLE_LIST_T *p_dl_handle_list = MainForm->Get_DlHandleList();

    for (int idx=0; idx < MAX_SUPPORT_LOAD_NUM; idx++)
    {
        DL_HANDLE_T *p_temp_dl_handle = new DL_HANDLE_T;
        DL_Create(p_temp_dl_handle);

        DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
        m_asScatFileName[idx] = m_INIFileH.read_Scatter(INI_FILE_NAME, Application->ExeName, idx+1 );

        if (m_asScatFileName[idx].AnsiCompareIC("") == 0)
        {
		      	Destroy_DLHandle(p_temp_dl_handle);
            continue;
        }

        if (OpenReadScatFile(m_asScatFileName[idx], idx, p_temp_dl_handle))
        {
            int iLD = m_asScatFileName[idx].LastDelimiter("\\:");
            if (iLD > 0)
            {
                m_asScatPath[idx] = m_asScatFileName[idx].SubString(1, iLD-1 );
            }

            if(!Auto_Load_Scatter_File(idx, ret, p_temp_dl_handle))
            {
                return;
            }

            #ifndef SERVICE_CENTER
            if(!Display_FAT_Info(idx, p_temp_dl_handle))
            {
                return;
            }
            #endif

            //Check BT type Uboot/Eboot
            unsigned short rom_count;
            ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
            if(!DL_GetCount(*p_temp_dl_handle, &rom_count))
            {
                ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
                if(!DL_Rom_GetInfoAll(*p_temp_dl_handle, rom_info, MAX_LOAD_SECTIONS))
                {
                    for (int i=0; i<rom_count; i++)
                    {
                        if (strcmpi(rom_info[i].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom_info[i].name, SCATTER_LK_NAME) == 0)
                        {
                            _bootloader_type = BT_UBOOT;
                        }
                        else if (strcmpi(rom_info[i].name, "eboot") == 0)
                        {
                            _bootloader_type = BT_EBOOT;
                        }
                    }
                }
            }

            if(BT_UBOOT == get_bt_type())
            {
                //Uboot
                AnsiString path;
                path = m_INIFileH.read_Preloader(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_preloader_ini_path[idx] = path;
                }

                path = m_INIFileH.read_DSPBL(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_DSPBL_ini_path[idx] = path;
                }

                path = m_INIFileH.read_UBoot(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_uboot_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Boot(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                   m_Boot_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Logo(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                   m_Logo_ini_path[idx] = path;
                }

				path = m_INIFileH.read_TEE1(INI_FILE_NAME, Application->ExeName, idx+1 );
				if( path.AnsiCompareIC("") != 0 )
				{
					m_TEE1_ini_path[idx] = path;
				}

				
				path = m_INIFileH.read_TEE2(INI_FILE_NAME, Application->ExeName, idx+1 );
				if( path.AnsiCompareIC("") != 0 )
				{
				    m_TEE2_ini_path[idx] = path;
				}


                path = m_INIFileH.read_Secro(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Secro_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Recovery(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Recovery_ini_path[idx] = path;
                }

                path = m_INIFileH.read_AndroidSystem(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_AndroidSystem_ini_path[idx] = path;
                }

                path = m_INIFileH.read_UserData(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_UserData_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Custpack(INI_FILE_NAME, Application->ExeName, 1, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Custpack1_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Custpack(INI_FILE_NAME, Application->ExeName, 2, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Custpack2_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Custpack(INI_FILE_NAME, Application->ExeName, 3, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Custpack3_ini_path[idx] = path;
                }

                path = m_INIFileH.read_Custpack(INI_FILE_NAME, Application->ExeName, 4, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_Custpack4_ini_path[idx] = path;
                }

			       	//eMMC added
			       	path = m_INIFileH.read_emmc_FAT(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_FAT_ini_path[idx] = path;
                }

				path = m_INIFileH.read_cache(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_cache_ini_path[idx] = path;
                }

				path = m_INIFileH.read_EBR1(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_EBR1_ini_path[idx] = path;
                }

				path = m_INIFileH.read_EBR2(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_EBR2_ini_path[idx] = path;
                }

				path = m_INIFileH.read_MBR(INI_FILE_NAME, Application->ExeName, idx+1 );
                if( path.AnsiCompareIC("") != 0 )
                {
                    m_MBR_ini_path[idx] = path;
                }

				map<string,ImageDetailInfo*>::iterator it = imageMap->begin();

				for(; it!= imageMap->end(); it++)
				{
					string imageScatterName = it->first;
					path = NewIniHandle::ReadStringFromIni(INI_FILE_NAME, Application->ExeName, idx+1, "form", (AnsiString)imageScatterName.c_str(), "");

					if( path.AnsiCompareIC("") != 0 )
					{
						it->second->SetImageIniPath(path);

					}

					_BOOL imageChecked = NewIniHandle::ReadBoolFromIni(INI_FILE_NAME, Application->ExeName, idx+1, "form", (AnsiString)imageScatterName.c_str(), "yes");
					it->second->SetImageChecked(imageChecked);
				}

                b_SCATTER_checkEnable=m_INIFileH.read_SCATTER(INI_FILE_NAME, Application->ExeName);
                b_checksum_ver=m_INIFileH.read_Checksum_Ver(INI_FILE_NAME, Application->ExeName);
                b_preloader_checked[idx] = m_INIFileH.read_Preloader_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_DSPBL_checked[idx] = m_INIFileH.read_DSPBL_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_uboot_checked[idx] = m_INIFileH.read_UBoot_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_Logo_checked[idx] = m_INIFileH.read_Logo_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_TEE1_checked[idx] = m_INIFileH.read_TEE1_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_TEE2_checked[idx] = m_INIFileH.read_TEE2_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_Boot_checked[idx] = m_INIFileH.read_Boot_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_Secro_checked[idx] = m_INIFileH.read_Secro_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_Recovery_checked[idx] = m_INIFileH.read_Recovery_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_AndroidSystem_checked[idx] = m_INIFileH.read_AndroidSystem_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_UserData_checked[idx] = m_INIFileH.read_UserData_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
                b_Custpack1_checked[idx] = m_INIFileH.read_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 1, idx+1);
                b_Custpack2_checked[idx] = m_INIFileH.read_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 2, idx+1);
                b_Custpack3_checked[idx] = m_INIFileH.read_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 3, idx+1);
                b_Custpack4_checked[idx] = m_INIFileH.read_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 4, idx+1);

				//eMMC added
				b_FAT_checked[idx] = m_INIFileH.read_emmc_FAT_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_cache_checked[idx] = m_INIFileH.read_cache_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_EBR1_checked[idx] = m_INIFileH.read_EBR1_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_EBR2_checked[idx] = m_INIFileH.read_EBR2_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
				b_MBR_checked[idx] = m_INIFileH.read_MBR_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
            }
            else
            {
            //Eboot & mloder
            AnsiString as_mloder, as_eboot;
            as_mloder = m_INIFileH.read_MLoader(INI_FILE_NAME, Application->ExeName, idx+1 );
            if( as_mloder.AnsiCompareIC("") != 0 )
            {
                m_xldr_ini_path[idx] = as_mloder;
            }

            as_eboot = m_INIFileH.read_EBoot(INI_FILE_NAME, Application->ExeName, idx+1 );
            if( as_eboot.AnsiCompareIC("") != 0 )
            {
                m_eboot_ini_path[idx] = as_eboot;
            }

           b_xlder_checked[idx] = m_INIFileH.read_MLoader_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
           b_eboot_checked[idx] = m_INIFileH.read_EBoot_Checked(INI_FILE_NAME, Application->ExeName, idx+1);
            }

            // secure RO
            if(!Display_Ro_File(idx))
            {
                return;
            }

            // GPS, get GPS ROM path and gps.ini path
            if(!Display_GPS_File(idx))
            {
                return;
            }

          // update handle list
          if( NULL != p_dl_handle ) // not new handle
          {
              ret = DL_CheckHandleSameSWVersionInList( *p_dl_handle_list, *p_temp_dl_handle  );

              if( 0 == ret ) // no same S/W version in list
              {
                  if( 0 != DL_CheckHandleInList( *p_dl_handle_list, *p_dl_handle ) ) // handle in list
                  {
                      ret = DL_DeleteHandleFromList( *p_dl_handle_list, *p_dl_handle );
                      if( ret != 0 )
                      {
                          Application->MessageBox( "Delete handle from list fail", "FAIL", MB_OK );
                          return;
                      }
                  }
                  Destroy_DLHandle( p_dl_handle );
                  p_dl_handle = p_temp_dl_handle;
                  MainForm->Set_DlHandle( idx, p_dl_handle );
                  ret = DL_AddHandleToList( *p_dl_handle_list, *p_dl_handle  );
                  {
                      if( ret != 0 )
                      {
                          Application->MessageBox( "Add handle to list fail", "FAIL", MB_OK );
                          return ;
                      }
                  }

                  SetScatterFileCaption(idx, m_asScatFileName[idx]);
                  MainForm->SetDAChecksum(MainForm->GetChecksumEnable());
              }
              else // same S/W in handle list
              {
                  Destroy_DLHandle(p_temp_dl_handle);
                  Application->MessageBox( "Has same S/W in handle list", "NOTICE", MB_OK );
                  return ;
              }
          }
          else // new handle
          {
              p_dl_handle = p_temp_dl_handle;
              ret = DL_AddHandleToList( *p_dl_handle_list, *p_dl_handle );
              {
                  if( ret != 0 )
                  {
                      Application->MessageBox( "Add handle to list fail", "FAIL", MB_OK );
                      return ;
                  }
              }
              SetScatterFileCaption(idx, m_asScatFileName[idx]);
              Destroy_DLHandle(p_temp_dl_handle);
          }


            RedrawLVList(idx, p_dl_handle);
            RedrawTypeComBox();
            RedrawLVParamList(idx, p_dl_handle);
        }
        else
        {
            Destroy_DLHandle(p_temp_dl_handle);
        }
    }

	
	MainForm->SecroReworkingEnable = m_INIFileH.read_SecroReworkingSupport(INI_FILE_NAME, Application->ExeName);
	if(MainForm->SecroReworkingEnable)
        {
        	GC_TRACE("<Debug> Display_ScatterAndLoad: Do LoadSecroScatter-(SecroReworkingEnable)");
            //LoadSecroScatter();
        }
	else
		GC_TRACE("<Debug> Display_ScatterAndLoad: SecroReworkingEnable NOT true");
	
}

bool TfrmMultiLoad::Display_Ro_File(unsigned int index)
{

  AnsiString as_sec_ro = m_INIFileH.read_SEC_RO(INI_FILE_NAME, Application->ExeName, index+1 );

  if( as_sec_ro.AnsiCompareIC("") != 0 )
  {
      if( ! LoadFile(as_sec_ro.c_str(), &m_sParameter[index].m_sec_ro.m_data, &m_sParameter[index].m_sec_ro.m_len, NULL))
      {
          AnsiString as;
          as.printf("Fail to load SEC_RO file \"%s\" !", dlgOpenSecRo->FileName.c_str());
          ShowMessage(as);
          return false;
      }

      AnsiString as_sec_ro_checked = m_INIFileH.read_SEC_RO_Checked(INI_FILE_NAME, Application->ExeName, index+1 );
      if( as_sec_ro_checked.AnsiCompareIC("yes") == 0 )
      {
          m_sParameter[index].m_sec_ro_enable = _TRUE;
      }
      else
      {
          m_sParameter[index].m_sec_ro_enable = _FALSE;
      }

      m_sParameter[index].m_sec_ro_filepath = as_sec_ro;
  }

  return true;
}


bool TfrmMultiLoad::Display_GPS_File(unsigned int index)
{
    AnsiString as_scatter_dir = ::ExtractFileDir(m_asScatFileName[index]);
    AnsiString as_gps_dir = as_scatter_dir + "\\GPS";
    if (DirectoryExists(as_gps_dir))
    {
        unsigned int bFindGpsRom = 0;
        m_sParameter[index].m_gps_download = _TRUE;
        AnsiString as_gps_rom = as_gps_dir + "\\M-core";
        AnsiString as_gps_ini = as_gps_dir + "\\gps.ini";
        if (FileExists(as_gps_rom))
        {
            bFindGpsRom = 1;
        }
        else
        {
            TSearchRec sr;
            if (FindFirst(as_gps_dir+"\\*.bin", 0, sr) == 0)
            {
                do
                {
                    as_gps_rom = as_gps_dir + "\\" + sr.Name;
                    bFindGpsRom = 1;
                    break;
                }
                while (FindNext(sr) == 0);

                FindClose(sr);
            }
        }

        // Get the GPS GPIO information from gps.ini
      if (bFindGpsRom && FileExists(as_gps_ini))
      {
          if(!get_GPS_gpio_information_from_GPS_ini(index, as_gps_dir, as_gps_rom, as_gps_ini))
          {
             return false;
          }
      }
      else
      {
          m_sParameter[index].m_gps_enable        = _FALSE;
      }
    }
    else
    {
      m_sParameter[index].m_gps_download  = (_BOOL) false;
    }

    return true;
}

bool  TfrmMultiLoad::get_GPS_gpio_information_from_GPS_ini(int index, AnsiString gps_dir, AnsiString gps_rom,
                                                           AnsiString gps_ini)
{
    m_sParameter[index].m_gps_enable        = _TRUE;
    m_sParameter[index].m_gps_rom_dirpath   = gps_dir;
    m_sParameter[index].m_gps_rom_filepath  = gps_rom;
    m_sParameter[index].m_gps_ini_filepath  = gps_ini;

    if (!LoadFileInfo(gps_rom.c_str(), &m_sParameter[index].m_gps_rom_filesize, NULL))
    {
        Application->MessageBox("Load file information fail", "FAIL", MB_OK);
        return false;
    }

    if(!read_GPS_gpio_information_from_GPS_ini(index, gps_ini))
    {
        Application->MessageBox("Load file information fail", "FAIL", MB_OK);
        return false;
    }

    return true;
}

bool TfrmMultiLoad::read_GPS_gpio_information_from_GPS_ini(int index, AnsiString gps_ini)
{
    TIniFile *p_ini = new TIniFile(gps_ini);

    if(NULL == p_ini)
    {
       return false;
    }

    m_sParameter[index].m_gps_power_pin     = p_ini->ReadInteger("GPIO", "gpio_gps_power_pin",   0xFFFF);
    m_sParameter[index].m_gps_reset_pin     = p_ini->ReadInteger("GPIO", "gpio_gps_reset_pin",   0xFFFF);
    m_sParameter[index].m_gps_standby_pin   = p_ini->ReadInteger("GPIO", "gpio_gps_standby_pin", 0xFFFF);
    m_sParameter[index].m_gps_32k_pin       = p_ini->ReadInteger("GPIO", "gpio_gps_32k_pin",     0xFFFF);

    delete p_ini;

    return true;
}
//-----------------------------------------------------------------------------
void TfrmMultiLoad::Save_ScatterInfo(bool is_redraw)
{
      try
    {
      for (int i=0; i<MAX_SUPPORT_LOAD_NUM; i++)
      {
             m_INIFileH.write_SCATTER(INI_FILE_NAME, Application->ExeName, b_SCATTER_checkEnable);
	      m_INIFileH.write_Checksum_Ver(INI_FILE_NAME, Application->ExeName, b_checksum_ver);
             m_INIFileH.write_SecroReworkingSupport(INI_FILE_NAME, Application->ExeName, MainForm->SecroReworkingEnable);
      }
    }
    catch(...)
    {
       //int a = 0;
	   GC_TRACE("<MDLF> Save_ScatterAndLoad() exception!");
    }

     
}
//-----------------------------------------------------------------------------
void TfrmMultiLoad::Save_ScatterAndLoad(bool is_redraw)
{
    DL_HANDLE_T* p_dl_handle;

    try
    {
      for (int i=0; i<MAX_SUPPORT_LOAD_NUM; i++)
      {
          if(is_redraw)
          {
              RedrawLVCheckBox(i);
          }

          if (m_asScatFileName[i].AnsiCompareIC("") != 0)
          {
              m_INIFileH.write_Scatter(INI_FILE_NAME, Application->ExeName, i+1, m_lblScatFileName[i]->Caption);

              #ifndef SERVICE_CENTER
              m_INIFileH.write_FAT(INI_FILE_NAME, Application->ExeName, i+1, m_asFATFileName[i]);
              m_INIFileH.write_LV_FAT_index(INI_FILE_NAME, Application->ExeName, i+1, IntToStr(m_iLV_FAT_index));
              #endif


              if(BT_UBOOT == get_bt_type())
              {
                  m_INIFileH.write_SCATTER(INI_FILE_NAME, Application->ExeName, b_SCATTER_checkEnable);
		              m_INIFileH.write_Checksum_Ver(INI_FILE_NAME, Application->ExeName, b_checksum_ver);
                  m_INIFileH.write_Preloader(INI_FILE_NAME, Application->ExeName, i+1, m_preloader_path[i]);
                  m_INIFileH.write_Preloader_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_preloader_checked[i]);

                  m_INIFileH.write_DSPBL(INI_FILE_NAME, Application->ExeName, i+1, m_DSPBL_path[i]);
                  m_INIFileH.write_DSPBL_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_DSPBL_checked[i]);

                  m_INIFileH.write_UBoot(INI_FILE_NAME, Application->ExeName, i+1, m_uboot_path[i]);
                  m_INIFileH.write_UBoot_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_uboot_checked[i]);

                  m_INIFileH.write_Boot(INI_FILE_NAME, Application->ExeName, i+1, m_Boot_path[i]);
                  m_INIFileH.write_Boot_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_Boot_checked[i]);

                  m_INIFileH.write_Logo(INI_FILE_NAME, Application->ExeName, i+1, m_Logo_path[i]);
                  m_INIFileH.write_Logo_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_Logo_checked[i]);

				  m_INIFileH.write_TEE1(INI_FILE_NAME, Application->ExeName, i+1, m_TEE1_path[i]);
                  m_INIFileH.write_TEE1_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_TEE1_checked[i]);

				  
				  m_INIFileH.write_TEE2(INI_FILE_NAME, Application->ExeName, i+1, m_TEE2_path[i]);
				  m_INIFileH.write_TEE2_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_TEE2_checked[i]);

                  m_INIFileH.write_Secro(INI_FILE_NAME, Application->ExeName, i+1, m_Secro_path[i]);
                  m_INIFileH.write_Secro_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_Secro_checked[i]);

                  m_INIFileH.write_Recovery(INI_FILE_NAME, Application->ExeName, i+1, m_Recovery_path[i]);
                  m_INIFileH.write_Recovery_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_Recovery_checked[i]);

                  m_INIFileH.write_AndroidSystem(INI_FILE_NAME, Application->ExeName, i+1, m_AndroidSystem_path[i]);
                  m_INIFileH.write_AndroidSystem_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_AndroidSystem_checked[i]);

                  m_INIFileH.write_UserData(INI_FILE_NAME, Application->ExeName, i+1, m_UserData_path[i]);
                  m_INIFileH.write_UserData_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_UserData_checked[i]);

                  m_INIFileH.write_Custpack(INI_FILE_NAME, Application->ExeName, 1, i+1, m_Custpack1_path[i]);
                  m_INIFileH.write_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 1, i+1, b_Custpack1_checked[i]);

                  m_INIFileH.write_Custpack(INI_FILE_NAME, Application->ExeName, 2, i+1, m_Custpack2_path[i]);
                  m_INIFileH.write_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 2, i+1, b_Custpack2_checked[i]);

                  m_INIFileH.write_Custpack(INI_FILE_NAME, Application->ExeName, 3, i+1, m_Custpack3_path[i]);
                  m_INIFileH.write_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 3, i+1, b_Custpack3_checked[i]);

                  m_INIFileH.write_Custpack(INI_FILE_NAME, Application->ExeName, 4, i+1, m_Custpack4_path[i]);
                  m_INIFileH.write_Custpack_Checked(INI_FILE_NAME, Application->ExeName, 4, i+1, b_Custpack4_checked[i]);

				  //eMMC added
				  m_INIFileH.write_emmc_FAT(INI_FILE_NAME, Application->ExeName, i+1, m_FAT_path[i]);
                  m_INIFileH.write_emmc_FAT_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_FAT_checked[i]);

				  m_INIFileH.write_cache(INI_FILE_NAME, Application->ExeName, i+1, m_cache_path[i]);
                  m_INIFileH.write_cache_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_cache_checked[i]);

				  m_INIFileH.write_EBR1(INI_FILE_NAME, Application->ExeName, i+1, m_EBR1_path[i]);
                  m_INIFileH.write_EBR1_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_EBR1_checked[i]);

				  m_INIFileH.write_EBR2(INI_FILE_NAME, Application->ExeName, i+1, m_EBR2_path[i]);
                  m_INIFileH.write_EBR2_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_EBR2_checked[i]);

				  m_INIFileH.write_MBR(INI_FILE_NAME, Application->ExeName, i+1, m_MBR_path[i]);
                  m_INIFileH.write_MBR_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_MBR_checked[i]);


				  map<string,ImageDetailInfo*>::iterator it = imageMap->begin();

				  for(; it!= imageMap->end(); it++)
				  {
					  string imageScatterName = it->first;
					  //AnsiString imagePrefixName = it->second->GetImagePrefixName();
					  _BOOL imageChecked = it->second->GetImageChecked();
					  AnsiString imagePath = it->second->GetImagePath();
					  NewIniHandle::WriteStringToIni(INI_FILE_NAME, Application->ExeName, i+1, "form", (AnsiString)imageScatterName.c_str(), imagePath);
					  NewIniHandle::WriteBoolToIni(INI_FILE_NAME, Application->ExeName, i+1, "form", (AnsiString)imageScatterName.c_str(), imageChecked);

				  }


              }
              else //BT_UBOOT
              {
             //mloder & Eboot
             m_INIFileH.write_MLoader(INI_FILE_NAME, Application->ExeName, i+1, m_xldr_path[i]);
             m_INIFileH.write_EBoot(INI_FILE_NAME, Application->ExeName, i+1, m_eboot_path[i]);

             m_INIFileH.write_MLoader_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_xlder_checked[i]);
             m_INIFileH.write_EBoot_Checked(INI_FILE_NAME, Application->ExeName, i+1, b_eboot_checked[i]);
              }

              // security RO
              m_INIFileH.write_SEC_RO(INI_FILE_NAME, Application->ExeName, i+1, m_sParameter[i].m_sec_ro_filepath);
              if (m_sParameter[i].m_sec_ro_enable)
              {
                  m_INIFileH.write_SEC_RO_Checked(INI_FILE_NAME, Application->ExeName, i+1, "yes");
              }
              else
              {
                  m_INIFileH.write_SEC_RO_Checked(INI_FILE_NAME, Application->ExeName, i+1, "no");
              }

              // GPS
              m_INIFileH.write_GPS_ROM_Path(INI_FILE_NAME, Application->ExeName, i+1, m_sParameter[i].m_gps_rom_filepath);
              if (m_sParameter[i].m_gps_enable)
              {
                  m_INIFileH.write_GPS_ROM_Checked(INI_FILE_NAME, Application->ExeName, i+1, "yes");
              }
              else
              {
                  m_INIFileH.write_GPS_ROM_Checked(INI_FILE_NAME, Application->ExeName, i+1, "no");
              }
          }
      }
    }
    catch(...)
    {
       //int a = 0;
	   GC_TRACE("<MDLF> Save_ScatterAndLoad() exception!");
    }

}



//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::btnScatFileClick(TObject *Sender)
{
    TButton *btn = (TButton*)Sender;
   // FileOpenScatExecute( (int) btn->Tag );
   ScatterFileOpenExecute((int) btn->Tag);
}
//-----------------------new load scatter file flow(gone .li 2013.11.21)----------------
bool TfrmMultiLoad::ScatterFileOpenExecute(int idx)
{
    int ret;
    DL_HANDLE_T *p_temp_dl_handle = new DL_HANDLE_T;
    DL_Create(p_temp_dl_handle);
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
    DL_HANDLE_LIST_T *p_dl_handle_list = MainForm->Get_DlHandleList();
	 
    if (m_asScatPath[idx].Length() > 0)
    {
        m_dlgOpenScatter[idx]->InitialDir = m_asScatPath[idx];
    }

    if (m_dlgOpenScatter[idx]->Execute())    
    {
               //Auto load scatter file by two API(DL_LoadScatter,DL_AutoLoadRomImages)
        ret=DL_LoadScatter(*p_temp_dl_handle, m_dlgOpenScatter[idx]->FileName.c_str(), NULL);
        if (ret)
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                ShowMessage("Program is busy! Can not reload scat file right now.");
            }
            else if( S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN == ret )
            {
                ShowMessage("Failed to load scatter file \"" + m_dlgOpenScatter[idx]->FileName + "\"!\n\nThe offset value of one load region is not WORD alignment!");
            }
            else if( S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN == ret )
            {
                ShowMessage("Failed to load scatter file \"" + m_dlgOpenScatter[idx]->FileName + "\"!\n\nThe region begin address of one load region is not WORD alignment!");
            }
            else if( S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER == ret )
            {
                ShowMessage("Failed to load scatter file \"" + m_dlgOpenScatter[idx]->FileName + "\"!\n\nThe region begin address of one load region is not ascending sorting sequence!");
            }
            else
            {
                ShowMessage("Incorrect scatter file format!");
            }
            return false;
        }
		
        ret = DL_AutoLoadRomImages(*p_temp_dl_handle, m_dlgOpenScatter[idx]->FileName.c_str());
        if(ret != S_DONE && ret != 1)
        {
			GC_TRACE("<MDLF> ScatterFileOpenExecute(): Rom files auto load failed, ret(%d)", ret);
			Application->MessageBox("Load Scatter File Fail", "FAIL", MB_OK);
            return false;
        }
        // DL_HANDLE process
        if (NULL != p_dl_handle) // not new handle
        {
            if (0 != DL_CheckHandleInList(*p_dl_handle_list, *p_dl_handle)) // handle in list
            {
                ret = DL_DeleteHandleFromList(*p_dl_handle_list, *p_dl_handle);
                if (ret != 0)
                {
                    Application->MessageBox("Delete handle from list fail", "FAIL", MB_OK);
                    return false;
                }
            }
            Destroy_DLHandle(p_dl_handle);
            //p_dl_handle = p_temp_dl_handle;
            MainForm->Set_DlHandle(idx, p_temp_dl_handle);
            ret = DL_AddHandleToList(*p_dl_handle_list, *p_temp_dl_handle);
            if (ret != 0)
            {
                Application->MessageBox("Add handle to list fail", "FAIL", MB_OK);
                return false;
            }
            SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);
            MainForm->SetDAChecksum(MainForm->GetChecksumEnable());

        }
        else // new handle
        {
            //p_dl_handle = p_temp_dl_handle;
            ret = DL_AddHandleToList(*p_dl_handle_list, *p_temp_dl_handle);
            if (ret != 0)
            {
                Application->MessageBox("Add handle to list fail", "FAIL", MB_OK);
                return false;
            }
            SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);
                       //Destroy_DLHandle(p_temp_dl_handle);
        }
        
        //auto detect secro image or scr-lock-sign.img
        AutoLoadSecroSignImg(p_temp_dl_handle, m_dlgOpenScatter[idx]->FileName.c_str());
        
	    //platform info and update BMT
        GetPlatformInfoAndBmtBlock(m_dlgOpenScatter[idx]->FileName, p_temp_dl_handle,TRUE);
		
        UpdateRawLVListAndSaveRomInfo(p_temp_dl_handle,m_dlgOpenScatter[idx]->FileName);
        SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);

		//Set DA checksum for download verify
        MainForm->SetDAChecksum(MainForm->GetChecksumEnable());
		
        RedrawTypeComBox();
        //RedrawLVParamList(idx, p_dl_handle);
        MainForm->Update_UI();

		//secro download for re-work
        MainForm->SecroReworkingEnable = m_INIFileH.read_SecroReworkingSupport(INI_FILE_NAME, Application->ExeName);
        if(MainForm->SecroReworkingEnable)
        {
		     //Save_ScatterAndLoad(false); //update scatter file
		     //Save_ScatterAndLoad(false);
		    GC_TRACE("<Debug> Display_ScatterAndLoad: Do LoadSecroScatter-(SecroReworkingEnable)");
            LoadSecroScatter(m_dlgOpenScatter[idx]->FileName);
        }
		else
		    GC_TRACE("<Debug> Display_ScatterAndLoad: SecroReworkingEnable NOT true");
		
        return true;
    }
	  return false;
}
void TfrmMultiLoad:: UpdateRawLVListAndSaveRomInfo(DL_HANDLE_T *p_dl_handle,AnsiString scatter_file)
{
     unsigned short rom_count;
     ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
     m_lv[0]->Items->Clear();
    if(!DL_GetCount(*p_dl_handle, &rom_count))
    {
        if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS))
        {    
            for (int i=0; i<rom_count; i++)
            {
                AnsiString  as_reg_prefix;
                TListItem	*p_list_item;
                if(rom_info[i].item_is_visable)
                {
                    //updte RawLVList
                    p_list_item = m_lv[0]->Items->Add();
                    p_list_item->Caption = rom_info[i].name;
                    p_list_item->Checked = rom_info[i].enable;
                    if (ABSOLUTE_ADDR == rom_info[i].addr_type)
                    {
                        as_reg_prefix = "";
                    }
                    else
                    {
                        as_reg_prefix = "+";
                    }
                    p_list_item->SubItems->Add(as_reg_prefix + "0x" + IntToHex((int)rom_info[i].region_addr, 8));
                    p_list_item->SubItems->Add("0x" + IntToHex((int)rom_info[i].begin_addr, 8));
                    p_list_item->SubItems->Add("0x" + IntToHex((int)rom_info[i].end_addr, 8));
                    p_list_item->SubItems->Add(rom_info[i].filepath);
                    //save download image path to SPMultiPortFlashDownloadProject.ini
                    string m_RomName(rom_info[i].name);
                    string m_RomPath(rom_info[i].filepath);
                    m_INIFileH.write_RomInfo(INI_FILE_NAME, Application->ExeName,m_RomName.c_str() , m_RomPath.c_str(), "Bin_File_Path");
                }
                else
                {
                    continue;
                }                  	    
              }
            }
        }
        m_INIFileH.write_RomInfo(INI_FILE_NAME, Application->ExeName,"ScatterFile", scatter_file, "Scatter_File_Path");  
        m_lv[0]->Enabled=b_SCATTER_checkEnable;
        MainForm->ScatterEnable=b_SCATTER_checkEnable;
        // add md5 ,20130413.
        MainForm->Checksum_ver=b_checksum_ver;

        MainForm->ScatterLoadCompleteFlag=FALSE;
        if(!DL_GetCount(*p_dl_handle, &rom_count))
        {
             ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
             if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS))
             {
                for (int i=0; i<rom_count; i++)
                 {
                    if(_TRUE == rom_info[i].item_is_visable)
                        {
	                      if(rom_info[i].enable !=_TRUE)
	                      	{
	                      	MainForm->ScatterLoadCompleteFlag=TRUE;
	                      	break;
	                      	}
                         }
             	    }
              }
       }
}

/*
function: this function is replaced by "ScatterFileOpenExecute" function.
//--------------------------------------------------------------------------
bool TfrmMultiLoad::FileOpenScatExecute(int idx)
{

    int ret;
    DL_HANDLE_T *p_temp_dl_handle = new DL_HANDLE_T;
    DL_Create(p_temp_dl_handle);
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
    DL_HANDLE_LIST_T *p_dl_handle_list = MainForm->Get_DlHandleList();

    if (m_asScatPath[idx].Length() > 0)
    {
        m_dlgOpenScatter[idx]->InitialDir = m_asScatPath[idx];
    }

    if (m_dlgOpenScatter[idx]->Execute())    // *.bin file
    {

        if (OpenReadScatFile(m_dlgOpenScatter[idx]->FileName, idx, p_temp_dl_handle))
        {
            DelLoad(idx);  // press Delete Load button first to delete old scatter , because WinMo scat handle and Android scat handle are same.
            m_asScatFileName[idx] = m_dlgOpenScatter[idx]->FileName;
            int iLD = m_asScatFileName[idx].LastDelimiter("\\:");

            if (iLD > 0)
            {
                m_asScatPath[idx] = m_asScatFileName[idx].SubString(1, iLD-1 );
            }

            if (MainForm->Get_AutoLoadResByScatFlag())
            {
                ret = DL_AutoLoadByScatRegionName(*p_temp_dl_handle, m_dlgOpenScatter[idx]->FileName.c_str(), NORMAL_ROM, _TRUE);
                if (ret != 0)
                {
                    Application->MessageBox("Automatic load by scatter region name fail, please load images manually!", "Reminder", MB_OK);
                    //Destroy_DLHandle(p_temp_dl_handle);
                    //return false;
                }
            }

            // update handle list
            if (NULL != p_dl_handle) // not new handle
            {
                ret = DL_CheckHandleSameSWVersionInList(*p_dl_handle_list, *p_temp_dl_handle);
                if (0 == ret) // no same S/W version in list
                {
                    if (0 != DL_CheckHandleInList(*p_dl_handle_list, *p_dl_handle)) // handle in list
                    {
                        ret = DL_DeleteHandleFromList(*p_dl_handle_list, *p_dl_handle);
                        if (ret != 0)
                        {
                            Application->MessageBox("Delete handle from list fail", "FAIL", MB_OK);
                            return false;
                        }
                    }
                    Destroy_DLHandle(p_dl_handle);
                    p_dl_handle = p_temp_dl_handle;
                    MainForm->Set_DlHandle(idx, p_dl_handle);
                    ret = DL_AddHandleToList(*p_dl_handle_list, *p_dl_handle);
                    {
                        if (ret != 0)
                        {
                            Application->MessageBox("Add handle to list fail", "FAIL", MB_OK);
                            return false;
                        }
                    }

                    SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);
                    MainForm->SetDAChecksum(MainForm->GetChecksumEnable());
                }
                else // same S/W in handle list
                {
                    Destroy_DLHandle( p_temp_dl_handle  );
                    Application->MessageBox( "Has same S/W in handle list", "NOTICE", MB_OK );
                    return false;
                }

            }
            else // new handle
            {
                p_dl_handle = p_temp_dl_handle;
                ret = DL_AddHandleToList(*p_dl_handle_list, *p_dl_handle);
                {
                    if (ret != 0)
                    {
                        Application->MessageBox("Add handle to list fail", "FAIL", MB_OK);
                        return false;
                    }
                }

                SetScatterFileCaption(idx, m_dlgOpenScatter[idx]->FileName);
                Destroy_DLHandle(p_temp_dl_handle);
            }

            // secure RO
            AnsiString as_sec_ro = m_asScatPath[idx] + "\\SECURE_RO";
            m_sParameter[idx].m_sec_ro_enable = _FALSE;
            m_sParameter[idx].m_sec_ro_filepath = "";
            if (as_sec_ro.AnsiCompareIC("") != 0)
            {
                if( LoadFile(as_sec_ro.c_str(), &m_sParameter[idx].m_sec_ro.m_data,
                                                &m_sParameter[idx].m_sec_ro.m_len, NULL))
                {
                    m_sParameter[idx].m_sec_ro_filepath = as_sec_ro;
                    m_sParameter[idx].m_sec_ro_enable = _TRUE;
                }
            }

            // GPS, get GPS ROM path and gps.ini path
            AnsiString as_scatter_dir = ExtractFileDir(m_dlgOpenScatter[idx]->FileName);
            AnsiString as_gps_dir = as_scatter_dir + "\\GPS";
            if (DirectoryExists(as_gps_dir))
            {
                unsigned int bFindGpsRom = 0;
                m_sParameter[idx].m_gps_download = _TRUE;
                AnsiString as_gps_rom = as_gps_dir + "\\M-core";
                AnsiString as_gps_ini = as_gps_dir + "\\gps.ini";
                if (FileExists(as_gps_rom))
                {
                    bFindGpsRom = 1;
                }
                else
                {
                    TSearchRec sr;
                    if (FindFirst(as_gps_dir+"\\*.bin", 0, sr) == 0)
                    {
                        do
                        {
                            as_gps_rom = as_gps_dir + "\\" + sr.Name;
                            bFindGpsRom = 1;
                            break;
                        }
                        while (FindNext(sr) == 0);
                        FindClose(sr);
                    }
                }

                // Get the GPS GPIO information from gps.ini
                if (bFindGpsRom && FileExists(as_gps_ini))
                {
                    if(!get_GPS_gpio_information_from_GPS_ini(idx, as_gps_dir, as_gps_rom, as_gps_ini))
                    {
                       return false;
                    }
                }
                else
                {
                    m_sParameter[idx].m_gps_enable  = _FALSE;
                }
            }
            else
            {
                m_sParameter[idx].m_gps_download  = (_BOOL) false;
            }

            Auto_load_uRoms(as_scatter_dir.c_str() ,p_dl_handle);
            RedrawLVList(idx, p_dl_handle);
            RedrawTypeComBox();
            RedrawLVParamList(idx, p_dl_handle);

            if(m_bPlatformChange)
            {
                E_CHIP_GROUP eChipGroup;
                eChipGroup = MainForm->CheckChipGroup(m_chip_type);
                if(BBCHIP_GROUP0 == eChipGroup)
                {
                     MainForm->Reset_UIComponent();
		                 GC_TRACE("<MDLF> BBCHIP_MT6516 == m_chip_type");
                }
                else if (BBCHIP_GROUP1 == eChipGroup)
                {
                    // Added by Shaoying Han
                    MainForm->Set6573DefaultChecked();
		                GC_TRACE("<MDLF> BBCHIP_MT6573 == m_chip_type");
                }
                else if (BBCHIP_GROUP2 == eChipGroup)
                {
                    MainForm->Set6575DefaultChecked();
		               	GC_TRACE("<MDLF> m_chip_type = %d ", m_chip_type);
                }
	         }
	         MainForm->Update_UI();


			MainForm->SecroReworkingEnable = m_INIFileH.read_SecroReworkingSupport(INI_FILE_NAME, Application->ExeName);
			if(MainForm->SecroReworkingEnable)
		        {
		        	Save_ScatterAndLoad(false); //update scatter file
		        	GC_TRACE("<Debug> Display_ScatterAndLoad: Do LoadSecroScatter-(SecroReworkingEnable)");
		            //LoadSecroScatter();
		        }
			else
				GC_TRACE("<Debug> Display_ScatterAndLoad: SecroReworkingEnable NOT true");
	
            return true;
         }
         else
         {
             return false;
         }
    }
    else
    {
        return false;
    }
}
*/

//--------------------------------------------------------------------------
void TfrmMultiLoad::BromDLTypeTblEntry(DL_HANDLE_T *p_dl_handle )
{

	int ret = -1;
	ImagesFormat imagesFormat;
	std::map<string, ImageDetailInfo*> *imageFormatMap = imagesFormat.GetImageForamtMap();
	std::map<string, ImageDetailInfo*>::iterator mapIterator = imageFormatMap->begin();
	std::string imageName;
	int imageFormat;
	for(; mapIterator != imageFormatMap->end(); mapIterator++)
	{
		imageName = mapIterator->second->GetImagePrefixName().c_str();
		imageFormat = mapIterator->second->GetBromFormat();
		GC_TRACE("<TfrmMultiLoad> BromDLTypeTblEntry   the image name is %s .", imageName);
		GC_TRACE("<TfrmMultiLoad> BromDLTypeTblEntry   the image brom format is %d .", imageFormat);

		ret = DL_ImgTypeTblEntryAdd(*p_dl_handle, imageName.c_str(), static_cast<ROM_TYPE>(imageFormat));
		if (S_DONE != ret)
		{
			GC_TRACE("ERROR: BromDLTypeTblEntry  DL_ImgTypeTblEntryAdd set fail,the image name is %s .", imageName);
			return;
		}
	}


	DL_ImgTypeTblEntryList(*p_dl_handle);

}


//--------------------------------------------------------------------------
bool TfrmMultiLoad::OpenReadScatFile4SecroReworking(AnsiString asMapFileName, int idx, DL_HANDLE_T *p_dl_handle )
{
    int ret;
    unsigned short rom_count;
    AnsiString name;
    AnsiString chipName;
    char tmpBuf[256];

	  TformPCMemoryChecksum::mChecksumFlag = FALSE;

	 // BromDLTypeTblEntry(p_dl_handle);

    //Add By wuxiong ----2013/03/27
    ret=DL_LoadScatter(*p_dl_handle, asMapFileName.c_str(),NULL);
    if (ret)
    {
        if( S_FTHND_HANDLE_BUSY_NOW == ret )
        {
            ShowMessage("Program is busy! Can not reload scat file right now.");
        }
        else if( S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe offset value of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe region begin address of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe region begin address of one load region is not ascending sorting sequence!");
        }
        else
        {
            ShowMessage("Incorrect scatter file format!");
        }
        return false;
    }

    DL_PlatformInfo plat_info;
    DL_GetPlatformInfo(*p_dl_handle, &plat_info);

    //check whether support chip type
    if(plat_info.m_bbchip_type == UNKNOWN_BBCHIP_TYPE)
    {
        ShowMessage("Not Support chip type:"+ AnsiString(plat_info.m_szBBChipType));
        return false;
    }

    //check whether whether support storage type
    //if()

    //Get m_strage_type
    m_Storage_Type = plat_info.m_storage_type;

    if(m_chip_type != plat_info.m_bbchip_type)
    {
        m_bPlatformChange = true;
        m_chip_type = plat_info.m_bbchip_type;
    }
    else
    {
        m_bPlatformChange = false;
    }

    unsigned int bmt_block_count = 0;
    ret = DL_GetBMTBlockCount(*p_dl_handle, &bmt_block_count);
    //if(S_DONE != ret)
     if(S_DONE != ret && ret != S_INVALID_BMTSIZE)
	{
        GC_TRACE("<MDLF> Get BMT block count fail!Set default value to 80.");
        m_BMT_blocks = 80;
    }
    else
    {
        GC_TRACE("<MDLF> Get BMT block count is %d.", bmt_block_count);
        m_BMT_blocks = bmt_block_count;
       if(m_Storage_Type==1)  // if nand Falsh ,check BMT number.mtk71502,20130409
       {
           if(bmt_block_count > MAX_BMT_SIZE)
           {
            sprintf(tmpBuf, "Error: BMT size is too large: %d > Max size:%d",bmt_block_count,MAX_BMT_SIZE);
            ShowMessage(AnsiString(tmpBuf));
            return false;
           }
       }
    }
    return true;
}
//----------------------gone.li----------------------------------------------------
bool TfrmMultiLoad::GetPlatformInfoAndBmtBlock(AnsiString asMapFileName, DL_HANDLE_T *p_dl_handle,BOOL checksumFile)
{
    int ret;
    unsigned short rom_count;
    AnsiString name;
    AnsiString chipName;
    char tmpBuf[256];

    TformPCMemoryChecksum::mChecksumFlag = FALSE;

    
    //Get chip type and storage type
    if(checksumFile)
    {
           if('\0' != asMapFileName[1])
              {
                  int iLD = asMapFileName.LastDelimiter("\\");
                  if(iLD>0)
                  {
                       name = asMapFileName.SubString(iLD + 1, asMapFileName.Length() -iLD);
                       GC_TRACE("<MDLF> name: %s", name.c_str());
                       iniFilePath = asMapFileName.SubString(0, iLD - 1);
                       GC_TRACE("<MDLF> <OpenReadScatFile> iniFilePath is : %s", iniFilePath.c_str());
                   }
    	      }
    }
 
     DL_PlatformInfo plat_info;
     DL_GetPlatformInfo(*p_dl_handle, &plat_info);
	 
      ret = DL_GetScatterInfo(*p_dl_handle, &m_scatter_info);
      if(ret == S_DONE)
	 {
	     GC_TRACE("<MDLF> DL_GetScatterInfo Success.Version of m_scatter_info is %s", m_scatter_info.version);
	  }else
	  {
	      GC_TRACE("<MDLF> DL_GetScatterInfo Fail.");
	  }
	

    //check whether support chip type
    if(plat_info.m_bbchip_type == UNKNOWN_BBCHIP_TYPE)
    {
        ShowMessage("Not Support chip type:"+ AnsiString(plat_info.m_szBBChipType));
        return false;
    }

    //check whether whether support storage type

    //Get m_strage_type
    m_Storage_Type = plat_info.m_storage_type;

    if(m_chip_type != plat_info.m_bbchip_type)
    {
        m_bPlatformChange = true;
        m_chip_type = plat_info.m_bbchip_type;
    }else{
        m_bPlatformChange = false;
    }

    unsigned int bmt_block_count = 0;
    ret = DL_GetBMTBlockCount(*p_dl_handle, &bmt_block_count);
    //if(S_DONE != ret)
    if(S_DONE != ret && ret != S_INVALID_BMTSIZE)
	{
        GC_TRACE("<MDLF> Get BMT block count fail!Set default value to 80.");
        m_BMT_blocks = 80;
    }else{
        GC_TRACE("<MDLF> Get BMT block count is %d.", bmt_block_count);
        m_BMT_blocks = bmt_block_count;
        if(m_Storage_Type==1)  // if nand Falsh ,check BMT number.mtk71502,20130409
        {
           if(bmt_block_count > MAX_BMT_SIZE)
           {
               sprintf(tmpBuf, "Error: BMT size is too large: %d > Max size:%d",bmt_block_count,MAX_BMT_SIZE);
               ShowMessage(AnsiString(tmpBuf));
               return false;
           }
        }
    }
    return true;
}

void TfrmMultiLoad::ForceDLWithBattery()
{
    switch(m_chip_type)
    {
        case MT6595:
		case MT6752:
            //Not support without battery
            MainForm->m_miDADLBattery->Enabled = true;
            MainForm->m_miDADLNoBattery->Enabled = false;
            MainForm->m_miDADLAutoDetect->Enabled = false;

            MainForm->m_miDADLBattery->Checked = true;
            MainForm->m_miDADLNoBattery->Checked = false;
            MainForm->m_miDADLAutoDetect->Checked = false;
            break;
        default:
            MainForm->m_miDADLBattery->Enabled = true;
            MainForm->m_miDADLNoBattery->Enabled = true;
            MainForm->m_miDADLAutoDetect->Enabled = true;
            break;
	   	   
    }
}

//--------------------------------------------------------------------------
bool TfrmMultiLoad::OpenReadScatFile(AnsiString asMapFileName, int idx, DL_HANDLE_T *p_dl_handle )
{
    int ret;
    unsigned short rom_count;
    AnsiString name;
    AnsiString chipName;
    char tmpBuf[256];

    TformPCMemoryChecksum::mChecksumFlag = FALSE;

    //Get chip type and storage type
    if('\0' != asMapFileName[1])
    {
        int iLD = asMapFileName.LastDelimiter("\\");
        if(iLD>0)
        {
             name = asMapFileName.SubString(iLD + 1, asMapFileName.Length() -iLD);
             GC_TRACE("<MDLF> name: %s", name.c_str());
             iniFilePath = asMapFileName.SubString(0, iLD - 1);
             GC_TRACE("<MDLF> <OpenReadScatFile> iniFilePath is : %s", iniFilePath.c_str());
        }// MTk71502,20130409
#if 0
        if (iLD > 0)
        {

            name = asMapFileName.SubString(iLD + 1, asMapFileName.Length() -iLD);
            GC_TRACE("<MDLF> name: %s", name.c_str());
            iniFilePath = asMapFileName.SubString(0, iLD - 1);
            GC_TRACE("<MDLF> <OpenReadScatFile> iniFilePath is : %s", iniFilePath.c_str());
            if (name.Pos(SCATTER_STORAGE_TYPE_EMMC) > 0 && name.Pos(SCATTER_STORAGE_TYPE_EMMC) < asMapFileName.Length())
            {
                m_Storage_Type = HW_STORAGE_EMMC;
                GC_TRACE("<MDLF> The storage type is eMMC.------------------");
            }
            else if (name.Pos(SCATTER_STORAGE_TYPE_SDMMC) > 0 && name.Pos(SCATTER_STORAGE_TYPE_SDMMC) < asMapFileName.Length())
            {
                m_Storage_Type = HW_STORAGE_SDMMC;
                GC_TRACE("<MDLF> The storage type is SDMMC.------------------");
            }
            else
            {
                m_Storage_Type = HW_STORAGE_NAND;
                GC_TRACE("<MDLF> The storage type is NAND.-------------------");
            }
        }

        if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6516_PREFIX, strlen(SCATTER_BBCHIP6516_PREFIX)) )
        {
            m_bPlatformChange = (BBCHIP_MT6516 == m_chip_type) ? false : true;
            m_chip_type = BBCHIP_MT6516;
            chipName = SCATTER_BBCHIP6516_PREFIX;
            GC_TRACE("<MDLF> The scatter file of MT6516 is loaded.");
        }
        else if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6573_PREFIX, strlen(SCATTER_BBCHIP6573_PREFIX)) )
        {
            m_bPlatformChange = (BBCHIP_MT6573 == m_chip_type) ? false : true;
            m_chip_type = BBCHIP_MT6573;
            chipName = SCATTER_BBCHIP6573_PREFIX;
            GC_TRACE("<MDLF> The scatter file of MT6573 is loaded.");
        }
        else if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6575_PREFIX, strlen(SCATTER_BBCHIP6575_PREFIX)) )
        {
            m_bPlatformChange = (BBCHIP_MT6575 == m_chip_type) ? false : true;
            m_chip_type = BBCHIP_MT6575;
            chipName = SCATTER_BBCHIP6575_PREFIX;
            GC_TRACE("<MDLF> The scatter file of MT6575 is loaded.");
        }
		else if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6577_PREFIX, strlen(SCATTER_BBCHIP6577_PREFIX)) )
        {
            m_bPlatformChange = (BBCHIP_MT6577 == m_chip_type) ? false : true;
            m_chip_type = BBCHIP_MT6577;
            chipName = SCATTER_BBCHIP6577_PREFIX;
            GC_TRACE("<MDLF> The scatter file of MT6577 is loaded.");
        }
		else if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6589_PREFIX, strlen(SCATTER_BBCHIP6589_PREFIX)) )
        {
            m_bPlatformChange = (BBCHIP_MT6589 == m_chip_type) ? false : true;
            m_chip_type = BBCHIP_MT6589;
            chipName = SCATTER_BBCHIP6589_PREFIX;
            GC_TRACE("<MDLF> The scatter file of MT6589 is loaded.");
        }
        else if(0 == strncmpi(name.c_str(), SCATTER_BBCHIP6572_PREFIX, strlen(SCATTER_BBCHIP6572_PREFIX)) )
        {
             m_bPlatformChange = (BBCHIP_MT6572 == m_chip_type) ? false : true;
             m_chip_type = BBCHIP_MT6572;
             chipName = SCATTER_BBCHIP6572_PREFIX;
             GC_TRACE("<MDLF> The scatter file of MT6572 is loaded.");
        }
        else
        {
            m_chip_type = BBCHIP_UNKNOWN;
            GC_TRACE("<MDLF> The scatter file naming error! Name: %s", name.c_str());
            Application->MessageBox( "Incorrect scatter file naming!", "FAIL", MB_OK );
            return false;
        }
#endif
    }

	  BromDLTypeTblEntry(p_dl_handle);
#if 0
    if(BBCHIP_MT6516 == m_chip_type)
    {
        ret=DL_LoadScatter(*p_dl_handle, asMapFileName.c_str(), chipName.c_str()/*SCATTER_BBCHIP6516_PREFIX*/);
        if (ret)
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not reload scat file right now.", "Fail", MB_OK );
            }
            else
            {
                Application->MessageBox( "Incorrect scatter file format!", "Fail", MB_OK );
            }
            m_chip_type = BBCHIP_UNKNOWN;
            return false;
        }
    }
    else
    {
        ret=DL_LoadScatter(*p_dl_handle, asMapFileName.c_str(), chipName.c_str()/*SCATTER_BBCHIP6573_PREFIX*/);
        if (ret)
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not reload scat file right now.", "Fail", MB_OK );
            }
            else
            {
                Application->MessageBox( "Incorrect scatter file format!", "Fail", MB_OK );
            }
            m_chip_type = UNKNOWN_BBCHIP_TYPE;
            return false;
        }

        unsigned int bmt_block_count = 0;
        ret = DL_GetBMTBlockCount(*p_dl_handle, &bmt_block_count);
        if(S_DONE != ret)
        {
            GC_TRACE("<MDLF> Get BMT block count fail!Set default value to 80.");
            m_BMT_blocks = 80;
        }
        else
        {
            GC_TRACE("<MDLF> Get BMT block count is %d.", bmt_block_count);
            m_BMT_blocks = bmt_block_count;
        }
    }
#endif
    //Add By wuxiong ----2013/03/27
    ret=DL_LoadScatter(*p_dl_handle, asMapFileName.c_str(),NULL);

    if (ret)
    {
        if( S_FTHND_HANDLE_BUSY_NOW == ret )
        {
            ShowMessage("Program is busy! Can not reload scat file right now.");
        }
        else if( S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe offset value of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe region begin address of one load region is not WORD alignment!");
        }
        else if( S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER == ret )
        {
            ShowMessage("Failed to load scatter file \"" + asMapFileName + "\"!\n\nThe region begin address of one load region is not ascending sorting sequence!");
        }
        else
        {
            ShowMessage("Incorrect scatter file format!");
        }
        return false;
    }

    DL_PlatformInfo plat_info;
    DL_GetPlatformInfo(*p_dl_handle, &plat_info);

	ret = DL_GetScatterInfo(*p_dl_handle, &m_scatter_info);
	if(ret == S_DONE)
	{
	    GC_TRACE("<MDLF> DL_GetScatterInfo Success.Version of m_scatter_info is %s", m_scatter_info.version);
	}else{
	    GC_TRACE("<MDLF> DL_GetScatterInfo Fail.");
	}
	

    //check whether support chip type
    if(plat_info.m_bbchip_type == UNKNOWN_BBCHIP_TYPE)
    {
        ShowMessage("Not Support chip type:"+ AnsiString(plat_info.m_szBBChipType));
        return false;
    }

    //check whether whether support storage type

    //Get m_strage_type
    m_Storage_Type = plat_info.m_storage_type;

    if(m_chip_type != plat_info.m_bbchip_type)
    {
        m_bPlatformChange = true;
        m_chip_type = plat_info.m_bbchip_type;
    }else{
        m_bPlatformChange = false;
    }

    unsigned int bmt_block_count = 0;
    ret = DL_GetBMTBlockCount(*p_dl_handle, &bmt_block_count);
    if(S_DONE != ret && ret != S_INVALID_BMTSIZE)
    {
        GC_TRACE("<MDLF> Get BMT block count fail!Set default value to 80.");
        m_BMT_blocks = 80;
    }else{
        GC_TRACE("<MDLF> Get BMT block count is %d.", bmt_block_count);
        m_BMT_blocks = bmt_block_count;
        if(m_Storage_Type==1)  // if nand Falsh ,check BMT number.mtk71502,20130409
        {
           if(bmt_block_count > MAX_BMT_SIZE)
           {
               sprintf(tmpBuf, "Error: BMT size is too large: %d > Max size:%d",bmt_block_count,MAX_BMT_SIZE);
               ShowMessage(AnsiString(tmpBuf));
               return false;
           }
        }
    }
    return true;
}

void TfrmMultiLoad::Auto_load_uRoms(string scatter_file, DL_HANDLE_T *p_dl_handle )
{
    bool is_with_uboot = false;
    unsigned short rom_count;
    ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
    string rom_path;
    //unsigned short       cust_count = 0;
	bool noMemoryFlag  = false;

    try{

        if(!DL_GetCount(*p_dl_handle, &rom_count))
        {
            if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS))
            {
                //1st check uboot
                for (int idx=0; idx<rom_count; idx++)
                    if (strcmpi(rom_info[idx].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom_info[idx].name, SCATTER_LK_NAME) == 0)
                       is_with_uboot = true;

                if(!is_with_uboot) //auto load DUMA
                {
                   for (int i=0; i<rom_count; i++)
                   {
                   if (strcmpi(rom_info[i].name, "xldr") == 0)
                   {
                      rom_path = scatter_file + "\\MT6516_mldrnandforMTK.nb0";
                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                      {
                         m_xldr_path[0] = rom_path.c_str();
                         b_xlder_checked[0] = _TRUE;
                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                      }
                      else
                      {
                          noMemoryFlag  = true;
                      }
                   }
                   else if(strcmpi(rom_info[i].name, "eboot") == 0)
                   {
                      rom_path = scatter_file + "\\MT6516_EBOOTNAND.nb0";
                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                      {
                         m_eboot_path[0] = rom_path.c_str();
                         b_eboot_checked[0] = _TRUE;
                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                      }
                      else
                      {
                          noMemoryFlag  = true;
                      }
                   }
                  }   //for

                  rom_path = scatter_file + "\\FLASH.bin";
                  if( INVALID_FILE_ATTRIBUTES == GetFileAttributes(rom_path.c_str()) ) {
                     return ;
                  }
                   //MainForm->edit_flash_path->Text = rom_path.c_str();
                   return;
                }


                for (int i=0; i<rom_count; i++)
                {
                    //GC_TRACE("<MDLF> For scater file ,partition name %s", rom_info[i].name);
                    //GC_TRACE("<MDLF> For  scater file , item is visable %d", rom_info[i].item_is_visable);
                    if (strcmpi(rom_info[i].name, SCATTER_PRELOADER_NAME) == 0)
                    {
                        // debug erjing.20130408.mtk71502
                        // GC_TRACE("<MDLF> For PRELOADER ,name %s", rom_info[i].name);
                        // GC_TRACE("<MDLF> For PRELOADER , item is visable %d", rom_info[i].item_is_visable);
                        AnsiString temp_scat_path = scatter_file.c_str();
                        AnsiString temp_rom_file ;
                        if(search_image_files(temp_scat_path, temp_rom_file, AUTO_PRELOADER_PREFIX))
                        {
                            //rom_path = scatter_file + "\\u-boot-mt6516.bin";
                            rom_path = temp_rom_file.c_str();

                            if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                            {
                                m_preloader_path[0] = rom_path.c_str();
                                b_preloader_checked[0] = _TRUE;
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                            }
                            else
                            {
                                noMemoryFlag  = true;
                            }
                        }
                   }
                   else if (strcmpi(rom_info[i].name, SCATTER_DSPBL_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_DSPBL_PREFIX))
                       {
                            rom_path = temp_rom_file.c_str();

                            if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                            {
                                m_DSPBL_path[0] = rom_path.c_str();
                                b_DSPBL_checked[0] = _TRUE;
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                            }
                            else
                            {
                                noMemoryFlag  = true;
                            }
                        }
                   }
                   else if (strcmpi(rom_info[i].name, SCATTER_UBOOT_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_LK_PREFIX)
                         || search_image_files(temp_scat_path, temp_rom_file, AUTO_UBOOT_PREFIX)
	                       || search_image_files(temp_scat_path, temp_rom_file, AUTO_UBOOT_OLD_PREFIX))
                       {
                           //  rom_path = scatter_file + "\\u-boot-mt6516.bin";
                           rom_path = temp_rom_file.c_str();

                           if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                           {
                               m_uboot_path[0] = rom_path.c_str();
                               b_uboot_checked[0] = _TRUE;
                               DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                           }
                           else
                           {
                               noMemoryFlag  = true;
                           }
                        }
                   }
				   else if (strcmpi(rom_info[i].name, SCATTER_LK_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_LK_PREFIX))
                       {
                       //       rom_path = scatter_file + "\\u-boot-mt6516.bin";
                            rom_path = temp_rom_file.c_str();

                            if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
                            {
                                m_uboot_path[0] = rom_path.c_str();
                                b_uboot_checked[0] = _TRUE;
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
                            }
                            else
                            {
                                noMemoryFlag  = true;
                            }
                        }
                   }
                   else if(strcmpi(rom_info[i].name, SCATTER_LOGO_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_LOGO_PREFIX))
                       {
	                      //rom_path = scatter_file + "\\logo.bin";
	                      rom_path = temp_rom_file.c_str();

	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_Logo_path[0] = rom_path.c_str();
	                         b_Logo_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
	                      else
	                      {
    	                      noMemoryFlag  = true;
	                      }
	                   }
                  }
				  else if(strcmpi(rom_info[i].name, SCATTER_TZ_TEE1_NAME) == 0)
                  {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_TEE1_PREFIX))
                       {
	                      //rom_path = scatter_file + "\\logo.bin";
	                      rom_path = temp_rom_file.c_str();

	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_TEE1_path[0] = rom_path.c_str();
	                         b_TEE1_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
	                      else
	                      {
    	                      noMemoryFlag  = true;
	                      }
	                   }
				  }
				  else if(strcmpi(rom_info[i].name, SCATTER_TZ_TEE2_NAME) == 0)
                  {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_TEE2_PREFIX))
                       {
	                      //rom_path = scatter_file + "\\logo.bin";
	                      rom_path = temp_rom_file.c_str();

	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_TEE2_path[0] = rom_path.c_str();
	                         b_TEE2_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
	                      else
	                      {
    	                      noMemoryFlag  = true;
	                      }
	                   }
				  }
                  else if(strcmpi(rom_info[i].name, SCATTER_BOOTIMG_NAME) == 0)
                  {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_BOOTIMG_PREFIX))
                       {
	                          //rom_path = scatter_file + "\\boot.img";
	                          rom_path = temp_rom_file.c_str();

	                          if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                          {
	                              m_Boot_path[0] = rom_path.c_str();
	                              b_Boot_checked[0] = _TRUE;
	                              DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                          }
	                          else
	                          {
    	                          noMemoryFlag  = true;
	                          }
	                     }
                   }
                   else if(strcmpi(rom_info[i].name, SCATTER_SECRO_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_SECRO_PREFIX))
                       {
	                         //rom_path = scatter_file + "\\secro.img";
	                      rom_path = temp_rom_file.c_str();

	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_Secro_path[0] = rom_path.c_str();
	                         b_Secro_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
    					  else
    					  {
    						  noMemoryFlag  = true;
    					  }
	               }
                   }
                    else if(strcmpi(rom_info[i].name, SCATTER_RECOVERY_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_RECOVERY_PREFIX))
                        {
	                      //rom_path = scatter_file + "\\recovery.img";
	                      rom_path = temp_rom_file.c_str();

	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_Recovery_path[0] = rom_path.c_str();
	                         b_Recovery_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
    					  else
    					  {
    						  noMemoryFlag  = true;
    					  }
	               }
                   }
                   else if(strcmpi(rom_info[i].name, SCATTER_ANDROID_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_ANDROID_PREFIX))
                       {
	                      //rom_path = scatter_file + "\\system.img";
	                      rom_path = temp_rom_file.c_str();


	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_AndroidSystem_path[0] = rom_path.c_str();
	                         b_AndroidSystem_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
    					  else
    					  {
    						  noMemoryFlag  = true;
    					  }
                       }
                   }
                   else if(strcmpi(rom_info[i].name, SCATTER_USERDATA_NAME) == 0)
                   {
                       AnsiString temp_scat_path = scatter_file.c_str();
                       AnsiString temp_rom_file ;
                       if(search_image_files(temp_scat_path, temp_rom_file, AUTO_USERDATA_PREFIX))
                        {
	                      //rom_path = scatter_file + "\\userdata.img";
	                      rom_path = temp_rom_file.c_str();


	                      if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
	                      {
	                         m_UserData_path[0] = rom_path.c_str();
	                         b_UserData_checked[0] = _TRUE;
	                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
	                      }
	                      else
	                      {
	                      noMemoryFlag  = true;
	                      }
	                      }
                   }
                   //Cust packet
                   else
                   {
	                     //load cust image
	                     if(strcmpi(rom_info[i].name, SCATTER_CUSTPACK1_NAME) == 0)
	                     {
	                         AnsiString temp_scat_path = scatter_file.c_str();
	                         AnsiString temp_rom_file;
	                         if(search_image_files(temp_scat_path, temp_rom_file, AUTO_CUSTPACK1_PREFIX))
	                         {
		                         //rom_path = scatter_file + "\\" + CUSTPACK1_NAME;
		                         rom_path = temp_rom_file.c_str();
		                         if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
		                         {
			                         m_Custpack1_path[0] = rom_path.c_str();
			                         b_Custpack1_checked[0] = _TRUE;
			                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
		                         }
  	                         else
  	                         {
  	                         noMemoryFlag  = true;
  	                         }
	                          }
	                     }
	                     if(strcmpi(rom_info[i].name, SCATTER_CUSTPACK2_NAME) == 0)
	                     {
	                     AnsiString temp_scat_path = scatter_file.c_str();
	                     AnsiString temp_rom_file;
	                     if(search_image_files(temp_scat_path, temp_rom_file, AUTO_CUSTPACK2_PREFIX))
	                     {
		                         //rom_path = scatter_file + "\\" + CUSTPACK2_NAME;
		                         rom_path = temp_rom_file.c_str();
		                         if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
		                         {
			                         m_Custpack2_path[0] = rom_path.c_str();
			                         b_Custpack2_checked[0] = _TRUE;
			                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
		                         }
	                     else
	                     {
    	                     noMemoryFlag  = true;
	                     }
		                 }
	                }
	                if(strcmpi(rom_info[i].name, SCATTER_CUSTPACK3_NAME) == 0)
	                {
		                AnsiString temp_scat_path = scatter_file.c_str();
		                AnsiString temp_rom_file;
		                if(search_image_files(temp_scat_path, temp_rom_file, AUTO_CUSTPACK3_PREFIX))
		                {
		                         //rom_path = scatter_file + "\\" + CUSTPACK3_NAME;
		                         rom_path = temp_rom_file.c_str();
		                         if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
		                         {
			                         m_Custpack3_path[0] = rom_path.c_str();
			                         b_Custpack3_checked[0] = _TRUE;
			                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
		                         }
	                           else
	                           {
	                           noMemoryFlag  = true;
	                           }
		                 }
	                }
	                if(strcmpi(rom_info[i].name, SCATTER_CUSTPACK4_NAME) == 0)
	                {
		                AnsiString temp_scat_path = scatter_file.c_str();
		                AnsiString temp_rom_file;
		                if(search_image_files(temp_scat_path, temp_rom_file, AUTO_CUSTPACK4_PREFIX))
		                {
		                         //rom_path = scatter_file + "\\" + CUSTPACK4_NAME;
		                         rom_path = temp_rom_file.c_str();
		                         if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
		                         {
			                         m_Custpack4_path[0] = rom_path.c_str();
			                         b_Custpack4_checked[0] = _TRUE;
			                         DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
		                         }
                                         else
                                         {
                                                 noMemoryFlag  = true;
                                         }
		                 }
	                }
//eMMC added
					if(strcmpi(rom_info[i].name, SCATTER_DL_CACHE_NAME) == 0)
					{
						AnsiString temp_scat_path = scatter_file.c_str();
						AnsiString temp_rom_file;
						if(search_image_files(temp_scat_path, temp_rom_file, AUTO_CACHE_PREFIX))
						{
							//rom_path = scatter_file + "\\" + CACHE_NAME;
							rom_path = temp_rom_file.c_str();


							if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
							{
								m_cache_path[0] = rom_path.c_str();
								b_cache_checked[0] = _TRUE;
								DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
							}
							else
							{
								noMemoryFlag  = true;
							}
						}
	                }

					//if(strcmpi(rom_info[i].name, SCATTER_DL_FAT_NAME) == 0)
					// Sopport new scatter file,mtk71502.20130408
					if(strcmpi(rom_info[i].name, SCATTER_DL_FAT_NAME) == 0 || strcmpi(rom_info[i].name, SCATTER_FAT_NAME) == 0)
					{
					     //GC_TRACE("<MDLF> For FAT,is download is %d",rom_info[i].item_is_visable);
					     if(rom_info[i].item_is_visable)  // if FAT is download , load FAT img.
					     {
						  AnsiString temp_scat_path = scatter_file.c_str();
						  AnsiString temp_rom_file;
						  if(search_image_files(temp_scat_path, temp_rom_file, AUTO_FAT_PREFIX))
						  {
							//rom_path = scatter_file + "\\" + FAT_NAME;
							rom_path = temp_rom_file.c_str();


							if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
							{
								m_FAT_path[0] = rom_path.c_str();
								b_FAT_checked[0] = _TRUE;
								DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
							}
							else
							{
								noMemoryFlag  = true;
							}
						}
	                             }
			            }

					if(strcmpi(rom_info[i].name, SCATTER_DL_EBR1_NAME) == 0)
					{
						AnsiString temp_scat_path = scatter_file.c_str();
						AnsiString temp_rom_file;
						if(search_image_files(temp_scat_path, temp_rom_file, AUTO_EBR1_PREFIX))
						{
							//rom_path = scatter_file + "\\" + EBR1_NAME;
							rom_path = temp_rom_file.c_str();

							if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
							{
								m_EBR1_path[0] = rom_path.c_str();
								b_EBR1_checked[0] = _TRUE;
								DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
							}
							else
							{
								noMemoryFlag  = true;
							}
						}
	                }

					if(strcmpi(rom_info[i].name, SCATTER_DL_EBR2_NAME) == 0)
					{
						AnsiString temp_scat_path = scatter_file.c_str();
						AnsiString temp_rom_file;
						if(search_image_files(temp_scat_path, temp_rom_file, AUTO_EBR2_PREFIX))
						{
							//rom_path = scatter_file + "\\" + EBR2_NAME;
							rom_path = temp_rom_file.c_str();

							if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
							{
								m_EBR2_path[0] = rom_path.c_str();
								b_EBR2_checked[0] = _TRUE;
								DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
							}
							else
							{
								noMemoryFlag  = true;
							}
						}
	                }

					if(strcmpi(rom_info[i].name, SCATTER_DL_MBR_NAME) == 0)
					{
						AnsiString temp_scat_path = scatter_file.c_str();
						AnsiString temp_rom_file;
						if(search_image_files(temp_scat_path, temp_rom_file, AUTO_MBR_PREFIX))
						{
							//rom_path = scatter_file + "\\" + MBR_NAME;
							rom_path = temp_rom_file.c_str();


							if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
							{
								m_MBR_path[0] = rom_path.c_str();
								b_MBR_checked[0] = _TRUE;
								DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
							}
							else
							{
								noMemoryFlag  = true;
							}
						}
	                }

				  map<string,ImageDetailInfo*>::iterator it = imageMap->begin();

				  for(; it!= imageMap->end(); it++)
				  {
					  string imageScatterName =    it->first;
					  if(strcmpi(rom_info[i].name, imageScatterName.c_str()) == 0)
					  {
						    AnsiString temp_scat_path = scatter_file.c_str();
							AnsiString temp_rom_file;

							AnsiString imagePrefixName = it->second->GetImagePrefixName();
							if(search_image_files(temp_scat_path, temp_rom_file, imagePrefixName))
							{
								//rom_path = scatter_file + "\\" + MBR_NAME;
								rom_path = temp_rom_file.c_str();
								if(S_DONE ==  DL_Rom_Load(*p_dl_handle, i, rom_path.c_str()))
								{
									it->second->SetImagePath(rom_path.c_str());
									it->second->SetImageChecked(_TRUE);
									//m_MBR_path[0] = rom_path.c_str();
									//b_MBR_checked[0] = _TRUE;
									DL_Rom_SetEnableAttr( *p_dl_handle, i, _TRUE);
								}
								else
								{
									noMemoryFlag  = true;
								}
							}

					  }

				  } //end for

                   }
                }
		         //delete check_sum_ini_;
            }
        }
        if (noMemoryFlag)
        {
            string strErrorMessage = "[Warning]There is no enough memory in the PC to load the images!";
            Application->MessageBox(strErrorMessage.c_str(), "FAIL", MB_OK);
        }
    }
    catch(...)
    {
        return;
    }
}

bool TfrmMultiLoad::search_image_files(AnsiString folder, AnsiString & rom_file, AnsiString filetype)
{
    vector<string>  m_p_files;
    m_p_files.clear();
    doSearch(folder.c_str(), m_p_files, filetype.c_str());

    if(m_p_files.empty())
    {
      return false;
    }

    string *it = m_p_files.begin();
    rom_file = (*it).c_str();
    return true;
}

bool TfrmMultiLoad::doSearch(string folder, vector<string>  &m_p_files, string filetype)
{
     WIN32_FIND_DATA find_data;
    HANDLE hFind;
    bool bIsDirectory;
    string file_name, file_path, search_path;
    search_path = folder + "\\*.*";

    hFind = FindFirstFile(search_path.c_str(), &find_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
    file_name = find_data.cFileName;

    if(!bIsDirectory)
    {
     //  on_find(folder_path, file_name);

         char full_path[255];

         if(is_prefix_wanted(file_name, filetype))
        {
             sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

             m_p_files.push_back(full_path);
        }
    }
    /*
    else
    {
       if((".." != file_name) && ("." != file_name))
       {
          file_path = folder + "\\";
          file_path += find_data.cFileName;
          doSearch(file_path, m_p_files, filetype);
       }
    }
    */

    //For Find next file
    while(FindNextFile(hFind, &find_data))
    {
        bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
        file_name = find_data.cFileName;

        if(!bIsDirectory)
        {
          // on_find(folder_path, file_name);
            char full_path[255];

             if(is_prefix_wanted(file_name, filetype))
            {
                 sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

                 m_p_files.push_back(full_path);

             }
        }
        /*
        else
        {
           if((".." != file_name) && ("." != file_name))
           {
              file_path = folder + "\\";
              file_path += find_data.cFileName;
              doSearch(file_path, m_p_files, filetype);
           }
        }
        */
    }

    return true;
}

bool TfrmMultiLoad::is_prefix_wanted(string file, string filetype)
{

   //First condition -- uboot file's type is u-boot
    if(file.size() < filetype.size())
    {
       return false;
    }

    //Second condition -- begin with  scat
    if(strnicmp(file.c_str(), filetype.c_str(), filetype.size()) != 0)
    {
       return false;
    }

    return true;
}

/*
//---------------------------------------------------------------------------
bool TfrmMultiLoad::search_uboot_files(AnsiString folder,AnsiString & rom_file)
{

    vector<string>  m_p_files;
    m_p_files.clear();
    search(folder.c_str(),m_p_files);

    if(m_p_files.empty())
   {
      return false;
   }

   string *it = m_p_files.begin();
   rom_file = (*it).c_str();
    return true;

}

bool TfrmMultiLoad::search(string folder,vector<string>  &m_p_files)
{
     WIN32_FIND_DATA find_data;
    HANDLE hFind;
    bool bIsDirectory;
    string file_name, file_path, search_path;
    search_path = folder + "\\*.*";

    hFind = FindFirstFile(search_path.c_str(), &find_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
    file_name = find_data.cFileName;

    if(!bIsDirectory)
    {
     //  on_find(folder_path, file_name);

         char full_path[255];

         if(is_uboot_file(file_name))
        {
             sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

             m_p_files.push_back(full_path);
        }
    }
    else
    {
       if((".." != file_name) && ("." != file_name))
       {
          file_path = folder + "\\";
          file_path += find_data.cFileName;
          search(file_path,m_p_files);
       }
    }

    //For Find next file
    while(FindNextFile(hFind, &find_data))
    {
        bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
        file_name = find_data.cFileName;

        if(!bIsDirectory)
        {
          // on_find(folder_path, file_name);
            char full_path[255];

             if(is_uboot_file(file_name))
            {
                 sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

                 m_p_files.push_back(full_path);

             }
        }
        else
        {
           if((".." != file_name) && ("." != file_name))
           {
              file_path = folder + "\\";
              file_path += find_data.cFileName;
              search(file_path,m_p_files);
           }
        }
    }

    return true;
}


bool TfrmMultiLoad::is_uboot_file(string file)
{

   //First condition -- uboot file's type is u-boot
    if(file.size() < 6)
    {
       return false;
    }

    //Second condition -- begin with  scat
    if(file.compare(0, 6, "u-boot") != 0)
    {
       return false;
    }

    return true;
}

//---------------------------------------------------------------------------
bool TfrmMultiLoad::search_preloader_files(AnsiString folder,AnsiString & rom_file)
{

    vector<string>  m_p_files;
    m_p_files.clear();
    search_preloader(folder.c_str(),m_p_files);

    if(m_p_files.empty())
   {
      return false;
   }

   string *it = m_p_files.begin();
   rom_file = (*it).c_str();
    return true;

}

bool TfrmMultiLoad::search_preloader(string folder,vector<string>  &m_p_files)
{
     WIN32_FIND_DATA find_data;
    HANDLE hFind;
    bool bIsDirectory;
    string file_name, file_path, search_path;
    search_path = folder + "\\*.*";

    hFind = FindFirstFile(search_path.c_str(), &find_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
    file_name = find_data.cFileName;

    if(!bIsDirectory)
    {
     //  on_find(folder_path, file_name);

         char full_path[255];

         if(is_preloader_file(file_name))
        {
             sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

             m_p_files.push_back(full_path);
        }
    }
    else
    {
       if((".." != file_name) && ("." != file_name))
       {
          file_path = folder + "\\";
          file_path += find_data.cFileName;
          search_preloader(file_path,m_p_files);
       }
    }

    //For Find next file
    while(FindNextFile(hFind, &find_data))
    {
        bIsDirectory = ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
        file_name = find_data.cFileName;

        if(!bIsDirectory)
        {
          // on_find(folder_path, file_name);
            char full_path[255];

             if(is_preloader_file(file_name))
            {
                 sprintf(full_path, "%s\\%s", folder.c_str(), file_name.c_str());

                 m_p_files.push_back(full_path);

             }
        }
        else
        {
           if((".." != file_name) && ("." != file_name))
           {
              file_path = folder + "\\";
              file_path += find_data.cFileName;
              search_preloader(file_path,m_p_files);
           }
        }
    }

    return true;
}


bool TfrmMultiLoad::is_preloader_file(string file)
{

   //First condition -- preloader file's type is preloader
    if(file.size() < 9)
    {
       return false;
    }

    //Second condition -- begin with  scat
    if(file.compare(0, 9, "preloader") != 0)
    {
       return false;
    }

    return true;
}
*/

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::RedrawLVList( int idx, DL_HANDLE_T *p_dl_handle )
{
    unsigned short rom_count;

    try
    {
      TListItem	*pLI;
      m_lv[idx]->Items->Clear();
      //S_DN_PARAM_T* p_dn_param;

      for( int i=0; i<MAX_SUPPORT_LOAD_NUM; i++ )
      {
          m_asROMFileName[i] = "";
          m_asFATFileName[i] = "";
          m_xldr_path[i] = "";
          m_eboot_path[i] = "";

          m_preloader_path[i] = "";
          m_uboot_path[i] = "";
          m_Logo_path[i] = "";
		  m_TEE1_path[i] = "";
		  m_TEE2_path[i] = "";
          m_Boot_path[i] = "";
          m_Secro_path[i] = "";
          m_Recovery_path[i] = "";
          m_AndroidSystem_path[i] = "";
          m_UserData_path[i] = "";

          m_Custpack1_path[i] = "";
          m_Custpack2_path[i] = "";
          m_Custpack3_path[i] = "";
          m_Custpack4_path[i] = "";

		  m_cache_path[i] = "";
		  m_FAT_path[i] = "";

      }

     // p_dn_param = MainForm->Get_DNParameter();

      if(!DL_GetCount(*p_dl_handle, &rom_count))
      {
          ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
          if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS))
          {
              // access
              unsigned int total_file_size = 0;
              for (int i=0; i<rom_count; i++)
              {
                  AnsiString  as_reg_prefix;
                  TListItem	*p_list_item;

                  // access
                  total_file_size += rom_info[i].filesize;

                  {
	                  if(_TRUE == rom_info[i].item_is_visable)
                      {
	                      p_list_item = m_lv[idx]->Items->Add();
	                      p_list_item->Caption = rom_info[i].name;
                      }
                      else
                      {
	                      continue;
                      }

                      if (strcmpi(rom_info[i].name, "BOOTLOADER") == 0)
                      {
                          m_asROMFileName[idx] = rom_info[i].filepath;
                      }
                      else if (strcmpi(rom_info[i].name, "ROM") == 0)
                      {
                          m_asROMFileName[idx] = rom_info[i].filepath;
                      }
#if 0
                      else if (strcmpi(rom_info[i].name, "FAT") == 0)
                      {
                          m_iLV_FAT_index[idx] = i;
                          m_asFATFileName[idx] = rom_info[i].filepath;
                      }
#endif
                      else if (strcmpi(rom_info[i].name, "xldr") == 0)
                      {
                         if(m_xldr_ini_path[idx] != "")
                         {
                             if(Load_Rom(p_dl_handle, i, m_xldr_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_xlder_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_xlder_checked[idx] = rom_info[i].enable;
                             m_xldr_ini_path[idx] = "";
                         }

                         m_xldr_path[idx] = rom_info[i].filepath;
						 if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, "eboot") == 0)
                      {
                        if(m_eboot_ini_path[idx] != "")
                        {
                           if(Load_Rom(p_dl_handle, i, m_eboot_ini_path[idx] ))
                           {
                               DL_Rom_SetEnableAttr( *p_dl_handle, i, b_eboot_checked[idx]);
                               DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                           }

                           b_eboot_checked[idx] = rom_info[i].enable;
                           m_eboot_ini_path[idx] = "";
                        }

                        m_eboot_path[idx] = rom_info[i].filepath;

                        _bootloader_type = BT_EBOOT;
						if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom_info[i].name, SCATTER_LK_NAME) == 0)
                      {

                        if(m_uboot_ini_path[idx] != "")
                        {
                           if(Load_Rom(p_dl_handle, i, m_uboot_ini_path[idx] ))
                           {
                               DL_Rom_SetEnableAttr( *p_dl_handle, i, b_uboot_checked[idx]);
                               DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                           }

                           b_uboot_checked[idx] = rom_info[i].enable;
                           m_uboot_ini_path[idx] = "";
                        }

                         m_uboot_path[idx] = rom_info[i].filepath;
                         _bootloader_type = BT_UBOOT;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_PRELOADER_NAME) == 0)
                      {
                          if(m_preloader_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_preloader_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_preloader_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_preloader_checked[idx] = rom_info[i].enable;
                             m_preloader_ini_path[idx] = "";
                          }

                          m_preloader_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_DSPBL_NAME) == 0)
                      {
                          if(m_DSPBL_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_DSPBL_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_DSPBL_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_DSPBL_checked[idx] = rom_info[i].enable;
                             m_DSPBL_ini_path[idx] = "";
                          }

                          m_DSPBL_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_LOGO_NAME) == 0)
                      {
                          if(m_Logo_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Logo_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Logo_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Logo_checked[idx] = rom_info[i].enable;
                             m_Logo_ini_path[idx] = "";
                          }

                          m_Logo_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
					  else if (strcmpi(rom_info[i].name, SCATTER_TZ_TEE1_NAME) == 0)
                      {
                          if(m_TEE1_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_TEE1_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_TEE1_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_TEE1_checked[idx] = rom_info[i].enable;
                             m_TEE1_ini_path[idx] = "";
                          }

                          m_TEE1_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
					   else if (strcmpi(rom_info[i].name, SCATTER_TZ_TEE2_NAME) == 0)
                      {
                          if(m_TEE2_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_TEE2_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_TEE2_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_TEE2_checked[idx] = rom_info[i].enable;
                             m_TEE2_ini_path[idx] = "";
                          }

                          m_TEE2_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_BOOTIMG_NAME) == 0)
                      {
                          if(m_Boot_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Boot_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Boot_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Boot_checked[idx] = rom_info[i].enable;
                             m_Boot_ini_path[idx] = "";
                          }

                          m_Boot_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_SECRO_NAME) == 0)
                      {
                          if(m_Secro_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Secro_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Secro_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Secro_checked[idx] = rom_info[i].enable;
                             m_Secro_ini_path[idx] = "";
                          }

                          m_Secro_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_RECOVERY_NAME) == 0)
                      {
                          if(m_Recovery_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Recovery_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Recovery_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Recovery_checked[idx] = rom_info[i].enable;
                             m_Recovery_ini_path[idx] = "";
                          }

                          m_Recovery_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_ANDROID_NAME) == 0)
                      {
                          if(m_AndroidSystem_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_AndroidSystem_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_AndroidSystem_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_AndroidSystem_checked[idx] = rom_info[i].enable;
                             m_AndroidSystem_ini_path[idx] = "";
                          }

                          m_AndroidSystem_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_USERDATA_NAME) == 0)
                      {
                          if(m_UserData_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_UserData_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_UserData_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_UserData_checked[idx] = rom_info[i].enable;
                             m_UserData_ini_path[idx] = "";

                          }

                          m_UserData_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }

                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_CUSTPACK1_NAME) == 0)
                      {
                          if(m_Custpack1_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Custpack1_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Custpack1_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Custpack1_checked[idx] = rom_info[i].enable;
                             m_Custpack1_ini_path[idx] = "";
                          }

                          m_Custpack1_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_CUSTPACK2_NAME) == 0)
                      {
                          if(m_Custpack2_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Custpack2_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Custpack2_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Custpack2_checked[idx] = rom_info[i].enable;
                             m_Custpack2_ini_path[idx] = "";
                          }

                          m_Custpack2_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_CUSTPACK3_NAME) == 0)
                      {
                          if(m_Custpack3_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Custpack3_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Custpack3_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Custpack3_checked[idx] = rom_info[i].enable;
                             m_Custpack3_ini_path[idx] = "";
                          }

                          m_Custpack3_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
                      else if (strcmpi(rom_info[i].name, SCATTER_CUSTPACK4_NAME) == 0)
                      {
                          if(m_Custpack4_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_Custpack4_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_Custpack4_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_Custpack4_checked[idx] = rom_info[i].enable;
                             m_Custpack4_ini_path[idx] = "";
                          }

                          m_Custpack4_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
                      }
					else if(strcmpi(rom_info[i].name, SCATTER_DL_CACHE_NAME) == 0)//eMMC added
					{
                          if(m_cache_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_cache_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_cache_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_cache_checked[idx] = rom_info[i].enable;
                             m_cache_ini_path[idx] = "";
                          }

                          m_cache_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
	                }
					else if(strcmpi(rom_info[i].name, SCATTER_DL_FAT_NAME) == 0)//eMMC added
					{
                          if(m_FAT_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_FAT_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_FAT_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_FAT_checked[idx] = rom_info[i].enable;
                             m_FAT_ini_path[idx] = "";
                          }

                          m_FAT_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
	                }
					else if(strcmpi(rom_info[i].name, SCATTER_DL_EBR1_NAME) == 0)//eMMC added
					{
                          if(m_EBR1_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_EBR1_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_EBR1_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_EBR1_checked[idx] = rom_info[i].enable;
                             m_EBR1_ini_path[idx] = "";
                          }

                          m_EBR1_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
	                }
					else if(strcmpi(rom_info[i].name, SCATTER_DL_EBR2_NAME) == 0)//eMMC added
					{
                          if(m_EBR2_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_EBR2_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_EBR2_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_EBR2_checked[idx] = rom_info[i].enable;
                             m_EBR2_ini_path[idx] = "";
                          }

                          m_EBR2_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
	                }
					else if(strcmpi(rom_info[i].name, SCATTER_DL_MBR_NAME) == 0)//eMMC added
					{
                          if(m_MBR_ini_path[idx] != "")
                          {
                             if(Load_Rom(p_dl_handle, i, m_MBR_ini_path[idx] ))
                             {
                                DL_Rom_SetEnableAttr( *p_dl_handle, i, b_MBR_checked[idx]);
                                DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
                             }

                             b_MBR_checked[idx] = rom_info[i].enable;
                             m_MBR_ini_path[idx] = "";
                          }

                          m_MBR_path[idx] = rom_info[i].filepath;
						  if (rom_info[i].end_addr > m_Load_End_Address)
						  {
							  m_Load_End_Address = rom_info[i].end_addr;
						  }
	                }
					else
					{
						map<string,ImageDetailInfo*>::iterator it = imageMap->begin();

						for(; it!= imageMap->end(); it++)
						{

							string imageScatterName = it->first;
							if(strcmpi(rom_info[i].name, imageScatterName.c_str()) == 0)
							{
								AnsiString imageIniPath = it->second->GetImageIniPath();
								_BOOL imageChecked = it->second->GetImageChecked();
								if(imageIniPath != "")
								{
									if(Load_Rom(p_dl_handle, i, imageIniPath))
									{
										DL_Rom_SetEnableAttr( *p_dl_handle, i, imageChecked);
										DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS);
									}
									it->second->SetImageChecked(rom_info[i].enable);
									it->second->SetImageIniPath("");
								}
								it->second->SetImagePath(rom_info[i].filepath);

								if (rom_info[i].end_addr > m_Load_End_Address)
								{
									m_Load_End_Address = rom_info[i].end_addr;
								}

							}

						} // end for
					}


                      p_list_item->Checked = rom_info[i].enable;

                      if (ABSOLUTE_ADDR == rom_info[i].addr_type)
                      {
                          as_reg_prefix = "";
                      }
                      else
                      {
                          as_reg_prefix = "+";
                      }

                      if(_TRUE == rom_info[i].item_is_visable)
                      {
	                      p_list_item->SubItems->Add(as_reg_prefix + "0x" + IntToHex((int)rom_info[i].region_addr, 8));
	                      p_list_item->SubItems->Add("0x" + IntToHex((int)rom_info[i].begin_addr, 8));
	                      p_list_item->SubItems->Add("0x" + IntToHex((int)rom_info[i].end_addr, 8));
	                      p_list_item->SubItems->Add(rom_info[i].filepath);
                      }
                  }
              } // for
              MainForm->Set_TotalFileSize(total_file_size);
          }
      }
      m_lv[idx]->Enabled=b_SCATTER_checkEnable;
      MainForm->ScatterEnable=b_SCATTER_checkEnable;
      // add md5 ,20130413.
      MainForm->Checksum_ver=b_checksum_ver;

      MainForm->ScatterLoadCompleteFlag=FALSE;
      if(!DL_GetCount(*p_dl_handle, &rom_count))
      {
          ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
          if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, MAX_LOAD_SECTIONS))
          {
             for (int i=0; i<rom_count; i++)
              {
                if(_TRUE == rom_info[i].item_is_visable)
                      {
	                      if(rom_info[i].enable !=_TRUE)
	                      	{
	                      	    MainForm-> ScatterLoadCompleteFlag=TRUE;
	                         	break;
	                      	}
                      }
             	}
          }
      	}
    }
    catch(...)
    {
        return;
    }

}

void __fastcall TfrmMultiLoad::Redraw_LV_SEC_RO_ParamList(int idx, TListItem *pLI)
{
    AnsiString as;
    pLI = m_lvPar[idx]->Items->Add();
    pLI->Caption = "Secure RO Data";
    pLI->Checked = m_sParameter[idx].m_sec_ro_enable?true:false;
    as.printf("len(%lu): %s", m_sParameter[idx].m_sec_ro.m_len, m_sParameter[idx].m_sec_ro_filepath.c_str());
    pLI->SubItems->Add(as);
    pLI->Data = &g_LV_ItemType[LV_SEC_RO];
}

void __fastcall TfrmMultiLoad::Redraw_GPS_ROM(int idx, TListItem *pLI)
{
  AnsiString as;

  // add GPS ROM
  if (m_sParameter[idx].m_gps_download)
  {
      pLI = m_lvPar[idx]->Items->Add();
      pLI->Caption = "GPS ROM";
      pLI->Checked = m_sParameter[idx].m_gps_enable ? _TRUE : _FALSE;
      as.printf("len(%lu): %s", m_sParameter[idx].m_gps_rom_filesize, m_sParameter[idx].m_gps_rom_filepath.c_str());
      pLI->SubItems->Add(as);
      pLI->Data = &g_LV_ItemType[LV_GPS_ROM];
  }
  else
  {
      m_sParameter[idx].m_gps_enable = _FALSE;
  }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::RedrawLVParamList(int idx, DL_HANDLE_T *p_dl_handle)
{
    // update parameter list view
    TListItem	*pLI;
    DL_INFO	 dl_info;

    m_lvPar[idx]->Items->Clear();
    DL_GetInfo(*p_dl_handle, &dl_info);

    // add SEC_RO m_parameters
    if(dl_info.m_config.m_security_supported)
    {
        Redraw_LV_SEC_RO_ParamList(idx, pLI);
    }

    // add GPS ROM
    Redraw_GPS_ROM(idx, pLI);

    // disable parameter list view ,because sp did not support now
    m_lvPar[idx]->Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::RedrawLVCheckBox( int idx )
{
    unsigned short rom_count;
    //int	rom_begin_index;
    // S_DN_PARAM_T* p_dn_param = MainForm->Get_DNParameter();
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);

        //rom_begin_index = 0;


    if(!DL_GetCount(*p_dl_handle, &rom_count))
    {
        ROM_INFO	rom[MAX_LOAD_SECTIONS];
        //int		checked_count=0;

        if(!DL_Rom_GetInfoAll(*p_dl_handle, rom, MAX_LOAD_SECTIONS))
        {
            for(int i=0; i<rom_count; i++)
            {
                if (strcmpi(rom[i].name, "xldr") == 0)
                {
                    b_xlder_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, "eboot") == 0)
                {
                    b_eboot_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_PRELOADER_NAME) == 0)
                {
                    b_preloader_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_DSPBL_NAME) == 0)
                {
                    b_DSPBL_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom[i].name, SCATTER_LK_NAME) == 0)
                {
                    b_uboot_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_LOGO_NAME) == 0)
                {
                    b_Logo_checked[idx] = rom[i].enable;
	            }
				else if (strcmpi(rom[i].name, SCATTER_TZ_TEE1_NAME) == 0)
                {
                    b_TEE1_checked[idx] = rom[i].enable;
	            }
				else if (strcmpi(rom[i].name, SCATTER_TZ_TEE2_NAME) == 0)
                {
                    b_TEE2_checked[idx] = rom[i].enable;
	            }
                else if (strcmpi(rom[i].name, SCATTER_BOOTIMG_NAME) == 0)
                {
                    b_Boot_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_SECRO_NAME) == 0)
                {
                    b_Secro_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_RECOVERY_NAME) == 0)
                {
                    b_Recovery_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_ANDROID_NAME) == 0)
                {
                    b_AndroidSystem_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_USERDATA_NAME) == 0)
                {
                    b_UserData_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_CUSTPACK1_NAME) == 0)
                {
                    b_Custpack1_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_CUSTPACK2_NAME) == 0)
                {
                    b_Custpack2_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_CUSTPACK3_NAME) == 0)
                {
                    b_Custpack3_checked[idx] = rom[i].enable;
                }
                else if (strcmpi(rom[i].name, SCATTER_CUSTPACK4_NAME) == 0)
                {
                    b_Custpack4_checked[idx] = rom[i].enable;
                }
				else if (strcmpi(rom[i].name, SCATTER_DL_CACHE_NAME) == 0)//eMMC added
                {
                    b_cache_checked[idx] = rom[i].enable;
                }
				else if (strcmpi(rom[i].name, SCATTER_DL_FAT_NAME) == 0)//eMMC added
                {
                    b_FAT_checked[idx] = rom[i].enable;
                }
				else if (strcmpi(rom[i].name, SCATTER_DL_EBR1_NAME) == 0)//eMMC added
                {
                    b_EBR1_checked[idx] = rom[i].enable;
                }
				else if (strcmpi(rom[i].name, SCATTER_DL_EBR2_NAME) == 0)//eMMC added
                {
                    b_EBR2_checked[idx] = rom[i].enable;
                }
				else if (strcmpi(rom[i].name, SCATTER_DL_MBR_NAME) == 0)//eMMC added
                {
                    b_MBR_checked[idx] = rom[i].enable;
                }
				else
				{
					map<string,ImageDetailInfo*>::iterator it = imageMap->begin();


					for(; it!= imageMap->end(); it++)
					{
						if(strcmpi(rom[i].name, it->first.c_str()) == 0)
						{
							it->second->SetImageChecked(rom[i].enable);

						}
					}

				}
        }
    }

    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::lvChange(TObject *Sender, TListItem *Item,
      TItemChange Change)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);
    if( (Change == ctState) && (Item->Checked != m_slvChanged[plv->Tag].bPreCheckBoxState) )
    {
        m_slvChanged[plv->Tag].iChangedIndex = Item->Index;
        return;
    }

    /*
    enum TItemChange {
        ctText,         //A change to the Caption property of the list item.
        ctImage,     //A change to the list item's ImageIndex property or the list view's image list.
        ctState       //A change to the list item's
    };
    */

    m_slvChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::lvChanging(TObject *Sender, TListItem *Item,
      TItemChange Change, bool &AllowChange)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);
    m_slvChanged[plv->Tag].bPreCheckBoxState = Item->Checked;
    m_slvChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::lvClick(TObject *Sender)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);

    //S_DN_PARAM_T* p_dn_param = MainForm->Get_DNParameter();
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle((int) plv->Tag);


    //For only one listview existed, m_slvChanged[plv->Tag] == m_slvChanged[0]
    if( 0 <=m_slvChanged[plv->Tag].iChangedIndex && m_slvChanged[plv->Tag].iChangedIndex < plv->Items->Count )
    {
        TListItem *pItem=plv->Items->Item[m_slvChanged[plv->Tag].iChangedIndex];
        int itemIndex = GetROMIndex(pItem->Caption.c_str(), *p_dl_handle);
        if ( -1 == itemIndex )
        {
            ShowMessage("Obtaining rom index fails.");
            return;
        }
        DL_Rom_SetEnableAttr( *p_dl_handle, itemIndex, plv->Items->Item[m_slvChanged[plv->Tag].iChangedIndex]->Checked?_TRUE:_FALSE );
        RedrawLVCheckBox( (int) plv->Tag );
    }
    else
    {
        //lv_oldClick(Sender);
        Listview_ReplaceFile(Sender);
    }

    m_slvChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}

int TfrmMultiLoad::GetROMIndex(const char* rom_name, const DL_HANDLE_T dl_handle)
{
    int itemIndex = -1;

    unsigned short	count=0;
    ROM_INFO romInfo;
    DL_GetCount( dl_handle, &count );

    for (unsigned short i = 0; i < count; i++)
    {
        DL_Rom_GetInfo( dl_handle, i, &romInfo);
        if( 0 == strcmp(romInfo.name, rom_name) )
        {
            itemIndex = romInfo.index;
            break;
        }
    }
    return itemIndex;
}

bool  TfrmMultiLoad::Load_Rom(DL_HANDLE_T * dl_handle, int index, AnsiString rom_path)
{
    int ret=DL_Rom_Load(*dl_handle, index, rom_path.c_str());

    if (ret)
    {
        if( S_FTHND_HANDLE_BUSY_NOW == ret )
        {
            Application->MessageBox( "Program is busy! Can not update setting right now.", "Fail", MB_OK );
            return true;
        }
        else if( S_DL_LOAD_REGION_IS_OVERLAP == ret )
        {
            AnsiString as_msg = "\"" + rom_path+ "\" is too large and causes region overlap!";
            Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
            return false;
        }
        else
        {
            AnsiString as_msg = " Load file \"" + rom_path+ "\" fail!";
            Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
            return false;
        }
    }

    return true;
}

void TfrmMultiLoad::initial_dlg_open_dir(int index, AnsiString dir)
{
    m_dlgOpenROM[index]->InitialDir = dir;
    m_dlgOpenROM[index]->FileName = "";
}

//---------------------------------------------------------------------------
/*
void __fastcall TfrmMultiLoad::lv_oldClick(TObject *Sender)
{
    TListView *p_lv = dynamic_cast<TListView *>(Sender);
    int ret;
    int idx=p_lv->Tag;
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);

    if( NULL == p_lv->Selected )
       return;

    //Get current image path in the subitems
    if( m_asROMPath[idx].Length()>0 )
    {
        initial_dlg_open_dir(idx, m_asROMPath[idx]);
    }
    else if( 4 <= p_lv->Selected->SubItems->Count )
    {
        int iLD = p_lv->Selected->SubItems->Strings[3].LastDelimiter("\\:");
        if ( iLD > 0)
        {
          initial_dlg_open_dir(idx, p_lv->Selected->SubItems->Strings[3].SubString(1, iLD-1 ));
        }
    }

    //Open file dialog and position to this path
    if( m_dlgOpenROM[idx]->Execute( ))
    {
        DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
        S_DN_PARAM_T* p_dn_param = MainForm->Get_DNParameter();

        //Reload the image
        if(!Load_Rom(p_dl_handle, p_lv->ItemIndex, m_dlgOpenROM[idx]->FileName))
        {
            return;
        }

        int iLD = m_dlgOpenROM[idx]->FileName.LastDelimiter("\\:");
        if ( iLD > 0)
        {
            m_asROMPath[idx] = m_dlgOpenROM[idx]->FileName.SubString(1, iLD-1 );
        }

        // auto load download bin files
        if(MainForm->Get_AutoLoadResByScatFlag())
        {
           ret = DL_AutoLoadByScatRegionName(*p_dl_handle, m_dlgOpenROM[idx]->FileName.c_str(), NORMAL_ROM|RESOURCE_BIN|JUMPTABLE_BIN, _FALSE);

           if( ret != 0 )
           {
               Application->MessageBox( "Automatic load by scatter region name fail", "FAIL", MB_OK );
               return;
           }
        }
   }

   RedrawLVList( (int) p_lv->Tag, p_dl_handle );

   return;
}
*/


//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::Listview_ReplaceFile(TObject *Sender)
{
    TListView * p_lv = dynamic_cast<TListView *>(Sender);
    TListItem * pItem = p_lv->Selected;
    int ret;
    int idx=p_lv->Tag;
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);

    if( NULL == p_lv->Selected )
    {
        return;
    }

    //Get current image path in the subitems
    if( Listview_Subitem_Num <= pItem->SubItems->Count )
    {
        int iLD = pItem->SubItems->Strings[3].LastDelimiter("\\:");
        if ( iLD > 0)
        {
            m_dlgOpenROM[idx]->InitialDir = pItem->SubItems->Strings[3].SubString(1, iLD-1 );
            m_dlgOpenROM[idx]->FileName = "";
        }
    }

    //Open file dialog and position to this path
    if( m_dlgOpenROM[idx]->Execute( ))
    {
        DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
        //S_DN_PARAM_T* p_dn_param = MainForm->Get_DNParameter();

        //Find the actual image index
        int itemIndex = GetROMIndex(pItem->Caption.c_str(), *p_dl_handle);
        if ( -1 == itemIndex )
        {
            ShowMessage("Obtaining rom index fails.");
            return;
        }

        //Reload the image
        if(!Load_Rom(p_dl_handle, itemIndex, m_dlgOpenROM[idx]->FileName))
        {
            return;
        }
    }
    AnsiString ScatterfilePath;
    ScatterfilePath= m_INIFileH.read_RomInfo(INI_FILE_NAME, Application->ExeName,"ScatterFile", "Scatter_File_Path");
    UpdateRawLVListAndSaveRomInfo(p_dl_handle,ScatterfilePath);
    //RedrawLVList( (int) p_lv->Tag, p_dl_handle );

}



//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::btnDelLoadClick(TObject *Sender)
{
    TButton *btn = (TButton*)Sender;
    DelLoad(btn->Tag);
}

void __fastcall TfrmMultiLoad::DelLoad(int idx)
{
    int ret;

    DL_HANDLE_LIST_T *p_dl_handle_list = MainForm->Get_DlHandleList();
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);

    SetScatterFileCaption(idx, "");

    //m_asScatFileName[idx] = "";

    m_lv[idx]->Clear();
    m_lvPar[idx]->Clear();

    ret = DL_CheckHandleInList( *p_dl_handle_list, *p_dl_handle );

    if( ret != 0 )
    {
        DL_DeleteHandleFromList( *p_dl_handle_list, *p_dl_handle );
    }
}

//---------------------------------------------------------------------------
bool TfrmMultiLoad::Destroy_DLHandle(DL_HANDLE_T *p_dl_handle)
{
    if( NULL != p_dl_handle )
    {
        int ret;
        ret = DL_Rom_UnloadAll( *p_dl_handle );
        if( ret != 0 )
        {
            Application->MessageBox( "Unload all ROM fail", "FAIL", MB_OK );
            return false;
        }

        ret = DL_Destroy( p_dl_handle );
        if( ret != 0 )
        {
            Application->MessageBox( "Unload all ROM fail", "FAIL", MB_OK );
            return false;
        }

        delete p_dl_handle;
    }

    return true;
}

void __fastcall TfrmMultiLoad::lvParamChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);

    if( (Change == ctState) && (Item->Checked != m_sLvParamChanged[plv->Tag].bPreCheckBoxState) )
    {
        m_sLvParamChanged[plv->Tag].iChangedIndex = Item->Index;
        return;
    }

    m_sLvParamChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMultiLoad::lvParamChanging(TObject *Sender,
      TListItem *Item, TItemChange Change, bool &AllowChange)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);

    m_sLvParamChanged[plv->Tag].bPreCheckBoxState = Item->Checked;
    m_sLvParamChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMultiLoad::lvParamClick(TObject *Sender)
{
    TListView *plv = dynamic_cast<TListView *>(Sender);

    if( 0<=m_sLvParamChanged[plv->Tag].iChangedIndex && m_sLvParamChanged[plv->Tag].iChangedIndex < plv->Items->Count )
    {
        TListItem *pItem = plv->Items->Item[m_sLvParamChanged[plv->Tag].iChangedIndex];
	    E_LISTVIEW_ITEM_TYPE	type=*((E_LISTVIEW_ITEM_TYPE *)pItem->Data);

        switch(type)
        {
            case LV_OTP:
            {
                m_sParameter[plv->Tag].m_otp_enable = pItem->Checked ?_TRUE : _FALSE;
            }
            break;

            case LV_FTCFG:
            {
                m_sParameter[plv->Tag].m_ft_cfg_enable = pItem->Checked ? _TRUE : _FALSE;
            }
            break;

            case LV_SEC_RO:
            {
		        m_sParameter[plv->Tag].m_sec_ro_enable = pItem->Checked ? _TRUE : _FALSE;
            }
            break;

            case LV_CUST_PARA:
            {
                m_sParameter[plv->Tag].m_cust_para_enable = pItem->Checked ? _TRUE : _FALSE;
            }
            break;

            case LV_GPS_ROM:
            {
                m_sParameter[plv->Tag].m_gps_enable = pItem->Checked ? _TRUE : _FALSE;
            }
            break;

            default:
            {
            }
            break;
        }
    }
    else if( NULL != plv->Selected )
    {
        TListItem *pItem=plv->Selected;
        E_LISTVIEW_ITEM_TYPE	type=*((E_LISTVIEW_ITEM_TYPE *)pItem->Data);

        switch(type)
        {
            case LV_SEC_RO:
            {
                LoadSecRO(plv->Tag);
            }
            break;

            case LV_GPS_ROM:
            {
                LoadGPS(plv->Tag);
            }
            break;

            default:
            break;
        }
    }

    m_sLvParamChanged[plv->Tag].iChangedIndex = NO_CHANGE;
}

//---------------------------------------------------------------------------
bool __fastcall TfrmMultiLoad::LoadFileInfo(const char *pathname, unsigned int *p_nFileLength, long *p_last_modify_time)
{
    FILE *fp;
    unsigned char *buf;
    unsigned int file_len;
    struct _stat  file_stat={0};

    if ((NULL == pathname)    ||
        ('\0' == pathname[0]) ||
        (NULL == p_nFileLength)
       )
    {
        return false;
    }

    fp = fopen(pathname, "rb");
    if (NULL == fp)
    {
        return false;
    }

    if (_fstat(fileno(fp), &file_stat))
    {
        return false;
    }

    if (0 >= (file_len=filelength(fileno(fp))))
    {
        return false;
    }

    *p_nFileLength = calc_buf_len_by_file_len(file_len);

    if (NULL != p_last_modify_time)
    {
        *p_last_modify_time = file_stat.st_mtime;
    }

    return true;
}

unsigned int TfrmMultiLoad::calc_buf_len_by_file_len( unsigned int file_len)
{
    unsigned int buf_len;

    if( 0 != (file_len%2) )
    {
        buf_len = file_len+1;
    }
    else
    {
        buf_len = file_len;
    }

    return buf_len;
}

//-------------------------------------------------------------------------
bool __fastcall TfrmMultiLoad::LoadFile(const char *filename, unsigned char **pp_buf, unsigned int *p_nFileLength, long *p_last_modify_time)
{
    FILE *fp;
    unsigned char *buf;
    unsigned int buf_len;
    unsigned int file_len ;
    struct _stat  file_stat={0};

    if( NULL == filename || '\0' == filename[0] || NULL == pp_buf || NULL == p_nFileLength )
    {
        return false;
    }

    fp = fopen(filename, "rb");

    if( NULL == fp )
    {
        return false;
    }

    if( _fstat(fileno(fp), &file_stat) )
    {
        return false;
    }

    if( 0 >= (file_len=filelength(fileno(fp))) )
    {
        return false;
    }

    buf_len = calc_buf_len_by_file_len(file_len);

    if( NULL == (buf=(unsigned char *)malloc(buf_len)) )
    {
        return false;
    }

    if( file_len > fread(buf, 1, file_len, fp) )
    {
        free(buf);
        return false;
    }

    if( NULL != *pp_buf )
    {
        free(*pp_buf);
    }

    *pp_buf = buf;
    *p_nFileLength = buf_len;
    if( NULL != p_last_modify_time )
    {
        *p_last_modify_time = file_stat.st_mtime;
    }

    fclose(fp);
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::LoadSecRO(int idx)
{
    AnsiString	as;
    if (dlgOpenSecRo->Execute())
    {
        if (!LoadFile(dlgOpenSecRo->FileName.c_str(), &m_sParameter[idx].m_sec_ro.m_data, &m_sParameter[idx].m_sec_ro.m_len, NULL))
        {
            as.printf("Fail to load SEC_RO file \"%s\" !", dlgOpenSecRo->FileName.c_str());
            ShowMessage(as);
            return;
        }
    }

    m_sParameter[idx].m_sec_ro_filepath = dlgOpenSecRo->FileName;
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
    RedrawLVParamList(idx, p_dl_handle);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMultiLoad::LoadGPS(int idx)
{
    if (m_dlgOpenGPS->Execute())
    {
        AnsiString selectDir;
        AnsiString selectIni;

        // load GPS firmware
        if (!LoadFileInfo(m_dlgOpenGPS->FileName.c_str(), &m_sParameter[idx].m_gps_rom_filesize, NULL))
        {
            AnsiString as;
            as.printf("Fail to load GPS ROM \"%s\" !", m_dlgOpenGPS->FileName.c_str());
            ShowMessage(as);
            return;
        }

        // parse the gps.ini GPIO setting
        selectDir = ::ExtractFilePath(m_dlgOpenGPS->FileName);
        selectIni = selectDir + "gps.ini";

        if (!::FileExists(selectIni))
        {
            AnsiString as;
            as.printf("Fail to load GPS FW's INI File \"%s\" !", selectIni.c_str());
            ShowMessage(as);
            return;
        }
    }
    else
    {
        return;
    }

    // Get the GPS GPIO information from gps.ini
    m_sParameter[idx].m_gps_rom_filepath   = m_dlgOpenGPS->FileName;
    m_sParameter[idx].m_gps_rom_dirpath    = ::ExtractFilePath(m_sParameter[idx].m_gps_rom_filepath);
    m_sParameter[idx].m_gps_ini_filepath   = m_sParameter[idx].m_gps_rom_dirpath + "gps.ini";

    if(!read_GPS_gpio_information_from_GPS_ini(idx, m_sParameter[idx].m_gps_ini_filepath))
    {
        Application->MessageBox("Load file information fail", "FAIL", MB_OK);
    }

    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(idx);
    RedrawLVParamList(idx, p_dl_handle);
}

//===========================================================================
////////////////////////        share information     ///////////////////////
//===========================================================================
S_PARAMETER_DOWNLOAD_CONFIG TfrmMultiLoad::Get_Parameter(void)
{
    return m_sParameter[0];
}

void TfrmMultiLoad::SetScatterFileCaption(int idx, AnsiString  text)
{
    //TODO: Add your source code here
    m_lblScatFileName[idx]->Caption = text;
    MainForm->m_stScatFileName[idx]->Caption = text;
}

bool TfrmMultiLoad::IsHostBufferCksmFileExisted()
{
    //bool enable;
	GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():Check Checksum.ini start, Application->ExeName is %s", Application->ExeName.c_str());
    if (!iniFilePath.IsEmpty())
    {
		AnsiString temp = iniFilePath + "\\Checksum.ini";
		if (Check_FileExist(temp))
		{
			//enable = m_iniFile.read_HostBufferEnable("Checksum.ini", temp);
    		GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():Load ini.");
			//return (1 == enable)?(true):(false);
			return true;
		}

    }

    //enable = m_iniFile.read_HostBufferEnable("Checksum.ini", Application->ExeName);
    GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():Tool ini, Application->ExeName is %s", Application->ExeName.c_str());

	AnsiString asPath;
	AnsiString asSetupFile;

    if ( getPathFromStr(Application->ExeName, asPath) )
    {
	   GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():asPath is %s.", asPath);
       asSetupFile = asPath + "Checksum.ini";
	   if (Check_FileExist(asSetupFile))
	   {
		   GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():Tool ini.");
			//return (1 == enable)?(true):(false);
			return true;
	   }
    }

    //return (1 == enable)?(true):(false);
	GC_TRACE("<MDLF> IsHostBufferCksmFileExisted():Checksum.ini file is not found!", Application->ExeName.c_str());
	return false;
}
bool TfrmMultiLoad::MD5CheckSumVerfy(AnsiString &path,string &checksum)
{
           //string checksum_ini;
	    //TIniFile* check_sum_ini;
    AnsiString inichecksumPath;   // add by mtk71502 20130228
	    //inichecksumPath = iniFilePath + "\\CheckSum.ini"; // add by mtk71502 20130228
	    //check_sum_ini= new TIniFile(inichecksumPath.c_str());
    char outbuffer[128];
    const char *copy_path=path.c_str();
    char *RomFile_path=new char[strlen(copy_path)+1];
    strcpy(RomFile_path,copy_path);
    MD5_Reset();
    MD5_File(RomFile_path);
    MD5_GetResult(outbuffer);


    const char *checksumini_to_char=checksum.c_str();
    char *checksumini_char=new char[strlen(checksumini_to_char)+1];
    strcpy(checksumini_char,checksumini_to_char);
    if(strcmp(checksumini_char,outbuffer))
    {
        GC_TRACE("<MDLF>romfile path%s, Checksum.ini  is %s.  MD5 Calculation value is %s", RomFile_path,checksumini_char,outbuffer);
        Application->MessageBox( "md5 calculation is not the same with checksum.ini ", "Fail", MB_OK );
        return false;
    }

    delete [] RomFile_path;
    delete [] checksumini_char;
    return true;
}
bool TfrmMultiLoad::GetCheckSumVer()
{
      // add md5 ,mtk71502,20130413
	// step1 read sp MDT ini version
    AnsiString as_KEY_NAME = "Force_Checksum_Version" ;
    AnsiString as_Checked = m_INIFileH.read_field(INI_FILE_NAME, Application->ExeName, "form", as_KEY_NAME, "V1");
       // step2 read checksum ini version
	AnsiString inichecksumPath;   // add by mtk71502 20130228
	inichecksumPath = iniFilePath + "\\CheckSum.ini"; // add by mtk71502 20130228

    string  check_sum_ver;
	TIniFile* check_sum_ini;
	check_sum_ini= new TIniFile(inichecksumPath.c_str());
	check_sum_ver = check_sum_ini->ReadString("VERSION", "VERSION", "0x00000000").c_str();
	GC_TRACE("<TfrmMultiLoad> GetCheckSumVer():Checksum Version is %s!", check_sum_ver.c_str());    if(as_Checked=="V1")  // if V1, please do not md5
	   	return false;
	else
	{
	   if(check_sum_ver=="V2")// Md5
		  	return true;
	   else
	   	  {
	   	    Application->MessageBox( "do you want to run md5 check sum? ", "Fail", MB_OK );
	   	    return false;
	   	}

	}

}

bool TfrmMultiLoad::MD5Checksum()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned short rom_count;
    ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
    AnsiString inichecksumPath;   // add by mtk71502 20130228
    inichecksumPath = iniFilePath + "\\CheckSum.ini"; // add by mtk71502 20130228
    string checksum_ini;

    TIniFile* check_sum_ini;
    check_sum_ini= new TIniFile(inichecksumPath.c_str());

    int ret=DL_GetCount(dl_handle, &rom_count);//return 0 is done ok
    if(ret)
        {
        GC_TRACE("<MDLF>get count false!");
        return false;
        }

    ret=DL_Rom_GetInfoAll(dl_handle, rom_info, MAX_LOAD_SECTIONS);//return 0 is done ok
    if(ret)
        {
        GC_TRACE("<MDLF>get info false!");
        return false;
        }

    for (int i=0; i<rom_count; i++)
        {
           AnsiString temp_rom_file ;
           //GC_TRACE("<MDLF> erjing For PRELOADER , Checksu.ini  is %s.", rom_info[i].name);
           if(NULL != rom_info[i].enable)
            {
            //if(search_image_files(iniFilePath, temp_rom_file, rom_info[i].name))
            // {
               temp_rom_file = rom_info[i].filepath;
               checksum_ini =check_sum_ini->ReadString("CheckSum_V2", rom_info[i].name, "0x00000000").c_str();
               if(MD5CheckSumVerfy(temp_rom_file,checksum_ini))
               {
                   GC_TRACE("<MDLF> Md5 %s checksum match!",rom_info[i].name);
               }
               else
               {
                   GC_TRACE("<MDLF> Md5 %s checksum not match!",rom_info[i].name);
                   return false;
               }
            //}
             // }
            //else
            //	{
            //	GC_TRACE("<MDLF> Md5 %s checksum not found!",rom_info[i].name);
            ///	return false;
            //	}
            }
       }

     return true;
}

void TfrmMultiLoad::LoadHostBufferStdCksm()
{
    // Get the check sum file path
	AnsiString iniFileNameWithPath;

	if (!iniFilePath.IsEmpty())
	{
		GC_TRACE("<MDLF> LoadHostBufferStdCksm(): the load path is %s", iniFilePath);
		iniFileNameWithPath = iniFilePath + "\\CheckSum.ini";
		if (Check_FileExist(iniFileNameWithPath))
		{
			GC_TRACE("<MDLF> LoadHostBufferStdCksm(): the checksum ini path is %s", iniFileNameWithPath);
		}
		else // Get tool path checksum ini.
		{
			AnsiString asPath = NULL;

			if (getPathFromStr(Application->ExeName,asPath))
			{
				GC_TRACE("<MDLF> LoadHostBufferStdCksm(): the tool path is %s", asPath);
				iniFileNameWithPath = asPath + "CheckSum.ini";
				if (Check_FileExist(iniFileNameWithPath))
				{
					GC_TRACE("<MDLF> LoadHostBufferStdCksm(): there is valid checksum ini file in tool path.");
				}
				else
				{
					GC_TRACE("<MDLF> LoadHostBufferStdCksm(): Can not find valid checksum ini file.");
					return;
				}
			}
		}
	}


	TIniFile* check_sum_ini_;
	TStringList* str_list = NULL;
	try {
		str_list = new TStringList;
		check_sum_ini_ = new TIniFile(iniFileNameWithPath.c_str());
		check_sum_ini_->ReadSection("CheckSum", str_list);

		unsigned int check_sum = 0;
		int checksum_items_count = str_list->Count;

		m_cksmVt.clear();

		for ( int i = 0; i < checksum_items_count; i++) {
		    check_sum = strtoul(check_sum_ini_->ReadString("CheckSum", str_list->Strings[i], "0x00000000").c_str(), NULL, 16);
			m_cksmVt.push_back(std::make_pair(str_list->Strings[i].c_str(), check_sum));
			GC_TRACE("<TfrmMultiLoad> LoadHostBufferStdCksm(): Load Checksum>> %s = %04X.", str_list->Strings[i], check_sum);
		}
	} catch (const std::exception& ex) {
		if (NULL != str_list) {
		    delete str_list;
			str_list = NULL;
		}
		if (NULL != check_sum_ini_) {
		    delete check_sum_ini_;
			check_sum_ini_ = NULL;
		}
		GC_TRACE("<MDLF> Init image_checksum_map_ failed, reason(%s).", ex.what());
	}
    if (NULL != str_list)
	{
	    delete str_list;
		str_list = NULL;
	}
	if (NULL != check_sum_ini_)
	{
	    delete check_sum_ini_;
		check_sum_ini_ = NULL;
	}
}

#if 0
int TfrmMultiLoad::VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult)
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    CksmVector::iterator it;
    unsigned short count;
    ROM_INFO rom[MAX_LOAD_SECTIONS];
    int ret = S_DONE;
    bool failFlag = false;


    if(m_cksmVt.empty())
    {
        GC_TRACE("<MDLF> VerifyHostBufferCksm(): Cksm vector is empty, no golden value!");
        return -1;
    }

    DLCALL(DL_GetCount(dl_handle, &count));
    DLCALL(DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS));

    for( it = m_cksmVt.begin(); (it != m_cksmVt.end()) /*&& (0xFFFFFFFF != (*it).second)*/; it++ )
    {
        int k = 0;
        for(; k < count; k++)
        {
           if(0 == (*it).first.compare(rom[k].name))
           {
               //ret = DL_VerifyROMMemBuf(dl_handle, rom[k].index, (*it).second);
			   checksumArg.index =  rom[k].index;
			   checksumArg.chksum = (*it).second;
			   ret = DL_VerifyROMMemBuf(dl_handle, &checksumArg, &checksumResult);
               if(S_DONE != ret)
               {
                   GC_TRACE("<MDLF> VerifyHostBufferCksm(): %s(0x%x) checksum verify fail!", (*it).first.c_str(), (*it).second);
                   failFlag = true;
                   break;
               }
               else
               {
                   GC_TRACE("<MDLF> VerifyHostBufferCksm(): %s(0x%x) checksum verify pass.", (*it).first.c_str(), (*it).second);
                   break;
               }
           }
        }

        /*if(k >= count)
        {
            GC_TRACE("<MDLF> VerifyHostBufferCksm(): Cannot find ROM info(%s)!", (*it).first.c_str());
            return -1;
        }*/
    }

    return (failFlag)?(-1):(0);
}
#endif

/////////////////////////////////////////////////////////
//For Android DL
/*
void TfrmMultiLoad::Remove_Other_Rom_File( bool &is_need_dl_uboot,
                                           bool &is_need_dl_image )
{
   DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
   unsigned short       count;
   unsigned short       cust_count = 0;

   is_need_dl_uboot = false;
   is_need_dl_image = false;

   for(int i=0; i<ANDROID_IMAGE_FILE_NUMBER; i++)
   {
        m_image_files[i].is_download = false;
   }

   if ( !DL_GetCount(dl_handle, &count) )
   {
       ROM_INFO	rom[MAX_LOAD_SECTIONS];

       if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
       {
            for(int i = 0; i < count; i++)
            {
     	       GC_TRACE("<MDLF> (Name: %s) (Addr: 0x%x) (Enable: %d)", rom[i].name, rom[i].begin_addr, rom[i].enable);
            }


	   for ( int i=0; i<count; i++ )
            {
	       if(_TRUE != rom[i].item_is_visable)
	       {
		       continue;
	       }

                if(strcmpi(rom[i].name, SCATTER_PRELOADER_NAME) == 0)
                {
                   is_need_dl_uboot = rom[i].enable;
                }
                else if(strcmpi(rom[i].name, SCATTER_UBOOT_NAME) == 0)
                {
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].is_download = rom[i].enable;

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_type = DM_IMG_TYPE_UBOOT;
                   m_image_files[DM_IMG_TYPE_UBOOT_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_UBOOT_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(uboot_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_path = uboot_path;
                }
                else if(strcmpi(rom[i].name, SCATTER_LOGO_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].is_download = rom[i].enable;

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_type = DM_IMG_TYPE_LOGO;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(logo_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_path = logo_path;
                }
                else if(strcmpi(rom[i].name, SCATTER_BOOTIMG_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].is_download = rom[i].enable;

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_type =  DM_IMG_TYPE_BOOTIMG;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(bootimg_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_path = bootimg_path;
                }

                else if(strcmpi(rom[i].name, SCATTER_RECOVERY_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].is_download = rom[i].enable;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_type =   DM_IMG_TYPE_RECOVERY;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(recovery_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_path = recovery_path;
                }

                else if(strcmpi(rom[i].name, SCATTER_SECRO_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].is_download = rom[i].enable;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_type =   DM_IMG_TYPE_SECSTATIC;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(secro_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_path = secro_path;
                }
                else if(strcmpi(rom[i].name, SCATTER_ANDROID_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].is_download = rom[i].enable;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_type =   DM_IMG_TYPE_ANDROID;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].addr_boundary = (int)rom[i+1].begin_addr;

                   strncpy(android_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_path = android_path;
                }
                else if(strcmpi(rom[i].name, SCATTER_USERDATA_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].is_download = rom[i].enable;

                   DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

                   if(rom[i].enable)
                     is_need_dl_image = true;

                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_type =   DM_IMG_TYPE_USRDATA;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].addr_boundary = (int)(0xFFFFFFFF);

                   strncpy(usedata_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_path = usedata_path;
                }
                else
                {
	                //Judge the format type of this image
	                DM_IMG_FORMAT format;

	                if( (NULL != strstr(rom[i].name, "FAT")) ||(NULL != strstr(rom[i].name, "fat")) )
	                {
		                format = DM_IMG_FORMAT_FAT;
	                }
	                else if( (NULL != strstr(rom[i].name, "YAFFS")) ||(NULL != strstr(rom[i].name, "yaffs")) )
	                {
		                format = DM_IMG_FORMAT_YAFFS2;
	                }
	                else
	                {
		                GC_TRACE("<MDLF> The format of image is obscure, please indicate in image name explicitly!");
		                continue;
	                }

	                //Set parameters for cust image
	                if(strcmpi(rom[i].name, SCATTER_CUSTPACK1_NAME) == 0)
	                {
				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].is_download = rom[i].enable;

				DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

				if(rom[i].enable)
					is_need_dl_image = true;

				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_format = format;
				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE1;
				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].addr_off = (int)rom[i].begin_addr;
				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].addr_boundary = (int)(0xFFFFFFFF);

				strncpy(custpack1_path, rom[i].filepath, 254);
				m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_path = custpack1_path;
	                }
	                else if(strcmpi(rom[i].name, SCATTER_CUSTPACK2_NAME) == 0)
	                {
				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].is_download = rom[i].enable;

				DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

				if(rom[i].enable)
					is_need_dl_image = true;

				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_format = format;
				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE2;
				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].addr_off = (int)rom[i].begin_addr;
				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].addr_boundary = (int)(0xFFFFFFFF);

				strncpy(custpack2_path, rom[i].filepath, 254);
				m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_path = custpack2_path;
	                }
	                else if(strcmpi(rom[i].name, SCATTER_CUSTPACK3_NAME) == 0)
	                {
				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].is_download = rom[i].enable;

				DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

				if(rom[i].enable)
					is_need_dl_image = true;

				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_format = format;
				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE3;
				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].addr_off = (int)rom[i].begin_addr;
				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].addr_boundary = (int)(0xFFFFFFFF);

				strncpy(custpack3_path, rom[i].filepath, 254);
				m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_path = custpack3_path;
	                }
	                else if(strcmpi(rom[i].name, SCATTER_CUSTPACK4_NAME) == 0)
	                {
				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].is_download = rom[i].enable;

				DL_Rom_SetEnableAttr(dl_handle, i, _FALSE);

				if(rom[i].enable)
					is_need_dl_image = true;

				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_format = format;
				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE4;
				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].addr_off = (int)rom[i].begin_addr;
				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].addr_boundary = (int)(0xFFFFFFFF);

				strncpy(custpack4_path, rom[i].filepath, 254);
				m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_path = custpack4_path;
	                }
                }
	    }
	}
   }
}
*/


int TfrmMultiLoad::FindImageAttrVector(char * name, FORMATTYPE & fmtp)
{
    FMTPVector::iterator it;

    for( it = m_fmtp_vector.begin() ; it != m_fmtp_vector.end(); it++ )
    {
        if( 0 == strncmpi(name, (*it).first.scatter_name, strlen(name)) )
        {
            fmtp = (*it).second;
            return 0;
        }
    }

    return -1;

}

int TfrmMultiLoad::FillDownloadImageInfo()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned short count;
    int seq = 0;
    IMAGE_ATTR attr;
    FORMATTYPE fmtp;
    SCATTER_NAME name;


    for(int i = 0; i < ANDROID_IMAGE_FILE_NUMBER; i++)
    {
        m_image_files[i].is_download = false;
    }

    m_attr_vector.clear();


    if ( !DL_GetCount(dl_handle, &count) )
    {
        ROM_INFO	rom[MAX_LOAD_SECTIONS];

        if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
        {
            for(int i = 0; i < count; i++)
            {
                GC_TRACE("<MDLF> FillDownloadImageInfo(): Name(%s), Addr(0x%x), Enable(%d)", rom[i].name, rom[i].begin_addr, rom[i].enable);
            }


            for ( int i=0; i<count; i++ )
            {
                if(_TRUE != rom[i].item_is_visable)
                {
                    //Get BinRegion info from scatter file
                    //if( 0 == strncmpi(rom[i].name, SCATTER_NVRAM_NAME, strlen(SCATTER_NVRAM_NAME)) )
			if( 0 == strncmpi(rom[i].name, SCATTER_NVRAM_NAME2,strlen(SCATTER_NVRAM_NAME2)) || 0 == strncmpi(rom[i].name, SCATTER_NVRAM_NAME,strlen(SCATTER_NVRAM_NAME))) // need modify to support new scatter, 20130408
                    {
                        m_frm_bin_region.m_path = NULL;
                        m_frm_bin_region.m_type = DM_IMG_TYPE_CUST_IMAGE3;
                        m_frm_bin_region.m_format = DM_IMG_FORMAT_FAT;
                        m_frm_bin_region.addr_off = rom[i].begin_addr;
                        m_frm_bin_region.addr_boundary = rom[i+1].begin_addr;
                        m_frm_bin_region.is_patch = false;
                        m_frm_bin_region.is_download = true;

                        GC_TRACE("<MDLF> FillDownloadImageInfo(): [BinRegion] Name(%s), Format(%d), Type(%d), Addr(0x%x), Bdy(0x%x)",
	                        SCATTER_NVRAM_NAME2,
	                        m_frm_bin_region.m_format,
	                        m_frm_bin_region.m_type,
	                        m_frm_bin_region.addr_off,
	                        m_frm_bin_region.addr_boundary);
                    }

                    continue;
                }

                if( !FindImageAttrVector(rom[i].name, fmtp) )
                {
                    m_image_files[seq].m_type = fmtp.type;
                    m_image_files[seq].m_format = fmtp.format;

                    m_image_files[seq].is_download = rom[i].enable;
                    m_image_files[seq].is_patch = false;
                    m_image_files[seq].addr_off = (int)rom[i].begin_addr;

                    if( i < count -2)
                    {
                        m_image_files[seq].addr_boundary = (int)rom[i+1].begin_addr;
                    }
                    else
                    {
                        m_image_files[seq].addr_boundary = (int)(0xFFFFFFFF);
                    }


                    memset(&attr, 0, sizeof(attr));
                    memset(&name, 0, sizeof(name));
                    attr.seq = seq;
                    attr.pos = i;
                    strncpy(m_image_path_memory[seq], rom[i].filepath, 256);
                    strncpy(name.scatter_name, rom[i].name, 64);
                    m_attr_vector.push_back(make_pair(name, attr));

                    m_image_files[seq].m_path = m_image_path_memory[seq];

                    GC_TRACE("<MDLF> FillDownloadImageInfo(): [Image inserted] Path(%s), Format(%d), Type(%d), IsD(%d), Seq(%d), Pos(%d)",
                        m_image_files[seq].m_path,
                        m_image_files[seq].m_format,
                        m_image_files[seq].m_type,
                        m_image_files[seq].is_download,
                        attr.seq, attr.pos);


                    seq++;

                }
                else
                {
                    GC_TRACE("<MDLF> FillDownloadImageInfo(): [Partition excluded] Name(%s)", rom[i].name);
                }

            }

            return 0;
        }
    }

    return -1;
}

int TfrmMultiLoad::DisableDLAttribute()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    ImageAttrVector::iterator it;

    if(m_attr_vector.empty())
    {
        GC_TRACE("<MDLF> DisableDLAttribute(): DL vector is empty, cannot disable DL attribute!");
        return -1;
    }

    for( it = m_attr_vector.begin() ; it != m_attr_vector.end(); it++ )
    {
        DL_Rom_SetEnableAttr(dl_handle, (*it).second.pos, _FALSE);
    }

    return 0;
}

int TfrmMultiLoad::RestoreDLAttribute()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    ImageAttrVector::iterator it;

    if(m_attr_vector.empty())
    {
        GC_TRACE("<MDLF> RestoreDLAttribute(): DL vector is empty, cannot disable DL attribute!");
        return -1;
    }

    for( it = m_attr_vector.begin() ; it != m_attr_vector.end(); it++ )
    {
        if( m_image_files[(*it).second.seq].is_download )
        {
            DL_Rom_SetEnableAttr(dl_handle, (*it).second.pos, _TRUE);
        }
    }

    return 0;
}

bool TfrmMultiLoad::IsDLAnyImage()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned short       count;


    if ( !DL_GetCount(dl_handle, &count) )
    {
        ROM_INFO	rom[MAX_LOAD_SECTIONS];

        if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
        {
        	   for ( int i = 0; i < count; i++ )
            {
                if(true == rom[i].enable)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool TfrmMultiLoad::IsDLBootloader()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned short       count;


    if ( !DL_GetCount(dl_handle, &count) )
    {
        ROM_INFO	rom[MAX_LOAD_SECTIONS];

        if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
        {
        	   for ( int i = 0; i < count; i++ )
            {
                if(strcmpi(rom[i].name, SCATTER_PRELOADER_NAME) == 0)
                {
                    return rom[i].enable;
                }
            }
        }
    }

    return false;
}

bool TfrmMultiLoad::IsDLSystem()
{
    for(int i = 0; i < ANDROID_IMAGE_FILE_NUMBER; i++)
    {
        if(true == m_image_files[i].is_download)
        {
            return true;
        }
    }

    return false;
}

ULL64 TfrmMultiLoad::GetUbootStartAddr()
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned short                   count;

    if ( !DL_GetCount(dl_handle, &count) )
    {
        ROM_INFO       rom[MAX_LOAD_SECTIONS];

        if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
        {
            for ( int i = 0; i < count; i++ )
            {
                if(strcmpi(rom[i].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom[i].name, SCATTER_LK_NAME) == 0)
                {
                    return rom[i].begin_addr;
                }
            }
        }
    }
    return 0x340000;
}


/*
void TfrmMultiLoad::Restore_Rom_File()
{
   DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
   unsigned short  count;
   unsigned short  cust_count = 0;

   if ( !DL_GetCount(dl_handle, &count) )
   {
       ROM_INFO	rom[MAX_LOAD_SECTIONS];

       if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
       {
	   for ( int i=0; i<count; i++ )
            {
                if(_TRUE != rom[i].item_is_visable)
	       {
		       continue;
	       }

                if(strcmp(rom[i].name, SCATTER_PRELOADER_NAME) == 0)
                {
                }
                else if(strcmp(rom[i].name, SCATTER_UBOOT_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_UBOOT_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }
                else if(strcmp(rom[i].name, SCATTER_LOGO_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_LOGO_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }
                else if(strcmp(rom[i].name, SCATTER_BOOTIMG_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }

                else if(strcmp(rom[i].name, SCATTER_RECOVERY_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }

                else if(strcmp(rom[i].name, SCATTER_SECRO_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_SECRO_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }
                else if(strcmp(rom[i].name, SCATTER_ANDROID_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_ANDROID_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }
                else if(strcmp(rom[i].name, SCATTER_USERDATA_NAME) == 0)
                {
                   if(m_image_files[DM_IMG_TYPE_USRDATA_INDEX].is_download)
                        DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
                }
                else
                {
	                if(strcmp(rom[i].name, SCATTER_CUSTPACK1_NAME) == 0)
	                {
		                 if(m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].is_download)
			                 DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
	                }
	                else if(strcmp(rom[i].name, SCATTER_CUSTPACK2_NAME) == 0)
	                {
		                 if(m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].is_download)
			                 DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
	                }
	                else if(strcmp(rom[i].name, SCATTER_CUSTPACK3_NAME) == 0)
	                {
		                 if(m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].is_download)
			                 DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
	                }
	                else if(strcmp(rom[i].name, SCATTER_CUSTPACK4_NAME) == 0)
	                {
		                 if(m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].is_download)
			                 DL_Rom_SetEnableAttr(dl_handle, i, _TRUE);
	                }
                }
	    }
	}
   }
}
*/

void TfrmMultiLoad::RedrawTypeComBox()
{
   int index= MainForm->cbType->ItemIndex;

    MainForm->cbType->Clear();
    MainForm->cbType->Items->Add("Format and Download All");
    MainForm->cbType->Items->Add("Firmware upgrade");
    MainForm->cbType->Items->Add("Format All");
    MainForm->cbType->Items->Add("Format bootloader");
#ifndef SERVICE_CENTER
    MainForm->cbType->ItemIndex = index;
#else
    MainForm->cbType->ItemIndex = OP_FIRMWARE_UPGRADE;
#endif
}

/*
void __fastcall TfrmMultiLoad::Set_Android_Image_Info()
{
   DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
   unsigned short       count;


   for(int i=0; i<ANDROID_IMAGE_FILE_NUMBER; i++)
   {
        m_image_files[i].is_download = false;
   }

   if ( !DL_GetCount(dl_handle, &count) )
   {
       ROM_INFO	rom[MAX_LOAD_SECTIONS];
       if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
       {
            for(int i = 0; i < count; i++)
            {
     	       GC_TRACE("<MDLF> (Name: %s) (Addr: 0x%x) (Enable: %d)", rom[i].name, rom[i].begin_addr, rom[i].enable);
            }

	   for ( int i=0; i<count; i++ )
            {
                if(strcmp(rom[i].name, SCATTER_PRELOADER_NAME) == 0)
                {

                }
                else if(strcmp(rom[i].name, SCATTER_UBOOT_NAME) == 0)
                {
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].is_download = rom[i].enable;
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_format = DM_IMG_FORMAT_FAT;
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_type = DM_IMG_TYPE_UBOOT;
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].addr_off = (int)rom[i].begin_addr;
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                    strncpy(uboot_path, rom[i].filepath, 254);
                    m_image_files[DM_IMG_TYPE_UBOOT_INDEX].m_path = uboot_path;
                }
                else if(strcmp(rom[i].name, SCATTER_LOGO_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_type = DM_IMG_TYPE_LOGO;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                   strncpy(logo_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_LOGO_INDEX].m_path = logo_path;
                }
                else if(strcmp(rom[i].name, SCATTER_BOOTIMG_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_type =  DM_IMG_TYPE_BOOTIMG;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                   strncpy(bootimg_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_BOOTIMG_INDEX].m_path = bootimg_path;
                }
                else if(strcmp(rom[i].name, SCATTER_RECOVERY_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_format = DM_IMG_FORMAT_FAT;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_type =   DM_IMG_TYPE_RECOVERY;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                   strncpy(recovery_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_RECOVERY_INDEX].m_path = recovery_path;
                }
                else if(strcmp(rom[i].name, SCATTER_SECRO_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_type =   DM_IMG_TYPE_SECSTATIC;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                   strncpy(secro_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_SECRO_INDEX].m_path = secro_path;
                }
                else if(strcmp(rom[i].name, SCATTER_ANDROID_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_type =   DM_IMG_TYPE_ANDROID;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].addr_boundary = (int)rom[i+1].begin_addr;
                   strncpy(android_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_ANDROID_INDEX].m_path = android_path;
                }
                else if(strcmp(rom[i].name, SCATTER_USERDATA_NAME) == 0)
                {
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].is_download = rom[i].enable;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_format = DM_IMG_FORMAT_YAFFS2;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_type =   DM_IMG_TYPE_USRDATA;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].addr_off = (int)rom[i].begin_addr;
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].addr_boundary = (int)(0xFFFFFFFF);
                   strncpy(usedata_path, rom[i].filepath, 254);
                   m_image_files[DM_IMG_TYPE_USRDATA_INDEX].m_path = usedata_path;
                }
                else
                {
                	                //Judge the format type of this image
	                DM_IMG_FORMAT format;

	                if( (NULL != strstr(rom[i].name, "FAT")) ||(NULL != strstr(rom[i].name, "fat")) )
	                {
		                format = DM_IMG_FORMAT_FAT;
	                }
	                else if( (NULL != strstr(rom[i].name, "YAFFS")) ||(NULL != strstr(rom[i].name, "yaffs")) )
	                {
		                format = DM_IMG_FORMAT_YAFFS2;
	                }
	                else
	                {
		                GC_TRACE("<MDLF> The format of image is obscure, please indicate in image name explicitly!");
		                continue;
	                }

	                if(strcmp(rom[i].name, SCATTER_CUSTPACK1_NAME) == 0)
	                {
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].is_download = rom[i].enable;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_format = format;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE1;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].addr_off = (int)rom[i].begin_addr;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].addr_boundary = (int)(0xFFFFFFFF);
	                   strncpy(custpack1_path, rom[i].filepath, 254);
	                   m_image_files[DM_IMG_TYPE_CUSTPACK1_INDEX].m_path = custpack1_path;
	                }
	                if(strcmp(rom[i].name, SCATTER_CUSTPACK2_NAME) == 0)
	                {
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].is_download = rom[i].enable;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_format = format;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE2;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].addr_off = (int)rom[i].begin_addr;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].addr_boundary = (int)(0xFFFFFFFF);
	                   strncpy(custpack2_path, rom[i].filepath, 254);
	                   m_image_files[DM_IMG_TYPE_CUSTPACK2_INDEX].m_path = custpack2_path;
	                }
	                if(strcmp(rom[i].name, SCATTER_CUSTPACK3_NAME) == 0)
	                {
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].is_download = rom[i].enable;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_format = format;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE3;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].addr_off = (int)rom[i].begin_addr;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].addr_boundary = (int)(0xFFFFFFFF);
	                   strncpy(custpack3_path, rom[i].filepath, 254);
	                   m_image_files[DM_IMG_TYPE_CUSTPACK3_INDEX].m_path = custpack3_path;
	                }
	                if(strcmp(rom[i].name, SCATTER_CUSTPACK4_NAME) == 0)
	                {
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].is_download = rom[i].enable;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_format = format;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_type =   DM_IMG_TYPE_CUST_IMAGE4;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].addr_off = (int)rom[i].begin_addr;
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].addr_boundary = (int)(0xFFFFFFFF);
	                   strncpy(custpack4_path, rom[i].filepath, 254);
	                   m_image_files[DM_IMG_TYPE_CUSTPACK4_INDEX].m_path = custpack4_path;
	                }
                }
	    }
	}
   }


}
*/

void TfrmMultiLoad::MakeImageAttrVector()
{
    SCATTER_NAME name;
    FORMATTYPE fmtp;

    m_fmtp_vector.clear();

    strncpy(name.scatter_name, SCATTER_UBOOT_NAME, strlen(SCATTER_UBOOT_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_UBOOT;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_BOOTIMG_NAME, strlen(SCATTER_BOOTIMG_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_BOOTIMG;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_LOGO_NAME, strlen(SCATTER_LOGO_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_LOGO;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_RECOVERY_NAME, strlen(SCATTER_RECOVERY_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_RECOVERY;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_SECRO_NAME, strlen(SCATTER_SECRO_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_YAFFS2;
    fmtp.type = DM_IMG_TYPE_SECSTATIC;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_ANDROID_NAME, strlen(SCATTER_ANDROID_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_YAFFS2;
    fmtp.type = DM_IMG_TYPE_ANDROID;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_USERDATA_NAME, strlen(SCATTER_USERDATA_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_YAFFS2;
    fmtp.type = DM_IMG_TYPE_USRDATA;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_CUSTPACK1_NAME, strlen(SCATTER_CUSTPACK1_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_CUST_IMAGE1;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_CUSTPACK2_NAME, strlen(SCATTER_CUSTPACK2_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_YAFFS2;
    fmtp.type = DM_IMG_TYPE_CUST_IMAGE2;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_CUSTPACK3_NAME, strlen(SCATTER_CUSTPACK3_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_FAT;
    fmtp.type = DM_IMG_TYPE_CUST_IMAGE3;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

    strncpy(name.scatter_name, SCATTER_CUSTPACK4_NAME, strlen(SCATTER_CUSTPACK4_NAME)+1);
    fmtp.format = DM_IMG_FORMAT_YAFFS2;
    fmtp.type = DM_IMG_TYPE_CUST_IMAGE4;
    m_fmtp_vector.push_back(make_pair(name, fmtp));

}

int TfrmMultiLoad::AutoLoadSecroSignImg(DL_HANDLE_T *p_dl_handle, string strPath)
{
    unsigned short rom_count;
	int iRet = -1;
	ROM_INFO rom_info[MAX_LOAD_SECTIONS];
	string strSecro = SCATTER_SECRO_NAME;
	string strImagepath;
	int iFind;
    GC_TRACE("<MDLF> AutoLoadSecroSignImg: Enter...");
	
    //Get image path
    if(strPath.empty()){
		return iRet;
	}
	
    iFind = strPath.find_last_of("\\");
	if(iFind == string::npos){
        return iRet;
	}	

    strImagepath = strPath.substr(0, iFind) + "\\sro-lock-sign.img";
	
	GC_TRACE("<MDLF> AutoLoadSecroSignImg: secro path = %s", strImagepath.c_str());
    if(!DL_GetCount(*p_dl_handle, &rom_count))
    {
        if(rom_count > MAX_LOAD_SECTIONS)
        {
           return -1;
        }
        
		if(!DL_Rom_GetInfoAll(*p_dl_handle, rom_info, rom_count))
        {    
            for (int i=0; i<rom_count; i++)
			{
			   iFind = strSecro.find(rom_info[i].name);
               if(iFind != string::npos){ // find out the secro img
                   if(rom_info[i].enable == false)
                   { 
                      GC_TRACE("<MDLF> AutoLoadSecroSignImg: need load sro-lock-sign.img...");
					  if(Load_Rom(p_dl_handle,rom_info[i].index, strImagepath.c_str())){
					      GC_TRACE("<MDLF> AutoLoadSecroSignImg: load secro success! path = %s", strImagepath.c_str());
					  }else{
					      GC_TRACE("<MDLF> AutoLoadSecroSignImg: load secro Fail! path = %s", strImagepath.c_str());
					  }   
                   }else{
                      GC_TRACE("<MDLF> AutoLoadSecroSignImg: secro is enable,do not need load sro-lock-sign.img again!");
                   }
                   
			   	   break;
               }
            }
		}
    }
	return iRet;
}

