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
 *   T_GPS.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Backup and restore calibration data thread source file
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
 * Sep 8 2008 MTK02172
 * [STP100001803] [Multiport download tool] Multiport download tool ver 3.0836.0
 *
 *
 * Aug 13 2008 mtk00490
 * [STP100001788] [Multiport download tool] Multiport download tool ver 3.0832.0
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef _FLASHDOWNLOAD_H_
#include "FlashDownload.h"
#endif

#ifndef _FORM_MULTILOAD_H_
#include "Form\form_MultiLoad.h"
#endif


#include "NvramBackupMap.h"
#include "BackupConfig.h"

// form
#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#include "MultiportFlashDownloadTool.h"
#endif

#include <sstream>
#include <iomanip>
#include <algorithm>    // std::transform

using namespace std;


//===========================================================================

//extern int g_iStopFlag[MAX_SUPPORT_PORT_NUM];
extern int g_download_finish_flag[MAX_SUPPORT_PORT_NUM];
extern TMainForm * MainForm;
extern TfrmMultiLoad *frmMultiLoad;
extern RB_HANDLE_T         g_RB_HANDLE;


const char BkRsParentFolderName[50] = "backup_and_restore_data";
const char BkRsCksmFileName[20] = "checksum";
const char BkRsDataFileName[20] = "BinRegion.img";
const char BkRsProductInfoCksmFileName[20] = "ProductInfoChecksum";
const char BkRsProductInfoDataFileName[20] = "ProductInfo.img";
const int RIDLength = 16;

//===========================================================================

//const char SCATTER_NVRAM_NAME[100] = "__NODL_NVRAM";

//===========================================================================

//---------------------------------------------------------------------------
static int __stdcall cb_write_memory_init( void *usr_arg )
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->prefix = "WR ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, (TColor) 0x0088A050);

    return 0;
}
//---------------------------------------------------------------------------
static int __stdcall cb_write_memory_progress( unsigned char percent,
                                  unsigned int   sent_bytes,
                                  unsigned int   total_bytes,
                                  void*         usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}

//===========================================================================
///////////////////////////////// Query  ////////////////////////////////////
//===========================================================================
bool _fastcall TBOOT1::Query_BackupNVRAMData_Support(void)
{
    META_RESULT MetaResult = SP_META_QueryIfFunctionSupportedByTarget_r(m_sDnParam.i_meta_handle, 500, "META_MISC_GetIMEILocation_r");
    if (MetaResult != META_SUCCESS)
    {
        return false;
    }
    return true;
}

//===========================================================================
int _fastcall TBOOT1::CheckBackupLocation(void)
{
    GC_TRACE("[COM%d] Start check backup location", m_sComPortSetting.com.number);
    int ret = 0;

    //Update UI
    prefix = "NV ck ";
    SetBackupProgressColor();
    SyncUpdateProgressPercentage(0);
    SyncInitProgress();

    AnsiString parent = m_sDnParam.as_working_dir + BkRsParentFolderName;
    if( -1 == CheckBkRsParentFolder(parent.c_str()) )
    {
        return -1;
    }
    SyncUpdateProgressPercentage(10);


    if(OP_COM_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        //Check backup folder
        AnsiString as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + IntToStr(m_sComPortSetting.com.number);

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            if( !CheckBackupDataFileExisted(as_backup_dir.c_str()) )
            {
                GC_TRACE("[COM%d] CheckBackupDataFileExisted() fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Backup data files are not found!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        else
        {
            GC_TRACE("[COM%d] Backup folder is not found!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Backup folder is not found!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        SyncUpdateProgressPercentage(100);
        SyncUpdateTextHint(as_backup_dir.c_str());

    }
    else if(OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        ret = PrepareMETAOperation();
        if(0 != ret)
        {
            return ret;
        }

        //META get chip ID
        unsigned char chipID[20] = {0};
        unsigned int ms_timeout = 10000;
        char strRID[255];
        memset(strRID, 0, 255);

        GC_TRACE("[COM%d] Start SP_META_GetChipID_r...", m_sComPortSetting.com.number);
        ret = SP_META_GetChipID_r(m_sDnParam.i_meta_handle, ms_timeout, chipID);
        if(META_SUCCESS != ret)
        {
            GC_TRACE("[COM%d] SP_META_GetChipID_r fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get chip ID fail!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return ret;
        }
        ConvertRIDtoString(chipID, RIDLength, strRID);
        GC_TRACE("[COM%d] Complete SP_META_GetChipID_r, ChipID: %s", m_sComPortSetting.com.number, strRID);


        //Check backup folder
        AnsiString as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + strRID;

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            if( !CheckBackupDataFileExisted(as_backup_dir.c_str()) )
            {
                GC_TRACE("[COM%d] CheckBackupDataFileExisted() fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Backup data files are not found!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        else
        {
            GC_TRACE("[COM%d] Backup folder is not found!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Backup folder is not found!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        SyncUpdateProgressPercentage(100);
        SyncUpdateTextHint(as_backup_dir.c_str());

        ret = TerminateMETAOperation();

    }

    return ret;

}

//===========================================================================
int _fastcall TBOOT1::BackupNVRAMData(void)
{
    GC_TRACE("[COM%d] Start backup NVRAM data", m_sComPortSetting.com.number);

    char * sMapName = "backup.map";
    char * sDatName = "backup.dat";
    AnsiString pMapFile;
    AnsiString pDatFile;
    int ret = 0;


    //Update UI
    prefix = "NV Bk ";
    SetBackupProgressColor();
    SyncUpdateProgressPercentage(0);
    SyncInitProgress();

    AnsiString parent = m_sDnParam.as_working_dir + BkRsParentFolderName;
    if( -1 == CheckBkRsParentFolder(parent.c_str()) )
    {
        return -1;
    }
    SyncUpdateProgressPercentage(10);


    ret = PrepareMETAOperation();
    if(0 != ret)
    {
        return ret;
    }


    if(OP_COM_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        //Check backup folder
        AnsiString as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + IntToStr(m_sComPortSetting.com.number);
        pMapFile = as_backup_dir + "\\" + sMapName;
        pDatFile = as_backup_dir + "\\" + sDatName;

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            GC_TRACE("[COM%d] DirectoryExists(), same COM name has existed!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: There is an backup folder existed has same COM name!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        else
        {
            if( !::CreateDirectory(as_backup_dir.c_str(),NULL) )
            {
                GC_TRACE("[COM%d] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
                                                                                      as_backup_dir.c_str(), ::GetLastError());
                m_asMsg = "ERROR: Create NVRAM backup folder fail!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        SyncUpdateProgressPercentage(80);
        SyncUpdateTextHint(as_backup_dir.c_str());

    }
    else if(OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        //META get chip ID
        unsigned char chipID[20] = {0};
        unsigned int ms_timeout = 10000;
        char strRID[255];
        memset(strRID, 0, 255);

        GC_TRACE("[COM%d] Start SP_META_GetChipID_r...", m_sComPortSetting.com.number);
        ret = SP_META_GetChipID_r(m_sDnParam.i_meta_handle, ms_timeout, chipID);
        if(META_SUCCESS != ret)
        {
            GC_TRACE("[COM%d] SP_META_GetChipID_r fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get chip ID fail!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return ret;
        }
        ConvertRIDtoString(chipID, RIDLength, strRID);
        GC_TRACE("[COM%d] Complete SP_META_GetChipID_r, ChipID: %s", m_sComPortSetting.com.number, strRID);


        //Check backup folder
        AnsiString as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + strRID;
        pMapFile = as_backup_dir + "\\" + sMapName;
        pDatFile = as_backup_dir + "\\" + sDatName;

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            GC_TRACE("[COM%d] DirectoryExists(), same RID has existed!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: There is an backup folder existed named as same RID!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        else
        {
            if( !::CreateDirectory(as_backup_dir.c_str(),NULL) )
            {
                GC_TRACE("[COM%d] CreateDirectory() fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Create NVRAM backup folder fail!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        SyncUpdateProgressPercentage(80);
        SyncUpdateTextHint(as_backup_dir.c_str());

    }


    //META NVRAM backup
    GC_TRACE("[COM%d] Check NVRAM_BACKUP.INI...", m_sComPortSetting.com.number);
    NvramBackupMap nvramBackupMap("NVRAM_BACKUP.INI");
    nvramBackupMap.UpdateNvramBackupINIStatus();
    if(nvramBackupMap.IsReadNvramBackupINISuccess())
    {
        string strNvramBackupFliterNames = nvramBackupMap.GetNvramBackupFilterFloderNames();
        int delCount = nvramBackupMap.GetFilterFloderCount();
        unsigned int ms_timeout = 10000;

        GC_TRACE("[COM%d] ===================================", m_sComPortSetting.com.number);
        GC_TRACE("[COM%d] META_handle: 0x%x", m_sComPortSetting.com.number, m_sDnParam.i_meta_handle);
        GC_TRACE("[COM%d] Timeout: %d", m_sComPortSetting.com.number, ms_timeout);
        GC_TRACE("[COM%d] MapFile: %s", m_sComPortSetting.com.number, pMapFile.c_str());
        GC_TRACE("[COM%d] DatFile: %s", m_sComPortSetting.com.number, pDatFile.c_str());
        GC_TRACE("[COM%d] FliterNames: %s", m_sComPortSetting.com.number, strNvramBackupFliterNames.c_str());
        GC_TRACE("[COM%d] Mode: %d", m_sComPortSetting.com.number, delCount);
        GC_TRACE("[COM%d] ===================================", m_sComPortSetting.com.number);

        GC_TRACE("[COM%d] Start SP_META_Nvram_Backup_Custom_r...", m_sComPortSetting.com.number);
        ret = SP_META_Nvram_Backup_Custom_r(m_sDnParam.i_meta_handle, ms_timeout,
	    			                                pMapFile.c_str(), pDatFile.c_str(),
	    			                                strNvramBackupFliterNames.c_str(),
                                                                delCount, 0);
        if(META_SUCCESS != ret)
        {
            GC_TRACE("[COM%d] SP_META_Nvram_Backup_Custom_r() fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: META NVRAM backup fail!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return ret;
        }
        SyncUpdateProgressPercentage(100);
        GC_TRACE("[COM%d] Complete SP_META_Nvram_Backup_Custom_r!", m_sComPortSetting.com.number);
    }
    else
    {
        GC_TRACE("[COM%d] Read NVRAMBackup ini file fail!", m_sComPortSetting.com.number);
        m_asMsg = "Read NVRAMBackup ini file fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return -1;
    }

    ret = TerminateMETAOperation();

    return ret;
}

int _fastcall TBOOT1::RestoreNVRAMData()
{
    GC_TRACE("[COM%d] Start restore NVRAM data", m_sComPortSetting.com.number);

    char * sMapName = "backup.map";
    char * sDatName = "backup.dat";
    AnsiString pMapFile;
    AnsiString pDatFile;
    AnsiString as_backup_dir;

    int ret = 0;

    //Update UI
    prefix = "NV Rs ";
    SetRestoreProgressColor();
    SyncUpdateProgressPercentage(0);
    SyncInitProgress();

    AnsiString parent = m_sDnParam.as_working_dir + BkRsParentFolderName;
    if( -1 == CheckBkRsParentFolder(parent.c_str()) )
    {
        return -1;
    }
    SyncUpdateProgressPercentage(10);



    ret = PrepareMETAOperation();
    if(0 != ret)
    {
        return ret;
    }



    if(OP_COM_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        //Check backup folder
        as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + IntToStr(m_sComPortSetting.com.number);
        pMapFile = as_backup_dir + "\\" + sMapName;
        pDatFile = as_backup_dir + "\\" + sDatName;

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            if( !CheckBackupDataFileExisted(as_backup_dir.c_str()) )
            {
                GC_TRACE("[COM%d] CheckBackupDataFileExisted() fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Backup data files are not found!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        else
        {
            GC_TRACE("[COM%d] Backup folder is not found!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Backup folder is not found!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        SyncUpdateProgressPercentage(80);
        SyncUpdateTextHint(as_backup_dir.c_str());

    }
    else if(OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        //META get chip ID
        unsigned char chipID[20] = {0};
        unsigned int ms_timeout = 10000;
        char strRID[255];
        memset(strRID, 0, 255);

        GC_TRACE("[COM%d] Start SP_META_GetChipID_r...", m_sComPortSetting.com.number);
        ret = SP_META_GetChipID_r(m_sDnParam.i_meta_handle, ms_timeout, chipID);
        if(META_SUCCESS != ret)
        {
            GC_TRACE("[COM%d] SP_META_GetChipID_r fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get chip ID fail!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return ret;
        }
        ConvertRIDtoString(chipID, RIDLength, strRID);
        GC_TRACE("[COM%d] Complete SP_META_GetChipID_r, ChipID: %s", m_sComPortSetting.com.number, strRID);



        //Check backup folder
        as_backup_dir = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + strRID;
        pMapFile = as_backup_dir + "\\" + sMapName;
        pDatFile = as_backup_dir + "\\" + sDatName;

        if ( ::DirectoryExists(as_backup_dir.c_str()) )
        {
            if( !CheckBackupDataFileExisted(as_backup_dir.c_str()) )
            {
                GC_TRACE("[COM%d] CheckBackupDataFileExisted() fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Backup data files are not found!";
                ExitMETAMode(m_sDnParam.i_meta_handle);
                return -1;
            }
        }
        else
        {
            GC_TRACE("[COM%d] Backup folder is not found!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Backup folder is not found!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        SyncUpdateProgressPercentage(80);
        SyncUpdateTextHint(as_backup_dir.c_str());

    }



    //META NVRAM restore
    unsigned int ms_timeout=60000; //60s
    GC_TRACE("[COM%d] Start SP_META_Nvram_Restore_r()...", m_sComPortSetting.com.number);
    ret = SP_META_Nvram_Restore_r(m_sDnParam.i_meta_handle, ms_timeout, pMapFile.c_str(), pDatFile.c_str());
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_Nvram_Restore_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: META NVRAM restore fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    SyncUpdateProgressPercentage(90);
    GC_TRACE("[COM%d] Complete SP_META_Nvram_Restore_r().", m_sComPortSetting.com.number);


    //Backup NVRAM to BinRegion
    GC_TRACE("[COM%d] Start SP_META_SetCleanBootFlag_r()...", m_sComPortSetting.com.number);
    SetCleanBootFlag_REQ req;
    SetCleanBootFlag_CNF cnf;
    ret = SP_META_SetCleanBootFlag_r(m_sDnParam.i_meta_handle, ms_timeout, &req, &cnf);
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_SetCleanBootFlag_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Set clean boot flag fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    SyncUpdateProgressPercentage(100);
    GC_TRACE("[COM%d] Complete SP_META_SetCleanBootFlag_r().", m_sComPortSetting.com.number);



    //Remove backup folder
    if( (OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName) &&
                                                                   ( !m_sDnParam.b_BkRs_deletion)   )
    {
        GC_TRACE("[COM%d] After restore, retain backup data.", m_sComPortSetting.com.number);
    }
    else
    {
        GC_TRACE("[COM%d] After restore, remove backup data.", m_sComPortSetting.com.number);
        if( !RemoveAllFilesInFolder(as_backup_dir.c_str()) )
        {
            GC_TRACE("[COM%d] RemoveAllFilesInFolder() fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Remove backup folder fail!";
            ExitMETAMode(m_sDnParam.i_meta_handle);
            return -1;
        }
        SyncUpdateTextHint("");
    }


    ExitMETAMode(m_sDnParam.i_meta_handle);

    return ret;
}

void TBOOT1::SetBackupProgressColor()
{
    _bk_col =  (TColor) (clWhite);
    _ft_col =  (TColor) (0x00007F9E);
    Synchronize(UpdateProgressColor);
}

void TBOOT1::SetRestoreProgressColor()
{
    _bk_col =  (TColor) (clWhite);
    _ft_col =  (TColor) (0x00005381);
    Synchronize(UpdateProgressColor);
}

EBOOT_RESULT TBOOT1::EnterIntoMETAMode()
{
     int index = m_sDnParam.iCOMIndex;

     EBOOT_RESULT EBOOT_Result = EBOOT_EXCEPTION;
     Meta_Boot_Arg meta_arg;
     meta_arg.read_retry_time = 0;		  // 0 means use default value 2400 times
     meta_arg.read_interval_timeout = 0;   // 0 means use default value 10 ms
     meta_arg.m_p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);

     // Try to connect into META mode 10 times
     for(int i=0; i < 10 ; i++)
     {
         if( BOOT_STOP == (*(meta_arg.m_p_stopflag)) )
         {
              EBOOT_Result = USER_PRESS_STOP_BUTTON;
              break;
         }
         else
         {
              EBOOT_Result = SP_BootAsMeta(m_sComPortSetting.com.number, BAUD115200, meta_arg);	// Eboot use baud 115200 to sync with target
              if(EBOOT_SUCCESS == EBOOT_Result)
              {
                  break;
              }
         }
     }

     return EBOOT_Result;
}

void TBOOT1::ExitMETAMode( int meta_handle )
{
    //Meta operations fail, change processing hint
    GC_TRACE("[COM%d] Start exit META mode...", m_sComPortSetting.com.number);
    SP_META_ShutDownTarget_r(meta_handle);
    SP_META_DisconnectWithTarget_r(meta_handle);
    SP_META_Deinit_r(&meta_handle);
    GC_TRACE("[COM%d] Complete exit META mode!", m_sComPortSetting.com.number);
}

int  TBOOT1::ConnectInMetaModeWrapper(const int meta_handle)
{
    int index = GetCOMPortIndex();
    int * stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);

    WM_META_ConnectInMETA_Req     WM_META_ConnectInMETA_Req;
    WM_META_ConnectInMETA_Report  WM_META_ConnectInMETA_Report;

    // fill META Connet with Target paprameter
    memset(&WM_META_ConnectInMETA_Req,    0, sizeof(WM_META_ConnectInMETA_Req));
    memset(&WM_META_ConnectInMETA_Report, 0, sizeof(WM_META_ConnectInMETA_Report));
    WM_META_ConnectInMETA_Req.com_port = m_sComPortSetting.com.number;

    // set baud rate = AUTO
    WM_META_ConnectInMETA_Req.baudrate[0] = META_BAUD57600;
    WM_META_ConnectInMETA_Req.baudrate[1] = META_BAUD115200;
    WM_META_ConnectInMETA_Req.baudrate[2] = META_BAUD230400;
    WM_META_ConnectInMETA_Req.baudrate[3] = META_BAUD460800;
    WM_META_ConnectInMETA_Req.baudrate[4] = META_BAUD921600;
    WM_META_ConnectInMETA_Req.baudrate[5] = META_BAUD1500000;
    WM_META_ConnectInMETA_Req.baudrate[6] = META_BAUD_END;
    WM_META_ConnectInMETA_Req.flowctrl = META_NO_FLOWCTRL;

    WM_META_ConnectInMETA_Req.ms_connect_timeout = 40000;
    return SP_META_ConnectInMetaMode_r(meta_handle,&WM_META_ConnectInMETA_Req, stopflag, &WM_META_ConnectInMETA_Report);
}

bool TBOOT1::RemoveAllFilesInFolder(char * path)
{
    char fullName[255];
    ::WIN32_FIND_DATA FindFileData ={0};

    AnsiString findStr = path;
    findStr += "\\*.*";

    HANDLE hFind = ::FindFirstFile(findStr.c_str(), &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (true)
        {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (FindFileData.cFileName[0] != '.')
                {
                    ::strcpy(fullName, path);
                    ::strcat(fullName, "\\");
                    ::strcat(fullName, FindFileData.cFileName);
                    GC_TRACE("[COM%d] RemoveAllFilesInFolder(): Enter into folder %s", m_sComPortSetting.com.number, fullName);
                    RemoveAllFilesInFolder(fullName);
                }
            }
            else
            {
                ::strcpy(fullName, path);
                ::strcat(fullName, "\\");
                ::strcat(fullName, FindFileData.cFileName);
                if( !::DeleteFile(fullName) )
                {
                    GC_TRACE("[COM%d] RemoveAllFilesInFolder(): DeleteFile fail!, error code %d", m_sComPortSetting.com.number, ::GetLastError());
                    return false;
                }
                GC_TRACE("[COM%d] RemoveAllFilesInFolder(): <File> %s is deleted.", m_sComPortSetting.com.number, fullName);
            }

            if (!::FindNextFile(hFind, &FindFileData))
                break;
        }
        ::FindClose(hFind);

        if( !::RemoveDirectory(path) )
        {
            GC_TRACE("[COM%d] RemoveAllFilesInFolder(): DeleteFile fail!, error code %d", m_sComPortSetting.com.number, ::GetLastError());
            return false;
        }
        GC_TRACE("[COM%d] RemoveAllFilesInFolder(): <Directory> %s is deleted.", m_sComPortSetting.com.number, path);

        return true;
    }
    else
    {
        GC_TRACE("[COM%d] RemoveAllFilesInFolder(): FindFirstFile fail!", m_sComPortSetting.com.number);
        return false;
    }
}

int TBOOT1::PrepareMETAOperation(void)
{
    int ret = 0;

    //Enter META mode
    GC_TRACE("[COM%d] Boot as META mode...", m_sComPortSetting.com.number);
    ret = EnterIntoMETAMode();
    if( EBOOT_SUCCESS != ret )
    {
        GC_TRACE("[COM%d] EnterIntoMETAMode() fail!!!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Enter META mode fail!";
        Synchronize(ShowMsg);
        return -1;
    }
    SyncUpdateProgressPercentage(20);
    GC_TRACE("[COM%d] Enter META mode successfully!", m_sComPortSetting.com.number);


    //Get META handle
    GC_TRACE("[COM%d] Start SP_META_GetAvailableHandle...", m_sComPortSetting.com.number);
    ret = SP_META_GetAvailableHandle( &(m_sDnParam.i_meta_handle) );
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_GetAvailableHandle() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: META get available handle fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    SyncUpdateProgressPercentage(30);
    GC_TRACE("[COM%d] Complete SP_META_GetAvailableHandle.", m_sComPortSetting.com.number);



    //META init
    GC_TRACE("[COM%d] Start SP_META_Init_r...", m_sComPortSetting.com.number);
    ret = SP_META_Init_r(m_sDnParam.i_meta_handle, NULL);
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_Init_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: META init fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    SyncUpdateProgressPercentage(40);
    GC_TRACE("[COM%d] Complete SP_META_Init_r.", m_sComPortSetting.com.number);



    //META connect
    GC_TRACE("[COM%d] Start SP_META_ConnectInMetaMode_r...", m_sComPortSetting.com.number);
    ret = ConnectInMetaModeWrapper( m_sDnParam.i_meta_handle);
    if( META_SUCCESS != ret )
    {
        GC_TRACE("[COM%d] SP_META_ConnectInMetaMode_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Connect in META mode fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    SyncUpdateProgressPercentage(60);
    GC_TRACE("[COM%d] Complete SP_META_ConnectInMetaMode_r.", m_sComPortSetting.com.number);

    return ret;
}

int TBOOT1::TerminateMETAOperation(void)
{
    int ret = 0;

    //The second parameter is delay time for reboot, the unit must second.
    GC_TRACE("[COM%d] Start META reboot and disconnect...", m_sComPortSetting.com.number);
    ret = SP_META_RebootTarget_r(m_sDnParam.i_meta_handle, 5);
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_RebootTarget_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: META reboot target fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }

    ret = SP_META_DisconnectWithTarget_r(m_sDnParam.i_meta_handle);
    if(META_SUCCESS != ret)
    {
        GC_TRACE("[COM%d] SP_META_DisconnectWithTarget_r() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: META disconnect with target fail!";
        ExitMETAMode(m_sDnParam.i_meta_handle);
        return ret;
    }
    GC_TRACE("[COM%d] Complete META reboot and disconnect.", m_sComPortSetting.com.number);

    return 0;
}

bool TBOOT1::CheckBackupDataFileExisted(char * path)
{
    ::WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    AnsiString dat = path;
    dat += "\\backup.dat";
    hFind = ::FindFirstFile(dat.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    ::FindClose(hFind);

    AnsiString map = path;
    map += "\\backup.map";
    hFind = ::FindFirstFile(map.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    ::FindClose(hFind);

    return true;

}

int TBOOT1::CheckBkRsParentFolder(char * parentName)
{
    if ( !::DirectoryExists(parentName) )
    {
        if( !::CreateDirectory(parentName, NULL) )
        {
            GC_TRACE("[COM%d] CheckBkRsParentFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
                                                                                  parentName, ::GetLastError());
            m_asMsg = "ERROR: Create parent folder fail!";
            return -1;
        }
    }

    return 0;
}

void TBOOT1::ConvertRIDtoString(unsigned char * raw, unsigned int len, char * str)
{
    std::ostringstream oss;
    oss<<std::hex;
    for(unsigned int i=0 ; i < len; ++i){
        oss<<std::setfill('0')<<std::setw(2)<<static_cast<unsigned int>(raw[i]);
    }
    string chipName = oss.str();
    memcpy(str, chipName.c_str(), chipName.length());
}

int TBOOT1::CheckBackupFolderBeforeBackup()
{
    int iRet = S_DONE;
    AnsiString parent = m_sDnParam.as_working_dir + BkRsParentFolderName;
	AnsiString strBackupFolder;

	if(CreateFolder(parent)==false)
	{
	   return -1;
	}

	GetBackupFolderName(strBackupFolder);

	if(CreateFolder(strBackupFolder) == false)
	{
	   return -1;
	}
	
	return iRet;
}


int TBOOT1::CheckBackupFolder(AnsiString &backupPath)
{
    GC_TRACE("[COM%d] CheckBackupFolder(): Start checking...", m_sComPortSetting.com.number);

    //Step1 Check parent backup folder
    AnsiString parent = m_sDnParam.as_working_dir + BkRsParentFolderName;
    if ( !::DirectoryExists(parent.c_str()) )
    {
        //Create "backup_and_restore_data" floder
        if( !::CreateDirectory(parent.c_str(), NULL) )
        {
            GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
                                                                                  parent.c_str(), ::GetLastError());
            m_asMsg = "ERROR: Create parent backup folder fail!";
            return -1;
        }
    }

    //Step2 Check and create backup folder for doing backup or restore phone
    if(OP_COM_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {   //Step2.1 use comport as folder name
        backupPath = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + IntToStr(m_sComPortSetting.com.number);
    }
    else if(OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        char strRID[255];
        memset(strRID, 0, 255);
        //Step2.2 use RID as folder name
        if(NULL != m_sConnResult.m_da_report.m_random_id)
        {
              ConvertRIDtoString(m_sConnResult.m_da_report.m_random_id, RIDLength, strRID);
        }

        backupPath = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + strRID;
    }
    GC_TRACE("[COM%d] CheckBackupFolder(): Backup folder: %s", m_sComPortSetting.com.number, backupPath.c_str());

    //Step3 check whether the BinRegion.img and productInfo exist
    if ( ::DirectoryExists(backupPath.c_str()) )
    {
        //Step3.1 check BinRegion
        if( !CheckBinRegionFileExisted(backupPath.c_str()) )
        {
            GC_TRACE("[COM%d] CheckBackupFolder(): BinRegion data is not found in the backup folder.", m_sComPortSetting.com.number);
            SyncUpdateTextHint(backupPath.c_str());
            return 0;
        }

         //Step3.2 check produnct Info BinRegion
       // if (MainForm->hasProductInfo||MainForm->hasProtectf||MainForm->hasProtects){
         if(m_protected_image.bHasProInfo_gpt || m_protected_image.bHasProtect1 || m_protected_image.bHasProtect2
		 	|| m_protected_image.bHasProtect_F || m_protected_image.bHasProtect_S ||m_protected_image.bHasPro_Info){
            GC_TRACE("[COM%d] CheckBackupFolder(): product info and protectf and protecs.", m_sComPortSetting.com.number);
            if( !CheckProductInfoFileExisted(backupPath.c_str())){
                GC_TRACE("[COM%d] CheckBackupFolder(): ProductInfo data is not found in the backup folder.", m_sComPortSetting.com.number);
                SyncUpdateTextHint(backupPath.c_str());
                return 0;
            }
         }

		    //check protect_region.ini is existed
        GC_TRACE("[COM%d] CheckBackupFolder(): BinRegion data has been found in the backup folder!", m_sComPortSetting.com.number);
    }
    else
    {
        GC_TRACE("[COM%d] CheckBackupFolder(): Backup folder is not found, create new one.", m_sComPortSetting.com.number);
        if( !::CreateDirectory(backupPath.c_str(), NULL) )
        {
            GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
                                                                                  backupPath.c_str(), ::GetLastError());
            m_asMsg = "ERROR: Create backup folder fail!";
            return -1;
        }

        SyncUpdateTextHint(backupPath.c_str());
        return 0;
    }

    //Step4 Calculate BinRegion(nvram) checksum and checking
    AnsiString cksmPath = backupPath + "\\" + BkRsCksmFileName;
    AnsiString dataPath = backupPath + "\\" + BkRsDataFileName;

    unsigned char oriCksm = 0;
    unsigned char actCksm = 0;

    if(LoadCheckSum(cksmPath, oriCksm) && ComputeCheckSum(dataPath, actCksm) && (oriCksm == actCksm))
    {
        GC_TRACE("[COM%d] CheckBackupFolder(): Checksum verification pass!", m_sComPortSetting.com.number);
    }
    else
    {
        GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] Load or verify checksum fail! OriCksm(%d), ActCksm(%d)", m_sComPortSetting.com.number, oriCksm, actCksm);
        RemoveAllFilesInFolder(backupPath.c_str());
        if( !::CreateDirectory(backupPath.c_str(), NULL) )
        {
            GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
                                                                                  backupPath.c_str(), ::GetLastError());
            m_asMsg = "ERROR: Create backup folder fail!";
            return -1;
        }

        SyncUpdateTextHint(backupPath.c_str());
        return 0;
    }

    //Step5 Calculate protectInfo(nvram) checksum and checking
    if (MainForm->hasProductInfo||MainForm->hasProtectf||MainForm->hasProtects)
    {
        PART_INFO ProductInfoPartInfo;
        std::string backupfolderPath = backupPath.c_str();
	    BackupConfig backupConfig(backupfolderPath);
        BackupItem backupItem;
        std::list<std::string> backupItemTable;

        GC_TRACE("[COM%d] BackupBinRegionData(): backupConfig(backupfolderPath) (%s)!", m_sComPortSetting.com.number, backupPath.c_str());
        ret = backupConfig.InitBackupInfoTable();
        backupItemTable = backupConfig.backup_partitions();
	    if(ret != S_DONE )
        {
            GC_TRACE("Initial Backup Setting Table fail!");
	          return ret;
        }

        for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
        {
            GC_TRACE("[COM%d] BackupBinRegionData(): for ok!",m_sComPortSetting.com.number);
            backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
            GC_TRACE("[COM%d] partition_name(%s), file_path(%s), chksum_file(%s)",m_sComPortSetting.com.number,backupItem.partition_name().c_str(), backupItem.file_path().c_str(),backupItem.chksum_file().c_str());

            AnsiString ProductInfoDataPath = AnsiString (backupItem.file_path().c_str());
            AnsiString ProductInfoCksmPath =  AnsiString (backupItem.chksum_file().c_str());

            unsigned char ProductInfoOriCksm = 0;
            unsigned char ProductInfoActCksm = 0;

            if(LoadCheckSum(ProductInfoCksmPath, ProductInfoOriCksm) && ComputeCheckSum(ProductInfoDataPath, ProductInfoActCksm) && (ProductInfoOriCksm == ProductInfoActCksm))
            {
                GC_TRACE("[COM%d] CheckBackupFolder(): Product info Checksum verification pass!", m_sComPortSetting.com.number);
            }
            else
            {
                GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] Load or verify Product info checksum fail! OriCksm(%d), ActCksm(%d)", m_sComPortSetting.com.number, ProductInfoOriCksm, ProductInfoActCksm);
                RemoveAllFilesInFolder(backupPath.c_str());
                if( !::CreateDirectory(backupPath.c_str(), NULL) )
                {
                    GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d", m_sComPortSetting.com.number,
																					  backupPath.c_str(), ::GetLastError());
                    m_asMsg = "ERROR: Create backup folder fail!";
                    return -1;
                }

                SyncUpdateTextHint(backupPath.c_str());
                return 0;
            }
        }
    }
    SyncUpdateTextHint(backupPath.c_str());
    return 1;
}

bool TBOOT1::CheckBinRegionFileExisted(const char * path)
{
    ::WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    AnsiString dat = path;

    dat += "\\BinRegion.img";
    hFind = ::FindFirstFile(dat.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    ::FindClose(hFind);

    return true;
}

bool TBOOT1::FindStr(const char *pSrcStr, const char *pDstStr )
{
    string strSrc = pSrcStr;
    string strDst = pDstStr;
    unsigned iFound;

    if(strDst.empty() == true)
    {
        return false;
    }

    //transfer the string to lower case first
    transform(strSrc.begin(),strSrc.end(),strSrc.begin(),tolower);
	transform(strDst.begin(),strDst.end(),strDst.begin(),tolower);

 //  GC_TRACE("[COM%d] FindStr(): strSrc(%s) and strDst(%s).", m_sComPortSetting.com.number,strSrc.c_str(), strDst.c_str());

    iFound = strSrc.find(strDst);
    if(iFound != string::npos){
       return true;
    }
    else{
       return false;
    }
}


bool TBOOT1::FindPartName(string strSrc, vector<string> vctDstStr )
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

void TBOOT1::InitVectorForFindPartName(string strName, vector<string> &vctDstTemp )
{
    if( strName.compare(SCATTER_FAT_NAME) == 0 ||strName.compare("FAT") == 0)  //for FAT
	{
	    vctDstTemp.push_back("FAT");
		vctDstTemp.push_back(SCATTER_FAT_NAME);
	}
	else if(strName.compare(SCATTER_PRELOADER_NAME) == 0 || strName.compare("preloader") == 0)// For
	{
	    vctDstTemp.push_back(SCATTER_PRELOADER_NAME);
		vctDstTemp.push_back("preloader");
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
	else if(strName.compare(SCATTER_SECCFG_NAME) == 0 || strName.compare("SECCFG") == 0)
	{
	    vctDstTemp.push_back(SCATTER_SECCFG_NAME);
		vctDstTemp.push_back("SECCFG");
	}
	else if(strName.compare(SCATTER_EXPDB_NAME) == 0 || strName.compare("EXPDB") == 0)
	{
	    vctDstTemp.push_back(SCATTER_EXPDB_NAME);
		vctDstTemp.push_back("EXPDB");
	}
	else if(strName.compare(SCATTER_MISC_NAME) == 0 || strName.compare("MISC") == 0)
	{
	    vctDstTemp.push_back(SCATTER_MISC_NAME);
		vctDstTemp.push_back("MISC");
	}
	else
	{
	    vctDstTemp.push_back(strName); 
	}
	
}


bool TBOOT1::CheckProductInfoFileExisted(const char * path)
{
    ::WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    PART_INFO ProductInfoPartInfo;
    GC_TRACE("backupfolderPath is ok");
    //std::string backupfolderPath = path;
    //AnsiString backupfolderPath = path;
    //GC_TRACE("backupfolderPath is %s",backupfolderPath.c_str());

    std::string backupfolderPath;
    backupfolderPath.assign(path);

    BackupConfig backupConfig(backupfolderPath);
    GC_TRACE("backupfolderPath is ok 2 %s",backupfolderPath);
    BackupItem backupItem;
    std::list<std::string> backupItemTable;

    ret = backupConfig.InitBackupInfoTable();
    backupItemTable = backupConfig.backup_partitions();
    if(ret != S_DONE )
    {
        GC_TRACE("Initial Backup Setting Table fail!");
	    return false;
    }

    for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
    {
        GC_TRACE("[COM%d] CheckProductInfoFileExisted(): for ok!",m_sComPortSetting.com.number);
        backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
        GC_TRACE("[COM%d]  CheckProductInfoFileExisted(): partition_name(%s), file_path(%s), chksum_file(%s)",m_sComPortSetting.com.number,backupItem.partition_name().c_str(), backupItem.file_path().c_str(),backupItem.chksum_file().c_str());

        //AnsiString dat = AnsiString (backupItem.file_path().c_str());
        hFind = ::FindFirstFile(backupItem.file_path().c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        ::FindClose(hFind);
    }

    return true;
}

int TBOOT1::BackupBinRegionData(void)
{
    int ret = S_DONE;
    RB_INFO rb_info;
    RB_INFO ProductInfo_rb_info;
    AnsiString backupPath;
	
	GC_TRACE("[COM%d] BackupBinRegionData(): Enter...", m_sComPortSetting.com.number);

    if(CheckBackupFolderBeforeBackup() != S_DONE){
		GC_TRACE("[COM%d] BackupBinRegionData(): CheckBackupFolderBeforeBackup fail", m_sComPortSetting.com.number);
		return -1;
	}

    GetBackupFolderName(backupPath);
    GC_TRACE("[COM%d] BackupBinRegionData(): CheckBackupFolder(backupPath): %s", m_sComPortSetting.com.number, backupPath.c_str());

	
   /* if(-1 == retCheck)
    {
        GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Check backup folder fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Check backup folder fail!";
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return -1;
    }
    else if(0 == retCheck)
    {*/

   /*
    ret = IsHaveProtectRegion();
   
    if(ret != S_DONE)
    {
		GC_TRACE("[COM%d] BackupBinRegionData(): IsHaveProtectRegion check fail.", m_sComPortSetting.com.number);
        return ret;
    }*/
    
    //begin to do backup bin
   	{
		//NVRAM: Get BinRegion Partition info
		ROM_INFO romInfo;
        PART_INFO partInfo;
        ret = GetPartitionInfo(SCATTER_NVRAM_NAME, partInfo);  // 20130408,need modify
	    if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Getting readback information fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get BinRegion Partition info fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }

        rb_info.part_id = partInfo.part_id;
        rb_info.readback_addr = partInfo.begin_addr;
        rb_info.readback_len = partInfo.image_length;
        //set m_read_flag of RB_INFO for image of Yaffs format
        ret = GetROMInfo(SCATTER_NVRAM_NAME, romInfo);
        if(ret != S_DONE){
            GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Get RomInfo of Nvram bin fail before readback nvram bin!", m_sComPortSetting.com.number);
            return -1;
        }
        GC_TRACE("[COM%d] BackupBinRegionData(): readback image name: %s!", m_sComPortSetting.com.number,romInfo.name);
        if(NORMAL_ROM == romInfo.rom_type || romInfo.rom_type == EXT4_IMG){
            rb_info.m_read_flag = NUTL_READ_PAGE_ONLY;
            GC_TRACE("[COM%d] BackupBinRegionData(): rom_type of readback is normal image", m_sComPortSetting.com.number);
        }else if(YAFFS_IMG == romInfo.rom_type){
            rb_info.m_read_flag = NUTL_READ_PAGE_SPARE_WITH_ECCDECODE;
            GC_TRACE("[COM%d] BackupBinRegionData(): rom_type of readback is yaffs image", m_sComPortSetting.com.number);
        }else{
            GC_TRACE("[COM%d] BackupBinRegionData() ERROR: rom_type of readback is not normal image or yaffs image", m_sComPortSetting.com.number);
            return -1;
        }

        //Readback BinRegion data
        ret = ReadbackBinRegion(rb_info, backupPath, BkRsDataFileName, BkRsCksmFileName);
        GC_TRACE("[COM%d] BackupBinRegionData(): Backup folder: %s", m_sComPortSetting.com.number, backupPath.c_str());
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Readback BinRegion data fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Readback BinRegion data fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }


		//backup product info/protect_s/protect_f
        PART_INFO ProductInfoPartInfo;
        ROM_INFO ProductInfoRomInfo;
        std::string backupfolderPath = backupPath.c_str();
        BackupConfig backupConfig(backupfolderPath);
        BackupItem backupItem;
        std::list<std::string> backupItemTable;

        GC_TRACE("[COM%d] BackupBinRegionData(): backupConfig(backupfolderPath) (%s)!", m_sComPortSetting.com.number, backupPath.c_str());
        ret = backupConfig.InitBackupInfoTable();
        backupItemTable = backupConfig.backup_partitions();
        if(ret != S_DONE )
        {
            GC_TRACE("Initial Backup Setting Table fail!");
            return ret;
        }

        for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
        {
            //GC_TRACE("[COM%d] BackupBinRegionData(): for ok!",m_sComPortSetting.com.number);
            backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
            GC_TRACE("[COM%d] BackupBinRegionData(): partition_name(%s), file_path(%s), chksum_file(%s)",m_sComPortSetting.com.number,backupItem.partition_name().c_str(), backupItem.file_path().c_str(),backupItem.chksum_file().c_str());
            char partitionName[100];
            char partitionFileName[100];
            char partitionchksumName[100];

            strcpy(partitionName,backupItem.partition_name().c_str());

            GC_TRACE("[COM%d] BackupBinRegionData(): partition_name(%s)",m_sComPortSetting.com.number,partitionName);
            std::string partition_FileName=backupItem.file_path();
            partition_FileName.erase(0,backupPath.Length()+1);
            strcpy(partitionFileName,partition_FileName.c_str());
            GC_TRACE("[COM%d] BackupBinRegionData(): partition_FileName(%s)",m_sComPortSetting.com.number,partitionFileName);
            std::string partition_chksumName=backupItem.chksum_file();
            partition_chksumName.erase(0,backupPath.Length()+1);
            strcpy(partitionchksumName,partition_chksumName.c_str());
            GC_TRACE("[COM%d] BackupBinRegionData(): partition_chksumName(%s)",m_sComPortSetting.com.number,partitionchksumName);

            //ret = GetPartitionInfo(partitionName, ProductInfoPartInfo);
            ret = GetPartitionInfo(backupItem.partition_name().c_str(), ProductInfoPartInfo);
            if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] BackupBinRegionData(): [Warning] Getting readback product info information fail!", m_sComPortSetting.com.number);
            }
            else
            {
                ret = GetROMInfo(backupItem.partition_name().c_str(), ProductInfoRomInfo);
                if(ret != S_DONE){
                    GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Get RomInfo of Protect bin fail before readback Protect bin!", m_sComPortSetting.com.number);
                    return -1;
                }
                GC_TRACE("[COM%d] BackupBinRegionData(): readback image name: %s!", m_sComPortSetting.com.number,ProductInfoRomInfo.name);
                //set m_read_flag of RB_INFO for image of Yaffs format
                if(NORMAL_ROM == ProductInfoRomInfo.rom_type || ProductInfoRomInfo.rom_type == EXT4_IMG){
                    ProductInfo_rb_info.m_read_flag = NUTL_READ_PAGE_ONLY;
                    GC_TRACE("[COM%d] BackupBinRegionData(): rom_type of readback is normal image", m_sComPortSetting.com.number);
                }else if(YAFFS_IMG == ProductInfoRomInfo.rom_type){
                    ProductInfo_rb_info.m_read_flag = NUTL_READ_PAGE_SPARE_WITH_ECCDECODE;
                    GC_TRACE("[COM%d] BackupBinRegionData(): rom_type of readback is yaffs image", m_sComPortSetting.com.number);
                }else{
                    GC_TRACE("[COM%d] BackupBinRegionData() ERROR: rom_type of readback is not normal image or yaffs image", m_sComPortSetting.com.number);
                    return -1;
                }

                ProductInfo_rb_info.part_id = ProductInfoPartInfo.part_id;
                ProductInfo_rb_info.readback_addr = ProductInfoPartInfo.begin_addr;
                ProductInfo_rb_info.readback_len = ProductInfoPartInfo.image_length;
                //Readback BinRegion data
                ret = ReadbackBinRegion(ProductInfo_rb_info, backupPath, partitionFileName, partitionchksumName);
                if(S_DONE != ret)
                {
                   GC_TRACE("[COM%d] BackupBinRegionData(): [ERROR] Readback ProductInfo BinRegion data fail!", m_sComPortSetting.com.number);
                       m_asMsg = "ERROR: Readback ProductInfo BinRegion data fail!";
                   FlashTool_Disconnect(&m_ft_api_handle);
                   m_is_DA_connected = false;
                   return ret;
                }
            }
        }
   	}
       //}
       //else
       //{
       //   GC_TRACE("[COM%d] BackupBinRegionData(): check protect info fail, ret = %d from IsHaveProtectRegion", m_sComPortSetting.com.number, ret);
       //   return ret;
       //}
   /* }
    else
    {
        GC_TRACE("[COM%d] BackupBinRegionData(): BinRegion data has existed!", m_sComPortSetting.com.number);
    }

    //Set image arguments for later download
    AnsiString dataPath = backupPath + "\\" + BkRsDataFileName;
    strcpy(m_BinRegion_path, dataPath.c_str());
    m_BinRegion.m_path = m_BinRegion_path;
    m_BinRegion.is_download = true;*/
    GC_TRACE("[COM%d] BackupBinRegionData(): OK", m_sComPortSetting.com.number);
    return S_DONE;
}

/*Function: For PART_NOT_FOUND,the download and  partition restoring should be allowed,but before
            download and  partition restoring,we should check whether the partition bins in the
            backup folder exist comparing with scatter file.
History:
1. 2012/01/17 add by wx:

*/
int TBOOT1::CheckBinRegionBeforeRestore(void)
{
    AnsiString asBackupFoloderPath;
    AnsiString asTempPath1, asTempPath2;
    vector<ROM_INFO>::iterator iterRomInfo;
    std::string strTmpBackupFolder;
    AnsiString asParentPath = m_sDnParam.as_working_dir + BkRsParentFolderName;

    GC_TRACE("[COM%d] CheckBinRegionBeforeRestore():Enter...",m_sComPortSetting.com.number);
    //Step1: check whether the backup folder exist
    if(!CheckIsFoldExist(asParentPath))
    {
        GC_TRACE("[COM%d] CheckBinRegionBeforeRestore(): Backup folder do not exist: %s",
                 m_sComPortSetting.com.number, asParentPath.c_str());
        return -1;
    }

    //get backup folder path, using comport name or Rid name
    GetBackupFolderName(asBackupFoloderPath);

    //step2: check whether BinRegion exist and compute the checksum
    asTempPath1 = asBackupFoloderPath+"\\"+ BkRsDataFileName; //bindata
    asTempPath2 = asBackupFoloderPath+"\\"+ BkRsCksmFileName; //checksum file
    if(!ComputeBinfileCheckSum(asTempPath1, asTempPath2))
    {
       return -1;
    }

    //Step3: check protect info bin
    BackupConfig backupConfig(asBackupFoloderPath.c_str());
    BackupItem backupItem;
    std::list<std::string> backupItemTable;

	vector<string> vctDstTemp;
    vctDstTemp.clear();
    InitVectorForFindPartName(SCATTER_PRODUCT_INFO_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PROTECTF_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PROTECTS_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PRODUCT_INFO_FOR_GPT_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PROTECT1_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PROTECT2_NAME,vctDstTemp);
	InitVectorForFindPartName(SCATTER_PERSIST_NAME,vctDstTemp);

    ret = backupConfig.InitBackupInfoTable();
    backupItemTable = backupConfig.backup_partitions();

	for(iterRomInfo = m_vctRomInfo.begin();iterRomInfo != m_vctRomInfo.end(); iterRomInfo++)
    {
        if(FindPartName(iterRomInfo->name,  vctDstTemp) == _TRUE)
        {   
           GC_TRACE("[COM%d] CheckBinRegionBeforeRestore(): verifying %s...",m_sComPortSetting.com.number,iterRomInfo->name);
           if(!backupConfig.QueryBackupItemByPartitionName(iterRomInfo->name, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
            
        }
    }

    /*
	//Step3.1 Get Rom info from scatter file
    for(iterRomInfo = m_vctRomInfo.begin();iterRomInfo != m_vctRomInfo.end(); iterRomInfo++)
    {
       //Step3.2 Get protectinfo from protected_region.ini file
       if(FindStr(SCATTER_PRODUCT_INFO_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PRODUCT_INFO_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }
       else if(FindStr(SCATTER_PROTECTF_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PROTECTF_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }
       else if(FindStr(SCATTER_PROTECTS_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PROTECTS_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }else if(FindStr(SCATTER_PRODUCT_INFO_FOR_GPT_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PRODUCT_INFO_FOR_GPT_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }else if(FindStr(SCATTER_PROTECT1_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PROTECT1_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }else if(FindStr(SCATTER_PROTECT2_NAME, iterRomInfo->name)){
           if(!backupConfig.QueryBackupItemByPartitionName(SCATTER_PROTECT2_NAME, &backupItem))
           {
               return -1;
           }

           if(!ComputeBinfileCheckSum(backupItem.file_path().c_str(), backupItem.chksum_file().c_str()))
           {
                return -1;
           }
       }
    }*/
    GC_TRACE("[COM%d] CheckBinRegionBeforeRestore():OK!",m_sComPortSetting.com.number);
	return S_DONE;
}

bool TBOOT1::CheckIsFoldExist(AnsiString as_backup_dir)
{

    if ( !::DirectoryExists(as_backup_dir.c_str()) )
    {
        return false;
    }

    return true;
}

bool TBOOT1::CheckIsFileExist(AnsiString as_file_path)
{
    ::WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = ::FindFirstFile(as_file_path.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    ::FindClose(hFind);
    return true;
}

bool TBOOT1::ComputeBinfileCheckSum(AnsiString asBinFilePath, AnsiString asChecksumFilePath)
{
    unsigned char pChecksum1;
    unsigned char pChecksum2;
    unsigned char buffer[1024];
    unsigned int read_size = 0;
   
	GC_TRACE("[COM%d] ComputeBinfileCheckSum(): Enter checksum check, bin path = %s", m_sComPortSetting.com.number, asBinFilePath.c_str());
    //Step1: check whether file exist
    if(!CheckIsFileExist(asBinFilePath) || !CheckIsFileExist(asChecksumFilePath))
    {
         //GC_TRACE("[COM%d] ComputeBinfileCheckSum(): [ERROR] can't find checksum file fail!", m_sComPortSetting.com.number);
		 GC_TRACE("[COM%d] ComputeBinfileCheckSum(): [ERROR] can't find checksum file fail! bin path = %s", m_sComPortSetting.com.number, asBinFilePath.c_str());
         m_asMsg = "ERROR: Can't find backup data files!";
         return false;
    }

    //Step2: read checksum file value
    FILE *file = fopen( asChecksumFilePath.c_str() , "rb" );
    if (file==NULL)
    {
        GC_TRACE("[COM%d] ComputeBinfileCheckSum(): [ERROR] Open checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }


    read_size = fread(buffer, 1, 2, file);
    if(read_size <= 0)
    {
        GC_TRACE("[COM%d] ComputeBinfileCheckSum(): [ERROR] Read checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }
    pChecksum1 = buffer[0];
    GC_TRACE("[COM%d] ComputeBinfileCheckSum(): Checksum1(%08X) from checksum file.", m_sComPortSetting.com.number, pChecksum1);
    fclose (file);

    //Step3 Read bin file and compute checksum of bin file
    file = fopen( asBinFilePath.c_str() , "rb");
    if (file==NULL)
    {
        GC_TRACE("[COM%d] ComputeBinfileCheckSum(): [ERROR] Open data file fail!", m_sComPortSetting.com.number);
        return false;
    }

    pChecksum2 = 0;
    read_size = 0;
    while(!feof(file))
    {
        read_size = fread(buffer, 1, 1024, file);

        for(unsigned int i=0;i<read_size;i++)
        {
            pChecksum2 ^= buffer[i];
        }
    }

    GC_TRACE("[COM%d] ComputeBinfileCheckSum(): Checksum2(%08X) from computing bin file.", m_sComPortSetting.com.number, pChecksum2);
    fclose (file);

    //step4 compare checksum
    if(pChecksum1 == pChecksum2)
    {
        GC_TRACE("[COM%d] ComputeBinfileCheckSum(): Checksums are matched", m_sComPortSetting.com.number);
        return true;
    }
    else
    {
        GC_TRACE("[COM%d] ComputeBinfileCheckSum(): Checksums are dismatched", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Backup data files with Checksums file are dismatched!";
        return false;
    }

}

AnsiString TBOOT1::GetBackupFolderName(AnsiString &as_backup_folder)
{
    AnsiString backupPath;
    //use comport as folder name
    if(OP_COM_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
       backupPath = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + IntToStr(m_sComPortSetting.com.number);
    }
    else if(OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName)
    {
        char strRID[255];
        memset(strRID, 0, 255);
        //use RID as folder name
        if(NULL != m_sConnResult.m_da_report.m_random_id)
        {
             ConvertRIDtoString(m_sConnResult.m_da_report.m_random_id, RIDLength, strRID);
        }
        backupPath = m_sDnParam.as_working_dir + BkRsParentFolderName + "\\" + strRID;
    }
   // GC_TRACE("[COM%d] GetBackupFolderName(): Backup folder: %s", m_sComPortSetting.com.number, backupPath.c_str());
    as_backup_folder = backupPath;
    return backupPath;
}

bool TBOOT1::CreateFolder(AnsiString as_create_dir)
{
    if ( !::DirectoryExists(as_create_dir.c_str()) )
    {
        if( !::CreateDirectory(as_create_dir.c_str(), NULL) )
        {
            GC_TRACE("[COM%d] CheckBackupFolder(): [ERROR] CreateDirectory fail, path: %s, error code: %d",m_sComPortSetting.com.number,as_create_dir.c_str(), ::GetLastError());
             m_asMsg = "ERROR: Create parent backup folder fail!";
             return false;
        }
    }
    return true;
}

int TBOOT1::CheckBinRegionData(void)
{
    int ret = S_DONE;
    RB_INFO rb_info;
    AnsiString backupPath;

    ret = IsHaveProtectRegion();
	if(ret != S_DONE)
	{
	     GC_TRACE("[COM%d] CheckBinRegionData(): [ERROR] IsHaveProtectRegion fail!", m_sComPortSetting.com.number);
		 return ret;
	}

    //Check backup folder
    int retCheck;
    retCheck = CheckBackupFolder(backupPath);
    if(1 != retCheck)
    {
        GC_TRACE("[COM%d] CheckBinRegionData(): [ERROR] Check backup folder fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Check backup folder fail!";
        //FlashTool_Disconnect(&m_ft_api_handle);
        //m_is_DA_connected = false;
        return -1;
    }

    GC_TRACE("[COM%d] CheckBinRegionData(): BinRegion data has existed!", m_sComPortSetting.com.number);

    //Set image arguments for later download
    AnsiString dataPath = backupPath + "\\" + BkRsDataFileName;
    strcpy(m_BinRegion_path, dataPath.c_str());
    m_BinRegion.m_path = m_BinRegion_path;
    m_BinRegion.is_download = true;

	/*
    if(S_DONE == ret)
	{
		//Set image arguments for later download
        std::string backupfolderPath = backupPath.c_str();
	    BackupConfig backupConfig(backupfolderPath);
        BackupItem backupItem;
        std::list<std::string> backupItemTable;

        GC_TRACE("[COM%d] CheckBinRegionData(): backupConfig(backupfolderPath) (%s)!", m_sComPortSetting.com.number, backupPath.c_str());
        ret = backupConfig.InitBackupInfoTable();
        backupItemTable = backupConfig.backup_partitions();
	      if(ret != S_DONE )
        {
            GC_TRACE("Initial Backup Setting Table fail!");
	          return ret;
        }

        for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
        {
            backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
            GC_TRACE("[COM%d] CheckBinRegionData(): partition_name(%s), file_path(%s), chksum_file(%s)",
                     m_sComPortSetting.com.number,backupItem.partition_name().c_str(), backupItem.file_path().c_str(),
                     backupItem.chksum_file().c_str());
            strcpy(m_ProductInfo_path, backupItem.file_path().c_str());
            m_ProductInfo.m_path = m_ProductInfo_path;
            m_ProductInfo.is_download = true;
            GC_TRACE("[COM%d] CheckBinRegionData():m_ProductInfo_path(%s)",m_sComPortSetting.com.number, m_ProductInfo_path);
	     }
    }
	else
	{
	    return ret;
    }*/
    return ret;
}

 /*
 Funtion: check whether the backup partition size change.
 History:
 1. 2013/01/16 by wx: add this function.
 */
int TBOOT1::isPartitionSizeChange()
{
    if( PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_NVRAM_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PRODUCT_INFO_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PROTECTF_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PROTECTS_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PROTECT1_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PROTECT2_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PRODUCT_INFO_NAME, m_vctPartInfo, m_vctRomInfo)
        ||PART_SIZE_HAS_CHANGED == CheckPartitionSizeChange(SCATTER_PRODUCT_INFO_FOR_GPT_NAME, m_vctPartInfo, m_vctRomInfo))
    {
         GC_TRACE("[COM%d]Match: CheckPartitionSizeChange(): Partition Size change!",
                     m_sComPortSetting.com.number);
         return true;
    }
    else
    {
         GC_TRACE("[COM%d]Match: CheckPartitionSizeChange(): Partition Size do not change!",
                     m_sComPortSetting.com.number);
        return false;
    }
}

/*
Funtion: check whether the backup partition size change by partion name.
History:
1. 2013/01/16 by wx: add this function.
*/
/*
int TBOOT1::CheckPartitionSizeChange(const char *pPartitionName)
{
    vector<ROM_INFO>::iterator iterRomInfo;
    vector<PART_INFO>::iterator iterPartInfo;

    GC_TRACE("[COM%d]CheckPartitionSizeChange(): check part:%s !",m_sComPortSetting.com.number, pPartitionName);
    //Step1 Get Rom info from scatter file
    for(iterRomInfo = m_vctRomInfo.begin();iterRomInfo !=  m_vctRomInfo.end(); iterRomInfo++)
    {
       //if(0 == strcmpi(pPartitionName, iterRomInfo->name))
       if(FindStr(pPartitionName, iterRomInfo->name))  // modify to support new scatter file.20130408
       {
           break;
       }
    }

    for(iterPartInfo = m_vctPartInfo.begin();iterPartInfo!=m_vctPartInfo.end(); iterPartInfo++)
    {
       //if(0 == strcmpi(pPartitionName, iterPartInfo->name))
       if(FindStr(pPartitionName, iterPartInfo->name))
       {
           break;
       }
    }

    //no found
    if(iterRomInfo == m_vctRomInfo.end() && iterPartInfo==m_vctPartInfo.end()){
       GC_TRACE("[COM%d] CheckPartitionSizeChange(): Both of rominfo and partition do not exist!",m_sComPortSetting.com.number);
       return PART_SIZE_NO_CHANGED;
    }
    else if(iterRomInfo != m_vctRomInfo.end() && iterPartInfo != m_vctPartInfo.end()){
       //partition length no change
       if( iterRomInfo->partition_size == iterPartInfo->image_length){
           GC_TRACE("[COM%d] CheckPartitionSizeChange(): Size no change, partition_name(%s), OriSize( 0x%016x), NewSize( 0x%016x)",
                     m_sComPortSetting.com.number, iterPartInfo->name,iterRomInfo->partition_size, iterPartInfo->image_length);
           return PART_SIZE_NO_CHANGED;
       }
       else{
	   	   GC_TRACE("[COM%d]: debug CheckPartitionSizeChange():Size have been changed, partition_name(%s), OriSize(0x%016x), NewSize( 0x%016x)",
                     m_sComPortSetting.com.number, iterPartInfo->name,iterRomInfo->end_addr-iterRomInfo->begin_addr, iterPartInfo->image_length);
           m_asMsg = "ERROR: Original Partition size is not matched with new partition size,Please check whether both of them exist!";
           return PART_SIZE_HAS_CHANGED;
       }
    }//only found one
    else{
       GC_TRACE("[COM%d]ERROR: CheckPartitionSizeChange(): only one of rominfo and partition exists!",
                     m_sComPortSetting.com.number);
       m_asMsg = "ERROR: Original Partition size is not matched with new partition size,Please check whether both of them exist!";
       return PART_SIZE_HAS_CHANGED;
    }
}*/

int TBOOT1::CheckPartitionSizeChange(string strName, vector<PART_INFO> vctPartInfo, vector<ROM_INFO> vctRomInfo)
{
    std::vector<PART_INFO>::iterator itPartInfo;
	std::vector<ROM_INFO>::iterator itRomInfo;
    vector<string> vctDstTemp;

	vctDstTemp.clear();
	InitVectorForFindPartName(strName,vctDstTemp);
	
	for(itPartInfo = vctPartInfo.begin(); itPartInfo != vctPartInfo.end(); itPartInfo++)
	{
	   if(FindPartName(itPartInfo->name,vctDstTemp) == _TRUE)
	   {
	      break;
	   }
	}

	for(itRomInfo = vctRomInfo.begin(); itRomInfo != vctRomInfo.end(); itRomInfo++){
       if(FindPartName(itRomInfo->name,vctDstTemp) == _TRUE)
	   {
	      break;
	   }
	}

	if(itPartInfo == vctPartInfo.end() && itRomInfo == vctRomInfo.end())  //part name do not exist in both side
	{ 
	    GC_TRACE("[COM%d] CheckPartitionSizeChange(): Both of rominfo and partition do not exist!",m_sComPortSetting.com.number);
        return PART_SIZE_NO_CHANGED;
	}
	else if(itPartInfo != vctPartInfo.end() && itRomInfo != vctRomInfo.end()) //part name exist in both side
    { 
        if(itPartInfo->image_length == itRomInfo->partition_size){
			GC_TRACE("[COM%d] CheckPartitionSizeChange(): Size no change, partition_name(%s), OriSize( 0x%016x), NewSize( 0x%016x)",
                     m_sComPortSetting.com.number, itPartInfo->name,itRomInfo->partition_size, itPartInfo->image_length);
            return PART_SIZE_NO_CHANGED;
		}else{
		   GC_TRACE("[COM%d]: debug CheckPartitionSizeChange():Size have been changed, partition_name(%s), OriSize(0x%016x), NewSize( 0x%016x)",
                     m_sComPortSetting.com.number, itPartInfo->name, itRomInfo->partition_size, itPartInfo->image_length);
           m_asMsg = "ERROR: Original Partition size is not matched with new partition size,Please check whether both of them exist!";
           return PART_SIZE_HAS_CHANGED;
		}
	}else{  // the part name only exist in one side, not both sides.
	    GC_TRACE("[COM%d]ERROR: CheckPartitionSizeChange(): only one of rominfo and partition exists!  partition_name(%s)", 
        m_sComPortSetting.com.number, strName.c_str());
        m_asMsg = "ERROR: Only one of rominfo and partition exists!,Please check whether both of them exist!";
        return PART_SIZE_HAS_CHANGED;
	}
}



int TBOOT1::GetRomInfoData()
{
    int iRet = S_DONE;
	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
	unsigned int iRomsize =0, iTmp;
	ROM_INFO rom_info_all[MAX_LOAD_SECTIONS];

	m_vctRomInfo.clear();

    iRet = FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&iRomsize);
    if ( iRet != S_DONE){
		GC_TRACE("[COM%d]ERROR: GetRomInfoData(): FlashTool_RomGetCount fail!",m_sComPortSetting.com.number);
	    goto FuncEnd;
	}

	//GC_TRACE("[COM%d] GetRomInfoData():FlashTool_RomGetCount->rom count = %d!",m_sComPortSetting.com.number, iRomsize);

	iRet = FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom_info_all,iRomsize);
	if(iRet != S_DONE){
		GC_TRACE("[COM%d]ERROR: GetRomInfoData(): FlashTool_RomGetInfoAll fail!",m_sComPortSetting.com.number);
	    goto FuncEnd;
	}

    //GC_TRACE("[COM%d]GetRomInfoData(): Rom info from PC side:",m_sComPortSetting.com.number);
	for(iTmp = 0; iTmp < iRomsize; iTmp++){
		//for debug
		//GC_TRACE("[COM%d]GetRomInfoData(): name = %-16s, beginAddr =0x%x",m_sComPortSetting.com.number,rom_info_all[iTmp].name, rom_info_all[iTmp].begin_addr);
		m_vctRomInfo.push_back(rom_info_all[iTmp]);
	}

FuncEnd:
    return iRet;

}

int TBOOT1::GetPartitionInfoData()
{
    int iRet = S_DONE;
    PART_INFO partinfo;
    unsigned int iCycle = 0;
    PART_INFO * pOri = NULL;
    void ** ppOri = (void **)&pOri;
    unsigned int sizeOri = 0;
	m_vctPartInfo.clear();

    iRet = GetAllPartitionInfo(ppOri, sizeOri);
    if((S_DONE != iRet) ||(NULL == pOri) ||(0 == sizeOri))
    {
        iRet = -1;
        delete[] pOri;
        return iRet;
    }

	//GC_TRACE("[COM%d]GetPartitionInfoData(): Rom info from target side:",m_sComPortSetting.com.number);
    for(iCycle = 0; iCycle < sizeOri; iCycle++)
    {
        partinfo = pOri[iCycle];
        m_vctPartInfo.push_back(partinfo);
    }

    delete[] pOri;
    return iRet;
}



int TBOOT1::WriteBinRegionData()
{
    int ret = S_DONE;

    //=====================Write memory====================
    if(true == MainForm->m_miEnableDADL->Checked)
    {
        //Check backup folder
	    ret = CheckBinRegionBeforeRestore();
		if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] WriteBinRegionData(): [ERROR] CheckBinRegionBeforeRestore fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Check BinRegion BeforeRestore fail!";
            //FlashTool_Disconnect(&m_ft_api_handle);
            //m_is_DA_connected = false;
            return -1;
        }
		
        //Step1 Write BinRegion
        WriteFlashMemoryParameter writeArg;
        ROM_INFO romInfo;

        ret = GetROMInfo(SCATTER_NVRAM_NAME, romInfo);  // modify code order to Support new scatter file , mtk71502.20130408
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] WriteBinRegionData(): [ERROR] Get ROM info fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get ROM info fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }
        GC_TRACE("[COM%d] WriteBinRegionData(): Write back image name: %s!", m_sComPortSetting.com.number,romInfo.name);

        AnsiString asBackupFoloderPath;
        AnsiString asTempPath;
		
        GetBackupFolderName(asBackupFoloderPath);
        asTempPath = asBackupFoloderPath+"\\"+ BkRsDataFileName; //bindata
        strcpy(m_BinRegion_path, asTempPath.c_str());

        writeArg.m_flash_type = m_sDnParam.eStorageType;
        writeArg.m_addressing_mode = AddressingMode_LogicalAddress;
        writeArg.m_address = romInfo.begin_addr;
        writeArg.m_container_length = 0;
        writeArg.m_input_mode = InputMode_FromFile;
        writeArg.m_input = m_BinRegion_path;
        writeArg.m_input_length = 0;
        writeArg.m_cb_progress = cb_write_memory_progress;
        writeArg.m_cb_progress_arg = this;
		writeArg.m_part_id = romInfo.part_id;
		
        //Set m_program_mode of WriteFlashMemoryParameter for yaffs image
        if(NORMAL_ROM == romInfo.rom_type || romInfo.rom_type == EXT4_IMG){
            writeArg.m_program_mode = ProgramMode_PageOnly;
            GC_TRACE("[COM%d] WriteBinRegionData(): rom_type of writeback is normal image", m_sComPortSetting.com.number);
        }else if(YAFFS_IMG == romInfo.rom_type){
            writeArg.m_program_mode = ProgramMode_PageSpare;
             GC_TRACE("[COM%d] WriteBinRegionData(): rom_type of writeback is yaffs image", m_sComPortSetting.com.number);
        }else{
            GC_TRACE("[COM%d] WriteBinRegionData() ERROR: rom_type of writeback is not normal image or yaffs image", m_sComPortSetting.com.number);
            return -1;
        }

        GC_TRACE("[COM%d] WriteBinRegionData(): m_BinRegion_path = %s!",m_sComPortSetting.com.number,m_BinRegion_path);
        GC_TRACE("[COM%d] WriteBinRegionData(): Flashtype(%d), Addr(0x%LX), Path(%s)", m_sComPortSetting.com.number,
            writeArg.m_flash_type, writeArg.m_address, writeArg.m_input);

        cb_write_memory_init(this);

        GC_TRACE("[COM%d] WriteBinRegionData(): Write flash memory...", m_sComPortSetting.com.number);
        ret = FlashTool_WriteFlashMemory(m_ft_api_handle, &writeArg);
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] WriteBinRegionData(): [ERROR] Write flash memory fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Write flash memory fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }

		GC_TRACE("[COM%d] WriteBinRegionData(): Write bin back success! bin name = %s.", m_sComPortSetting.com.number, romInfo.name);
		
        //Step2 check protect info, if hava protect info, write back ProductInfo
        if( S_DONE == IsHaveProtectRegion())
        {
            WriteFlashMemoryParameter writeProductInfoArg;
		    ROM_INFO ProductInfoRomInfo;
		    PART_INFO ProductInfoPartInfo;
            std::string backupfolderPath = asBackupFoloderPath.c_str();
    		BackupConfig backupConfig(backupfolderPath);
            BackupItem backupItem;
            std::list<std::string> backupItemTable;
    
            GC_TRACE("[COM%d] WriteBinRegionData(): backupConfig init,backupfolderPath = %s!", m_sComPortSetting.com.number, asBackupFoloderPath.c_str());
            ret = backupConfig.InitBackupInfoTable();
            backupItemTable = backupConfig.backup_partitions();
    
            for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
            {
                backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
				GC_TRACE("[COM%d] WriteBinRegionData(): Begin write...", m_sComPortSetting.com.number);
    	        GC_TRACE("[COM%d] WriteBinRegionData: Partition_name(%s), file_path(%s), chksum_file(%s),start_addr(0x%LX), length(0x%LX)",m_sComPortSetting.com.number,
                           backupItem.partition_name().c_str(), backupItem.file_path().c_str(),backupItem.chksum_file().c_str());
    
                strcpy(m_ProductInfo_path, backupItem.file_path().c_str());
                m_ProductInfo.m_path = m_ProductInfo_path;
                m_ProductInfo.is_download = true;
                GC_TRACE("[COM%d] WriteBinRegionData: m_ProductInfo.m_path (%s)",m_sComPortSetting.com.number,m_ProductInfo_path);
    
                ret = GetROMInfo(backupItem.partition_name().c_str(), ProductInfoRomInfo);
                if(S_DONE != ret)
                {
                    GC_TRACE("[COM%d] WriteBinRegionData(): [Warning] Get GetROMInfo(%s) info fail! continue... ", m_sComPortSetting.com.number,backupItem.partition_name().c_str());
                    continue;
                }

      	        writeProductInfoArg.m_flash_type = m_sDnParam.eStorageType;
      	        writeProductInfoArg.m_addressing_mode = AddressingMode_LogicalAddress;
      	        writeProductInfoArg.m_address = ProductInfoRomInfo.begin_addr;
      	        writeProductInfoArg.m_container_length = 0;
      	        writeProductInfoArg.m_input_mode = InputMode_FromFile;
      	        writeProductInfoArg.m_input = m_ProductInfo_path;
      	        writeProductInfoArg.m_input_length = 0;
      	        writeProductInfoArg.m_cb_progress = cb_write_memory_progress;
      	        writeProductInfoArg.m_cb_progress_arg = this;
    			writeProductInfoArg.m_part_id = ProductInfoRomInfo.part_id;
      	        //Set m_program_mode of WriteFlashMemoryParameter for yaffs image
      	        if(NORMAL_ROM == ProductInfoRomInfo.rom_type|| ProductInfoRomInfo.rom_type == EXT4_IMG){
          	        writeProductInfoArg.m_program_mode = ProgramMode_PageOnly;
          	        GC_TRACE("[COM%d] WriteBinRegionData(): rom_type of writeback is normal image", m_sComPortSetting.com.number);
      	        }else if(YAFFS_IMG == ProductInfoRomInfo.rom_type){
          	        writeProductInfoArg.m_program_mode = ProgramMode_PageSpare;
          	        GC_TRACE("[COM%d] WriteBinRegionData(): rom_type of writeback is yaffs image", m_sComPortSetting.com.number);
      	        }else{
      	            GC_TRACE("[COM%d] WriteBinRegionData() ERROR: rom_type of writeback is not normal image or yaffs image", m_sComPortSetting.com.number);
                    return -1;
      	        }
    
      	        GC_TRACE("[COM%d] WriteBinRegionData(): Flashtype(%d), Addr(0x%08X), Path(%s)", m_sComPortSetting.com.number,
      	        writeProductInfoArg.m_flash_type, writeProductInfoArg.m_address, writeProductInfoArg.m_input);
    
      	        cb_write_memory_init(this);
    
      	        GC_TRACE("[COM%d] WriteBinRegionData(): Write product info flash memory...", m_sComPortSetting.com.number);
      	        ret = FlashTool_WriteFlashMemory(m_ft_api_handle, &writeProductInfoArg);
      	        if(S_DONE != ret)
      	        {
          	        GC_TRACE("[COM%d] WriteBinRegionData(): [ERROR] Write product info flash memory fail!", m_sComPortSetting.com.number);
          	        m_asMsg = "ERROR: Write product info flash memory fail!";
      	            FlashTool_Disconnect(&m_ft_api_handle);
          	        m_is_DA_connected = false;
          	        return ret;
      	        }

				GC_TRACE("[COM%d] WriteBinRegionData(): Write bin back success! bin name = %s.", m_sComPortSetting.com.number, backupItem.partition_name().c_str());
            }
        }
		else
		{
		    return -1;
		}

    }

    return S_DONE;
}


int TBOOT1::CleanBinRegionData(void)
{
    int ret = S_DONE;
    AnsiString image_path = m_BinRegion_path;
    AnsiString as_backup_dir;

    int iLD = image_path.LastDelimiter("\\");
    if (iLD > 0)
    {
        as_backup_dir = image_path.SubString(1, iLD-1);

        //Remove backup folder
        if( (OP_RID_FOLDER_NAME == m_sDnParam.e_BkRs_folderName) &&
                                                                   ( !m_sDnParam.b_BkRs_deletion)   )
        {
            GC_TRACE("[COM%d] CleanBinRegionData(): After restore, retain backup data.", m_sComPortSetting.com.number);
        }
        else
        {
            GC_TRACE("[COM%d] CleanBinRegionData(): After restore, remove backup data.", m_sComPortSetting.com.number);
            if ( ::DirectoryExists(as_backup_dir.c_str()) )
            {
                if( !RemoveAllFilesInFolder(as_backup_dir.c_str()) )
                {
                    GC_TRACE("[COM%d] CleanBinRegionData(): Remove all files in backup folder fail!", m_sComPortSetting.com.number);
                    m_asMsg = "ERROR: Remove backup folder fail!";
                    return -1;
                }
            }

            SyncUpdateTextHint("");
        }
    }
    else
    {
        GC_TRACE("[COM%d] CleanBinRegionData(): Incorrect backup folder!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Incorrect backup folder!";
        return -1;
    }

    return ret;
}


int TBOOT1::GetAllPartitionInfo(void ** ppArray, unsigned int &size)
{
    GC_TRACE("[COM%d] GetAllPartitionInfo(): Get partition information...", m_sComPortSetting.com.number);
    int ret = S_DONE;
    PART_INFO * pArray = NULL;


    ret = FlashTool_ReadPartitionCount(m_ft_api_handle, &size);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] GetAllPartitionInfo(): [ERROR] FlashTool_ReadPartitionCount() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: [Brom] Read partition count fail!";
        return ret;
    }

    GC_TRACE("[COM%d] GetAllPartitionInfo():FlashTool_ReadPartitionCount()-> part count = %d", m_sComPortSetting.com.number, size);
    pArray = new struct PART_INFO[size];
    if(NULL == pArray)
    {
        GC_TRACE("[COM%d] GetAllPartitionInfo(): [ERROR] Memory allocation fail!", m_sComPortSetting.com.number);
        return -1;
    }
	
    memset(pArray, 0, sizeof(PART_INFO) * size);
	
    ret = FlashTool_ReadPartitionInfo(m_ft_api_handle, pArray, size);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] GetAllPartitionInfo(): [ERROR] FlashTool_ReadPartitionInfo() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: [Brom] Read partition information fail!";
        delete[] pArray;
        return ret;
    }

    *ppArray = (void *)pArray;
    return ret;
}

int TBOOT1::GetPartitionInfo(const char * name, PART_INFO &ptInfo)
{
    PART_INFO *pPartInfo = NULL;
    unsigned int rom_count = 0;
    int ret = S_DONE;
   
    GC_TRACE("[COM%d] GetPartitionInfo(): Enter...! part name is %s", m_sComPortSetting.com.number,name);
    ret = FlashTool_ReadPartitionCount(m_ft_api_handle, &rom_count);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] GetPartitionInfo(): [ERROR] FlashTool_ReadPartitionCount() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: [Brom] Read partition count fail!";
        return ret;
    }


    pPartInfo = new struct PART_INFO[rom_count];
    if(NULL == pPartInfo)
    {
        GC_TRACE("[COM%d] GetPartitionInfo(): [ERROR] Memory allocation fail!", m_sComPortSetting.com.number);
        return -1;
    }

    ret = FlashTool_ReadPartitionInfo(m_ft_api_handle, pPartInfo, rom_count);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] GetPartitionInfo(): [ERROR] FlashTool_ReadPartitionInfo() fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: [Brom] Read partition information fail!";
        delete[] pPartInfo;
        return ret;
    }

    vector<string> vctDstTemp;
    vctDstTemp.clear();
    InitVectorForFindPartName(name,vctDstTemp);
   
    for(unsigned int i = 0; i < rom_count; i++)
    {
        //GC_TRACE("[COM%d] GetPartitionInfo(): name = %s, indes = %d.", m_sComPortSetting.com.number, pPartInfo[i].name, i);
        if(FindPartName(pPartInfo[i].name,vctDstTemp) == _TRUE)
        {
            strcpy(ptInfo.name, pPartInfo[i].name);
            ptInfo.begin_addr = pPartInfo[i].begin_addr;
            ptInfo.image_length = pPartInfo[i].image_length;
            GC_TRACE("[COM%d] GetPartitionInfo(): Get partition info successfully!Name(%s), Addr(0x%LLX), Len(0x%LLX)",
                m_sComPortSetting.com.number, ptInfo.name, ptInfo.begin_addr, ptInfo.image_length);
            delete[] pPartInfo;
            return S_DONE;
        }
    }

    
	GC_TRACE("[COM%d] GetPartitionInfo(): [ERROR] Fail!", m_sComPortSetting.com.number);
    delete[] pPartInfo;
    return -1;
}

int TBOOT1::ReadbackBinRegion(RB_INFO &rb_info, AnsiString & as_backup_dir, const char* BackupName, const char* ChecksumName)
{
    int ret = S_DONE;

    m_hRB = NULL;
    RB_Create(&m_hRB);
    ReadbackArgSetting();

    //rb_info.m_read_flag = NUTL_READ_PAGE_ONLY; //Notice: this flag should be set before calling ReadbackBinRegion!
    rb_info.enable = _TRUE;
    rb_info.index = 0;

    //AnsiString dataPath = as_backup_dir + "\\" + BkRsDataFileName;
	AnsiString dataPath = as_backup_dir + "\\" + BackupName;
    strcpy(rb_info.filepath, dataPath.c_str());

    ret=RB_Append(m_hRB, rb_info.filepath, rb_info.readback_addr, rb_info.readback_len);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): RB_Append() fail!", m_sComPortSetting.com.number);
        RB_Destroy(&m_hRB);
        return ret;
    }

    ret = RB_SetReadbackFlag(m_hRB, 0, rb_info.m_read_flag);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): RB_SetReadbackFlag() fail!", m_sComPortSetting.com.number);
        RB_Delete(m_hRB, 0);
        RB_Destroy(&m_hRB);
        return ret;
    }

	ret = RB_SetPacketLength(m_hRB,4*1024*10);
	if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): RB_SetPacketLength() fail!", m_sComPortSetting.com.number);
        RB_Delete(m_hRB, 0);
        RB_Destroy(&m_hRB);
        return ret;
    }

    ret = RB_SetAddrTypeFlag(m_hRB, 0, NUTL_ADDR_LOGICAL);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): RB_SetAddrTypeFlag() fail!", m_sComPortSetting.com.number);
        RB_Delete(m_hRB, 0);
        RB_Destroy(&m_hRB);
        return ret;
    }


    ret = FlashTool_Readback(m_ft_api_handle, &m_sRBArg, &m_sRBResult);
    if ( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): FlashTool_Readback() fail!", m_sComPortSetting.com.number);
        RB_Delete(m_hRB, 0);
        RB_Destroy(&m_hRB);
        return ret;
    }

    ret = RB_Delete(m_hRB, 0);
    if ( S_DONE != ret )
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): RB_Delete() fail!", m_sComPortSetting.com.number);
    }

    RB_Destroy(&m_hRB);


    //Calculate checksum and checking
    //AnsiString cksmPath = as_backup_dir + "\\" + BkRsCksmFileName;
  	AnsiString cksmPath = as_backup_dir + "\\" + ChecksumName;
    unsigned char actCksm = 0;
    if(ComputeCheckSum(dataPath, actCksm) && SaveCheckSum(cksmPath, actCksm))
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): Checksum has been saved!", m_sComPortSetting.com.number);
		GC_TRACE("[COM%d] ReadbackBinRegion(): Readback bin succeess!  bin name = %s", m_sComPortSetting.com.number, BackupName);
    }
    else
    {
        GC_TRACE("[COM%d] ReadbackBinRegion(): [ERROR] Compute or save checksum fail! Readback bin fail!", m_sComPortSetting.com.number);
        return -1;
    }

    
    return ret;
}


bool TBOOT1::ComputeCheckSum(const AnsiString &dataPath, unsigned char &cksm)
{
    unsigned char buffer[1024];

    FILE *file = fopen( dataPath.c_str() , "rb" );
    if (file==NULL)
    {
        GC_TRACE("[COM%d] ComputeCheckSum(): [ERROR] Open data file fail!", m_sComPortSetting.com.number);
        return false;
    }

    cksm = 0;
    unsigned int read_size = 0;
    while(!feof(file))
    {
        read_size = fread(buffer, 1, 1024, file);

        for(unsigned int i=0;i<read_size;i++)
        {
            cksm ^= buffer[i];
        }
    }

    GC_TRACE("[COM%d] ComputeCheckSum(): Checksum(%d)", m_sComPortSetting.com.number, cksm);

    fclose (file);
    return true;
}

bool TBOOT1::LoadCheckSum(const AnsiString &cksmPath, unsigned char &cksm)
{
    unsigned char buffer[2];

    FILE *file = fopen( cksmPath.c_str() , "rb" );
    if (file==NULL)
    {
        GC_TRACE("[COM%d] LoadCheckSum(): [ERROR] Open checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }

    unsigned int read_size = 0;
    read_size = fread(buffer, 1, 2, file);
    if(read_size <= 0)
    {
        GC_TRACE("[COM%d] LoadCheckSum(): [ERROR] Read checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }

    cksm = buffer[0];
    GC_TRACE("[COM%d] LoadCheckSum(): Checksum(%d)", m_sComPortSetting.com.number, cksm);

    fclose (file);
    return true;
}

bool TBOOT1::SaveCheckSum(const AnsiString &cksmPath, const unsigned char &cksm)
{
    unsigned char buffer[2] = {0};
    buffer[0] = cksm;

    FILE *file = fopen( cksmPath.c_str() , "wb" );
    if (file==NULL)
    {
        GC_TRACE("[COM%d] SaveCheckSum(): [ERROR] Open checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }

    unsigned int write_size = 0;
    write_size = fwrite(buffer , 1 , sizeof(buffer) , file );
    if(sizeof(buffer) != write_size)
    {
        GC_TRACE("[COM%d] SaveCheckSum(): [ERROR] Write checksum file fail!", m_sComPortSetting.com.number);
        return false;
    }

    fclose (file);
    return true;
}


