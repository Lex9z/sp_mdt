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
 *   FlashDownload.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Flash download/format thread source file
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
 * Oct 7 2008 MTK02172
 * [STP100001827] [Multiport download tool] Multiport download tool ver 3.0840.0
 *
 *
 * Oct 7 2008 MTK02172
 * [STP100001827] [Multiport download tool] Multiport download tool ver 3.0840.0
 *
 *
 * Sep 10 2008 MTK02172
 * [STP100001803] [Multiport download tool] Multiport download tool ver 3.0836.0
 *
 *
 * Aug 12 2008 mtk00490
 * [STP100001788] [Multiport download tool] Multiport download tool ver 3.0832.0
 *
 *
 * May 21 2008 mtk00490
 * [STP100001729] [Multiport download tool] Multiport download tool ver 3.0820.0
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <vcl.h>
#pragma hdrstop

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>


#include <algorithm>
#include <time.h>

#include <objbase.h>
#include <initguid.h>

#include "CustomerSetup.h"
#include "file_utils.h"

#include "Efuse\EfuseWriter_Console.h"
#include "Efuse\EfuseSettings.h"

#include "ProtectRegion.h"
#include "ProtectRegionInfo.h"
#include "BackupConfig.h"
#include "MetaProcess.h"

//#include "MultiPortFlashDownloadTool.h"


//---------------------------------------------------------------------------

#ifndef SPMETA_DLL_H
#include "spmeta.h"
#endif

#ifndef _SLA_CHALLENGE_H_
#include "sla_challenge.h"
#endif

#ifndef _MULTIPORT_COMMON_H_
#include "Multiport_Common.h"
#endif

#ifndef _FORM_MULTILOAD_H_
#include "Form\form_MultiLoad.h"
#endif

#ifndef _MD_MSG_
#include "MD_msg.h"
#endif

#ifndef _EBOOT_INTERFACE_H_
#include "..\Eboot\interface.h"
#endif

#ifndef _FLASHDOWNLOAD_H_
#include "FlashDownLoad.h"
#endif

#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#include "MultiPortFlashDownloadTool.h"
#endif

#ifndef _FACTORY_COMMON_H_
#include "factory_common.h"
#endif

#ifndef __COM_PORT_UTILS_H
#include "COMPortUtils.h"
#endif

#ifndef __Tool_Settings_H
#include "Toolsettings.h"
#endif


#include "formChecksumProgress.h"

using namespace std;


#pragma package(smart_init)


#define  CHECK_TERMINATED     if( this->Terminated )\
                              {\
                                  goto clean;\
                              }
#define DLCALL(a) {if((a)) {return -1;}}
// for android usb port find
DEFINE_GUID(Guid_ANDROID_USB2Ser,0xA5DCBF10L,0x6530,0x11D2,0x90,0x1F,0x00,0xC0,0x4F,
                            0xB9,0x51,0xED);


#define     PID_PROLOADER       "2000"
#define     PID_DA_HS      "2001"
#define     PID_BOOTROM        "0003"
#define     VID_ANDROID       "0E8D"

#define  InterfaceClassGuidConstant		Guid_ANDROID_USB2Ser

// handle
extern AUTH_HANDLE_T	g_AUTH_HANDLE;
extern SCERT_HANDLE_T	g_SCERT_HANDLE;
extern DL_HANDLE_LIST_T        g_DL_HANDLE_LIST;
extern DL_HANDLE_T *g_DL_HANDLE_SecroReworking;


bool isShowNoFormatInfo = false;
bool isShowFileNotLoaded = false;
HANDLE Flash_Download_Event[MAX_SUPPORT_PORT_NUM];

CRITICAL_SECTION  g_Meta_CS;

// access
extern HANDLE hSemaphore;
extern TMainForm *MainForm;


static int __stdcall cb_checkusbstatus_init(void * arg, const char* usb_status)
{
    //TBOOT1 *t = (TBOOT1 *)arg;

    return 0;
}

//==============================================================================
__fastcall TBOOT1::TBOOT1(bool CreateSuspended, HANDLE hMsgHandle, COM_PORT_SETTING com_port_setting, S_DN_PARAM_T dn_param)
        : TThread(CreateSuspended)
{
    hPostMsgDestHandle = hMsgHandle;

    m_sComPortSetting = com_port_setting;
    m_sDnParam       = dn_param;

    m_is_DA_connected = false;
    m_part_status = PART_NO_CHANGE;
}

//=============================================================================
////////////////////////   call back   ////////////////////////////////////////
//=============================================================================
int __stdcall cb_PowerOnOxford(HANDLE hCOM, void *usr_arg)
{
    return 0;
}

void _fastcall TBOOT1::Sync_Update_UI()
{
   Synchronize(Update_UI);
}

void _fastcall TBOOT1::OutputFailReport(int iRet)
{
    if(MainForm->m_EnableOutputResult == 1 && MainForm->m_cbEnableAutoPulling->Checked == true)
    {
       AnsiString ansiOutputpath = m_sDnParam.as_working_dir+"\\Failreport.log";
       AnsiString ansiRecord;
       AnsiString strTemp;
       int iWriteSize;
     //  TDateTime DateTime = Time();  // store the current date and time
       FILE *pOutputFile = fopen(ansiOutputpath.c_str(),"a+");
       if(NULL != pOutputFile)
       {
          DateTimeToString(strTemp,"yyyy-mm-dd hh:nn:ss",Now());
          ansiRecord.sprintf("%s[Com%2d]Error Return = %d\n",strTemp.c_str(),m_sComPortSetting.com.number, iRet);
          fwrite(ansiRecord.c_str(),ansiRecord.Length(),1,pOutputFile);
          fclose(pOutputFile);
       }
    }
}

void _fastcall TBOOT1::Update_UI()
{
   MainForm->Update_UI();
}

void _fastcall TBOOT1::SetShowString(AnsiString strShow)
{
   m_asMsg = strShow;
}

void __stdcall Set_send_flash_bin_percentage(void *usr_arg, unsigned int percentage)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percentage);
}

void __stdcall Apply_entry_mpdl_mode(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    AnsiString log;

    log.printf("[Com%d] Apply_entry_mpdl_mode", t->GetCOMPortNumber());

    Eboot_Log(log.c_str());

    MainForm->_t_manager.Apply_enter_mpdl_queue(t->GetCOMPortIndex());
}

bool __stdcall Enter_mpdl_mode_finish(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    AnsiString log;

    log.printf("[Com%d] Enter_mpdl_mode_finish", t->GetCOMPortNumber());

    Eboot_Log(log.c_str());

    MainForm->_t_manager.Enter_mpdl_finish(t->GetCOMPortIndex());
    return true;
}

bool __stdcall Is_exit_permited(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    Eboot_Log(MainForm->_t_manager.Print_usb_protected_flag().c_str());


    return !MainForm->_t_manager.Is_any_other_in_usb_protected_mode(t->GetCOMPortIndex());
}

void __stdcall Send_image_finish(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    MainForm->_t_manager.Release_usb_protected_flag(t->GetCOMPortIndex());
}

//---------------------------------------------------------------------------
static int __stdcall cb_download_da_init(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    int i = t->GetCOMPortIndex();
    MainForm->DownLoadTimeSec[i]=0;

    // access
    if (MainForm->Get_EnableFactoryDatabaseLog())
    {
        TDateTime datetime;
        TDateTime date = datetime.CurrentDate();
        TDateTime time = datetime.CurrentTime();
        sTime     stime;
        time.DecodeTime( &stime.hour, &stime.min, &stime.sec, &stime.msec);
        MainForm->SetDnDate( i, date );
        MainForm->SetDnStartTime( i, time );
        MainForm->SetDnStartsTime( i, stime );
    }

    t->prefix = "DA ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clRed);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_download_da(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);
    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_download_flash_init(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->prefix = "DL ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clYellow);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_download_flash(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_da_dl_checksum_init(void *usr_arg, const char* image_name)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->prefix.sprintf("Cksm(%s) ", image_name);
    t->SyncInitProgress();
    //t->SyncUpdateProgressColor(clWhite, clMaroon);
    // 2011-09-14 Modified by Shaoying Han
    t->SyncUpdateProgressColor(clWhite, clSilver);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_da_dl_checksum(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}


//-----------------------------------------------------------------------------
static int __stdcall cb_download_bloader_init(void *usr_arg)
{
    return 0;
}

//-----------------------------------------------------------------------------
static int __stdcall cb_download_bloader(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    return 0;
}

static int __stdcall cb_download_bloader_init_android(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    if(usr_arg == NULL){
		return -1;
	}
    t->prefix = "DL ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clPurple);

    return 0;
}

//-----------------------------------------------------------------------------
static int __stdcall cb_download_bloader_android(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    if(usr_arg == NULL){
		return -1;
	}
		
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}

//==============================================================================
static int __stdcall cb_format_report_init(HW_StorageType_E storage_type, ULL64 begin_addr, ULL64 length, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

	std::string storageStr = "UNKNOWN";

	if (HW_STORAGE_NAND == storage_type)
	{
		storageStr = "NAND";
	}
	else if ( HW_STORAGE_EMMC == storage_type )
	{
		storageStr = "EMMC";
	}
	else if ( HW_STORAGE_SDMMC == storage_type )
	{
		storageStr = "SDMMC";
	}

    t->m_asMsg.printf("Format %s flash:  begin_addr(0x%016LLX), length(0x%016LLX). ", storageStr.c_str(), begin_addr, length);
    t->SyncShowMsg();
    t->prefix = "FR ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clGreen);

    return 0;
}

//------------------------------------------------------------------------------
static int __stdcall cb_format_report(unsigned char percent, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_readback_flash_init(HW_StorageType_E  storage_type, ULL64 rb_addr, unsigned int rb_length, const char *rb_filepath, void *usr_arg)
{

    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->m_asMsg.printf("Readback from Flash:  begin_addr(0x%08X), length(0x%08X). ", rb_addr, rb_length);
    t->SyncShowMsg();
    t->prefix = "RB ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clBlue);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_readback_flash(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

    return 0;
}

//-----------------------------------------------------------------------------------
static int __stdcall cb_before_process(const DA_REPORT_T  *p_da_report, void *usr_arg)
{
    return 0;
}

//-----------------------------------------------------------------------------------
void __stdcall set_send_image_file_info(void *usr_arg, const char * file_type,
                                       unsigned int num_send, unsigned int total)
{
    TBOOT1 *t = (TBOOT1 *)usr_arg;

    t->prefix = file_type;
    t->SyncUpdateProgressColor(clWhite, clYellow);
    t->SyncInitProgress();
}

//------------------------------------------------------------------------------
static int __stdcall cb_in_brom_stage(unsigned int brom_handle, HANDLE hCOM, void *usr_arg)
{

  int ret;
  unsigned short	value16;
  TBOOT1 *t=(TBOOT1 *)usr_arg;

  if( t->m_sDnParam.b_PullDownGPIO17 && (MT6218B == t->m_sDnParam.e_bbchip_type) )
  {
      // configure GPIO17 mode to GPIO
      value16 = 0x000C;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x0120+0x08)/*GPIO_MODE3_CLR*/, &value16, 1);

      // configure GPIO17 direction as output
      value16 = 0x0002;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x0010+0x04)/*GPIO_DIR2_SET*/, &value16, 1);

      // pull down GPIO17 to disable vibrator
      value16 = 0x0002;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x00a0+0x08)/*GPIO_DOUT2_CLR*/, &value16, 1);
      return ret;
  }
  return 0;

}

//---------------------------------------------------------------------------
static int __stdcall cb_format_statistics(const FormatStatisticsReport_T  *p_report, void *usr_arg)
{
    return 0;
}

//--------------------------------------------------------------------------
static int __stdcall cb_update_param_init(void *usr_arg)
{
    return 0;
}

//--------------------------------------------------------------------------
static int __stdcall cb_update_param(unsigned char percent, void *usr_arg)
{
    return 0;
}

//--------------------------------------------------------------------------
static int __stdcall cb_post_process_init(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->prefix = "Generate MAC";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clNavy);

    return 0;
}

//--------------------------------------------------------------------------
static int __stdcall cb_post_process(unsigned char percent, void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);

   return 0;

}

//---------------------------------------------------------------------------
static int __stdcall cb_security_pre_process_notify(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->m_asMsg.printf("Proceed 1st time download pre-process ... (it takes about 10~15 seconds)\n\nPlease press OK to continue!");
    t->SyncShowMsg();

    return 0;
}

//--------------------------------------------------------------------------
static int __stdcall cb_security_post_process_notify(void *usr_arg)
{
    TBOOT1 *t=(TBOOT1 *)usr_arg;

    t->m_asMsg.printf("Proceed 1st time download post-process ... (it takes about 10~15 seconds)\n\nPlease press OK to continue!");
    t->SyncShowMsg();

    return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void _fastcall TBOOT1::StopTimer()
{
    MainForm->Stop_Timer(GetCOMPortIndex());
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncStopTimer()
{
    Synchronize(StopTimer);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::ShowMsg()
{
	AnsiString strAutoPollingNum = "Auto Polling number:";
	MainForm->sbMPDT->Panels->Items[0]->Text = m_asMsg + strAutoPollingNum + MainForm->GetPollingNumber();
    MainForm->pbCOMArray[GetCOMPortIndex()]->Hint = m_asMsg;
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncShowMsg()
{
    Synchronize(ShowMsg);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::Start()
{
    int i;
    i = GetCOMPortIndex();
    MainForm->Set_COMPercentageCaption(i, "START");
    MainForm->Set_shpCOMColor(i, clBlue);
    MainForm->SetPbComProgress(i, 0);
    m_asMsg = "";
    SyncShowMsg();
    MainForm->Clear_BackupRestoreStatus(i);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncStart()
{
    Synchronize(Start);
}


void _fastcall TBOOT1::UpdateProgressColor()
{
   int i = GetCOMPortIndex();
   MainForm->SetPbComBackColor(i, _bk_col);
   MainForm->SetPbComForeColor(i, _ft_col);
}

void _fastcall TBOOT1::SyncUpdateProgressColor(TColor bk, TColor ft)
{
    _bk_col =  bk;
    _ft_col =  ft;
    Synchronize(UpdateProgressColor);
}

void _fastcall TBOOT1::UpdateProgressPercentage()
{
    int i = GetCOMPortIndex();
    MainForm->SetPbComProgress(i, m_ucProgressPercentage);
}

void _fastcall TBOOT1::SyncUpdateProgressPercentage(int precentage)
{
   m_ucProgressPercentage = precentage;
   Synchronize(UpdateProgressPercentage);
}

void _fastcall TBOOT1::UpdateTextHint()
{
    int i = GetCOMPortIndex();
    if(0 == strcmp(_hint, ""))
    {
        MainForm->m_stBkStatus[i]->Caption = "";
    }
    else
    {
        MainForm->m_stBkStatus[i]->Caption = "A";
    }

    MainForm->m_stBkStatus[i]->Hint = _hint;
    GC_TRACE("[COM%d] UpdateTextHint(): _hint(%s)", m_sComPortSetting.com.number, _hint);
}

void _fastcall TBOOT1::SyncUpdateTextHint(char * hint)
{
   memset(_hint,0,512);
   strcpy(_hint,hint);
  // memcpy(_hint, hint, 512);
   Synchronize(UpdateTextHint);
   GC_TRACE("[COM%d] SyncUpdateTextHint(): _hint(%s)", m_sComPortSetting.com.number, _hint);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::InitProgress()
{
    int i = GetCOMPortIndex();

    MainForm->Set_COMPercentageCaption(i, prefix);
    MainForm->pbCOMArray[i]->Progress = 0;
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncInitProgress()
{
    Synchronize(InitProgress);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void _fastcall TBOOT1::UpdateResultError()
{
    AnsiString	s1, s2;
    AnsiString s_error;
    int i;
    s_error = prefix + "ERROR";

    i = GetCOMPortIndex();

    if( MainForm->_t_manager.Is_stopped(i) )
        {  s_error = prefix + "STOP"; };

    MainForm->COMPercentageArray[i]->Caption = s_error;
    MainForm->Set_COMPercentageCaption( i, s_error );

    SyncStopTimer();
    //MainForm->btnCOMStartArray[i]->Enabled = true;
    MainForm->cbCOMArray[i]->Enabled = true;
    MainForm->Set_shpCOMColor( i, clRed );
    //MainForm->DownloadStatus[i] = DOWNLOAD_STATUS_NOTRUNNING;

    // access
    if( MainForm->Get_EnableFactoryDatabaseLog() )
    {
        MainForm->SetPassFail( i, TEST_RESULT_FAIL);
        WaitForSingleObject( hSemaphore, 5000 );
        MainForm->AddDnResultToAccess( i );
    }

    switch (ret)
    {
        case -1:
            // if ret is -1, means user will show message by himself
            break;

        case 9000:
            m_asMsg.printf("Error: can not find usb port.");
            break;

        case S_DA_NOR_UNSUPPORTED_DEV_ID:
        case S_DA_NAND_UNSUPPORTED_DEV_ID:
        {
            m_asMsg.printf("Error: Flash not support, please refer to Memory_Validation_List_External.xls for current support flash list.");
        }
            break;

        case TARGET_EXCEED_BOUNDARY_ERROR:
        {
	        m_asMsg.printf("Error: Image exceed boundary address!");
	        break;
        }

        default:
        {
            if (S_SECURITY_ERROR_BEGIN <= ret)
            {
                m_asMsg.printf("Error: %s(%d) \n\nMSP Error Code: 0x%02X", StatusToString(ret), ret, m_sFtResult.m_da_report.m_msp_err_code);
            }
            else
            {
                m_asMsg.printf("Error: %s(%d) ", StatusToString(ret), ret);
            }
        }
        break;
    }

    Synchronize(ShowMsg);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncUpdateResultError()
{
    Synchronize(UpdateResultError);
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::UpdateResultOk()
{
    AnsiString s_ok;
    int i;

    s_ok = prefix + "OK";

    i = GetCOMPortIndex();

    // access
    if( MainForm->Get_EnableFactoryDatabaseLog() )
    {
        TDateTime datetime;
        TDateTime time = datetime.CurrentTime();
        sTime     stime;
        time.DecodeTime( &stime.hour, &stime.min, &stime.sec, &stime.msec);
        MainForm->SetPassFail( i, TEST_RESULT_PASS);
        MainForm->SetDnEndsTime( i, stime );
        MainForm->SetDnTime( i );
        MainForm->SetThroughput( i );
        WaitForSingleObject( hSemaphore, 5000 );
        MainForm->AddDnResultToAccess( i );
    }

    MainForm->COMPercentageArray[i]->Caption = s_ok;
    MainForm->Set_COMPercentageCaption( i, s_ok );
    MainForm->pbCOMArray[i]->Progress = 100;
    //MainForm->TimerCOMArray[i]->Enabled = false;
    SyncStopTimer();
    MainForm->btnCOMStartArray[i]->Enabled = true;
    MainForm->cbCOMArray[i]->Enabled = true;
    MainForm->Set_shpCOMColor( i, clGreen );
 //   MainForm->DownloadStatus[i] = DOWNLOAD_STATUS_NOTRUNNING;
/*    PostMessage(
                     hPostMsgDestHandle,
                     WM_MD_DOWNLOAD_SUCCESSFUL,
                     i,
                     0
                    );
*/
}

//---------------------------------------------------------------------------
void _fastcall TBOOT1::SyncUpdateResultOk()
{
	Synchronize(UpdateResultOk);
}


void TBOOT1::BootArgSetting(BOOT_FLASHTOOL_ARG	&boot_arg)
{
    if(MT6516_AP == frmMultiLoad->m_chip_type)
    {
        boot_arg.m_bbchip_type = MT6516_AP;
    }
    else
    {
        boot_arg.m_bbchip_type = AUTO_DETECT_BBCHIP;
    }

    //boot_arg.m_bbchip_type = m_sDnParam.e_bbchip_type;
    boot_arg.m_ext_clock = AUTO_DETECT_EXT_CLOCK; //m_sDnParam.e_ext_clock;
    boot_arg.m_ms_boot_timeout = BOOT_INFINITE;
    boot_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;
    boot_arg.m_p_bank0_mem_cfg = m_sDnParam.s_emi_param.b_bank0_auto?NULL:&(m_sDnParam.s_emi_param.ui_bank0_cfg);
    boot_arg.m_p_bank1_mem_cfg = m_sDnParam.s_emi_param.b_bank1_auto?NULL:&(m_sDnParam.s_emi_param.ui_bank1_cfg);
    boot_arg.m_da_handle = g_DA_HANDLE;

    boot_arg.m_cb_download_da_init = ::cb_download_da_init;
    boot_arg.m_cb_download_da_init_arg = this;
    boot_arg.m_cb_download_da = ::cb_download_da;
    boot_arg.m_cb_download_da_arg = this;

    boot_arg.m_cb_in_brom_stage = ::cb_in_brom_stage;
    boot_arg.m_cb_in_brom_stage_arg = this;
    boot_arg.m_speedup_brom_baudrate = _TRUE;

    // WM_BROM_READY_TO_POWER_ON_TGT
    boot_arg.m_ready_power_on_wnd_handle	= NULL;
    boot_arg.m_ready_power_on_wparam		= NULL;
    boot_arg.m_ready_power_on_lparam		= NULL;

    // serial link authentication arg
    boot_arg.m_auth_handle              = g_AUTH_HANDLE;
    boot_arg.m_scert_handle              = g_SCERT_HANDLE;
    boot_arg.m_cb_sla_challenge         = SLA_Challenge;
    boot_arg.m_cb_sla_challenge_arg     = this;
    boot_arg.m_cb_sla_challenge_end     = SLA_Challenge_END;
    boot_arg.m_cb_sla_challenge_end_arg = this;

    // EDGE card
    boot_arg.m_cb_com_init_stage = ::cb_PowerOnOxford;
    boot_arg.m_cb_com_init_stage_arg = this;

    // EMI Control for bank0 and bank1
    boot_arg.m_p_bank0_mem_cfg            = NULL;
    boot_arg.m_p_bank1_mem_cfg            = NULL;

    boot_arg.m_usb_enable = _FALSE;
}

//--------------------------------------------------------------------------
void _fastcall TBOOT1::EnableMainSpecificMsUI()
{
    //int i = m_sComPortSetting.com.number - m_sDnParam.i_FirstComNo;
    int i = GetCOMPortIndex();
    MainForm->cbCOMArray[i]->Enabled = true;
    StopTimer();
}

//--------------------------------------------------------------------------
void _fastcall TBOOT1::SyncEnableMainSpecificMsUI()
{
    Synchronize(EnableMainSpecificMsUI);
}

void TBOOT1::ResetArgMemory(void)
{
    memset(&m_sFtArg, 0, sizeof(m_sFtArg));
    memset(&m_sFtDlArg, 0, sizeof(m_sFtDlArg));
    memset(&m_sFtResult, 0, sizeof(m_sFtResult));
    memset(&m_sFmtArg, 0, sizeof(m_sFmtArg));
    memset(&m_sFmtResult, 0, sizeof(m_sFmtResult));


    memset(&m_sConnArg, 0, sizeof(m_sConnArg));
    memset(&m_sConnResult, 0, sizeof(m_sConnResult));
    memset(&m_sRBArg, 0, sizeof(m_sRBArg));
    memset(&m_sRBResult, 0, sizeof(m_sRBResult));
	memset(&m_protected_image, 0, sizeof(m_protected_image));
   
    return;
}


void TBOOT1::FlashtoolArgSetting(void)
{

    BootArgSetting(m_sFtArg.m_boot_arg);

    m_sFtArg.m_cb_set_high_speed_baudrate = NULL;
    m_sFtArg.m_cb_set_high_speed_baudrate_arg = NULL;
    m_sFtArg.m_nor_chip_select[0] = CS_0;
    m_sFtArg.m_nor_chip_select[1] = CS_WITH_DECODER;
    m_sFtArg.m_nand_chip_select   = m_sDnParam.s_bbchip_op.b_enable_nfi_cs1?CS_1:CS_0;
    m_sFtArg.m_p_nand_acccon      = NULL  /* Disabled for SPDL &m_sDnParam.sEMI_NFI_Setting.ui_nand_acccon*/;
    m_sFtArg.m_baudrate_full_sync_count = 0x01;

    // enable DRAM setting from UI thread
    m_sFtArg.m_enable_ui_dram_cfg = _FALSE;

    return;
}


void TBOOT1::DownloadArgSetting(void)
{
      //FLASHTOOL_DOWNLOAD_ARG settings
      m_sFtDlArg.m_dl_handle_list                      = g_DL_HANDLE_LIST;
      m_sFtDlArg.m_dl_handle                           = NULL;
      m_sFtDlArg.m_download_accuracy                   = ACCURACY_AUTO;
      m_sFtDlArg.m_cb_download_flash_init              = ::cb_download_flash_init;
      m_sFtDlArg.m_cb_download_flash_init_arg          = this;
      m_sFtDlArg.m_cb_download_flash                   = cb_download_flash;
      m_sFtDlArg.m_cb_download_flash_arg               = this;
      m_sFtDlArg.m_cb_security_post_process_notify     = ::cb_security_post_process_notify;
      m_sFtDlArg.m_cb_security_post_process_notify_arg = this;
      m_sFtDlArg.m_enable_tgt_res_layout_check         = m_sDnParam.s_multibin_op.b_check_target_resource_layout?_TRUE:_FALSE;
      m_sFtDlArg.m_enable_bbchip_ver_check             = m_sDnParam.s_bbchip_op.b_check_eco_ver ?_TRUE:_FALSE;
      m_sFtDlArg.m_cb_format_statistics                = ::cb_format_statistics;
      m_sFtDlArg.m_cb_format_statistics_arg            = this;

      m_sFtDlArg.m_format_enable = _FALSE;
      m_sFtDlArg.m_format_cfg.m_auto_format_fat = _FALSE;
      m_sFtDlArg.m_enable_wdt_timeout = _FALSE;
      m_sFtDlArg.m_ms_wdt_timeout_interval = 0;

      //checksum
      m_sFtDlArg.m_cb_checksum_init = cb_da_dl_checksum_init;
      m_sFtDlArg.m_cb_checksum_init_arg = this;
      m_sFtDlArg.m_cb_checksum = cb_da_dl_checksum;
      m_sFtDlArg.m_cb_checksum_arg = this;


      // parameter
      m_sFtDlArg.m_otp_enable = _FALSE;
      m_sFtDlArg.m_otp_op = OTP_WRITE;  /*modified by Shaoying 20120614 m_sDnParam.s_param.m_otp_op*/;
      m_sFtDlArg.m_otp.m_addr = m_sDnParam.s_param.m_otp.m_addr;
      m_sFtDlArg.m_otp.m_data = m_sDnParam.s_param.m_otp.m_data;
      m_sFtDlArg.m_otp.m_len = m_sDnParam.s_param.m_otp.m_len;
      m_sFtDlArg.m_ft_cfg_enable = _FALSE;
      m_sFtDlArg.m_ft_cfg.m_security_cfg.m_uint32 = m_sDnParam.s_param.m_ft_cfg.m_security_cfg.m_uint32;
      m_sFtDlArg.m_sec_ro_enable = m_sDnParam.s_param.m_sec_ro_enable;
      m_sFtDlArg.m_sec_ro.m_data = m_sDnParam.s_param.m_sec_ro.m_data;
      m_sFtDlArg.m_sec_ro.m_len = m_sDnParam.s_param.m_sec_ro.m_len;
      m_sFtDlArg.m_cust_para_enable = _FALSE;
      m_sFtDlArg.m_cust_para.m_data = m_sDnParam.s_param.m_cust_para.m_data;
      m_sFtDlArg.m_cust_para.m_len = m_sDnParam.s_param.m_cust_para.m_len;

      m_sFtDlArg.m_cb_param_process_init     = ::cb_update_param_init;
      m_sFtDlArg.m_cb_param_process_init_arg = this;
      m_sFtDlArg.m_cb_param_process	       = ::cb_update_param;
      m_sFtDlArg.m_cb_param_process_arg      = this;

      m_sFtDlArg.m_cb_post_process_init      = ::cb_post_process_init;
      m_sFtDlArg.m_cb_post_process_init_arg  = this;
      m_sFtDlArg.m_cb_post_process	       = ::cb_post_process;
      m_sFtDlArg.m_cb_post_process_arg       = this;

      m_sFtDlArg.m_cb_format_report_init       = ::cb_format_report_init;
      m_sFtDlArg.m_cb_format_report_init_arg   = this;
      m_sFtDlArg.m_cb_format_report            = ::cb_format_report;
      m_sFtDlArg.m_cb_format_report_arg        = this;
      m_sFtDlArg.m_enable_tgt_res_layout_check = m_sDnParam.s_multibin_op.b_check_target_resource_layout?_TRUE:_FALSE;
      m_sFtDlArg.m_enable_bbchip_ver_check     = m_sDnParam.s_bbchip_op.b_check_eco_ver ?_TRUE:_FALSE;
      m_sFtDlArg.m_cb_format_statistics        = ::cb_format_statistics;
      m_sFtDlArg.m_cb_format_statistics_arg    = this;

      m_sFmtArg.m_storage_type = frmMultiLoad->m_Storage_Type;
      if( OPERATION_NFB == m_sDnParam.e_Operation )
      {
          // for NFB bloader
          m_sFtDlArg.m_format_cfg.m_validation      = m_sDnParam.s_nfb_param.b_validation;
          m_sFtDlArg.m_cb_download_bloader_init     = ::cb_download_bloader_init;
          m_sFtDlArg.m_cb_download_bloader_init_arg = this;
          m_sFtDlArg.m_cb_download_bloader          = ::cb_download_bloader;
          m_sFtDlArg.m_cb_download_bloader_arg      = this;
          //m_sFtDlArg.m_storage_type = HW_STORAGE_NAND;
          //m_sFmtArg.m_storage_type = HW_STORAGE_NAND;
      }
      else
      {
          m_sFtDlArg.m_format_cfg.m_validation      = m_sDnParam.s_nor_param.b_validation;
          m_sFtDlArg.m_cb_download_bloader_init     = NULL;
          m_sFtDlArg.m_cb_download_bloader_init_arg = NULL;
          m_sFtDlArg.m_cb_download_bloader          = NULL;
          m_sFtDlArg.m_cb_download_bloader_arg      = NULL;
          //m_sFtDlArg.m_storage_type = HW_STORAGE_NOR;
          //m_sFmtArg.m_storage_type = HW_STORAGE_NOR;
      }

      // GPS
      m_sFtDlArg.m_relay_da_enable           = m_sDnParam.s_param.m_gps_enable;
      m_sFtDlArg.m_gpio_gps.m_power_pin      = 65535 /*m_sDnParam.s_param.m_gps_power_pin*/;
      m_sFtDlArg.m_gpio_gps.m_reset_pin      = 65535/*m_sDnParam.s_param.m_gps_reset_pin*/;
      m_sFtDlArg.m_gpio_gps.m_standby_pin    = 65535/*m_sDnParam.s_param.m_gps_standby_pin*/;
      m_sFtDlArg.m_gpio_gps.m_32k_pin        = 65535/*m_sDnParam.s_param.m_gps_32k_pin*/;

      // Maobing Add for Smart phone download to enable watch dog
      m_sFtDlArg.m_enable_wdt_timeout      = _TRUE;
      m_sFtDlArg.m_ms_wdt_timeout_interval = 1000;

      return;
}


void TBOOT1::FormatArgSetting(void)
{
    // storage type
    m_sFmtArg.m_storage_type = frmMultiLoad->m_Storage_Type;
    switch(m_sDnParam.e_Operation)
    {
        case OPERATION_NFB:
        {
            //m_sFmtArg.m_storage_type = HW_STORAGE_NAND;
            m_sFmtArg.m_format_cfg.m_format_begin_addr = m_sDnParam.s_nfb_param.ui_nand_fat_begin_addr;
            m_sFmtArg.m_format_cfg.m_format_length     = m_sDnParam.s_nfb_param.ui_nand_fat_length;
            m_sFmtArg.m_format_cfg.m_validation        = m_sDnParam.s_nfb_param.b_validation;
        }
        break;

        case OPERATION_NOR:
        default:
        {
            //m_sFmtArg.m_storage_type = HW_STORAGE_NOR;
            m_sFmtArg.m_format_cfg.m_format_begin_addr = m_sDnParam.s_nor_param.ui_fat_begin_addr;
            m_sFmtArg.m_format_cfg.m_format_length     = m_sDnParam.s_nor_param.ui_fat_length;
            m_sFmtArg.m_format_cfg.m_validation        = m_sDnParam.s_nor_param.b_validation;
        }
    } // switch()

    // m_format_cfg
    m_sFmtArg.m_format_cfg.m_auto_format_fat   = _FALSE;
    m_sFmtArg.m_erase_flag = NUTL_ERASE;
    m_sFmtArg.m_cb_format_report_init     = ::cb_format_report_init;
    m_sFmtArg.m_cb_format_report_init_arg = this;
    m_sFmtArg.m_cb_format_report          = ::cb_format_report;
    m_sFmtArg.m_cb_format_report_arg      = this;
    m_sFmtArg.m_cb_format_statistics      = ::cb_format_statistics;
    m_sFmtArg.m_cb_format_statistics_arg  = this;

    m_sFmtArg.m_enable_wdt_timeout      = _FALSE;
    m_sFmtArg.m_ms_wdt_timeout_interval = 0;

}

void TBOOT1::ReadbackArgSetting()
{
    m_sRBArg.m_storage_type = frmMultiLoad->m_Storage_Type;//HW_STORAGE_NAND;
    // an object of RB_HANDLE
    m_sRBArg.m_rb_handle = m_hRB;
    // the accuracy of the read back progress
    m_sRBArg.m_readback_accuracy = ACCURACY_AUTO;
    // readback call back funciton
    m_sRBArg.m_cb_readback_flash_init = cb_readback_flash_init;
    m_sRBArg.m_cb_readback_flash_init_arg = this;
    m_sRBArg.m_cb_readback_flash = cb_readback_flash;
    m_sRBArg.m_cb_readback_flash_arg = this;
}

int _fastcall TBOOT1::DoPriorOperation(void)
{
    AnsiString tmp;
    int ret = 0;

    switch (0)
    {
        case 0  /*OP_DF_WITH_BK_RS_NVRAM*/:
        {
            if(MT6516_AP == frmMultiLoad->m_chip_type)
            {
                return S_DONE;
            }
            else if(MT6573 == frmMultiLoad->m_chip_type)
            {
                ret = BackupBinRegionData();
            }


            if(S_DONE != ret)
            {
                return -1;
            }
            break;

        }
        case 1 /*OP_DF_RS_NVRAM*/:
        {
            if(MT6516_AP == frmMultiLoad->m_chip_type)
            {
                return S_DONE;
            }
            else if(MT6573 == frmMultiLoad->m_chip_type)
            {
                //ret = CheckBinRegionData();
            }


            if(0 != ret)
            {
                return -1;
            }
            break;

        }
        case 2 /*OP_RS_NVRAM_ONLY*/:
            break;

        default:
            break;
    }

    return 0;

}


int _fastcall TBOOT1::DoPostOperation(void)
{
    AnsiString tmp;
    int ret =0;

    switch (0)
    {
        case 0 /*OP_DF_WITH_BK_RS_NVRAM*/:
        case 1 /*OP_DF_RS_NVRAM*/:
        case 2 /*OP_RS_NVRAM_ONLY*/:
        {
            if(MT6516_AP == frmMultiLoad->m_chip_type)
            {
                //ret = RestoreNVRAMData();
                return S_DONE;
            }
            else if(MT6573 == frmMultiLoad->m_chip_type)
            {
                ret = CleanBinRegionData();
            }


            if(0 != ret)
            {
                tmp.sprintf(" (%d)", ret);
                m_asMsg += tmp;
                return -1;
            }
            break;

        }

        default:
            break;
    }

    return 0;
}

int __fastcall TBOOT1::FormatAll()
{
    ret = S_DONE;

    //Connect and Change baudrate
    if(false == m_is_DA_connected)
    {
    	//Connect with DA and change baudrate
         GC_TRACE("[COM%d] FormatAll(): DA connect...", m_sComPortSetting.com.number);
    	ret  = DAConnect();
    	if(S_DONE != ret)
    	{
    	    GC_TRACE("[COM%d] FormatAll(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
    	    return ret;
    	}

    	m_is_DA_connected = true;
    }


    ret = FM_Android_All();
    if(S_DONE != ret)
    {
        return ret;
    }


    DisConnectWithDA();

    return ret;
}

int __fastcall TBOOT1::FormatBootloader()
{
    ret = S_DONE;
   
    //Connect and Change baudrate
    if(false == m_is_DA_connected)
    {
    	//Connect with DA and change baudrate
         GC_TRACE("[COM%d] FM_Android_Bootloader(): DA connect...", m_sComPortSetting.com.number);
    	ret  = DAConnect();
    	if(S_DONE != ret)
    	{
    	    GC_TRACE("[COM%d] FM_Android_Bootloader(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
    	    return ret;
    	}

    	m_is_DA_connected = true;
    }


    ret = FM_Android_Bootloader();
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Bootloader(): [ERROR] Format bootloader fail!", m_sComPortSetting.com.number);
        return ret;
    }


    ret = FlashTool_Disconnect(&m_ft_api_handle);
    m_is_DA_connected = false;

    return ret;
}


static bool FriendlyToNum(char * friendly, int * num)
{
    char strNum[20];
    memset(strNum, 0, 20);

    char * tmp = friendly;
    char * start = NULL;
    char * end = NULL;

    while(1)
    {
        tmp = strstr(tmp, "COM");
        if(NULL != tmp)
        {
            tmp += strlen("COM");
            start = tmp;
        }
        else
        {
            break;
        }
    }

    if(NULL != start)
    {
        int count = 0;
        end = start;
        while(1)
        {
            if(('0' <= *end) && ('9' >= *end))
            {
               count++;
               end++;
            }
            else
            {
               break;
            }
        }

        memcpy(strNum, start, count);
        *num = atoi(strNum);
        if(0 == *num)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

int __fastcall TBOOT1::DAFullSpeedDownloadWithoutReset()
{

    FlashTool_Download_Result  dl_result;
    FlashTool_Download_Arg     dl_arg;

    dl_arg.m_dl_handle       = m_sFtDlArg.m_dl_handle;
    dl_arg.m_dl_handle_list  = m_sFtDlArg.m_dl_handle_list;

    dl_arg.m_cb_da_report        = NULL;
    dl_arg.m_cb_da_report_arg    = NULL;

    dl_arg.m_download_accuracy   =   m_sFtDlArg.m_download_accuracy;
    dl_arg.m_cb_download_flash_init =  m_sFtDlArg.m_cb_download_flash_init;
    dl_arg.m_cb_download_flash_init_arg =  m_sFtDlArg.m_cb_download_flash_init_arg;
    dl_arg.m_cb_download_flash         =   m_sFtDlArg.m_cb_download_flash;
    dl_arg.m_cb_download_flash_arg     = m_sFtDlArg.m_cb_download_flash_arg;

    dl_arg.m_cb_download_bloader_init     = cb_download_bloader_init_android;
    dl_arg.m_cb_download_bloader_init_arg = m_sFtDlArg.m_cb_download_bloader_init_arg;
    dl_arg.m_cb_download_bloader          = cb_download_bloader_android;
    dl_arg.m_cb_download_bloader_arg      = m_sFtDlArg.m_cb_download_bloader_arg;
    dl_arg.m_cb_security_post_process_notify = m_sFtDlArg.m_cb_security_post_process_notify;
    dl_arg.m_cb_security_post_process_notify_arg = m_sFtDlArg.m_cb_security_post_process_notify_arg;

    dl_arg.m_enable_tgt_res_layout_check   = _FALSE;
    dl_arg.m_enable_bbchip_ver_check       = _FALSE;
    dl_arg.m_downloadstyle_sequential      = _FALSE;

    dl_arg.m_cb_checksum_init = m_sFtDlArg.m_cb_checksum_init;
    dl_arg.m_cb_checksum_init_arg = m_sFtDlArg.m_cb_checksum_init_arg;
    dl_arg.m_cb_checksum = m_sFtDlArg.m_cb_checksum;
    dl_arg.m_cb_checksum_arg = m_sFtDlArg.m_cb_checksum_arg;

    GC_TRACE("[COM%d] DAFullSpeedDownloadWithoutReset(): Do download...", m_sComPortSetting.com.number);
    ret = FlashTool_Download(m_ft_api_handle, &dl_arg, &dl_result);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] DAFullSpeedDownloadWithoutReset(): [ERROR] Download fail!", m_sComPortSetting.com.number);
        return ret;
    }

    return ret;
}

//
int __fastcall TBOOT1::WatchDogResetForAutoTest()
{
    int iRet = S_DONE;
	GC_TRACE("[COM%d] WatchDogResetForAutoTest(): AutoTest Enalbe", m_sComPortSetting.com.number);

	if(MainForm->m_FmPreloader == 1)
	{
	   iRet = FM_Android_Bootloader();
	   if( iRet != S_DONE)
	   	   return iRet;
	}
      
	
	iRet = DoWatchDogReset();
    return iRet;
}

int __fastcall TBOOT1::DoWatchDogReset(bool bReboot2Charger)
{
    int iRet = S_DONE;
    GC_TRACE("[COM%d] DoWatchDogReset(): Watchdog reset...", m_sComPortSetting.com.number);
    FlashTool_EnableWDT_Arg wdt_arg;
    memset(&wdt_arg, 0, sizeof(FlashTool_EnableWDT_Arg));
    wdt_arg.m_timeout_ms = 5000;
    wdt_arg.m_async = _FALSE;
    wdt_arg.m_reboot = (_BOOL)bReboot2Charger;
    ret = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, &wdt_arg);
    DisConnectWithDA();
	return iRet;
}

//
int __fastcall TBOOT1::ForceWatchDogReset()
{
    int iRet = S_DONE;
    time_t	 start, finish;
    GC_TRACE("[COM%d] ForceWatchDogReset(): Enter...", m_sComPortSetting.com.number);
    double dTime;
    time(&start);
    while(1)
    {
        time(&finish);
        dTime = difftime(finish, start);
        if(difftime(finish, start) >= MainForm->m_ForceWdtDogResetTimeout)
        {
            GC_TRACE("[COM%d] ForceWatchDogReset(): difftime = %f, Enter watchdog reset flow.", m_sComPortSetting.com.number, difftime(finish, start));
            iRet = DoWatchDogReset(false);
            break;
        }
        Sleep(500);
    }
    return iRet;
}

int __fastcall TBOOT1::DAFullSpeedDownload()
{

    FlashTool_Download_Result  dl_result;
    FlashTool_Download_Arg     dl_arg;

    dl_arg.m_dl_handle       = m_sFtDlArg.m_dl_handle;
    dl_arg.m_dl_handle_list  = m_sFtDlArg.m_dl_handle_list;

    dl_arg.m_cb_da_report        = NULL;
    dl_arg.m_cb_da_report_arg    = NULL;

    dl_arg.m_download_accuracy   =   m_sFtDlArg.m_download_accuracy;
    dl_arg.m_cb_download_flash_init =  m_sFtDlArg.m_cb_download_flash_init;
    dl_arg.m_cb_download_flash_init_arg =  m_sFtDlArg.m_cb_download_flash_init_arg;
    dl_arg.m_cb_download_flash         =   m_sFtDlArg.m_cb_download_flash;
    dl_arg.m_cb_download_flash_arg     = m_sFtDlArg.m_cb_download_flash_arg;

    dl_arg.m_cb_download_bloader_init     = cb_download_bloader_init_android;
    dl_arg.m_cb_download_bloader_init_arg = m_sFtDlArg.m_cb_download_bloader_init_arg;
    dl_arg.m_cb_download_bloader          = cb_download_bloader_android;
    dl_arg.m_cb_download_bloader_arg      = m_sFtDlArg.m_cb_download_bloader_arg;
    dl_arg.m_cb_security_post_process_notify = m_sFtDlArg.m_cb_security_post_process_notify;
    dl_arg.m_cb_security_post_process_notify_arg = m_sFtDlArg.m_cb_security_post_process_notify_arg;

    dl_arg.m_cb_checksum_init = m_sFtDlArg.m_cb_checksum_init;
    dl_arg.m_cb_checksum_init_arg = m_sFtDlArg.m_cb_checksum_init_arg;
    dl_arg.m_cb_checksum = m_sFtDlArg.m_cb_checksum;
    dl_arg.m_cb_checksum_arg = m_sFtDlArg.m_cb_checksum_arg;

    dl_arg.m_enable_tgt_res_layout_check   = _FALSE;
    dl_arg.m_enable_bbchip_ver_check       = _FALSE;
    dl_arg.m_downloadstyle_sequential      = _FALSE;

    return FlashTool_Download(m_ft_api_handle, &dl_arg, &dl_result);
}

int __fastcall TBOOT1::SwitchComport()
{
    int ret = S_DONE;
    FlashTool_USB_Status_Arg usb_status_arg;
    usb_status_arg.m_cb_usb_status_init = cb_checkusbstatus_init;
    usb_status_arg.cb_usb_status_init_arg = this;

    FlashTool_USB_Status_Result  usb_statue_result;
    usb_statue_result.usb_speed_status = USB_STATUS_UNKNOWN;

    ret = FlashTool_CheckUSBStatus(m_ft_api_handle, &usb_status_arg, &usb_statue_result);

    if (usb_statue_result.usb_speed_status != USB_FULL_SPEED) // If usb is not full speed status, do not switch.
    {
        GC_TRACE("[COM%d] SwitchComport(): The com port is in high speed, do not switch it.", m_sComPortSetting.com.number);
        return S_DONE;
    }
    int index = GetCOMPortIndex();
    int com_port;
    string   s_com_symbol;
    time_t   start, finish;

    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);
    bool * p_is_enter = MainForm->_t_manager.Get_enter_mpdl_flag_addr(index);
    * p_stopflag = FALSE;


    if(MainForm->DAPort[index] == -1)
    {
        GC_TRACE("[COM%d] SwitchComport(): Scan DA com port firstly, enter into multiport download queue...", m_sComPortSetting.com.number);
        MainForm->_t_manager.Apply_enter_mpdl_queue(index);

        while(!(*p_is_enter))
        {
            GC_TRACE("[COM%d] SwitchComport(): Wait enter download section...", m_sComPortSetting.com.number);
            if(BOOT_STOP == *p_stopflag)
            {
                MainForm->_t_manager.Enter_mpdl_finish(index);
                return -1;
            }

            Sleep(100);
        }

        //Scan exist com port
        GC_TRACE("[COM%d] SwitchComport(): Begin FIRST scan USB COM port...", m_sComPortSetting.com.number);
        m_exist_com_port.erase(m_exist_com_port.begin(), m_exist_com_port.end());
        ScanFriendlyNameByDDK(m_exist_com_port_str);

        GC_TRACE("[COM%d] SwitchComport(): Setup USB DL...", m_sComPortSetting.com.number);
        ret = FlashTool_SetupUSBDL(&m_ft_api_handle, 1);
        if( S_DONE != ret )
        {
            GC_TRACE("[COM%d] SwitchComport(): [ERROR] Setup USB DL fail!", m_sComPortSetting.com.number);
            return ret;
        }

        //Get start time
        time(&start);
        GC_TRACE("[COM%d] SwitchComport(): Begin SECOND scan USB COM port...", m_sComPortSetting.com.number);
        do
        {
            if(BOOT_STOP == *p_stopflag)
            {
                GC_TRACE("[COM%d] SwitchComport(): User press stop button!", m_sComPortSetting.com.number);
                MainForm->_t_manager.Enter_mpdl_finish(index);
                return -1;
            }

            if(GetNewPortFriendlyName(s_com_symbol))
            {
                //find usb_com port
                GC_TRACE("[COM%d] SwitchComport(): Find USB COM port successfully!(%s)", m_sComPortSetting.com.number, s_com_symbol.c_str());
                break;
            }

            //Get current time
            time( &finish );

            //wait 10s if there still not found a new usb com port
            //then we quite
            if(difftime( finish, start ) > 30 /* 30s */)
            {
            GC_TRACE("[COM%d] SwitchComport(): Search USB COM port timeout!!(30s)", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(index);
            ret = 9000;    //error status  can't find usb port
            return -1;
            }

            Sleep(100);

        }while(1);
        MainForm->_t_manager.Enter_mpdl_finish(index);

        char friendly[256];
        memset(friendly, 0, 256);
        memcpy(friendly, s_com_symbol.c_str(), 256);
        if(!FriendlyToNum(friendly, &com_port))
        {
        GC_TRACE("[COM%d] SwitchComport(): [ERROR] Transfer symbolic to num fail!", m_sComPortSetting.com.number);
        return -1;
        }
        MainForm->DAPort[index] = com_port;
     }
    else
    {
        GC_TRACE("[COM%d] SwitchComport(): After first scanning DA COM, get the DA COM port directly from DA COM port array.", m_sComPortSetting.com.number);
        ret = FlashTool_SetupUSBDL(&m_ft_api_handle, 1);
        if( S_DONE != ret )
        {
            GC_TRACE("[COM%d] (): [ERROR] Setup USB DL fail!", m_sComPortSetting.com.number);
            return ret;
        }

        com_port = MainForm->DAPort[index];
        HANDLE hCOM = NULL;

        //int portNumber = GetCOMPortNumber();
        int i = GetCOMPortIndex();
        int * p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);

        char comStr[128];
        sprintf(comStr,"\\\\.\\COM%d", com_port);

        //Get start time
        time(&start);
        GC_TRACE("[COM%d] SwitchComport(): Try to open COM%d port...(Polling),comStr = %s", m_sComPortSetting.com.number, com_port, comStr);
        do
        {
            if(BOOT_STOP == *p_stopflag)
            {
                GC_TRACE("[COM%d] CheckPortAvailable(): User press stop button!", m_sComPortSetting.com.number);
                return -1;
            }

            hCOM = CreateFile( comStr, GENERIC_READ |GENERIC_WRITE, 0, NULL,
												OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

            if(INVALID_HANDLE_VALUE == hCOM)
            {
                //GC_TRACE("[COM%d] CheckPortAvailable(): Cannot open USB port!", m_sComPortSetting.com.number);
            }
            else
            {
                GC_TRACE("[COM%d] CheckPortAvailable(): ++++++Open USB succeed!++++++", m_sComPortSetting.com.number);
                CloseHandle(hCOM);
                //return 0;
                break;
            }
						//Get current time
            time( &finish );

            //wait 10s if there still not found a new usb com port
            //then we quite
            if(difftime( finish, start ) > 30 /* 30s */)
            {
                GC_TRACE("[COM%d] SwitchComport(): Polling USB COM port timeout!!(30s)", m_sComPortSetting.com.number);
                MainForm->_t_manager.Enter_mpdl_finish(index);
                ret = 9000;    //error status  can't find usb port
                return -1;
            }

            Sleep(100);
        }while(1);
    }

    //Set protected flag for closing USB COM issue
    int i = GetCOMPortIndex();
    MainForm->_t_manager.Set_usb_protected_flag(i);

    GC_TRACE("[COM%d] SwitchComport(): Change COM port to COM%d", m_sComPortSetting.com.number, com_port);
    ret = FlashTool_ChangeCOM(&m_ft_api_handle, com_port);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] SwitchComport(): [ERROR] Change COM port fail!", m_sComPortSetting.com.number);
        return ret;
    }

    return ret;
}

int __fastcall TBOOT1::DAHighSpeedDownloadWithoutReset()
{
#if 0
    int ret = S_DONE;
    int index = GetCOMPortIndex();
    int com_port;
    string   s_com_symbol;
    time_t   start, finish;

    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);
    bool * p_is_enter = MainForm->_t_manager.Get_enter_mpdl_flag_addr(index);
    * p_stopflag = FALSE;


    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Enter into multiport download queue...", m_sComPortSetting.com.number);
    MainForm->_t_manager.Apply_enter_mpdl_queue(index);

    while(!(*p_is_enter))
    {
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Wait enter download section...", m_sComPortSetting.com.number);

        if(BOOT_STOP == *p_stopflag)
        {
            MainForm->_t_manager.Enter_mpdl_finish(index);
            return -1;
        }

        Sleep(200);
    }

    //Scan exist com port
    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Begin FIRST scan USB COM port...", m_sComPortSetting.com.number);
    m_exist_com_port.erase(m_exist_com_port.begin(), m_exist_com_port.end());
    ScanFriendlyNameByDDK(m_exist_com_port_str);


    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Setup USB DL...", m_sComPortSetting.com.number);
    ret = FlashTool_SetupUSBDL(&m_ft_api_handle, 1);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): [ERROR] Setup USB DL fail!", m_sComPortSetting.com.number);
        return ret;
    }


    //Get start time
    time(&start);
    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Begin SECOND scan USB COM port...", m_sComPortSetting.com.number);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): User press stop button!", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(index);
            return -1;
        }

        if(GetNewPortFriendlyName(s_com_symbol))
        {
            //find usb_com port
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Find USB COM port successfully!(%s)", m_sComPortSetting.com.number, s_com_symbol.c_str());
            break;
        }

        //Get current time
        time( &finish );

        //wait 10s if there still not found a new usb com port
        //then we quite
        if(difftime( finish, start ) > 30 /* 30s */)
        {
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Search USB COM port timeout!!(30s)", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(index);
            ret = 9000;    //error status  can't find usb port
            return -1;
        }

		Sleep(1000);

    }while(1);

    MainForm->_t_manager.Enter_mpdl_finish(index);

    char friendly[256];
    memset(friendly, 0, 256);
    memcpy(friendly, s_com_symbol.c_str(), 256);
    if(!FriendlyToNum(friendly, &com_port))
    {
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): [ERROR] Transfer symbolic to num fail!", m_sComPortSetting.com.number);
        return -1;
    }


    //Set protected flag for closing USB COM issue
    int i = GetCOMPortIndex();
    MainForm->_t_manager.Set_usb_protected_flag(i);

    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Change COM port to COM%d", m_sComPortSetting.com.number, com_port);
    ret = FlashTool_ChangeCOM(&m_ft_api_handle, com_port);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): [ERROR] Change COM port fail!", m_sComPortSetting.com.number);
        return ret;
    }
#endif
	//int ret = SwitchComport();
	/* if (S_DONE != ret)
	{

		GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): [ERROR] Change COM port fail!", m_sComPortSetting.com.number);
        return ret;
	}*/

    ///////////////////////////////////////////////////////////
    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Set download parameters", m_sComPortSetting.com.number);
    FlashTool_Download_Result  dl_result;
    FlashTool_Download_Arg     dl_arg;
    memset(&dl_result,0, sizeof(dl_result));
	memset(&dl_arg,0, sizeof(FlashTool_Download_Arg));
    dl_arg.m_dl_handle       = m_sFtDlArg.m_dl_handle;
    dl_arg.m_dl_handle_list  = m_sFtDlArg.m_dl_handle_list;

    dl_arg.m_cb_da_report        = NULL;
    dl_arg.m_cb_da_report_arg    = NULL;

    dl_arg.m_download_accuracy   =   m_sFtDlArg.m_download_accuracy;
    dl_arg.m_cb_download_flash_init =  m_sFtDlArg.m_cb_download_flash_init;
    dl_arg.m_cb_download_flash_init_arg =  m_sFtDlArg.m_cb_download_flash_init_arg;
    dl_arg.m_cb_download_flash         =   m_sFtDlArg.m_cb_download_flash;
    dl_arg.m_cb_download_flash_arg     = m_sFtDlArg.m_cb_download_flash_arg;

    dl_arg.m_cb_download_bloader_init     = cb_download_bloader_init_android;
    dl_arg.m_cb_download_bloader_init_arg = m_sFtDlArg.m_cb_download_bloader_init_arg;
    dl_arg.m_cb_download_bloader          = cb_download_bloader_android;
    dl_arg.m_cb_download_bloader_arg      = m_sFtDlArg.m_cb_download_bloader_arg;
    dl_arg.m_cb_security_post_process_notify = m_sFtDlArg.m_cb_security_post_process_notify;
    dl_arg.m_cb_security_post_process_notify_arg = m_sFtDlArg.m_cb_security_post_process_notify_arg;

    dl_arg.m_cb_checksum_init = m_sFtDlArg.m_cb_checksum_init;
    dl_arg.m_cb_checksum_init_arg = m_sFtDlArg.m_cb_checksum_init_arg;
    dl_arg.m_cb_checksum = m_sFtDlArg.m_cb_checksum;
    dl_arg.m_cb_checksum_arg = m_sFtDlArg.m_cb_checksum_arg;

    dl_arg.m_enable_tgt_res_layout_check   = _FALSE;
    dl_arg.m_enable_bbchip_ver_check       = _FALSE;
    dl_arg.m_downloadstyle_sequential      = _FALSE;

    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Do download...", m_sComPortSetting.com.number);
    ret = FlashTool_Download(m_ft_api_handle, &dl_arg, &dl_result);
    if( S_DONE != ret )
    {
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Download fail!", m_sComPortSetting.com.number);
        return ret;
    }


//For multi-thread control
#if 0
    //Release protected flag for closing USB COM issue
    MainForm->_t_manager.Release_usb_protected_flag(i);


    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Wait all thread finish...", m_sComPortSetting.com.number);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): User press stop button!", m_sComPortSetting.com.number);
            return -1;
        }

        Sleep(500);
        if (!MainForm->_t_manager.Is_any_other_in_usb_protected_mode(i))
        {
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): No protected flag set.", m_sComPortSetting.com.number);
            /*
            ret = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, 5000);
            if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] FlashTool_EnableWatchDogTimeout() Fail", index);
                FlashTool_Disconnect(&m_ft_api_handle);
                return ret;
            }
            */

            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): DA disconnect...", m_sComPortSetting.com.number);
            ret = FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            break;
        }
    } while(1);
#endif


//No multi-thread control
#if 0
    GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): DA disconnect...", m_sComPortSetting.com.number);
    ret = FlashTool_Disconnect(&m_ft_api_handle);
    m_is_DA_connected = false;
#endif

/*
//For auto test
    if(MainForm->m_EnableWdtDogReset == 1){
        GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Watchdog reset...", m_sComPortSetting.com.number);
        FlashTool_EnableWDT_Arg wdt_arg;
        memset(&wdt_arg, 0, sizeof(FlashTool_EnableWDT_Arg));
        wdt_arg.m_timeout_ms = 3000;
		    wdt_arg.m_async = _FALSE;
		    wdt_arg.m_reboot = _TRUE;
        ret = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, &wdt_arg);
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] DAHighSpeedDownloadWithoutReset(): Watchdog reset fail!", m_sComPortSetting.com.number);
            DisConnectWithDA();
            return ret;
        }
    }
*/
    return ret;
}


//==============================================================================
int __fastcall TBOOT1::DAConnect()
{
    int ret;
    int index = GetCOMPortIndex();
    int force_charge = FORCE_CHARGE_ON;

    if(MainForm->m_miDADLNoBattery->Checked)
    {
        force_charge = FORCE_CHARGE_ON;
    }
    else if(MainForm->m_miDADLBattery->Checked)
    {
        force_charge = FORCE_CHARGE_OFF;
    }
    else if(MainForm->m_miDADLAutoDetect->Checked)
    {
        force_charge = FORCE_CHARGE_AUTO;
    }

    /* set FlashTool_Connect_Arg */
    m_sConnArg.m_com_ms_read_timeout  =  COM_DEFAULT_TIMEOUT;
    m_sConnArg.m_com_ms_write_timeout =  COM_DEFAULT_TIMEOUT;
    BootArgSetting(m_sConnArg.m_boot_arg);
    m_sConnArg.m_cb_security_pre_process_notify = NULL;
    m_sConnArg.m_cb_security_pre_process_notify_arg = NULL;
    m_sConnArg.m_nor_chip_select[0] = CS_0;
    m_sConnArg.m_nor_chip_select[1] = CS_WITH_DECODER;
    m_sConnArg.m_nand_chip_select   = m_sDnParam.s_bbchip_op.b_enable_nfi_cs1?CS_1:CS_0;
    m_sConnArg.m_p_nand_acccon      = NULL  ;
    m_sConnArg.m_boot_arg.m_bmt_block_count = frmMultiLoad->m_BMT_blocks;
    m_sConnArg.m_storage_type = frmMultiLoad->m_Storage_Type;// m_storage_type
    m_sConnArg.m_force_charge = force_charge;

    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
    m_sConnArg.m_p_dl_handle = *p_dl_handle;
    m_sConnArg.m_reset_key = m_sDnParam.m_reset_key;
	GC_TRACE("[COM%d] DAConnect(): DA reset key is 0x%x", m_sComPortSetting.com.number, m_sConnArg.m_reset_key);
	
    //GC_TRACE("[TAG]: %s(): [COM%d]: m_p_dl_handle(%p).(%s, %d).",
    //    __FUNC__, m_sComPortSetting.com.number, m_sConnArg.m_p_dl_handle,//*p_dl_handle,
    //    __FILE__, __LINE__);

    /* connect to target */
    ret = FlashTool_Connect(
                            /*index + m_sDnParam.i_FirstComNo,*/
                            m_sComPortSetting.com.number,
                            &m_sConnArg,
                            &m_sConnResult,
                            NULL,
                            MainForm->_t_manager.Get_stop_flag_addr(index),
                            &m_ft_api_handle);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] DAConnect(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        return ret;
    }

    ret = UpdateBMTCount();
    if(ret!= S_DONE && ret != S_INVALID_BMTSIZE)
    {
        GC_TRACE("[COM%d] DAConnect(): [ERROR] UpdateBMTCount fail!", m_sComPortSetting.com.number);
        return ret;
    }
    /*
    FlashTool_ChangeBaudrate_Arg  ft_cb_arg;
    ft_cb_arg.m_baudrate                       = MainForm->Get_Baudrate();
    ft_cb_arg.m_baudrate_full_sync_count       = 1;
    ft_cb_arg.m_cb_set_high_speed_baudrate     = NULL;
    ft_cb_arg.m_cb_set_high_speed_baudrate_arg = NULL;

    ret = FlashTool_ChangeBaudrate(m_ft_api_handle, &ft_cb_arg);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FlashTool_ChangeBaudrate(): [ERROR] DA change baudrate fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        return ret;
    }*/

    return ret;
}

int __fastcall TBOOT1::UpdateBMTCount()
{
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
	unsigned int bmt_block_count = 0;

    if(frmMultiLoad->m_Storage_Type != HW_STORAGE_NAND)
	{
	    GC_TRACE("[COM%d] UpdateBMTCount(): [Warming] not nand flash, do not need update bmt count!", m_sComPortSetting.com.number);
        return S_DONE;
	}

	frmMultiLoad->m_BMT_blocks = 80;
    /*Update BMT counts for Combo nand feature of MT6572, the nand page size maybe 2k/4k, and after da connect
        flashtoollib can get the page size and update the bmt count correctly*/
    ret = DL_GetBMTBlockCount(*p_dl_handle, &bmt_block_count);
	//if(ret != S_DONE)
    if(S_DONE != ret && ret != S_INVALID_BMTSIZE)
	{
	    GC_TRACE("[COM%d] DL_GetBMTBlockCount(): [ERROR] Get BMT Block count fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        return ret;
	}
	GC_TRACE("[COM%d] DAConnect(): Get BMT Block count success,BMT count is %d!", m_sComPortSetting.com.number, bmt_block_count);
    frmMultiLoad->m_BMT_blocks = bmt_block_count;
	m_sConnArg.m_boot_arg.m_bmt_block_count = frmMultiLoad->m_BMT_blocks;
	return ret;
}


//==============================================================================
int __fastcall TBOOT1::DL_Android_All()
{
    int ret = S_DONE;
    int index = GetCOMPortIndex();

    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);
    bool * p_is_enter = MainForm->_t_manager.Get_enter_mpdl_flag_addr(index);
    * p_stopflag = FALSE;


    // Fill download argument
    GC_TRACE("[COM%d] DL_Android_All(): Fill download parameters", m_sComPortSetting.com.number);
    DownloadArgSetting();
    m_sFtArg.m_enable_ui_dram_cfg = _FALSE;
    prefix = "DL ";

    GC_TRACE("[COM%d] DL_Android_All(): Check if image need to be downloaded", m_sComPortSetting.com.number);
    if(!frmMultiLoad->IsDLAnyImage())
    {
        //Application->MessageBox("Please select images for download", "FAIL", MB_OK);
        GC_TRACE("[COM%d] DL_Android_All(): No images is selected to be download!", m_sComPortSetting.com.number);
        return -1;
    }


	try
	{
		if(true == MainForm->m_miEnableDADL->Checked)
		{
			if(true == MainForm->m_miDADLFullSpeed->Checked)
			{
				  GC_TRACE("[COM%d] DL_Android_All(): DA full speed download all images...", m_sComPortSetting.com.number);
				  ret = DAFullSpeedDownloadWithoutReset();
				  if(S_DONE != ret)
				  {
					  GC_TRACE("[COM%d] DL_Android_All(): [ERROR] DA full speed download fail!", m_sComPortSetting.com.number);
				  	FlashTool_Disconnect(&m_ft_api_handle);
					  m_is_DA_connected = false;
				  	goto clean;
				  }


			}
			else if(true == MainForm->m_miDADLHighSpeed->Checked)
			{
				GC_TRACE("[COM%d] DL_Android_All(): DA high speed download all images...", m_sComPortSetting.com.number);
				ret = DAHighSpeedDownloadWithoutReset();
				if(S_DONE != ret)
				{
					GC_TRACE("[COM%d] DL_Android_All(): [ERROR] DA high speed download fail!", m_sComPortSetting.com.number);
					FlashTool_Disconnect(&m_ft_api_handle);
					m_is_DA_connected = false;
					goto clean;
				}
			}
			return ret;
		}
		else
    {
			GC_TRACE("[COM%d] DL_Android_All(): DA + preloder download all images...", m_sComPortSetting.com.number);
			GC_TRACE("[COM%d] DL_Android_All(): Enter into multiport download queue...", m_sComPortSetting.com.number);
			MainForm->_t_manager.Apply_enter_mpdl_queue(index);

			while(!(*p_is_enter))
			{
				GC_TRACE("[COM%d] DL_Android_All(): Wait enter download section...", m_sComPortSetting.com.number);

				if(BOOT_STOP == *p_stopflag)
				{
					MainForm->_t_manager.Enter_mpdl_finish(index);
					return -1;
				}

				Sleep(200);
			}

			//Fill download parameters
			GC_TRACE("[COM%d] DL_Android_All(): Fill download image info...", m_sComPortSetting.com.number);
			if(-1 == frmMultiLoad->FillDownloadImageInfo() )
			{
				GC_TRACE("[COM%d] DL_Android_All(): [ERROR] Fill download image info fail!");
				throw -1;
			}

			//Disable all rom files exclude preloader
			GC_TRACE("[COM%d] DL_Android_All(): Disable all rom files exclude preloader", m_sComPortSetting.com.number);
			if(-1 == frmMultiLoad->DisableDLAttribute() )
			{
				GC_TRACE("[COM%d] DL_Android_All(): [ERROR] Disable DL attribute fail!");
				throw -1;
			}

			//Download bootloader
			if(frmMultiLoad->IsDLBootloader())
			{
			         GC_TRACE("[COM%d] DL_Android_All(): Download preloader...", m_sComPortSetting.com.number);
				ret = DAFullSpeedDownload();
				if(S_DONE != ret)
				{
					GC_TRACE("[COM%d] DL_Android_All(): [ERROR] DA full speed download fail!", m_sComPortSetting.com.number);
					FlashTool_Disconnect(&m_ft_api_handle);
					m_is_DA_connected = false;
					frmMultiLoad->RestoreDLAttribute();
					goto clean;
				}
			}

			//Enable all rom files
			GC_TRACE("[COM%d] DL_Android_All(): Restore download image info", m_sComPortSetting.com.number);
			if (-1 == frmMultiLoad->RestoreDLAttribute())
			{
				GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] Restore DL attribute fail!");
				throw -1;
			}


        //For MT6516, set download control bit is still necessary
        if(MT6516_AP == frmMultiLoad->m_chip_type)
        {
            GC_TRACE("[COM%d] DL_Android_All(): Write USB download control bit...", m_sComPortSetting.com.number);
            ret  = FlashTool_WriteUSBDownloadControlBit(m_ft_api_handle);
            if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] DL_Android_All(): [ERROR] Write USB download control bit fail!", index);
                goto clean;
            }
        }

			//Set watchdog timeout and restore target
	    GC_TRACE("[COM%d] DL_Android_All(): Enable watchdog timeout...", m_sComPortSetting.com.number);

			FlashTool_EnableWDT_Arg wdt_arg;
			FlashTool_EnableWDT_Arg *p_wdt_arg = &wdt_arg;
			memset(p_wdt_arg, 0, sizeof(FlashTool_EnableWDT_Arg));
			//timeout to reset bootRom
			p_wdt_arg->m_timeout_ms = 5000;
			p_wdt_arg->m_async = _FALSE;
			p_wdt_arg->m_reboot = _TRUE;

			ret  = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, p_wdt_arg);
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] DL_Android_All(): [ERROR] Enable watchdog timeout fail!", index);
				goto clean;
			}

	    GC_TRACE("[COM%d] DL_Android_All(): DA disconnect...", m_sComPortSetting.com.number);
			ret = FlashTool_Disconnect(&m_ft_api_handle);
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] DL_Android_All(): [ERROR] DA disconnect fail!", m_sComPortSetting.com.number);
				goto clean;
			}

			m_is_DA_connected = false;

			//Download other images by eboot.dll
			if(frmMultiLoad->IsDLSystem())
			{
				//Scan exist com port
				GC_TRACE("[COM%d] DL_Android_All(): Begin first scan USB COM port...", m_sComPortSetting.com.number);
				m_exist_com_port.erase(m_exist_com_port.begin(), m_exist_com_port.end());
				ScanSymbolicNameByDDK(m_exist_com_port_str);


				GC_TRACE("[COM%d] DL_Android_All(): List download image info", m_sComPortSetting.com.number);
				for(int i = 0; i < ANDROID_IMAGE_FILE_NUMBER; i++)
				{
					GC_TRACE("[COM%d] DL_Android_All(): Path(%s), Format(%d), Type(%d), IsD(%d)", m_sComPortSetting.com.number,
						frmMultiLoad->m_image_files[i].m_path,
						frmMultiLoad->m_image_files[i].m_format,
						frmMultiLoad->m_image_files[i].m_type,
						frmMultiLoad->m_image_files[i].is_download);
				}


				if(/*OP_FIRMWARE_UPGRADE == MainForm->cbType->ItemIndex*/
				    PART_HAS_CHANGED == m_part_status)
	       {
            GC_TRACE("[COM%d] DL_Android_All(): Ready to restore and download all images...", m_sComPortSetting.com.number);
            if(!DownloadAndRestoreBinRegionByEboot())
            {
                MainForm->_t_manager.Enter_mpdl_finish(index);
                if(ret!=9000)
                return -1;
            }
        }
        else
        {
            GC_TRACE("[COM%d] DL_Android_All(): Ready to download all images...", m_sComPortSetting.com.number);
            if(!DownloadImagesByEboot())
            {
                MainForm->_t_manager.Enter_mpdl_finish(index);
                if(ret!=9000)
                return -1;
            }
        }

			}
			else
			{
				MainForm->_t_manager.Enter_mpdl_finish(index);
			}

			prefix = "ANDROID ";
			return ret;
		}

	}
	catch(...)
	{
		MainForm->_t_manager.Enter_mpdl_finish(index);
		* p_is_enter = FALSE;
		return -1;
	}

clean:
	MainForm->_t_manager.Enter_mpdl_finish(index);
	* p_is_enter = FALSE;
	return ret;
}


int __fastcall TBOOT1::DL_Android_Partial()
{
    return 0;
}


/*
Function name:FM_Android_All()
bool bToDoDisconnet: if bToDoDisconnet is true,we will do DA dicconnect when fail
*/
int __fastcall TBOOT1::FM_Android_All(bool bToDoDisconnet)
{
    prefix = "FR ";

    int iEmmcType = E_LINEAR_EMMC;

	GC_TRACE("[COM%d] FM_Android_All():bToDoDisconnet is %d, if bToDoDisconnet is 1, it will do DA DisConnect when fail!",  m_sComPortSetting.com.number,bToDoDisconnet?1:0);
    //=====================Format=======================================
    GC_TRACE("[COM%d] FM_Android_All(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;
	memset(&fm_res, 0, sizeof(FlashTool_Format_Result));

    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type;
    GC_TRACE("[COM%d] FM_Android_All(): fm_arg.m_storage_type is %d, nand = 1, Emmc = 2, SDMMC = 3.", m_sComPortSetting.com.number,fm_arg.m_storage_type);
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_PHYSICAL;
	fm_arg.m_erase_flag = NUTL_ERASE;

    fm_arg.m_format_cfg.m_format_begin_addr = 0;
    if (HW_STORAGE_NAND == fm_arg.m_storage_type)
    {
        if(frmMultiLoad->m_chip_type == MT8127){
		fm_arg.m_erase_flag = NUTL_FORCE_ERASE;
        GC_TRACE("[COM%d] FM_Android_All():MT8127,NUTL_FORCE_ERASE"  );       
		}
        fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size;
    }
    else if (HW_STORAGE_EMMC == fm_arg.m_storage_type)
    {
        DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
        CheckEmmcType(*p_dl_handle,iEmmcType);
        if(iEmmcType == E_COMBO_EMMC){
			
        }else{
            fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
                                                + m_sConnResult.m_da_report.m_emmc_boot2_size
                                                + m_sConnResult.m_da_report.m_emmc_rpmb_size
                                                + m_sConnResult.m_da_report.m_emmc_gp1_size
                                                + m_sConnResult.m_da_report.m_emmc_gp2_size
                                                + m_sConnResult.m_da_report.m_emmc_gp3_size
                                                + m_sConnResult.m_da_report.m_emmc_gp4_size
                                                + m_sConnResult.m_da_report.m_emmc_ua_size;
            unsigned int alignment = 512;
            unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
            if(0 != reminder )
            {
                GC_TRACE("[COM%d] FM_Android_All(): Before alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
                fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
                GC_TRACE("[COM%d] FM_Android_All(): After alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
            }
            GC_TRACE("[COM%d] FM_Android_All(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
                       fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
		}

    }
	else if(HW_STORAGE_SDMMC == fm_arg.m_storage_type)
	{
		fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_sdmmc_ua_size;

		unsigned int alignment = 512;
		unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
		if(0 != reminder )
		{
			GC_TRACE("[COM%d] FM_Android_All(): Before alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
			fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
			GC_TRACE("[COM%d] FM_Android_All(): After alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);

		}
		GC_TRACE("[COM%d] FM_Android_All(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
            fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
	}

    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

    GC_TRACE("[COM%d] FM_Android_All(): Do format...", m_sComPortSetting.com.number);
	if(iEmmcType == E_COMBO_EMMC){
		ret = FM_ComboEmmc_ALL(m_ft_api_handle);
	}else{
	    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
	}
    
    if(S_DONE != ret)
    {
    
        GC_TRACE("[COM%d] FM_Android_All(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        goto FM_ALL_Finish;
    }

    //check whether the bad block number is larger than BMT	count
    GC_TRACE("[COM%d] FM_Android_All(): Bad block number is %d!", m_sComPortSetting.com.number, fm_res.m_format_statistics.m_bad_blocks);
	if(fm_res.m_format_statistics.m_bad_blocks > (frmMultiLoad->m_BMT_blocks*2/3)){
		GC_TRACE("[COM%d] FM_Android_All(): [ERROR] Bad block number is larger than %d!", m_sComPortSetting.com.number, (frmMultiLoad->m_BMT_blocks*2/3));
		ret = -1;
		goto FM_ALL_Finish;
	}

FM_ALL_Finish:
	if(bToDoDisconnet == true && ret != S_DONE)  //when error happen, we will do DA disconnect if bToDoDisconnet == true
	{
	    FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
	}
	
    return ret;
}


int __fastcall TBOOT1::FM_Android_All_ExeceptOTP()
{
    prefix = "FR ";

#if 0
    //=====================Connect and Change baudrate====================
    if(false == m_is_DA_connected)
    {
    	//Connect with DA and change baudrate
         GC_TRACE("[COM%d] FM_Android_All(): DA connect...", m_sComPortSetting.com.number);
    	ret  = DAConnect();
    	if(S_DONE != ret)
    	{
    	    GC_TRACE("[COM%d] FM_Android_All(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
    	    return ret;
    	}

    	m_is_DA_connected = true;
    }
#endif

    //=====================Format=======================================
    GC_TRACE("[COM%d] ge(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type;//HW_STORAGE_NAND;
    GC_TRACE("[fm_arg.m_storage_type is %d .]", fm_arg.m_storage_type);
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_PHYSICAL;

    fm_arg.m_format_cfg.m_format_begin_addr = 0;
    if (HW_STORAGE_NAND == fm_arg.m_storage_type)
    {
        fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size;
    }
    else if (HW_STORAGE_EMMC == fm_arg.m_storage_type)
    {
        // The eMMC flash is too large, so we only format the load part.
		/*if (frmMultiLoad->m_Load_End_Address > 0)
		{
			fm_arg.m_format_cfg.m_format_length = frmMultiLoad->m_Load_End_Address;

		}
		else
		{
			fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_ua_size;
		}
        */
		//fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_ua_size;
	   ULL64 OTPNum;
	   ULL64 BMTNum;

	   PART_INFO partOTPInfo;
	   PART_INFO partBMTInfo;
	   ret = GetPartitionInfo(SCATTER_OTP_NAME, partOTPInfo);
	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d] FM_Android_All_ExeceptOTP(): [ERROR]Getting readback PMT information fail", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: Get OTP Partition info fail!";
		  FlashTool_Disconnect(&m_ft_api_handle);
		  m_is_DA_connected = false;
		  return ret;
	   }
	   OTPNum = partOTPInfo.begin_addr & 0x0000FFFF;
       
       ret = GetPartitionInfo(SCATTER_RSV_BMT_NAME, partBMTInfo);
	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d FM_Android_All_ExeceptOTP(): [ERROR]Getting readback BMT information fail]", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: Get BMT Partition info fail!";
		  FlashTool_Disconnect(&m_ft_api_handle);
		  m_is_DA_connected = false;
		  return ret;
	   }
	   BMTNum = partBMTInfo.begin_addr & 0x0000FFFF;
		fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
											+ m_sConnResult.m_da_report.m_emmc_boot2_size
											+ m_sConnResult.m_da_report.m_emmc_rpmb_size
											+ m_sConnResult.m_da_report.m_emmc_gp1_size
											+ m_sConnResult.m_da_report.m_emmc_gp2_size
											+ m_sConnResult.m_da_report.m_emmc_gp3_size
											+ m_sConnResult.m_da_report.m_emmc_gp4_size
											+ m_sConnResult.m_da_report.m_emmc_ua_size
											-OTPNum*128*1024;
		unsigned int alignment = 512;
		unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
		if(0 != reminder )
		{
			GC_TRACE("[COM%d] Before alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
			fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
			GC_TRACE("[COM%d] After alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);

		}
		GC_TRACE("[COM%d] FM_Android_All_ExeceptOTP(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
            fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);

    }


    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

    GC_TRACE("[COM%d] FM_Android_All_ExeceptOTP(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_All_ExeceptOTP(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    return ret;
}

// Format from NVRAM end address to FAT start address or end address.
int __fastcall TBOOT1::FM_Android_Partial()
{
    prefix = "FR ";

    //=====================Format=======================================
    GC_TRACE("[COM%d] FM_Android_Partial(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

	//Common part
    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type; //HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

    ULL64 startAddrPartial;
	//ULL64 pmtStartAddr;

    PART_INFO fatPartInfo;
    E_CHIP_GROUP eChipGroup;
    eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);
	memset(&fatPartInfo, 0, sizeof(PART_INFO));

    if(eChipGroup != BBCHIP_GROUP0) //MT6573 or MT6513 process flow.
    {
        //Get BinRegion Partition info
        PART_INFO partInfo;
        memset(&partInfo, 0, sizeof(PART_INFO));
		if(frmMultiLoad->m_chip_type == MT8127)
		{
	        ret = GetPartitionInfo(SCATTER_ANDROID_NAME, partInfo); // for kb
		}
		else
		{
		    ret = GetPartitionInfo(SCATTER_SECCFG_NAME, partInfo); 
		}
		
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FM_Android_Partial(): [ERROR] Getting readback PMT information fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get PMT Partition info fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }

		//pmtStartAddr = partInfo.begin_addr;
        //startAddrPartial = partInfo.begin_addr + partInfo.image_length;
        startAddrPartial = partInfo.begin_addr;
        fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_LOGICAL;
        fm_arg.m_format_cfg.m_format_begin_addr = startAddrPartial;

	    // support new scatter file, mtk71502,20130409
        FAT_STATUS fat_status;

        fat_status=IsFATChecked();  // check FAT scatter file status.
        FAT_CHANGED_STATUS fat_changed_status=IsFATChanged();
		unsigned int bmtBlocks = 80;
        //// support new scatter file, mtk71502,20130409 end
        if ((eChipGroup == BBCHIP_GROUP2) && frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC) {
            GC_TRACE("[COM%d] FM_Android_Partial(): BBCHIP_MT6575/6577/6589/6572/6582/6592 and HW_STORAGE_EMMC!", m_sComPortSetting.com.number);
            // Get FAT start address
            //PART_INFO fatPartInfo;
            // mtk71502.20120409, To support new scatter file.
            if(fat_status==FAT_UNDOWNLOAD || fat_status==FAT_NO_FOUND_IN_SCATTER ||  fat_status==FAT_CHECKED) // No exsit FAT ,format Flash except BMT
            {
                GC_TRACE("[COM%d] FM_Android_Partial(): FAT is not FAT_UNCHECKED(2)(fat_status = %d), set the end as the format length!", m_sComPortSetting.com.number,fat_status);
				
                if (0 < frmMultiLoad->m_BMT_blocks)
                {
                    bmtBlocks = frmMultiLoad->m_BMT_blocks;
                }

                fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
												+ m_sConnResult.m_da_report.m_emmc_boot2_size
												+ m_sConnResult.m_da_report.m_emmc_rpmb_size
												+ m_sConnResult.m_da_report.m_emmc_gp1_size
												+ m_sConnResult.m_da_report.m_emmc_gp2_size
												+ m_sConnResult.m_da_report.m_emmc_gp3_size
												+ m_sConnResult.m_da_report.m_emmc_gp4_size
												+ m_sConnResult.m_da_report.m_emmc_ua_size
												- startAddrPartial - bmtBlocks*128*1024;
            }
            else if(fat_status==FAT_UNCHECKED)// not format FAT
            {
                if(fat_changed_status==FAT_HAS_CHANGED){
                    GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                    m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                    if(true == m_is_DA_connected)
                    {
                        ret = FlashTool_Disconnect(&m_ft_api_handle);
                        m_is_DA_connected = false;
                    }
                    return -1;
                }else{
                    ret = GetPartitionInfo(SCATTER_FAT_NAME, fatPartInfo);
                    if(ret==S_DONE){
                        GC_TRACE("[COM%d] FM_Android_Partial(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                        fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
                    }
                }
            }

            unsigned int alignment = 512;
            unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
            if(0 != reminder )
            {
                fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
            }
        }
        else if ((eChipGroup == BBCHIP_GROUP2) && frmMultiLoad->m_Storage_Type == HW_STORAGE_SDMMC){
            GC_TRACE("[COM%d] FM_Android_Partial(): BBCHIP_MT6575/6577/6589/6572/6582/6592 and HW_STORAGE_SDMMC!", m_sComPortSetting.com.number);
            if(fat_status==FAT_UNDOWNLOAD|| fat_status==FAT_NO_FOUND_IN_SCATTER || fat_status ==FAT_CHECKED) // No exsit FAT ,format Flash except BMT
            {
                GC_TRACE("[COM%d] FM_Android_Partial(): FAT is not FAT_UNCHECKED(2)(fat_status = %d), set the end as the format length", m_sComPortSetting.com.number,fat_status);
                if (0 < frmMultiLoad->m_BMT_blocks)
                {
                    bmtBlocks = frmMultiLoad->m_BMT_blocks;
                }

                fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_sdmmc_ua_size
												- startAddrPartial - bmtBlocks*128*1024;
            }else if(fat_status==FAT_UNCHECKED){   // not format FAT
                if(fat_changed_status==FAT_HAS_CHANGED)
                {
                    GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                    m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                   if(true == m_is_DA_connected)
                   {
                     ret = FlashTool_Disconnect(&m_ft_api_handle);
                     m_is_DA_connected = false;
                   }
                   return -1;
                }else{
                    ret = GetPartitionInfo(SCATTER_FAT_NAME, fatPartInfo);
                    if(ret==S_DONE)
                    {
                        GC_TRACE("[COM%d] FM_Android_Partial(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                        fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
                    }
                }
            }

            unsigned int alignment = 512;
            unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
            if(0 != reminder )
            {
                fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
            }
        }
        else{
            if (0 < frmMultiLoad->m_BMT_blocks)
            {
                bmtBlocks = frmMultiLoad->m_BMT_blocks;
            }
            fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size - startAddrPartial -
				((bmtBlocks + PMT_BLOCK_COUNT) * m_sConnResult.m_da_report.m_nand_pagesize * m_sConnResult.m_da_report.m_nand_pages_per_block);

        }

        GC_TRACE("[COM%d] FM_Android_Partial(): BMT_Block_Count(0x%x) + PMT_Block_Count(0x%x), Page_Size(0x%x), Page_Num_per_Block(0x%x)", m_sComPortSetting.com.number,
        bmtBlocks, PMT_BLOCK_COUNT, m_sConnResult.m_da_report.m_nand_pagesize, m_sConnResult.m_da_report.m_nand_pages_per_block);

        GC_TRACE("[COM%d] FM_Android_Partial(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
                fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
    }
    else
    {
        GC_TRACE("[COM%d] For MT6516 platform, Begin addr of format is the start addr of Uboot.", m_sComPortSetting.com.number);
        fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_PHYSICAL;
        fm_arg.m_format_cfg.m_format_begin_addr = frmMultiLoad->GetUbootStartAddr();//m_sDnParam.s_nfb_param.ui_nand_fat_begin_addr;
        fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size - fm_arg.m_format_cfg.m_format_begin_addr; //m_sDnParam.s_nfb_param.ui_nand_fat_length;
        GC_TRACE("[COM%d] FM_Android_Partial(): StartAddr(0x%LLX), Len(0x%LLX)",m_sComPortSetting.com.number,
                 fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
    }

    GC_TRACE("[COM%d] FM_Android_Partial(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Partial(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    //check whether the bad block number is larger than BMT
    GC_TRACE("[COM%d] FM_Android_Partial(): Bad block number is %d!", m_sComPortSetting.com.number, fm_res.m_format_statistics.m_bad_blocks);
	if(fm_res.m_format_statistics.m_bad_blocks > (frmMultiLoad->m_BMT_blocks*2/3)){
		GC_TRACE("[COM%d] FM_Android_Partial(): [ERROR] Bad block number is larger than %d!", m_sComPortSetting.com.number, (frmMultiLoad->m_BMT_blocks*2/3));
		FlashTool_Disconnect(&m_ft_api_handle);
		m_is_DA_connected = false;
		return -1;
	}

    return ret;
}


int __fastcall TBOOT1::FM_Android_Partial_ExeceptOTP()
{
    prefix = "FR ";

    //=====================Format=======================================
    GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

	//Common part
    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type; //HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

	  ULL64 startAddrPartial;
	  //ULL64 pmtStartAddr;

	  PART_INFO fatPartInfo;
    E_CHIP_GROUP eChipGroup;
    eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);

    if(eChipGroup != BBCHIP_GROUP0) //MT6573 or MT6513 process flow.
    {
        //Get BinRegion Partition info
        PART_INFO partInfo;
        //ret = GetPartitionInfo(SCATTER_NVRAM_NAME, partInfo);
	 ret = GetPartitionInfo(SCATTER_SECCFG_NAME, partInfo); // need modify to support new scatter file.mtk71502.20130408
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): [ERROR] Getting readback PMT information fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get PMT Partition info fail!";
            FlashTool_Disconnect(&m_ft_api_handle);
            m_is_DA_connected = false;
            return ret;
        }

		    //pmtStartAddr = partInfo.begin_addr;
        //startAddrPartial = partInfo.begin_addr + partInfo.image_length;
        startAddrPartial = partInfo.begin_addr;
        fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_LOGICAL;
        fm_arg.m_format_cfg.m_format_begin_addr = startAddrPartial;

	// support new scatter file, mtk71502,20130409
         FAT_STATUS fat_status;

         fat_status=IsFATChecked();  // check FAT scatter file status.
         FAT_CHANGED_STATUS fat_changed_status=IsFATChanged();
          //// support new scatter file, mtk71502,20130409 end
        if ((eChipGroup == BBCHIP_GROUP2) && frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC) {
            GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): BBCHIP_MT6575/6577/6589 and HW_STORAGE_EMMC!", m_sComPortSetting.com.number);
            // Get FAT start address
            //PART_INFO fatPartInfo;
            // mtk71502.20120409, To support new scatter file.


	    /*if(ret==S_DONE)
	    {
                 if (strcmpi(fatRomInfo[i].name, "FAT") == 0)
		{
			if(_TRUE == fatRomInfo[i].item_is_visable)
			{
				fat_status = FAT_CHECKED; // FAT is checked.

			}
			if(_FALSE == fatRomInfo[i].item_is_visable)
			{
				fat_status = FAT_UNCHECKED; // FAT is unchecked.

			}
			break;
		}

		if (strcmpi(fatRomInfo[i].name, "__NODL_FAT") == 0)
		{
			fat_status = FAT_UNDOWNLOAD; // FAT is none download. Now, we can consider this case as FAT is checked.
			break;
		}


	    }*/
	    if(fat_status==FAT_UNDOWNLOAD || fat_status==FAT_NO_FOUND_IN_SCATTER) // No exsit FAT ,format Flash except BMT

           // ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo); // need modify to support new scatter file.mtk71502,20130409
            //if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info fail, set the end as the format length", m_sComPortSetting.com.number);
        				/*m_asMsg = "ERROR: Get FAT Partition info fail!";
        				FlashTool_Disconnect(&m_ft_api_handle);
        				m_is_DA_connected = false;
        				return ret;*/

                unsigned int bmtBlocks = 80;
				ULL64 otpBlocks;
				PART_INFO partOTPInfo;
				ret = GetPartitionInfo(SCATTER_OTP_NAME, partOTPInfo);
				if(S_DONE != ret)
				{  
				   GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): [ERROR]Getting readback PMT information fail", m_sComPortSetting.com.number);
				   m_asMsg = "ERROR: Get OTP Partition info fail!";
				   FlashTool_Disconnect(&m_ft_api_handle);
				   m_is_DA_connected = false;
				   return ret;
				}
				otpBlocks = partOTPInfo.begin_addr & 0x0000FFFF;
				
                if (0 < frmMultiLoad->m_BMT_blocks)
                {
                    bmtBlocks = frmMultiLoad->m_BMT_blocks;
                }

                fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
												+ m_sConnResult.m_da_report.m_emmc_boot2_size
												+ m_sConnResult.m_da_report.m_emmc_rpmb_size
												+ m_sConnResult.m_da_report.m_emmc_gp1_size
												+ m_sConnResult.m_da_report.m_emmc_gp2_size
												+ m_sConnResult.m_da_report.m_emmc_gp3_size
												+ m_sConnResult.m_da_report.m_emmc_gp4_size
												+ m_sConnResult.m_da_report.m_emmc_ua_size
												- startAddrPartial - otpBlocks*128*1024;
            }
	   else if(fat_status==FAT_CHECKED)// not format FAT
	   {
	       ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo);
		if(ret==S_DONE)
		{
			GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                     fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
		}
	   }
	   else // FAT_UNCHECKED
	   {
	   	if(fat_changed_status==FAT_HAS_CHANGED)
              {
                 GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                 m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                 if(true == m_is_DA_connected)
                 {
                     ret = FlashTool_Disconnect(&m_ft_api_handle);
                     m_is_DA_connected = false;
                 }
                 return -1;
             }
		else
		{
		   ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo);
		   if(ret==S_DONE)
		   {
			GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                     fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
		   }

		 }
	   }

            //else
            //{
               // GC_TRACE("[COM%d] FM_Android_Partial(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
               // fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
            //}

			/*fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
												+ m_sConnResult.m_da_report.m_emmc_boot2_size
												+ m_sConnResult.m_da_report.m_emmc_rpmb_size
												+ m_sConnResult.m_da_report.m_emmc_gp1_size
												+ m_sConnResult.m_da_report.m_emmc_gp2_size
												+ m_sConnResult.m_da_report.m_emmc_gp3_size
												+ m_sConnResult.m_da_report.m_emmc_gp4_size
												+ m_sConnResult.m_da_report.m_emmc_ua_size
												- startAddrPartial;*/

            unsigned int alignment = 512;
            unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
            if(0 != reminder )
            {
                fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
            }

        }
        else if ((eChipGroup == BBCHIP_GROUP2) && frmMultiLoad->m_Storage_Type == HW_STORAGE_SDMMC){
        GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): BBCHIP_MT6575/6577/6589 and HW_STORAGE_SDMMC!", m_sComPortSetting.com.number);
        // Get FAT start address
        //PART_INFO fatPartInfo;
       // ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo);
        if(fat_status==FAT_UNDOWNLOAD|| fat_status==FAT_NO_FOUND_IN_SCATTER) // No exsit FAT ,format Flash except BMT

       // if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info fail, set the end as the format length", m_sComPortSetting.com.number);

            unsigned int bmtBlocks = 80;
            if (0 < frmMultiLoad->m_BMT_blocks)
            {
                bmtBlocks = frmMultiLoad->m_BMT_blocks;
            }

            fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_sdmmc_ua_size
												- startAddrPartial - bmtBlocks*128*1024;
        }
        else if(fat_status==FAT_CHECKED)// not format FAT
        {
            ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo);
	    if(ret==S_DONE)
           {
               GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
	    }
        }
	   else // FAT_UNCHECKED
	   {
	   	if(fat_changed_status==FAT_HAS_CHANGED)
              {
                 GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                 m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                 if(true == m_is_DA_connected)
                 {
                     ret = FlashTool_Disconnect(&m_ft_api_handle);
                     m_is_DA_connected = false;
                 }
                 return -1;
             }
		else
		{
		    ret = GetPartitionInfo(SCATTER_DL_FAT_NAME, fatPartInfo);
		   if(ret==S_DONE)
		   {
			GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Getting FAT Partition info success!", m_sComPortSetting.com.number);
                     fm_arg.m_format_cfg.m_format_length = fatPartInfo.begin_addr - startAddrPartial;
		   }

		}
	   }

        unsigned int alignment = 512;
        unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
        if(0 != reminder )
        {
            fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
        }
    }

     GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
            fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
    }

    GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Partial_ExeceptOTP(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    return ret;
}

// Format FAT region
int __fastcall TBOOT1::FM_Android_FAT()
{
    prefix = "FR ";

    //=====================Format=======================================
    GC_TRACE("[COM%d] FM_Android_FAT(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

	//Common part
    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type; //HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

	PART_INFO fatPartInfo;
	ret = GetPartitionInfo(SCATTER_FAT_NAME, fatPartInfo);  // need modify to support new scatter file, 20130408
	if (S_DONE != ret)
	{
		GC_TRACE("[COM%d] FM_Android_FAT(): Getting FAT Partition info fail, do not format FAT.", m_sComPortSetting.com.number);
		return 0;

	}

    E_CHIP_GROUP eChipGroup;
    eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);
	//Format from FAT start address to BMT pool start address.
	if(BBCHIP_GROUP0 != eChipGroup)
	{
		if ((BBCHIP_GROUP2 == eChipGroup)&& frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC)
		{
			fm_arg.m_format_cfg.m_format_begin_addr = fatPartInfo.begin_addr;
			unsigned int bmtBlocks = 80;
			if (0 < frmMultiLoad->m_BMT_blocks)
			{
				bmtBlocks = frmMultiLoad->m_BMT_blocks;
			}

			fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
												+ m_sConnResult.m_da_report.m_emmc_boot2_size
												+ m_sConnResult.m_da_report.m_emmc_rpmb_size
												+ m_sConnResult.m_da_report.m_emmc_gp1_size
												+ m_sConnResult.m_da_report.m_emmc_gp2_size
												+ m_sConnResult.m_da_report.m_emmc_gp3_size
												+ m_sConnResult.m_da_report.m_emmc_gp4_size
												+ m_sConnResult.m_da_report.m_emmc_ua_size
												- fatPartInfo.begin_addr - bmtBlocks*128*1024;

			GC_TRACE("[COM%d] FM_Android_FAT(): Do format FAT. ", m_sComPortSetting.com.number);
			GC_TRACE("[COM%d] FM_Android_FAT(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
				fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
			ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] FM_Android_FAT(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
				FlashTool_Disconnect(&m_ft_api_handle);
				m_is_DA_connected = false;
				return ret;
			}

		}
		else if ((BBCHIP_GROUP2 == eChipGroup) && frmMultiLoad->m_Storage_Type == HW_STORAGE_SDMMC)
		{
			fm_arg.m_format_cfg.m_format_begin_addr = fatPartInfo.begin_addr;

			unsigned int bmtBlocks = 80;
			if (0 < frmMultiLoad->m_BMT_blocks)
			{
				bmtBlocks = frmMultiLoad->m_BMT_blocks;
			}

			fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_sdmmc_ua_size
												- fatPartInfo.begin_addr - bmtBlocks*128*1024;

			GC_TRACE("[COM%d] FM_Android_FAT(): Do format FAT. ", m_sComPortSetting.com.number);
			GC_TRACE("[COM%d] FM_Android_FAT(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
				fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
			ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] FM_Android_FAT(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
				FlashTool_Disconnect(&m_ft_api_handle);
				m_is_DA_connected = false;
				return ret;
			}
		}

	}

    return ret;
}

int __fastcall TBOOT1::FM_Android_Bootloader()
{
    prefix = "FR ";
    
    int iEmmcType = E_LINEAR_EMMC;
    //Get partition info of bootloader
    /* PART_INFO partInfo;
    GC_TRACE("[COM%d] FM_Android_Bootloader():Enter...", m_sComPortSetting.com.number);
    /*ret = GetPartitionInfo(SCATTER_PRELOADER_NAME, partInfo);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Bootloader(): [ERROR] Getting bootloader information fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Get Bootloader Partition info fail!";
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }*/

    //Get memory Type
	DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
    CheckEmmcType(*p_dl_handle,iEmmcType);

    //Format
    GC_TRACE("[COM%d] FM_Android_Bootloader(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type;//HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_LOGICAL;

    /* fm_arg.m_format_cfg.m_format_begin_addr = partInfo.begin_addr;
         fm_arg.m_format_cfg.m_format_length = partInfo.image_length;*/
    fm_arg.m_format_cfg.m_format_begin_addr = 0;
    fm_arg.m_format_cfg.m_format_length = 0x40000;
    GC_TRACE("[COM%d] FM_Android_All(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
        fm_arg.m_format_cfg.m_format_begin_addr , fm_arg.m_format_cfg.m_format_length);

    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

	if(fm_arg.m_storage_type  == HW_STORAGE_EMMC && iEmmcType == E_COMBO_EMMC)
	{
	    fm_arg.m_format_cfg.m_part_id = EMMC_PART_BOOT1;
	}

    GC_TRACE("[COM%d] FM_Android_Bootloader(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Bootloader(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    return ret;
}

/*
int __fastcall TBOOT1::FM_Android_Partial()
{
    prefix = "FR ";

    //=====================Connect and Change baudrate====================
    if(false == m_is_DA_connected)
    {
    	//Connect with DA and change baudrate
         GC_TRACE("[COM%d] FM_Android_Partial(): DA connect...", m_sComPortSetting.com.number);
    	ret  = DAConnect();
    	if(S_DONE != ret)
    	{
    	    GC_TRACE("[COM%d] FM_Android_Partial(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
    	    return ret;
    	}

    	m_is_DA_connected = true;
    }


    //Get BinRegion Partition info
    PART_INFO partInfo;
    ret = GetPartitionInfo(SCATTER_NVRAM_NAME, partInfo);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_Partial(): [ERROR] Getting readback information fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Get BinRegion Partition info fail!";
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    unsigned int startAddrPartial = partInfo.begin_addr + partInfo.image_length;

    //=====================Format=======================================
    GC_TRACE("[COM%d] FM_Android_Partial(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

    fm_arg.m_storage_type = HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_LOGICAL;

    fm_arg.m_format_cfg.m_format_begin_addr = startAddrPartial;
    fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size - startAddrPartial -
        (m_sConnArg.m_boot_arg.m_bmt_block_count * m_sConnResult.m_da_report.m_nand_pagesize * m_sConnResult.m_da_report.m_nand_pages_per_block);
    GC_TRACE("[COM%d] FM_Android_All(): BMT_Block_Count(0x%x), Page_Size(0x%x), Page_Num_per_Block(0x%x)", m_sComPortSetting.com.number,
        m_sConnArg.m_boot_arg.m_bmt_block_count, m_sConnResult.m_da_report.m_nand_pagesize, m_sConnResult.m_da_report.m_nand_pages_per_block);
    GC_TRACE("[COM%d] FM_Android_All(): StartAddr(0x%x), Len(0x%x)", m_sComPortSetting.com.number,
        fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);

    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

    GC_TRACE("[COM%d] FM_Android_All(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FM_Android_All(): [ERROR] Flash format fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        return ret;
    }

    return ret;
}
*/


//==============================================================================
int __fastcall TBOOT1::DL_Android_FirstSite()
{
    int ret = S_DONE;
    int index = GetCOMPortIndex();

    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);
    * p_stopflag = FALSE;


    // Fill flash download argument
    DownloadArgSetting();
    prefix = "DL ";


    GC_TRACE("[COM%d] DL_Android_FirstSite(): Check preloader enabled...", m_sComPortSetting.com.number);
    if(!frmMultiLoad->IsDLBootloader())
    {
        //Application->MessageBox("Please select preloader for download", "FAIL", MB_OK);
        GC_TRACE("[COM%d] DL_Android_FirstSite(): Preloader is NOT selected to be downloaded!", m_sComPortSetting.com.number);
        return ret;
    }


    if(false == m_is_DA_connected)
    {
        //Connect with DA and change baudrate
        GC_TRACE("[COM%d] DL_Android_FirstSite(): DA connect...", m_sComPortSetting.com.number);
        ret  = DAConnect();
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
            return ret;
        }

        m_is_DA_connected = true;
    }



    //Fill download parameter
    GC_TRACE("[COM%d] DL_Android_FirstSite(): Fill download image info...", m_sComPortSetting.com.number);
    if(-1 == frmMultiLoad->FillDownloadImageInfo() )
    {
        GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] Fill download image info fail!");
        return -1;
    }

    //Disable all rom files exclude bootloader
    GC_TRACE("[COM%d] DL_Android_FirstSite(): Disable all rom files exclude preloader...", m_sComPortSetting.com.number);
    if(-1 == frmMultiLoad->DisableDLAttribute() )
    {
        GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] Disable DL attribute fail!");
        return -1;
    }

    //Download Preloader
    GC_TRACE("[COM%d] DL_Android_FirstSite(): Download preloader......", m_sComPortSetting.com.number);
    ret  = DAFullSpeedDownload();
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] DAFullSpeedDownload() fail!", m_sComPortSetting.com.number);
        FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
        frmMultiLoad->RestoreDLAttribute();
        return ret;
    }

    //Restore all rom files
    GC_TRACE("[COM%d] DL_Android_FirstSite(): Restore all rom files...", m_sComPortSetting.com.number);
    if (-1 == frmMultiLoad->RestoreDLAttribute())
    {
        GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] Restore DL attribute fail!");
        return -1;
    }

/*
    GC_TRACE("[COM%d] Enable watchdog timeout...", m_sComPortSetting.com.number);
    ret  = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, 5000);
    if(S_DONE != ret)
    {
    	GC_TRACE("[COM%d] FlashTool_EnableWatchDogTimeout() Fail", index);
    	return ret;
    }
*/

    GC_TRACE("[COM%d] DL_Android_FirstSite(): DA disconnect...", m_sComPortSetting.com.number);
    ret = FlashTool_Disconnect(&m_ft_api_handle);
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] DL_Android_FirstSite(): [ERROR] DA disconnect fail!", m_sComPortSetting.com.number);
        return ret;
    }

    m_is_DA_connected = false;

//    prefix = "ANDROID ";
    return ret;

}


int __fastcall TBOOT1::DL_Android_SecondSite()
{
    int ret = S_DONE;
    HANDLE hCOM = NULL;
    char sz_port_number[10];

    int portNumber = GetCOMPortNumber();
    int i = GetCOMPortIndex();
    int * p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);
    prefix = "DL ";

    sprintf(sz_port_number, "\\\\.\\COM%d", portNumber);



    //Fill download parameter
    GC_TRACE("[COM%d] DL_Android_SecondSite(): Fill download image info...", m_sComPortSetting.com.number);
    if(-1 == frmMultiLoad->FillDownloadImageInfo() )
    {
        GC_TRACE("[COM%d] Fill download image info fail!");
        return -1;
    }

    GC_TRACE("[COM%d] DL_Android_SecondSite(): Check other images enabled...", m_sComPortSetting.com.number);
    if( !frmMultiLoad->IsDLSystem() )
    {
        //Application->MessageBox("Please select images for download", "FAIL", MB_OK);
        GC_TRACE("[COM%d] DL_Android_SecondSite(): No other images is selected to be downloaded!", m_sComPortSetting.com.number);
        return ret;
    }


/*
    if(!GetSymbolicName(sz_port_number, sz_symbol_name))
    {
        GC_TRACE("[COM%d] Cannot retrieve symbolic name from registry!");
        return -1;
    }

    sz_symbol_name[1] = '\\';
    GC_TRACE("[COM%d] Port: %s, Symbolic name: %s", m_sComPortSetting.com.number, sz_port_number, sz_symbol_name);
*/


    GC_TRACE("[COM%d] DL_Android_SecondSite(): Try to open USB COM port...(Polling)", m_sComPortSetting.com.number);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] DL_Android_SecondSite(): User press stop button!", m_sComPortSetting.com.number);
            return -1;
        }

        hCOM = CreateFile( sz_port_number, GENERIC_READ |GENERIC_WRITE, 0, NULL,
                                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if(INVALID_HANDLE_VALUE == hCOM)
        {
            GC_TRACE("[COM%d] DL_Android_SecondSite(): Cannot open USB port!", m_sComPortSetting.com.number);
        }
        else
        {
            GC_TRACE("[COM%d] DL_Android_SecondSite(): ++++++Open USB succeed!++++++", m_sComPortSetting.com.number);
            CloseHandle(hCOM);
            break;
        }

        Sleep(500);

    }while(1);



    //======================================================
    //======================================================
    EBOOT_RESULT ret_e;
    char log[128];

    Android_Boot_ARG             boot_arg;
    Android_Download_ARG         dl_arg;

    // Boot As Download Mode
    boot_arg.m_baud_rate  = MainForm->Get_Baudrate();
    boot_arg.m_p_stopflag = p_stopflag;
    boot_arg.m_p_thread   = this;

    //Region information
    if( !GetRegionInfo(dl_arg) )
    {
            GC_TRACE("[COM%d] DL_Android_SecondSite(): [ERROR] Get region information fail!", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            return false;
    }

    // Download Image
    memcpy(&dl_arg.m_image_files,
                    &frmMultiLoad->m_image_files,
                    ANDROID_IMAGE_FILE_NUMBER*sizeof(Android_Image_File));


    if( IsNeedBoundaryCheck() )
    {
        dl_arg.is_boundary_Check = true;
    }
    else
    {
        dl_arg.is_boundary_Check = false;
    }

    dl_arg.m_p_stopflag                 = p_stopflag;
    dl_arg.m_p_thread                   = this;
    dl_arg.m_p_send_flash_percentage    = Set_send_flash_bin_percentage;
    dl_arg.m_p_send_image_file_status   = set_send_image_file_info;

    //boot_arg.m_p_comsymbol= sz_symbol_name;
    boot_arg.m_p_comsymbol = sz_port_number;



    GC_TRACE("[COM%d] DL_Android_SecondSite(): Calling eboot.dll for other images download...", m_sComPortSetting.com.number);
    ret_e = Android_Flash_Download(boot_arg, &dl_arg);
    if(EBOOT_SUCCESS != ret_e)
    {
        ret = ret_e;
        GC_TRACE("[COM%d] DL_Android_SecondSite(): Download other images fail!", m_sComPortSetting.com.number);
        return -1;
    }

    prefix = "ANDROID ";
    return ret;
}


int __fastcall TBOOT1::DownloadSecro(bool FormatEnable)
{
    int ret = S_DONE;
	
try{

	//int __fastcall TBOOT1::FM_Android_All()
 /*   if(FormatEnable)
    {
     prefix = "FR ";

    //=====================Format=======================================
    GC_TRACE("[COM%d] DownloadSecro(): Set format parameters...", m_sComPortSetting.com.number);
    FlashTool_Format_Arg       fm_arg;
    FlashTool_Format_Result    fm_res;

    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type;//HW_STORAGE_NAND;
    GC_TRACE("[fm_arg.m_storage_type is %d .]", fm_arg.m_storage_type);
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_PHYSICAL;

    fm_arg.m_format_cfg.m_format_begin_addr = 0;
    if (HW_STORAGE_NAND == fm_arg.m_storage_type)
    {
        fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_nand_flash_size;
    }
    else if (HW_STORAGE_EMMC == fm_arg.m_storage_type)
    {
		fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_emmc_boot1_size
											+ m_sConnResult.m_da_report.m_emmc_boot2_size
											+ m_sConnResult.m_da_report.m_emmc_rpmb_size
											+ m_sConnResult.m_da_report.m_emmc_gp1_size
											+ m_sConnResult.m_da_report.m_emmc_gp2_size
											+ m_sConnResult.m_da_report.m_emmc_gp3_size
											+ m_sConnResult.m_da_report.m_emmc_gp4_size
											+ m_sConnResult.m_da_report.m_emmc_ua_size;
		unsigned int alignment = 512;
		unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
		if(0 != reminder )
		{
			GC_TRACE("[COM%d] Before alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
			fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
			GC_TRACE("[COM%d] After alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);

		}
		GC_TRACE("[COM%d] DownloadSecro(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
            fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);

    }
	else if(HW_STORAGE_SDMMC == fm_arg.m_storage_type)
	{
		fm_arg.m_format_cfg.m_format_length = m_sConnResult.m_da_report.m_sdmmc_ua_size;

		unsigned int alignment = 512;
		unsigned int reminder = fm_arg.m_format_cfg.m_format_length%alignment;
		if(0 != reminder )
		{
			GC_TRACE("[COM%d] Before alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);
			fm_arg.m_format_cfg.m_format_length = fm_arg.m_format_cfg.m_format_length - reminder;
			GC_TRACE("[COM%d] After alignment: fm_arg.m_format_cfg.m_format_length is %LLX", m_sComPortSetting.com.number, fm_arg.m_format_cfg.m_format_length);

		}
		GC_TRACE("[COM%d] DownloadSecro(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
            fm_arg.m_format_cfg.m_format_begin_addr, fm_arg.m_format_cfg.m_format_length);
	}

    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

    GC_TRACE("[COM%d] DownloadSecro(): Do format...", m_sComPortSetting.com.number);
    ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
    ret = FM_Android_All(false);
    if(S_DONE == ret)
    {
        GC_TRACE("[COM%d] DownloadSecro(): [DONE] Flash format OK!", m_sComPortSetting.com.number);
		//=====================Download===========================
	    GC_TRACE("[COM%d] FormatAndDownload(): Image download...", m_sComPortSetting.com.number);
	    ret = DL_Android_All();
	    if(S_DONE != ret)
	    {
	        GC_TRACE("[COM%d] DownloadSecro(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
	        return ret;
	    }
		if(MainForm->m_EnableWdtDogReset == 1){
           WatchDogResetForAutoTest();
        }

	    if(true == m_is_DA_connected)
	    {
		    	//FlashTool_EnableDAFinishCMD(m_ft_api_handle, DISABLE_DA_FINISH_CMD);
	        ret = FlashTool_Disconnect(&m_ft_api_handle);
	        m_is_DA_connected = false;
			m_SWSec_FormatDL_Done = true;
	    }
        return ret;
    }
	else if(ret == S_SECURITY_SF_CODE_FORMAT_FORBIDDEN)
		m_SWSec_FormatDL_Done = false;
	else //error
		return ret;

    }
	*/
	m_SWSec_FormatDL_Done = false;
	GC_TRACE("[COM%d] DownloadSecro():FormatEnable is %d!", m_sComPortSetting.com.number, FormatEnable?1:0);
	
	if(FormatEnable)
	{
	     GC_TRACE("[COM%d] DownloadSecro(): Doing format for detect secro status...!", m_sComPortSetting.com.number);
	     ret = FM_Android_All(false);
		 switch(ret)
		 {
		     case S_DONE:
		 	      m_SWSec_FormatDL_Done = true;
				  GC_TRACE("[COM%d] DownloadSecro(): format success, don't go Secro rework flow, but go normal download flow!", m_sComPortSetting.com.number);
				  goto clean;
				  
             case S_SECURITY_SF_CODE_FORMAT_FORBIDDEN:
			 	  GC_TRACE("[COM%d] DownloadSecro(): format error is S_SECURITY_SF_CODE_FORMAT_FORBIDDEN, so continue rework secro flow!", m_sComPortSetting.com.number);
			 	  m_SWSec_FormatDL_Done = false;
				  break;
				  
		     default:
			 	  GC_TRACE("[COM%d] DownloadSecro(): format error is %X! the rework secro flow will be stop.", m_sComPortSetting.com.number, ret);
			 	  m_SWSec_FormatDL_Done = false;
				  goto clean;
		 }
	}

     GC_TRACE("[COM%d] DownloadSecro(): download secro for secro rework flow...", m_sComPortSetting.com.number);
	//void TBOOT1::DownloadArgSetting(void)
	{
		  //FLASHTOOL_DOWNLOAD_ARG settings
		  m_sFtDlArg.m_dl_handle_list					   = NULL;
		  m_sFtDlArg.m_dl_handle						   = *g_DL_HANDLE_SecroReworking;
		  m_sFtDlArg.m_download_accuracy				   = ACCURACY_AUTO;
		  m_sFtDlArg.m_cb_download_flash_init			   = ::cb_download_flash_init;
		  m_sFtDlArg.m_cb_download_flash_init_arg		   = this;
		  m_sFtDlArg.m_cb_download_flash				   = cb_download_flash;
		  m_sFtDlArg.m_cb_download_flash_arg			   = this;
		  m_sFtDlArg.m_cb_security_post_process_notify	   = ::cb_security_post_process_notify;
		  m_sFtDlArg.m_cb_security_post_process_notify_arg = this;
		  m_sFtDlArg.m_enable_tgt_res_layout_check		   = m_sDnParam.s_multibin_op.b_check_target_resource_layout?_TRUE:_FALSE;
		  m_sFtDlArg.m_enable_bbchip_ver_check			   = m_sDnParam.s_bbchip_op.b_check_eco_ver ?_TRUE:_FALSE;
		  m_sFtDlArg.m_cb_format_statistics 			   = ::cb_format_statistics;
		  m_sFtDlArg.m_cb_format_statistics_arg 		   = this;
	
		  m_sFtDlArg.m_format_enable = _FALSE;
		  m_sFtDlArg.m_format_cfg.m_auto_format_fat = _FALSE;
		  m_sFtDlArg.m_enable_wdt_timeout = _FALSE;
		  m_sFtDlArg.m_ms_wdt_timeout_interval = 0;
	
		  //checksum
		  m_sFtDlArg.m_cb_checksum_init = cb_da_dl_checksum_init;
		  m_sFtDlArg.m_cb_checksum_init_arg = this;
		  m_sFtDlArg.m_cb_checksum = cb_da_dl_checksum;
		  m_sFtDlArg.m_cb_checksum_arg = this;
	
	
		  // parameter
		  m_sFtDlArg.m_otp_enable = _FALSE;
		  m_sFtDlArg.m_otp_op = OTP_WRITE;	/*modified by Shaoying 20120614 m_sDnParam.s_param.m_otp_op*/;
		  m_sFtDlArg.m_otp.m_addr = m_sDnParam.s_param.m_otp.m_addr;
		  m_sFtDlArg.m_otp.m_data = m_sDnParam.s_param.m_otp.m_data;
		  m_sFtDlArg.m_otp.m_len = m_sDnParam.s_param.m_otp.m_len;
		  m_sFtDlArg.m_ft_cfg_enable = _FALSE;
		  m_sFtDlArg.m_ft_cfg.m_security_cfg.m_uint32 = m_sDnParam.s_param.m_ft_cfg.m_security_cfg.m_uint32;
		  m_sFtDlArg.m_sec_ro_enable = m_sDnParam.s_param.m_sec_ro_enable;
		  m_sFtDlArg.m_sec_ro.m_data = m_sDnParam.s_param.m_sec_ro.m_data;
		  m_sFtDlArg.m_sec_ro.m_len = m_sDnParam.s_param.m_sec_ro.m_len;
		  m_sFtDlArg.m_cust_para_enable = _FALSE;
		  m_sFtDlArg.m_cust_para.m_data = m_sDnParam.s_param.m_cust_para.m_data;
		  m_sFtDlArg.m_cust_para.m_len = m_sDnParam.s_param.m_cust_para.m_len;
	
		  m_sFtDlArg.m_cb_param_process_init	 = ::cb_update_param_init;
		  m_sFtDlArg.m_cb_param_process_init_arg = this;
		  m_sFtDlArg.m_cb_param_process 	   = ::cb_update_param;
		  m_sFtDlArg.m_cb_param_process_arg 	 = this;
	
		  m_sFtDlArg.m_cb_post_process_init 	 = ::cb_post_process_init;
		  m_sFtDlArg.m_cb_post_process_init_arg  = this;
		  m_sFtDlArg.m_cb_post_process		   = ::cb_post_process;
		  m_sFtDlArg.m_cb_post_process_arg		 = this;
	
		  m_sFtDlArg.m_cb_format_report_init	   = ::cb_format_report_init;
		  m_sFtDlArg.m_cb_format_report_init_arg   = this;
		  m_sFtDlArg.m_cb_format_report 		   = ::cb_format_report;
		  m_sFtDlArg.m_cb_format_report_arg 	   = this;
		  m_sFtDlArg.m_enable_tgt_res_layout_check = m_sDnParam.s_multibin_op.b_check_target_resource_layout?_TRUE:_FALSE;
		  m_sFtDlArg.m_enable_bbchip_ver_check	   = m_sDnParam.s_bbchip_op.b_check_eco_ver ?_TRUE:_FALSE;
		  m_sFtDlArg.m_cb_format_statistics 	   = ::cb_format_statistics;
		  m_sFtDlArg.m_cb_format_statistics_arg    = this;
	
		  m_sFmtArg.m_storage_type = frmMultiLoad->m_Storage_Type;
		  if( OPERATION_NFB == m_sDnParam.e_Operation )
		  {
			  // for NFB bloader
			  m_sFtDlArg.m_format_cfg.m_validation		= m_sDnParam.s_nfb_param.b_validation;
			  m_sFtDlArg.m_cb_download_bloader_init 	= ::cb_download_bloader_init;
			  m_sFtDlArg.m_cb_download_bloader_init_arg = this;
			  m_sFtDlArg.m_cb_download_bloader			= ::cb_download_bloader;
			  m_sFtDlArg.m_cb_download_bloader_arg		= this;
			  //m_sFtDlArg.m_storage_type = HW_STORAGE_NAND;
			  //m_sFmtArg.m_storage_type = HW_STORAGE_NAND;
		  }
		  else
		  {
			  m_sFtDlArg.m_format_cfg.m_validation		= m_sDnParam.s_nor_param.b_validation;
			  m_sFtDlArg.m_cb_download_bloader_init 	= NULL;
			  m_sFtDlArg.m_cb_download_bloader_init_arg = NULL;
			  m_sFtDlArg.m_cb_download_bloader			= NULL;
			  m_sFtDlArg.m_cb_download_bloader_arg		= NULL;
			  //m_sFtDlArg.m_storage_type = HW_STORAGE_NOR;
			  //m_sFmtArg.m_storage_type = HW_STORAGE_NOR;
		  }
	
		  // GPS
		  m_sFtDlArg.m_relay_da_enable			 = m_sDnParam.s_param.m_gps_enable;
		  m_sFtDlArg.m_gpio_gps.m_power_pin 	 = 65535 /*m_sDnParam.s_param.m_gps_power_pin*/;
		  m_sFtDlArg.m_gpio_gps.m_reset_pin 	 = 65535/*m_sDnParam.s_param.m_gps_reset_pin*/;
		  m_sFtDlArg.m_gpio_gps.m_standby_pin	 = 65535/*m_sDnParam.s_param.m_gps_standby_pin*/;
		  m_sFtDlArg.m_gpio_gps.m_32k_pin		 = 65535/*m_sDnParam.s_param.m_gps_32k_pin*/;
	
		  // Maobing Add for Smart phone download to enable watch dog
		  m_sFtDlArg.m_enable_wdt_timeout	   = _TRUE;
		  m_sFtDlArg.m_ms_wdt_timeout_interval = 1000;
	}

	prefix = "DL ";

    GC_TRACE("[COM%d] DownloadSecro(): Check if there are some images needed to be downloaded", m_sComPortSetting.com.number);
    if(!frmMultiLoad->IsDLAnyImage())
    {
        //Application->MessageBox("Please select images for download", "FAIL", MB_OK);
        GC_TRACE("[COM%d] DownloadSecro(): No images is selected to be download!", m_sComPortSetting.com.number);
        return -1;
    }

	if(true == MainForm->m_miEnableDADL->Checked)
    {
			if(true == MainForm->m_miDADLFullSpeed->Checked)
			{
				  GC_TRACE("[COM%d] DownloadSecro(): DA full speed download all images...", m_sComPortSetting.com.number);
				  ret = DAFullSpeedDownloadWithoutReset();
				  if(S_DONE != ret)
				  {
					  GC_TRACE("[COM%d] DownloadSecro(): [ERROR] DA full speed download fail!", m_sComPortSetting.com.number);
				  	FlashTool_Disconnect(&m_ft_api_handle);
					  m_is_DA_connected = false;
				  	goto clean;
				  }


			}
			else if(true == MainForm->m_miDADLHighSpeed->Checked)
			{
				GC_TRACE("[COM%d] DownloadSecro(): DA high speed download all images...", m_sComPortSetting.com.number);
				ret = DAHighSpeedDownloadWithoutReset();
				if(S_DONE != ret)
				{
					GC_TRACE("[COM%d] DownloadSecro(): [ERROR] DA high speed download fail!", m_sComPortSetting.com.number);
					FlashTool_Disconnect(&m_ft_api_handle);
					m_is_DA_connected = false;
					goto clean;
				}
			}
		}

}
catch(...)
		{
			return 1;
		}
		  
clean:
	
    return ret;
}


int __fastcall TBOOT1::FormatAndDownload(bool bTodoDisconnectDA)
{
    int ret = S_DONE;
	EfuseSettings efuseSettings;
    int index = GetCOMPortIndex();
    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(index);
    * p_stopflag = FALSE;

    GC_TRACE("[COM%d] FormatAndDownload(): bTodoDisconnectDA = %d, if bTodoDisconnectDA is 1, it will do Disconnect when fail", m_sComPortSetting.com.number, bTodoDisconnectDA?1:0);
    //Connect with DA
    prefix = "DA ";
    if(false == m_is_DA_connected)
    {
        GC_TRACE("[COM%d] FormatAndDownload(): Connect with DA...", m_sComPortSetting.com.number);
        ret  = DAConnect();

        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FormatAndDownload(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: [Brom] DA connect fail!";
            return ret;
        }
        m_is_DA_connected = true;

		if (true == MainForm->m_miEnableDADL->Checked && true == MainForm->m_miDADLHighSpeed->Checked)
		{
			  ret = SwitchComport();
		}

		if (S_DONE != ret)
		{
			if ( -1 == ret)
			{
				GC_TRACE("[COM%d] FormatAndDownload(): SwitchComport user stop!", m_sComPortSetting.com.number);
			}
			else
			{
				GC_TRACE("[COM%d] FormatAndDownload(): [ERROR] SwitchComport fail!", m_sComPortSetting.com.number);
				m_asMsg = "ERROR: SwitchComport fail!";
			}

            return ret;
		}

    }
	
	if(MainForm->SecroReworkingEnable && MainForm->SecroReworkRunningFlag){
	    ret = DownloadSecro(true);
	    if(S_DONE != ret)
		{
		    GC_TRACE("[COM%d] FormatAndDownload():[SWSec] Download SECRO fail!", m_sComPortSetting.com.number);
		    goto FM_And_DL_Finish;
		}
	}else{
	     GC_TRACE("[COM%d] FormatAndDownload():[SWSec] SecroReworkingEnable =%d, SecroReworkRunningFlag = %d", 
		 	m_sComPortSetting.com.number,MainForm->SecroReworkingEnable?1:0, MainForm->SecroReworkRunningFlag?1:0);
	}
	
    //=====================All format=============================
    if(m_SWSec_FormatDL_Done == false){ //normal flow the m_SWSec_FormatDL_Done will be flase, but if SecroReworkingEnable is true, m_SWSec_FormatDL_Done maybe equal to true
        GC_TRACE("[COM%d] FormatAndDownload(): Flash format all...", m_sComPortSetting.com.number);
        ret = FM_Android_All();
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FormatAndDownload(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
		    goto FM_And_DL_Finish;
        //return ret;
        }
	}
   
    //=====================Download===========================
    GC_TRACE("[COM%d] FormatAndDownload(): Image download...", m_sComPortSetting.com.number);
    ret = DL_Android_All();
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FormatAndDownload(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
        goto FM_And_DL_Finish;
        //return ret;
    }
    GC_TRACE("[COM%d] FormatAndDownload(): DL_Android_All success!", m_sComPortSetting.com.number);

	
	//========================================================
	efuseSettings.ReadEfuseSettings();
	if ( 0 != efuseSettings.GetEfuseEnable())
	{
	    ret = WriteEfuse();
	}else if(MainForm->m_EnableEnterMetaAfterDL == _TRUE )
	{
	    GC_TRACE("[COM%d] FormatAndDownload(): Enter mata mode after download is enable!!", m_sComPortSetting.com.number);
	    ret = EnterMetaModeAfterDL();		
	}
	else{
	    if(MainForm->m_EnableForceWdtDogReset == 1 && MainForm->m_cbEnableAutoPulling->Checked == false){
			ret =ForceWatchDogReset();
		}
		else if(MainForm->m_EnableWdtDogReset == 1 && MainForm->m_cbEnableAutoPulling->Checked == true){
            ret = WatchDogResetForAutoTest();
        }
	}
	

    
	if(ret != S_DONE)
	{
	   goto FM_And_DL_Finish;
	}
	
FM_And_DL_Finish:
	if(bTodoDisconnectDA == true){
		DisConnectWithDA();
	}
    return ret;
}

int __fastcall TBOOT1::WriteEfuse()
{
    int iRet = -1;
    AnsiString strShow;
    E_EFUSE_WRITE_TYPE efusetype = GetWriteEufseType(frmMultiLoad->m_chip_type);
    SyncUpdateProgressColor(clWhite, clNavy);
	prefix = "WR Efuse ";
    SyncInitProgress();
	
    switch(efusetype)
    {
        case WRITE_EEUSE_BY_BROM:
            iRet = WriteEfuseByBrom();
            break;
        case WRITE_EEUSE_BY_DA:
            iRet = WriteEfuseByDA();
            break;
        default:
            iRet = -1;
            strShow.printf("[COM%d] Error: Un-support chip for writing efuse by DA! ", m_sComPortSetting.com.number);
            SetShowString(strShow);
			break;
	}
    return iRet;
}

int __fastcall TBOOT1::WriteEfuseByBrom()
{
     // For efuse: if Writing efuse by Brom, before DA disconnect, sending watchdog reset cmd
    FlashTool_EnableWDT_Arg wdt_arg;
    wdt_arg.m_timeout_ms = 5000;
    wdt_arg.m_async = _FALSE;
    wdt_arg.m_reboot = _TRUE;
    ret = FlashTool_EnableWatchDogTimeout(m_ft_api_handle, &wdt_arg);
    if(S_DONE != ret){
        GC_TRACE("[COM%d] Enable watchdog timeout...", m_sComPortSetting.com.number);
        return ret;
    }

	DisConnectWithDA();
    GC_TRACE("[COM%d] WriteEfuseByDA(): Write Efuse by Brom begin", m_sComPortSetting.com.number);
    EfuseSettings efuseSettings;
	efuseSettings.ReadEfuseSettings();
	if ( 0 != efuseSettings.GetEfuseEnable() && WRITE_EEUSE_BY_BROM == GetWriteEufseType(frmMultiLoad->m_chip_type))
    {
	
        //TODO: Search BootRom/Preloader COM port
        //Check if USB/UART is available
        ret = CheckPortAvailable();
        GC_TRACE("[COM%d] FormatAndDownload(): Efuse search the new com port complete....", m_sComPortSetting.com.number);
        if(-1 == ret)
        {
            GC_TRACE("[COM%d] FormatAndDownload(): Efuse search the new com port fail....", m_sComPortSetting.com.number);
            return ret;
        }
	
        GC_TRACE("[COM%d] FormatAndDownload(): before getPathFromStr()", m_sComPortSetting.com.number);
        prefix = "Efuse ";
        SyncInitProgress();
        AnsiString asPath = "";
	
        if(getPathFromStr(Application->ExeName,asPath))
        {
            GC_TRACE("[COM%d] Success: FormatAndDownload(): the tool path is %s", m_sComPortSetting.com.number, asPath);
        }
        else
        {
            // If get the tool path fail, we only give the file name to the Efuse API. It will find the file in the current path.
            GC_TRACE("[COM%d] Fail:	FormatAndDownload(): the tool path is %s", m_sComPortSetting.com.number, asPath);
        }
        AnsiString asSettingsFilePath = asPath + "setting.ini";
        GC_TRACE("[COM%d] FormatAndDownload(): The efuse settings file path is %s", m_sComPortSetting.com.number, asSettingsFilePath.c_str());
        AnsiString asLockFilePath = asPath + "lock.ini";
        GC_TRACE("[COM%d] FormatAndDownload(): The efuse lock file path is %s", m_sComPortSetting.com.number, asLockFilePath.c_str());
        AnsiString asReadBackFilePath = asPath + "readback.ini";
        GC_TRACE("[COM%d] FormatAndDownload(): The efuse readback file path is %s", m_sComPortSetting.com.number, asReadBackFilePath.c_str());
	
        std::string settingFilePath = "";
        std::string lockFilePath = "";
        std::string readBackFilePath = "";
	
        if (0 != efuseSettings.GetEfuseSettingsEnable())
        {
            settingFilePath = (std::string)asSettingsFilePath.c_str();
        }
        if (0 != efuseSettings.GetEfuseLockEnable())
        {
			   lockFilePath = (std::string)asLockFilePath.c_str();
        }
        if (0 != efuseSettings.GetEfuseReadBackEnable())
        {
            readBackFilePath = (std::string)asReadBackFilePath.c_str();
        }
	
        GC_TRACE("[COM%d] FormatAndDownload(): Start write efuse....", m_sComPortSetting.com.number);
	
        ret = EfuseWriter( settingFilePath, lockFilePath, readBackFilePath,
							 (unsigned char) m_sComPortSetting.com.number);
	
        GC_TRACE("[COM%d] FormatAndDownload(): Write efuse done, the ret = %d .", m_sComPortSetting.com.number, ret);
	
        if (S_DONE == ret)
        {
            prefix = "Efuse OK";
        }
        else
        {
            prefix = "Efuse ERROR";
            return ret;
        }
	
        SyncInitProgress();
        prefix = "Efuse ";
    }	
	return ret;
}

int __fastcall TBOOT1::DownloadOnly()
{
    if( (MT6516_AP == frmMultiLoad->m_chip_type)/*&&
                                  (true == MainForm->m_miEnableTwoSite->Checked) */)
    {
        return TwoSiteDownload();
    }
    else
    {
        return DL_Android_All();
    }
}

int __fastcall TBOOT1::FirmwareUpgrade(bool bTodoDisconnectDA)
{
    int ret = S_DONE;

    GC_TRACE("[COM%d] FirmwareUpgrade(): bTodoDisconnectDA = %d, if bTodoDisconnectDA is 1, it will do Disconnect when fail", m_sComPortSetting.com.number, bTodoDisconnectDA?1:0);
    //Connect with DA
    prefix = "DA ";
    GC_TRACE("[COM%d] FirmwareUpgrade(): m_is_DA_connected = %d", m_sComPortSetting.com.number, m_is_DA_connected);
    if(false == m_is_DA_connected)
    {
        GC_TRACE("[COM%d] FirmwareUpgrade(): Connect with DA...", m_sComPortSetting.com.number);
        ret  = DAConnect();
        GC_TRACE("[COM%d] FirmwareUpgrade(): After DAConnect.", m_sComPortSetting.com.number);

        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: [Brom] DA connect fail!";
            return ret;
        }
        m_is_DA_connected = true;

        if (true == MainForm->m_miEnableDADL->Checked && true == MainForm->m_miDADLHighSpeed->Checked)
        {
            ret = SwitchComport();
        }

        if (S_DONE != ret)
        {
            if ( -1 == ret){
                GC_TRACE("[COM%d] FirmwareUpgrade(): SwitchComport user stop!", m_sComPortSetting.com.number);
            }
            else{
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] SwitchComport fail!", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: SwitchComport fail!";
            }
            return ret;
        }
    }

	if(MainForm->SecroReworkingEnable && MainForm->SecroReworkRunningFlag)
	{
		GC_TRACE("[COM%d] FirmwareUpgrade():[SWSec] Download SECRO enable!", m_sComPortSetting.com.number);
		ret = DownloadSecro(false); //format disable
		if(S_DONE != ret)
		{
		    GC_TRACE("[COM%d] FirmwareUpgrade():[SWSec] Download SECRO fail!", m_sComPortSetting.com.number);
		    return ret;
		}
	}

    //get ROM_INFO from PC image,then save them to m_vctRomInfo
    GetRomInfoData();
    //get partinfo from target,then save them to m_vctPartInfo
    GetPartitionInfoData();
	
    //Check layout
    ret = IsPartitionLayoutChanged(m_part_status);
	
	switch(m_part_status)
	{
	    case PART_NO_CHANGE:
			ret = LayoutNoChange();
			break;
			
		case PART_HAS_CHANGED:
			ret = LayoutChange();
			break;
			
		case PART_NOT_FOUND:
			ret = LayoutNotFound();
			break;
			
		default:
			break;
	}

	if(ret != S_DONE)
		goto FuncEnd;
/*
    if(PART_NO_CHANGE == m_part_status)
    {
        int iEmmcType = E_LINEAR_EMMC;
        E_CHIP_GROUP eChipGroup;
        eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);

        GC_TRACE("[COM%d] FirmwareUpgrade(): Layout not changed.", m_sComPortSetting.com.number);

		// FAT checked?
        FAT_STATUS fat_status = IsFATChecked();
        GC_TRACE("[COM%d] FirmwareUpgrade(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);

        //if storage type is EMMC, we will check the version of the scatter file, if the version> "V1.1.2", it is combo emmc download.
        if(frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC && eChipGroup == BBCHIP_GROUP2){
			DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
			ret = CheckEmmcType(*p_dl_handle,iEmmcType);
            if(ret != S_DONE){
				GC_TRACE("[COM%d] FirmwareUpgrade(): Error, CheckEmmcType fail!", m_sComPortSetting.com.number);
				return ret;
			}
		}

		if(frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC && iEmmcType == E_COMBO_EMMC){
			ret = FM_ComboEmmc_For_FirmwareUpgrade(m_ft_api_handle);
		}else{
		    // Format from NVRAM end address to FAT start address.
            ret = FM_Android_Partial();
		}
        
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash partial format fail!", m_sComPortSetting.com.number);
            DisConnectWithDA();
            return ret;
        }

        FAT_CHANGED_STATUS fat_changed_status=IsFATChanged();

        GC_TRACE("[COM%d] FirmwareUpgrade(): fat_changed_status is %d .", m_sComPortSetting.com.number, fat_changed_status);
        switch(fat_status)
        {
        case FAT_UNDOWNLOAD:
        case FAT_CHECKED:

			//if it is combo emmc type of download, we do not need do "FM_Android_FAT() " any more.
			if(frmMultiLoad->m_Storage_Type != HW_STORAGE_EMMC || iEmmcType == E_LINEAR_EMMC){
                ret = FM_Android_FAT();
			}
            break;
        case FAT_UNCHECKED:
            if(fat_changed_status==FAT_HAS_CHANGED)
            {
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                if(true == m_is_DA_connected)
                {
                    ret = FlashTool_Disconnect(&m_ft_api_handle);
                    m_is_DA_connected = false;
                }
                return -1;
            }
            break;
			  default:
			      break;
        }

        ret = DL_Android_All();
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
            return ret;
        }

        // Check backup data
        if (S_DONE == CheckBinRegionData())
        {
            // Write back
            GC_TRACE("[COM%d] FirmwareUpgrade(): Layout not changed, find available backup data, Write back BinRegion data...", m_sComPortSetting.com.number);
            ret = WriteBinRegionData();
            if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
                return ret;
            }

            GC_TRACE("[COM%d] FirmwareUpgrade(): Clean BinRegion data...", m_sComPortSetting.com.number);
            ret = CleanBinRegionData();
            if(S_DONE != ret)
            {
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
                return ret;
            }
        }
        else
        {
             GC_TRACE("[COM%d] FirmwareUpgrade():Layout not changed, not find available backup data, do not write back.", m_sComPortSetting.com.number);
	}

    }
    else if(PART_HAS_CHANGED == m_part_status)
    {
        GC_TRACE("[COM%d] FirmwareUpgrade(): Layout changed.", m_sComPortSetting.com.number);
        // FAT Checked?
        FAT_STATUS fat_status = IsFATChecked();
        GC_TRACE("[COM%d] FirmwareUpgrade(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);

        if (fat_status == FAT_UNCHECKED){
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";

            prefix = "FAT ";
            if(true == m_is_DA_connected){
                ret = FlashTool_Disconnect(&m_ft_api_handle);
                m_is_DA_connected = false;
            }
            return -1;
        }

        //before doing backup option, the backup partion's size can not change,so we must check
        if(isPartitionSizeChange()){
           if(true == m_is_DA_connected){
                ret = FlashTool_Disconnect(&m_ft_api_handle);
                m_is_DA_connected = false;
            }
            return -1;
        }

        //Check backup data
        ret = CheckBinRegionData();
        if( S_DONE != ret ){
            ret = BackupBinRegionData();
            if(S_DONE != ret){
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] BinRegion readback fail!", m_sComPortSetting.com.number);
                if(true == m_is_DA_connected)
                {
                    FlashTool_Disconnect(&m_ft_api_handle);
                    m_is_DA_connected = false;
                }
                return ret;
            }
		}

        //Format all
        ret = FM_Android_All();
        if(S_DONE != ret)
        {
		        GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
		        return ret;
        }

        ret = DL_Android_All();
        if(S_DONE != ret)
        {
    		    GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
		        return ret;
        }

		    // Write back
        ret = WriteBinRegionData();
        if(S_DONE != ret)
        {
    		    GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
    		    return ret;
        }

        GC_TRACE("[COM%d] FirmwareUpgrade(): Clean BinRegion data...", m_sComPortSetting.com.number);
        ret = CleanBinRegionData();
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
            return ret;
        }
    }
    else if(PART_NOT_FOUND == m_part_status)
    {
        if(S_DONE != CheckBinRegionBeforeRestore()){
            DisConnectWithDA();
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Check backup bin before restore error!", m_sComPortSetting.com.number);
            return -1;
        }

        //because original upgrade is fail,Format all,but PMT has lost ,FAT has been format.
        ret = FM_Android_All();
        if(S_DONE != ret){
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
            return ret;
        }

        ret = DL_Android_All();
        if(S_DONE != ret){
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
            return ret;
        }

        // Write back
        ret = WriteBinRegionData();
        if(S_DONE != ret){
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
            return ret;
        }

        GC_TRACE("[COM%d] FirmwareUpgrade(): Clean BinRegion data...", m_sComPortSetting.com.number);
        ret = CleanBinRegionData();
        if(S_DONE != ret){
                GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
                return ret;
        }
    }*/

	if(MainForm->m_EnableForceWdtDogReset == 1 && MainForm->m_cbEnableAutoPulling->Checked == false){
		ret =ForceWatchDogReset();
	}
	else if(MainForm->m_EnableWdtDogReset == 1 && MainForm->m_cbEnableAutoPulling->Checked == true){
        ret = WatchDogResetForAutoTest();
    }

FuncEnd:
    if(bTodoDisconnectDA == true)
    {
        DisConnectWithDA();
    }

    return ret;
}


int __fastcall TBOOT1::LayoutNoChange()
{
	int iEmmcType = E_LINEAR_EMMC;
    E_CHIP_GROUP eChipGroup;
    eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);
	
    GC_TRACE("[COM%d] FirmwareUpgrade(): Layout not changed.", m_sComPortSetting.com.number);
	
    // FAT checked?
    FAT_STATUS fat_status = IsFATChecked();
    GC_TRACE("[COM%d] FirmwareUpgrade(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);
	
    //if storage type is EMMC, we will check the version of the scatter file, if the version> "V1.1.2", it is combo emmc download.
    if(frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC){
        DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
        ret = CheckEmmcType(*p_dl_handle,iEmmcType);
        if(ret != S_DONE){
            GC_TRACE("[COM%d] FirmwareUpgrade(): Error, CheckEmmcType fail!", m_sComPortSetting.com.number);
            return ret;
        }
    }

	//format
    if(frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC && iEmmcType == E_COMBO_EMMC){
        ret = FM_ComboEmmc_Partial(m_ft_api_handle);
		if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash partial format fail!", m_sComPortSetting.com.number);
            DisConnectWithDA();
            return ret;
        }
    }else{
        // Format from NVRAM end address to FAT start address.
        ret = FM_Android_Partial();

		if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash partial format fail!", m_sComPortSetting.com.number);
            DisConnectWithDA();
            return ret;
        }

		FAT_CHANGED_STATUS fat_changed_status=IsFATChanged();
	
        GC_TRACE("[COM%d] FirmwareUpgrade(): fat_changed_status is %d .", m_sComPortSetting.com.number, fat_changed_status);
        switch(fat_status)
        {
            case FAT_UNDOWNLOAD:
            case FAT_CHECKED:
			case FAT_NO_FOUND_IN_SCATTER:
                break;
		
            case FAT_UNCHECKED:
                if(fat_changed_status==FAT_HAS_CHANGED)
                {
                    GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
                    m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                    if(true == m_is_DA_connected)
                    {
                        ret = FlashTool_Disconnect(&m_ft_api_handle);
                        m_is_DA_connected = false;
                    }
                    return -1;
                }else{
                    ret = FM_Android_FAT();
                }
                break;
            default:
                break;
        }

		
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Flash partial format fail!", m_sComPortSetting.com.number);
			DisConnectWithDA();
			return ret;
		}
    }
			
	
    ret = DL_Android_All();
    if(S_DONE != ret)
    {
        GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
        return ret;
    }
	
    // Check backup data
    if (S_DONE == CheckBinRegionBeforeRestore())
    {
        // Write back
        GC_TRACE("[COM%d] FirmwareUpgrade(): Layout not changed, find available backup data, Write back BinRegion data...", m_sComPortSetting.com.number);
        ret = WriteBinRegionData();
        if(S_DONE != ret)
        {
			GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
			return ret;
        }
	
        GC_TRACE("[COM%d] FirmwareUpgrade(): Clean BinRegion data...", m_sComPortSetting.com.number);
        ret = CleanBinRegionData();
        if(S_DONE != ret)
        {
            GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
            return ret;
        }
	}
	else
	{
        GC_TRACE("[COM%d] FirmwareUpgrade():Layout not changed, and not find available backup data, do not write back.", m_sComPortSetting.com.number);
	}

	return ret;

}

int __fastcall TBOOT1::LayoutChange()
{
	GC_TRACE("[COM%d] LayoutChange(): Layout changed.", m_sComPortSetting.com.number);
	
	// FAT Checked?
	FAT_STATUS fat_status = IsFATChecked();
	GC_TRACE("[COM%d] LayoutChange(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);
	
	if (fat_status == FAT_UNCHECKED){
		GC_TRACE("[COM%d] LayoutChange(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
		m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
	
		prefix = "FAT ";
		if(true == m_is_DA_connected){
			ret = FlashTool_Disconnect(&m_ft_api_handle);
			m_is_DA_connected = false;
		}

		return -1;
    }
	
	//before doing backup option, the backup partion's size can not change,so we must check
	if(isPartitionSizeChange()){
        if(true == m_is_DA_connected){
		    ret = FlashTool_Disconnect(&m_ft_api_handle);
			m_is_DA_connected = false;
		}

		return -1;
	}
	
	if(IsHaveProtectRegion() != S_DONE) 
	{
	    GC_TRACE("[COM%d] LayoutChange(): IsHaveProtectRegion check fail",m_sComPortSetting.com.number);
	    return -1;
	}
	
	ret = CheckBinRegionBeforeRestore();

	//if CheckBinRegionBeforeRestore() pass, which the protect partition was backuped before, so no need to backup again.
	if( S_DONE != ret ){
		ret = BackupBinRegionData();
		if(S_DONE != ret){
			GC_TRACE("[COM%d] LayoutChange(): [ERROR] BinRegion readback fail!", m_sComPortSetting.com.number);
			if(true == m_is_DA_connected)
			{
				FlashTool_Disconnect(&m_ft_api_handle);
			    m_is_DA_connected = false;
			}

			return ret;
		}
	}
	
    //Format all
	ret = FM_Android_All();
	if(S_DONE != ret)
	{
		GC_TRACE("[COM%d] LayoutChange(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
	    return ret;
	}
	
	ret = DL_Android_All();
	if(S_DONE != ret)
	{
		GC_TRACE("[COM%d] LayoutChange(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
		return ret;
	}
	
	// Write back
	ret = WriteBinRegionData();
	if(S_DONE != ret)
	{
		GC_TRACE("[COM%d] LayoutChange(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
		return ret;
	}
	
	GC_TRACE("[COM%d] LayoutChange(): Clean BinRegion data...", m_sComPortSetting.com.number);
	ret = CleanBinRegionData();
	if(S_DONE != ret)
	{
		GC_TRACE("[COM%d] LayoutChange(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
		return ret;
    }

	return ret;

}

int __fastcall TBOOT1::LayoutNotFound()
{

    GC_TRACE("[COM%d] LayoutNotFound(): Enter...", m_sComPortSetting.com.number);
	if(S_DONE != CheckBinRegionBeforeRestore()){
		DisConnectWithDA();
		GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] Check backup bin before restore error!", m_sComPortSetting.com.number);
		return -1;
	}
	
	//because original upgrade is fail,Format all,but PMT has lost ,FAT has been format.
	ret = FM_Android_All();
	if(S_DONE != ret){
		GC_TRACE("[COM%d] LayoutNotFound(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
		return ret;
	}
	
	ret = DL_Android_All();
	if(S_DONE != ret){
		GC_TRACE("[COM%d] LayoutNotFound(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
		return ret;
	}
	
	// Write back
	ret = WriteBinRegionData();
	if(S_DONE != ret){
		GC_TRACE("[COM%d] LayoutNotFound(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
		return ret;
	}
	
	GC_TRACE("[COM%d] LayoutNotFound(): Clean BinRegion data...", m_sComPortSetting.com.number);
	ret = CleanBinRegionData();
	if(S_DONE != ret){
		GC_TRACE("[COM%d] LayoutNotFound(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
		return ret;
	}

    return ret;
}


int __fastcall TBOOT1::OTPLock()
{
    int ret=S_DONE;
	//Connect with DA;
	prefix = "DA ";
	GC_TRACE("[COM%d] OTPLock(): m_is_DA_connected = %d", m_sComPortSetting.com.number, m_is_DA_connected);
	if(false == m_is_DA_connected)
	{
	   GC_TRACE("[COM%d] OTPLock(): Connect with DA...", m_sComPortSetting.com.number);
	   ret = DAConnect();
	   GC_TRACE("[COM%d] OTPLock(): After DAConnect.", m_sComPortSetting.com.number);

	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d] OTPLock(): [ERROR] DA connect fail", m_sComPortSetting.com.number);
		  m_asMsg = " ERROR: DA connect fail";
		  return ret;
	   }
	   m_is_DA_connected = true;
	   if(true == MainForm->m_miEnableDADL->Checked && true == MainForm->m_miDADLHighSpeed->Checked)
	   {
	      ret = SwitchComport();
		  if(S_DONE != ret)
		  {
		     if( -1 == ret)
		     {
		         GC_TRACE("[COM%d] OTPLock(): SwitchComport user stop!", m_sComPortSetting.com.number);
		     }
			 else
			 {
			     GC_TRACE("[COM%d] OTPLock(): [Error] SwitchComport fail!", m_sComPortSetting.com.number);
				 m_asMsg = "ERROR: Switch Comport fail!";
			 }
			 return ret;
		  }
	   }
	}

    prefix = "OTP LK";
	GC_TRACE("[COM%d] OTPLock(): OTP Lock Status check...", m_sComPortSetting.com.number);
	ret = FlashTool_OTP_LockCheckStatus(m_ft_api_handle);

	if(ret != S_DA_OTP_UNLOCKED)
	{
	   if(ret == S_DA_OTP_LOCKED)
	   {
	      GC_TRACE("[COM%d] OTPLock(): [ERROR]OTP status is locked!", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: OTP staus is locked!";
		  return ret;
	   }
	   GC_TRACE("[COM%d] OTPLock(): [ERROR]OTP Status is not unlocked",m_sComPortSetting.com.number);
	   m_asMsg = "ERROR: OTP status is not unlockde";
	   return ret;
	}

	ret = FlashTool_OTP_Lock(m_ft_api_handle);
	if(ret != S_DONE)
	{
	   GC_TRACE("[COM%d] OTPLock(): [ERROR]OTP Lock fail!", m_sComPortSetting.com.number);
	   m_asMsg = "ERROR: OTP Lock fail!";
	   return ret;
	}

	if(MainForm->m_EnableWdtDogReset == 1)
	{
	   WatchDogResetForAutoTest();
	}
	if(true == m_is_DA_connected)
	{
	   ret = FlashTool_Disconnect(&m_ft_api_handle);
	   m_is_DA_connected = false;
	}

	return ret;

}


int __fastcall TBOOT1::FirmwareUpgradeAndOTPLock()
{
    
	/*int ret = S_DONE;
	
	//Connect with DA
	prefix = "DA ";
	GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): m_is_DA_connected = %d", m_sComPortSetting.com.number, m_is_DA_connected);
	if(false == m_is_DA_connected)
	{
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Connect with DA...", m_sComPortSetting.com.number);
		ret  = DAConnect();
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): After DAConnect.", m_sComPortSetting.com.number);
	
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] DA connect fail!", m_sComPortSetting.com.number);
			m_asMsg = "ERROR: [Brom] DA connect fail!";
			return ret;
		}
		m_is_DA_connected = true;
	
		if (true == MainForm->m_miEnableDADL->Checked && true == MainForm->m_miDADLHighSpeed->Checked)
		{
			ret = SwitchComport();
		}
	
		if (S_DONE != ret)
		{
			if ( -1 == ret)
			{
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): SwitchComport user stop!", m_sComPortSetting.com.number);
			}
			else
			{
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] SwitchComport fail!", m_sComPortSetting.com.number);
				m_asMsg = "ERROR: SwitchComport fail!";
			}
			return ret;
		}
	}
	
	
	//get ROM_INFO from PC image,then save them to m_vctRomInfo and get partinfo from target,then save them to m_vctPartInfo
	GetRomInfoData();
	GetPartitionInfoData();

	//Check layout
	ret = IsPartitionLayoutChanged(m_part_status);
	
	if(PART_NO_CHANGE == m_part_status)
	{
		// FAT checked?
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Layout not changed.", m_sComPortSetting.com.number);
		FAT_STATUS fat_status = IsFATChecked();
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);
	
		// Format from NVRAM end address to FAT start address.
		ret = FM_Android_Partial_ExeceptOTP();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Flash partial format fail!", m_sComPortSetting.com.number);
			DisConnectWithDA();
			return ret;
		}
	
	
		FAT_CHANGED_STATUS fat_changed_status=IsFATChanged();
	
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): fat_changed_status is %d .", m_sComPortSetting.com.number, fat_changed_status);
		switch(fat_status)
		{
		case FAT_UNDOWNLOAD:
		case FAT_CHECKED:
		    ret = FM_Android_FAT();
			break;
		case FAT_UNCHECKED:
			if(fat_changed_status==FAT_HAS_CHANGED)
			{
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
				m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
				if(true == m_is_DA_connected)
				{
					ret = FlashTool_Disconnect(&m_ft_api_handle);
					m_is_DA_connected = false;
				}
				return -1;
			}
			break;
				 default:
					 break;
		}
	
		ret = DL_Android_All();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
			return ret;
		}
	
		// Check backup data
		if (S_DONE == CheckBinRegionData())
		{
			// Write back
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Layout not changed, find available backup data, Write back BinRegion data...", m_sComPortSetting.com.number);
			ret = WriteBinRegionData();
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
				return ret;
			}
	
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Clean BinRegion data...", m_sComPortSetting.com.number);
			ret = CleanBinRegionData();
			if(S_DONE != ret)
			{
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
				return ret;
			}
		}
	    else
	   {
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock():Layout not changed, not find available backup data, do not write back.", m_sComPortSetting.com.number);
	   }
	
	}
	else if(PART_HAS_CHANGED == m_part_status)
	{
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Layout changed.", m_sComPortSetting.com.number);
		// FAT Checked?
		FAT_STATUS fat_status = IsFATChecked();
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): fat_status is %d .", m_sComPortSetting.com.number, fat_status);
	
		if (fat_status == FAT_UNCHECKED){
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
			m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
	
			prefix = "FAT ";
			if(true == m_is_DA_connected){
				ret = FlashTool_Disconnect(&m_ft_api_handle);
				m_is_DA_connected = false;
			}
			return -1;
		}
	
		//before doing backup option, the backup partion's size can not change,so we must check
		if(isPartitionSizeChange()){
			if(true == m_is_DA_connected){
				ret = FlashTool_Disconnect(&m_ft_api_handle);
				m_is_DA_connected = false;
			}
			return -1;
		}
	
		//Check backup data
		ret = CheckBinRegionData();
		if( S_DONE != ret ){
			ret = BackupBinRegionData();
			if(S_DONE != ret){
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] BinRegion readback fail!", m_sComPortSetting.com.number);
				if(true == m_is_DA_connected)
				{
					FlashTool_Disconnect(&m_ft_api_handle);
					m_is_DA_connected = false;
				}
				return ret;
			}
		}
	
		//Format all
		ret = FM_Android_All_ExeceptOTP();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
			return ret;
		}
	
		ret = DL_Android_All();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
			return ret;
		}
	
		// Write back
		ret = WriteBinRegionData();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
			return ret;
		}
	
		GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Clean BinRegion data...", m_sComPortSetting.com.number);
		ret = CleanBinRegionData();
		if(S_DONE != ret)
		{
			GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
			return ret;
				}
		}
		else if(PART_NOT_FOUND == m_part_status)
		{
			if(S_DONE != CheckBinRegionBeforeRestore()){
				DisConnectWithDA();
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Check backup bin before restore error!", m_sComPortSetting.com.number);
				return -1;
			}
	
		   //because original upgrade is fail,Format all,but PMT has lost ,FAT has been format.
		   ret = FM_Android_All();
		   if(S_DONE != ret){
			  GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Flash format all fail!", m_sComPortSetting.com.number);
			  return ret;
		   }
	
		   ret = DL_Android_All();
		   if(S_DONE != ret){
			  GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Image download fail!", m_sComPortSetting.com.number);
			  return ret;
		   }
	
		   // Write back
		   ret = WriteBinRegionData();
		   if(S_DONE != ret){
			  GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Write BinRegion data fail!", m_sComPortSetting.com.number);
			  return ret;
		   }
	
		   GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): Clean BinRegion data...", m_sComPortSetting.com.number);
		   ret = CleanBinRegionData();
		   if(S_DONE != ret){
				GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR] Clean BinRegion data fail!", m_sComPortSetting.com.number);
				return ret;
		   }
	  }*/

    ret = FirmwareUpgrade(false);
    if(ret != S_DONE)
    {
        GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): FirmwareUpgrade fail before doing OTP lock!", m_sComPortSetting.com.number);
        goto FuncEnd;
		//return ret;
    }
	
    prefix = "OTP LK";
	GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): OTP Lock Status check...", m_sComPortSetting.com.number);
	ret = FlashTool_OTP_LockCheckStatus(m_ft_api_handle);

	if(ret != S_DA_OTP_UNLOCKED)
	{  

	   if(ret == S_DA_EMMC_OTP_LOCKED)
	   {
	      GC_TRACE("[COM%d] FirmwareUpgradeAndOTPLock(): [ERROR]OTP Status is locked!", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: OTP status is locked!";
		  //return ret;
		  goto FuncEnd;
	   }
	   
	   GC_TRACE("[COM%d] OTPLock(): [ERROR]OTP Status is not unlocked",m_sComPortSetting.com.number);
	   m_asMsg = "ERROR: OTP status is not unlockde";
	   goto FuncEnd;
	}

	ret = FlashTool_OTP_Lock(m_ft_api_handle); 
	//ret = S_DA_OTP_UNLOCKED;

	if(ret != S_DONE)
	{
	   GC_TRACE("[COM%d] OTPLock(): [ERROR]OTP Lock fail!", m_sComPortSetting.com.number);
	   m_asMsg = "ERROR: OTP Lock fail!";
	   goto FuncEnd;
	}	

FuncEnd:
    DisConnectWithDA();
	return ret;
}


int __fastcall TBOOT1::OTPFormat()
{
    int ret=S_DONE;
	//Connect with DA;
	prefix = "DA ";
	GC_TRACE("[COM%d] OTPFormat(): m_is_DA_connected = %d", m_sComPortSetting.com.number, m_is_DA_connected);
	if(false == m_is_DA_connected)
	{
	   GC_TRACE("[COM%d] OTPFormat(): Connect with DA...", m_sComPortSetting.com.number);
	   ret = DAConnect();
	   GC_TRACE("[COM%d] OTPFormat(): After DAConnect.", m_sComPortSetting.com.number);

	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d] OTPFormat(): [ERROR] DA connect fail", m_sComPortSetting.com.number);
		  m_asMsg = " ERROR: DA connect fail";
		  return ret;
	   }
	   m_is_DA_connected = true;
	   if(true == MainForm->m_miEnableDADL->Checked && true == MainForm->m_miDADLHighSpeed->Checked)
	   {
	      ret = SwitchComport();
		  if(S_DONE != ret)
		  {
		     if( -1 == ret)
		     {
		         GC_TRACE("[COM%d] OTPFormat(): SwitchComport user stop!", m_sComPortSetting.com.number);
		     }
			 else
			 {
			     GC_TRACE("[COM%d] OTPFormat(): [Error] SwitchComport fail!", m_sComPortSetting.com.number);
				 m_asMsg = "ERROR: Switch Comport fail!";
			 }
			 return ret;
		  }
	   }
	}


    prefix = "OTP FR";

	GC_TRACE("[COM%d] OTPFormat(): Set format parameters...", m_sComPortSetting.com.number);
	FlashTool_Format_Arg    fm_arg;
	FlashTool_Format_Result fm_res;

	//Common part
    fm_arg.m_storage_type = frmMultiLoad->m_Storage_Type; //HW_STORAGE_NAND;
    fm_arg.m_format_cfg.m_auto_format_fat = _FALSE;
    fm_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fm_arg.m_erase_flag = NUTL_ERASE;
    fm_arg.m_cb_format_report_init =::cb_format_report_init;
    fm_arg.m_cb_format_report_init_arg = this;
    fm_arg.m_cb_format_report =::cb_format_report;
    fm_arg.m_cb_format_report_arg =this;
    fm_arg.m_cb_format_statistics= ::cb_format_statistics;
    fm_arg.m_cb_format_statistics_arg = this;

	ULL64 startAddrOTP;
    ULL64 OTPNum;
	ULL64 BMTNum;
	
	PART_INFO fatPartInfo;
	E_CHIP_GROUP eChipGroup;
	eChipGroup = MainForm->CheckChipGroup(frmMultiLoad->m_chip_type);
    
	if(eChipGroup != BBCHIP_GROUP0)
	{
	   PART_INFO partOTPInfo;
	   PART_INFO partBMTInfo;
	   ret = GetPartitionInfo(SCATTER_OTP_NAME, partOTPInfo);
	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d] OTPFormat(): [ERROR]Getting readback PMT information fail", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: Get OTP Partition info fail!";
		  FlashTool_Disconnect(&m_ft_api_handle);
		  m_is_DA_connected = false;
		  return ret;
	   }
	   OTPNum = partOTPInfo.begin_addr & 0x0000FFFF;
       
       ret = GetPartitionInfo(SCATTER_RSV_BMT_NAME, partBMTInfo);
	   if(S_DONE != ret)
	   {
	      GC_TRACE("[COM%d OTPFormat(): [ERROR]Getting readback BMT information fail]", m_sComPortSetting.com.number);
		  m_asMsg = "ERROR: Get BMT Partition info fail!";
		  FlashTool_Disconnect(&m_ft_api_handle);
		  m_is_DA_connected = false;
		  return ret;
	   }
	   BMTNum = partBMTInfo.begin_addr & 0x0000FFFF;
	   
	   if((eChipGroup == BBCHIP_GROUP2) && frmMultiLoad->m_Storage_Type == HW_STORAGE_EMMC)
	   {
	       GC_TRACE("[COM%d] OTPFormat(): BBCHIP_MT6575/6577/6589 and HW_STORAGE_EMMC!", m_sComPortSetting.com.number);
	       fm_arg.m_format_cfg.m_AddrType = NUTL_ADDR_LOGICAL;
	       //fm_arg.m_format_cfg.m_format_begin_addr = startAddrPartial;
		   fm_arg.m_format_cfg.m_format_begin_addr= m_sConnResult.m_da_report.m_emmc_boot1_size
									+ m_sConnResult.m_da_report.m_emmc_boot2_size
									+ m_sConnResult.m_da_report.m_emmc_rpmb_size
									+ m_sConnResult.m_da_report.m_emmc_gp1_size
									+ m_sConnResult.m_da_report.m_emmc_gp2_size
									+ m_sConnResult.m_da_report.m_emmc_gp3_size
									+ m_sConnResult.m_da_report.m_emmc_gp4_size
									+ m_sConnResult.m_da_report.m_emmc_ua_size
									- OTPNum*128*1024;

	       fm_arg.m_format_cfg.m_format_length = OTPNum*128*1024;
		   GC_TRACE("[COM%d] OTPFormat(): StartAddr(0x%LLX), Len(0x%LLX)", m_sComPortSetting.com.number,
		   	          fm_arg.m_format_cfg.m_format_begin_addr,fm_arg.m_format_cfg.m_format_length);
		   
		   GC_TRACE("[COM%d] OTPFormat(): Do format...", m_sComPortSetting.com.number);

		   GC_TRACE("[COM%d] OTPFormat(): OTP Lock Status check...", m_sComPortSetting.com.number);
	       ret = FlashTool_OTP_LockCheckStatus(m_ft_api_handle);

           if(ret != S_DA_OTP_UNLOCKED)
	       {
	          GC_TRACE("[COM%d] OTPFormat(): [ERROR]OTP Status is not unlocked",m_sComPortSetting.com.number);
	          m_asMsg = "ERROR: OTP status is not unlocked";
	          return ret;
	       }
		   
		   ret = FlashTool_Format(m_ft_api_handle, &fm_arg, &fm_res);
		   if(S_DONE != ret)
		   {
		      GC_TRACE("[COM%d] OTPFormat: [ERROR] OTP format fail!", m_sComPortSetting.com.number);
			  m_asMsg = "ERROR, Format OTP fail!";
			  FlashTool_Disconnect(&m_ft_api_handle);
			  m_is_DA_connected = false;
			  return ret;
		   }
		   
	    }
	   else
	    {
	       GC_TRACE("[COM%d] OTPFormat(): OTP format not support!",m_sComPortSetting.com.number);
		   m_asMsg = "ERROR, OTP format not support!";
		   FlashTool_Disconnect(&m_ft_api_handle);
		   m_is_DA_connected = false;
		   return -1;
	    }
	   
	}
	else{
		GC_TRACE("[COM%d] OTPFormat(): un-support chip!",m_sComPortSetting.com.number);
		DisConnectWithDA();
		return -1;
	}
	
    return ret;
}

int __fastcall TBOOT1::TwoSiteDownload()
{
    if(true == MainForm->rbUART->Checked)
    {
        GC_TRACE("[COM%d] TwoSiteDownload(): First site (UART)...", m_sComPortSetting.com.number);
        ret = DL_Android_FirstSite();
    }
    else
    {
        GC_TRACE("[COM%d] TwoSiteDownload(): Second site (USB)...", m_sComPortSetting.com.number);
        ret = DL_Android_SecondSite();
    }

    return ret;
}

int __fastcall TBOOT1::DisConnectWithDA()
{
    int iRet = S_DONE;
    if(true == m_is_DA_connected)
    {
        iRet = FlashTool_Disconnect(&m_ft_api_handle);
        m_is_DA_connected = false;
    }

	return iRet;
}

bool TBOOT1::GetSymbolicName(char * sz_port_number, char * sz_symbol_name)
{
	HKEY hUSBKey, hAddKey;
	long ret;
	char tmpBuf[100];
	string tmpStr;

	char sz_USB_Subpath[512] = "SYSTEM\\CurrentControlSet\\Enum\\USB\\";
	char sz_USB_SubKey[100];

	char sz_tmp_COM[20];
	char sz_tmp_Symbolic[512];

	vector<string> v_USB_SubKey;

	int USBIndex = 0;
	unsigned long keyLen = 100, dataLen;

	memset(sz_tmp_Symbolic, 0, 512);

	//Locate USB key
	sprintf(tmpBuf, "Vid_%s&Pid_%s\\", VID_ANDROID, PID_PROLOADER);
	strcat(sz_USB_Subpath, tmpBuf);
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sz_USB_Subpath, 0, KEY_READ, &hUSBKey);

	//Find subkeys in the USB path
	if(ERROR_SUCCESS == ret)
	{
		do
		{
			ret = RegEnumKeyEx(hUSBKey, USBIndex++, sz_USB_SubKey, &keyLen, NULL, NULL, NULL, NULL);
			if(ERROR_SUCCESS == ret)
			{
				tmpStr.assign(sz_USB_SubKey);
				v_USB_SubKey.push_back(tmpStr);
				memset(sz_USB_SubKey, 0, 100);
				keyLen = 100;
			}
		} while(ERROR_SUCCESS == ret);

		for( vector<string>::iterator it = v_USB_SubKey.begin(); it < v_USB_SubKey.end(); it++)
		{
			memset(tmpBuf, 0, 100);
			sprintf(tmpBuf, "%s\\Device Parameters\\", (*it).c_str());

			//Open subitem key for every driver installed
			ret = RegOpenKeyEx(hUSBKey, tmpBuf, 0, KEY_READ, &hAddKey);
	//    		GC_TRACE("USB_Subpath: %s%s\n", sz_USB_Subpath, tmpBuf);

			if(ERROR_SUCCESS == ret)
			{
				//Restore valueLen for every use, otherwise
				dataLen = 20;
				ret = RegQueryValueEx(hAddKey, "PortName", NULL, NULL, (LPBYTE)sz_tmp_COM, &dataLen);
				dataLen = 512;
				ret = RegQueryValueEx(hAddKey, "SymbolicName", NULL, NULL, (LPBYTE)sz_tmp_Symbolic, &dataLen);
	//    			GC_TRACE("PortName: %s\nSymbolicName: %s\n", sz_tmp_COM, sz_tmp_Symbolic);
				if( 0 == strncmp(sz_port_number, sz_tmp_COM, strlen(sz_port_number)) )
				{
					strncpy(sz_symbol_name, sz_tmp_Symbolic, strlen(sz_tmp_Symbolic));
					return true;
				}
			}
			else
			{
			         GC_TRACE("RegOpenKey error!");
	                       return false;
			}
		}
	}
	else
	{
	    GC_TRACE("RegOpenKey error!");
	    return false;
	}

	return false;
}


int TBOOT1::CheckPortAvailable()
{
    HANDLE hCOM = NULL;

    int iBromComNum = GetCOMPortNumByComType(BOOTROM_COMPORT);
    int iPreLoaderComNum = GetCOMPortNumByComType(PRELOADER_COMPORT);
    int i = GetCOMPortIndex();
    int * p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);

    char pBromComStr[128];
    char pPreLoaderComStr[128];

    sprintf(pBromComStr,"\\\\.\\COM%d", iBromComNum);
    sprintf(pPreLoaderComStr,"\\\\.\\COM%d", iPreLoaderComNum);
    GC_TRACE("[COM%d] CheckPortAvailable(): Try to open COM%d port...(Polling),comStr = %s", m_sComPortSetting.com.number, iBromComNum, pBromComStr);

    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] CheckPortAvailable(): User press stop button!", m_sComPortSetting.com.number);
            return -1;
        }

        //Try to open bootrom comport
        if(iBromComNum >0){
            hCOM = CreateFile( pBromComStr, GENERIC_READ |GENERIC_WRITE, 0, NULL,
                                                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
            if(INVALID_HANDLE_VALUE == hCOM){
                //GC_TRACE("[COM%d] CheckPortAvailable(): Cannot open bootrom USB port!", m_sComPortSetting.com.number);
            }
            else{
                GC_TRACE("[COM%d] CheckPortAvailable(): ++++++Open bootrom USB succeed!++++++", m_sComPortSetting.com.number);
                CloseHandle(hCOM);
                UpdateCOMPortNumber(iBromComNum);
                m_iDownloadComportType = BOOTROM_COMPORT;
                return 0;
            }
        }
         Sleep(10);

        //Try to open preloader comport
        if(iPreLoaderComNum >0){
            hCOM = CreateFile( pPreLoaderComStr, GENERIC_READ |GENERIC_WRITE, 0, NULL,
                                                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
            if(INVALID_HANDLE_VALUE == hCOM){
                //GC_TRACE("[COM%d] CheckPortAvailable(): Cannot open preloader USB port!", m_sComPortSetting.com.number);
            }
            else{
                GC_TRACE("[COM%d] CheckPortAvailable(): ++++++Open preloader USB succeed!++++++", m_sComPortSetting.com.number);
                CloseHandle(hCOM);
                UpdateCOMPortNumber(iPreLoaderComNum);
                m_iDownloadComportType = PRELOADER_COMPORT;
                return 0;
            }
        }
        Sleep(10);
    }while(1);
	
}

int TBOOT1::CheckPortUnavailable()
{
    HANDLE hCOM = NULL;

    int portNumber = GetCOMPortNumber();
    int i = GetCOMPortIndex();
    int * p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);

    char comStr[128];
    sprintf(comStr,"\\\\.\\COM%d", portNumber);


    GC_TRACE("[COM%d] CheckPortUnavailable(): Try to open COM%d port...(Polling)", m_sComPortSetting.com.number, portNumber);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] CheckPortUnavailable(): User press stop button!", m_sComPortSetting.com.number);
            return -1;
        }

        hCOM = CreateFile( comStr, GENERIC_READ |GENERIC_WRITE, 0, NULL,
                                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if(INVALID_HANDLE_VALUE == hCOM)
        {
            GC_TRACE("[COM%d] CheckPortUnavailable(): ++++++BootRom USB has been closed!++++++", m_sComPortSetting.com.number);
            return 0;
        }
        else
        {
            GC_TRACE("[COM%d] CheckPortUnavailable(): BootRom USB is still alive.", m_sComPortSetting.com.number);
            CloseHandle(hCOM);

        }

        Sleep(200);

    }while(1);
}

int TBOOT1::CheckUSBPortType(int comNo)
{
    unsigned long	device_number = 0;
    char			device_path[512];
    unsigned long	device_path_real_length;
    char			port_name[512];
    unsigned long	port_name_real_length;
    int    num;
    //bool    isFound = false;

    CustomerSetup customerSetup;

    customerSetup.ReadCustomerSettings();
    std::string customerPid;

    std::string customerVid = customerSetup.GetAndroidVid();
    std::string DAHighSpeedPid = customerSetup.GetDAHighSpeedPid();

    GUID customerGuid = customerSetup.GetGUID();

    Sleep(300);  //wait for register ready

    if (m_iDownloadComportType == BOOTROM_COMPORT)
    {
        customerPid = customerSetup.GetBromPid();
        GC_TRACE("[COM%d] CheckUSBPortType(): Check Bootrom comport type!", m_sComPortSetting.com.number);
    }
    else
    {
        customerPid = customerSetup.GetPreloaderPid();
        GC_TRACE("[COM%d] CheckUSBPortType(): Check preloader comport type!", m_sComPortSetting.com.number);
    }

    memset(device_path, 0, 512);
    memset(port_name, 0, 512);

    device_number = GetDeviceNumber(customerGuid, customerPid.c_str(), customerVid.c_str());

    if( device_number == 0)
    {
        m_asMsg.sprintf("ERROR: No Bootrom(preloader) USB port was found! COM%d is not a Bootrom(preloader) USB port.", comNo);
        GC_TRACE("[COM%d] CheckPortType(): [ERROR] No Bootrom(preloader) USB port was found! COM%d is not a Bootrom(preloader) USB port.", m_sComPortSetting.com.number, comNo);
        return -1;
    }

    for(unsigned int i = 0; i < device_number; i++)
    {
        if(GetPresentDevicePath(customerGuid, customerPid.c_str(), customerVid.c_str(), i,
            port_name, 512, (unsigned long *) &port_name_real_length,
            device_path, 512, (unsigned long *) &device_path_real_length))
        {
            if(!FriendlyToNum(port_name, &num))
            {
                GC_TRACE("[COM%d] CheckPortType(): [ERROR] Transfer symbolic to num fail!", m_sComPortSetting.com.number);
                return -1;
            }

            if(num == comNo)
            {
                GC_TRACE("[COM%d] CheckPortType(): Port type check success! COM%d is a Bootrom(preloader) USB port.", m_sComPortSetting.com.number, comNo);
                return 0;
            }
        }
        else
        {
            GC_TRACE("[COM%d] CheckPortType(): [ERROR] Get_Device_Path() fail!", m_sComPortSetting.com.number);
            return -1;
        }
    }


    m_asMsg.sprintf("ERROR: Port type check fail! COM%d is not a Bootrom(preloader) USB port.", comNo);
    GC_TRACE("[COM%d] CheckPortType(): [ERROR] Port type check fail! COM%d is not a Bootrom(preloader) USB port.", m_sComPortSetting.com.number, comNo);
    return -1;

}


int TBOOT1::CheckUARTPortType(int comNo)
{
    unsigned long	device_number = 0;
    char			device_path[512];
    unsigned long	device_path_real_length;
    char			port_name[512];
    unsigned long	port_name_real_length;
    int    num;
    //bool    isFound = false;

    memset(device_path, 0, 512);
    memset(port_name, 0, 512);

    device_number =  GetDeviceNumber(InterfaceClassGuidConstant, PID_BOOTROM, VID_ANDROID);
    if( device_number == 0)
    {
        m_asMsg.sprintf("ERROR: No Bootrom USB port was found! COM%d is not a Bootrom USB port.", comNo);
        GC_TRACE("[COM%d] CheckPortType(): [ERROR] No Bootrom USB port was found! COM%d is not a Bootrom USB port.", m_sComPortSetting.com.number, comNo);
        return -1;
    }


    for(unsigned int i = 0; i < device_number; i++)
    {
        if(GetPresentDevicePath(InterfaceClassGuidConstant, PID_BOOTROM, VID_ANDROID, i,
            port_name, 512, (unsigned long *) &port_name_real_length,
            device_path, 512, (unsigned long *) &device_path_real_length))
        {
            if(!FriendlyToNum(port_name, &num))
            {
                GC_TRACE("[COM%d] CheckPortType(): [ERROR] Transfer symbolic to num fail!", m_sComPortSetting.com.number);
                return -1;
            }

            if(num == comNo)
            {
                GC_TRACE("[COM%d] CheckPortType(): Port type check succeed! COM%d is a Bootrom USB port.", m_sComPortSetting.com.number, comNo);
                return 0;
            }
        }
        else
        {
            GC_TRACE("[COM%d] CheckPortType(): [ERROR] Get_Device_Path() fail!", m_sComPortSetting.com.number);
            return -1;
        }
    }


    m_asMsg.sprintf("ERROR: Port type check fail! COM%d is not a Bootrom USB port.", comNo);
    GC_TRACE("[COM%d] CheckPortType(): [ERROR] Port type check fail! COM%d is not a Bootrom USB port.", m_sComPortSetting.com.number, comNo);
    return -1;


}


//==============================================================================
void __fastcall TBOOT1::Execute()
{
    GC_TRACE("[COM%d] Execute(): ================Operation start================", m_sComPortSetting.com.number);
    GC_TRACE("[COM%d] Execute(): Update UI for operation start...", m_sComPortSetting.com.number);

    int i = GetCOMPortIndex();
    ostringstream bom_prefix;

    SyncStart();
    ret = S_DONE;
    MainForm->btnCOMStartArray[i]->Enabled = false;

    ResetArgMemory();
    FlashtoolArgSetting();

    if( (MT6516_AP != frmMultiLoad->m_chip_type) &&
	    (OP_FORMAT_BOOTLOADER != MainForm->cbType->ItemIndex) )
    {
        //Check if USB/UART is available
        ret = CheckPortAvailable();
        if(-1 == ret){
            goto clean;
        }

        ret = CheckUSBPortType(m_sComPortSetting.com.number);
        if(-1 == ret){
            goto clean;
        }

    }

    // Add comport prefix to every brom log.
    GC_TRACE("[COM%d] Execute(): Add prefix to every brom log", m_sComPortSetting.com.number);
    bom_prefix << "COM" << (unsigned short)m_sComPortSetting.com.number;
    Brom_Debug_SetLogPrefix(bom_prefix.str().c_str());

#if 0
    //Do prior operation, such as backup NVRAM data
    GC_TRACE("[COM%d] Do META action before format/download", m_sComPortSetting.com.number);
    ret = DoPriorOperation();
    if( 0!= ret )
    {
        goto clean;
    }
#endif

    if(Form_tool_settings->m_cbOTPLock->Checked||Form_tool_settings->m_cbOTPFormat->Checked
        ||Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked)
    { 
        //Do OTP operation
        GC_TRACE("[COM%d] Execute(): Do OTP Operations...", m_sComPortSetting.com.number);
        if( Form_tool_settings->m_cbOTPLock->Checked)
        {
            GC_TRACE("[COM%d] Execute(): Do OTP Lock...", m_sComPortSetting.com.number);
			ret = OTPLock();
        }
        else if( Form_tool_settings->m_cbOTPFormat->Checked)
        {
            GC_TRACE("[COM%d] Execute(): Do OTP Format...", m_sComPortSetting.com.number);
			ret = OTPFormat();
        }
		else if( Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked )
		{
		    GC_TRACE("[COM%d] Execute(): Do Firmware Upgrade and OTP Lock...", m_sComPortSetting.com.number);
			ret = FirmwareUpgradeAndOTPLock();
		}

    }
	else
	{  
	  //Do main operation (Format/Download)
	  if( OP_FORMAT_AND_DOWNLOAD == MainForm->cbType->ItemIndex )
	  {
		  m_SWSec_FormatDL_Done = false;
		  GC_TRACE("[COM%d] Execute(): Do format and download...", m_sComPortSetting.com.number);
		  ret = FormatAndDownload();
	  }
	  else if( OP_FIRMWARE_UPGRADE == MainForm->cbType->ItemIndex )
	  {
		  GC_TRACE("[COM%d] Execute(): Do firmware upgrade...", m_sComPortSetting.com.number);
		  ret = FirmwareUpgrade();
	  }
	  else if( OP_FORMAT_ALL == MainForm->cbType->ItemIndex )
	  {
		  GC_TRACE("[COM%d] Execute(): Do format only...", m_sComPortSetting.com.number);
		  ret = FormatAll();
	  }
	  else if( OP_FORMAT_BOOTLOADER == MainForm->cbType->ItemIndex )
	  {
		  GC_TRACE("[COM%d] Execute(): Do two site download...", m_sComPortSetting.com.number);
		  ret = FormatBootloader();
	  }
	}

    if(S_DONE != ret)
    {
        goto clean;
    }

#if 0
    //Do post operation, such as restore NVRAM data
    GC_TRACE("[COM%d] Do META action after format/download", m_sComPortSetting.com.number);
    ret = DoPostOperation();
    if( 0!= ret )
    {
        goto clean;
    }
#endif


    if( (MT6516_AP != frmMultiLoad->m_chip_type) &&
	  (OP_FORMAT_BOOTLOADER != MainForm->cbType->ItemIndex))
    {
        //Check if USB/UART is unavailable
        ret = CheckPortUnavailable();
        if(-1 == ret){
            goto clean;
        }
    }

    /*if(!MainForm->VerifyHostBufferAfterOP())
    {
        GC_TRACE("[COM%d] Execute(): Memory buffer verification fail!", m_sComPortSetting.com.number);
        ret = S_CHKSUM_ERROR;
        goto clean;
    }*/

clean:

    SyncEnableMainSpecificMsUI();
    SetEvent(Flash_Download_Event[i]);

    if (ret != S_DONE)
    {
        GC_TRACE("[COM%d] Execute(): Update result <<Error>>", m_sComPortSetting.com.number);
        SyncUpdateResultError();
        MainForm->_t_manager.Enter_mpdl_finish(i);
        MainForm->_t_manager.Mark_finish(i);
        Sync_Update_UI();
        OutputFailReport(ret);
    }
    else
    {
        GC_TRACE("[COM%d] Execute(): Update result <<OK>>", m_sComPortSetting.com.number);
        SyncUpdateResultOk();
        MainForm->_t_manager.Enter_mpdl_finish(i);
        MainForm->_t_manager.Mark_finish(i);
        Sync_Update_UI();

        PostMessage(hPostMsgDestHandle, WM_MD_DOWNLOAD_SUCCESSFUL, i, 0);
        GC_TRACE("[COM%d] Execute(): Post successful message...", m_sComPortSetting.com.number);
    }

//	MainForm->_t_manager.Enter_mpdl_finish(i);
//	MainForm->_t_manager.Mark_finish(i);
//	Sync_Update_UI();
    GC_TRACE("[COM%d] Execute(): ================Operation end ==================", m_sComPortSetting.com.number);
    return;
}


//---------------------------------------------------------------------------
int TBOOT1::GetCOMPortIndex()
{
    return m_sDnParam.iCOMIndex;
}

int TBOOT1::GetCOMPortNumber()
{
    return m_sComPortSetting.com.number;
}

int TBOOT1::GetCOMPortNumByComType(const int iComportType)
{
    if(BOOTROM_COMPORT == iComportType)
    {
        return m_sDnParam.iBromComportNum;
    }
    else
    {
        return m_sDnParam.iPreLoaderComportNum;
    }

}

void TBOOT1::UpdateCOMPortNumber(const int iComport)
{
     m_sComPortSetting.com.number = iComport;
}


bool TBOOT1::DownloadImagesByEboot( void )
{
    EBOOT_RESULT ret_e;
    char log[128];

    Android_Boot_ARG             boot_arg;
    Android_Download_ARG         dl_arg;
    string   s_com_symbol;
    time_t   start, finish;


    // Get USB Com port number
    int i = GetCOMPortIndex();
    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);


    // Boot As Download Mode
    boot_arg.m_baud_rate  = MainForm->Get_Baudrate();
    boot_arg.m_p_stopflag = p_stopflag;
    boot_arg.m_p_thread   = this;

    //Region information
    GC_TRACE("[COM%d] DownloadImagesByEboot(): Get region info...", m_sComPortSetting.com.number);
    if( !GetRegionInfo(dl_arg) )
    {
            GC_TRACE("[COM%d] DownloadImagesByEboot(): [ERROR] Get region information fail!", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            return false;
    }

    // Download Image
    GC_TRACE("[COM%d] DownloadImagesByEboot(): Set download parameters for eboot.dll...", m_sComPortSetting.com.number);
    memcpy(&dl_arg.m_image_files,
           &frmMultiLoad->m_image_files,
           ANDROID_IMAGE_FILE_NUMBER*sizeof(Android_Image_File));

    if( IsNeedBoundaryCheck() )
    {
        dl_arg.is_boundary_Check = true;
    }
    else
    {
        dl_arg.is_boundary_Check = false;
    }

    dl_arg.m_p_stopflag                 = p_stopflag;
    dl_arg.m_p_thread                   = this;
    dl_arg.m_p_send_flash_percentage    = Set_send_flash_bin_percentage;
    dl_arg.m_p_send_image_file_status   = set_send_image_file_info;
    dl_arg.is_verify                          = false;


    //Get start time
    time(&start);
    GC_TRACE("[COM%d] DownloadImagesByEboot(): Begin SECOND scan USB COM port...", m_sComPortSetting.com.number);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] DownloadImagesByEboot(): User press stop button!", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            return false;
        }

        if(GetNewPortSymbolicName(s_com_symbol))
        {
            //find usb_com port
            GC_TRACE("[COM%d] DownloadImagesByEboot(): Find USB COM port successfully!(%s)", m_sComPortSetting.com.number, s_com_symbol.c_str());
            break;
        }

        //Get current time
        time( &finish );

        //wait 10s if there still not found a new usb com port
        //then we quite
        if(difftime( finish, start ) > 15 /* 15s */)
        {
            GC_TRACE("[COM%d] DownloadImagesByEboot(): [ERROR] Search USB COM port timeout!!(15s)", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            ret = 9000;    //error status  can't find usb port
            return false;
        }

    }while(1);

    MainForm->_t_manager.Enter_mpdl_finish(i);

    boot_arg.m_p_comsymbol= (char*)s_com_symbol.c_str();



    GC_TRACE("[COM%d] DownloadImagesByEboot(): Calling eboot.dll for other images download...", m_sComPortSetting.com.number);
    ret_e = Android_Flash_Download(boot_arg, &dl_arg);
    if(EBOOT_SUCCESS != ret_e)
    {
        ret = ret_e;
        GC_TRACE("[COM%d] DownloadImagesByEboot(): [ERROR] Download other images fail!", m_sComPortSetting.com.number);
        return false;
    }

    return true;
}



bool TBOOT1::DownloadAndRestoreBinRegionByEboot( void )
{
    EBOOT_RESULT ret_e;
    char log[128];

    Android_Boot_ARG             boot_arg;
    Android_Download_ARG         dl_arg;
    string   s_com_symbol;
    time_t   start, finish;


    // Get USB Com port number
    int i = GetCOMPortIndex();
    int	* p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);


    // Boot As Download Mode
    boot_arg.m_baud_rate  = MainForm->Get_Baudrate();
    boot_arg.m_p_stopflag = p_stopflag;
    boot_arg.m_p_thread   = this;


    //Region information
    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Get region info...", m_sComPortSetting.com.number);
    if( !GetRegionInfo(dl_arg) )
    {
            GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Get region information fail!", m_sComPortSetting.com.number);
            m_asMsg = "ERROR: Get region information fail!";
            MainForm->_t_manager.Enter_mpdl_finish(i);
            return false;
    }

    // Set download parameters for eboot.dll
    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Set download parameters for eboot.dll...", m_sComPortSetting.com.number);
    memcpy(&dl_arg.m_image_files,
           &frmMultiLoad->m_image_files,
           ANDROID_IMAGE_FILE_NUMBER*sizeof(Android_Image_File));

    if( IsNeedBoundaryCheck() )
    {
        dl_arg.is_boundary_Check = true;
    }
    else
    {
        dl_arg.is_boundary_Check = false;
    }

    dl_arg.m_p_stopflag                 = p_stopflag;
    dl_arg.m_p_thread                   = this;
    dl_arg.m_p_send_flash_percentage    = Set_send_flash_bin_percentage;
    dl_arg.m_p_send_image_file_status   = set_send_image_file_info;
    dl_arg.is_verify                          = false;


    //Get start time
    time(&start);
    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Begin SECOND scan USB COM port...", m_sComPortSetting.com.number);
    do
    {
        if(BOOT_STOP == *p_stopflag)
        {
            GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): User press stop button!", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            return false;
        }

        if(GetNewPortSymbolicName(s_com_symbol))
        {
            //find usb_com port
            GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Find USB COM port successfully!(%s)", m_sComPortSetting.com.number, s_com_symbol.c_str());
            break;
        }

        //Get current time
        time( &finish );

        //wait 10s if there still not found a new usb com port
        //then we quite
        if(difftime( finish, start ) > 15 /* 15s */)
        {
            GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Search USB COM port timeout!!(15s)", m_sComPortSetting.com.number);
            MainForm->_t_manager.Enter_mpdl_finish(i);
            ret = 9000;    //error status  can't find usb port
            return false;
        }

    }while(1);

    MainForm->_t_manager.Enter_mpdl_finish(i);

    boot_arg.m_p_comsymbol= (char*)s_com_symbol.c_str();


    //========================================================
    //=================Android advanced download=================
    //========================================================

    ANDROID_ADV_HANDLE_T  dl_handle;
    ANDROID_ADV_HANDLE_T  * p_dl_handle = &dl_handle;


    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Android advanced handle creation...", m_sComPortSetting.com.number);
    if(-1 == Android_ADV_Create(p_dl_handle) )
    {
        GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Android advanced handle creation fail !", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Android advanced handle creation fail !";
        MainForm->_t_manager.Enter_mpdl_finish(i);
        return false;
    }


    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Set advanced download arguments...", m_sComPortSetting.com.number);
    Android_ADV_SetBootArg(dl_handle, boot_arg);


    Android_ADV_Download_ARG adv_dl_arg;
    adv_dl_arg.m_is_verify = dl_arg.is_verify;
    adv_dl_arg.m_p_send_flash_percentage = Set_send_flash_bin_percentage;
    adv_dl_arg.m_p_send_image_file_status = set_send_image_file_info;
    adv_dl_arg.m_region_num = dl_arg.m_region_num;
    adv_dl_arg.sec_arg_result = dl_arg.sec_arg_result;
    adv_dl_arg.patch_result = dl_arg.patch_result;
    adv_dl_arg.m_is_boundary_Check = dl_arg.is_boundary_Check;
    memcpy(adv_dl_arg.m_region_info, dl_arg.m_region_info, MAX_REGION_NUM * sizeof(REGION_INFO_E) );

    Android_ADV_SetDownloadArg(dl_handle, adv_dl_arg);


    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Start connection for advanced download...", m_sComPortSetting.com.number);
    ret_e = Android_ADV_Connect(dl_handle);
    if(EBOOT_SUCCESS != ret_e)
    {
        ret = ret_e;
        GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Android advanced download connection fail !", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Android advanced download connection fail !";
        MainForm->_t_manager.Enter_mpdl_finish(i);
        Android_ADV_Destory(p_dl_handle);
        return false;
    }

    //For BinRegion download
    Android_Image_File binRegion;
    memcpy(&binRegion, &frmMultiLoad->m_frm_bin_region, sizeof(Android_Image_File));
    binRegion.m_path = m_BinRegion.m_path;

    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [BinRegion] Path(%s), Format(%d), Type(%d), Addr(0x%x), Bdy(0x%x), IsD(%d)", m_sComPortSetting.com.number,
        binRegion.m_path,
        binRegion.m_format,
        binRegion.m_type,
        binRegion.addr_off,
        binRegion.addr_boundary,
        binRegion.is_download);

    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Download BinRegion using eboot.dll...", m_sComPortSetting.com.number);
    ret_e = Android_ADV_SendImage(dl_handle, binRegion);
    if(EBOOT_SUCCESS != ret_e)
    {
        ret = ret_e;
        GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Android advanced download images fail !", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Android advanced download images fail !";
        MainForm->_t_manager.Enter_mpdl_finish(i);
        return false;
    }



    //For common download
    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Download common images using eboot.dll...", m_sComPortSetting.com.number);
    for(int i = 0; i < ANDROID_IMAGE_FILE_NUMBER; i++)
    {
        if(NULL != dl_arg.m_image_files[i].m_path)
        {
            ret_e = Android_ADV_SendImage(dl_handle, dl_arg.m_image_files[i]);
            if(EBOOT_SUCCESS != ret_e)
            {
                ret = ret_e;
                GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Android advanced download images fail !", m_sComPortSetting.com.number);
                m_asMsg = "ERROR: Android advanced download images fail !";
                MainForm->_t_manager.Enter_mpdl_finish(i);
                return false;
            }
        }
    }



    GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): Start reboot for advanced download...", m_sComPortSetting.com.number);
    ret_e = Android_ADV_Reboot(dl_handle, dl_arg.reboot_mode);
    if(EBOOT_SUCCESS != ret_e)
    {
        ret = ret_e;
        GC_TRACE("[COM%d] DownloadAndRestoreBinRegionByEboot(): [ERROR] Android advanced download reboot fail !", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: Android advanced download reboot fail !";
        Android_ADV_Destory(p_dl_handle);
        MainForm->_t_manager.Enter_mpdl_finish(i);
        return false;
    }

    Android_ADV_Destory(p_dl_handle);

    return true;
}


bool __fastcall TBOOT1::IsNeedBoundaryCheck()
{
	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
	unsigned int count;

	//if ( !DL_GetCount(dl_handle, &count) )
    if(!FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&count))
	{
		ROM_INFO	rom[MAX_LOAD_SECTIONS];

		//if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
        if(!FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom,count))
		{
			for(unsigned int i = 0; i < count; i++)
			{
				if( 0 == strncmpi(rom[i].name, SCATTER_REGION_HIDDEN, strlen(SCATTER_REGION_HIDDEN)) )
					return true;
			}
			return false;
		}
		else
		{
			GC_TRACE("[COM%d] FlashTool_RomGetInfoAll() error!", m_sComPortSetting.com.number);
			return false;
		}
	}
	else
	{
		GC_TRACE("[COM%d] FlashTool_RomGetCount() error!", m_sComPortSetting.com.number);
		return false;
	}
}

int TBOOT1::GetROMInfo(const char * name, ROM_INFO & romInfo)
{
    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    unsigned int       count;
	
	vector<string> vctDstTemp;
    vctDstTemp.clear();
    InitVectorForFindPartName(name,vctDstTemp);
	
    GC_TRACE("[COM%d] GetROMInfo(): name is %s", m_sComPortSetting.com.number,name);
    //if ( !DL_GetCount(dl_handle, &count) )
    if(!FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&count))
    {
        ROM_INFO	rom[MAX_LOAD_SECTIONS];
        //if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) ){
        if(!FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom,count))
		{
            for(unsigned int i = 0; i < count; i++){
                if(FindPartName(rom[i].name,  vctDstTemp) == _TRUE)
                {
                    memcpy(&romInfo, &rom[i], sizeof(ROM_INFO));
                    GC_TRACE("[COM%d] GetROMInfo(): Get Rom info success! name is %s, rom_type = 0x%08X", m_sComPortSetting.com.number,rom[i].name, rom[i].rom_type);
                    return S_DONE;
                }
			    
               // if( FindStr(name,rom[i].name)){
               //     memcpy(&romInfo, &rom[i], sizeof(ROM_INFO));
               //     GC_TRACE("[COM%d] GetROMInfo(): Get Rom info success! name is %s, rom_type = 0x%08X", m_sComPortSetting.com.number,rom[i].name, rom[i].rom_type);
               //     return S_DONE;
               // }
            }
        }
    }

    return -1;
}

int TBOOT1::GetAllROMInfo(void ** ppArray, unsigned int &size)
{
	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
	ROM_INFO * pArray = NULL;

	//if ( !DL_GetCount(dl_handle, (unsigned short *)&size) )
	if(!FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&size))
	{
		pArray = new struct ROM_INFO[size];
		if(NULL == pArray)
		{
			GC_TRACE("[COM%d] GetAllROMInfo(): [ERROR] Memory allocation fail!", m_sComPortSetting.com.number);
			return -1;
		}

		//if( !DL_Rom_GetInfoAll(dl_handle, pArray, MAX_LOAD_SECTIONS) )
		if(!FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, pArray, size))
		{
			*ppArray = (void *)pArray;
			return S_DONE;
		}
	}

	delete[] pArray;
	return -1;
}

bool __fastcall TBOOT1::GetRegionInfo(Android_Download_ARG & dl_arg)
{
	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
	unsigned int       count;

	//if ( !DL_GetCount(dl_handle, &count) )
	if(!FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&count))
	{
		ROM_INFO	rom[MAX_LOAD_SECTIONS];

		//if( !DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS) )
		if(!FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom, count))
		{
			for(unsigned int i = 0; i < count; i++)
			{
				strncpy(dl_arg.m_region_info[i].region_name, rom[i].name, sizeof(rom[i].name));
				dl_arg.m_region_info[i].start_addr = rom[i].begin_addr;
				dl_arg.m_region_info[i].is_visible = rom[i].item_is_visable;
				dl_arg.m_region_info[i].is_selected = rom[i].enable;


				if(strcmpi(rom[i].name, SCATTER_UBOOT_NAME) == 0 || strcmpi(rom[i].name, SCATTER_LK_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_UBOOT;
				}
				else if(strcmpi(rom[i].name, SCATTER_BOOTIMG_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_BOOTIMG;
				}
				else if(strcmpi(rom[i].name, SCATTER_SECRO_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_SECSTATIC;
				}
				else if(strcmpi(rom[i].name, SCATTER_ANDROID_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_ANDROID;
				}
				else if(strcmpi(rom[i].name, SCATTER_LOGO_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_LOGO;
				}
				else if(strcmpi(rom[i].name, SCATTER_RECOVERY_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_RECOVERY;
				}
				else if(strcmpi(rom[i].name, SCATTER_USERDATA_NAME) == 0)
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_USRDATA;
				}
				else
				{
					dl_arg.m_region_info[i].region_type = DM_IMG_TYPE_UNKNOWN;
				}

			}

			dl_arg.m_region_num = count;

		}
		else
		{
			GC_TRACE("[COM%d] GetRegionInfo(): [ERROR] FlashTool_RomGetInfoAll() error!", m_sComPortSetting.com.number);
			return false;
		}
	}
	else
	{
		GC_TRACE("[COM%d] GetRegionInfo(): [ERROR] FlashTool_RomGetCount() error!", m_sComPortSetting.com.number);
		return false;
	}

	return true;
}

bool TBOOT1::CheckPartitionAddr(const char * name, PART_INFO * pOri, int sizeOri, ROM_INFO * pNew, int sizeNew)
{
    int idxOri = -1;
    int idxNew = -1;
    bool ckRet = false;
	string strTemp;

    for(int i = 0; i < sizeOri; i++)
    {
        strTemp = pOri[i].name;
        if(true == FindStr(name, pOri[i].name) && strTemp.empty() == false)  // support new scatter file , mtk71502,20130408
        {
            idxOri = i;
			break;
        }
    }

    for(int j = 0; j < sizeNew; j++)
    {
        strTemp = pNew[j].name;
        if(true == FindStr(name, pNew[j].name) && strTemp.empty() == false)
        {
            idxNew = j;
			break;
        }
    }

    if ((-1 != idxOri) &&(-1 != idxNew))
    {
        if (pOri[idxOri].begin_addr == pNew[idxNew].begin_addr)
        {
            ckRet = false;
        }
        else
        {
            ckRet = true;
        }

    }
    else if ((-1 == idxOri) &&(-1 != idxNew) || (-1 != idxOri) &&(-1 == idxNew))
    {
        ckRet = true;
    }
    else if ((-1 == idxOri) &&(-1 == idxNew))
    {
        ckRet = false;
    }

    GC_TRACE("[COM%d] CheckPartitionAddr(): Name(%s), ckRet(%d)", m_sComPortSetting.com.number, name, ckRet);
    return ckRet;

}

bool TBOOT1::IsPartAddrChange(string strName, vector<PART_INFO> vctPartInfo, vector<ROM_INFO> vctRomInfo)
{
    bool bRet = false;
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
	    GC_TRACE("[COM%d] IsPartAddrChange(): PartName(%-16s).The part dose not exist in both side! ", m_sComPortSetting.com.number, strName.c_str());
	    bRet = false;
	}
	else if(itPartInfo != vctPartInfo.end() && itRomInfo != vctRomInfo.end()) //part name exist in both side
    { 
        if(itPartInfo->begin_addr == itRomInfo->begin_addr){
			GC_TRACE("[COM%d] IsPartAddrChange(): PartName(%-16s).The begin addr are the same!", m_sComPortSetting.com.number, strName.c_str());
            bRet = false;
		}else{
		    GC_TRACE("[COM%d] IsPartAddrChange(): PartName(%-16s).The begin addr are not the same.", m_sComPortSetting.com.number, strName.c_str());
		    bRet = true;
		}
	}else{  // the part name only exist in one side, not both sides.
	    GC_TRACE("[COM%d] IsPartAddrChange(): PartName(%-16s). Warning! The part only exists in one side!", m_sComPortSetting.com.number, strName.c_str());
	    bRet = true;
	}

	return bRet;
	
	/*int idxOri = -1;
    int idxNew = -1;
    bool ckRet = false;
	string strTemp;

    for(int i = 0; i < sizeOri; i++)
    {
        strTemp = pOri[i].name;
        if(true == FindStr(name, pOri[i].name) && strTemp.empty() == false)  // support new scatter file , mtk71502,20130408
        {
            idxOri = i;
			break;
        }
    }

    for(int j = 0; j < sizeNew; j++)
    {
        strTemp = pNew[j].name;
        if(true == FindStr(name, pNew[j].name) && strTemp.empty() == false)
        {
            idxNew = j;
			break;
        }
    }

    if ((-1 != idxOri) &&(-1 != idxNew))
    {
        if (pOri[idxOri].begin_addr == pNew[idxNew].begin_addr)
        {
            ckRet = false;
        }
        else
        {
            ckRet = true;
        }

    }
    else if ((-1 == idxOri) &&(-1 != idxNew) || (-1 != idxOri) &&(-1 == idxNew))
    {
        ckRet = true;
    }
    else if ((-1 == idxOri) &&(-1 == idxNew))
    {
        ckRet = false;
    }

    GC_TRACE("[COM%d] CheckPartitionAddr(): Name(%s), ckRet(%d)", m_sComPortSetting.com.number, name, ckRet);
    return ckRet;*/

}


//---------------------------------------------------------------------------
static int __stdcall cb_checksum_init(void *usr_arg, const char*image_name)
{

	TBOOT1 *t=(TBOOT1 *)usr_arg;
    AnsiString imageName = (AnsiString)image_name;
	int iLD = imageName.LastDelimiter("\\");
	imageName = imageName.SubString(iLD + 1, imageName.Length());
	GC_TRACE("<TBOOT1> cb_checksum_init(): imageName = %s", imageName);

    t->prefix = "ChkSum(" + imageName + ")";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clFuchsia);

    return 0;
}
//---------------------------------------------------------------------------
static int __stdcall cb_checksum_progress(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void *usr_arg)
{

	TBOOT1 *t=(TBOOT1 *)usr_arg;
    t->SyncUpdateProgressPercentage(finished_percentage);

    return 0;
}

//---------------------------------------------------------------------------
void TBOOT1::InitChecksumArgs(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult)
{
	memset(&checksumArg, 0, sizeof(checksumArg));
	memset(&checksumResult, 0, sizeof(checksumResult));

	checksumArg.m_cb_rom_mem_checksum_init = cb_checksum_init;
	checksumArg.m_cb_rom_mem_checksum = cb_checksum_progress;
	checksumArg.m_cb_rom_mem_checksum_arg = this;
	checksumArg.m_cb_rom_mem_checksum_init_arg = this;
}


int TBOOT1::VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult)
{

	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    CksmVector::iterator it;
    unsigned int count;
    ROM_INFO rom[MAX_LOAD_SECTIONS];
    int ret = S_DONE;
    bool failFlag = false;


    if(frmMultiLoad->m_cksmVt.empty())
    {
        GC_TRACE("<MDLF> VerifyHostBufferCksm(): Cksm vector is empty, no golden value!");
        return -1;
    }

    //DLCALL(DL_GetCount(dl_handle, &count));
    //DLCALL(DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS));
    DLCALL(FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&count));
    DLCALL(FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom, count));

    for( it = frmMultiLoad->m_cksmVt.begin(); (it != frmMultiLoad->m_cksmVt.end()) /*&& (0xFFFFFFFF != (*it).second)*/; it++ )
    {
        unsigned int k = 0;
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

bool TBOOT1::VerifyHostBufferBeforeStart()
{
	if(frmMultiLoad->IsHostBufferCksmFileExisted())
    {
        frmMultiLoad->LoadHostBufferStdCksm();
        RomMemChecksumArg checksumArg;
		RomMemChecksumResult checksumResult;
		InitChecksumArgs(checksumArg, checksumResult);
        if(VerifyHostBufferCksm( checksumArg, checksumResult ))
        {
            Application->MessageBox( "The memory buffer for loading image verification fail!", "FAIL", MB_OK );
            return false;
        }
		return true;
    }
	else
	{
		Application->MessageBox( "Can not find Checksum.ini file! Please use ChecksumGenerateTool to generate the Checksum.ini .", "FAIL", MB_OK );
	}


    return false;
}

FAT_STATUS TBOOT1::IsFATChecked()
{

	int iRet = S_DONE;
	DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
	ROM_INFO	 rom_info[MAX_LOAD_SECTIONS];
	unsigned int rom_count = 0;
	FAT_STATUS fat_status = FAT_NO_FOUND_IN_SCATTER;
	std::vector<string> vctDestTemp;

	//DL_Rom_GetInfoAll(*dl_handle, rom_info, MAX_LOAD_SECTIONS);
	
	iRet = FlashTool_RomGetCount(m_ft_api_handle,dl_handle,&rom_count);
	if(iRet != S_DONE){
		FAT_STATUS fat_status = FAT_NO_FOUND_IN_SCATTER;
		return fat_status;
	}

	iRet = FlashTool_RomGetInfoAll(m_ft_api_handle, dl_handle, rom_info, rom_count);	
	if(iRet != S_DONE){
		FAT_STATUS fat_status = FAT_NO_FOUND_IN_SCATTER;
		return fat_status;
	}
	
    vctDestTemp.clear();
	InitVectorForFindPartName(SCATTER_FAT_NAME,vctDestTemp);
	//DL_GetCount(*dl_handle, &rom_count);
	for(unsigned int i=0; i < rom_count; i++)
	{
		// support new scatter file,.mtk71502,20130408
		//if(FindStr(rom_info[i].name, "FAT")==true)
		if(FindPartName(rom_info[i].name,vctDestTemp) == _TRUE)
		{
		    if(rom_info[i].item_is_visable)
		    {
			  if(_TRUE == rom_info[i].enable)
			  {
				 fat_status = FAT_CHECKED; // FAT is checked.

			   }
			   if(_FALSE == rom_info[i].enable)
			   {
				 fat_status = FAT_UNCHECKED; // FAT is unchecked.
			   }
			   break;

		     }
		     else
		     {
			     fat_status = FAT_UNDOWNLOAD;	  //
			     break;
		     }

		}

	} // End for

	return fat_status;
}

FAT_CHANGED_STATUS TBOOT1::IsFATChanged()
{
    //int status;
    FAT_CHANGED_STATUS fat_changed_status = FAT_NO_CHANGED;
    if(IsPartAddrChange(SCATTER_FAT_NAME, m_vctPartInfo, m_vctRomInfo))
    {
        fat_changed_status=FAT_HAS_CHANGED;
    }
    else
    {
        fat_changed_status=FAT_NO_CHANGED;
    }
	
    return fat_changed_status;
/*   //Get original partition info from target
    PART_INFO * pOri = NULL;
    void ** ppOri = (void **)&pOri;
    unsigned int sizeOri = 0;
    ret = GetAllPartitionInfo(ppOri, sizeOri);

    //Get new partition info from scatter file
    ROM_INFO * pNew = NULL;
    void ** ppNew = (void **)&pNew;
    unsigned int sizeNew = 0;
    ret = GetAllROMInfo(ppNew, sizeNew);

    if(CheckPartitionAddr(SCATTER_FAT_NAME, pOri, sizeOri, pNew, sizeNew))  // modify FAT name to be NO dl ,mtk71502.20130408
    {
        fat_changed_status=FAT_HAS_CHANGED;
    }else
    {
        fat_changed_status=FAT_NO_CHANGED;
    }*/
}

/*

bool TBOOT1::IsHaveProductInfo()
{
    PART_INFO partInfo;
    ret = GetPartitionInfo(SCATTER_PRODUCT_INFO_NAME, partInfo);
    if(S_DONE != ret)
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool TBOOT1::IsHaveProtectf()
{
	  PART_INFO partInfo;
    ret = GetPartitionInfo(SCATTER_PROTECTF_NAME, partInfo);
    if(S_DONE != ret)
	  {
		    return false;
	  }
	  else
	  {
		    return true;
	  }
}

bool TBOOT1::IsHaveProtects()
{
	PART_INFO partInfo;
    ret = GetPartitionInfo(SCATTER_PROTECTS_NAME, partInfo);
    if(S_DONE != ret)
	{
		return false;
	}
	else
	{
		return true;
	}
}*/

/*
Function: make true the pc side and the target side have the same layout of protected images

*/
int TBOOT1::IsHaveProtectRegion()
{
    int ret1=S_DONE;
    int ret2=S_DONE;
	AnsiString asBackupFoloderPath;
	
	// int retCheck = S_DONE;
    // retCheck = CheckBackupFolder(backupPath);

    GC_TRACE("[COM%d] IsHaveProtectRegion(): Enter... ", m_sComPortSetting.com.number);

	GetBackupFolderName(asBackupFoloderPath);

    // std::string backupfolderPath = backupPath.c_str();
    // BackupConfig backupConfig(backupfolderPath);
    BackupConfig backupConfig(asBackupFoloderPath.c_str());
    
    BackupItem backupItem;
    std::list<std::string> backupItemTable;

    ret1 = backupConfig.InitBackupInfoTable();
    backupItemTable = backupConfig.backup_partitions();

    for(std::list<std::string>::iterator it = backupItemTable.begin();it != backupItemTable.end(); ++it)
    {
        backupConfig.QueryBackupItemByPartitionName((*it), &backupItem);
        GC_TRACE("partition_name(%s), file_path(%s), chksum_file(%s),start_addr(0x%I64X), length(0x%X)",
        backupItem.partition_name().c_str(), backupItem.file_path().c_str(),backupItem.chksum_file().c_str());
  	    PART_INFO partInfo;
  	    ROM_INFO  RomInfo;
        ret1 = GetROMInfo(backupItem.partition_name().c_str(), RomInfo);
        ret2 = GetPartitionInfo(backupItem.partition_name().c_str(), partInfo);
        if((S_DONE != ret1 && S_DONE == ret2) || (S_DONE == ret1 && S_DONE != ret2))
        {
  		    GC_TRACE("[COM%d] IsHaveProtectRegion(): [ERROR]Get protect ROM info fail in new partition info ", m_sComPortSetting.com.number);
  			m_asMsg = "ERROR: Get protect ROM info fail!";
            if(ret1 != S_DONE){
                return ret1;
            }
            else{
                return ret2;
            }
        }

        //check which partition shuold be verified
		/*if(ret1 == S_DONE && ret2 == S_DONE)
		{
		    if(FindStr(SCATTER_PROTECTS_NAME,backupItem.partition_name().c_str())){
				m_protected_image.bHasProtect_S = true;
			}else if(FindStr(SCATTER_PROTECTF_NAME,backupItem.partition_name().c_str())){
			    m_protected_image.bHasProtect_F= true;
			}else if(FindStr(SCATTER_PRODUCT_INFO_NAME,backupItem.partition_name().c_str())){
			    m_protected_image.bHasPro_Info= true;
			}else if(FindStr(SCATTER_PRODUCT_INFO_FOR_GPT_NAME,backupItem.partition_name().c_str())){
			    m_protected_image.bHasProInfo_gpt = true;
			}else if(FindStr(SCATTER_PROTECT1_NAME,backupItem.partition_name().c_str())){
			    m_protected_image.bHasProtect1 = true;
			}else if(FindStr(SCATTER_PROTECT2_NAME,backupItem.partition_name().c_str())){
			    m_protected_image.bHasProtect2 = true;
			}
		}*/

    }
    return S_DONE;
}

int TBOOT1::IsPartitionLayoutChanged(int & status)
{
    int ret = S_DONE;
	std::vector<PART_INFO>::iterator itPartInfo;
	std::vector<ROM_INFO>::iterator itRomInfo;

	if(m_vctPartInfo.size() == 0)
	{
	   GC_TRACE("[COM%d] FirmwareUpgrade():Warning! There are no images in the target side!", m_sComPortSetting.com.number);
	   status = PART_NOT_FOUND;
	   return ret;
	}

    GC_TRACE("[COM%d] IsPartitionLayoutChanged(): part count = %d, partition info of target site:", m_sComPortSetting.com.number, m_vctPartInfo.size());
	for(itPartInfo = m_vctPartInfo.begin(); itPartInfo != m_vctPartInfo.end(); itPartInfo++)
	{
	   GC_TRACE("[COM%d] Name(%-16s), Addr(0x%016X), Len(0x%x).", m_sComPortSetting.com.number, itPartInfo->name, itPartInfo->begin_addr,itPartInfo->image_length);
	}

	GC_TRACE("[COM%d] IsPartitionLayoutChanged(): Rom count = %d,  Rom info of PC site:", m_sComPortSetting.com.number, m_vctRomInfo.size());
	for(itRomInfo = m_vctRomInfo.begin(); itRomInfo != m_vctRomInfo.end(); itRomInfo++)
	{
	   GC_TRACE("[COM%d] Name(%-16s), Addr(0x%016X), Len(0x%x).", m_sComPortSetting.com.number, itRomInfo->name, itRomInfo->begin_addr,itRomInfo->partition_size);
	}

	//Check if partition address changed
    if( IsPartAddrChange(SCATTER_PRELOADER_NAME, m_vctPartInfo, m_vctRomInfo)           ||
		IsPartAddrChange(SCATTER_DSPBL_NAME, m_vctPartInfo, m_vctRomInfo)               ||
		IsPartAddrChange(SCATTER_NVRAM_NAME, m_vctPartInfo, m_vctRomInfo)               ||
		IsPartAddrChange(SCATTER_DL_MBR_NAME, m_vctPartInfo, m_vctRomInfo)              ||
		IsPartAddrChange(SCATTER_DL_EBR1_NAME, m_vctPartInfo, m_vctRomInfo)             ||
		IsPartAddrChange(SCATTER_PRODUCT_INFO_NAME, m_vctPartInfo, m_vctRomInfo)        ||
		IsPartAddrChange(SCATTER_PRODUCT_INFO_FOR_GPT_NAME, m_vctPartInfo, m_vctRomInfo)||
		IsPartAddrChange(SCATTER_PROTECTF_NAME, m_vctPartInfo, m_vctRomInfo)            ||
		IsPartAddrChange(SCATTER_PROTECTS_NAME, m_vctPartInfo, m_vctRomInfo)            ||
		IsPartAddrChange(SCATTER_PROTECT1_NAME, m_vctPartInfo, m_vctRomInfo)            ||
		IsPartAddrChange(SCATTER_PROTECT2_NAME, m_vctPartInfo, m_vctRomInfo)            ||
		IsPartAddrChange(SCATTER_PERSIST_NAME, m_vctPartInfo, m_vctRomInfo)            ||
		IsPartAddrChange("KB", m_vctPartInfo, m_vctRomInfo)                             ||           
		IsPartAddrChange("DKB", m_vctPartInfo, m_vctRomInfo)                            ||
		(IsPartAddrChange("BMTPOOL", m_vctPartInfo, m_vctRomInfo)&&(frmMultiLoad->m_chip_type == MT8127)&&(frmMultiLoad->m_Storage_Type == HW_STORAGE_NAND)))
    
    {
        status = PART_HAS_CHANGED;
    }
	else
    {  
        status = PART_NO_CHANGE;
    }
	
	return ret;
	
	/*
    char tempStr[512];

    //For MT6516
    if(frmMultiLoad->m_chip_type == MT6516_AP)
    {
        GC_TRACE("[COM%d] IsPartitionLayoutChanged(): For MT6516 platform, skip partition layout check.",m_sComPortSetting.com.number);
        status = PART_NO_CHANGE;
        return ret;
    }

    GC_TRACE("[COM%d] IsPartitionLayoutChanged(): List partition info...", m_sComPortSetting.com.number);

    //Get original partition info from target
    PART_INFO * pOri = NULL;
    void ** ppOri = (void **)&pOri;
    unsigned int sizeOri = 0;
    ret = GetAllPartitionInfo(ppOri, sizeOri);
    if((S_DONE != ret) ||(NULL == pOri) ||(0 == sizeOri))
    {
        ret = S_DONE;
        status = PART_NOT_FOUND;
        delete[] pOri;
        return ret;
    }

    GC_TRACE("[COM%d] IsPartitionLayoutChanged(): part count = %d, partition info of target site:", m_sComPortSetting.com.number, sizeOri);
    for(unsigned int i = 0; i < sizeOri; i++)
    {
        GC_TRACE("[COM%d]   Name(%-16s), Addr(0x%016X), Len(0x%x).",
            m_sComPortSetting.com.number, pOri[i].name, pOri[i].begin_addr, pOri[i].image_length);
    }

    //Get new partition info from scatter file
    ROM_INFO * pNew = NULL;
    void ** ppNew = (void **)&pNew;
    unsigned int sizeNew = 0;
    ret = GetAllROMInfo(ppNew, sizeNew);
    if((S_DONE != ret) ||(NULL == pNew) ||((unsigned int)0 == sizeNew))
    {
        GC_TRACE("[COM%d] IsPartitionLayoutChanged(): [ERROR] Get  partition info of PC site fail!", m_sComPortSetting.com.number);
        m_asMsg = "ERROR: [Brom] Get new partition info fail!";
        delete[] pOri;
        return ret;
    }

    GC_TRACE("[COM%d] IsPartitionLayoutChanged(): partition info of PC site:", m_sComPortSetting.com.number);
    for(unsigned int i = 0; i < sizeNew; i++)
    {
        sprintf(tempStr, "   Name(%-16s), Addr(0x%016X), len(0x%x)", pNew[i].name, pNew[i].begin_addr, pNew[i].partition_size);
		GC_TRACE("[COM%d]%s",m_sComPortSetting.com.number, tempStr);
    }

    //Check if partition address changed
    if(CheckPartitionAddr(SCATTER_PRELOADER_NAME, pOri, sizeOri, pNew, sizeNew)
    ||CheckPartitionAddr(SCATTER_DSPBL_NAME, pOri, sizeOri, pNew, sizeNew)
    ||CheckPartitionAddr(SCATTER_NVRAM_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_DL_MBR_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_DL_EBR1_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_PRODUCT_INFO_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_PRODUCT_INFO_FOR_GPT_NAME, pOri, sizeOri, pNew, sizeNew)  //for name rule of gpt
		||CheckPartitionAddr(SCATTER_PROTECTF_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_PROTECTS_NAME, pOri, sizeOri, pNew, sizeNew)
		||CheckPartitionAddr(SCATTER_PROTECT1_NAME, pOri, sizeOri, pNew, sizeNew)   //for name rule of gpt
		||CheckPartitionAddr(SCATTER_PROTECT2_NAME, pOri, sizeOri, pNew, sizeNew)  //for name rule of gpt
		||CheckPartitionAddr(SCATTER_PGPT_NAME, pOri, sizeOri, pNew, sizeNew)
		)
    {
        GC_TRACE("[COM%d] IsPartitionLayoutChanged(): [warning] layout change!", m_sComPortSetting.com.number);
        status = PART_HAS_CHANGED;
    }
    else
    {
        status = PART_NO_CHANGE;
    }

    delete[] pOri;
    delete[] pNew;
      return ret;
    */
  
}

bool TBOOT1::GetNewPortNumber( int &usb_com_port )
{
   std::set<int>       current_exist_com_port;
   std::vector<int>    result_port(255);
   std::vector<int>::iterator  it_last_result;

    HANDLE     h_Com;
    char strCom[64];
    char log[128];

   //Get current exist com port
   ScanPortNumberByRegistry(current_exist_com_port);

   it_last_result = set_difference(current_exist_com_port.begin(), current_exist_com_port.end(),
                                   m_exist_com_port.begin(), m_exist_com_port.end(),
                                   result_port.begin());

   if(result_port.begin() == it_last_result)
   {
      return false;
   }

   usb_com_port =  *result_port.begin();

      sprintf(strCom, "\\\\.\\COM%d", usb_com_port);

      h_Com = CreateFile(strCom, GENERIC_READ | GENERIC_WRITE,
			 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

      if(INVALID_HANDLE_VALUE == h_Com)
      {
         sprintf(log, "YuSu: open com port %d ! fail", usb_com_port);
         GC_TRACE(log);
         return false;
      }
   CloseHandle(h_Com);

   return true;
}


void TBOOT1::ScanPortNumberByRegistry(std::set<int>  &exist_com_ports)
{
/*
  HANDLE     h_Com;
  char strCom[64];
  char log[128];

  //GC_TRACE("YuSu: scan_exist_com_port !");

  for(int nCom = 3; nCom<256; nCom++)
  {
      sprintf(strCom, "\\\\.\\COM%d", nCom);

      h_Com = CreateFile(strCom, GENERIC_READ | GENERIC_WRITE,
			 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

      if(INVALID_HANDLE_VALUE == h_Com)
         continue;

      exist_com_ports.insert(nCom);

      GC_TRACE("YuSu: insert com port %d !", nCom);

      CloseHandle(h_Com);
  }

  GC_TRACE("YuSu: scan_exist_com_port over total scan %d !",
           exist_com_ports.size());
*/

    int nCom = 0;
    char log[128];
    HKEY   hKey;
          long   lResult(0);
          int   nValueIndex(0);
          char   szValue[255];
          char   btData[255];

           unsigned   long   dwDataSize(255);
           unsigned   long   dwValueNameSize(255);
           unsigned   long   dwType;
          char   *lpRoot   ="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
//          Eboot_Log("YuSu: scan_exist_com_port !");

          lResult   =   ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,   lpRoot,   0,   KEY_READ,   &hKey);
          if(lResult   !=   ERROR_SUCCESS)
                  return;

                          do
                          {
                                  dwDataSize = 255;
                                  dwValueNameSize = 255;
                                  lResult   =   ::RegEnumValue(hKey,   nValueIndex++,
                                          szValue,   &dwValueNameSize,   0,   &dwType,btData,   &dwDataSize);
                                 if((lResult == ERROR_SUCCESS))
                                  {
                                     AnsiString sdata = btData;
                                     sdata= sdata.SubString(4,dwDataSize-4);
                                     nCom = StrToInt( sdata);
                                     exist_com_ports.insert(nCom);
                           //          sprintf(log, "YuSu: insert com port %d !", nCom);
                           //          Eboot_Log(log);
                                  }

                  }while (lResult == ERROR_SUCCESS );

          ::RegCloseKey(hKey);
//     sprintf(log, "YuSu: scan_exist_com_port over total scan %d !", exist_com_ports.size());
//     Eboot_Log(log);

}

bool TBOOT1::GetNewPortSymbolicName( string &usb_com_port_str )
{


   std::set<string>       current_exist_com_port_str;
//   std::vector<string>    result_port_str;
   std::vector<string>::iterator  it_last_result_str;
   string   result_port_str_test[10];

   //Get current exist com port
   ScanSymbolicNameByDDK(current_exist_com_port_str);

   /*
   it_last_result_str = set_difference(current_exist_com_port_str.begin(), current_exist_com_port_str.end(),
                                   m_exist_com_port_str.begin(), m_exist_com_port_str.end(),
                                   result_port_str.begin());
   */
   it_last_result_str = set_difference(current_exist_com_port_str.begin(), current_exist_com_port_str.end(),
                                   m_exist_com_port_str.begin(), m_exist_com_port_str.end(),
                                   result_port_str_test);

   if(result_port_str_test == it_last_result_str)
   {
      return false;
   }

   usb_com_port_str =  *result_port_str_test;


   return true;
}

void TBOOT1::ScanSymbolicNameByDDK(std::set<string>  &exist_com_ports_str /*out*/)
{


        unsigned long	device_number = 0;
	char			device_path[512];
	unsigned long	device_path_real_length;
	char			port_name[512];
	unsigned long	port_name_real_length;

      device_number =  GetDeviceNumber(InterfaceClassGuidConstant, PID_PROLOADER, VID_ANDROID);
 //       GC_TRACE( "YuSu: there is %d devices on PC", device_number);
        if( device_number == 0)
        {

           return;
        }


	for(unsigned int i=0; i<device_number; i++)
	{
	  if(GetPresentDevicePath(InterfaceClassGuidConstant, PID_PROLOADER, VID_ANDROID, i,
						port_name, 512, (unsigned long *) &port_name_real_length,
						device_path, 512, (unsigned long *) &device_path_real_length))

              {
                exist_com_ports_str.insert(device_path);

 //                 GC_TRACE( "YuSu: insert com port device name: %s ! friendly neme:%s", device_path,friend_name);
              }
              else
              {
                  GC_TRACE("[COM%d] Get_Device_Path() fail", m_sComPortSetting.com.number);
              }
	}
}


bool TBOOT1::GetNewPortFriendlyName( string &usb_com_port_str )
{


   std::set<string>       current_exist_com_port_str;
//   std::vector<string>    result_port_str;
   std::vector<string>::iterator  it_last_result_str;
   string   result_port_str_test[10];

   //Get current exist com port
   ScanFriendlyNameByDDK(current_exist_com_port_str);

   /*
   it_last_result_str = set_difference(current_exist_com_port_str.begin(), current_exist_com_port_str.end(),
                                   m_exist_com_port_str.begin(), m_exist_com_port_str.end(),
                                   result_port_str.begin());
   */
   it_last_result_str = set_difference(current_exist_com_port_str.begin(), current_exist_com_port_str.end(),
                                   m_exist_com_port_str.begin(), m_exist_com_port_str.end(),
                                   result_port_str_test);

   if(result_port_str_test == it_last_result_str)
   {
      return false;
   }

   usb_com_port_str =  *result_port_str_test;


   return true;
}

void TBOOT1::ScanFriendlyNameByDDK(std::set<string>  &exist_com_ports_str /*out*/)
{


        unsigned long	device_number = 0;
	char			device_path[512];
	unsigned long	device_path_real_length;
	char			port_name[512];
	unsigned long	port_name_real_length;

      device_number =  GetDeviceNumber(InterfaceClassGuidConstant, PID_DA_HS, VID_ANDROID);
 //       GC_TRACE( "YuSu: there is %d devices on PC", device_number);
        if( device_number == 0)
        {

           return;
        }


	for(unsigned int i=0; i<device_number; i++)
	{
	  if(GetPresentDevicePath(InterfaceClassGuidConstant, PID_DA_HS, VID_ANDROID, i,
						port_name, 512, (unsigned long *) &port_name_real_length,
						device_path, 512, (unsigned long *) &device_path_real_length))

              {
                exist_com_ports_str.insert(port_name);

 //                 GC_TRACE( "YuSu: insert com port device name: %s ! friendly neme:%s", device_path,friend_name);
              }
              else
              {
                  GC_TRACE("[COM%d] Get_Device_Path() fail", m_sComPortSetting.com.number);
              }
	}
}

//---------------------------------------------------------------------------
void __fastcall TBOOT1::GetErrorMessag(STATUS_E id, unsigned int m_msp_error_code)
{
    AnsiString prefix;

    if( S_SECURITY_ERROR_BEGIN <= id )
    {
       prefix.printf("Error: %s(%d) \n\nMSP Error Code: 0x%02X", StatusToString(id), id, m_msp_error_code);
    }
    else
    {
       prefix.printf("Error: %s(%d) ", StatusToString(id), id);
    }

    m_asMsg = "";

    switch (id)
    {
	// common error code (1000~1999)
        case S_COM_PORT_OPEN_FAIL:
        {
           m_asMsg.printf("[COM] Faile to open COM%d!", m_sComPortSetting.com.number);
        }
        break;

        case S_COM_PORT_SET_TIMEOUT_FAIL:
        {
            m_asMsg.printf("[COM] Fail to set COM%d timeout!", m_sComPortSetting.com.number);
        }
        break;

	    case S_UNKNOWN_TARGET_BBCHIP:
			m_asMsg = "[User] Unsupported target baseband chip type!\n\n[ACTION]\nPlease upgrade to the latest FlashTool then try again.";
			break;
	    case S_UNSUPPORTED_VER_OF_BOOT_ROM:
            m_asMsg = "[User] Unsupported Boot ROM !\n\n[ACTION]\nPlease upgrade to the latest FlashTool then try again.";
            break;
	    case S_UNSUPPORTED_VER_OF_BOOTLOADER:
            m_asMsg = "[MAUI] Boot Loader Image file contains an unsupported version in its header! Please ask for help.";
            break;
	    case S_UNSUPPORTED_VER_OF_DA:
            m_asMsg = "[DA] DA binary file contains an unsupported version in its header! Please ask for help.";
            break;
	    case S_UNSUPPORTED_VER_OF_SEC_INFO:
            m_asMsg = "[MAUI] MAUI binary file contains an unsupported security version! Please ask for help.";
            break;
	    case S_UNSUPPORTED_VER_OF_ROM_INFO:
            m_asMsg = "[MAUI] MAUI binary file contains an unsupported MAUI version! Please ask for help.";
            break;
	    case S_SEC_INFO_NOT_FOUND:
            m_asMsg = "[User] Your ROM files doesn't enable secure!\nSecure-on BB must boot up with secure-on ROM files!\n\n[ACTION]\nPlease use a secure-on ROM file then try again.";
            break;
	    case S_ROM_INFO_NOT_FOUND:
            m_asMsg = "[MAUI] There doesn't exist a header in MAUI file! Please ask for help.";
            break;
	    case S_CUST_PARA_NOT_SUPPORTED:
            m_asMsg = "[User] The phone doesn't contain Customer Parameter Area!\n\nPlease check it again.";
            break;
	    case S_CUST_PARA_WRITE_LEN_INCONSISTENT:
            m_asMsg = "[User] The size of Customer Parameter file is inconsistent with that on the phone!\n\nPlease check it again.";
            break;
//	    case S_SEC_RO_NOT_SUPPORTED:
//	    case S_SEC_RO_WRITE_LEN_INCONSISTENT:
//	    case S_ADDR_N_LEN_NOT_32BITS_ALIGNMENT:
	    case S_UART_CHKSUM_ERROR:
            m_asMsg = "[H/W] Read-back SEC_RO or CUST_PARA and its checksum is mismatched with the checksum BROM DLL calculates after receiving through UART!\n\nPlease check your UART cable is stable or not.";
            break;
//	    case S_NOR_FLASH_BOOT:
//	    case S_NAND_FLASH_BOOT:
	    case S_UNSUPPORTED_VER_OF_EMI_INFO:
            m_asMsg = "[MAUI] MAUI binary file contains an unsupported version of EMI setting! Please ask for help.";
            break;

	// BOOT ROM error code (2000~2999)
//	    case S_BROM_SET_META_REG_FAIL:
//	    case S_BROM_SET_FLASHTOOL_REG_FAIL:
//	    case S_BROM_SET_REMAP_REG_FAIL:
//	    case S_BROM_SET_EMI_FAIL:
	    case S_BROM_DOWNLOAD_DA_FAIL:
			m_asMsg = "[H/W] Fail to download DA to baseband chip's internal SRAM!\n\nPossible reasons:\n1. There's an UART communication problem between FlashTool and BootROM.\n2. Target might be shutdown unexpectedly.\n\n[ACTION]\n1. Try again.\n2. Proceed H/W checking in download cable and target side UART path.\n3. Monitor if power-drop occured in target side.";
			break;
	    case S_BROM_CMD_STARTCMD_FAIL:
			m_asMsg = "[BROM] Can not pass bootrom start command! Possibly target power up too early.";
			break;
	    case S_BROM_CMD_STARTCMD_TIMEOUT:
			m_asMsg = "[BROM] BootROM start command timeout!";
			break;
//	    case S_BROM_CMD_JUMP_FAIL:
//	    case S_BROM_CMD_WRITE16_MEM_FAIL:
//	    case S_BROM_CMD_READ16_MEM_FAIL:
//	    case S_BROM_CMD_WRITE16_REG_FAIL:
//	    case S_BROM_CMD_READ16_REG_FAIL:
//	    case S_BROM_CMD_CHKSUM16_MEM_FAIL:
//	    case S_BROM_CMD_WRITE32_MEM_FAIL:
//	    case S_BROM_CMD_READ32_MEM_FAIL:
//	    case S_BROM_CMD_WRITE32_REG_FAIL:
//	    case S_BROM_CMD_READ32_REG_FAIL:
//	    case S_BROM_CMD_CHKSUM32_MEM_FAIL:
//	    case S_BROM_JUMP_TO_META_MODE_FAIL:
//	    case S_BROM_WR16_RD16_MEM_RESULT_DIFF:
	    case S_BROM_CHKSUM16_MEM_RESULT_DIFF:
			m_asMsg = "[H/W] Fail to download DA to baseband chip's internal SRAM!\n\nThe internal SRAM has the problem that leads to 16 bits checksum error!";
			break;
	    case S_BROM_BBCHIP_HW_VER_INCORRECT:
			m_asMsg = "[H/W] Target baseband chip is inconsistent with bin files!\n\n[ACTION]\nPlease select the suitable bin files for the target's baseband chip.";
			break;
	    case S_BROM_FAIL_TO_GET_BBCHIP_HW_VER:
			m_asMsg = "[H/W] Fail to get target baseband chip H/W version!\n\nPossible reasons:\n1. There's an UART communication problem between FlashTool and BootROM.\n2. Target might be shutdown unexpectedly.\n\n[ACTION]\n1. Try again.\n2. Proceed H/W checking in download cable and target side UART path.\n3. Monitor if power-drop occured in target side.";
			break;
	    case S_BROM_AUTOBAUD_FAIL:
			m_asMsg = "[H/W] BootROM start command passed but fail to multiple BootROM baudrate!\n\nPossible reasons:\n1. There's an UART communication problem between FlashTool and BootROM.\n2. Target might be shutdown unexpectedly.\n\n[ACTION]\n1. Try again.\n2. If the problem still exists. Disable \"Options\"->\"Transmission option\"->\"Speed-Up BootROM baudrate\"\n    in advance mode (CTRL+ALT+V) then try again.\n3. Proceed H/W checking in download cable and target side UART path.\n4. Monitor if power-drop occured in target side.";
			break;
	    case S_BROM_SPEEDUP_BAUDRATE_FAIL:
			m_asMsg = "[H/W] BootROM start command passed but fail to multiple BootROM baudrate!\n\nPossible reasons:\n1. There's an UART communication problem between FlashTool and BootROM.\n2. Target might be shutdown unexpectedly.\n\n[ACTION]\n1. Try again.\n2. If the problem still exists. Disable \"Options\"->\"Transmission option\"->\"Speed-Up BootROM baudrate\"\n    in advance mode (CTRL+ALT+V) then try again.\n3. Proceed H/W checking in download cable and target side UART path.\n4. Monitor if power-drop occured in target side.";
			break;
//	    case S_BROM_LOCK_POWERKEY_FAIL:
//	    case S_BROM_WM_APP_MSG_OUT_OF_RANGE:
//	    case S_BROM_NOT_SUPPORT_MT6205B:
	    case S_BROM_EXCEED_MAX_DATA_BLOCKS:
			m_asMsg = "[MAUI] The number of Multi-Bin files is more than 10!\n\nPlease don't build more than 10 binary files.";
			break;
//	    case S_BROM_EXTERNAL_SRAM_DETECTION_FAIL:
//	    case S_BROM_EXTERNAL_DRAM_DETECTION_FAIL:

	// DA error code (3000~3999)
	    case S_DA_INT_RAM_ERROR:
			m_asMsg = "[H/W] DA didn't detect internal RAM!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
			break;
	    case S_DA_EXT_RAM_ERROR:
			m_asMsg = "[H/W] DA didn't detect external RAM!\n\nPossible reasons:\n1. It maybe a SMT issue.\n2. It maybe a EMI issue.\n\nPlease ask for help.";
			break;
//	    case S_DA_SETUP_DRAM_FAIL:
//	    case S_DA_SETUP_PLL_ERR:
	    case S_DA_DRAM_NOT_SUPPORT:
			m_asMsg = "[H/W] the handset doesn't support DRAM. Please ask for help.";
			break;
	    case S_DA_RAM_FLOARTING:
			m_asMsg = "[H/W] DA detects that DRAM is floating!\n\nPossible reasons:\n1. It maybe a SMT issue.\n2. It maybe a EMI issue.\n\nPlease ask for help.";
			break;
	    case S_DA_RAM_UNACCESSABLE:
            m_asMsg = "[H/W] DA detects that RAM is un-accessable. It maybe a SMT issue. Please ask for help.";
            break;
	    case S_DA_RAM_ERROR:
            m_asMsg = "[H/W] DA detects other access error of RAM! Please ask for help.";
            break;
	    case S_DA_DEVICE_NOT_FOUND:
            m_asMsg = "[H/W] DA didn't detect external SRAM or DRAM on the phone.\n\nPossible reasons:\n1. It maybe a SMT issue.\n2. It maybe a EMI issue.\n3. NOR or NAND hasn't been validated.\n Please check Memory_Validation_List_External.xls\n\nPlease ask for help.";
            break;
	    case S_DA_NOR_UNSUPPORTED_DEV_ID:
			m_asMsg = "[User] Unsupported NOR Flash type!\n\n[ACTION]\nPlease upgrade to the latest FlashTool then try again.";
			break;
	    case S_DA_NAND_UNSUPPORTED_DEV_ID:
			m_asMsg = "[User] Unsupported NAND Flash type!\n\n[ACTION]\nPlease upgrade to the latest FlashTool then try again.";
			break;
	    case S_DA_NOR_FLASH_NOT_FOUND:
            m_asMsg = "[H/W] DA didn't detect NOR Flash on the phone.\n\nPossible reasons:\n1. It maybe a SMT issue.\n2. It maybe a EMI issue.\n3. NOR or NAND hasn't been validated.\n Please check Memory_Validation_List_External.xls\n\nPlease ask for help.";
            break;
	    case S_DA_NAND_FLASH_NOT_FOUND:
            m_asMsg = "[H/W] DA didn't detect NAND Flash on the phone.\n\nPossible reasons:\n1. It maybe a SMT issue.\n2. It maybe a NFI issue.\n3. NOR or NAND hasn't been validated.\n Please check Memory_Validation_List_External.xls\n\nPlease ask for help.";
            break;
	    case S_DA_SOC_CHECK_FAIL:
			m_asMsg = "[User] SOC verification fail!";
			break;
	    case S_DA_NOR_PROGRAM_FAILED:
			m_asMsg = "[H/W] DA fails to program NOR Flash!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
	    case S_DA_NOR_ERASE_FAILED:
			m_asMsg = "[H/W] DA fails to erase NOR Flash!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
	    case S_DA_NAND_PAGE_PROGRAM_FAILED:
			m_asMsg = "[H/W] DA fails to page-program NAND Flash!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
	    case S_DA_NAND_SPARE_PROGRAM_FAILED:
			m_asMsg = "[H/W] DA fails to spare-program NAND Flash!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
//	    case S_DA_NAND_HW_COPYBACK_FAILED:
//	    case S_DA_NAND_ERASE_FAILED:
//	    case S_DA_TIMEOUT:
//	    case S_DA_IN_PROGRESS:
//	    case S_DA_SUPERAND_ONLY_SUPPORT_PAGE_READ:
//	    case S_DA_SUPERAND_PAGE_PRGRAM_NOT_SUPPORT:
//	    case S_DA_SUPERAND_SPARE_PRGRAM_NOT_SUPPORT:
//	    case S_DA_SUPERAND_COPYBACK_NOT_SUPPORT:
//	    case S_DA_NOR_CMD_SEQUENCE_ERR:
//	    case S_DA_NOR_BLOCK_IS_LOCKED:
//	    case S_DA_NAND_BLOCK_IS_LOCKED:
	    case S_DA_NAND_BLOCK_DATA_UNSTABLE:
			m_asMsg = "[H/W] Data of the block is unstable after erasing NAND Flash.!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
	    case S_DA_NOR_BLOCK_DATA_UNSTABLE:
			m_asMsg = "[H/W] Data of the block is unstable after erasing NOR Flash.!\n\nPossible reasons:\n1. It maybe a SMT issue.\n\nPlease ask for help.";
            break;
	    case S_DA_NOR_VPP_RANGE_ERR:
			m_asMsg = "[H/W] Vpp is out of working range for NOR Flash! Please ask for help.";
            break;
//	    case S_DA_INVALID_BEGIN_ADDR:
	    case S_DA_NOR_INVALID_ERASE_BEGIN_ADDR:
			m_asMsg = "[User] An invalid begging address is given for erase operation on NOR Flash.\n\nPlease check your erase address again.";
			break;
	    case S_DA_NOR_INVALID_READ_BEGIN_ADDR:
			m_asMsg = "[User] An invalid begging address is given for read operation on NOR Flash.\n\nPlease check your read address again.";
			break;
	    case S_DA_NOR_INVALID_PROGRAM_BEGIN_ADDR:
			m_asMsg = "[User] An invalid begging address is given for program operation on NOR Flash.\n\nPlease check your program address again.";
			break;
	    case S_DA_INVALID_RANGE:
			m_asMsg = "[User] DA_MEM_CMD command fail!\n\n 1. One of the download blocks has invalid range.\n 2. Possibly invalid begin address or end address exceeds target flash size.";
			break;
	    case S_DA_NOR_PROGRAM_AT_ODD_ADDR:
			m_asMsg = "[User] DA_MEM_CMD command fail!\n\n 1. One of the download blocks has the ODD begin address!";
			break;
	    case S_DA_NOR_PROGRAM_WITH_ODD_LENGTH:
			m_asMsg = "[User] DA_MEM_CMD command fail!\n\n 1. One of the download blocks has the EVEN end address! That is download block is in ODD length.";
			break;
//	    case S_DA_NOR_BUFPGM_NO_SUPPORT:
//	    case S_DA_NAND_UNKNOWN_ERR:
//	    case S_DA_NAND_BAD_BLOCK:
//	    case S_DA_NAND_ECC_1BIT_CORRECT:
//	    case S_DA_NAND_ECC_2BITS_ERR:
//	    case S_DA_NAND_SPARE_CHKSUM_ERR:
//	    case S_DA_NAND_HW_COPYBACK_DATA_INCONSISTENT:
//	    case S_DA_NAND_INVALID_PAGE_INDEX:
//	    case S_DA_NFI_NOT_SUPPORT:
//	    case S_DA_NFI_CS1_NOT_SUPPORT:
//	    case S_DA_NFI_16BITS_IO_NOT_SUPPORT:
	    case S_DA_NFB_BOOTLOADER_NOT_EXIST:
			m_asMsg = "[User] BootLoader doesn't exist on NAND flash! Please download BootLoader first!";
			break;
	    case S_DA_NAND_NO_GOOD_BLOCK:
			m_asMsg = "[H/W] There doesn't exist any good block on NAND Flash!";
			break;
	    case S_DA_BOOTLOADER_IS_TOO_LARGE:
			m_asMsg = "[MAUI] Boot Loader Image is more than 7 blocks on NAND Flash!\n\nPlease ask for help.";
			break;

	    case S_DA_BLANK_FLASH:
			m_asMsg = "[User] There doesn't exist MAUI code and SEC_RO (option) on the flash.\n\nPlease choose MAUI file and SEC_RO file (option) for downloading.";
			break;
	    case S_DA_CODE_AREA_IS_BLANK:
			m_asMsg = "[User] There doesn't exist MAUI code on the flash.\n\nPlease choose MAUI file for downloading.";
			break;
	    case S_DA_SEC_RO_AREA_IS_BLANK:
			m_asMsg = "[User] There doesn't exist SEC_RO (option) on the flash.\n\nPlease choose SEC_RO file (option) for downloading.";
			break;
//	    case S_DA_NOR_OTP_UNLOCKED:
	    case S_DA_UNSUPPORTED_BBCHIP:
   			m_asMsg = "[User] BROM DLL is too old to support this baseband chip on the phone!\n\n[ACTION]\nPlease upgrade to the latest FlashTool then try again.";
			break;
	    case S_DA_FAT_NOT_EXIST:
   			m_asMsg = "[User] FAT doesn't exist on the phone!";
			break;
	    case S_DA_EXT_SRAM_NOT_FOUND:
   			m_asMsg = "[H/W] DA didn't detect external SRAM on the phone! Please ask for help.";
			break;
	    case S_DA_EXT_DRAM_NOT_FOUND:
   			m_asMsg = "[H/W] DA didn't detect external DRAM on the phone! Please ask for help.";
			break;
	    case S_DA_MT_PIN_LOW:
   			m_asMsg = "[H/W] DA detects that a pin is always low on the phone while memory test! Please ask for help.";
			break;
	    case S_DA_MT_PIN_HIGH:
   			m_asMsg = "[H/W] DA detects that a pin is always high on the phone while memory test! Please ask for help.";
			break;
	    case S_DA_MT_PIN_SHORT:
   			m_asMsg = "[H/W] DA detects that two pin is short on the phone while memory test! Please ask for help.";
			break;
	    case S_DA_MT_BUS_ERROR:
   			m_asMsg = "[H/W] DA detects bus error on the phone while memory test! Please ask for help.";
			break;
	    case S_DA_MT_ADDR_NOT_2BYTE_ALIGNMENT:
   			m_asMsg = "[User] Input address is not 2-Byte aligned while memory test!\n\nPlease ask for help.";
			break;
	    case S_DA_MT_ADDR_NOT_4BYTE_ALIGNMENT:
   			m_asMsg = "[User] Input address is not 4-Byte aligned while memory test!\n\nPlease ask for help.";
			break;
	    case S_DA_MT_SIZE_NOT_2BYTE_ALIGNMENT:
   			m_asMsg = "[User] Input size is not 2-Byte aligned while memory test!\n\nPlease ask for help.";
			break;
	    case S_DA_MT_SIZE_NOT_4BYTE_ALIGNMENT:
   			m_asMsg = "[User] Input size is not 4-Byte aligned while memory test!\n\nPlease ask for help.";
			break;
	    case S_DA_MT_DEDICATED_PATTERN_ERROR:
   			m_asMsg = "[H/W] DA detects error on the phone while memory test with dedicated pattern! Please ask for help.";
			break;
	    case S_DA_MT_INC_PATTERN_ERROR:
   			m_asMsg = "[H/W] DA detects error on the phone while memory test with incremental pattern! Please ask for help.";
			break;
	    case S_DA_MT_DEC_PATTERN_ERROR:
   			m_asMsg = "[H/W] DA detects error on the phone while memory test with decremental pattern! Please ask for help.";
			break;
        case S_DA_NFB_BLOCK_0_IS_BAD:
   			m_asMsg = "[H/W] DA detects Block 0 of NAND Flash is bad! Please ask for help.";
			break;

	// FlashTool error code (4000~4999)
//	    case S_FT_CALLBACK_DA_REPORT_FAIL:
	    case S_FT_DA_NO_RESPONSE:
			m_asMsg = "DA didn't send response data to FlashTool!";
			break;
	    case S_FT_DA_SYNC_INCORRECT:
			m_asMsg = "[User] DA sync fail! Possibly download an illegal DA.";
			break;
	    case S_FT_DA_VERSION_INCORRECT:
			m_asMsg = "[User] DA version is incorrect!";
			break;
	    case S_FT_DA_INIT_SYNC_ERROR:
			m_asMsg.printf("Fail to send sync data to DA in initialization stage!");
			break;

	    case S_FT_CHANGE_BAUDRATE_FAIL:
        	{
				unsigned int baudrate_tbl[] = {	0, 921600, 460800, 230400, 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, 110 };
				m_asMsg = "Fail to change baudrate to " + IntToStr(baudrate_tbl[m_sComPortSetting.baudrate]) + ".";
    	    }
    		break;
	    case S_FT_SET_DOWNLOAD_BLOCK_FAIL:
			m_asMsg = "DA_MEM_CMD command fail! Possibly the following reason.\n\n 1. Download block exceeds target flash size.\n 2. Download block start address is not WORD alignment.\n 3. Download block length is not multiple of WORD.";
			break;
	    case S_FT_DOWNLOAD_FAIL:
			m_asMsg = "Download to flash fail! Please press CTRL+ALT+T to turn on debug log then reproduce again!\n C:\\BROM_DLL.log would be created and record more detail information.";
			break;
	    case S_FT_READBACK_FAIL:
			m_asMsg = "Readback from flash fail! Please press CTRL+ALT+T to turn on debug log then reproduce again!\n C:\\BROM_DLL.log would be created and record more detail information.";
			break;
	    case S_FT_FORMAT_FAIL:
			m_asMsg = "Format flash error! Possibly given a wrong format range, please check your format setting.";
			break;
	    case S_FT_FINISH_CMD_FAIL:
			m_asMsg = "DA_FINISH_CMD command fail!";
			Synchronize(ShowMsg);
			break;
//	    case S_FT_ENABLE_WATCHDOG_FAIL:
	    case S_FT_NFB_DOWNLOAD_BOOTLOADER_FAIL:
			m_asMsg.printf("Fail to download BootLoader to NAND flash!");
			break;
	    case S_FT_NFB_DOWNLOAD_CODE_FAIL:
			m_asMsg.printf("Fail to download Image(s) to NAND flash!");
			break;
//	    case S_FT_NFB_INVALID_BOOTLOADER_DRAM_SETTING:
	    case S_FT_NAND_READADDR_NOT_PAGE_ALIGNMENT:
   			m_asMsg = "[User] The begging address of read-back operation is not page-aligned on NAND Flash!\n\nPlease check your read address again.";
			break;
	    case S_FT_NAND_READLEN_NOT_PAGE_ALIGNMENT:
   			m_asMsg = "[User] The length of read-back operation is not page-aligned on NAND Flash!\n\nPlease check your read address again.";
			break;
//	    case S_FT_READ_REG16_FAIL:
//	    case S_FT_WRITE_REG16_FAIL:
//	    case S_FT_CUST_PARA_GET_INFO_FAIL:
//	    case S_FT_CUST_PARA_READ_FAIL:
//	    case S_FT_CUST_PARA_WRITE_FAIL:
//	    case S_FT_INVALID_FTCFG_OPERATION:
//	    case S_FT_INVALID_CUST_PARA_OPERATION:
//	    case S_FT_INVALID_SEC_RO_OPERATION:
//	    case S_FT_INVALID_OTP_OPERATION:
//	    case S_FT_POST_PROCESS_FAIL:
//	    case S_FT_FTCFG_UPDATE_FAIL:
//	    case S_FT_SEC_RO_GET_INFO_FAIL:
//	    case S_FT_SEC_RO_READ_FAIL:
//	    case S_FT_SEC_RO_WRITE_FAIL:
//	    case S_FT_ENABLE_DRAM_FAIL:
//	    case S_FT_FS_FINDFIRSTEX_FAIL:
//	    case S_FT_FS_FINDNEXTEX_FAIL:
//	    case S_FT_FS_FOPEN_FAIL:
//	    case S_FT_FS_GETFILESIZE_FAIL:
//	    case S_FT_FS_READ_FAIL:
	    case S_FT_FS_FILENAME_INVALID:
   			m_asMsg = "[User] The used filename is valid! Please check if your file exists.";
			break;
	    case S_FT_FS_FILENAME_TOO_LONG:
   			m_asMsg = "[User] The length of used filename is too long! Please check your filename is valid.";
			break;
//	    case S_FT_FS_ASSERT:
	    case S_FT_OTP_ADDR_NOT_WORD_ALIGNMENT:
   			m_asMsg = "[User] The address given of OTP operation is not word-aligned! Please check your OTP address again.";
			break;
	    case S_FT_OTP_LENGTH_NOT_WORD_ALIGNMENT:
   			m_asMsg = "[User] The length given of OTP operation is not word-aligned! Please check your OTP length again.";
			break;
	    case S_FT_OTP_INVALID_ADDRESS_RANGE:
   			m_asMsg = "[User] The address given of OTP operation is out of range! Please check your OTP address and length again.";
			break;
//	    case S_FT_NAND_READ_TO_BUFFER_NOT_SUPPORT:
//	    case S_FT_GET_PROJECT_ID_FAIL:
	    case S_FT_ENFB_ROM_FILE_SMALL_THAN_HEADER_DESCRIBE:
   			m_asMsg = "[MAUI] The size of E-NFB Rom file is smaller than that described in header of E-NFB Rom file! Please ask for help.";
			break;
//	    case S_FT_RW_EXTRACT_NFB_FAIL:
//	    case S_FT_MEMORY_TEST_FAIL:

	// FlashTool Handle error code (5000~5999)
	    case S_AUTH_HANDLE_IS_NOT_READY:
   			m_asMsg = "[User] This is a secure Base-band Chip! Please select the authentication file first.";
			break;
	    case S_INVALID_AUTH_FILE:
   			m_asMsg = "[User] This is an invalid authentication file with wrong format! Please ask for help.";
			break;
	    case S_INVALID_DA_FILE:
   			m_asMsg = "[User] This is an invalid DA file with wrong format! Please ask for help.";
			break;
//	    case S_DA_HANDLE_IS_NOT_READY:
//	    case S_FTHND_ILLEGAL_INDEX:
//	    case S_FTHND_HANDLE_BUSY_NOW:
//	    case S_FTHND_FILE_IS_UPDATED:
	    case S_FTHND_FILE_IS_NOT_LOADED_YET:
            m_asMsg = "There is file not loaded yet!!\n\n";
            m_asMsg.cat_printf(" 1. Please check if the DA path is correct.\n\n");
            m_asMsg.cat_printf(" 2. Please check if all the ROM files exist.\n\n");
            m_asMsg.cat_printf(" 3. Please check if the scatter file description is sync with the exist ROM files.\n\n");
			break;
//	    case S_FTHND_FILE_LOAD_FAIL:
//	    case S_FTHND_FILE_UNLOAD_FAIL:
	    case S_FTHND_LIST_IS_EMPTY:
            m_asMsg = "Download or Readback list is empty or all disabled!\n\nPlease select at least one entry.";
			break;
	    case S_DL_SCAT_INCORRECT_FORMAT:
            m_asMsg = "[MAUI] The scatter file has incorrect format! Please ask for help.";
            break;
	    case S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN:
            m_asMsg = "[MAUI] The address of some Load Region in the scatter file is not word-aligned! Please ask for help.";
            break;
	    case S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN:
            m_asMsg = "[MAUI] The offset of some Load Region in the scatter file is not word-aligned! Please ask for help.";
            break;
	    case S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER:
            m_asMsg = "[MAUI] The addresses of all Load Region in the scatter file are not ascending in order! Please ask for help.";
            break;
	    case S_DL_SCAT_JUMPTABLE_IS_NOT_ABSOLUTE_ADDR:
            m_asMsg = "[MAUI] The address of jump table binary file in the scatter file is not absolute address! Please ask for help.";
            break;
	    case S_DL_LOAD_REGION_IS_OVERLAP:
			m_asMsg = "[MAUI] the addresses of some Load Regions in the scatter file are overlapped! Please ask for help.";
			break;
//	    case S_DL_LOAD_REGION_NOT_FOUND:
//	    case S_DL_NOT_RESOURCE_BIN:
//	    case S_DL_MULTIBIN_MECHANISM_DISABLED:
	    case S_DL_RESOURCE_NOT_MATCH_IN_JUMPTABLE:
			m_asMsg = "[DL_HANDLE] a bin file doesn't match any entry in jump table!";
			break;
	    case S_DL_RESOURCE_MUST_DOWNLOAD_WITH_JUMPTABLE:
			m_asMsg = "[User] Resources and jump table are not selected to download together! Please select all resource you wanna download";
			break;
//	    case S_DL_OVERLAP_WITH_EXISTING_RESOURCE:
//	    case S_DL_INVALID_RESOURCE_BIN:
	    case S_DL_JUMPTABLE_INCONSISTENT_WITH_SCAT:
			m_asMsg = "[DL_HANDLE] jump table is different from scatter!";
			break;
	    case S_DL_INVALID_JUMPTABLE:
			m_asMsg = "[MAUI] Jump table is not loaded yet!\nBecause its content is not a correct jump table format.";
			break;
//	    case S_DL_REGION_ADDR_INCONSISTENT_WITH_SCAT:
	    case S_DL_REGION_ADDR_INCONSISTENT_WITH_RESOURCE_ADDR:
			m_asMsg = "[MAUI] The address of resource binary file in scatter file is inconsistent with the address in the header of resource binary file! Please ask for help.";
			break;
	    case S_DL_INVALID_BOOTLOADER:
			m_asMsg = "[MAUI] The Boot Loader Image file has invalid information in its header! Please ask for help.";
			break;
//	    case S_DL_BOOTLOADER_IS_NOT_LOADED_YET:
//	    case S_DL_BOOTLOADER_NOT_FOUND:
	    case S_DL_REMOTE_FILE_UNSUPPORTED_BY_BL_AUTOLOAD:
			m_asMsg = "[User] Boot Loader Image file is in remote server!\n\nPlease use a local Boot Loader Image file in your computer.";
			break;
//	    case S_DLIST_SAME_BBCHIP_SW_VER:
	    case S_DLIST_BBCHIP_HW_VER_NOT_MATCHED:
			m_asMsg = "[H/W] Target baseband chip is inconsistent with bin files!\n\n[ACTION]\nPlease select the suitable bin files for the target's baseband chip.";
			break;
//	    case S_DLIST_NO_MATCHED_DL_HANDLE_FOUND:
//	    case S_DLIST_DL_HANDLE_NOT_IN_LIST:
//	    case S_DLIST_DL_HANDLE_ALREADY_IN_LIST:
//	    case S_FTHND_CALLBACK_REMOTE_GET_FILE_LEN_FAIL:
//	    case S_FTHND_CALLBACK_REMOTE_READ_FILE_FAIL:
//	    case S_FTHND_CALLBACK_FILE_INTEGRITY_CHECK_FAIL:
	    case S_UNSUPPORTED_VER_OF_AUTH_FILE:
			m_asMsg = "[User] The authentication file contains an unsupported version!\n\nPlease check the FlashTool is the latest version, or ask for help.";
			break;
	    case S_DL_PROJECT_ID_DIFF_BETWEEN_MAIN_CODE_AND_JUMP_TBL:
			m_asMsg = "[MAUI] The project IDs between MAUI code and jump table binary file are different!\n\nPlease check the MAUI and jump table are in the same project.";
			break;
	    case S_DL_SCAT_OPEN_FAIL:
			m_asMsg = "[User] The scatter file cannot be opened!\n\nPlease check if your scatter file exists and is not used by other software.";
			break;
//	    case S_FTHND_CALLBACK_COM_INIT_STAGE_FAIL:
        case S_DL_UNSECURE_MAUI_TO_SECURE_BB:
			m_asMsg = "[User] Un-secure MAUI cannot be downloaded into Secure BB!\n\nPlease use security-enabled MAUI.";
            break;

	// security error code (6000~6999)
//	    case S_SECURITY_CALLBACK_SLA_CHALLENGE_FAIL:
	    case S_SECURITY_SLA_WRONG_AUTH_FILE:
			m_asMsg = "[User] The authentication file with a custom name is different from the custom name in the handset!\n\nPlease ask the owner of this handset for an authentication file with a correct custom name.";
			break;
	    case S_SECURITY_SLA_INVALID_AUTH_FILE:
			m_asMsg = "[User] An invalid authentication file is used!\n\nThis error message will occurs with MSP (MTK Secure Platform) code. Please keep up the MSP code and ask for help.";
			break;
//	    case S_SECURITY_SLA_CHALLENGE_FAIL:
//	    case S_SECURITY_SLA_FAIL:
//	    case S_SECURITY_DAA_FAIL:
//	    case S_SECURITY_SBC_FAIL:
	    case S_SECURITY_SF_SECURE_VER_CHECK_FAIL:
			m_asMsg = "[User] The secure version of the secure-MAUI is older than that in the handset!\n\nPlease choose a secure-MAUI with an equal or newer secure version.";
			break;
	    case S_SECURITY_SF_HANDSET_SECURE_CUSTOM_NAME_NOT_MATCH:
			m_asMsg = "[User] The custom name of the secure-MAUI is different from that in the handset!\n\nPlease choose a secure-MAUI with the same custom name.";
			break;
	    case S_SECURITY_SF_FTCFG_LOCKDOWN:
			m_asMsg = "[User] The Tool configuration is locked down!";
			break;
	    case S_SECURITY_SF_CODE_DOWNLOAD_FORBIDDEN:
			m_asMsg = "[User] To download code area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_CODE_READBACK_FORBIDDEN:
			m_asMsg = "[User] To read-back code area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_CODE_FORMAT_FORBIDDEN:
			m_asMsg = "[User] To format code area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_SEC_RO_DOWNLOAD_FORBIDDEN:
			m_asMsg = "[User] To download SEC_RO is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_SEC_RO_READBACK_FORBIDDEN:
			m_asMsg = "[User] To read-back SEC_RO is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_SEC_RO_FORMAT_FORBIDDEN:
			m_asMsg = "[User] To format SEC_RO is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_FAT_DOWNLOAD_FORBIDDEN:
			m_asMsg = "[User] To download FAT area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_FAT_READBACK_FORBIDDEN:
			m_asMsg = "[User] To read-back FAT area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_FAT_FORMAT_FORBIDDEN:
			m_asMsg = "[User] To format FAT area is forbidden in this phone!";
			break;
	    case S_SECURITY_SF_RESTRICTED_AREA_ACCESS_FORBIDDEN:
			m_asMsg = "[User] To access restricted area (SEC_RO area and Flash Tool configuration area in the handset) is forbidden in this phone!";
			break;
	    case S_SECURITY_SECURE_CUSTOM_NAME_NOT_MATCH_BETWEEN_AUTH_AND_DL_HANDLE:
			m_asMsg = "[User] The custom name in the authentication file is different from that in secure-MAUI!\n\nPlease choose the same custom name between authentication file and secure-MAUI!";
			break;
	    case S_SECURITY_DOWNLOAD_FILE_IS_CORRUPTED:
			m_asMsg = "[MAUI] Some information about security in secure-MAUI file is corrupted or broken! Please ask for help.";
			break;
		case S_BROM_EFUSE_ALREADY_BROKEN:
			m_asMsg = "[Efuse] This efuse is writed before,please do not use it again!!!";
			break;
//	    case S_SECURITY_NOT_SUPPORT:

        default:
            break;
    }

    m_asMsg = prefix + "\n\n" + m_asMsg;
}

E_EFUSE_WRITE_TYPE __fastcall TBOOT1::GetWriteEufseType(BBCHIP_TYPE chip_type)
{
   E_EFUSE_WRITE_TYPE ret = WRITE_EEUSE_BY_BROM;
   GC_TRACE("[COM%d] GetWriteEufseType():chip type is %d", m_sComPortSetting.com.number, chip_type);
   switch(chip_type)
   {
     case MT6573:
     case MT6575:
     case MT6577:
     case MT6589:
          ret = WRITE_EEUSE_BY_BROM;
          break;
     case MT6572:
	 case MT6571:
     case MT6582:
     case MT6592:
     case MT8127:
	 case MT6595:
	 case MT6752:
	 case MT8173:
	 case MT6795:
          ret = WRITE_EEUSE_BY_DA;
          break;
     default:
	 	  //if the chip we don't know, we set WRITE_EEUSE_BY_DA.
          ret = WRITE_EFUSE_TYPE_UNKNOWN;
          break;
   }

   if(ret == WRITE_EEUSE_BY_BROM){
      GC_TRACE("[COM%d] GetWriteEufseType(): Use Brom write efuse...", m_sComPortSetting.com.number);
   }
   else if(ret == WRITE_EEUSE_BY_DA){
      GC_TRACE("[COM%d] GetWriteEufseType(): Use DA write efuse...", m_sComPortSetting.com.number);
   }else{
      GC_TRACE("[COM%d] GetWriteEufseType(): [Error] unknow chip type", m_sComPortSetting.com.number);
   }
   
   return ret;
}

int __fastcall TBOOT1::WriteEfuseByDA()
{
    int iRet;
	string strXmlFilePath = MainForm->m_clsEfuseXmlSetting.GetXmlFilePath();
	
	GC_TRACE("[COM%d] WriteEfuseByDA(): Write Efuse by DA begin.", m_sComPortSetting.com.number);
	if(!MainForm->m_clsEfuseXmlSetting.IsXmlParseReady())
	{
	   GC_TRACE("[COM%d] WriteEfuseByDA() Parse XML file is not ready!!!", m_sComPortSetting.com.number);
	   iRet = -1;
	   goto clean;
	}

	GC_TRACE("[COM%d] WriteEfuseByDA(): XML file path=%s", m_sComPortSetting.com.number,strXmlFilePath.c_str());
#define RID_LEN 16
	char strRID[255];
    memset(strRID, 0, 255);
    if(NULL != m_sConnResult.m_da_report.m_random_id)
    {
        ConvertRIDtoString(m_sConnResult.m_da_report.m_random_id, RID_LEN, strRID);
    }
    GC_TRACE("[COM%d] WriteEfuseByDA(): Chip_RID = %s", m_sComPortSetting.com.number,strRID);
	
    //================================Write Efuse======================================
	//dump efuse setting
	GC_TRACE("[COM%d] WriteEfuseByDA(): Dump Efuse data before writing efuse!", m_sComPortSetting.com.number);
	DumpEfuseCommonArg(MainForm->m_clsEfuseXmlSetting.common_arg, m_sComPortSetting.com.number);
	DumpEfuseSecureArg(MainForm->m_clsEfuseXmlSetting.secure_arg, m_sComPortSetting.com.number);
	DumpEfuseLockArg(MainForm->m_clsEfuseXmlSetting.lock_arg, m_sComPortSetting.com.number);
	DumpSTBArg(MainForm->m_clsEfuseXmlSetting.stb_key_arg_, m_sComPortSetting.com.number);
 #if 0
	//Be careful!! Now Writing eufuse
	GC_TRACE("[COM%d] WriteEfuseByDA(): Writing Efuse registers...", m_sComPortSetting.com.number);
    iRet = FlashTool_WriteEfuseAll_Ex(m_ft_api_handle,&MainForm->m_clsEfuseXmlSetting.common_arg, 
		&MainForm->m_clsEfuseXmlSetting.secure_arg, &MainForm->m_clsEfuseXmlSetting.lock_arg, &MainForm->m_clsEfuseXmlSetting.stb_key_arg_, NULL);
	if(iRet != S_DONE)
	{  
	    GC_TRACE("[COM%d] WriteEfuseByDA(): [Error] FlashTool_WriteEfuseAll error!!", m_sComPortSetting.com.number);
	    
		if(S_BROM_EFUSE_ALREADY_BROKEN == iRet)
		   GC_TRACE("[COM%d]FlashTool_WriteEfuseAll(): [Error] Efuse blowing error (already broken) [%s](%d)", m_sComPortSetting.com.number,StatusToString(ret), ret);
		
	    goto clean;
	}

	GC_TRACE("[COM%d] WriteEfuseByDA(): FlashTool_WriteEfuseAll success!!", m_sComPortSetting.com.number);
#endif

    //================================Read Efuse======================================
    iRet = ReadEfuseByDA();
 clean:
    prefix = "Efuse ";
    return iRet;

}

int __fastcall TBOOT1::ReadEfuseByDA()
{
	//readback arg after write efuse
	int iRet;
	GC_TRACE("[COM%d] ReadEfuseByDA(): Read Efuse begin.", m_sComPortSetting.com.number);
    Efuse_Common_Arg common_arg = { 0 };
	Efuse_Secure_Arg secure_arg = { 0 };
	Efuse_Lock_Arg	 lock_arg	= { 0 };
	Efuse_STB_Key_Arg stb_arg   = { 0 };
	Efuse_Extra_Arg   extra_arg = { 0 };
	
	char spare_buf[12] = { 0 };
	char ackey_buf[16] = { 0 };
	char sbcpk_buf[32] = { 0 };
	
	char stbkey_bufs[EFUSE_STK_KEY_NUMBER][32] = {0};
    char stbkey_names[EFUSE_STK_KEY_NUMBER][64] = {0};

    char extra_bufs[END_KEY][64] = {0};
	
	common_arg.spare.buf = spare_buf;
	common_arg.spare.buf_len = sizeof(spare_buf);
	
	secure_arg.ac_key.buf = ackey_buf;
	secure_arg.ac_key.buf_len = sizeof(ackey_buf);
	
	secure_arg.sbc_pub_key_u.r_sbc_pub_key_hash.buf = sbcpk_buf;
	secure_arg.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len = sizeof(sbcpk_buf);
    
    for(int i = 0; i < EFUSE_STK_KEY_NUMBER; i++)
    {
        stb_arg.stb_blow_keys[i].key_name = stbkey_names[i];
        stb_arg.stb_blow_keys[i].stb_key.buf = stbkey_bufs[i];
        stb_arg.stb_blow_keys[i].stb_key.buf_len = sizeof(stbkey_bufs[i]);
    }

	
	for(int i = 0; i < END_KEY; i++)
    {
        extra_arg.items[i].data.key_pair.key.buf = extra_bufs[i];
    }
	
	GC_TRACE("[COM%d] ReadEfuseByDA(): Reading Efuse registers...", m_sComPortSetting.com.number);

    //iRet = FlashTool_ReadEfuseAll(m_ft_api_handle,&common_arg,&secure_arg,&lock_arg);
    iRet = FlashTool_ReadEfuseAll_Ex(m_ft_api_handle,&common_arg,&secure_arg,&lock_arg, &stb_arg, &extra_arg);
	if(iRet != S_DONE)
	{
	    GC_TRACE("[COM%d] ReadEfuseByDA():[Error] FlashTool_ReadEfuseAll fail,ret = 0x%X!!", m_sComPortSetting.com.number, iRet);
		goto clean;
	}
    
    GC_TRACE("[COM%d] ReadEfuseByDA(): Dump Readback efuse value...", m_sComPortSetting.com.number);
	DumpEfuseCommonArg(common_arg, m_sComPortSetting.com.number, true);
	DumpEfuseSecureArg(secure_arg, m_sComPortSetting.com.number, true);
	DumpEfuseLockArg(lock_arg, m_sComPortSetting.com.number);
	DumpSTBArg(stb_arg, m_sComPortSetting.com.number, false);
    GC_TRACE("[COM%d] ReadEfuseByDA(): Read efuse successfully!", m_sComPortSetting.com.number);
	
clean:
	return iRet;
}


int __fastcall TBOOT1::GetSecureFilePath(string &strSecureLockPath, string &strSecureSettingPath, string &strReadbackPath)
{
    AnsiString asPath;
    int iRet = S_DONE;
    if(getPathFromStr(Application->ExeName,asPath)){
       GC_TRACE("[COM%d] GetSecureFilePath(): the tool path is %s", m_sComPortSetting.com.number, asPath);
    }else{
       // If get the tool path fail, we only give the file name to the Efuse API. It will find the file in the current path.
       GC_TRACE("[COM%d] GetSecureFilePath() ERROR: get mdt tool path error, path:%s", m_sComPortSetting.com.number, asPath);
       iRet = -1;
    }
    AnsiString asSettingsFilePath = asPath + "setting.ini";
    GC_TRACE("[COM%d] GetSecureFilePath(): The efuse settings file path is %s", m_sComPortSetting.com.number, asSettingsFilePath.c_str());
    AnsiString asLockFilePath = asPath + "lock.ini";
    GC_TRACE("[COM%d] GetSecureFilePath(): The efuse lock file path is %s", m_sComPortSetting.com.number, asLockFilePath.c_str());
    AnsiString asReadBackFilePath = asPath + "readback.ini";
    GC_TRACE("[COM%d] GetSecureFilePath(): The efuse readback file path is %s", m_sComPortSetting.com.number, asReadBackFilePath.c_str());

    EfuseSettings efuseSettings;
	efuseSettings.ReadEfuseSettings();

    if (0 != efuseSettings.GetEfuseSettingsEnable()){
        strSecureSettingPath = asSettingsFilePath.c_str();
    }else{
        strSecureSettingPath = "";
    }


    if (0 != efuseSettings.GetEfuseLockEnable()){
        strSecureLockPath =  asLockFilePath.c_str();
    }else{
        strSecureLockPath = "";
    }

    if (0 != efuseSettings.GetEfuseReadBackEnable()){
        strReadbackPath =  asReadBackFilePath.c_str();
    }else{
        strReadbackPath = "";
    }
    return iRet;
}


/*

function: Form region from Seccfg begin addr to ReservedRom
History:
1. format parts from begin Addr of SECCFG to RSV
   
*/
int __fastcall TBOOT1::FM_ComboEmmc_Partial(FLASHTOOL_API_HANDLE_T ft_api_handle)
{
    //Create part list for combo emmc format when firmware upgrade
    int iRet = S_DONE;
	std::vector<PART_INFO> vctPartInfo;
	
    iRet = CreateFormatList_For_LayoutNotChange(vctPartInfo);
	if(iRet != S_DONE){
		return iRet;
	}

	vector<PART_INFO>::iterator it = vctPartInfo.begin();
	for (; it != vctPartInfo.end(); ++it)
	{
	    iRet = FM_Part_Item(ft_api_handle,it, NUTL_ADDR_LOGICAL);
	    if(iRet != S_DONE)
	    {
	        GC_TRACE("FM_ComboEmmc_Partial: FormatPartItem fail, ret = %d", iRet);
			break;
	    }
	}
	return iRet;
}

int __fastcall TBOOT1::CreateFormatList_For_LayoutNotChange(vector<PART_INFO> &vctPartInfo)
{
    int iRet = S_DONE;
	std::vector<string> vctDestTemp;
	std::vector<string>::iterator it;

    FAT_STATUS fat_Checked =IsFATChecked();

	FAT_CHANGED_STATUS fat_changed= IsFATChanged();

	GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():Enter...", m_sComPortSetting.com.number);
	if(m_vctPartInfo.empty()){
		GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():Error, m_vctPartInfo is empty!", m_sComPortSetting.com.number);
		return -1;
	}
	
    vector<PART_INFO>::iterator itPartInfo = m_vctPartInfo.begin();
	for (; itPartInfo != m_vctPartInfo.end(); ++itPartInfo)
	{
	    /*
        if( FindStr(SCATTER_PRODUCT_INFO_NAME,itPartInfo->name)   ||
			FindStr(SCATTER_NVRAM_NAME1,itPartInfo->name)   || 
			FindStr(SCATTER_PROTECTF_NAME,itPartInfo->name) ||
			FindStr(SCATTER_PROTECTS_NAME,itPartInfo->name) ||
			FindStr(SCATTER_BMTPOOL_NAME,itPartInfo->name)  ||
			FindStr(SCATTER_PRODUCT_INFO_FOR_GPT_NAME,itPartInfo->name)  ||  //for gpt
			FindStr(SCATTER_PROTECT1_NAME,itPartInfo->name) ||
			FindStr(SCATTER_PROTECT2_NAME,itPartInfo->name)  
			)
        {
	        GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():Not format: %s", m_sComPortSetting.com.number, itPartInfo->name);
	        continue;
        }*/
        vctDestTemp.clear();
	    InitVectorForFindPartName(SCATTER_PRODUCT_INFO_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_NVRAM_NAME1,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PROTECTF_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PROTECTS_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_BMTPOOL_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PRODUCT_INFO_FOR_GPT_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PROTECT1_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PROTECT2_NAME,vctDestTemp);
		InitVectorForFindPartName(SCATTER_PERSIST_NAME,vctDestTemp);
		//add by shanzong
		InitVectorForFindPartName("KB",vctDestTemp);
		InitVectorForFindPartName("DKB",vctDestTemp);
		if(FindPartName(itPartInfo->name,vctDestTemp))
		{
		    GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():Not format: %s", m_sComPortSetting.com.number, itPartInfo->name);
	        continue;
		}
		
		vctDestTemp.clear();
		InitVectorForFindPartName(SCATTER_FAT_NAME,vctDestTemp);
        if(FindPartName(itPartInfo->name,vctDestTemp)){
            switch(fat_Checked)
            {
              case FAT_UNDOWNLOAD:
              case FAT_NO_FOUND_IN_SCATTER:
              case FAT_CHECKED:
			  	 //GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():format: %s", m_sComPortSetting.com.number, itPartInfo->name);
                  break;// do format
				
              case FAT_UNCHECKED:
                 if(fat_changed==FAT_HAS_CHANGED)
                 {
                     GC_TRACE("[COM%d] FirmwareUpgrade(): [ERROR] The layout has been changed, you can not keep FAT data.", m_sComPortSetting.com.number);
					 m_asMsg = "ERROR: The layout has been changed, you can not keep FAT data! ";
                     if(true == m_is_DA_connected){
                         ret = FlashTool_Disconnect(&m_ft_api_handle);
                         m_is_DA_connected = false;
                     }
                     return -1;
                 }else{//if FAT add&length are not changed, and fat download enable is false, we will do not format it.
                     GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():Not format: %s", m_sComPortSetting.com.number, itPartInfo->name);
                     continue;
                 }
              default:
                break;
            }
        }

		

        if(itPartInfo->image_length == 0){
            GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange(): Warming: %s part's image_len is 0!", m_sComPortSetting.com.number, itPartInfo->name);
        }else{
           vctPartInfo.push_back(*itPartInfo);
        }
    }
    GC_TRACE("[COM%d] CreateFormatList_For_LayoutNotChange():OK!", m_sComPortSetting.com.number);
    return iRet;
}

int __fastcall TBOOT1::FM_ComboEmmc_ALL(FLASHTOOL_API_HANDLE_T ft_api_handle)
{
    //Create part list for format all
    int iRet = S_DONE;
	std::vector<PART_INFO> vctPartInfo;
	GC_TRACE("[COM%d] FM_ComboEmmc_ALL(): Enter FM_ComboEmmc_ALL...", m_sComPortSetting.com.number);
    iRet = CreateFormatAllList(vctPartInfo);
	if(iRet != S_DONE){
		return iRet;
	}

	vector<PART_INFO>::iterator it = vctPartInfo.begin();
	for (; it != vctPartInfo.end(); ++it)
	{
	    
	    iRet = FM_Part_Item(ft_api_handle,it, NUTL_ADDR_LOGICAL);
	    if(iRet != S_DONE)
	    {   
	        GC_TRACE("[COM%d] FM_ComboEmmc_ALL(): FormatPartItem fail, ret = %d", m_sComPortSetting.com.number,iRet);
			break;
	    }
	}
	return iRet;
}

int __fastcall TBOOT1::CreateFormatAllList(vector<PART_INFO> &vctPartInfo)
{
    int ret = S_DONE;
	
    PART_INFO format_part;
    memset(&format_part, 0, sizeof(PART_INFO));
	vctPartInfo.clear();
	
    if( m_sConnResult.m_da_report.m_emmc_boot1_size>0)
    {
        GC_TRACE("[COM%d] CreateFormatAllList():  BOOT1...", m_sComPortSetting.com.number);
	    format_part.part_id = EMMC_PART_BOOT1;
		format_part.begin_addr = 0;
	    format_part.image_length = m_sConnResult.m_da_report.m_emmc_boot1_size;
	    vctPartInfo.push_back(format_part);
    }

    if( m_sConnResult.m_da_report.m_emmc_boot2_size>0){
		GC_TRACE("[COM%d] CreateFormatAllList():  BOOT2...", m_sComPortSetting.com.number);
	    format_part.part_id = EMMC_PART_BOOT2;
		format_part.begin_addr = 0;
	    format_part.image_length = m_sConnResult.m_da_report.m_emmc_boot2_size;
	    vctPartInfo.push_back(format_part);
    }

    if( m_sConnResult.m_da_report.m_emmc_rpmb_size>0){
		GC_TRACE("[COM%d] CreateFormatAllList():  RPMB...", m_sComPortSetting.com.number);
		format_part.part_id = EMMC_PART_RPMB;
		format_part.begin_addr = 0;
		format_part.image_length =  m_sConnResult.m_da_report.m_emmc_rpmb_size;
		vctPartInfo.push_back(format_part);
    }

	if( m_sConnResult.m_da_report.m_emmc_ua_size>0){
		GC_TRACE("[COM%d] CreateFormatAllList():  USER...", m_sComPortSetting.com.number);
		format_part.part_id = EMMC_PART_USER;
		format_part.begin_addr = 0;
		format_part.image_length =  m_sConnResult.m_da_report.m_emmc_ua_size;
		vctPartInfo.push_back(format_part);
    }

	return ret;
}

int __fastcall TBOOT1::FM_Part_Item(FLASHTOOL_API_HANDLE_T ft_api_handle,PART_INFO * partinfo, int iEraseFlag)
{
    int ret = S_DONE;
	FlashTool_Format_Arg fmt_arg;
	FlashTool_Format_Result fmt_res;
    //argument
    memset(&fmt_arg, 0, sizeof(fmt_arg));
    memset(&fmt_res, 0, sizeof(fmt_res));
   
   
    fmt_arg.m_storage_type = frmMultiLoad->m_Storage_Type;   //HW_STORAGE_NAND;

    fmt_arg.m_format_cfg.m_format_begin_addr = partinfo->begin_addr;
    fmt_arg.m_format_cfg.m_format_length = partinfo->image_length;
    fmt_arg.m_format_cfg.m_part_id =  partinfo->part_id;
    fmt_arg.m_format_cfg.m_AddrType = iEraseFlag;  //NUTL_ADDR_PHYSICAL or NUTL_ADDR_LOGICAL
    fmt_arg.m_format_cfg.m_auto_format_fat = _FALSE;

    fmt_arg.m_format_cfg.m_validation = m_sDnParam.s_nfb_param.b_validation;
    fmt_arg.m_erase_flag = NUTL_ERASE;
     
    fmt_arg.m_cb_format_report_init =::cb_format_report_init;
    fmt_arg.m_cb_format_report_init_arg = this;
    fmt_arg.m_cb_format_report =::cb_format_report;
    fmt_arg.m_cb_format_report_arg =this;
    fmt_arg.m_cb_format_statistics= ::cb_format_statistics;
    fmt_arg.m_cb_format_statistics_arg = this;

    //format informations
	GC_TRACE("[COM%d] FM_Part_Item(): part name = %s, Format begin Addr = 0x%LLX, Format Length = 0x%LLX", 
		  m_sComPortSetting.com.number, partinfo->name,fmt_arg.m_format_cfg.m_format_begin_addr, fmt_arg.m_format_cfg.m_format_length);
	
    ret = FlashTool_Format(ft_api_handle, &fmt_arg, &fmt_res);
    return ret;
}

int __fastcall TBOOT1::CheckEmmcType(DL_HANDLE_T dl_handle, int &type)
{
    int ret = S_DONE;
  
    if(std::string(frmMultiLoad->m_scatter_info.version) >= "V1.1.2"){
       //combo emmc type
       type = E_COMBO_EMMC;
	   GC_TRACE("[COM%d] CheckEmmcType(): Combo Emmc type", m_sComPortSetting.com.number);
    }else{
       type = E_LINEAR_EMMC;
	   GC_TRACE("[COM%d] CheckEmmcType(): Linear Emmc type", m_sComPortSetting.com.number);
    }

	return ret;
}

int __fastcall TBOOT1::EnterMetaModeAfterDL()
{
    int iRet = S_DONE;
    int index = GetCOMPortIndex();
    SyncUpdateProgressColor(clWhite, clMaroon);
    prefix = "Enter Meta ";
    SyncInitProgress();
    if(MainForm->m_KernelPort[index] == 0)
    {
        GC_TRACE("[COM%d] EnterMetaModeAfterDL(): Kernel comport is 0, need scan!", m_sComPortSetting.com.number);
        EnterCriticalSection(&g_Meta_CS);
        iRet =ForceWatchDogReset();
        if(iRet == S_DONE)
        {
            ret = CheckPortUnavailable();
            if(-1 == ret){
                return -1;
            }
            SyncUpdateProgressPercentage(50);
            iRet = EnterMetaMode();
            if(iRet != S_DONE)
            {
                GC_TRACE("[COM%d] EnterMetaModeAfterDL(): EnterMetaMode fail!", m_sComPortSetting.com.number);
            }

			GC_TRACE("[COM%d] EnterMetaModeAfterDL(): Enter Meta Success!", m_sComPortSetting.com.number);
        }
        else
        {
             GC_TRACE("[COM%d] EnterMetaModeAfterDL(): ForceWatchDogReset fail!", m_sComPortSetting.com.number);
        }
			    
        LeaveCriticalSection(&g_Meta_CS);
    }
    else
    {
        iRet =ForceWatchDogReset();
        if(iRet == S_DONE)
        {
            ret = CheckPortUnavailable();
            if(-1 == ret){
                return -1;
            }
            SyncUpdateProgressPercentage(50);
            iRet = EnterMetaMode();
            if(iRet != S_DONE)
            {
                GC_TRACE("[COM%d] EnterMetaModeAfterDL(): EnterMetaMode fail!", m_sComPortSetting.com.number);
            }

			GC_TRACE("[COM%d] EnterMetaModeAfterDL(): Enter Meta Success!", m_sComPortSetting.com.number);
        }
        else
        {
             GC_TRACE("[COM%d] EnterMetaModeAfterDL(): ForceWatchDogReset fail!", m_sComPortSetting.com.number);
        }
    }
    prefix = "Meta ";
    return iRet;
}

int __fastcall TBOOT1::EnterMetaMode()
{
    MetaProcess cMetapro;
    S_Meta_PARAM_T sMeta_Param;
	int iRet = S_DONE;
	int index = GetCOMPortIndex();
	
    sMeta_Param.iCOMPort = GetCOMPortNumByComType(PRELOADER_COMPORT);
	if(sMeta_Param.iCOMPort == 0)
	{
	    GC_TRACE("[COM%d] EnterMetaMode(): [ERROR] Comport is 0!!", sMeta_Param.iCOMPort);
		return -1;
	}
	
    sMeta_Param.iKernelComPort = MainForm->m_KernelPort[index];
    sMeta_Param.bUsbEnable = true;
    sMeta_Param.bWithoutBattery = false;
	for(int icycle=0; icycle< MAX_SUPPORT_PORT_NUM; icycle++)
		sMeta_Param.KernelComPort[icycle]= MainForm->m_KernelPort[icycle];
	
    cMetapro.InitMetaProcessParam(sMeta_Param);
	
	
    iRet = cMetapro.EnterAPMetaMode();
    if(iRet == S_DONE &&  MainForm->m_KernelPort[index] == 0)
    {
        MainForm->m_KernelPort[index] = cMetapro.GetKernelComport();
    }

	return iRet;
}


