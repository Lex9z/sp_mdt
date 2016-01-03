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
 *   form_MultiLoad.h
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  Multi-load form header file
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

#ifndef _FORM_MULTILOAD_H_
#define _FORM_MULTILOAD_H_
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <string>
#include <vector>
#include <map>

#include "ImageDetailInfo.h"

#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif

#ifndef _INIFILEHANDLE_H_
#include "INIFileHandle.h"
#endif

// parameter
#ifndef _PARAM_H_
#include "param.h"
#endif

#ifndef _DOWNLOAD_H_
#include "DOWNLOAD.h"
#endif

#include "Eboot\interface.h"

#include "GCTrace.h"
#include "INIFileHandle.h"
#include "NewAddedImages.h"

typedef enum{
  BT_EBOOT,
  BT_UBOOT,
  BT_UNKONW,
}BT_Type;

typedef struct
{
    char scatter_name[64];
}SCATTER_NAME;

typedef struct
{
    int seq;
    unsigned short pos;
}IMAGE_ATTR;

typedef struct
{
    DM_IMG_FORMAT format;
    DM_IMG_TYPE type;
}FORMATTYPE;

typedef std::pair<SCATTER_NAME, IMAGE_ATTR> ImageAttrPair;
typedef std::pair<SCATTER_NAME, FORMATTYPE> FMTPPair;
typedef std::vector<ImageAttrPair> ImageAttrVector;
typedef std::vector<FMTPPair> FMTPVector;
typedef std::pair<string, int> CksmPair;
typedef std::vector<CksmPair> CksmVector;

//Android image name for scatter file
const char SCATTER_PRELOADER_NAME[100] = "PRELOADER";
const char SCATTER_DSPBL_NAME[100] = "DSP_BL";
const char SCATTER_UBOOT_NAME[100] = "UBOOT";
const char SCATTER_LK_NAME[100] = "LK";
const char SCATTER_BOOTIMG_NAME[100] = "BOOTIMG";
const char SCATTER_SECRO_NAME[100] = "SEC_RO";
const char SCATTER_ANDROID_NAME[100] = "ANDROID";
const char SCATTER_LOGO_NAME[100] = "LOGO";
const char SCATTER_RECOVERY_NAME[100] = "RECOVERY";
const char SCATTER_USERDATA_NAME[100] = "USRDATA";
const char SCATTER_CUSTPACK1_NAME[100] = "CUSTPACK1";
const char SCATTER_CUSTPACK2_NAME[100] = "CUSTPACK2";
const char SCATTER_CUSTPACK3_NAME[100] = "CUSTPACK3";
const char SCATTER_CUSTPACK4_NAME[100] = "CUSTPACK4";

const char SCATTER_REGION_HIDDEN[100] = "__NODL";

//product info part
const char SCATTER_PRODUCT_INFO_NAME[100] = "__NODL_PRO_INFO";
const char SCATTER_PRODUCT_INFO_FOR_GPT_NAME[100] = "proinfo";//begin with MT6595, because of the name rule of gpt

//nvram part
const char SCATTER_NVRAM_NAME1[100]  = "__NODL_NVRAM";
const char SCATTER_NVRAM_NAME2[100]  = "NVRAM";
const char * const SCATTER_NVRAM_NAME = SCATTER_NVRAM_NAME1;

//protect part
const char SCATTER_PROTECTF_NAME[100] = "__NODL_PROTECT_F";
const char SCATTER_PROTECTS_NAME[100] = "__NODL_PROTECT_S";
const char SCATTER_PROTECT1_NAME[100] = "protect1";  //begin with MT6595, because of the name rule of gpt
const char SCATTER_PROTECT2_NAME[100] = "protect2";  //begin with MT6595, because of the name rule of gpt

const char SCATTER_PERSIST_NAME[100]  = "persist";   //begin with MT6595, because of the name rule of gpt


const char SCATTER_SECCFG_NAME[100] = "__NODL_SECCFG";
const char SCATTER_MISC_NAME[100] = "__NODL_MISC";
const char SCATTER_EXPDB_NAME[100] = "__NODL_EXPDB";
const char SCATTER_CACHE_NAME[100] = "__NODL_CACHE";
const char SCATTER_BMTPOOL_NAME[100] = "__NODL_BMTPOOL";
//gpt
const char SCATTER_PGPT_NAME[100]  = "pgpt";


const char SCATTER_FAT_NAME[100] = "__NODL_FAT";


const char SCATTER_NODL_PMT_NAME[] = "__NODL_PMT";

const char SCATTER_OTP_NAME[100] = "__NODL_RSV_OTP";
const char SCATTER_RSV_BMT_NAME[100] = "__NODL_RSV_BMTPOOL";

//eMMC added
const char SCATTER_DL_MBR_NAME[] = "MBR";
const char SCATTER_DL_EBR1_NAME[] = "EBR1";
const char SCATTER_DL_EBR2_NAME[] = "EBR2";
const char SCATTER_DL_CACHE_NAME[] = "CACHE";
const char SCATTER_DL_FAT_NAME[] = "FAT";

//image file prefix for auto load
const char AUTO_PRELOADER_PREFIX[100] = "preloader";
const char AUTO_DSPBL_PREFIX[100] = "DSP_BL";
const char AUTO_UBOOT_PREFIX[100] = "uboot";
const char AUTO_UBOOT_OLD_PREFIX[100] = "u-boot";
const char AUTO_LK_PREFIX[100] = "lk";
const char AUTO_BOOTIMG_PREFIX[100] = "boot";
const char AUTO_SECRO_PREFIX[100] = "secro";
const char AUTO_ANDROID_PREFIX[100] = "system";
const char AUTO_LOGO_PREFIX[100] = "logo";
const char AUTO_RECOVERY_PREFIX[100] = "recovery";
const char AUTO_USERDATA_PREFIX[100] = "userdata";
const char AUTO_CUSTPACK1_PREFIX[100] = "custpack1";
const char AUTO_CUSTPACK2_PREFIX[100] = "custpack2";
const char AUTO_CUSTPACK3_PREFIX[100] = "custpack3";
const char AUTO_CUSTPACK4_PREFIX[100] = "custpack4";
//eMMC added
const char AUTO_MBR_PREFIX[] = "MBR";
const char AUTO_EBR1_PREFIX[] = "EBR1";
const char AUTO_EBR2_PREFIX[] = "EBR2";
const char AUTO_CACHE_PREFIX[] = "cache";
const char AUTO_FAT_PREFIX[] = "fat";

//TEE
const char SCATTER_TZ_TEE1_NAME[100]  = "TEE1";
const char SCATTER_TZ_TEE2_NAME[100]  = "TEE2";
const char AUTO_TEE1_PREFIX[100] = "tz";
const char AUTO_TEE2_PREFIX[100] = "tz";


const char CHIP_NAME_MT6573[10] = "MT6573";

using namespace std;

//---------------------------------------------------------------------------
class TfrmMultiLoad : public TForm
{
__published:	// IDE-managed Components
        TStaticText *lblScatFileName1;
        TOpenDialog *dlgOpenScatter1;
        TOpenDialog *dlgOpenROM1;
        TOpenDialog *dlgOpenFAT1;
        TListView *lv1;
        TStatusBar *sbMultiLoad;
        TButton *btnScatFile1;
        TButton *btnDelLoad1;
        TOpenDialog *dlgOpenScatter2;
        TOpenDialog *dlgOpenScatter3;
        TOpenDialog *dlgOpenScatter4;
        TOpenDialog *dlgOpenROM3;
        TOpenDialog *dlgOpenROM4;
        TOpenDialog *dlgOpenROM2;
        TOpenDialog *dlgOpenFAT2;
        TOpenDialog *dlgOpenFAT3;
        TOpenDialog *dlgOpenFAT4;
        TListView *lvPar1;
        TOpenDialog *dlgOpenSecRo;
        TOpenDialog *m_dlgOpenGPS;



        void __fastcall lvChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
        void __fastcall lvChanging(TObject *Sender, TListItem *Item,
          TItemChange Change, bool &AllowChange);
        void __fastcall lvClick(TObject *Sender);
        void __fastcall btnScatFileClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnDelLoadClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall lvParamChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
        void __fastcall lvParamChanging(TObject *Sender, TListItem *Item,
          TItemChange Change, bool &AllowChange);
        void __fastcall lvParamClick(TObject *Sender);

        void __fastcall DelLoad(int idx);

private:	// User declarations
        // file
        TStaticText* m_lblScatFileName[MAX_SUPPORT_LOAD_NUM];
        TOpenDialog* m_dlgOpenScatter[MAX_SUPPORT_LOAD_NUM];
        TOpenDialog* m_dlgOpenROM[MAX_SUPPORT_LOAD_NUM];
        TOpenDialog* m_dlgOpenFAT[MAX_SUPPORT_LOAD_NUM];

        AnsiString m_asScatPath[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_asScatFileName[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_asROMPath[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_asROMFileName[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_asFATPath[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_asFATFileName[MAX_SUPPORT_LOAD_NUM];

        AnsiString m_xldr_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_eboot_path[MAX_SUPPORT_LOAD_NUM];

        AnsiString m_xldr_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString m_eboot_ini_path[MAX_SUPPORT_LOAD_NUM];

        _BOOL       b_xlder_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_eboot_checked[MAX_SUPPORT_LOAD_NUM];

        AnsiString       m_preloader_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_DSPBL_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_uboot_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Logo_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_TEE1_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_TEE2_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Boot_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Secro_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Recovery_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_AndroidSystem_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_UserData_path[MAX_SUPPORT_LOAD_NUM];

        //For cust packet
        AnsiString       m_Custpack1_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack2_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack3_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack4_path[MAX_SUPPORT_LOAD_NUM];
		//eMMC Added
		AnsiString       m_MBR_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_EBR1_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_EBR2_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_cache_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_FAT_path[MAX_SUPPORT_LOAD_NUM];

		//eMMC Added
		AnsiString       m_MBR_ini_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_EBR1_ini_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_EBR2_ini_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_cache_ini_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_FAT_ini_path[MAX_SUPPORT_LOAD_NUM];

        AnsiString       m_preloader_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_DSPBL_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_uboot_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Logo_ini_path[MAX_SUPPORT_LOAD_NUM];
	    AnsiString       m_TEE1_ini_path[MAX_SUPPORT_LOAD_NUM];
		AnsiString       m_TEE2_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Boot_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Secro_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Recovery_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_AndroidSystem_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_UserData_ini_path[MAX_SUPPORT_LOAD_NUM];

        //For cust packet
        AnsiString       m_Custpack1_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack2_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack3_ini_path[MAX_SUPPORT_LOAD_NUM];
        AnsiString       m_Custpack4_ini_path[MAX_SUPPORT_LOAD_NUM];

		AnsiString m_SecroScatter_path; 
		AnsiString m_Secro4Reworking_path; 

        _BOOL       b_preloader_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_DSPBL_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_uboot_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Logo_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_TEE1_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_TEE2_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Boot_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Secro_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Recovery_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_AndroidSystem_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_UserData_checked[MAX_SUPPORT_LOAD_NUM];

        //For cust packet
        _BOOL       b_Custpack1_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Custpack2_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Custpack3_checked[MAX_SUPPORT_LOAD_NUM];
        _BOOL       b_Custpack4_checked[MAX_SUPPORT_LOAD_NUM];

		//eMMC added
		_BOOL       b_MBR_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_EBR1_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_EBR2_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_cache_checked[MAX_SUPPORT_LOAD_NUM];
		_BOOL       b_FAT_checked[MAX_SUPPORT_LOAD_NUM];


        // ini file
         CINIFileHandle m_INIFileH;

        // form
        bool Display_Ro_File(unsigned int index);
        bool Display_GPS_File(unsigned int index);

        // List view
        TListView *m_lv[MAX_SUPPORT_LOAD_NUM];
        TListView *m_lvPar[MAX_SUPPORT_LOAD_NUM];
        int m_iLV_FAT_index[MAX_SUPPORT_LOAD_NUM];
        S_LV_CHANGED m_slvChanged[MAX_SUPPORT_LOAD_NUM];
        S_LV_CHANGED m_sLvParamChanged[MAX_SUPPORT_LOAD_NUM];

        // parameter
        S_PARAMETER_DOWNLOAD_CONFIG m_sParameter[MAX_SUPPORT_LOAD_NUM];
        // handle
        //DL_HANDLE_T m_TempDlHandle;

		NewAddedImages addedImages;
		std::map<string, ImageDetailInfo*> *imageMap;

public:		// User declarations
        __fastcall TfrmMultiLoad(TComponent* Owner);
       // bool FileOpenDownloadAgentExecute( void );
       // bool OpenReadDAFile( AnsiString asFileName );
        bool ScatterFileOpenExecute(int idx);
         // bool FileOpenScatExecute( int idx );
	    bool GetPlatformInfoAndBmtBlock(AnsiString asMapFileName, DL_HANDLE_T *p_dl_handle,BOOL checksumFileFlag);
	    void ForceDLWithBattery(); //MT6595 not support without battery
        bool OpenReadScatFile(AnsiString asMapFileName, int idx, DL_HANDLE_T *p_dl_handle );
		bool OpenReadScatFile4SecroReworking(AnsiString asMapFileName, int idx, DL_HANDLE_T *p_dl_handle );
        void __fastcall RedrawLVList( int idx, DL_HANDLE_T *p_dl_handle );
	 void UpdateRawLVListAndSaveRomInfo(DL_HANDLE_T *p_dl_handle,AnsiString scatter_file);
        void __fastcall RedrawLVParamList( int idx, DL_HANDLE_T *p_dl_handle );
        void __fastcall RedrawLVCheckBox( int idx );
        void __fastcall lv_oldClick(TObject *Sender);
        void __fastcall Listview_ReplaceFile(TObject *Sender);

        int GetROMIndex(const char* rom_name, const DL_HANDLE_T dl_handle);

        void RedrawTypeComBox();
        // DL handle
        bool Destroy_DLHandle( DL_HANDLE_T *p_dl_handle );
        void initial_dlg_open_dir(int index, AnsiString dir);

       void Display_LoadScatterAndBinFile(void);
	void Display_ScatterAndLoad( void );
	void LoadSecroScatter(AnsiString scatterfilePath);
        void Save_ScatterAndLoad( bool is_redraw = true );
	 void Save_ScatterInfo(bool is_redraw= true );

        //Add load scatter file checksum
        bool IsHostBufferCksmFileExisted();
        void LoadHostBufferStdCksm();
	  bool GetCheckSumVer();//mtk71502,20130415
	  bool MD5Checksum();
	   bool MD5CheckSumVerfy(AnsiString &path,string &checksum);// erjing,20130413
        //int VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult);
        CksmVector m_cksmVt;
        CINIFileHandle m_iniFile;

        bool  Load_Rom(DL_HANDLE_T * dl_handle, int index, AnsiString rom_path);
        bool Update_Handle_List(unsigned int index, DL_HANDLE_T ** pp_temp_dl_handle,
                                DL_HANDLE_T ** pp_dl_handle, int &ret);

        // parameter
        bool __fastcall LoadFileInfo(const char *pathname, unsigned int *p_nFileLength, long *p_last_modify_time);
        bool __fastcall LoadFile(const char *pathname, unsigned char **pp_buf, unsigned int *p_nFileLength, long *p_last_modify_time);
        void __fastcall LoadSecRO(int idx);
        void __fastcall LoadGPS(int idx);

/*
        bool search_uboot_files(AnsiString folder,AnsiString & rom_file);
        bool is_uboot_file(string file);
        bool search(string folder,vector<string>  &m_p_files);
        bool search_preloader_files(AnsiString folder,AnsiString & rom_file);
        bool is_preloader_file(string file);
        bool search_preloader(string folder,vector<string>  &m_p_files);
        */
        bool search_image_files(AnsiString folder, AnsiString & rom_file, AnsiString filetype);
        bool doSearch(string folder, vector<string>  &m_p_files, string filetype);
        bool is_prefix_wanted(string file, string filetype);

        // share information
        S_PARAMETER_DOWNLOAD_CONFIG Get_Parameter(void);
        bool Display_FAT_Info(unsigned int idx, DL_HANDLE_T * p_temp_dl_handle);
        bool Auto_Load_Scatter_File(unsigned int idx, int &ret, DL_HANDLE_T * p_temp_dl_handle);
        void SetScatterFileCaption(int idx, AnsiString  text);

        bool get_GPS_gpio_information_from_GPS_ini(int index, AnsiString gps_dir,
                                                   AnsiString gps_rom,
                                                   AnsiString gps_ini);
        bool read_GPS_gpio_information_from_GPS_ini(int index, AnsiString gps_ini);

        void __fastcall Redraw_LV_SEC_RO_ParamList(int idx, TListItem *pLI);
        void __fastcall Redraw_GPS_ROM(int idx, TListItem *pLI);

        unsigned int calc_buf_len_by_file_len( unsigned int file_len);

        void Auto_load_uRoms(string scatter_file, DL_HANDLE_T *p_dl_handle);

		void BromDLTypeTblEntry(DL_HANDLE_T* p_dl_handle);

        BT_Type get_bt_type(){return _bootloader_type;};

        Android_Image_File   m_image_files[ANDROID_IMAGE_FILE_NUMBER];
        Android_Image_File   m_frm_bin_region;
        char auth_path[255];
        char m_image_path_memory[ANDROID_IMAGE_FILE_NUMBER][256];
        _BOOL b_SCATTER_checkEnable;
	    _BOOL   b_checksum_ver;
        ImageAttrVector m_attr_vector;
        FMTPVector m_fmtp_vector;

		//Scatter file info
		SCATTER_Head_Info m_scatter_info;

        //void Remove_Other_Rom_File( bool &is_need_dl_uboot, bool &is_need_dl_image );
        //void Restore_Rom_File();
        //void __fastcall Set_Android_Image_Info();

        int FindImageAttrVector(char * name, FORMATTYPE & fmtp);
        int FillDownloadImageInfo();
        int DisableDLAttribute();
        int RestoreDLAttribute();
        bool IsDLAnyImage();
        bool IsDLBootloader();
        bool IsDLSystem();
        void MakeImageAttrVector();
        ULL64 GetUbootStartAddr();
		int AutoLoadSecroSignImg(DL_HANDLE_T *p_dl_handle, string strPath);

        BBCHIP_TYPE m_chip_type;
        unsigned int m_BMT_blocks;
        bool m_bPlatformChange;
        HW_StorageType_E m_Storage_Type;
        unsigned int m_Load_End_Address;

private:

        char uboot_path[255];
        char logo_path[255];
        char bootimg_path[255];
        char recovery_path[255];
        char secro_path[255];
        char android_path[255];
        char usedata_path[255];
        char custpack1_path[255];
        char custpack2_path[255];
        char custpack3_path[255];
        char custpack4_path[255];

        BT_Type _bootloader_type;
		AnsiString iniFilePath;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMultiLoad *frmMultiLoad;
//---------------------------------------------------------------------------
#endif
