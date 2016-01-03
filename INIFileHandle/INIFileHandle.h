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
 *   INIFileHandle.h
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  INI file handling header file
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
#ifndef _INIFILEHANDLE_H_
#define _INIFILEHANDLE_H_
#include <vector.h>
#include "flashtool_handle.h"


#define COM_PORT_UART_STR "UART COM"
#define COM_PORT_USB_STR "USB COM"

class CINIFileHandle
{
public:

    /*---------------------------------------------------------------------------*/
    int read_BkRs_Action(AnsiString asSetupFile,AnsiString asExeName);
    void write_BkRs_Action(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_BkRs_FolderName(AnsiString asSetupFile,AnsiString asExeName);
    void write_BkRs_FolderName(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    bool read_BkRs_Deletion(AnsiString asSetupFile,AnsiString asExeName);
    void write_BkRs_Deletion(AnsiString asSetupFile, AnsiString asExeName, bool iCheck);
    /*---------------------------------------------------------------------------*/
    int read_TwoSite_Enable(AnsiString asSetupFile,AnsiString asExeName);
    void write_TwoSite_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_TwoSite_Location(AnsiString asSetupFile,AnsiString asExeName);
    void write_TwoSite_Location(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_DADL_Enable(AnsiString asSetupFile,AnsiString asExeName);
    void write_DADL_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_DACksm_Enable(AnsiString asSetupFile,AnsiString asExeName);
    void write_DACksm_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_DADL_Speed(AnsiString asSetupFile,AnsiString asExeName);
    void write_DADL_Speed(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    int read_DADL_Battery(AnsiString asSetupFile,AnsiString asExeName);
    void write_DADL_Battery(AnsiString asSetupFile, AnsiString asExeName, int iCheck);
    /*---------------------------------------------------------------------------*/
    AnsiString read_field(AnsiString asSetupFile,AnsiString asExeName, AnsiString asSecName, AnsiString asFieldName, AnsiString asDefStr);
    void write_field(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString asFieldName, AnsiString asStr);
    //-----------------------------------------------------------------------------------
    AnsiString read_Operation(AnsiString asSetupFile,AnsiString asExeName);
    void write_Operation(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Operation);
    //-----------------------------------------------------------------------------------
    AnsiString read_ComPortType(AnsiString asSetupFile,AnsiString asExeName);
    void write_ComPortType(AnsiString asSetupFile,AnsiString asExeName, AnsiString as_com_type);
    //-----------------------------------------------------------------------------------
    AnsiString read_DA_bin(AnsiString asSetupFile,AnsiString asExeName);
    void write_DA_bin(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DA_bin);
    //-----------------------------------------------------------------------------------
    AnsiString read_AuthFile(AnsiString asSetupFile,AnsiString asExeName);
    void write_AuthFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AuthFile);
    //-----------------------------------------------------------------------------------
    AnsiString read_EnableOutputResult(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_EnableWthDog(AnsiString asSetupFile,AnsiString asExeName);
	AnsiString read_SkipChecksum(AnsiString asSetupFile,AnsiString asExeName);
	AnsiString read_FmPreloaderFlag(AnsiString asSetupFile,AnsiString asExeName);
    //-----------------------------------------------------------------------------------
    AnsiString read_CertFile(AnsiString asSetupFile,AnsiString asExeName);
    void write_CertFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CertFile);
    //-----------------------------------------------------------------------------------
    AnsiString read_FlashBinFile(AnsiString asSetupFile,AnsiString asExeName);
    void write_FlashBinFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_BinFile);

    //-----------------------------------------------------------------------------------
    AnsiString read_Scatter( AnsiString asSetupFile,AnsiString asExeName, int idx );
    void write_Scatter( AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Scatter);
    //-----------------------------------------------------------------------------------
    AnsiString read_ROM(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_ROM(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_ROM);
    //-----------------------------------------------------------------------------------
    AnsiString read_FAT(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_FAT(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //-----------------------------------------------------------------------------------
    AnsiString read_MLoader(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_MLoader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

   //-----------------------------------------------------------------------------------
    AnsiString read_EBoot(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBoot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

  //==============================================================================
    _BOOL read_MLoader_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_MLoader_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_mloder_Checked);

    //==============================================================================
    _BOOL read_EBoot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBoot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

    //=============================================================================
    AnsiString read_RomInfo(AnsiString asSetupFile, AnsiString asExeName, AnsiString RomName, AnsiString SaveType);
    void write_RomInfo(AnsiString asSetupFile, AnsiString asExeName, AnsiString RomName, AnsiString as_Path,AnsiString SaveType);
    //=============================================================================
    //-----------------------------------------------------------------------------------
    AnsiString read_Preloader(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Preloader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_Preloader_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Preloader_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

    //-----------------------------------------------------------------------------------
    AnsiString read_DSPBL(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_DSPBL(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_DSPBL_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_DSPBL_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);


    //-----------------------------------------------------------------------------------
    AnsiString read_UBoot(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_UBoot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_UBoot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_UBoot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);


    //-----------------------------------------------------------------------------------
    AnsiString read_Boot(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Boot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_Boot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Boot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);


    //-----------------------------------------------------------------------------------
    AnsiString read_Logo(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Logo(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_Logo_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Logo_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

	    //-----------------------------------------------------------------------------------
    AnsiString read_TEE1(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_TEE1(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_TEE1_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_TEE1_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

	  AnsiString read_TEE2(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_TEE2(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_TEE2_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_TEE2_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

    //-----------------------------------------------------------------------------------
    AnsiString read_Secro(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Secro(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

	AnsiString read_Secro4Reworking(AnsiString asSetupFile,AnsiString asExeName);
	void write_Secro4Reworking(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_FAT);
	_BOOL read_SecroReworkingSupport(AnsiString asSetupFile,AnsiString asExeName);
	void write_SecroReworkingSupport(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked);
	AnsiString read_SecroScatter( AnsiString asSetupFile,AnsiString asExeName );
    void write_SecroScatter( AnsiString asSetupFile,AnsiString asExeName, AnsiString as_Scatter );
		
    //==============================================================================
    _BOOL read_Secro_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Secro_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

    //-----------------------------------------------------------------------------------
    AnsiString read_Recovery(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Recovery(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);
    //==============================================================================
    _BOOL read_Recovery_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_Recovery_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);


    //-----------------------------------------------------------------------------------

	AnsiString read_emmc_FAT(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_emmc_FAT(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_emmc_FAT_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_emmc_FAT_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

	AnsiString read_cache(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_cache(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_cache_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_cache_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

	AnsiString read_EBR1(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBR1(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_EBR1_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBR1_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);
  //==============================================================================
    _BOOL  read_SCATTER(AnsiString asSetupFile,AnsiString asExeName);  //
    _BOOL  read_Checksum_Ver(AnsiString asSetupFile,AnsiString asExeName);  //
     void  write_Checksum_Ver(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked); //
    void write_SCATTER(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked);
    //==============================================================================
	AnsiString read_EBR2(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBR2(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_EBR2_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_EBR2_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

	AnsiString read_MBR(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_MBR(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_MBR_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_MBR_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);

    //-----------------------------------------------------------------------------------
    AnsiString read_AndroidSystem(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_AndroidSystem(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_AndroidSystem_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_AndroidSystem_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked);


    //-----------------------------------------------------------------------------------
    AnsiString read_UserData(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_UserData(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT);

    //==============================================================================
    _BOOL read_UserData_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_UserData_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_Checked);

    AnsiString read_Custpack(AnsiString asSetupFile,AnsiString asExeName, int sequence, int idx);
    void write_Custpack(AnsiString asSetupFile, AnsiString asExeName, int sequence, int idx, AnsiString as_Path);
    _BOOL read_Custpack_Checked(AnsiString asSetupFile,AnsiString asExeName, int sequence, int idx);
    void write_Custpack_Checked(AnsiString asSetupFile, AnsiString asExeName, int sequence, int idx, _BOOL as_Checked);

    //-----------------------------------------------------------------------------------
    AnsiString read_SEC_RO(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_SEC_RO(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_SEC_RO);
    //-----------------------------------------------------------------------------------
    AnsiString read_SEC_RO_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_SEC_RO_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_SEC_RO_Checked);
    //-----------------------------------------------------------------------------------
    // backup and restore calibration data
    AnsiString read_BackupAndRestoreOp(AnsiString asSetupFile,AnsiString asExeName);
    void write_BackupAndRestoreOp(AnsiString asSetupFile, AnsiString asExeName, AnsiString as);
    //-----------------------------------------------------------------------------------
    // GPS
    AnsiString read_GPS_ROM_Path(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_GPS_ROM_Path(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as);
    //-----------------------------------------------------------------------------------
    AnsiString read_GPS_ROM_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_GPS_ROM_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as);
    //-----------------------------------------------------------------------------------
    AnsiString read_TheFirstComPortNo(AnsiString asSetupFile,AnsiString asExeName);
    void write_TheFirstComPortNo(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_TheFirstComPortNo);

    //-----------------------------------------------------------------------------------
    void read_COMNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size);
    void write_COMNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size);

	void read_COMPreloaderNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size);
	void write_COMPreloaderNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size);

    //-----------------------------------------------------------------------------------
    AnsiString read_ComEnabled(AnsiString asSetupFile,AnsiString asExeName);
    void write_ComEnabled(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_ComEnabled);
    //-----------------------------------------------------------------------------------
    AnsiString read_AutoPollingEnabled(AnsiString asSetupFile,AnsiString asExeName);
    void write_AutoPollingEnabled(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoPollingEnabled);
    //-----------------------------------------------------------------------------------
    AnsiString read_AutoPollingCount(AnsiString asSetupFile,AnsiString asExeName);
    void write_AutoPollingCount(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoPollingEnabled);
    //-----------------------------------------------------------------------------------
    AnsiString read_LV_FAT_index(AnsiString asSetupFile, AnsiString asExeName, int idx);
    void write_LV_FAT_index(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_LV_FAT_index);
    //------------------------------------------------------------------------------
    AnsiString read_LV_count(AnsiString asSetupFile,AnsiString asExeName);
    void write_LV_count(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_count);
    //------------------------------------------------------------------------------
    AnsiString read_LV_name(AnsiString asSetupFile,AnsiString asExeName, int index);
    void write_LV_name(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_name);
    //-----------------------------------------------------------------------------------
    void write_LV_region_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_region_addr);
    void write_LV_begin_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_begin_addr);
    //-----------------------------------------------------------------------------------
    void write_LV_end_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_end_addr);
    //-----------------------------------------------------------------------------------
    void write_LV_location(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_location);
    //-----------------------------------------------------------------------------------
    AnsiString read_Baseband_chip(AnsiString asSetupFile,AnsiString asExeName);
    void write_Baseband_chip(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Baseband_chip);
    //------------------------------------------------------------------------------
    AnsiString read_External_clock(AnsiString asSetupFile,AnsiString asExeName);
    void write_External_clock(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_External_clock);
    //------------------------------------------------------------------------------
    AnsiString read_CheckECOVersion(AnsiString asSetupFile,AnsiString asExeName);
    void write_CheckECOVersion(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CheckECOVersion);
    //------------------------------------------------------------------------------
    AnsiString read_CheckTgtResLayout(AnsiString asSetupFile,AnsiString asExeName);
    void write_CheckTgtResLayout(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CheckTgtResLayout);
    //------------------------------------------------------------------------------
    AnsiString read_AutoLoadResByScat(AnsiString asSetupFile,AnsiString asExeName);
    void write_AutoLoadResByScat(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoLoadResByScat);
    //------------------------------------------------------------------------------
    AnsiString read_CompareResProjectId(AnsiString asSetupFile,AnsiString asExeName);
    void write_CompareResProjectId(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CompareResProjectId);
    //------------------------------------------------------------------------------
    AnsiString read_BROMDllLog(AnsiString asSetupFile,AnsiString asExeName);
    void write_BROMDllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_BROMDllLog);
    //------------------------------------------------------------------------------
    AnsiString read_METADllLog(AnsiString asSetupFile,AnsiString asExeName);
    void write_METADllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_METADllLog);
     //------------------------------------------------------------------------------
    AnsiString read_EBootDllLog(AnsiString asSetupFile,AnsiString asExeName);
    void write_EBootDllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_METADllLog);
    //------------------------------------------------------------------------------
    AnsiString read_Baudrate(AnsiString asSetupFile,AnsiString asExeName);
    void write_Baudrate(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Baudrate);
    //------------------------------------------------------------------------------
    AnsiString read_Type(AnsiString asSetupFile,AnsiString asExeName);
    void write_Type(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Type);

	AnsiString read_OTPOption(AnsiString asSetupFile,AnsiString asExeName);
    void write_OTPOption(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Type);
    //------------------------------------------------------------------------------
    AnsiString read_Format_FAT_range(AnsiString asSetupFile,AnsiString asExeName);
    void write_Format_FAT_range(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_range);
	//------------------------------------------------------------------------------
    AnsiString read_Format_FAT_Begin_Address(AnsiString asSetupFile,AnsiString asExeName);
    void write_Format_FAT_Begin_Address(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_Begin_Address);
    //------------------------------------------------------------------------------
    AnsiString read_Format_FAT_length(AnsiString asSetupFile,AnsiString asExeName);
    void write_Format_FAT_length(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_length);
    //------------------------------------------------------------------------------
    AnsiString read_NAND_Format_FAT_Begin_Address(AnsiString asSetupFile,AnsiString asExeName);
    void write_NAND_Format_FAT_Begin_Address(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_NAND_Format_FAT_Begin_Address);
    //------------------------------------------------------------------------------
    AnsiString read_NAND_Format_FAT_length(AnsiString asSetupFile,AnsiString asExeName);
    void write_NAND_Format_FAT_length(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_length);
    //------------------------------------------------------------------------------
    AnsiString read_Erase_Validation(AnsiString asSetupFile,AnsiString asExeName);
    //------------------------------------------------------------------------------
    AnsiString read_Pull_down_GPIO17(AnsiString asSetupFile,AnsiString asExeName);

    //==============================================================================
    // NFB
    AnsiString read_BootLoader(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_BootLoader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_BootLoader);
    //------------------------------------------------------------------------------
    AnsiString read_BootLoaderEnable(AnsiString asSetupFile,AnsiString asExeName, int idx);
    void write_BootLoaderEnable(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_BootLoader);

    //==============================================================================
    // access
    AnsiString read_DataSource(AnsiString asSetupFile,AnsiString asExeName);
    void write_DataSource(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DataSource);
    //------------------------------------------------------------------------------
    AnsiString read_DbLogEnable(AnsiString asSetupFile,AnsiString asExeName);
    void write_DbLogEnable(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DbLogEnable);

    //==============================================================================
    // EMI
    AnsiString read_bank0_auto(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_bank0_cfg(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_bank1_auto(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_bank1_cfg(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_nand_acccon(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_gen_a(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_gen_b(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_gen_c(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_dram_con_i_mode(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_dram_con_i_ext_mode(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_dram_con_k(AnsiString asSetupFile,AnsiString asExeName);
    AnsiString read_emi_dram_con_l(AnsiString asSetupFile,AnsiString asExeName);

//    AnsiString read_wcescomm_path(AnsiString asSetupFile,AnsiString asExeName);

    //For image load checksum
    int read_HostBufferEnable(AnsiString asCksmFile, AnsiString asExeName);
    int read_HostBufferStdCksm(AnsiString asCksmFile, AnsiString asExeName, AnsiString name);

	//==========================Scan comport type===============================
	AnsiString read_ScanComportType(AnsiString asSetupFile,AnsiString asExeName);
	void write_ScanComportType(AnsiString asSetupFile, AnsiString asExeName,AnsiString as_ScanComportType);

    //==========================Reset Power Key================================
    AnsiString read_ResetKey(AnsiString asSetupFile,AnsiString asExeName);
    void write_ResetKey(AnsiString asSetupFile, AnsiString asExeName,AnsiString as_ResetKey);

	AnsiString read_EfuseXmlFilePath(AnsiString asSetupFile,AnsiString asExeName);

    AnsiString read_ForceWatchdogResetFlag( AnsiString asSetupFile,AnsiString asExeName );
	AnsiString read_ForceWatchdogResetTimeOut( AnsiString asSetupFile,AnsiString asExeName );
	void       write_ForceWatchdogResetFlag(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Value);
	void       write_ForceWatchdogResetTimeOut(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Value);
	_BOOL      read_EnableEnterMetaFlag(AnsiString asSetupFile,AnsiString asExeName);
};

#endif

