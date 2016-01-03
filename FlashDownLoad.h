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
 *   FlashDownload.h
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Flash download/format thread header file
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
 * Nov 29 2006 mtk00490
 * [STP100001239] [Multiport download tool] Multiport download tool ver 3.1.02
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

//---------------------------------------------------------------------------
#ifndef _FLASHDOWNLOAD_H_
#define _FLASHDOWNLOAD_H_
//---------------------------------------------------------------------------
#include <Classes.hpp>

#ifndef _FLASHTOOL_H_
#include "flashtool.h"
#endif

#ifndef _BROM_H_
#include "brom.h"
#endif

#ifndef _EBOOT_INTERFACE_H_
#include "..\Eboot\interface.h"
#endif

#ifndef	_FLASHTOOL_HANDLE_H_
#include "flashtool_handle.h"
#endif

#ifndef	_MTK_MCU_H_
#include "mtk_mcu.h"
#endif

#ifndef _DOWNLOAD_H_
#include "DOWNLOAD.h"
#endif

#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif

#include <set>
#include<vector>
#include "GCTrace.h"

extern struct DA_HANDLE	*g_DA_HANDLE;
extern RB_HANDLE_T  g_RB_HANDLE;

//#define MAX_WCEUSB_PROT_NUMBER   20
#define PMT_BLOCK_COUNT 2
//using std::vector;
typedef enum
{
    PART_NO_CHANGE = 0,
    PART_HAS_CHANGED,  //partition position
    PART_NOT_FOUND,
    PART_SIZE_HAS_CHANGED,
    PART_SIZE_NO_CHANGED
} PART_LAYOUT_STATUS_E;

typedef enum
{
    FAT_NO_FOUND_IN_SCATTER = 0,
	FAT_CHECKED,
    FAT_UNCHECKED,
    FAT_UNDOWNLOAD

} FAT_STATUS;

typedef enum
{
    FAT_NO_CHANGED = 0,
    FAT_HAS_CHANGED,

} FAT_CHANGED_STATUS;
//---------------------------------------------------------------------------
class TBOOT1 : public TThread
{
private:
         // message handle
        HANDLE hPostMsgDestHandle;
        int ret;

        FLASHTOOL_ARG	m_sFtArg;
        FLASHTOOL_DOWNLOAD_ARG	m_sFtDlArg;
        FLASHTOOL_RESULT  m_sFtResult;
        FLASHTOOL_DOWNLOAD_RESULT   m_sFtDnResult;
        FLASHTOOL_FORMAT_ARG	m_sFmtArg;
        FLASHTOOL_FORMAT_RESULT m_sFmtResult;

        FlashTool_Connect_Arg  m_sConnArg;
        FlashTool_Connect_Result  m_sConnResult;
        FlashTool_Readback_Arg  m_sRBArg;
        FlashTool_Readback_Result  m_sRBResult;

        FLASHTOOL_API_HANDLE_T     m_ft_api_handle;
        RB_HANDLE_T m_hRB;

        Android_Image_File   m_BinRegion;
        char m_BinRegion_path[255];

		Android_Image_File   m_ProductInfo;
        char m_ProductInfo_path[255];

		Android_Image_File   m_Protectf;
        char m_Protectf_path[255];

		Android_Image_File   m_Protects;
        char m_Protects_path[255];

        int m_part_status;
        bool m_is_DA_connected;
		
		bool m_SWSec_FormatDL_Done; 

        //for firmware update
        std::vector<ROM_INFO> m_vctRomInfo;
        std::vector<PART_INFO> m_vctPartInfo;

protected:
        void __fastcall Execute();


public:
        AnsiString m_asMsg;
        AnsiString prefix;
        unsigned char m_ucProgressPercentage;
        TColor _bk_col;
        TColor _ft_col;
        char _hint[512];

        //==============Ready to modify=================
        S_DN_PARAM_T       m_sDnParam;
        COM_PORT_SETTING  m_sComPortSetting;
        int m_iDownloadComportType;

		S_Protected_Image_T m_protected_image;

       __fastcall TBOOT1(bool CreateSuspended, HANDLE hMsgHandle, COM_PORT_SETTING com_port_setting, S_DN_PARAM_T dn_param);

        // ===============UI update & Synchronize function===============
        void _fastcall Start();
        void _fastcall SyncStart();
        void _fastcall StopTimer();
        void _fastcall SyncStopTimer();
        void _fastcall ShowMsg();
        void _fastcall SyncShowMsg();

        void _fastcall UpdateResultOk();
        void _fastcall SyncUpdateResultOk();
        void _fastcall UpdateResultError();
        void _fastcall SyncUpdateResultError();
        void _fastcall EnableMainSpecificMsUI();
        void _fastcall SyncEnableMainSpecificMsUI();
        void _fastcall Update_UI();
        void _fastcall Sync_Update_UI();
	    void _fastcall SetShowString(AnsiString strShow);

        //Progress bar
        void _fastcall InitProgress();
        void _fastcall SyncInitProgress();
        void _fastcall UpdateProgressColor();
        void _fastcall SyncUpdateProgressColor(TColor bk, TColor ft);
        void _fastcall UpdateProgressPercentage();
        void _fastcall SyncUpdateProgressPercentage(int precentage);

        //For firmware upgrade
        void _fastcall UpdateTextHint();
        void _fastcall SyncUpdateTextHint(char * hint);



        //===============Firmware upgrade===============
        int BackupBinRegionData(void);
        int CheckBinRegionData(void);
        int CheckPartitionSizeChange(string strName, vector<PART_INFO> vctPartInfo, vector<ROM_INFO> vctRomInfo);
        int isPartitionSizeChange();

        int WriteBinRegionData();
        int CleanBinRegionData(void);

        int GetPartitionInfo(const char * name, PART_INFO & ptInfo);
        int GetAllPartitionInfo(void ** pArray, unsigned int &size);
        int ReadbackBinRegion(RB_INFO &rb_info, AnsiString & as_backup_dir,const char* BackupName, const char* ChecksumName);
        int CheckBkRsParentFolder(char * parentName);
        int CheckBackupFolder(AnsiString & as_backup_dir);
		int CheckBackupFolderBeforeBackup();
        bool CheckBinRegionFileExisted(const char * path);
		bool CheckProductInfoFileExisted(const char * path);
		//bool CheckProtectfFileExisted(const char * path);
		//bool CheckProtectsFileExisted(const char * path);
        bool RemoveAllFilesInFolder(char * path);

        bool FindStr(const char *pSrcStr, const char *pDstStr );
		bool FindPartName(string strSrc, vector<string> vctDstStr );
		void InitVectorForFindPartName(string strName, vector<string> &vctDstTemp );
		
        int GetROMInfo(const char * name, ROM_INFO & romInfo);
        int GetAllROMInfo(void ** pArray, unsigned int &size);
        int GetRomInfoData();
        int GetPartitionInfoData();
        int IsPartitionLayoutChanged(int & status);
		//bool IsHaveProductInfo();
		//bool IsHaveProtectf();
		//bool IsHaveProtects();
		int IsHaveProtectRegion();
		FAT_STATUS IsFATChecked();
		FAT_CHANGED_STATUS IsFATChanged();
        bool CheckPartitionAddr(const char * name, PART_INFO * pOri, int sizeOri, ROM_INFO * pNew, int sizeNew);
        bool IsPartAddrChange(string strName, vector<PART_INFO> vctPartInfo, vector<ROM_INFO> vctRomInfo);

        bool ComputeCheckSum(const AnsiString &dataPath, unsigned char &cksm);
        bool LoadCheckSum(const AnsiString &cksmPath, unsigned char &cksm);
        bool SaveCheckSum(const AnsiString &cksmPath, const unsigned char &cksm);

        void SetBackupProgressColor();
        void SetRestoreProgressColor();

        void ConvertRIDtoString(unsigned char * raw, unsigned int len, char * str);
        //for check bin region existing info when firmware upgrade
        int CheckBinRegionBeforeRestore(void);
        bool CheckIsFoldExist(AnsiString as_backup_dir);
        bool CheckIsFileExist(AnsiString as_file_path);
        bool ComputeBinfileCheckSum(AnsiString asBinFilePath, AnsiString asChecksumFilePath);
        AnsiString GetBackupFolderName(AnsiString &as_backup_folder);
        bool CreateFolder(AnsiString as_create_dir);

        //===============Setting===============
        void BootArgSetting(BOOT_FLASHTOOL_ARG &boot_arg);
        void ResetArgMemory(void);
        void FlashtoolArgSetting(void);
        void ReadbackArgSetting();
        void DownloadArgSetting(void);
        void FormatArgSetting(void);
		int __fastcall UpdateBMTCount();

        //===============Ports===============
        int CheckPortAvailable();
        int CheckPortUnavailable();
        int CheckUSBPortType(int comNo);
        int CheckUARTPortType(int comNo);

        int  GetCOMPortIndex();
        int  GetCOMPortNumber();
        int  GetCOMPortNumByComType(const int iComportType);
        void UpdateCOMPortNumber(const int iComport);

        bool GetSymbolicName(char * sz_port_number, char * sz_symbol_name);

        std::set<int>   m_exist_com_port;
        std::set<string>   m_exist_com_port_str;
        bool GetNewPortNumber( int &usb_com_port );
        void ScanPortNumberByRegistry(std::set<int>  &exist_com_ports /*out*/);
        void ScanSymbolicNameByDDK( std::set<string>  &exist_com_ports_str /*out*/ );
        bool GetNewPortSymbolicName( string &usb_com_port_str );
        void ScanFriendlyNameByDDK(std::set<string>  &exist_com_ports_str /*out*/);
        bool GetNewPortFriendlyName( string &usb_com_port_str );


        //===============Main operations===============
        int __fastcall FormatAndDownload(bool bTodoDisconnectDA = true);
        int __fastcall DownloadOnly();
        int __fastcall FormatAll();
        int __fastcall FormatBootloader();
        int __fastcall FirmwareUpgrade(bool bTodoDisconnectDA = true);
		int __fastcall LayoutNoChange();
		int __fastcall LayoutChange();
		int __fastcall LayoutNotFound();
		
		int __fastcall OTPLock();
		int __fastcall OTPFormat();
		int __fastcall FirmwareUpgradeAndOTPLock();
		
		int __fastcall DownloadSecro(bool FormatEnable);

        int __fastcall DL_Android_All();
        int __fastcall DL_Android_Partial();
        int __fastcall FM_Android_All(bool bToDoDisconnet = true);
		int __fastcall FM_Android_All_ExeceptOTP();
        int __fastcall FM_Android_Partial();
        int __fastcall FM_Android_Partial_ExeceptOTP();
        int __fastcall FM_Android_FAT();
        int __fastcall FM_Android_Bootloader();
        int __fastcall DL_Android_FirstSite();
        int __fastcall DL_Android_SecondSite();
        int __fastcall TwoSiteDownload();

        int __fastcall DAConnect();
        int __fastcall DAFullSpeedDownload();
        int __fastcall DAHighSpeedDownloadWithoutReset();
        int __fastcall DAFullSpeedDownloadWithoutReset();
        bool __fastcall IsNeedBoundaryCheck();
        bool __fastcall GetRegionInfo(Android_Download_ARG & dl_arg);
        bool            DownloadImagesByEboot(void);
        bool            DownloadAndRestoreBinRegionByEboot( void );

		int __fastcall SwitchComport();
        int __fastcall DisConnectWithDA();

		//===============watchdogreset=============================
        int __fastcall  DoWatchDogReset(bool bReboot2Charger = false);
        int __fastcall  WatchDogResetForAutoTest();
		int __fastcall  ForceWatchDogReset();
		
        void _fastcall  OutputFailReport(int ret);

        //===============combo emmc=========================
		int __fastcall CreateFormatList_For_LayoutNotChange(vector<PART_INFO> &vctPartInfo);
		int __fastcall FM_ComboEmmc_Partial(FLASHTOOL_API_HANDLE_T ft_api_handle);

		
        int __fastcall CreateFormatAllList(vector<PART_INFO> &vctPartInfo);
        int __fastcall FM_ComboEmmc_ALL(FLASHTOOL_API_HANDLE_T ft_api_handle);
		
        int __fastcall CheckEmmcType(DL_HANDLE_T dl_handle, int &type);
        int __fastcall FM_Part_Item(FLASHTOOL_API_HANDLE_T ft_api_handle,PART_INFO * partinfo, int iEraseFlag);

	
		
        //===============Depreciated functions===================
        //Firmware upgrade
        int _fastcall                 DoPriorOperation(void);
        int _fastcall                 DoPostOperation(void);
        int _fastcall                 CheckBackupLocation(void);
        int _fastcall                 BackupNVRAMData(void);
        int _fastcall                 RestoreNVRAMData(void);
        bool                          CheckBackupDataFileExisted(char * path);

        // META related
        int                           PrepareMETAOperation(void);
        int                           TerminateMETAOperation(void);
        EBOOT_RESULT                  EnterIntoMETAMode();
        void                          ExitMETAMode( int meta_handle );
        int                           ConnectInMetaModeWrapper(const int meta_handle);
        bool _fastcall                Query_BackupNVRAMData_Support(void);

		//PC memory checksum
		int                           VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult);
		bool                          VerifyHostBufferBeforeStart();
		void                          InitChecksumArgs(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult);

        // error message
        void __fastcall               GetErrorMessag(STATUS_E id, unsigned int m_msp_error_code);

        //efuse related
        int __fastcall                WriteEfuse();
		int __fastcall                ReadEfuseByDA();
        int __fastcall                WriteEfuseByDA();
		int __fastcall                WriteEfuseByBrom();
        E_EFUSE_WRITE_TYPE __fastcall GetWriteEufseType(BBCHIP_TYPE chip_type);
        int __fastcall                GetSecureFilePath(string &strSecureLockPath, string &strSecureSettingPath, string &strReadbackPath);

	   int __fastcall                 EnterMetaMode();
	   int __fastcall                 EnterMetaModeAfterDL();

};
//---------------------------------------------------------------------------
#endif
