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
 *   MultiPortFlashDownloadTool.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Multiport download form source
 *
 * Author:
 * -------
 *  Andy Ueng (mtk00490)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:   1.12  $
 * $Modtime:   Jun 24 2005 15:13:30  $
 * $Log:   //mtkvs01/vmdata/Multiport_Download/archives/Multiport_Download/MultiPortFlashDownloadTool.cpp-arc  $
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
 * Feb 26 2008 mtk00490
 * [STP100001648] [Multiport download tool] Multiport download tool ver 3.0808.0
 *
 *
 * Dec 3 2007 mtk00490
 * [STP100001572] [Multiport download tool] Multiport download tool ver 3.0748.0
 *
 *
 * Sep 9 2007 mtk00490
 * [STP100001508] [Multiport download tool] Multiport download tool ver 3.2.0
 *
 *
 * Mar 13 2007 mtk00490
 * [STP100001333] [Multiport download tool] Multiport download tool ver 3.1.04
 *
 *
 * Nov 29 2006 mtk00490
 * [STP100001239] [Multiport download tool] Multiport download tool ver 3.1.02
 *
 *
 * Nov 29 2006 mtk00490
 * [STP100001237] [Multiport download tool] Multiport download tool ver 2.9.1006
 *
 *
 * Jun 21 2006 MTK00490
 * [STP100000939] Multiport download Tool_v2.9.1001
 *
 *
 *    Rev 1.12   Jun 24 2005 15:25:08   mtk00490
 * Support maximum 16 port download, format only type, multi-bin option
 *
 *    Rev 1.11   Apr 14 2005 16:08:36   mtk00490
 * add stop button to stop specific or all mobile downloading and formatting
 *
 *    Rev 1.10   Apr 14 2005 16:05:48   mtk00490
 * add stop button to stop specific or all mobile downloading and formatting
 *
 *    Rev 1.9   Feb 18 2005 11:18:38   mtk00490
 * Update brom.dll and download agent ver 2.4.1009 to resolve INTEL W18/W30 series flash doesn't support Buffered-Program
 *
 *    Rev 1.8   Dec 01 2004 10:00:16   mtk00490
 * Multiport download tool ver 2.4.1007 (Add status indicator--download successful:green light, download fail:red light)
 *
 *    Rev 1.7   Nov 05 2004 17:15:32   mtk00490
 * Multiport download tool ver 2.4.1007 (Support automatic baseband chip and external clock rate detection)
 *
 *    Rev 1.6   Sep 08 2004 19:48:50   mtk00490
 * Multiport download tool ver 2.4.1004
 *
 *    Rev 1.4   Aug 28 2004 21:10:28   mtk00490
 * Multiport download ver 2.4.1003
 *
 *    Rev 1.3   Aug 28 2004 20:47:52   mtk00490
 * Multiport download ver 2.4.1002
 *
 *    Rev 1.2   Jul 19 2004 17:27:52   mtk00490
 * Multiport download ver 2.4.0
 *
 *    Rev 1.1   Jun 02 2004 21:15:52   mtk00490
 * Multiport download ver 2.3.0
 * Resolution for 2: Multiport download ver 2.3.0
 *
 *    Rev 1.0   Mar 22 2004 15:13:00   mtk00490
 * Initial revision.
 * Resolution for 1: Multiport download ver 2.1.0
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
//---------------------------------------------------------------------------

#include <vcl.h>
#include <inifiles.hpp>
#include <fcntl.h>
#include <io.h>
#include "com_enum.h"
#include "spmeta.h"
#pragma hdrstop

// form
#ifndef form_AboutH
#include "form_About.h"
#endif

#ifndef ToolSettingsH
#include "ToolSettings.h"
#endif

#ifndef _FORM_MULTILOAD_H_
#include "form_MultiLoad.h"
#endif

#include "Multiport_Common.h"

#ifndef _MD_MSG_
#include "MD_msg.h"
#endif

#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#include "MultiPortFlashDownloadTool.h"
#endif

#ifndef _FLASHDOWNLOAD_H_
#include "FlashDownLoad.h"
#endif

// database
#ifndef _FACTORY_COMMON_H_
#include "factory_common.h"
#endif

#include "Form\SetBeginComPort.h"

#include "INIFileHandle\INIFileHandle.h"

// misc
#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

#include "IdentityVerify.h"
#include "Form_PasswordSetting.h"

#include "Eboot\interface.h"

#include "GCTrace.h"
//#include "formChecksumProgress.h"


#ifndef FORM_CHECKSUM_PROGRESS_H
#include "formChecksumProgress.h"
#endif
//---------------------------------------------------------------------------
typedef enum
{
    MPDT_SB_MSG_IDX = 0,
    MPDT_SB_TOTAL_NUM_IDX,
    MPDT_SB_CURRENT_NUM_IDX
}E_MPDT_SB_INDEX;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TMainForm *MainForm;
struct DL_HANDLE	*g_DL_HANDLE = NULL;
struct DA_HANDLE	*g_DA_HANDLE = NULL;
RB_HANDLE_T          g_RB_HANDLE = NULL;
AUTH_HANDLE_T	        g_AUTH_HANDLE = NULL;
SCERT_HANDLE_T          g_SCERT_HANDLE = NULL;
DL_HANDLE_T             *g_pDL_HANDLE[MAX_SUPPORT_LOAD_NUM+1];
DL_HANDLE_LIST_T        g_DL_HANDLE_LIST;
DL_HANDLE_T *g_DL_HANDLE_SecroReworking = NULL;


extern  HANDLE Flash_Download_Event[MAX_SUPPORT_PORT_NUM];
//extern  int    g_iStopFlag[MAX_SUPPORT_PORT_NUM];


HANDLE hPostMsgDestHandle;
//bool isStopTrigger[MAX_SUPPORT_PORT_NUM];

Login_Mode gLoginMode;

// access
HANDLE hSemaphore;

extern  bool isShowNoFormatInfo;
extern  bool isShowFileNotLoaded;
//extern  int boot_stop;
extern  HANDLE Flash_Download_Event[MAX_SUPPORT_PORT_NUM];

extern  CRITICAL_SECTION  g_Meta_CS;


//---------------------------------------------------------------------------
void __stdcall ErrorHandler(const META_CNF_ERR_CODE err)
{

}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{

    DAPort[0] = -1;
    DAPort[1] = -1;
    DAPort[2] = -1;
    DAPort[3] = -1;
    DAPort[4] = -1;
    DAPort[5] = -1;
    DAPort[6] = -1;
    DAPort[7] = -1;
    DAPort[8] = -1;
    DAPort[9] = -1;
    DAPort[10] = -1;
    DAPort[11] = -1;
    DAPort[12] = -1;
    DAPort[13] = -1;
    DAPort[14] = -1;
    DAPort[15] = -1;

    for(int i = 0; i< MAX_SUPPORT_PORT_NUM; i++)
    {
        m_KernelPort[i] = 0;
    }
	 
    hasProductInfo = false;
    hasProtectf= false;
    hasProtects= false;

    m_stScatFileName[0] = stScatterFile1;

    cbCOMArray[0]  = cbCOM1;
    cbCOMArray[1]  = cbCOM2;
    cbCOMArray[2]  = cbCOM3;
    cbCOMArray[3]  = cbCOM4;
    cbCOMArray[4]  = cbCOM5;
    cbCOMArray[5]  = cbCOM6;
    cbCOMArray[6]  = cbCOM7;
    cbCOMArray[7]  = cbCOM8;
    cbCOMArray[8]  = cbCOM9;
    cbCOMArray[9]  = cbCOM10;
    cbCOMArray[10] = cbCOM11;
    cbCOMArray[11] = cbCOM12;
    cbCOMArray[12] = cbCOM13;
    cbCOMArray[13] = cbCOM14;
    cbCOMArray[14] = cbCOM15;
    cbCOMArray[15] = cbCOM16;

    COMPercentageArray[0]  = COM1Percentage;
    COMPercentageArray[1]  = COM2Percentage;
    COMPercentageArray[2]  = COM3Percentage;
    COMPercentageArray[3]  = COM4Percentage;
    COMPercentageArray[4]  = COM5Percentage;
    COMPercentageArray[5]  = COM6Percentage;
    COMPercentageArray[6]  = COM7Percentage;
    COMPercentageArray[7]  = COM8Percentage;
    COMPercentageArray[8]  = COM9Percentage;
    COMPercentageArray[9]  = COM10Percentage;
    COMPercentageArray[10] = COM11Percentage;
    COMPercentageArray[11] = COM12Percentage;
    COMPercentageArray[12] = COM13Percentage;
    COMPercentageArray[13] = COM14Percentage;
    COMPercentageArray[14] = COM15Percentage;
    COMPercentageArray[15] = COM16Percentage;

    shpCOMArray[0]  = shpCOM1;
    shpCOMArray[1]  = shpCOM2;
    shpCOMArray[2]  = shpCOM3;
    shpCOMArray[3]  = shpCOM4;
    shpCOMArray[4]  = shpCOM5;
    shpCOMArray[5]  = shpCOM6;
    shpCOMArray[6]  = shpCOM7;
    shpCOMArray[7]  = shpCOM8;
    shpCOMArray[8]  = shpCOM9;
    shpCOMArray[9]  = shpCOM10;
    shpCOMArray[10] = shpCOM11;
    shpCOMArray[11] = shpCOM12;
    shpCOMArray[12] = shpCOM13;
    shpCOMArray[13] = shpCOM14;
    shpCOMArray[14] = shpCOM15;
    shpCOMArray[15] = shpCOM16;

   pbCOMArray[0]  = pbCOM1;
   pbCOMArray[1]  = pbCOM2;
   pbCOMArray[2]  = pbCOM3;
   pbCOMArray[3]  = pbCOM4;
   pbCOMArray[4]  = pbCOM5;
   pbCOMArray[5]  = pbCOM6;
   pbCOMArray[6]  = pbCOM7;
   pbCOMArray[7]  = pbCOM8;
   pbCOMArray[8]  = pbCOM9;
   pbCOMArray[9]  = pbCOM10;
   pbCOMArray[10] = pbCOM11;
   pbCOMArray[11] = pbCOM12;
   pbCOMArray[12] = pbCOM13;
   pbCOMArray[13] = pbCOM14;
   pbCOMArray[14] = pbCOM15;
   pbCOMArray[15] = pbCOM16;

   TimerCOMArray[0]  = TimerCOM1;
   TimerCOMArray[1]  = TimerCOM2;
   TimerCOMArray[2]  = TimerCOM3;
   TimerCOMArray[3]  = TimerCOM4;
   TimerCOMArray[4]  = TimerCOM5;
   TimerCOMArray[5]  = TimerCOM6;
   TimerCOMArray[6]  = TimerCOM7;
   TimerCOMArray[7]  = TimerCOM8;
   TimerCOMArray[8]  = TimerCOM9;
   TimerCOMArray[9]  = TimerCOM10;
   TimerCOMArray[10] = TimerCOM11;
   TimerCOMArray[11] = TimerCOM12;
   TimerCOMArray[12] = TimerCOM13;
   TimerCOMArray[13] = TimerCOM14;
   TimerCOMArray[14] = TimerCOM15;
   TimerCOMArray[15] = TimerCOM16;

   btnCOMStartArray[0]  = btnCOM1Start;
   btnCOMStartArray[1]  = btnCOM2Start;
   btnCOMStartArray[2]  = btnCOM3Start;
   btnCOMStartArray[3]  = btnCOM4Start;
   btnCOMStartArray[4]  = btnCOM5Start;
   btnCOMStartArray[5]  = btnCOM6Start;
   btnCOMStartArray[6]  = btnCOM7Start;
   btnCOMStartArray[7]  = btnCOM8Start;
   btnCOMStartArray[8]  = btnCOM9Start;
   btnCOMStartArray[9]  = btnCOM10Start;
   btnCOMStartArray[10] = btnCOM11Start;
   btnCOMStartArray[11] = btnCOM12Start;
   btnCOMStartArray[12] = btnCOM13Start;
   btnCOMStartArray[13] = btnCOM14Start;
   btnCOMStartArray[14] = btnCOM15Start;
   btnCOMStartArray[15] = btnCOM16Start;

   btnCOMStopArray[0]  = btnCOM1Stop;
   btnCOMStopArray[1]  = btnCOM2Stop;
   btnCOMStopArray[2]  = btnCOM3Stop;
   btnCOMStopArray[3]  = btnCOM4Stop;
   btnCOMStopArray[4]  = btnCOM5Stop;
   btnCOMStopArray[5]  = btnCOM6Stop;
   btnCOMStopArray[6]  = btnCOM7Stop;
   btnCOMStopArray[7]  = btnCOM8Stop;
   btnCOMStopArray[8]  = btnCOM9Stop;
   btnCOMStopArray[9]  = btnCOM10Stop;
   btnCOMStopArray[10] = btnCOM11Stop;
   btnCOMStopArray[11] = btnCOM12Stop;
   btnCOMStopArray[12] = btnCOM13Stop;
   btnCOMStopArray[13] = btnCOM14Stop;
   btnCOMStopArray[14] = btnCOM15Stop;
   btnCOMStopArray[15] = btnCOM16Stop;

    m_stBkStatus[0]  = m_stBkStatus0;
    m_stBkStatus[1]  = m_stBkStatus1;
    m_stBkStatus[2]  = m_stBkStatus2;
    m_stBkStatus[3]  = m_stBkStatus3;
    m_stBkStatus[4]  = m_stBkStatus4;
    m_stBkStatus[5]  = m_stBkStatus5;
    m_stBkStatus[6]  = m_stBkStatus6;
    m_stBkStatus[7]  = m_stBkStatus7;
    m_stBkStatus[8]  = m_stBkStatus8;
    m_stBkStatus[9]  = m_stBkStatus9;
    m_stBkStatus[10] = m_stBkStatus10;
    m_stBkStatus[11] = m_stBkStatus11;
    m_stBkStatus[12] = m_stBkStatus12;
    m_stBkStatus[13] = m_stBkStatus13;
    m_stBkStatus[14] = m_stBkStatus14;
    m_stBkStatus[15] = m_stBkStatus15;

    lbCOMTime[0] = lblCOM1Time;
    lbCOMTime[1] = lblCOM2Time;
    lbCOMTime[2] = lblCOM3Time;
    lbCOMTime[3] = lblCOM4Time;
    lbCOMTime[4] = lblCOM5Time;
    lbCOMTime[5] = lblCOM6Time;
    lbCOMTime[6] = lblCOM7Time;
    lbCOMTime[7] = lblCOM8Time;
    lbCOMTime[8] = lblCOM9Time;
    lbCOMTime[9] = lblCOM10Time;
    lbCOMTime[10] = lblCOM11Time;
    lbCOMTime[11] = lblCOM12Time;
    lbCOMTime[12] = lblCOM13Time;
    lbCOMTime[13] = lblCOM14Time;
    lbCOMTime[14] = lblCOM15Time;
    lbCOMTime[15] = lblCOM16Time;

    pScanThread = NULL;
    scanStopFlag = false;
	m_FmPreloader = 0;

}

//---------------------------------------------------------------------------
void _fastcall TMainForm::SubClassWndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
        case WM_MD_DOWNLOAD_SUCCESSFUL:
        {
             GC_TRACE("<WndProc> Receive successful message.");
             if (m_cbEnableAutoPulling->Checked )
             {
                 //Sleep(2000);
                 if((0 != iAutoPollingCount) && (hasPolling >= iAutoPollingCount) )
                 {
	                    GC_TRACE("<WndProc> Auto polling has reached max number(%d), stop polling!", hasPolling);
	                    break;
                 }

                 hasPolling++;
				         GC_TRACE("<WndProc> %dth operation will be started.", hasPolling);

                 Set_DownloadParameter((unsigned char)Message.WParam);
                 m_bStartMetaApp[(int)Message.WParam] = false;
                 btnCOMDownloadClick((int)Message.WParam);
             }
        }
        break;

        case WM_MD_MISC_FOLDER_EMPTY_CHECKING_FAIL:
        {

        }
        break;

        case WM_MD_SELECT_DOWNLOAD_TYPE_FAIL:
        {
            Enable_UIComponent();
        }
        break;

        default:
        {
            this->WndProc(Message);
        }
        break;
    }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
   TIniFile *ini;
   AnsiString as_ComportEnabled;

   hPostMsgDestHandle = this->Handle;
   WindowProc = SubClassWndProc;
   // ENABLE DEBUG
   DA_Create(&g_DA_HANDLE);
   RB_Create(&g_RB_HANDLE);
   AUTH_Create(&g_AUTH_HANDLE);
   SCERT_Create(&g_SCERT_HANDLE);


   pathSettings.setUserDataPath();
   std::string dll_log_path;
   m_DLL_log_path = pathSettings.getPath() + "\\" + pathSettings.getTimestamp();


   CheckDirectory(pathSettings.getPath(), true);
   CheckDirectory(m_DLL_log_path, true);

   dll_log_path = m_DLL_log_path + "\\SP_MDT.log";
   Log.set_log_path(dll_log_path);
   GC_TRACE_ON();

   for (int i=0; i<MAX_SUPPORT_LOAD_NUM; i++)
   {
        g_pDL_HANDLE[i] = new DL_HANDLE_T;
        DL_Create(g_pDL_HANDLE[i]);
   }

   DL_CreateList(&g_DL_HANDLE_LIST);

   //READ INI
   #ifdef SERVICE_CENTER
   m_MdtTitle = (AnsiString) "SmartPhone Multi-port download tool (After-Sale Service Center)";   
   rbAuto->Enabled = false;
   rbManual->Enabled = false;
   #else
   m_MdtTitle = (AnsiString) "SmartPhone Multi-port download tool";
   #endif
   MainForm->Caption = m_MdtTitle;
 
   cbType->Items->Add("Format and Download All");
   cbType->Items->Add("Firmware upgrade");
   cbType->Items->Add("Format All");
   cbType->Items->Add("Format bootloader");



    for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
    {
        m_bStartMetaApp[i] = false;
    }

    // initalization
    isFATFileLoaded = false;

   // multibin
    lvChanged.iChangedIndex = NO_CHANGE;
    m_sDnParam.s_multibin_op.b_check_target_resource_layout    = true;
    m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat = true;
    m_sDnParam.s_multibin_op.b_compare_resource_project_id     = true;

    // EMI
    m_sDnParam.sEMI_NFI_Setting.ui_nand_acccon = 0;
    m_sDnParam.sEMI_NFI_Setting.b_enable_dram_setting = false;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_a = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_b = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_c = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_i_mode = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_i_ext_mode = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_k = 0;
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_l = 0;

    // event for all process finish check
    for (int i=0; i < MAX_SUPPORT_PORT_NUM; i++)
    {
        Flash_Download_Event[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

	 InitializeCriticalSection(&g_Meta_CS);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{

    DeInitMETADll();

    // DESTROY HANDLE
    DA_Unload( g_DA_HANDLE );
    DA_Destroy( &g_DA_HANDLE );

    RB_Destroy(&g_RB_HANDLE);

    AUTH_Unload(g_AUTH_HANDLE);
    AUTH_Destroy(&g_AUTH_HANDLE);

    SCERT_Unload(g_SCERT_HANDLE);
    SCERT_Destroy(&g_SCERT_HANDLE);

//    GC_TRACE_OFF();

    for (int i=0; i<MAX_SUPPORT_LOAD_NUM+1; i++)
    {
        if( NULL != g_pDL_HANDLE[i] )
        {
            DL_Rom_UnloadAll( *g_pDL_HANDLE[i] );
            DL_Destroy(g_pDL_HANDLE[i]);
            delete g_pDL_HANDLE[i];
        }
    }

    DL_DestroyList(&g_DL_HANDLE_LIST);

    // event for all process finish
    for (int i=0; i < MAX_SUPPORT_PORT_NUM; i++)
    {
        CloseHandle(Flash_Download_Event[i]);
    }

    
    // Disable Debug
    Brom_DebugOff();

//    RestartActiveSync(m_wcescomm_path.c_str());
    DeleteCriticalSection(&g_Meta_CS);
   
}

/*
void __fastcall TMainForm::get_sp_cfg()
{
    m_wcescomm_path = INIFileH.read_wcescomm_path(INI_FILE_NAME, Application->ExeName);
}
*/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    AnsiString as_AutoLoadResByScat;
    AnsiString ansiTemp;
	
    gLoginMode = Identity->GetLoginMode();

    MainForm->Left = 13;
    MainForm->Top = 9;

//    get_sp_cfg();
    Display_BROMDllLog();
    Display_METADllLog();
    Display_EbootDllLog();

    ansiTemp = Form_AboutMD->lbToolVersion->Caption;
    GC_TRACE("=====MDT Tool version: %s=====", ansiTemp.c_str());
    //Print driver version
    GetPortsDriverVersion();

   Display_Operation();
   Display_BasebandChip();
   Display_ExternalClock();
  /* Display_CheckECOVersion();*/  // Added by maobin guo,  SP do not support this option now
   Display_CheckTargetResourceLayout();
   Display_AutoLoadResByScat();
   Display_CompareResProjectId();
   Display_Baudrate();
   Display_Type();
   Display_DAFile();
   Display_AuthFile();
   Display_CertFile();
   //Display_ComEnabled();
   Display_COMNoArray();

   Display_AutoPollingEnabled();
   Display_FormatFatRange();

   // access
   Display_DataSource();
   Display_DBLogOption();

   Read_FormatFATBeginAddress();
   Read_FormatFATLength();
   Read_NANDFormatFATBeginAddress();
   Read_NANDFormatFATLength();
   Read_EraseValidation();
   Read_EMI_Setting();
   Read_RunningTest();
   Read_DaResetKey();
   Read_EfuseXmlFile();
   Read_ForceWatchdogReset();
   Read_EnableEnterMetaAfterDlFlag();
   
  
   get_PullDownGPIO17();

   InitMETADll();

   frmMultiLoad->Display_LoadScatterAndBinFile();
   //Read backup and restore option
   Display_BkAndRsOptional();

   
   //Read OTP Option
   Display_OTPOption();
   //For DA download
   Display_DADLOption();

   //For two-site download
   Display_TwoSiteOption();

   //Hide scan button
   //HideScanButton();
   RefreshUIComponent();

   if(gLoginMode == LM_OPERATOR)//for OPERATOR
   {
	   OperatorModeSetting();
   }else if(gLoginMode == LM_ENGINEER){
       EngineerModeSetting();
   }
   else{
       Application->MessageBox("Read login mode fail!.", "Identify fail", MB_OK);
       Application->Terminate();
   }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    int temp_dn_status[MAX_SUPPORT_PORT_NUM];

    for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
    {
        ResetEvent(Flash_Download_Event[i]);
        //temp_dn_status[i] = DownloadStatus[i];
    }

    btnStopAllClick(this);
    for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
    {
        if (_t_manager.Is_running(i)/*DOWNLOAD_STATUS_RUNNING == temp_dn_status[i]*/)
        {
            WaitForSingleObject(Flash_Download_Event[i], 5000);
        }
    }

    // stop all timer
    for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
    {
        TimerCOMArray[i]->Enabled = false;
    }


    // save menu setting
    Save_Operation();
    Save_BasebandChip();
    Save_ExternalClock();
    Save_CheckECOVersion();
    Save_BROMDllLog();
    Save_METADllLog();
    Save_EbootDllLog();
    Save_Baudrate();
    //Save_ComEnabled();
    Save_COMNoArray();
    Save_AutoPollingEnabled();
    Save_Type();
    Save_DAFile();
    Save_ScatterFlag();
    Save_AuthFile();
    Save_CertFile();
    Save_FormatFatRange();
   // Save_LVSetting();

    // access
    Save_DataSource();
    Save_DBLogOption();

    //Backup and restore
    Save_BkAndRsOptional();

	//OTP Lock  add
    Save_OTPOption();
	
    //For DA download
    Save_DADLOption();

    //For two-site download
    Save_TwoSiteOption();

    Save_ScanComportType();

	Save_DaResetKey();

	Save_ForceWatchDogReset();

    //frmMultiLoad->Save_ScatterAndLoad(false);
    frmMultiLoad->Save_ScatterInfo(false);
}

//---------------------------------------------------------------------------
void TMainForm::InitMETADll( void )
{
    // initialize META
    GC_TRACE("<MF> Initialize SP_META_DLL...");

    META_RESULT meta_ret;
    for(int i = 0; i < MAX_SUPPORT_PORT_NUM; i++ )
    {
        if( (meta_ret = SP_META_GetAvailableHandle( &m_iMETA_handle_array[i] )) != META_SUCCESS )
        {
            AnsiString asMsg = "META_GetAvailableHandle(): ERROR: " + AnsiString(SP_META_GetErrorString(meta_ret));
            Application->MessageBox( asMsg.c_str(), "Fail", MB_OK );
            return;
        }

        if( (meta_ret = SP_META_Init_r( m_iMETA_handle_array[i], ErrorHandler )) != META_SUCCESS )
        {
            AnsiString asMsg = "META_Init_r(): ERROR: " + AnsiString(SP_META_GetErrorString(meta_ret));
            Application->MessageBox( asMsg.c_str(), "Fail", MB_OK );
            return;
        }
    }

    GC_TRACE("<MF> Complete SP_META_DLL initialization!");

}

//---------------------------------------------------------------------------
void TMainForm::DeInitMETADll( void )
{
    GC_TRACE("<MF> Finalize SP_META_DLL...");

    META_RESULT meta_ret;
    for(int i = 0; i < MAX_SUPPORT_PORT_NUM; i++ )
    {
        SP_META_Deinit_r( &m_iMETA_handle_array[i] );
    }

    GC_TRACE("<MF> Complete SP_META_DLL finalization!");

}


//---------------------------------------------------------------------------
bool TMainForm::FileOpenDownloadAgentExecute()
{
    if(m_asDAPath.Length()>0)
    {
        dlgOpenDA->InitialDir = m_asDAPath;
    }

    if( dlgOpenDA->Execute() )    // *.bin file
    {
        if ( OpenReadDAFile( dlgOpenDA->FileName ))
        {
            m_asDAFileName = dlgOpenDA->FileName;
            int iLD = m_asDAFileName.LastDelimiter("\\:");

            if ( iLD > 0)
            {
                m_asDAPath = m_asDAFileName.SubString(1, iLD-1 );
            }

            return true;
         }
         else
           return false;
    }
    else
        return false;
}

//--------------------------------------------------------------------------
int TMainForm::UpdateDAInfo( void )
{
    int ret;
    AnsiString as_msg;
    AnsiString as_dapath;
    DA_INFO	da_info;

    // if( MT6208!=m_sDnParam.e_bbchip_type && FTHND_OK!=(ret=DA_IsReady(g_DA_HANDLE, &da_info, _TRUE)) )
    if( S_DONE!=(ret=DA_IsReady(g_DA_HANDLE, &da_info, _TRUE)) ){
        if( S_FTHND_FILE_IS_NOT_LOADED_YET == ret || S_FTHND_FILE_IS_UPDATED == ret ){
            if( S_DONE != (ret=DA_Load(g_DA_HANDLE, da_info.filepath, _TRUE, _FALSE)) ){
                if( S_FTHND_HANDLE_BUSY_NOW == ret ){
                    Application->MessageBox( "Program is busy! Can not reload DA right now.", "Fail", MB_OK );
                }
                else if( S_INVALID_DA_FILE == ret ){
                    as_dapath = da_info.filepath;
                    //as_msg    = " \"" + as_dapath + "\" is not a valid DA format!";
                    as_msg    = "DA path is not valid.";
                    Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
                    return 1;
                }
                else{
                    as_dapath = da_info.filepath;
                    as_msg    = " Load DA fail!";
                    Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
                    return 2;
                }
            }
        }
        else{
            as_dapath = da_info.filepath;
            GC_TRACE("<MF> UpdateDAInfo:Check DA fail, da_path = %s, ret = %d",as_dapath, ret);
            as_msg    = " Check DA fail!";
            Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
            return 3;
        }
    }

    return 0;
}

//---------------------------------------------------------------------------
bool TMainForm::OpenReadDAFile( AnsiString asFileName)
{
    int ret = DA_Load(g_DA_HANDLE, asFileName.c_str(), _TRUE, _FALSE);

    if (ret)
    {
        if (S_FTHND_HANDLE_BUSY_NOW == ret)
        {
            Application->MessageBox("Program is busy! Can not reload DA right now.", "Fail", MB_OK);
        }
        else if (S_INVALID_DA_FILE == ret)
        {
            AnsiString as_msg = " \"" + asFileName + "\" is not a valid DA format!";
            Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
            return 1;
        }
        else
        {
            AnsiString as_msg = " Load DA file " + asFileName + " error ";
            Application->MessageBox( as_msg.c_str(), "Fail", MB_OK );
        }
        return false;
    }

    edit_da_path->Text = asFileName;

//stDAFileName->Caption = asFileName;
    return true;
}

//------------------------------------------------------------------------------
void TMainForm::Set_DownloadParameter(unsigned char index)
{
    //Set COM port setting
    //m_sComPortSetting.com.number       = index + m_sDnParam.i_FirstComNo;
    //m_sComPortSetting.com.number       = iCOMNoArray[index];
	m_sComPortSetting.com.number       = GetComNo(index,BOOTROM_COMPORT);
    m_sComPortSetting.baudrate         = Get_Baudrate();
    m_sComPortSetting.ms_read_timeout  = COM_DEFAULT_TIMEOUT;
    m_sComPortSetting.ms_write_timeout = COM_DEFAULT_TIMEOUT;


    //Set DN params
    m_sDnParam.e_bbchip_type           = Get_BBChipType();
    m_sDnParam.e_ext_clock             = Get_ExternalClockType();

    // For  SP  Only Support Com Port
    m_sDnParam.e_com_type          = UART_COM;
    m_sDnParam.e_Operation            = Get_Operation();
    m_sDnParam.e_format_op            = Get_FormatOption();
    m_sDnParam.s_multibin_op          = Get_MultiBinOption();
    Get_BBChipOption(m_sDnParam.s_bbchip_op);

    m_sDnParam.i_meta_handle          = m_iMETA_handle_array[index];
    m_sDnParam.iCOMIndex         =  index;
    //init Bootrom Comport & Preloader Comport
    m_sDnParam.iBromComportNum        =  GetComNo(index, BOOTROM_COMPORT);
    m_sDnParam.iPreLoaderComportNum   =  GetComNo(index, PRELOADER_COMPORT);
    m_sDnParam.eStorageType     =  frmMultiLoad->m_Storage_Type;//HW_STORAGE_NAND;

   // securiry
    m_sDnParam.s_param = frmMultiLoad->Get_Parameter();
/*
    if(edit_auth_path->Text.IsEmpty())
       {
                frmMultiLoad->m_auth_file.is_download = false;

        }
        else
        {
          //for android verify
                strncpy(frmMultiLoad->auth_path, edit_auth_path->Text.c_str(), 254);
                frmMultiLoad->m_auth_file.m_path = frmMultiLoad->auth_path;
                frmMultiLoad->m_auth_file.m_format = DM_IMG_FORMAT_FAT;
                frmMultiLoad->m_auth_file.m_type = DM_IMG_TYPE_AUTHEN_FILE;
                frmMultiLoad->m_auth_file.addr_off = 0;
                frmMultiLoad->m_auth_file.is_download = true;
        }
*/

    //The folder name of backup and restore
    if(Form_tool_settings->m_rbCOMAsName->Checked)
    {
        m_sDnParam.e_BkRs_folderName = OP_COM_FOLDER_NAME;
    }
    else
    {
        m_sDnParam.e_BkRs_folderName = OP_RID_FOLDER_NAME;
    }

    //set reset  
    m_sDnParam.m_reset_key = m_DaResetKey;

    //The deletion of backup data
    m_sDnParam.b_BkRs_deletion = Form_tool_settings->m_cbBackupDel->Checked;
    m_sDnParam.as_working_dir = GetWorkingDir();
}

bool TMainForm::CheckScatterFile()
{
    if(UNKNOWN_BBCHIP_TYPE == frmMultiLoad->m_chip_type)
    {
        Application->MessageBox( "Please select correct scatter file for operaton!", "FAIL", MB_OK );
        return false;
    }

    return true;
}

#if 0
bool TMainForm::VerifyHostBufferBeforeStart()
{
    if(frmMultiLoad->IsHostBufferCksmFileExisted())
    {
        frmMultiLoad->LoadHostBufferStdCksm();
        RomMemChecksumArg checksumArg;
		RomMemChecksumResult checksumResult;
        if(frmMultiLoad->VerifyHostBufferCksm( checksumArg, checksumResult ))
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
#endif

bool TMainForm::VerifyHostBufferAfterOP()
{
    if(frmMultiLoad->IsHostBufferCksmFileExisted())
    {
       /* if(frmMultiLoad->VerifyHostBufferCksm())
        {
            return false;
        }*/
		return true;
    }
	//Application->MessageBox( "Can not find Checksum.ini file!", "FAIL", MB_OK );

    return false;
}

//==============================================================================
bool TMainForm::DownloadCommonFunction( void )
{
    int ret;

    // flag
    isShowNoFormatInfo = false;
    isShowFileNotLoaded = false;

    if( UpdateDAInfo() )
    {
       return false;
    }

/*
    GC_TRACE("<MF> DownloadCommonFunction(): Set checksum");
    if( SetDAChecksum(m_miEnableDACksm->Checked) )
    {
        GC_TRACE("<MF> DownloadCommonFunction(): Set checksum fail!");
        return false;
    }
*/

    // check multi load and ECO consistency
    int HandleNumber;
    if( 0 != DL_GetHandleNumber(g_DL_HANDLE_LIST, &HandleNumber) )
    {
        sbMPDT->Panels->Items[0]->Text = "  [DL_HANDLE] Get handle number fail";
        Application->MessageBox( "[DL_HANDLE] Get handle number fail", "FAIL", MB_OK );
        return false;
    }

    if( (HandleNumber >= 2) && (! miCheckECOversion->Checked) )
    {
        sbMPDT->Panels->Items[0]->Text = "  Please enable ECO version check for multiload download";
        Application->MessageBox( "Please enable ECO version check for multiload download", "FAIL", MB_OK );
        return false;
    }
    return true;
}

int TMainForm::SetDAChecksum(bool enable)
{
		for( int i = 0; i < MAX_SUPPORT_LOAD_NUM; i++ )
		{
			GC_TRACE("<MF> SetDAChecksum(): DL handle(0x%x), Checksum enable(%d)", *g_pDL_HANDLE[i], enable);
			if(DL_SetChecksumEnable(*g_pDL_HANDLE[i], enable))
			{
				GC_TRACE("<MF> SetDAChecksum(): Set fail!");
				return -1;
			}
		}
    return 0;
}

//---------------------------------------------------------------------------
UART_BAUDRATE TMainForm::Get_Baudrate( AnsiString as_baudrate )
{
     UART_BAUDRATE baud_rate;

     if ( m_asBaudrate.AnsiCompareIC("115200") == 0 )
     {
          baud_rate =  UART_BAUD_115200;
     }
     else if ( m_asBaudrate.AnsiCompareIC("230400") == 0 )
     {
          baud_rate =  UART_BAUD_230400;
     }
     else if( m_asBaudrate.AnsiCompareIC("460800") == 0 )
     {
          baud_rate =  UART_BAUD_460800;
     }
     else if( m_asBaudrate.AnsiCompareIC("921600") == 0 )
     {
          baud_rate =  UART_BAUD_921600;
     }
     else // default set to 460800
     {
         Application->MessageBox( "Baud Rate setting is wrong. Set Default 460800", "Notice", MB_OK );
         baud_rate =  UART_BAUD_460800;
     }
     return baud_rate;
}

//----------------------------------------------------------------------------
UART_BAUDRATE TMainForm::Get_Baudrate(void)
{
    UART_BAUDRATE baud_rate;
    switch ((E_BAUD_RATE_INDEX_T) cbBaudrate->ItemIndex)
    {
        case BAUD_RATE_115200_INDEX:
        {
            baud_rate = UART_BAUD_115200;
        }
        break;

        case BAUD_RATE_230400_INDEX:
        {
            baud_rate = UART_BAUD_230400;
        }
        break;

        case BAUD_RATE_460800_INDEX:
        {
            baud_rate = UART_BAUD_460800;
        }
        break;

        case BAUD_RATE_921600_INDEX:
        default:
        {
            baud_rate =  UART_BAUD_921600;
        }
        break;
     }

     return baud_rate;

}

//============================================================================
void TMainForm::Disable_UIComponent( void )
{
     Disable_FileMenu();
     Disable_FactoryDatabaseMenu();
     Disable_OptionMenu();
     Disable_DebugLoggingMenu();
     Disable_FormatFatRangeRadioButton();
     Disable_BaudrateComboBox();
     Disable_TypeComboBox();
     Disable_SetAndDelScatterFile();

     DisableDADLComponent();
     DisableTwoSiteComponent();
     Disable_AutoPulling();
     Disable_ScanBtn();
}

//----------------------------------------------------------------------------
void TMainForm::Enable_UIComponent( void )
{
    if(gLoginMode == LM_ENGINEER)//for engineer
    {
        Enable_FileMenu();
        Enable_FactoryDatabaseMenu();
        Enable_OptionMenu();
        Enable_DebugLoggingMenu();
        //Enable_FormatFatRangeRadioButton();
        Enable_BaudrateComboBox();
 #ifndef SERVICE_CENTER
    	if(Form_tool_settings->m_cbOTPLock->Checked||Form_tool_settings->m_cbOTPFormat->Checked
            ||Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked)
    	{
    		Disable_TypeComboBox();
    	}	
        else
        {
            Enable_TypeComboBox();
        }
 #else
        Disable_TypeComboBox();
 #endif
        Enable_SetAndDelScatterFile();
    
        EnableDADLComponent();
        //EnableTwoSiteComponent();
        Enable_AutoPulling();
		frmMultiLoad->ForceDLWithBattery();
    }
    Enable_ScanBtn();
}

void TMainForm::Reset_DADownloadAll( void )
{
    m_miEnableDADL->Checked = false;
    DisableDADLComponent();
}

void TMainForm::Reset_DAChecksum( void )
{
    m_miEnableDACksm->Checked = false;
    SetDAChecksum(m_miEnableDACksm->Checked);
}

void TMainForm::Reset_UIComponent( void )
{
    Reset_DADownloadAll();
    Reset_DAChecksum();
}


void TMainForm::RefreshUIComponent(void)
{
    E_CHIP_GROUP eChipGroup;
    eChipGroup = CheckChipGroup(frmMultiLoad->m_chip_type);
    //For MT6516 case
    if(BBCHIP_GROUP0 == eChipGroup)
    {
        m_miEnableDADL->Enabled = false;
        m_miEnableDACksm->Enabled = false;
        DisableDADLComponent();
    }
    //For MT6573 case
    else if(BBCHIP_GROUP1 == eChipGroup)
    {
        m_miEnableDACksm->Enabled = true;

        if(true == m_miEnableDADL->Checked)
        {
            EnableDADLComponent();
        }
        else
        {
            DisableDADLComponent();
        }
    }
	  //For MT6575 case
     else if(BBCHIP_GROUP2 == eChipGroup)
    {
        m_miEnableDACksm->Enabled = true;

        if(true == m_miEnableDADL->Checked)
        {
            //EnableDADLComponent();
            m_miDADLFullSpeed->Enabled = true;
            m_miDADLHighSpeed->Enabled = true;
	     //m_miDADLBattery->Enabled = false;
            //m_miDADLNoBattery->Enabled = false;
        }
        else
        {
            DisableDADLComponent();
        }
    }

    //For all case
	  Enable_AutoPulling();
    /*
    if(OP_TWOSITE_DOWNLOAD == MainForm->cbType->ItemIndex)
    {
        EnableTwoSiteComponent();
    }
    else
    {
        DisableTwoSiteComponent();
    }
    */
}

void TMainForm::Disable_SetAndDelScatterFile( void )
{
      ScatterFileButton1->Enabled = false;
      btDeleteScatterFile1->Enabled = false;
}


void TMainForm::Enable_SetAndDelScatterFile( void )
{
      ScatterFileButton1->Enabled = true;
      btDeleteScatterFile1->Enabled = true;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_FileMenu( void )
{
    miFile->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_FileMenu( void )
{
    miFile->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_FactoryDatabaseMenu( void )
{
    miFactoryDatabase->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_FactoryDatabaseMenu( void )
{
    miFactoryDatabase->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_OptionMenu( void )
{
    miOption->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_OptionMenu( void )
{
    miOption->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_DebugLoggingMenu( void )
{
    miDebuglog->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_DebugLoggingMenu( void )
{
    miDebuglog->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_FormatFatRangeRadioButton( void )
{
     rbManual->Enabled = false;
     rbAuto->Enabled = false;

}

//----------------------------------------------------------------------------
void TMainForm::Enable_FormatFatRangeRadioButton( void )
{
     rbManual->Enabled = true;
     rbAuto->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_BaudrateComboBox( void )
{
    cbBaudrate->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_BaudrateComboBox( void )
{
    cbBaudrate->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_TypeComboBox( void )
{
     cbType->Enabled = false;
}

//----------------------------------------------------------------------------
void TMainForm::Enable_TypeComboBox( void )
{
     cbType->Enabled = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_SeperateDownloadButton( void )
{

     /*for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
     {
         btnCOMStartArray[i]->Enabled = false;
     }  */
}

//----------------------------------------------------------------------------
void TMainForm::Enable_SeperateDownloadButton( void )
{
   /*  for (int i=0; i<MAX_SUPPORT_PORT_NUM; i++)
     {
         btnCOMStartArray[i]->Enabled = true;
     }     */
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_AllDownloadButton( void )
{
     Disable_SeperateDownloadButton();
}

//----------------------------------------------------------------------------
void TMainForm::Enable_AllDownloadButton( void )
{
     Enable_SeperateDownloadButton();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Disable_SpecificMsUI(int index)
{
    cbCOMArray[index]->Enabled = false;
    btnCOMStartArray[index]->Enabled = false;
    /*
    if(BBCHIP_MT6516 != frmMultiLoad->m_chip_type)
    {
        cbAutoPollingArray[index]->Enabled = false;
    }*/
}

//----------------------------------------------------------------------------
void TMainForm::Enable_SpecificMsUI(int index)
{
    cbCOMArray[index]->Enabled = true;
    btnCOMStartArray[index]->Enabled = true;
    /*
    if(BBCHIP_MT6516 != frmMultiLoad->m_chip_type)
    {
        cbAutoPollingArray[index]->Enabled = true;
    }*/
}

void TMainForm::EnableAllChannelUI()
{
     for (int i = 0; i < MAX_SUPPORT_PORT_NUM; i++)
     {
         cbCOMArray[i]->Enabled = true;
        // cbAutoPollingArray[i]->Enabled = true;
         btnCOMStartArray[i]->Enabled = true;
         btnCOMStopArray[i]->Enabled = true;
     }
}
void TMainForm::DisableAllChannelUI()
{
     for (int i = 0; i < MAX_SUPPORT_PORT_NUM; i++)
     {
         cbCOMArray[i]->Enabled = false;
       //cbAutoPollingArray[i]->Enabled = false;
         btnCOMStartArray[i]->Enabled = false;
         btnCOMStopArray[i]->Enabled = false;
     }
}

bool TMainForm::GetChecksumEnable()
{
    return m_miEnableDACksm->Checked;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Start_Timer(int index)
{
    TimerCOMArray[index]->Enabled = true;
}

//----------------------------------------------------------------------------
void TMainForm::Stop_Timer(int index)
{
    TimerCOMArray[index]->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::mOpenDAFileClick(TObject *Sender)
{
    FileOpenDownloadAgentExecute();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Openlinkmapfile1Click(TObject *Sender)
{
   // FileOpenScatExecute();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::About1Click(TObject *Sender)
{
   // Form_About->Show();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenDownloadAgentFile1Click(TObject *Sender)
{
    FileOpenDownloadAgentExecute();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OpenScatterFile1Click(TObject *Sender)
{
    frmMultiLoad->Left = 1;
    frmMultiLoad->Top  = 1;
    frmMultiLoad->Show();
}

//=============================================================================
void TMainForm::Display_DAFile( void )
{
    m_asDAFileName =  INIFileH.read_DA_bin( INI_FILE_NAME, Application->ExeName );

    if(m_asDAFileName.IsEmpty())
    {
       int iLD = Application->ExeName.LastDelimiter("\\");
       if ( iLD > 0)
       {
           m_asDAFileName = Application->ExeName.SubString(1, iLD-1 );
           m_asDAFileName += "\\MTK_AllInOne_DA.bin";
       }
    }


    if( OpenReadDAFile(m_asDAFileName) )
    {
        int iLD = m_asDAFileName.LastDelimiter("\\:");
        if ( m_asDAFileName.LastDelimiter("\\:") > 0)
        {
            m_asDAPath = m_asDAFileName.SubString(1, iLD-1 );
        }
        edit_da_path->Text = m_asDAFileName;
    }

}

//----------------------------------------------------------------------------
void TMainForm::Save_DAFile( void )
{
    INIFileH.write_DA_bin( INI_FILE_NAME, Application->ExeName, m_asDAFileName);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Save_ScatterFlag( void )
{
    INIFileH.write_SCATTER(INI_FILE_NAME, Application->ExeName, ScatterEnable);
    INIFileH.write_Checksum_Ver(INI_FILE_NAME, Application->ExeName, Checksum_ver); // mtk71502,20130403
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Display_AuthFile( void )
{
   AnsiString as_auth_file;
    int ret;

    edit_auth_path->Text = "";
    //stAuthFile->Caption = "";

    as_auth_file =  INIFileH.read_AuthFile(INI_FILE_NAME, Application->ExeName );
    if( as_auth_file.AnsiCompareIC("") !=  0)
    {
        if( S_DONE != (ret=AUTH_Load(g_AUTH_HANDLE, as_auth_file.c_str())) )
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not update setting right now.", "FAIL", MB_OK );
                return;
            }
            else
            {
                AnsiString as_msg = " Load authentication File \"" + edit_auth_path->Text + "\" fail!";
	            	Application->MessageBox( as_msg.c_str(), "FAIL", MB_OK );
                return;
            }
        }
        edit_auth_path->Text = as_auth_file;
        sbMPDT->Panels->Items[0]->Text = "  Load authentication file successfully!";
    }
}

//----------------------------------------------------------------------------
void TMainForm::Save_AuthFile( void )
{
    INIFileH.write_AuthFile( INI_FILE_NAME, Application->ExeName, edit_auth_path->Text);
}

//=============================================================================
void TMainForm::Display_CertFile( void )
{
    AnsiString as_cert_file;
    int ret;

    edit_cert_path->Text = "";

    as_cert_file =  INIFileH.read_CertFile(INI_FILE_NAME, Application->ExeName );
    if( as_cert_file.AnsiCompareIC("") !=  0)
    {
        if( S_DONE != (ret = SCERT_Load(g_SCERT_HANDLE, as_cert_file.c_str())) )
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not update setting right now.", "FAIL", MB_OK );
                return;
            }
            else
            {
                AnsiString as_msg = " Load certification File \"" + edit_cert_path->Text + "\" fail!";
                Application->MessageBox( as_msg.c_str(), "FAIL", MB_OK );
                return;
            }
        }
        edit_cert_path->Text = as_cert_file;
        sbMPDT->Panels->Items[0]->Text = "Load certification file successfully!";
    }
}

//----------------------------------------------------------------------------
void TMainForm::Save_CertFile( void )
{
    INIFileH.write_CertFile( INI_FILE_NAME, Application->ExeName, edit_cert_path->Text);
}


//=============================================================================
void TMainForm::Display_Type(void)
{
    m_asType =  INIFileH.read_Type( INI_FILE_NAME, Application->ExeName );
#ifndef SERVICE_CENTER
    if(m_asType.AnsiCompareIC("Format and Download All") == 0)
    {
        cbType->ItemIndex = OP_FORMAT_AND_DOWNLOAD;
    }
    else if(m_asType.AnsiCompareIC("Firmware upgrade") == 0)
    {
        cbType->ItemIndex = OP_FIRMWARE_UPGRADE;
    }
    else if(m_asType.AnsiCompareIC("Format All") == 0)
    {
        cbType->ItemIndex = OP_FORMAT_ALL;
    }
    else if(m_asType.AnsiCompareIC("Format bootloader") == 0)
    {
        cbType->ItemIndex = OP_FORMAT_BOOTLOADER;
    }
#else
    Disable_TypeComboBox();
    cbType->ItemIndex = OP_FIRMWARE_UPGRADE;
#endif
}

//------------------------------------------------------------------------------
void TMainForm::Save_Type( void )
{
    switch ( cbType->ItemIndex )
    {
        case OP_FORMAT_AND_DOWNLOAD:
             INIFileH.write_Type(INI_FILE_NAME, Application->ExeName, (AnsiString) "Format and Download All");
             break;
        case OP_FIRMWARE_UPGRADE:
             INIFileH.write_Type(INI_FILE_NAME, Application->ExeName, (AnsiString) "Firmware upgrade");
             break;
        case OP_FORMAT_ALL:
             INIFileH.write_Type( INI_FILE_NAME, Application->ExeName, (AnsiString) "Format All");
             break;
        case OP_FORMAT_BOOTLOADER:
             INIFileH.write_Type( INI_FILE_NAME, Application->ExeName, (AnsiString) "Format bootloader");
             break;

         default:
              INIFileH.write_Type( INI_FILE_NAME, Application->ExeName, (AnsiString) "Format and Download All");
         break;
     }

}

//=============================================================================
E_FORMAT_OPTION TMainForm::Get_FormatOption( void )
{
    E_FORMAT_OPTION format_op;

    format_op = MANUAL_FORMAT;


    return format_op;
}

//---------------------------------------------------------------------------
void TMainForm::Display_FormatFatRange( void )
{
   /*Display_FormatFatRangeVisible();*/
    AnsiString as_Format_FAT_range =  INIFileH.read_Format_FAT_range(INI_FILE_NAME, Application->ExeName );
    if( as_Format_FAT_range.AnsiCompareIC( "manual" ) == 0 )
    {
        rbManual->Checked = true;
    }
    else
    {
        rbAuto->Checked = true;
    }
}

//------------------------------------------------------------------------------
void TMainForm::Save_FormatFatRange( void )
{
   AnsiString optional;
    if(rbManual->Checked )
    {
       optional ="manual" ;
    }
    else
    {
       optional ="auto" ;
    }
    INIFileH.write_Format_FAT_range( INI_FILE_NAME, Application->ExeName, optional);
}

//------------------------------------------------------------------------------
void TMainForm::Save_BkAndRsOptional(void)
{
    int folderName;
    if(Form_tool_settings->m_rbCOMAsName->Checked)
    {
	    folderName = OP_COM_FOLDER_NAME;
    }
    else
    {
	    folderName = OP_RID_FOLDER_NAME;
    }

    INIFileH.write_BkRs_FolderName("SPMultiPortFlashDownloadProject.ini", Application->ExeName, folderName);

    //Deletion
    bool deletion = Form_tool_settings->m_cbBackupDel->Checked;
    INIFileH.write_BkRs_Deletion("SPMultiPortFlashDownloadProject.ini", Application->ExeName, deletion);

}

//=============================================================================
void TMainForm::Display_BkAndRsOptional( void )
{
    int folderName =  INIFileH.read_BkRs_FolderName( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );

    switch(folderName)
    {
    case OP_COM_FOLDER_NAME:
        Form_tool_settings->m_rbCOMAsName->Checked = true;
        break;
    case OP_RID_FOLDER_NAME:
        Form_tool_settings->m_rbRIDAsName->Checked = true;
        break;

    default:
        Form_tool_settings->m_rbRIDAsName->Checked = true;
        break;
    }

    Form_tool_settings->m_cbBackupDel->Checked =  INIFileH.read_BkRs_Deletion( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );

}


void TMainForm::Save_OTPOption(void)
{
    if(Form_tool_settings->m_cbOTPLock->Checked)
    {
	    INIFileH.write_OTPOption(INI_FILE_NAME, Application->ExeName, (AnsiString) "OTP Lock");
    }
    else if(Form_tool_settings->m_cbOTPFormat->Checked)
    {
	    INIFileH.write_OTPOption(INI_FILE_NAME, Application->ExeName, (AnsiString) "OTP Format");
    }
    else if(Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked)
    {
	    INIFileH.write_OTPOption(INI_FILE_NAME, Application->ExeName, (AnsiString) "Firmware upgrade and OTP Lock");
    }
    else
		INIFileH.write_OTPOption(INI_FILE_NAME, Application->ExeName, (AnsiString) "NON OTP");

}

void TMainForm::Display_OTPOption( void )
{
    m_asOTPOption =  INIFileH.read_OTPOption( INI_FILE_NAME, Application->ExeName );

    if(m_asType.AnsiCompareIC("NON OTP") == 0)
    {
        Form_tool_settings->m_cbOTPLock->Checked = false;
		Form_tool_settings->m_cbOTPFormat->Checked = false;
		Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked = false;
    }

    else if(m_asType.AnsiCompareIC("OTP Lock") == 0)
    {
        Form_tool_settings->m_cbOTPLock->Checked = true;
    }
    else if(m_asType.AnsiCompareIC("OTP Format") == 0)
    {
        Form_tool_settings->m_cbOTPFormat->Checked = true;
    }
    else if(m_asType.AnsiCompareIC("Firmware Upgrade and OTP Lock") == 0)
    {
        Form_tool_settings->m_cbFirmwareUpgradeAndOTPLock->Checked = true;
    }
}


//=============================================================================
void TMainForm::Enable_AutoPulling( void )
{
    m_cbEnableAutoPulling->Enabled = true;
}

//=============================================================================
void TMainForm::Disable_AutoPulling( void )
{
    m_cbEnableAutoPulling->Enabled = false;
}

void TMainForm::Disable_ScanBtn( void )
{
    btnScan->Enabled = false;
}

void TMainForm::Enable_ScanBtn( void )
{
    btnScan->Enabled = true;
}

void TMainForm::Reset_Comport( void )
{
    int iCycle = 0;
    for(iCycle = 0; iCycle < MAX_SUPPORT_PORT_NUM; iCycle++){
        if(cbCOMArray[iCycle]->Checked == true){
            cbCOMArray[iCycle]->Caption = "N/A + N/A";
            iCOMNoArray[iCycle] = 0;
            iPreloaderCOMNoArray[iCycle] = 0;
        }
    }
    Save_COMNoArray();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//------------------------------------------------------------------------------
void TMainForm::Save_TwoSiteOption(void)
{
    int optional;

    if (rbUART->Checked)
    {
        optional = 0;
    }
    else if (rbUSB->Checked)
    {
        optional = 1;
    }

    INIFileH.write_TwoSite_Location("SPMultiPortFlashDownloadProject.ini", Application->ExeName, optional);

}


void TMainForm::Display_TwoSiteOption( void )
{
    int optional;
    optional =  INIFileH.read_TwoSite_Location( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );

    switch(optional)
    {
    case 0:
        rbUART->Checked = true;
        rbUSB->Checked = false;
        break;
    case 1:
        rbUART->Checked = false;
        rbUSB->Checked = true;
        break;

    default:
        rbUART->Checked = true;
        rbUSB->Checked = false;
        break;
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//------------------------------------------------------------------------------
void TMainForm::Save_DADLOption(void)
{
    int optional;

    optional = m_miEnableDADL->Checked ? 1 : 0;
    INIFileH.write_DADL_Enable("SPMultiPortFlashDownloadProject.ini", Application->ExeName, optional);

    optional = m_miEnableDACksm->Checked ? 1 : 0;
    INIFileH.write_DACksm_Enable("SPMultiPortFlashDownloadProject.ini", Application->ExeName, optional);


    if (m_miDADLFullSpeed->Checked)
    {
        optional = OP_DA_FULLSPEED;
    }
    else if (m_miDADLHighSpeed->Checked)
    {
        optional = OP_DA_HIGHSPEED;
    }

    INIFileH.write_DADL_Speed("SPMultiPortFlashDownloadProject.ini", Application->ExeName, optional);


    if (m_miDADLBattery->Checked)
    {
        optional = OP_PS_BATTERY;
    }
    else if (m_miDADLNoBattery->Checked)
    {
        optional = OP_PS_NOBAT;
    }
    else if (m_miDADLAutoDetect->Checked)
    {
        optional = OP_PS_AUTODETECT;
    }

    INIFileH.write_DADL_Battery("SPMultiPortFlashDownloadProject.ini", Application->ExeName, optional);


}


void TMainForm::Display_DADLOption( void )
{
    int optional =  INIFileH.read_DADL_Enable( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
    m_miEnableDADL->Checked = optional ? true : false;

    if(true == m_miEnableDADL->Checked)
    {
        EnableDADLComponent();
    }
    else
    {
        DisableDADLComponent();
    }


    optional =  INIFileH.read_DACksm_Enable( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
    m_miEnableDACksm->Checked = optional ? true : false;
    SetDAChecksum(m_miEnableDACksm->Checked);


    optional =  INIFileH.read_DADL_Speed( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
    switch(optional)
    {
    case OP_DA_FULLSPEED:
        m_miDADLFullSpeed->Checked = true;
        m_miDADLHighSpeed->Checked = false;
        break;
    case OP_DA_HIGHSPEED:
        m_miDADLFullSpeed->Checked = false;
        m_miDADLHighSpeed->Checked = true;
        break;

    default:
        /*m_miDADLFullSpeed->Checked = true;
        m_miDADLHighSpeed->Checked = false;*/
        m_miDADLFullSpeed->Checked = false;
        m_miDADLHighSpeed->Checked = true;
        break;
    }


    optional =  INIFileH.read_DADL_Battery( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
    switch(optional)
    {
    case OP_PS_BATTERY:
        m_miDADLBattery->Checked = true;
        m_miDADLNoBattery->Checked = false;
        m_miDADLAutoDetect->Checked = false;
        break;
    case OP_PS_NOBAT:
        m_miDADLBattery->Checked = false;
        m_miDADLNoBattery->Checked = true;
        m_miDADLAutoDetect->Checked = false;
        break;
    case OP_PS_AUTODETECT:
        m_miDADLBattery->Checked = false;
        m_miDADLNoBattery->Checked = false;
        m_miDADLAutoDetect->Checked = true;
        break;

    default:
        m_miDADLBattery->Checked = false;
        m_miDADLNoBattery->Checked = true;
        m_miDADLAutoDetect->Checked = false;
        break;
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_RunningTest(void)
{
    AnsiString ansiEnableWdtDogReset,ansiEnableOutputResult, ansiTemp; 
    ansiEnableWdtDogReset = INIFileH.read_EnableWthDog("RunningTest.ini",Application->ExeName);
    ansiEnableOutputResult = INIFileH.read_EnableOutputResult("RunningTest.ini",Application->ExeName);
    if(0==ansiEnableWdtDogReset.AnsiCompareIC("1"))
    {
        m_EnableWdtDogReset = 1;
    }
    else
    {
        m_EnableWdtDogReset = 0;
    }

    if(0==ansiEnableOutputResult.AnsiCompareIC("1"))
    {
        m_EnableOutputResult = 1;
    }
    else
    {
        m_EnableOutputResult = 0;
    }

	ansiTemp = INIFileH.read_SkipChecksum("RunningTest.ini",Application->ExeName);
	if(0==ansiTemp.AnsiCompareIC("yes"))
    {
        m_SkipChecksum = 1;
    }
    else
    {
        m_SkipChecksum = 0;
    }

	ansiTemp = INIFileH.read_FmPreloaderFlag("RunningTest.ini",Application->ExeName);
	if(0==ansiTemp.AnsiCompareIC("yes"))
    {
        m_FmPreloader = 1;
    }
    else
    {
        m_FmPreloader = 0;
    }
}

void __fastcall TMainForm::Read_ForceWatchdogReset(void)
{ 
    AnsiString ansiForceWatchdogFlag,ansiForceWatchdogTimeOut;
    ansiForceWatchdogFlag = INIFileH.read_ForceWatchdogResetFlag(INI_FILE_NAME,Application->ExeName);
    ansiForceWatchdogTimeOut = INIFileH.read_ForceWatchdogResetTimeOut(INI_FILE_NAME,Application->ExeName);
    if( 0== ansiForceWatchdogFlag.AnsiCompareIC("yes"))
    {
        m_EnableForceWdtDogReset= 1;
    }
    else
    {
        m_EnableForceWdtDogReset = 0;
    }

	m_ForceWdtDogResetTimeout = ansiForceWatchdogTimeOut.ToInt();
}

void __fastcall TMainForm::Read_EnableEnterMetaAfterDlFlag(void)
{
     m_EnableEnterMetaAfterDL = INIFileH.read_EnableEnterMetaFlag(INI_FILE_NAME,Application->ExeName);
}

void __fastcall TMainForm::Save_ForceWatchDogReset()
{   
    AnsiString as_WatchdogReset;
	AnsiString as_WatchdogResetTimeout(m_ForceWdtDogResetTimeout);
    if(m_EnableForceWdtDogReset == 1){
        as_WatchdogReset = "yes";
	}else{
	    as_WatchdogReset = "no";
	}
	
    INIFileH.write_ForceWatchdogResetFlag( INI_FILE_NAME, Application->ExeName, as_WatchdogReset);
	INIFileH.write_ForceWatchdogResetTimeOut( INI_FILE_NAME, Application->ExeName, as_WatchdogResetTimeout);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_EMI_Setting(void)
{
    AnsiString as;
    as = INIFileH.read_bank0_auto(INI_FILE_NAME, Application->ExeName);
    if (as.AnsiCompareIC("no") == 0 )
    {
        m_sDnParam.s_emi_param.b_bank0_auto = false;
    }
    else
    {
        m_sDnParam.s_emi_param.b_bank0_auto = true;
    }

    as = INIFileH.read_bank0_cfg(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.s_emi_param.ui_bank0_cfg = as.ToInt();

    as = INIFileH.read_bank1_auto(INI_FILE_NAME, Application->ExeName);
    if (as.AnsiCompareIC("no") == 0 )
    {
        m_sDnParam.s_emi_param.b_bank1_auto = false;
    }
    else
    {
        m_sDnParam.s_emi_param.b_bank1_auto = true;
    }

    as = INIFileH.read_bank1_cfg(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.s_emi_param.ui_bank1_cfg = as.ToInt();

    as = INIFileH.read_nand_acccon(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_nand_acccon = as.ToInt();

    as = INIFileH.read_emi_gen_a(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_a = as.ToInt();

    as = INIFileH.read_emi_gen_b(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_b = as.ToInt();

    as = INIFileH.read_emi_gen_c(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_gen_c = as.ToInt();

    as = INIFileH.read_emi_dram_con_i_mode(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_i_mode = as.ToInt();

    as = INIFileH.read_emi_dram_con_i_ext_mode(INI_FILE_NAME, Application->ExeName);
     m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_i_ext_mode = as.ToInt();

    as = INIFileH.read_emi_dram_con_k(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_k = as.ToInt();

    as = INIFileH.read_emi_dram_con_l(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.sEMI_NFI_Setting.ui_emi_dram_con_l = as.ToInt();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_FormatFATBeginAddress( void )
{
    AnsiString as_addr = INIFileH.read_Format_FAT_Begin_Address( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.s_nor_param.ui_fat_begin_addr = as_addr.ToInt();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_FormatFATLength( void )
{
    AnsiString as_len = INIFileH.read_Format_FAT_length( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.s_nor_param.ui_fat_length = as_len.ToInt();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_NANDFormatFATBeginAddress( void )
{
    AnsiString as_NAND_Format_FAT_begin_address = INIFileH.read_NAND_Format_FAT_Begin_Address( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.s_nfb_param.ui_nand_fat_begin_addr = as_NAND_Format_FAT_begin_address.ToInt();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_NANDFormatFATLength( void )
{
    AnsiString as_NAND_Format_FAT_length = INIFileH.read_NAND_Format_FAT_length( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.s_nfb_param.ui_nand_fat_length = as_NAND_Format_FAT_length.ToInt();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Read_EraseValidation( void )
{
    AnsiString as_EraseValidation = INIFileH.read_Erase_Validation( INI_FILE_NAME, Application->ExeName );
    if( as_EraseValidation.AnsiCompareIC("yes") == 0 )
    {   m_sDnParam.s_nfb_param.b_validation = _TRUE;
        m_sDnParam.s_nor_param.b_validation = _TRUE;
    }
    else
    {   m_sDnParam.s_nfb_param.b_validation = _FALSE;
        m_sDnParam.s_nor_param.b_validation = _FALSE;
    }
}


//=============================================================================
void TMainForm::Display_ComEnabled( void ) // It has been discarded.
{
    int i, j, bit_mask;

    AnsiString as_TheFirstComNo = INIFileH.read_TheFirstComPortNo( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.i_FirstComNo = as_TheFirstComNo.ToInt();
    AnsiString as_ComportChecked = INIFileH.read_ComEnabled( INI_FILE_NAME, Application->ExeName );
    iCOMChecked = as_ComportChecked.ToInt();

    //Set bit mask
    /*bit_mask = 0;
    for (i=0; i<MAX_SUPPORT_PORT_NUM; i++)
    {
        if(i!=0)
        {bit_mask <<=1;}

        bit_mask |= 1;
    }

    iCOMChecked &= bit_mask;*/


    //if(  miComManual->Checked )
    {
        for (i=0; i<MAX_SUPPORT_PORT_NUM; i++)
        {
            cbCOMArray[i]->Caption = "COM" + IntToStr( m_sDnParam.i_FirstComNo + i);
        }

        for(i=0;i<MAX_SUPPORT_PORT_NUM;i++)
        {
            j= (iCOMChecked & (1<<i));
            if(j==0)
            {
                cbCOMArray[i]->Checked = false;
            }
            else
            {
                cbCOMArray[i]->Checked = true;
            }
        }
    }

}

//----------------------------------------------------------------------------
void TMainForm::Save_ComEnabled( void )
{
    INIFileH.write_ComEnabled( INI_FILE_NAME, Application->ExeName, "0x"+IntToHex(iCOMChecked,2));
}

bool TMainForm::CheckCOMNoValid()
{
    for(int i = 0; i < MAX_SUPPORT_PORT_NUM; i++){
        if(true == cbCOMArray[i]->Checked){
            //if(0 == iCOMNoArray[i])
            if(0 == GetComNo(i, PRELOADER_COMPORT) && 0 == GetComNo(i, BOOTROM_COMPORT)){
                return false;
            }

			//if preloader comport and brom comport have the same comport number, tool will consider this situation as Novalid!
			if( (GetComNo(i, BOOTROM_COMPORT) == GetComNo(i, PRELOADER_COMPORT)) ){
                    return false;
            }

            for(int k = i+1; k < MAX_SUPPORT_PORT_NUM; k++){
                if( (true == cbCOMArray[k]->Checked) && (GetComNo(i, BOOTROM_COMPORT) == GetComNo(k, BOOTROM_COMPORT)) ){
                    if(GetComNo(k, BOOTROM_COMPORT) != 0)
                    return false;
                }

                if( (true == cbCOMArray[k]->Checked) && (GetComNo(i, PRELOADER_COMPORT) == GetComNo(k, PRELOADER_COMPORT)) ){
                    if(GetComNo(k, PRELOADER_COMPORT) != 0)
                    return false;
                }
            }
        }
    }

    return true;
}

//=============================================================================
void TMainForm::Display_COMNoArray( void )
{
    int i, j;
    int iBootromComNum, iPreloaderComNum;
    AnsiString str;
    INIFileH.read_COMNoArray( INI_FILE_NAME, Application->ExeName, iCOMNoArray, sizeof(iCOMNoArray)/sizeof(int));
	  INIFileH.read_COMPreloaderNoArray( INI_FILE_NAME, Application->ExeName, iPreloaderCOMNoArray, sizeof(iPreloaderCOMNoArray)/sizeof(int));

    for (i=0; i < MAX_SUPPORT_PORT_NUM; i++)
    {
        iBootromComNum = GetComNo(i,BOOTROM_COMPORT);
        iPreloaderComNum = GetComNo(i,PRELOADER_COMPORT);

        if(0 != iBootromComNum && 0 == iPreloaderComNum){
            str.sprintf("COM%2d + N/A", iBootromComNum);
        }
        else if(0 != iBootromComNum && 0 != iPreloaderComNum){
            str.sprintf("COM%2d + COM%2d", iBootromComNum, iPreloaderComNum);
        }
        else if(0 == iBootromComNum && 0 != iPreloaderComNum){
            str.sprintf("N/A + COM%2d", iPreloaderComNum);
        }
        else{
            str.sprintf("N/A + N/A");
        }
        cbCOMArray[i]->Caption =str;
    }

    AnsiString as_ComportChecked = INIFileH.read_ComEnabled( INI_FILE_NAME, Application->ExeName );
    iCOMChecked = as_ComportChecked.ToInt();
    for(i=0; i < MAX_SUPPORT_PORT_NUM; i++){
        j= (iCOMChecked & (1<<i));
        if(j==0){
            cbCOMArray[i]->Checked = false;
        }
        else{
            cbCOMArray[i]->Checked = true;
        }
    }
}

void TMainForm::Save_COMNoArray( void )
{
    INIFileH.write_COMNoArray( INI_FILE_NAME, Application->ExeName, iCOMNoArray, sizeof(iCOMNoArray)/sizeof(int));
	  INIFileH.write_COMPreloaderNoArray( INI_FILE_NAME, Application->ExeName, iPreloaderCOMNoArray, sizeof(iPreloaderCOMNoArray)/sizeof(int));
    INIFileH.write_ComEnabled( INI_FILE_NAME, Application->ExeName, "0x"+IntToHex(iCOMChecked,2));
}

//============================================================================
void TMainForm::Get_BBChipOption(S_BBCHIP_OPTION_T& bbchip_op)
{
    bbchip_op.b_check_eco_ver  = miCheckECOversion->Checked;
    bbchip_op.b_enable_nfi_cs1 = miEnableNFICS1->Checked;
}

//----------------------------------------------------------------------------
BBCHIP_TYPE TMainForm::Get_BBChipType(AnsiString as_BBChip)
{
    return AUTO_DETECT_BBCHIP;
}

//---------------------------------------------------------------------------
BBCHIP_TYPE TMainForm::Get_BBChipType(void)
{
    BBCHIP_TYPE bb_chip = MT6516_AP;
    if (miAutodetectBB->Checked)
    {
        bb_chip = AUTO_DETECT_BBCHIP;
    }

    return bb_chip;
}

//---------------------------------------------------------------------------
EXT_CLOCK TMainForm::Get_ExternalClockType( AnsiString as_ExtClock )
{
    if( as_ExtClock.AnsiCompareIC("auto detect") == 0 )  return AUTO_DETECT_EXT_CLOCK;
    else if( as_ExtClock.AnsiCompareIC("13MHz") == 0 )   return EXT_13M;
    else if( as_ExtClock.AnsiCompareIC("39MHz") == 0 )   return EXT_39M;
    else if( as_ExtClock.AnsiCompareIC("52MHz") == 0 )   return EXT_52M;
    else return EXT_26M;
}

//---------------------------------------------------------------------------
EXT_CLOCK TMainForm::Get_ExternalClockType( void )
{
    return AUTO_DETECT_EXT_CLOCK;
}

//=============================================================================
E_OPERATION_T TMainForm::Get_Operation(AnsiString as_Operation)
{
    E_OPERATION_T e_Operation;

    e_Operation = OPERATION_NFB;

    return e_Operation;
}

//----------------------------------------------------------------------------
E_OPERATION_T TMainForm::Get_Operation(void)
{
    E_OPERATION_T e_Operation;

    e_Operation = OPERATION_NFB;

    return e_Operation;
}

//----------------------------------------------------------------------------
void TMainForm::Display_Operation(void)
{
    AnsiString as_Operation = INIFileH.read_Operation(INI_FILE_NAME, Application->ExeName);
}

//------------------------------------------------------------------------------
void TMainForm::Save_Operation(void)
{
}

//=============================================================================
void TMainForm::Display_BasebandChip( void )
{

    m_asBBChip =  INIFileH.read_Baseband_chip( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.e_bbchip_type = Get_BBChipType(m_asBBChip);

    miAllBBChipUnChecked();

    miAutodetectBB->Checked = true;
    m_asBBChip = "auto detect";
}


//------------------------------------------------------------------------------
void TMainForm::Save_BasebandChip( void )
{
     INIFileH.write_Baseband_chip( INI_FILE_NAME, Application->ExeName, m_asBBChip);
}

//=============================================================================
void TMainForm::Display_ExternalClock( void )
{
    m_asExtClock =  INIFileH.read_External_clock( INI_FILE_NAME, Application->ExeName );
    m_sDnParam.e_ext_clock = Get_ExternalClockType(m_asExtClock);

    switch( m_sDnParam.e_ext_clock )
    {
        case AUTO_DETECT_EXT_CLOCK:
        {
            miAutodetectExtClock->Checked = true;
            mi13MHz->Checked = false;
            mi26MHz->Checked = false;
        }
        break;


        case EXT_13M:
        {
            miAutodetectExtClock->Checked = false;
            mi13MHz->Checked = true;
            mi26MHz->Checked = false;
        }
        break;


        case EXT_26M:
        default:
        {
            // set to 26mhz
            miAutodetectExtClock->Checked = false;
            mi26MHz->Checked = true;
            mi13MHz->Checked = false;
            m_asExtClock = (AnsiString) "26mhz";
        }
        break;
    }
}

//------------------------------------------------------------------------------
void TMainForm::Save_ExternalClock( void )
{
     INIFileH.write_External_clock( INI_FILE_NAME, Application->ExeName, m_asExtClock);
}

//=============================================================================
void TMainForm::Display_CheckECOVersion( void )
{
    miCheckECOversion->Checked = true;
}

//-----------------------------------------------------------------------------
void TMainForm::Save_CheckECOVersion( void )
{

}

//=============================================================================
S_MULTIBIN_OPTION TMainForm::Get_MultiBinOption( void )
{
    S_MULTIBIN_OPTION multibin_op;

    multibin_op.b_check_target_resource_layout    = miCheckTargetResourceLayout->Checked;
    multibin_op.b_auto_load_resource_file_by_scat = miAutoLoadResourcefilesByScatter->Checked;
    multibin_op.b_compare_resource_project_id     = miCompareResourceProjectId->Checked;

    return multibin_op;
}

//-----------------------------------------------------------------------------
void TMainForm::Display_CheckTargetResourceLayout( void )
{
    AnsiString as_CheckTgtResLayout =  INIFileH.read_CheckTgtResLayout( INI_FILE_NAME, Application->ExeName );
    if( as_CheckTgtResLayout.AnsiCompareIC("no") == 0 )
    {
        m_sDnParam.s_multibin_op.b_check_target_resource_layout = false;
        miCheckTargetResourceLayout->Checked = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_check_target_resource_layout = true;
        miCheckTargetResourceLayout->Checked = true;
    }
}

//-----------------------------------------------------------------------------
void TMainForm::Save_CheckTargetResourceLayout( void )
{
    if( miCheckTargetResourceLayout->Checked )
    {
        INIFileH.write_CheckTgtResLayout( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_CheckTgtResLayout( INI_FILE_NAME, Application->ExeName, "no");
    }
}

//=============================================================================
void TMainForm::Display_AutoLoadResByScat( void )
{
    AnsiString as_AutoLoadResByScat =  INIFileH.read_AutoLoadResByScat( INI_FILE_NAME, Application->ExeName );
    if( as_AutoLoadResByScat.AnsiCompareIC("no") == 0 )
    {
        m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat = false;
        miAutoLoadResourcefilesByScatter->Checked = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat = true;
        miAutoLoadResourcefilesByScatter->Checked = true;
    }
}

//-----------------------------------------------------------------------------
void TMainForm::Save_AutoLoadResByScat( void )
{
    if( miAutoLoadResourcefilesByScatter->Checked )
    {
        INIFileH.write_AutoLoadResByScat( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_AutoLoadResByScat( INI_FILE_NAME, Application->ExeName, "no");
    }
}

//=============================================================================
void TMainForm::Display_CompareResProjectId( void )
{
    AnsiString as_CompareResProjectId =  INIFileH.read_CompareResProjectId( INI_FILE_NAME, Application->ExeName );
    if( as_CompareResProjectId.AnsiCompareIC("no") == 0 )
    {
        m_sDnParam.s_multibin_op.b_compare_resource_project_id = false;
        miCompareResourceProjectId->Checked = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_compare_resource_project_id = true;
        miCompareResourceProjectId->Checked = true;
    }

    for( int i=0; i<MAX_SUPPORT_LOAD_NUM; i++ )
    {
        DL_ResourceProjectIdComparisonSetting(*g_pDL_HANDLE[i], m_sDnParam.s_multibin_op.b_compare_resource_project_id?_TRUE:_FALSE);
    }
}

//-----------------------------------------------------------------------------
void TMainForm::Save_CompareResProjectId( void )
{
    if( miCompareResourceProjectId->Checked )
    {
        INIFileH.write_CompareResProjectId( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_CompareResProjectId( INI_FILE_NAME, Application->ExeName, "no");
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Display_BROMDllLog( void )
{
    AnsiString as_BROMDllLog =  INIFileH.read_BROMDllLog( INI_FILE_NAME, Application->ExeName );

	std::string dll_log_path;
    dll_log_path = m_DLL_log_path + "\\BROM_DLL_V5.log";

    if( as_BROMDllLog.AnsiCompareIC("no") == 0 )
    {
        miBromDllLog->Checked = false;
    }
    else
    {
        miBromDllLog->Checked = true;
		Brom_Debug_SetLogFilename(dll_log_path.c_str());
        Brom_DebugOn();
    }

}

//----------------------------------------------------------------------------
void TMainForm::Save_BROMDllLog( void )
{
    if( miBromDllLog->Checked )
    {
        INIFileH.write_BROMDllLog( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_BROMDllLog( INI_FILE_NAME, Application->ExeName, "no");
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Display_EbootDllLog( void )
{
    AnsiString as_EbootDllLog =  INIFileH.read_EBootDllLog( INI_FILE_NAME, Application->ExeName );
	std::string dll_log_path;
    dll_log_path = m_DLL_log_path + "\\EBOOT_DLL.log";
    if( as_EbootDllLog.AnsiCompareIC("no") == 0 )
    {
        miEbootDllLog->Checked = false;
    }
    else
    {
        miEbootDllLog->Checked = true;

	    Eboot_DebugChangePath(dll_log_path.c_str());
        //Eboot_DebugOn();
        GC_TRACE_ON();
    }
}

//----------------------------------------------------------------------------
void TMainForm::Save_EbootDllLog( void )
{
    if( miEbootDllLog->Checked )
    {
        INIFileH.write_EBootDllLog( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_EBootDllLog( INI_FILE_NAME, Application->ExeName, "no");
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Display_METADllLog( void )
{
    AnsiString as_METADllLog =  INIFileH.read_BROMDllLog( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
	//std::string dll_log_path;
    //dll_log_path = m_DLL_log_path + "\\META_DLL.log";

    if( as_METADllLog.AnsiCompareIC("no") == 0 )
    {
        miMETADllLog->Checked = false;
    }
    else
    {
        miMETADllLog->Checked = true;

	    //META_DebugOn_With_FilePath(dll_log_path.c_str());
        SP_META_DebugOn();
    }
}

//----------------------------------------------------------------------------
void TMainForm::Save_METADllLog( void )
{
    if( miMETADllLog->Checked )
    {
        INIFileH.write_METADllLog( INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_METADllLog( INI_FILE_NAME, Application->ExeName, "no");
    }
}

//=============================================================================
void TMainForm::Display_Baudrate( void )
{
    m_asBaudrate = INIFileH.read_Baudrate(INI_FILE_NAME, Application->ExeName);
    m_sDnParam.e_baud_rate = Get_Baudrate(m_asBaudrate);

    switch (m_sDnParam.e_baud_rate)
    {
        case UART_BAUD_115200:
        {
            cbBaudrate->ItemIndex = BAUD_RATE_115200_INDEX;
        }
        break;

        case UART_BAUD_230400:
        {
            cbBaudrate->ItemIndex = BAUD_RATE_230400_INDEX;
        }
        break;

        case UART_BAUD_460800:
        {
            cbBaudrate->ItemIndex = BAUD_RATE_460800_INDEX;
        }
        break;

        case UART_BAUD_921600:
        {
            cbBaudrate->ItemIndex = BAUD_RATE_921600_INDEX;
        }
        break;

        default: // default set to 460800
        {
            m_sDnParam.e_baud_rate = UART_BAUD_460800;
            cbBaudrate->ItemIndex =  BAUD_RATE_460800_INDEX;
            m_asBaudrate = (AnsiString) "460800";
        }
        break;
    }
}

//------------------------------------------------------------------------------
void TMainForm::Save_Baudrate(void)
{
     switch ( cbBaudrate->ItemIndex )
     {
         case BAUD_RATE_115200_INDEX:
              INIFileH.write_Baudrate( INI_FILE_NAME, Application->ExeName, (AnsiString) "115200");
         break;

         case BAUD_RATE_230400_INDEX:
              INIFileH.write_Baudrate( INI_FILE_NAME, Application->ExeName, (AnsiString) "230400");
         break;

         case BAUD_RATE_460800_INDEX:
              INIFileH.write_Baudrate( INI_FILE_NAME, Application->ExeName, (AnsiString) "460800");
         break;

         case BAUD_RATE_921600_INDEX:
              INIFileH.write_Baudrate( INI_FILE_NAME, Application->ExeName, (AnsiString) "921600");
         break;

         default:  // default set to 460800
              INIFileH.write_Baudrate( INI_FILE_NAME, Application->ExeName, (AnsiString) "460800");
         break;
     }
}

//=============================================================================
void TMainForm::Display_AutoPollingEnabled( void )
{
    //update enable status of AutoPulling checkbox
    int iAutoPolling;
    m_asAutoPollEn = INIFileH.read_AutoPollingEnabled( INI_FILE_NAME, Application->ExeName );
    iAutoPolling = m_asAutoPollEn.ToInt();
    if(0 == iAutoPolling){
       m_cbEnableAutoPulling->Checked = false;
    }
    else{
       m_cbEnableAutoPulling->Checked = true;
    }

    AnsiString asAutoPollingCount;
    asAutoPollingCount = INIFileH.read_AutoPollingCount( INI_FILE_NAME, Application->ExeName );
    iAutoPollingCount = asAutoPollingCount.ToInt();
}

void  TMainForm::HideScanButton()
{
    btnScan->Visible = false;
}

//----------------------------------------------------------------------------
void  TMainForm::Save_AutoPollingEnabled( void )
{
    if(m_cbEnableAutoPulling->Checked == true){
        INIFileH.write_AutoPollingEnabled( INI_FILE_NAME, Application->ExeName, "1");
    }
    else{
        INIFileH.write_AutoPollingEnabled( INI_FILE_NAME, Application->ExeName, "0");
    }

    INIFileH.write_AutoPollingCount( INI_FILE_NAME, Application->ExeName, IntToStr(iAutoPollingCount));
}

//==============================================================================
void  TMainForm::get_PullDownGPIO17( void )
{
    m_asPullDownGPIO17 =  INIFileH.read_Pull_down_GPIO17( INI_FILE_NAME, Application->ExeName );
    if( m_asPullDownGPIO17.AnsiCompareIC( "yes" ) == 0 )
    {
        m_sDnParam.b_PullDownGPIO17 = true;
    }
    else
    {
        m_sDnParam.b_PullDownGPIO17 = false;
    }
}

void    TMainForm::clear_download_status(int index)
{
    Set_COMPercentageCaption(index, "");
    Set_shpCOMColor( index, clGreen );
    lbCOMTime[index]->Caption = "0 S";

    SetPbComBackColor(index, clWhite);
    SetPbComForeColor(index, clWhite);
    SetPbComProgress(index, 0);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::btnStartAllClick(TObject *Sender)
{

    if(!ScatterEnable && ScatterLoadCompleteFlag)
    {
         Application->MessageBox( "Bin File is not fully loaded , please check scatter and load setting!", "Reminder", MB_OK );
         return;
    }

	if(MainForm->m_SkipChecksum == 1)
	{
	     GC_TRACE("[COM%d] btnStartAllClick(): Skip Checksum!", m_sComPortSetting.com.number);
	}
	else
	{
        if (!TformPCMemoryChecksum::mChecksumFlag)
        {
            formPCMemoryChecksum->ShowModal();

        }
        if (!TformPCMemoryChecksum::mChecksumFlag)
        {
            GC_TRACE("[COM%d] btnStartAllClick(): PC Memory checksum mismatched!", m_sComPortSetting.com.number);
            return;
        }
	}

   


    //return;
    int m_DownLoadStatusResult;

    hasPolling = 0;

    if (!CheckScatterFile())
    {
        return;
    }

#if 0
    if( !VerifyHostBufferBeforeStart() )
    {
        return;
    }
#endif

    if( !CheckCOMNoValid() )
    {
        Application->MessageBox("Warning: Maybe error as follows:\n(1)COM ports number repetitive or invalid\n(2)Brom and preloader have the same comport number!\nPlease modify them in INI file or scan again.", "FAIL", MB_OK);
        return;
    }

    if (!DownloadCommonFunction())
    {
        return;
    }

    //btnScan->Enabled = false;
    //btnStartAll->Enabled = false;

    for (int i=0; i< MAX_SUPPORT_PORT_NUM; i++)
    {
        //If
        if(MainForm->_t_manager.Is_running(i))
        {
           continue;
        }

        clear_download_status(i);

        if (cbCOMArray[i]->Checked )
        {
            Set_DownloadParameter(i);
            if( miEnableFDLog->Checked )
            {
                // access
                TDateTime datetime;
                TDateTime date = datetime.CurrentDate();
                TDateTime time = datetime.CurrentTime();
                sTime     stime;
                time.DecodeTime( &stime.hour, &stime.min, &stime.sec, &stime.msec);
                MainForm->SetDnDate( i, date );
                MainForm->SetDnStartTime( i, time );
                MainForm->SetDnStartsTime( i, stime );
            }



            m_bStartMetaApp[i] = false;
            //g_iStopFlag[i] = 0;
            DownLoadTimeSec[i] = 0;
            COMPercentageArray[i]->Caption = "";
            TimerCOMArray[i]->Enabled = true;

            _t_manager.Start(i, hPostMsgDestHandle, m_sComPortSetting, m_sDnParam);

            btnCOMStartArray[i]->Enabled = false;
            cbCOMArray[i]->Enabled = false;
            //cbAutoPollingArray[i]->Enabled = false;
        }
   }

   Update_UI();
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::btnCOMDownloadClick(int index)
{
    if( !cbCOMArray[index]->Checked )
    {
        return;
    }

    if (!CheckScatterFile())
    {
        return;
    }

#if 0
    if( !VerifyHostBufferBeforeStart() )
    {
        return;
    }
#endif

    if( !CheckCOMNoValid() )
    {
        Application->MessageBox("Warning: Maybe error as follows:\n(1)COM ports number repetitive or invalid\n(2)Brom and preloader have the same comport number!\nPlease modify them in INI file or scan again.", "FAIL", MB_OK);
        return;
    }

    if (!DownloadCommonFunction())
    {
        return;
    }

    if( miEnableFDLog->Checked )
    {
        // access
        TDateTime datetime;
        TDateTime date = datetime.CurrentDate();
        TDateTime time = datetime.CurrentTime();
        sTime     stime;
        time.DecodeTime( &stime.hour, &stime.min, &stime.sec, &stime.msec);
        MainForm->SetDnDate( index, date );
        MainForm->SetDnStartTime( index, time );
        MainForm->SetDnStartsTime( index, stime );
    }

    Set_DownloadParameter( index );
    m_bStartMetaApp[index] = false;

    pbCOMArray[index]->Progress = 0;
    DownLoadTimeSec[index] = 0;
    TimerCOMArray[index]->Enabled = true;

    _t_manager.Start(index, hPostMsgDestHandle, m_sComPortSetting, m_sDnParam);

    Update_UI();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::btnSeperateDownloadClick(TObject *Sender)
{
    TButton *bt = (TButton*)Sender;
    hasPolling = 0;
   if(!ScatterEnable && ScatterLoadCompleteFlag)
   	{
		  Application->MessageBox( "Bin File is not fully loaded , please check scatter and load setting!", "Reminder", MB_OK );
		  return;
   	}
    if (!TformPCMemoryChecksum::mChecksumFlag)
    {
        formPCMemoryChecksum->ShowModal();

    }
    if (!TformPCMemoryChecksum::mChecksumFlag)
    {
        GC_TRACE("[COM%d] btnSeperateDownloadClick(): PC Memory checksum mismatched!", m_sComPortSetting.com.number);
        return;
    }

    btnCOMDownloadClick( bt->Tag );

}



//==============================================================================
/////////////////////////////    Timer     /////////////////////////////////////
//==============================================================================
void  __fastcall TMainForm::TimerCOMTimer(int index)
{
    if (m_bStartMetaApp[index])
    {
        if (pbCOMArray[index]->Progress < 100)
        {
           pbCOMArray[index]->Progress++;
        }
    }
    DownLoadTimeSec[index]++;
    lbCOMTime[index]->Caption = (AnsiString) DownLoadTimeSec[index] + " S";
}

void __fastcall TMainForm::TimerCOM1Timer(TObject *Sender)
{
   TimerCOMTimer(0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM2Timer(TObject *Sender)
{
    TimerCOMTimer(1);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM3Timer(TObject *Sender)
{
    TimerCOMTimer(2);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM4Timer(TObject *Sender)
{
    TimerCOMTimer(3);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM5Timer(TObject *Sender)
{
    TimerCOMTimer(4);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM6Timer(TObject *Sender)
{
    TimerCOMTimer(5);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM7Timer(TObject *Sender)
{
    TimerCOMTimer(6);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM8Timer(TObject *Sender)
{
   TimerCOMTimer(7);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM9Timer(TObject *Sender)
{
    TimerCOMTimer(8);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM10Timer(TObject *Sender)
{
    TimerCOMTimer(9);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM11Timer(TObject *Sender)
{
    TimerCOMTimer(10);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM12Timer(TObject *Sender)
{
    TimerCOMTimer(11);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM13Timer(TObject *Sender)
{
    TimerCOMTimer(12);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM14Timer(TObject *Sender)
{
    TimerCOMTimer(13);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM15Timer(TObject *Sender)
{
    TimerCOMTimer(14);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerCOM16Timer(TObject *Sender)
{
    TimerCOMTimer(15);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::miAllBBChipUnChecked(void)
{
    miAutodetectBB->Checked = false;

}

//--------------------------------------------------------------------------
void __fastcall TMainForm::miAutodetectBBClick(TObject *Sender)
{
    miAllBBChipUnChecked();
    miAutodetectBB->Checked = true;

    m_asBBChip = (AnsiString) "auto detect";
    m_sDnParam.e_bbchip_type = Get_BBChipType( m_asBBChip );

}

//---------------------------------------------------------------------------
bool TMainForm::Set_COMPercentageCaption( int index, AnsiString as_str )
{
     COMPercentageArray[index]->Caption = as_str;
     return true;
}

//---------------------------------------------------------------------------
bool TMainForm::Set_shpCOMColor(int index, TColor color)
{
     shpCOMArray[index]->Brush->Color = color;
     return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::SetPbComProgress( int index, unsigned char progress_percentage )
{   pbCOMArray[index]->Progress = progress_percentage;
}

//----------------------------------------------------------------------------
void TMainForm::SetPbComForeColor(int index, TColor color)
{   pbCOMArray[index]->ForeColor = color;
}

//----------------------------------------------------------------------------
void TMainForm::SetPbComBackColor(int index, TColor color)
{   pbCOMArray[index]->BackColor = color;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Set_StartMetaAppFlag(unsigned char index, bool flag)
{
    m_bStartMetaApp[index] = flag;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miCheckECOversionClick(TObject *Sender)
{
    if(miCheckECOversion->Checked)
    {
        miCheckECOversion->Checked = false;
        m_sDnParam.s_bbchip_op.b_check_eco_ver = false;
        m_asCheckECOVer = "no";
    }
    else
    {
        miCheckECOversion->Checked = true;
        m_sDnParam.s_bbchip_op.b_check_eco_ver = true;
        m_asCheckECOVer = "yes";
    }
}

void __fastcall TMainForm::cbSelAllCOMClick(TObject *Sender)
{
    TCheckBox *cb = (TCheckBox*)Sender;

    for(int i=0; i<MAX_SUPPORT_PORT_NUM /*MAX_SUPPORT_PORT_NUM*/; i++)
    {
      if(true == cbCOMArray[i]->Enabled)
      {
        if (cb->Checked)
        {
            iCOMChecked |= (1<<i);
            cbCOMArray[i]->Checked = 1;
        }
        else
        {
            iCOMChecked &= ~(1<<i);
            cbCOMArray[i]->Checked = 0;
        }
      }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::cbCOMClick(TObject *Sender)
{
    TCheckBox *cb = (TCheckBox*)Sender;
    if (cbCOMArray[cb->Tag]->Checked)
    {
        iCOMChecked |= (1<<cb->Tag);
    }
    else
    {
        iCOMChecked &= ~(1<<cb->Tag);
    }
    //set Sel All checkbox, now MAX_SUPPORT_PORT_NUM =6 ;
/*
    if( iCOMChecked == 0x0000003F )
    {
        cbSelAllCOM->Checked = true;
    }
    else if (iCOMChecked == 0)
    {
        cbSelAllCOM->Checked = false;
    }
    */

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miAboutClick(TObject *Sender)
{
    Form_AboutMD->Left = 234;
    Form_AboutMD->Top  = 103;
    Form_AboutMD->Show();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::cbTypeChange(TObject *Sender)
{
    switch (cbType->ItemIndex)
    {
        case OP_FORMAT_AND_DOWNLOAD:
        {
            m_asType = "Format and Download All";
            //DisableTwoSiteComponent();
            break;
        }
        case OP_FIRMWARE_UPGRADE:
        {
            m_asType = "Firmware upgrade";
            //Reset_DADownloadAll();
            //Reset_DAChecksum();
            //DisableTwoSiteComponent();
            break;
        }
        case OP_FORMAT_ALL:
        {
            m_asType = "Format All";
            //Reset_DADownloadAll();
            //Reset_DAChecksum();
            //DisableTwoSiteComponent();
            break;
        }
        case OP_FORMAT_BOOTLOADER:
        {
            m_asType = "Format bootloader";
            //Reset_DADownloadAll();
            //Reset_DAChecksum();
            //EnableTwoSiteComponent();
            break;
        }

        default:
	   m_asType = "Format and Download All";

    }
}

//---------------------------------------------------------------------------
void TMainForm::btnCOMStopClick(int i)
{
    if (_t_manager.Is_running(i) )
    {
        Set_COMPercentageCaption(i, "STOPPING") ;
        _t_manager.Stop(i);
        _t_manager.Terminate(i);
    }

    TimerCOMArray[i]->Enabled = false;

    Update_UI();
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::btnSeperateStopClick(TObject *Sender)
{
    TButton *bt = (TButton*)Sender;
    btnCOMStopClick( bt->Tag );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::btnStopAllClick(TObject *Sender)
{
    scanStopFlag = true;
    StopAllClick();
    //Enable_UIComponent();
    //RefreshUIComponent();
}

void __fastcall TMainForm::StopAllClick()
{
    for(int i=0; i< MAX_SUPPORT_PORT_NUM; i++)
    {
        _t_manager.Stop(i);
        Stop_Timer(i);

        if(_t_manager.Is_running(i) )
        {
            Set_COMPercentageCaption(i, "STOPPING");
            _t_manager.Terminate(i);
        }
    }

    Update_UI();
}



//---------------------------------------------------------------------------
void TMainForm::UpdateMenuItem( void )
{
    // multibin option
    if( m_sDnParam.s_multibin_op.b_check_target_resource_layout )
    {
        miCheckTargetResourceLayout->Checked = true;
    }
    else
    {
        miCheckTargetResourceLayout->Checked = false;
    }

    if( m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat )
    {
	miAutoLoadResourcefilesByScatter->Checked = true;
    }
    else
    {
	miAutoLoadResourcefilesByScatter->Checked = false;
    }

    if( m_sDnParam.s_multibin_op.b_compare_resource_project_id )
    {
        miCompareResourceProjectId->Checked = true;
    }
    else
    {
        miCompareResourceProjectId->Checked = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miCheckTargetResourceLayoutClick(
      TObject *Sender)
{
    if( m_sDnParam.s_multibin_op.b_check_target_resource_layout )
    {
        m_sDnParam.s_multibin_op.b_check_target_resource_layout = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_check_target_resource_layout = true;
    }

    UpdateMenuItem();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miAutoLoadResourcefilesByScatterClick(
      TObject *Sender)
{
    if( m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat )
    {
        m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat = true;
    }

    UpdateMenuItem();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miCompareResourceProjectIdClick(TObject *Sender)
{
    if( m_sDnParam.s_multibin_op.b_compare_resource_project_id )
    {
        m_sDnParam.s_multibin_op.b_compare_resource_project_id = false;
    }
    else
    {
        m_sDnParam.s_multibin_op.b_compare_resource_project_id = true;
    }

    for( int i=0; i<MAX_SUPPORT_LOAD_NUM; i++ )
    {
        DL_ResourceProjectIdComparisonSetting(*g_pDL_HANDLE[i], m_sDnParam.s_multibin_op.b_compare_resource_project_id?_TRUE:_FALSE);
    }
    UpdateMenuItem();
}
//---------------------------------------------------------------------------
//==============================================================================
////////////////////////////    Global information    //////////////////////////
//==============================================================================
bool TMainForm::Get_CheckTgtResLayout( void )
{
    return m_sDnParam.s_multibin_op.b_check_target_resource_layout;
}

//-----------------------------------------------------------------------------
bool TMainForm::Get_AutoLoadResByScat( void )
{
    return m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat;
}

//-----------------------------------------------------------------------------
bool TMainForm::Get_CompareResProjectId( void )
{
    return m_sDnParam.s_multibin_op.b_compare_resource_project_id;
}

//==============================================================================
void __fastcall TMainForm::cbBaudrateChange(TObject *Sender)
{
    switch (cbBaudrate->ItemIndex)
    {
        case BAUD_RATE_115200_INDEX:
        {
            m_sDnParam.e_baud_rate = UART_BAUD_115200;
        }
        break;

        case BAUD_RATE_230400_INDEX:
        {
            m_sDnParam.e_baud_rate = UART_BAUD_230400;
        }
        break;

        case BAUD_RATE_460800_INDEX:
        {
            m_sDnParam.e_baud_rate = UART_BAUD_460800;
        }
        break;

        case BAUD_RATE_921600_INDEX:
        {
            m_sDnParam.e_baud_rate = UART_BAUD_921600;
        }
        break;
    }
}

//==============================================================================
//----------------         Access related function       -----------------------
//==============================================================================
//==============================================================================
///////////////////////////    ADO connection    ///////////////////////////////
//==============================================================================
void TMainForm::Set_ADOConnLoginPromptOff(void)
{
     ADOConn->LoginPrompt = false;
}

//------------------------------------------------------------------------------
void TMainForm::Set_ADOConnString( AnsiString as_Data_Src )
{
    ADOConn->Close();
    AnsiString as_Provider = "Provider=Microsoft.Jet.OLEDB.4.0";
    AnsiString as_Security = "Persist Security Info=False";
    AnsiString as_DataSrc_str = "Data Source="+as_Data_Src;
    AnsiString as_ConnStr = as_Provider + ";" + as_DataSrc_str + ";" + as_Security;
    ADOConn->ConnectionString = as_ConnStr;
    ADOConn->Open();
}

//------------------------------------------------------------------------------
bool TMainForm::Check_ADOConnConnected( void )
{
    if( ADOConn->Connected )
    {
        return true;
    }
    return false;

}

//------------------------------------------------------------------------------
void TMainForm::InsertNewRecordToAccess( void )
{
    ADOTab->Insert();
}

//------------------------------------------------------------------------------
void TMainForm::ActivateADOTab( void )
{
    ADOTab->Active = true;
}

//------------------------------------------------------------------------------
void TMainForm::DeActivateADOTab( void )
{
    ADOTab->Active = false;
}

//------------------------------------------------------------------------------
void TMainForm::PostRecordToAccess( void )
{
     ADOTab->Post();
}


//------------------------------------------------------------------------------
void TMainForm::SetTabNameToMultiport_download( void )
{
    ADOTab->Active = false;
    ADOTab->TableName = "SP_Multiport_download";
    ADOTab->Active = true;
}

//------------------------------------------------------------------------------
void TMainForm::AddStationToAccess( void )
{
    ADOTab->FieldByName("STATION")->AsString = "SmartPhone Multiport Download";

}

//------------------------------------------------------------------------------
void TMainForm::AddPcToAccess( void )
{
    char hostname[256];
    unsigned long len=256;

    GetComputerName( hostname, &len );
    ADOTab->FieldByName("PC")->AsString = hostname;

}
//------------------------------------------------------------------------------
void TMainForm::AddToolTypeToAccess( void )
{
    ADOTab->FieldByName("TOOL_TYPE")->AsInteger = TOOL_TYPE_MULTIPORT_DN;
}

//------------------------------------------------------------------------------
void TMainForm::AddDateToAccess( int index )
{
    ADOTab->FieldByName("DATE")->AsString = DateToStr(dn_time[index].dn_date);

}

//------------------------------------------------------------------------------
void TMainForm::AddTimeToAccess( int index )
{
    ADOTab->FieldByName("TIME")->AsString = TimeToStr(dn_time[index].dn_start_time);

}

//------------------------------------------------------------------------------
void TMainForm::AddPassFailToAccess( int index )
{
    ADOTab->FieldByName("PASS_FAIL")->AsString = IntToStr(PassFail[index]);

}

//------------------------------------------------------------------------------
void TMainForm::AddFileSizeToAccess( void )
{
    ADOTab->FieldByName("FILE_SIZE")->AsString = IntToStr(m_uiTotalFileSize);

}

//------------------------------------------------------------------------------
void TMainForm::AddDownloadTimeToAccess( int index )
{
    float f_dn_time = dn_time[index].dn_time_ms / 1000.0;
    ADOTab->FieldByName("DOWNLOAD_TIME")->AsString = FloatToStr(f_dn_time);

}

//------------------------------------------------------------------------------
void TMainForm::AddThrougphputToAccess( int index )
{
    ADOTab->FieldByName("THROGHPUT")->AsString = FloatToStr(throughput[index]);

}


//------------------------------------------------------------------------------
void TMainForm::AddFailStepToAccess( int fail_step )
{
    ADOTab->FieldByName("FAIL_STEP")->AsString = IntToStr(fail_step);

}

//------------------------------------------------------------------------------
void TMainForm::AddFailReasonToAccess( int index )
{
    ADOTab->FieldByName("FAIL_REASON")->AsString = FailReason[index];
}

//------------------------------------------------------------------------------
bool TMainForm::AddDnResultToAccess( int index )
{
    hSemaphore = CreateSemaphore( NULL, 1, 1, NULL);

    if( ! Check_ADOConnConnected() )
    {
        Application->MessageBox( "Execution Failure : Connect to data source file", "FAILURE", MB_OK );
        return false;
    }

    SetTabNameToMultiport_download();
    InsertNewRecordToAccess();

    AddStationToAccess();
    AddPcToAccess();
    AddDateToAccess(index);
    AddTimeToAccess(index);
    AddPassFailToAccess(index);
    AddFailReasonToAccess(index);
    if(  TEST_RESULT_PASS == PassFail[index] )
    {
        AddFileSizeToAccess();
        AddDownloadTimeToAccess(index);
        AddThrougphputToAccess(index);
    }

    PostRecordToAccess();

    ReleaseSemaphore( hSemaphore,  1, NULL );

    return true;
}

//==============================================================================
/////////////////////       test result         ////////////////////////////////
//==============================================================================
bool TMainForm::SetPassFail( int index, int pass_fail)
{
    PassFail[index] = pass_fail;
    return true;
}

//------------------------------------------------------------------------------
int TMainForm::GetPassFail( int index )
{
    return PassFail[index];
}


//==============================================================================
/////////////////////       time handling       ////////////////////////////////
//==============================================================================
bool TMainForm::SetDnDate( int index, TDateTime date )
{
    dn_time[index].dn_date = date;
    return true;
}

//------------------------------------------------------------------------------
TDateTime TMainForm::GetDnDate( int index )
{
    return dn_time[index].dn_date;
}
//------------------------------------------------------------------------------
bool TMainForm::SetDnStartTime( int index, TDateTime time )
{
    dn_time[index].dn_start_time = time;
    return true;
}

//------------------------------------------------------------------------------
TDateTime TMainForm::GetDnStartTime( int index )
{
    return  dn_time[index].dn_start_time;
}

//------------------------------------------------------------------------------
bool TMainForm::SetDnStartsTime( int index, sTime time )
{
    dn_time[index].dn_start_stime = time;
    return true;
}

//------------------------------------------------------------------------------
sTime TMainForm::GetDnStartsTime( int index )
{
    return  dn_time[index].dn_start_stime;
}

//------------------------------------------------------------------------------
bool TMainForm::SetDnEndTime( int index, TDateTime time )
{
    dn_time[index].dn_end_time = time;
    return true;
}

//------------------------------------------------------------------------------
TDateTime TMainForm::GetDnEndTime( int index )
{
    return dn_time[index].dn_end_time;
}

//------------------------------------------------------------------------------
bool TMainForm::SetDnEndsTime( int index, sTime time )
{
    dn_time[index].dn_end_stime = time;
    return true;
}

//------------------------------------------------------------------------------
sTime TMainForm::GetDnEndsTime( int index )
{
    return dn_time[index].dn_end_stime;
}

//------------------------------------------------------------------------------
bool TMainForm::SetDnTime( int index )
{
    dn_time[index].dn_time_ms = DifferenceMiniSec( &dn_time[index].dn_start_stime, &dn_time[index].dn_end_stime );
    return true;
}

//------------------------------------------------------------------------------
unsigned TMainForm::GetDnTime( int index )
{
    return dn_time[index].dn_time_ms;
}

//------------------------------------------------------------------------------
bool TMainForm::SetThroughput( int index )
{
    if( m_uiTotalFileSize <= 0 )
    {
        return false;
    }
    throughput[index] = 1.0 * m_uiTotalFileSize / (dn_time[index].dn_time_ms / 1000.0);
    return true;
}

//------------------------------------------------------------------------------
float TMainForm::GetThroughput( int index )
{
    return  throughput[index];
}

//===========================================================================
void TMainForm::Display_DataSource( void )
{
    // set ADO connection
    as_DataSource = INIFileH.read_DataSource(INI_FILE_NAME, Application->ExeName);
    if( Check_FileExist( as_DataSource ) )
    {
        Set_ADOConnString( as_DataSource );
        edit_database_path->Text = as_DataSource;
    }
}

//---------------------------------------------------------------------------
void TMainForm::Save_DataSource( void )
{
    INIFileH.write_DataSource(INI_FILE_NAME, Application->ExeName, as_DataSource);
}

//==============================================================================
void TMainForm::Display_DBLogOption( void )
{
    AnsiString DbLogEnable = INIFileH.read_DbLogEnable(INI_FILE_NAME, Application->ExeName);
    if( DbLogEnable.AnsiCompareIC("yes") == 0 )
    {
        miEnableFDLog->Checked = true;
    }
    else
    {
        miEnableFDLog->Checked = false;
    }

}

//---------------------------------------------------------------------------
void TMainForm::Save_DBLogOption( void )
{
    if( miEnableFDLog->Checked )
    {
        INIFileH.write_DbLogEnable(INI_FILE_NAME, Application->ExeName, "yes");
    }
    else
    {
        INIFileH.write_DbLogEnable(INI_FILE_NAME, Application->ExeName, "no");
    }
}

//---------------------------------------------------------------------------
bool TMainForm::Get_EnableFactoryDatabaseLog( void )
{
    if( ! miEnableFDLog->Checked )
    {
        return false;
    }
    return true;
}

//===========================================================================
void __fastcall TMainForm::miSetupDataSrcClick(TObject *Sender)
{
    bool ok = dlgOpenDataSrc->Execute();
    if( !ok )
    {
        Application->MessageBox( " Setup factory datbase source file", "Fail", MB_OK );
        return;
    }

    edit_database_path->Text =  dlgOpenDataSrc->FileName;
    as_DataSource = edit_database_path->Text;
    Set_ADOConnString( edit_database_path->Text );
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miEnableFDLogClick(TObject *Sender)
{
    if(  miEnableFDLog->Checked )
    {
        miEnableFDLog->Checked = false;
    }
    else
    {
        miEnableFDLog->Checked = true;
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::miBromDllLogClick(TObject *Sender)
{
    if( miBromDllLog->Checked )
    {
        miBromDllLog->Checked = false;
        Brom_DebugOff();
    }
    else
    {
        miBromDllLog->Checked = true;
        Brom_DebugOn();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miMETADllLogClick(TObject *Sender)
{
    if( miMETADllLog->Checked )
    {
        miMETADllLog->Checked = false;
        SP_META_DebugOff();
    }
    else
    {
        miMETADllLog->Checked = true;
        SP_META_DebugOn();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miClearBromDllLogClick(TObject *Sender)
{
    Brom_DebugClear();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miClearMETADllLogClick(TObject *Sender)
{
    SP_META_DebugClear();
}


//============================================================================
void __fastcall TMainForm::rbNFBClick(TObject *Sender)
{
    m_sDnParam.e_Operation = OPERATION_NFB;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::rbNORClick(TObject *Sender)
{
    m_sDnParam.e_Operation = OPERATION_NOR;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::miSetupClick(TObject *Sender)
{
    frmMultiLoad->Left = 1;
    frmMultiLoad->Top  = 1;
    frmMultiLoad->Show();
}

//===========================================================================
/////////////////////////// Export information  /////////////////////////////
//===========================================================================
S_DN_PARAM_T* TMainForm::Get_DNParameter(void)
{
    return &m_sDnParam;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int TMainForm::Get_TotalFileSize(void)
{
    return m_uiTotalFileSize;
}

//---------------------------------------------------------------------------
void TMainForm::Set_TotalFileSize(unsigned int total_file_size)
{
    m_uiTotalFileSize = total_file_size;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DL_HANDLE_T* TMainForm::Get_DlHandle( int idx )
{
    return g_pDL_HANDLE[idx];
}

//---------------------------------------------------------------------------
void TMainForm::Set_DlHandle(int idx, DL_HANDLE_T* p_dl_handle)
{
    GC_TRACE("<MF> DL handle reset!Addr(0x%x), Value(0x%x)", g_pDL_HANDLE[idx], *g_pDL_HANDLE[idx]);
    g_pDL_HANDLE[idx] = p_dl_handle;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DL_HANDLE_LIST_T* TMainForm::Get_DlHandleList(void)
{
    return &g_DL_HANDLE_LIST;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool TMainForm::Get_CheckTgtResLayoutFlag( void )
{
    return m_sDnParam.s_multibin_op.b_check_target_resource_layout;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool TMainForm::Get_AutoLoadResByScatFlag( void )
{
    return m_sDnParam.s_multibin_op.b_auto_load_resource_file_by_scat;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool TMainForm::Get_CompareResProjectIdFlag( void )
{
    return m_sDnParam.s_multibin_op.b_compare_resource_project_id;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int TMainForm::Get_Type( void )
{
    return cbType->ItemIndex;
}

void __fastcall TMainForm::miOpenAuthFileClick(TObject *Sender)
{
    static AnsiString asAUTH_Path;
    int ret;

    if( edit_auth_path->Text.AnsiCompareIC("") != 0 )
    {
        int iLD = edit_auth_path->Text.LastDelimiter("\\:");
        if( 0 < iLD )
        {
            dlgOpenAuthFile->InitialDir = edit_auth_path->Text.SubString(1, iLD-1 );
        }
    }

    if( dlgOpenAuthFile->Execute() )
    {
        if( S_DONE != (ret=AUTH_Load(g_AUTH_HANDLE, dlgOpenAuthFile->FileName.c_str())) )
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not load Authentication file now.", "FAIL", MB_OK );
            }
            else
            {
                AnsiString as_msg = " Load authentication file \"" + dlgOpenAuthFile->FileName + "\" fail!";
                Application->MessageBox( as_msg.c_str(), "FAIL", MB_OK );

            }
            return;
        }

        edit_auth_path->Text = dlgOpenAuthFile->FileName;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::miEnableNFICS1Click(TObject *Sender)
{
    miEnableNFICS1->Checked = !miEnableNFICS1->Checked;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenMemoryVerificationListFile1Click(
      TObject *Sender)
{
    AnsiString asPath;
    AnsiString as_par;
    getPathFromStr(Application->ExeName, asPath);
    as_par = "\"" + asPath + "Memory_Validation_List_External.xls\"";
    HINSTANCE hinstance = ShellExecute(this->Handle, NULL, "excel.exe", as_par.c_str(), NULL, SW_SHOWNORMAL);
    if (ERROR_FILE_NOT_FOUND == (int)hinstance)
    {
        Application->MessageBox("If you want to open Memory_Validation_List_External.xls, please install Microsoft EXCEL.", "FAIL", MB_OK);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TMainForm::Clear_BackupRestoreStatus(unsigned char idx)
{
    m_stBkStatus[idx]->Caption = "";
}

//---------------------------------------------------------------------------
void TMainForm::Display_BackupRestoreStatus(unsigned char idx, unsigned char total_num, unsigned char current_num)
{
    AnsiString as = IntToStr(current_num) + "/" + IntToStr(total_num);
    m_stBkStatus[idx]->Caption = as;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ScatterFileButtonClick(TObject *Sender)
{
   TButton *btn = (TButton*)Sender;
   try
   {
      if(frmMultiLoad->ScatterFileOpenExecute( (int) btn->Tag ))
      {
        // frmMultiLoad->Save_ScatterAndLoad(false);
      }
   }
   catch(...)
   {
      //int a = 0;
      GC_TRACE("<MF> ScatterFileButtonClick(): Exception happened!");
   }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btDeleteScatterFileClick(TObject *Sender)
{
   TButton *btn = (TButton*)Sender;
   frmMultiLoad->DelLoad( (int) btn->Tag );

   frmMultiLoad->Save_ScatterAndLoad(false);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miAutodetectExtClockClick(TObject *Sender)
{
   miAutodetectExtClock->Checked = true;
   mi13MHz->Checked = false;
   mi26MHz->Checked = false;
   m_asExtClock = (AnsiString) "auto detect";
   m_sDnParam.e_ext_clock = Get_ExternalClockType( m_asExtClock );
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::mi13MHzClick(TObject *Sender)
{
   miAutodetectExtClock->Checked = false;
   mi13MHz->Checked = true;
   mi26MHz->Checked = false;
   m_asExtClock = (AnsiString) "13mhz";
   m_sDnParam.e_ext_clock = Get_ExternalClockType( m_asExtClock );
}

void __fastcall TMainForm::mi26MHzClick(TObject *Sender)
{
   miAutodetectExtClock->Checked = false;
   mi13MHz->Checked = false;
   mi26MHz->Checked = true;
   m_asExtClock = (AnsiString) "26mhz";
   m_sDnParam.e_ext_clock = Get_ExternalClockType( m_asExtClock );
}



void __fastcall TMainForm::OpenFlashBinFile1Click(TObject *Sender)
{
/*
    AnsiString m_asBinFile;

    if(edit_flash_path->Text.Length()>0)
    {
        dlgOpenFlashBinFile->InitialDir = edit_flash_path->Text;
    }

    if( dlgOpenFlashBinFile->Execute() )    // *.bin file
    {
        m_asBinFile = dlgOpenFlashBinFile->FileName;

        edit_flash_path->Text = m_asBinFile;
    }

    return;
*/
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miEbootDllLogClick(TObject *Sender)
{
    if( miEbootDllLog->Checked )
    {
        miEbootDllLog->Checked = false;
        Eboot_DebugOff();
        GC_TRACE_OFF();
    }
    else
    {
        miEbootDllLog->Checked = true;
        Eboot_DebugOn();
        GC_TRACE_ON();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miCllearEbootDllLogClick(TObject *Sender)
{
    Eboot_DebugClear();
    //GC_TRACE_CLEAR();
}

//---------------------------------------------------------------------------
void TMainForm::Update_UI()
{
    //Update single UI component
    for (int i=0; i < MAX_SUPPORT_PORT_NUM; i++)
    {
        //If the thread is running, then start button and
        //COM box should disabled.
        if(_t_manager.Is_running(i))
        {
            Disable_SpecificMsUI(i);
        }
        else
        {
            TimerCOMArray[i]->Enabled = false;
            Enable_SpecificMsUI(i);
        }


    }

    if(_t_manager.Is_all_finish())
    {

        btnScan->Enabled = true;
        btnStartAll->Enabled = true;
        Enable_UIComponent();
        RefreshUIComponent();

    }
    else
    {
        //Some thread is still running
        Disable_UIComponent();
    }

}

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  if(IDYES == Application->MessageBox("Do you want to exit?", "SP Multiport Download Tool", MB_YESNO))
  {
      CanClose = true;
  }
  else
  {
      // IDNO IDCANCLE
      CanClose = false;
  }
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

void __fastcall TMainForm::SetbeginCOMport1Click(TObject *Sender)
{
    dlgSetBeginComPort->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::m_miSettingsClick(TObject *Sender)
{
        Form_tool_settings->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::m_stBkStatusDbClick(TObject *Sender)
{
    TStaticText *stext = (TStaticText*)Sender;

    AnsiString folderpath;
    AnsiString currentfolder = stext->Hint;
    if(currentfolder == "")
    {
        return;
    }

    folderpath = currentfolder;

    //judge folder exist, if not, do nothing, return
    if  (!DirectoryExists(folderpath))
    {
        return;
    }

    AnsiString parameter = "/e," + folderpath;
    ShellExecute(NULL, "open", "explorer.exe", parameter.c_str(), NULL, SW_SHOWNORMAL);

}





void __fastcall TMainForm::miClearAlllogsClick(TObject *Sender)
{
    Brom_DebugClear();
    SP_META_DebugClear();
    GC_TRACE_CLEAR();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::miOpenDeviceManagerClick(TObject *Sender)
{
    //ShellExecute(NULL, "open", "devmgmt.bat", NULL, NULL, SW_SHOWNORMAL);
    ShellExecute(NULL, "open", "devmgmt.msc", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::EnableTwoSiteComponent()
{
    rbUART->Enabled = true;
    rbUSB->Enabled = true;
}

void __fastcall TMainForm::DisableTwoSiteComponent()
{
    rbUART->Enabled = false;
    rbUSB->Enabled = false;
}

void __fastcall TMainForm::miOpenCertFileClick(TObject *Sender)
{
    int ret;

    if( edit_cert_path->Text.AnsiCompareIC("") != 0 )
    {
        int iLD = edit_cert_path->Text.LastDelimiter("\\:");
        if( 0 < iLD )
        {
            dlgOpenCertFile->InitialDir = edit_cert_path->Text.SubString(1, iLD-1 );
        }
    }

    if( dlgOpenCertFile->Execute() )
    {
        if( S_DONE != (ret = SCERT_Load(g_SCERT_HANDLE, dlgOpenCertFile->FileName.c_str())) )
        {
            if( S_FTHND_HANDLE_BUSY_NOW == ret )
            {
                Application->MessageBox( "Program is busy! Can not load certification file now.", "FAIL", MB_OK );
            }
            else
            {
                AnsiString as_msg = " Load certification file \"" + dlgOpenCertFile->FileName + "\" fail!";
                Application->MessageBox( as_msg.c_str(), "FAIL", MB_OK );

            }
            return;
        }

        edit_cert_path->Text = dlgOpenCertFile->FileName;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DeviceManager1Click(TObject *Sender)
{
    ShellExecute(NULL, "open", "devmgmt.msc", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EnableDADLComponent()
{
    m_miDADLFullSpeed->Enabled = true;
    m_miDADLHighSpeed->Enabled = true;
    m_miDADLBattery->Enabled = true;
    m_miDADLNoBattery->Enabled = true;
    m_miDADLAutoDetect->Enabled = true;
}

void __fastcall TMainForm::DisableDADLComponent()
{
    m_miDADLFullSpeed->Enabled = false;
    m_miDADLHighSpeed->Enabled = false;
    m_miDADLBattery->Enabled = false;
    m_miDADLNoBattery->Enabled = false;
    m_miDADLAutoDetect->Enabled = false;
}

void __fastcall TMainForm::m_miEnableDADLClick(TObject *Sender)
{
    if(false == m_miEnableDADL->Checked)
    {
        m_miEnableDADL->Checked = true;
        EnableDADLComponent();
        if( (OP_FORMAT_AND_DOWNLOAD != cbType->ItemIndex) &&
             (OP_FIRMWARE_UPGRADE != cbType->ItemIndex) )
        {
            cbType->ItemIndex = OP_FORMAT_AND_DOWNLOAD;
        }
        //DisableTwoSiteComponent();
    }
    else
    {
        m_miEnableDADL->Checked = false;
        DisableDADLComponent();
    }
}
//---------------------------------------------------------------------------






void __fastcall TMainForm::ManualScanExecute(TObject *Sender)
{
    btnScan->Visible = (btnScan->Visible)?(false):(true);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btnScanClick(TObject *Sender)
{

    int iCycle;
	E_ScanComportType eScanType = UNKNOWN_SCAN_TYPE;
    memset(mChnChecked, 0, MAX_SUPPORT_PORT_NUM * sizeof(bool));

    //Make sure if the customer want to Scan the comport
    if(IDNO== Application->MessageBox("The selected comports will be reset, are you sure to scan comports?", "Scan Comport Warning", MB_YESNO))
    {
        return;
    }

    if (!CheckScatterFile())
    {
        return;
    }
	
    //update DA
    if( UpdateDAInfo() )
    {
       return;
    }

    //Reset checked comport
    Reset_Comport();

    for(iCycle = 0; iCycle < MAX_SUPPORT_PORT_NUM; iCycle++){
        DAPort[iCycle] = -1;
    }

	for(iCycle = 0; iCycle < MAX_SUPPORT_PORT_NUM; iCycle++){
        m_KernelPort[iCycle] = 0;
    }

    //Check all thread status
    for (iCycle = 0; iCycle < MAX_SUPPORT_PORT_NUM; iCycle++)
    {
        if( _t_manager.Is_running(iCycle) )
        {
            Application->MessageBox("Please stop all running channels, then do scan", "FAIL", MB_OK);
            GC_TRACE("<MF> btnScanClick(): Channel%d is running!", iCycle);
            return;
        }

        mChnChecked[iCycle] = cbCOMArray[iCycle]->Checked;
    }

    DisableAllChannelUI();
    Disable_UIComponent();
    btnStartAll->Enabled = false;
    btnScan->Enabled = false;
    //update display for begin scan
    for (int i = 0; i < MAX_SUPPORT_PORT_NUM; i++)
    {
         Set_COMPercentageCaption(i, "");
         pbCOMArray[i]->Progress = 0;
         MainForm->Set_shpCOMColor(i, clBlue);
     }

    //===========================================
	m_asScanComportType = INIFileH.read_ScanComportType(INI_FILE_NAME, Application->ExeName);
	if(m_asScanComportType.AnsiCompareIC("0") == 0){
		eScanType = SCAN_BROM_AND_PRELOADER;
	}else if(m_asScanComportType.AnsiCompareIC("1") == 0){
	    eScanType = SCAN_PRELOADER_ONLY;
	}else{
	    eScanType = UNKNOWN_SCAN_TYPE;
	    Application->MessageBox("Unknow scan comport type, pelease check the ini file!", "FAIL", MB_OK);
        GC_TRACE("<MF> btnScanClick(): Unknow scan comport type, scan type is %s!", m_asScanComportType.c_str());
		return;
	}

    //Create a thread for scanning
    scanStopFlag = false;
    pScanThread = new TScanPort(FALSE, mChnChecked, MAX_SUPPORT_PORT_NUM, eScanType,&scanStopFlag);
    pScanThread->FreeOnTerminate = true;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::m_miEnableDACksmClick(TObject *Sender)
{
    if(false == m_miEnableDACksm->Checked)
    {
        m_miEnableDACksm->Checked = true;
        if( (OP_FORMAT_AND_DOWNLOAD != cbType->ItemIndex) &&
            (OP_FIRMWARE_UPGRADE != cbType->ItemIndex) )
        {
            cbType->ItemIndex = OP_FORMAT_AND_DOWNLOAD;
        }
    }
    else
    {
        m_miEnableDACksm->Checked = false;
    }

    SetDAChecksum(m_miEnableDACksm->Checked);
}
//---------------------------------------------------------------------------

void TMainForm::UpdateChannel(int index, int iBromComNum, int iPreLoaderComNum, bool updateFlag)
{
    AnsiString str;
    if(iBromComNum != 0 && 0==iPreLoaderComNum){
       str.sprintf("COM%2d + N/A", iBromComNum);
    }else if(iBromComNum == 0 && 0 != iPreLoaderComNum){
       str.sprintf("N/A + COM%2d ", iPreLoaderComNum);
	}
    else if(iBromComNum != 0 && 0 != iPreLoaderComNum){
       str.sprintf("COM%2d + COM%2d", iBromComNum,iPreLoaderComNum);
       //becuase bromCom and preloader com has found,so enble this comport check.
       cbCOMArray[index]->Enabled = true;
	}else{
       str.sprintf("N/A + N/A");
    }

    cbCOMArray[index]->Caption = str.c_str();

    if (0!=iBromComNum){
        iCOMNoArray[index] = iBromComNum;
    }

    if (0!=iPreLoaderComNum){
        iPreloaderCOMNoArray[index] = iPreLoaderComNum;
    }

}


//---------------------------------------------------------------------------

void TMainForm::Set6573DefaultChecked(void)
{
    m_miEnableDADL->Checked = true;
    m_miEnableDACksm->Checked = true;
    SetDAChecksum(m_miEnableDACksm->Checked);
    EnableDADLComponent();
    m_miDADLHighSpeed->Checked = true;
    m_miDADLNoBattery->Checked = true;
}

//---------------------------------------------------------------------------

void TMainForm::Set6575DefaultChecked(void)
{
    m_miEnableDADL->Checked = true;
    m_miEnableDACksm->Checked = true;
    SetDAChecksum(m_miEnableDACksm->Checked);
    EnableDADLComponent();
    m_miDADLHighSpeed->Checked = true;
    m_miDADLNoBattery->Checked = true;
}

int TMainForm::GetPollingNumber()
{
	return hasPolling;
}

int TMainForm::GetComNo(unsigned char index, const int comportType)
{
    if (index > MAX_SUPPORT_PORT_NUM - 1)
    {
        GC_TRACE("<MF> GetComNo(): The index is out of bounds!");
        return -1;
    }

    if (comportType == BOOTROM_COMPORT){
        //GC_TRACE("<MF> GetComNo(): iCOMNoArray[%d] is %d!", index,iCOMNoArray[index]);
        return iCOMNoArray[index];
    }
    else if(comportType == PRELOADER_COMPORT)
    {
        //GC_TRACE("<MF> GetComNo(): iPreloaderCOMNoArray[%d] is %d!", index, iPreloaderCOMNoArray[index]);
        return iPreloaderCOMNoArray[index];
    }
    else{
        return 0;
    }
}
void TMainForm::SetComNo(unsigned char index, int value)
{
    if (index > MAX_SUPPORT_PORT_NUM - 1){
        GC_TRACE("<MF> SetComNo(): The index of COM NO Array is out of bounds!");
        return;
    }
   /* wuxiong
    if (rbBootRomDLDA->Checked){
        iCOMNoArray[index] = value;
    }
    else{
        iPreloaderCOMNoArray[index] = value;
    }  */
}

//---------------------------------------------------------------------------


void __fastcall TMainForm::miOpenLogFolderClick(TObject *Sender)
{
        CheckDirectory(pathSettings.getPath(), true);
        CheckDirectory(m_DLL_log_path, true);
	//ShellExecute(NULL, "open", m_DLL_log_path.c_str(), NULL, NULL, SW_SHOWNORMAL);
        ShellExecute(NULL, "open", pathSettings.getPath().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
E_CHIP_GROUP __fastcall TMainForm::CheckChipGroup(BBCHIP_TYPE iChip_type)
{
    E_CHIP_GROUP eGroupRet;
    switch(iChip_type)
    {
       case MT6516_AP:
       case MT6516_MD:
            eGroupRet = BBCHIP_GROUP0;
            break;
       case MT6573:
            eGroupRet = BBCHIP_GROUP1;
            break;
       case MT6575:
       case MT6577:
       case MT6589:
       case MT6572:
       case MT6582:
       case MT8135:
	   case MT6592:	
	   case MT6571:
	   case MT6595:
	   case MT8127:
            eGroupRet = BBCHIP_GROUP2;
            break;
       default:
            eGroupRet = BBCHIP_GROUP2;
            break;
    }
    return eGroupRet;
}

//======================================================================
void __fastcall TMainForm::Save_ScanComportType( void )
{
     INIFileH.write_ScanComportType( INI_FILE_NAME, Application->ExeName, m_asScanComportType);
}

//=======================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void __fastcall TMainForm::Read_DaResetKey( void )
{
    AnsiString as_ResetKey = INIFileH.read_ResetKey( INI_FILE_NAME, Application->ExeName);
    if(as_ResetKey.AnsiCompareIC("PH") == 0){
		m_DaResetKey = 0x68;
    }else if(as_ResetKey.AnsiCompareIC("P") == 0){
        m_DaResetKey = 0x50;
    }else{
        m_DaResetKey = 0x68;
    }
}

void __fastcall TMainForm::Read_EfuseXmlFile( void )
{
    string strEfuseXml;
    AnsiString as_EfuseXmlFile = INIFileH.read_EfuseXmlFilePath( INI_EFUSE_FILE_NAME, Application->ExeName);
	strEfuseXml = as_EfuseXmlFile.c_str();
    m_clsEfuseXmlSetting.SetLoadFilePath(strEfuseXml);
	m_clsEfuseXmlSetting.ParseXML();
}   


void __fastcall TMainForm::Save_DaResetKey( void )
{   AnsiString as_ResetKey;
    if(m_DaResetKey == 0x50){
        as_ResetKey = "P";
	}else{
	    as_ResetKey = "PH";
	}
	
    INIFileH.write_ResetKey( INI_FILE_NAME, Application->ExeName, as_ResetKey);
}


void __fastcall TMainForm::SwithToOperator1Click(TObject *Sender)
{
    if(gLoginMode == LM_OPERATOR){
        return;
    }
   
    if(Identity->SetLoginMode(LM_OPERATOR) == S_DONE){
        OperatorModeSetting();
    }else{
        MessageBox(NULL,"Change to operator mode fail!", "Changing mode", MB_OK);
    }      
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SwitchToEngineer1Click(TObject *Sender)
{
    // Login mode change
    TModalResult mdResult;
    if(gLoginMode == LM_ENGINEER){
        return;
    }
	
    mdResult = Identity->ShowModal();
    if(mdResult== mrOk && Identity->SetLoginMode(LM_ENGINEER) == S_DONE){
        EngineerModeSetting();
    }        
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EngineerModeSetting()
{
	m_EngeerMode->Enabled = true;
    m_OperatorMode->Enabled = false;
    gLoginMode = LM_ENGINEER;
    MainForm->Caption = m_MdtTitle;
    Enable_UIComponent();
    RefreshUIComponent();

}

void __fastcall TMainForm::OperatorModeSetting()
{
    m_OperatorMode->Enabled = true;
    m_EngeerMode->Enabled = false;
    gLoginMode = LM_OPERATOR;
    MainForm->Caption = m_MdtTitle + "(Operator Mode)";
    Disable_UIComponent();
    Enable_ScanBtn();
}



void __fastcall TMainForm::ChangePassword1Click(TObject *Sender)
{
    TModalResult mdResult; 
	mdResult = PasswordSetting->ShowModal();
    if(mdResult== mrOk){
        ShowMessage(AnsiString("Set password successfully!")); 
    }    
}
//---------------------------------------------------------------------------

