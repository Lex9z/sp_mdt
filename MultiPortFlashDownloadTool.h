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
 *   MultiPortFlashDownloadTool.h
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Multiport download form header
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#define _MUTLIPORTFLASHDOWNLOADTOOL_H_
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>


#include "ThreadManager.h"
#include "flashtool_handle.h"
#include "mtk_mcu.h"
#include "download.h"
#include "Multiport_Common.h"
#include "INIFileHandle.h"
#include "CGAUGES.h"
#include <ADODB.hpp>
#include <DB.hpp>
#include <StdCtrls.hpp>

#include "PathSettings.h"

#include "EfuseXMLSetting.h"


#ifndef _TIME_UTILS_H_
#include "time_utils.h"
#endif

#ifndef __SCAN_PORT_H
#include "TScanPort.h"
#endif

//#include "PCMemoryChecksumProgress.h"

#include<string>
using namespace std;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TTimer *TimerCOM1;
        TTimer *TimerCOM2;
        TTimer *TimerCOM3;
        TTimer *TimerCOM4;
        TTimer *TimerCOM5;
        TTimer *TimerCOM6;
        TOpenDialog *dlgOpenDA;
        TTimer *TimerCOM7;
        TTimer *TimerCOM8;
        TGroupBox *gbFormatFATRange;
        TRadioButton *rbManual;
        TButton *btnStartAll;
        TMainMenu *MainMenu1;
        TMenuItem *miFile;
        TMenuItem *OpenDownloadAgentFile1;
        TMenuItem *Help1;
        TMenuItem *OpenScatterFile1;
        TLabel *Label1;
        TMenuItem *miOption;
        TMenuItem *miBasebandchip;
        TMenuItem *miExternalclock;
        TMenuItem *miAutodetectBB;
        TMenuItem *miBasebandchipoption;
        TMenuItem *miCheckECOversion;
        TMenuItem *miAbout;
        TButton *btnStopAll;
        TTimer *TimerCOM9;
        TTimer *TimerCOM10;
        TTimer *TimerCOM11;
        TTimer *TimerCOM12;
        TTimer *TimerCOM13;
        TTimer *TimerCOM14;
        TTimer *TimerCOM15;
        TTimer *TimerCOM16;
        TMenuItem *MultiBindownloadoption1;
        TMenuItem *miCheckTargetResourceLayout;
        TMenuItem *miAutoLoadResourcefilesByScatter;
        TMenuItem *miCompareResourceProjectId;
        TStatusBar *sbMPDT;
        TDataSource *DataSrc;
        TADOTable *ADOTab;
        TADOConnection *ADOConn;
        TMenuItem *miFactoryDatabase;
        TMenuItem *miSetupDataSrc;
        TMenuItem *miEnableFDLog;
        TLabel *Label3;
        TOpenDialog *dlgOpenDataSrc;
        TMenuItem *miDebuglog;
        TMenuItem *miBromDllLog;
        TMenuItem *miMETADllLog;
        TLabel *Label2;
        TMenuItem *miClearBromDllLog;
        TMenuItem *miClearMETADllLog;
        TMenuItem *miAutodetectExtClock;
        TMenuItem *miOpenAuthFile;
        TMenuItem *miEnableNFICS1;
        TMenuItem *OpenMemoryVerificationListFile1;
        TGroupBox *GroupBox3;
        TButton *ScatterFileButton1;
        TStaticText *stScatterFile1;
        TOpenDialog *dlgOpenAuthFile;
        TButton *btDeleteScatterFile1;
        TOpenDialog *dlgOpenFlashBinFile;
        TMenuItem *miEbootDllLog;
        TMenuItem *miCllearEbootDllLog;
        TRadioButton *rbAuto;
        TEdit *edit_da_path;
        TEdit *edit_auth_path;
        TEdit *edit_database_path;
        TMenuItem *mi13MHz;
        TMenuItem *mi26MHz;
        TGroupBox *gbDLCable;
        TRadioButton *rbUART;
        TRadioButton *rbUSB;
        TMenuItem *N1;
        TMenuItem *m_miSettings;
        TMenuItem *N2;
        TMenuItem *miClearAlllogs;
        TMenuItem *N4;
        TEdit *edit_cert_path;
        TLabel *Label5;
        TMenuItem *miOpenCertFile;
        TOpenDialog *dlgOpenCertFile;
        TMenuItem *DeviceManager1;
        TGroupBox *GroupBox1;
        TLabel *COM1Percentage;
        TLabel *COM2Percentage;
        TLabel *COM3Percentage;
        TLabel *COM4Percentage;
        TLabel *COM5Percentage;
        TLabel *COM6Percentage;
        TLabel *COM7Percentage;
        TLabel *COM8Percentage;
        TLabel *lblCOM1Time;
        TLabel *lblCOM2Time;
        TLabel *lblCOM3Time;
        TLabel *lblCOM4Time;
        TLabel *lblCOM5Time;
        TLabel *lblCOM6Time;
        TLabel *lblCOM7Time;
        TLabel *lblCOM8Time;
        TCGauge *pbCOM1;
        TCGauge *pbCOM2;
        TCGauge *pbCOM3;
        TCGauge *pbCOM4;
        TCGauge *pbCOM5;
        TCGauge *pbCOM6;
        TCGauge *pbCOM7;
        TCGauge *pbCOM8;
        TShape *shpCOM1;
        TShape *shpCOM2;
        TShape *shpCOM4;
        TShape *shpCOM5;
        TShape *shpCOM6;
        TShape *shpCOM7;
        TShape *shpCOM8;
        TShape *shpCOM3;
        TButton *btnCOM1Start;
        TButton *btnCOM2Start;
        TButton *btnCOM3Start;
        TButton *btnCOM4Start;
        TButton *btnCOM5Start;
        TButton *btnCOM6Start;
        TButton *btnCOM7Start;
        TButton *btnCOM8Start;
        TCheckBox *cbCOM1;
        TCheckBox *cbCOM2;
        TCheckBox *cbCOM3;
        TCheckBox *cbCOM4;
        TCheckBox *cbCOM5;
        TCheckBox *cbCOM6;
        TCheckBox *cbCOM7;
        TCheckBox *cbCOM8;
        TButton *btnCOM1Stop;
        TButton *btnCOM2Stop;
        TButton *btnCOM3Stop;
        TButton *btnCOM4Stop;
        TButton *btnCOM5Stop;
        TButton *btnCOM6Stop;
        TButton *btnCOM7Stop;
        TButton *btnCOM8Stop;
        TStaticText *m_stBkStatus0;
        TStaticText *m_stBkStatus1;
        TStaticText *m_stBkStatus2;
        TStaticText *m_stBkStatus3;
        TStaticText *m_stBkStatus4;
        TStaticText *m_stBkStatus5;
        TStaticText *m_stBkStatus6;
        TStaticText *m_stBkStatus7;
        TCheckBox *cbSelAllCOM;
        TGroupBox *GroupBox2;
        TLabel *Label6;
        TLabel *m_lblBaudRate;
        TComboBox *cbBaudrate;
        TComboBox *cbType;
        TGroupBox *GroupBox4;
        TPanel *Panel1;
        TRadioButton *m_miDADLFullSpeed;
        TRadioButton *m_miDADLHighSpeed;
        TPanel *Panel2;
        TRadioButton *m_miDADLBattery;
        TRadioButton *m_miDADLNoBattery;
        TMenuItem *m_miEnableDADL;
        TActionList *ActionList1;
        TAction *ManualScan;
        TButton *btnScan;
        TMenuItem *m_miEnableDACksm;
        TCheckBox *cbCOM9;
        TShape *shpCOM9;
        TShape *shpCOM10;
        TShape *shpCOM11;
        TShape *shpCOM12;
        TShape *shpCOM13;
        TShape *shpCOM14;
        TShape *shpCOM15;
        TShape *shpCOM16;
        TLabel *COM9Percentage;
        TLabel *COM10Percentage;
        TLabel *COM11Percentage;
        TLabel *COM12Percentage;
        TLabel *COM13Percentage;
        TLabel *COM14Percentage;
        TLabel *COM15Percentage;
        TLabel *COM16Percentage;
        TCGauge *pbCOM9;
        TCGauge *pbCOM10;
        TCGauge *pbCOM11;
        TCGauge *pbCOM12;
        TCGauge *pbCOM13;
        TCGauge *pbCOM14;
        TCGauge *pbCOM15;
        TCGauge *pbCOM16;
        TLabel *lblCOM9Time;
        TLabel *lblCOM10Time;
        TLabel *lblCOM11Time;
        TLabel *lblCOM12Time;
        TLabel *lblCOM13Time;
        TLabel *lblCOM14Time;
        TLabel *lblCOM15Time;
        TLabel *lblCOM16Time;
        TCheckBox *cbCOM10;
        TCheckBox *cbCOM11;
        TCheckBox *cbCOM12;
        TCheckBox *cbCOM13;
        TCheckBox *cbCOM14;
        TCheckBox *cbCOM15;
        TCheckBox *cbCOM16;
        TButton *btnCOM9Start;
        TButton *btnCOM10Start;
        TButton *btnCOM11Start;
        TButton *btnCOM12Start;
        TButton *btnCOM13Start;
        TButton *btnCOM14Start;
        TButton *btnCOM15Start;
        TButton *btnCOM16Start;
        TButton *btnCOM9Stop;
        TButton *btnCOM10Stop;
        TButton *btnCOM11Stop;
        TButton *btnCOM12Stop;
        TButton *btnCOM13Stop;
        TButton *btnCOM14Stop;
        TButton *btnCOM15Stop;
        TButton *btnCOM16Stop;
        TStaticText *m_stBkStatus8;
        TStaticText *m_stBkStatus9;
        TStaticText *m_stBkStatus10;
        TStaticText *m_stBkStatus11;
        TStaticText *m_stBkStatus12;
        TStaticText *m_stBkStatus13;
        TStaticText *m_stBkStatus14;
        TStaticText *m_stBkStatus15;
        TGroupBox *GroupBox5;
        TMenuItem *miOpenLogFolder;
        TRadioButton *m_miDADLAutoDetect;
        TLabel *Label4;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label10;
        TLabel *Label9;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TCheckBox *m_cbEnableAutoPulling;
        TMenuItem *Identity1;
        TMenuItem *m_EngeerMode;
        TMenuItem *m_OperatorMode;
        TMenuItem *SwithToOperator1;
        TMenuItem *ChangePassword1;
        TMenuItem *SwitchToEngineer1;
        void __fastcall btnSeperateDownloadClick(TObject *Sender);
       // void __fastcall lvClick(TObject *Sender);
       // void __fastcall lvChanging(TObject *Sender, TListItem *Item,
       //   TItemChange Change, bool &AllowChange);
       // void __fastcall lvChange(TObject *Sender, TListItem *Item,
       //   TItemChange Change);
        void __fastcall mOpenDAFileClick(TObject *Sender);
        void __fastcall Openlinkmapfile1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall OpenDownloadAgentFile1Click(TObject *Sender);
        void __fastcall OpenScatterFile1Click(TObject *Sender);
        void __fastcall btnStartAllClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

        void __fastcall TimerCOMTimer(int index);
        void __fastcall TimerCOM1Timer(TObject *Sender);
        void __fastcall TimerCOM2Timer(TObject *Sender);
        void __fastcall TimerCOM3Timer(TObject *Sender);
        void __fastcall TimerCOM4Timer(TObject *Sender);
        void __fastcall TimerCOM5Timer(TObject *Sender);
        void __fastcall TimerCOM6Timer(TObject *Sender);
        void __fastcall TimerCOM7Timer(TObject *Sender);
        void __fastcall TimerCOM8Timer(TObject *Sender);
        void __fastcall miAutodetectBBClick(TObject *Sender);
        void __fastcall miCheckECOversionClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cbCOMClick(TObject *Sender);
        void __fastcall miAboutClick(TObject *Sender);
        void __fastcall cbTypeChange(TObject *Sender);
        void __fastcall btnStopAllClick(TObject *Sender);
        void __fastcall btnSeperateStopClick(TObject *Sender);
        void __fastcall TimerCOM9Timer(TObject *Sender);
        void __fastcall TimerCOM10Timer(TObject *Sender);
        void __fastcall TimerCOM11Timer(TObject *Sender);
        void __fastcall TimerCOM12Timer(TObject *Sender);
        void __fastcall TimerCOM13Timer(TObject *Sender);
        void __fastcall TimerCOM14Timer(TObject *Sender);
        void __fastcall TimerCOM15Timer(TObject *Sender);
        void __fastcall TimerCOM16Timer(TObject *Sender);
        void __fastcall miCheckTargetResourceLayoutClick(TObject *Sender);
        void __fastcall miAutoLoadResourcefilesByScatterClick(TObject *Sender);
        void __fastcall miCompareResourceProjectIdClick(TObject *Sender);
        void __fastcall cbBaudrateChange(TObject *Sender);
        void __fastcall miSetupDataSrcClick(TObject *Sender);
        void __fastcall miEnableFDLogClick(TObject *Sender);
        void __fastcall miBromDllLogClick(TObject *Sender);
        void __fastcall miMETADllLogClick(TObject *Sender);
        void __fastcall rbNFBClick(TObject *Sender);
        void __fastcall rbNORClick(TObject *Sender);
        void __fastcall miSetupClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall miClearBromDllLogClick(TObject *Sender);
        void __fastcall miClearMETADllLogClick(TObject *Sender);
        void __fastcall miOpenAuthFileClick(TObject *Sender);
        void __fastcall miEnableNFICS1Click(TObject *Sender);
        void __fastcall OpenMemoryVerificationListFile1Click(
          TObject *Sender);
        void __fastcall cbSelAllCOMClick(TObject *Sender);
        void __fastcall ScatterFileButtonClick(TObject *Sender);
        void __fastcall btDeleteScatterFileClick(TObject *Sender);
        void __fastcall miAutodetectExtClockClick(TObject *Sender);
        void __fastcall OpenFlashBinFile1Click(TObject *Sender);
        void __fastcall miEbootDllLogClick(TObject *Sender);
        void __fastcall miCllearEbootDllLogClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall mi13MHzClick(TObject *Sender);
        void __fastcall mi26MHzClick(TObject *Sender);
        void __fastcall SetbeginCOMport1Click(TObject *Sender);
        void __fastcall m_miSettingsClick(TObject *Sender);
        void __fastcall m_stBkStatusDbClick(TObject *Sender);
        void __fastcall miClearAlllogsClick(TObject *Sender);
        void __fastcall miOpenDeviceManagerClick(TObject *Sender);
        void __fastcall miOpenCertFileClick(TObject *Sender);

        void __fastcall DeviceManager1Click(TObject *Sender);
        void __fastcall m_miEnableDADLClick(TObject *Sender);

        void __fastcall EnableDADLComponent();
        void __fastcall DisableDADLComponent();

        void __fastcall EnableTwoSiteComponent();
        void __fastcall DisableTwoSiteComponent();
        void __fastcall ManualScanExecute(TObject *Sender);
        void __fastcall btnScanClick(TObject *Sender);
        void __fastcall m_miEnableDACksmClick(TObject *Sender);
        void __fastcall miOpenLogFolderClick(TObject *Sender);
        E_CHIP_GROUP  __fastcall CheckChipGroup(BBCHIP_TYPE iChip_type);
        void __fastcall SwithToOperator1Click(TObject *Sender);
        void __fastcall SwitchToEngineer1Click(TObject *Sender);
		void __fastcall EngineerModeSetting();
		void __fastcall OperatorModeSetting();
        void __fastcall ChangePassword1Click(TObject *Sender);
		
private:	// User declarations
        // common

        AnsiString m_asDAPath;
        AnsiString m_asAutoPollEn;
        AnsiString m_asBBChip;
        AnsiString m_asExtClock;
        AnsiString m_asCheckECOVer;
        AnsiString m_asBaudrate;
        AnsiString m_asType;
		AnsiString m_asOTPOption;
        AnsiString m_asFormat_FAT_range;
        AnsiString m_asPullDownGPIO17;
        bool  isFATFileLoaded;
        bool bCheckECOVersion;
        AnsiString m_asDAFileName;

        AnsiString m_asBLFileName;
		AnsiString m_asScanComportType;
		AnsiString m_asEfuseXmlFilePath;
		unsigned char m_DaResetKey;

        int iCOMChecked;
      //  int iAutoPolling;
        int iAutoPollingCount;
        int hasPolling;


        TShape *shpCOMArray[MAX_SUPPORT_PORT_NUM];

   // multibin
   // bool m_bCheckTgtResLayout;
   // bool m_bAutoLoadResByScat;
   // bool m_bCompareResProjectId;

    ROM_INFO   rom_info[MAX_LOAD_SECTIONS];
    S_LV_CHANGED lvChanged;
    AnsiString as_LVItem[MAX_LOAD_SECTIONS];
    int i_LV_FAT_index;


    // parameter to TBOOT1
    COM_PORT_SETTING m_sComPortSetting;
    S_DN_PARAM_T  m_sDnParam;

    // factory default
    bool m_bStartMetaApp[MAX_SUPPORT_PORT_NUM];
    int  m_iMETA_handle_array[MAX_SUPPORT_PORT_NUM];

    // NFB
    //NFB_PARAM_S  m_sNfbParam;

    // access
    int PassFail[MAX_SUPPORT_PORT_NUM];
    AnsiString  FailReason[MAX_SUPPORT_PORT_NUM];
    unsigned int m_uiTotalFileSize;
    sDownloadTime  dn_time[MAX_SUPPORT_PORT_NUM];
    float throughput[MAX_SUPPORT_PORT_NUM];
    AnsiString as_DataSource;

    // timer
    TTimer *TimerCOMArray[MAX_SUPPORT_PORT_NUM];

    // UI         lblCOM1Time
    TLabel * lbCOMTime[MAX_SUPPORT_PORT_NUM];

	PathSettings pathSettings;
	string m_DLL_log_path;

public:		// User declarations

        int DAPort[MAX_SUPPORT_PORT_NUM];
		int m_KernelPort[MAX_SUPPORT_PORT_NUM];

		bool hasProductInfo;

		bool hasProtectf;

		bool hasProtects;

        CINIFileHandle INIFileH;
        TCheckBox *cbCOMArray[MAX_SUPPORT_PORT_NUM];
     //   TCheckBox *cbAutoPollingArray[MAX_SUPPORT_PORT_NUM];
        TLabel    *COMPercentageArray[MAX_SUPPORT_PORT_NUM];
        TCGauge* pbCOMArray[MAX_SUPPORT_PORT_NUM];
        int DownLoadTimeSec[MAX_SUPPORT_PORT_NUM];
        TButton *btnCOMStartArray[MAX_SUPPORT_PORT_NUM];
        TButton *btnCOMStopArray[MAX_SUPPORT_PORT_NUM];

        TStaticText* m_stScatFileName[MAX_SUPPORT_LOAD_NUM];
        TStaticText *m_stBkStatus[MAX_SUPPORT_PORT_NUM];

        //int DownloadStatus[MAX_SUPPORT_PORT_NUM];
        ThreadManager    _t_manager;

       _BOOL  ScatterEnable;
	   _BOOL Checksum_ver;
        bool  ScatterLoadCompleteFlag;
	    _BOOL  SecroReworkingEnable;
		_BOOL  SecroReworkRunningFlag;
		AnsiString m_MdtTitle;
        // menu
        void __fastcall UpdateNandEraseOptionUi(TObject *Sender);

        __fastcall TMainForm(TComponent* Owner);
        void _fastcall SubClassWndProc(Messages::TMessage &Message);
        bool FileOpenDownloadAgentExecute();
        bool OpenReadDAFile(AnsiString asFileName);
        void Read_FormatFATBeginAddress( void );
        void Read_FormatFATLength(void);
        //------------------------------------------------------------
        void Read_NANDFormatFATBeginAddress( void );
        void Read_NANDFormatFATLength( void );
        void Read_EraseValidation( void );
        int  UpdateDAInfo( void );
        void __fastcall btnCOMDownloadClick( int index );
        void __fastcall lv_oldClick(TObject *Sender);

        //------------------------------------------------------------
        E_OPERATION_T Get_Operation(AnsiString as_Operation);
        E_OPERATION_T Get_Operation(void);
        void Display_Operation( void );
        void Save_Operation( void );
        //------------------------------------------------------------

        BBCHIP_TYPE Get_BBChipType( AnsiString as_BBChip );
        BBCHIP_TYPE Get_BBChipType( void );
        void Display_BasebandChip( void );
        void Save_BasebandChip( void );
        //-------------------------------------------------------------
        EXT_CLOCK Get_ExternalClockType( AnsiString as_ExtClock );
        EXT_CLOCK Get_ExternalClockType( void );
        void Display_ExternalClock( void );
        void Save_ExternalClock( void );
        //-------------------------------------------------------------
        void Get_BBChipOption(S_BBCHIP_OPTION_T& bbchip_op);
        void Display_CheckECOVersion( void );
        void Save_CheckECOVersion( void );
        //-------------------------------------------------------------
        S_MULTIBIN_OPTION Get_MultiBinOption( void );
        void Display_CheckTargetResourceLayout( void );
        void Save_CheckTargetResourceLayout( void );
        //-------------------------------------------------------------
        void Display_AutoLoadResByScat( void );
        void Save_AutoLoadResByScat( void );
        //-------------------------------------------------------------
        void Display_CompareResProjectId( void );
        void Save_CompareResProjectId( void );
        //-------------------------------------------------------------
        void Display_BROMDllLog( void );
        void Save_BROMDllLog( void );
        //-------------------------------------------------------------
        void Display_METADllLog( void );
        void Save_METADllLog( void );
        //-------------------------------------------------------------
        UART_BAUDRATE Get_Baudrate( AnsiString as_baudrate );
        UART_BAUDRATE Get_Baudrate( void );
        void Display_Baudrate( void );
        void Save_Baudrate( void );
        //-------------------------------------------------------------
        void Display_ComEnabled( void );
        void Save_ComEnabled( void );
        //-------------------------------------------------------------
        void Display_COMNoArray( void );
        void Save_COMNoArray( void );
        //-------------------------------------------------------------
        void Display_AutoPollingEnabled( void );
        void Save_AutoPollingEnabled( void );
        //-------------------------------------------------------------
//        void ReadFileSetting( void );
//        void WriteFileSetting( void );
        //-------------------------------------------------------------
        void Display_DAFile( void );
        void Save_DAFile( void );
	 void Save_ScatterFlag( void );
        //-------------------------------------------------------------
        void Display_AuthFile( void );
        void Save_AuthFile( void );
        //-------------------------------------------------------------
        void Display_CertFile( void );
        void Save_CertFile( void );
	 void Save_Checksum_Ver(void);
        //-------------------------------------------------------------
        void Display_Type( void );
        void Save_Type(void);
        //-------------------------------------------------------------
        E_FORMAT_OPTION Get_FormatOption(void);
        void Display_FormatFatRange(void);
        void Save_FormatFatRange(void);
        //-------------------------------------------------------------
        void Display_EbootDllLog( void );
        void Save_EbootDllLog( void );

        void Save_BkAndRsOptional(void);
        void Display_BkAndRsOptional( void );

		void Save_OTPOption();
		void Display_OTPOption();

        void Save_DADLOption(void);
        void Display_DADLOption(void);

        void Save_TwoSiteOption(void);
        void Display_TwoSiteOption(void);

        void  HideScanButton();

		void __fastcall Save_ScanComportType( void );
		void __fastcall Read_DaResetKey( void );
        void __fastcall Save_DaResetKey( void );
        // EMI
        void Read_EMI_Setting(void);
        //RunningTest
        void Read_RunningTest(void);
		void __fastcall Read_ForceWatchdogReset(void);
        void __fastcall Save_ForceWatchDogReset();

		void __fastcall Read_EnableEnterMetaAfterDlFlag(void);
		
       //read efuse xml File path
        void __fastcall TMainForm::Read_EfuseXmlFile( void );
       // void Display_LVSetting( void );
       // void Save_LVSetting( void );
        //-------------------------------------------------------------

        void get_PullDownGPIO17( void );
        //-------------------------------------------------------------
        void Reset_DADownloadAll( void );
        void Reset_DAChecksum( void );
        void Reset_UIComponent( void );

        void Disable_UIComponent( void );
        void Enable_UIComponent( void );
        void RefreshUIComponent(void);
        void Disable_FileMenu( void );
        void Enable_FileMenu( void );
        void Disable_FactoryDatabaseMenu( void );
        void Enable_FactoryDatabaseMenu( void );
        void Disable_OptionMenu( void );
        void Enable_OptionMenu( void );
        void Disable_DebugLoggingMenu( void );
        void Enable_DebugLoggingMenu( void );
        void Disable_lvListView( void );

        void Disable_SetAndDelScatterFile( void );
        void Enable_SetAndDelScatterFile( void );
       // void Enable_lvListView( void );
        void Disable_BasebandChipRadioButton( void );
        void Enable_BasebandChipRadioButton( void );
        void Disable_FormatFatRangeRadioButton( void );
        void Enable_FormatFatRangeRadioButton( void );
        void Disable_TypeComboBox( void );
        void Enable_TypeComboBox( void );
        void Disable_BaudrateComboBox( void );
        void Enable_BaudrateComboBox( void );
        void Disable_AllDownloadButton( void );
        void Enable_AllDownloadButton( void );
        void Disable_ExternalClockRadioButton( void );
        void Enable_ExternalClockRadioButton( void );
        void Disable_SeperateDownloadButton( void );
        void Enable_SeperateDownloadButton( void );
        void Disable_SpecificMsUI(int index);
        void Enable_SpecificMsUI(int index);
        void Start_Timer(int index);
        void Stop_Timer(int index);
        //-------------------------------------------------------------
        void UpdateMenuItem( void );
        // data format transform
        void Update_UI( void );

        // get, set MainForm member
        bool Get_miMT6218BChecked( void );
        bool Set_COMPercentageCaption( int index, AnsiString as_str );
        bool Set_shpCOMColor( int index, TColor color );
        void SetPbComProgress( int index, unsigned char progress_percentage );
        void SetPbComForeColor(int index, TColor color);
        void SetPbComBackColor(int index, TColor color);
        //------------------------------------------------------------
        void Set_StartMetaAppFlag(unsigned char index, bool flag);
        //------------------------------------------------------------
        // download, format
        void Set_DownloadParameter( unsigned char	index );
        bool DownloadCommonFunction( void );

        // multi-bin
        // stop
        void btnCOMStopClick( int i );

        // Global information
        bool Get_CheckTgtResLayout( void );
        bool Get_AutoLoadResByScat( void );
        bool Get_CompareResProjectId( void );
        int  Get_FlashToolFunction( void );
        int  Get_Format_FAT_begin_address( void );
        int  Get_Format_FAT_length( void );


        // database
        // access
        //---------------------------------------------------------------
        // UI
        void Display_DataSource(void);
        void Save_DataSource(void);

        void Display_DBLogOption(void);
        void Save_DBLogOption(void);

        void miAllBBChipUnChecked(void);

        // ADO connection
        void Set_ADOConnLoginPromptOff( void );
        void Set_ADOConnString( AnsiString as_Data_Src );
        bool Check_ADOConnConnected( void );

        void InsertNewRecordToAccess( void );
        void ActivateADOTab( void );
        void DeActivateADOTab( void );
        void PostRecordToAccess( void );
        void SetTabNameToMultiport_download( void );
        void AddStationToAccess( void );
        void AddPcToAccess( void );
        void AddToolTypeToAccess( void );
        void AddDateToAccess( int index );
        void AddTimeToAccess( int index );
        void AddPassFailToAccess(int pass_fail );
        void AddFileSizeToAccess( void );
        void AddDownloadTimeToAccess( int index );
        void AddThrougphputToAccess( int index );
        void AddFailStepToAccess( int fail_step );
        void AddFailReasonToAccess( int fail_reason_code );
        bool AddDnResultToAccess( int index );
        //----------------------------------------------------------------
        // test result
        bool SetPassFail( int index, int pass_fail);
        int GetPassFail( int index );

        // time handling
        bool SetDnDate( int index, TDateTime date );
        TDateTime GetDnDate( int index );
        bool SetDnStartTime( int index, TDateTime time );
        TDateTime GetDnStartTime( int index );
        bool SetDnStartsTime( int index, sTime time );
        sTime GetDnStartsTime( int index );
        bool SetDnEndTime( int index, TDateTime time );
        TDateTime GetDnEndTime( int index );
        bool SetDnEndsTime( int index, sTime time );
        sTime GetDnEndsTime( int index );
        bool SetDnTime( int index );
        unsigned GetDnTime( int index );
        // throughput
        bool SetThroughput( int index );
        float GetThroughput( int index );
        bool Get_EnableFactoryDatabaseLog( void );

        // META_DLL
        void InitMETADll(void);
        void DeInitMETADll(void);


        void clear_download_status(int index);

        // export information
        S_DN_PARAM_T* Get_DNParameter(void);
        unsigned int Get_TotalFileSize(void);
        void Set_TotalFileSize(unsigned int total_file_size);
        DL_HANDLE_T* Get_DlHandle(int idx);
        void Set_DlHandle(int idx, DL_HANDLE_T* p_dl_handle);
        DL_HANDLE_LIST_T* Get_DlHandleList(void);
        bool Get_CheckTgtResLayoutFlag(void);
        bool Get_AutoLoadResByScatFlag(void);
        bool Get_CompareResProjectIdFlag(void);
        int  Get_Type(void);
        void Clear_BackupRestoreStatus(unsigned char idx);
        void Display_BackupRestoreStatus(unsigned char idx, unsigned char total_num, unsigned char current_num);

        //SP cfg
//        void __fastcall get_sp_cfg();
//        AnsiString      m_wcescomm_path;
        int             begin_usb_port;
        int             end_usb_port;
        int             iCOMNoArray[MAX_SUPPORT_PORT_NUM];
		int             iPreloaderCOMNoArray[MAX_SUPPORT_PORT_NUM];
        bool            mChnChecked[MAX_SUPPORT_PORT_NUM];
        //runningtest
        unsigned int m_EnableWdtDogReset;
        unsigned int m_EnableOutputResult;
		unsigned int m_SkipChecksum;
		unsigned int m_FmPreloader;

        //forcewatchdogreset
		unsigned int m_EnableForceWdtDogReset;
		unsigned int m_ForceWdtDogResetTimeout;

		unsigned int m_EnableEnterMetaAfterDL;
				
        void __fastcall StopAllClick();

        bool CheckScatterFile();
        bool CheckCOMNoValid();
        int SetDAChecksum(bool enable);
        //bool VerifyHostBufferBeforeStart();
        bool VerifyHostBufferAfterOP();

        TScanPort * pScanThread;
        //PCMemoryChecksumProgress * mmChecksumProgress;
        bool scanStopFlag;
        void UpdateChannel(int index, int iBromComNum, int iPreLoaderComNum, bool updateFlag);
        void EnableAllChannelUI();
        void DisableAllChannelUI();
        bool GetChecksumEnable();
        // Added by Shaoying Han
        void Set6573DefaultChecked();
        void Set6575DefaultChecked();
        int GetPollingNumber();
        int GetComNo(unsigned char index, const int comportType);
        void SetComNo(unsigned char index, int value);

		void Enable_AutoPulling();
		void Disable_AutoPulling();
        void Disable_ScanBtn(void);
        void Enable_ScanBtn( void );
		void Reset_Comport();
        //efuse
        CEfuseXMLSetting m_clsEfuseXmlSetting;

};

extern TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
