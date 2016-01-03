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
 *   INIFileHandle.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  INI file handling source file
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include <inifiles.hpp>

#pragma hdrstop


#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif



#include "MultiPort_Utils.h"

#include "INIFileHandle.h"

#define COM_PORT_TYPE_STR "COM port type"

//==============================================================================
AnsiString CINIFileHandle::read_field(AnsiString asSetupFile,AnsiString asExeName, AnsiString asSecName, AnsiString asFieldName, AnsiString asDefStr)
{
    AnsiString asPath;
    AnsiString as_str;
    if ( getPathFromStr(asExeName, asPath) &&
         withPath( asPath) &&
         !withPath( asSetupFile)
       )
    {
       asSetupFile = asPath + asSetupFile;
    }

    TIniFile *ini;

    try
    {
       ini = new TIniFile(asSetupFile);
       if(ini != NULL)
       {
           as_str  = ini->ReadString(asSecName, asFieldName, asDefStr);
           delete ini;
       }
    }
    catch (...)
    {
        ShowMessage( " read setup file " + asSetupFile + " error: " + asFieldName);
    }
    return as_str;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_field(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString asFieldName, AnsiString asStr)
{
    AnsiString asPath;
    if ( getPathFromStr(asExeName, asPath) &&
         withPath( asPath) &&
         !withPath( asSetupFile)
       )
    {
       asSetupFile = asPath + asSetupFile;
    }

    TIniFile *ini;

    try
    {
        ini = new TIniFile( asSetupFile );

        if(ini != NULL)
        {
            ini->WriteString(asSecName, asFieldName, asStr );
            delete ini;
        }

    }
    catch (...)
    {
        ShowMessage( " write setup file : " + asSetupFile + " error" + asStr);
    }
}

//==============================================================================
AnsiString CINIFileHandle::read_Operation(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_Operation = read_field(asSetupFile, asExeName, "form", "Operation", "NOR");
    return  as_Operation;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_Operation(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Operation)
{
    write_field(asSetupFile, asExeName, "form", "Operation", as_Operation);
}

//==============================================================================
AnsiString CINIFileHandle::read_ComPortType(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_Operation = read_field(asSetupFile, asExeName, "form", COM_PORT_TYPE_STR, COM_PORT_UART_STR);
    return  as_Operation;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_ComPortType(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_com_type)
{
    write_field(asSetupFile, asExeName, "form", COM_PORT_TYPE_STR, as_com_type);
}

//==============================================================================
AnsiString CINIFileHandle::read_DA_bin(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_DA_bin = read_field(asSetupFile, asExeName, "Bin_File_Path","DA_bin", "");
    return  as_DA_bin;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_DA_bin(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DA_bin)
{
    write_field(asSetupFile, asExeName, "Bin_File_Path", "DA_bin", as_DA_bin);
}

//==============================================================================
AnsiString CINIFileHandle::read_AuthFile(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_AuthFile = read_field(asSetupFile, asExeName, "form", "Auth file", "");
    return  as_AuthFile;
}

//==============================================================================
AnsiString CINIFileHandle::read_EnableWthDog(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_EnableWthDog = read_field(asSetupFile, asExeName, "AutoRunning", "Enable_WatchDog", "0");
    return  as_EnableWthDog;
}

//==============================================================================
AnsiString CINIFileHandle::read_EnableOutputResult(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_EnableOutputResult = read_field(asSetupFile, asExeName, "AutoRunning", "Enable_WatchDog", "0");
    return  as_EnableOutputResult;
}

//==============================================================================
AnsiString CINIFileHandle::read_SkipChecksum(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_SkipChecksum = read_field(asSetupFile, asExeName, "AutoRunning", "Skip_Checksum", "no");
    return  as_SkipChecksum;
}

//==============================================================================
AnsiString CINIFileHandle::read_FmPreloaderFlag(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_FmPlreoader = read_field(asSetupFile, asExeName, "AutoRunning", "format_preloader", "no");
    return  as_FmPlreoader;
}



//---------------------------------------------------------------------------
void CINIFileHandle::write_AuthFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AuthFile)
{
    write_field(asSetupFile, asExeName, "form", "Auth file", as_AuthFile);
}

//==============================================================================
AnsiString CINIFileHandle::read_CertFile(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_CertFile = read_field(asSetupFile, asExeName, "form", "Cert file", "");
    return  as_CertFile;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_CertFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CertFile)
{
    write_field(asSetupFile, asExeName, "form", "Cert file", as_CertFile);
}

//==============================================================================
AnsiString CINIFileHandle::read_FlashBinFile(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_BinFile = read_field(asSetupFile, asExeName, "form", "FlashBin file", "");
    return  as_BinFile;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_FlashBinFile(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_BinFile)
{
    write_field(asSetupFile, asExeName, "form", "FlashBin file", as_BinFile);
}

//==============================================================================
AnsiString CINIFileHandle::read_Scatter( AnsiString asSetupFile,AnsiString asExeName, int idx )
{
    AnsiString as_KEY_NAME = "Scatter" + IntToStr( idx );
    AnsiString as_Scatter = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Scatter;
}

AnsiString CINIFileHandle::read_SecroScatter( AnsiString asSetupFile,AnsiString asExeName )
{
    AnsiString as_KEY_NAME = "Secro Scatter";
    AnsiString as_Scatter = read_field(asSetupFile, asExeName, "Scatter_File_Path", as_KEY_NAME, "");
    return  as_Scatter;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Scatter(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Scatter)
{
    AnsiString as_KEY_NAME = "Scatter" + IntToStr( idx );
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Scatter);
}

void CINIFileHandle::write_SecroScatter(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Scatter)
{
    AnsiString as_KEY_NAME = "Secro Scatter";
    write_field(asSetupFile, asExeName, "Scatter_File_Path", as_KEY_NAME, as_Scatter);
}


//=============================================================================
AnsiString CINIFileHandle::read_ForceWatchdogResetFlag( AnsiString asSetupFile,AnsiString asExeName )
{
    AnsiString as_KEY_NAME = "ForceWatchdogReset";
    AnsiString as_Scatter = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "no");
    return  as_Scatter;
}

AnsiString CINIFileHandle::read_ForceWatchdogResetTimeOut( AnsiString asSetupFile,AnsiString asExeName )
{
    AnsiString as_KEY_NAME = "ForceWatchdogResetTimeout";
    AnsiString as_Scatter = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "0");
    return  as_Scatter;
}

void CINIFileHandle::write_ForceWatchdogResetFlag(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Value)
{
    AnsiString as_KEY_NAME = "ForceWatchdogReset";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Value);
}

void CINIFileHandle::write_ForceWatchdogResetTimeOut(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Value)
{
    AnsiString as_KEY_NAME = "ForceWatchdogResetTimeout";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Value);
}


//==============================================================================
AnsiString CINIFileHandle::read_ROM(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "ROM" + IntToStr(idx);
    AnsiString as_ROM = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_ROM;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_ROM(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_ROM)
{
    AnsiString as_KEY_NAME = "ROM" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_ROM);
}

//==============================================================================
AnsiString CINIFileHandle::read_FAT(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "FAT" + IntToStr(idx);
    AnsiString as_FAT = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "MT6205B_FAT.bin");
    return  as_FAT;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_FAT(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_FAT)
{
    AnsiString as_KEY_NAME = "FAT" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_FAT);
}

//==============================================================================
AnsiString CINIFileHandle::read_MLoader(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "MLoader" + IntToStr(idx);
    AnsiString as_FAT = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_FAT;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_MLoader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_MLoader)
{
    AnsiString as_KEY_NAME = "MLoader" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_MLoader);
}

//==============================================================================
_BOOL CINIFileHandle::read_MLoader_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "MLoder" + IntToStr(idx) + " checked";
    AnsiString as_MLoder_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_MLoder_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_MLoader_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_Molder_Checked)
{
    AnsiString  b_value;

    if(b_Molder_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "MLoder" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_EBoot(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "EBoot" + IntToStr(idx);
    AnsiString as_FAT = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_FAT;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBoot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_EBoot)
{
    AnsiString as_KEY_NAME = "EBoot" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_EBoot);
}

//==============================================================================
_BOOL CINIFileHandle::read_EBoot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "EBoot" + IntToStr(idx) + " checked";
    AnsiString as_EBoot_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_EBoot_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBoot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL b_EBoot_Checked)
{
    AnsiString  b_value;

    if(b_EBoot_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "EBoot" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_SEC_RO(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "SEC_RO" + IntToStr(idx);
    AnsiString as_SEC_RO = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "SECURE_RO");
    return  as_SEC_RO;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_SEC_RO(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_SEC_RO)
{
    AnsiString as_KEY_NAME = "SEC_RO" + IntToStr(idx) ;
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_SEC_RO);
}

//==============================================================================
AnsiString CINIFileHandle::read_SEC_RO_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "SEC_RO" + IntToStr(idx) + " checked";
    AnsiString as_SEC_RO_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");
    return  as_SEC_RO_Checked;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_SEC_RO_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_SEC_RO_Checked)
{
    AnsiString as_KEY_NAME = "SEC_RO" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_SEC_RO_Checked);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_GPS_ROM_Path(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "GPS ROM Path" + IntToStr(idx);
    AnsiString as = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "GPS_ROM");
    return  as;
}


//wcescomm
/*
AnsiString CINIFileHandle::read_wcescomm_path(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as = read_field(asSetupFile, asExeName, "form", "Wcescomm_exe", "");
    return  as;
}
*/

//===========================================================================
//////////////////////////// backup and restore /////////////////////////////
//===========================================================================
AnsiString CINIFileHandle::read_BackupAndRestoreOp(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as = read_field(asSetupFile, asExeName, "form", "Backup and restore option", "download without backup and restore calibration data");
    return  as;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_BackupAndRestoreOp(AnsiString asSetupFile, AnsiString asExeName, AnsiString as)
{
    write_field(asSetupFile, asExeName, "form", "Backup and restore option", as);
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_GPS_ROM_Path(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as)
{
    AnsiString as_KEY_NAME = "GPS ROM Path" + IntToStr(idx) ;
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as);
}

//==============================================================================
AnsiString CINIFileHandle::read_GPS_ROM_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "GPS_ROM" + IntToStr(idx) + " checked";
    AnsiString as = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");
    return  as;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_GPS_ROM_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as)
{
    AnsiString as_KEY_NAME = "GPS_ROM" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as);
}


//==============================================================================
AnsiString CINIFileHandle::read_TheFirstComPortNo(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_FirstComPortNo = read_field(asSetupFile, asExeName, "form","TheFirstComPortNo", "3");
    return  as_FirstComPortNo;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_TheFirstComPortNo(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_TheFirstComPortNo)
{
    write_field(asSetupFile, asExeName, "form", "TheFirstComPortNo", as_TheFirstComPortNo);
}

//==============================================================================
void CINIFileHandle::read_COMNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size)
{
    AnsiString strNo;
    AnsiString strProp = "Channel";
    AnsiString tmp;

    for(int i = 0; i < size; i++)
    {
        tmp = IntToStr(i);
        strNo = read_field(asSetupFile, asExeName, "Ports", strProp + tmp, /*tmp*/0);
        comArray[i] = strNo.ToIntDef(0);
        if(strNo < 0)
        {
            comArray[i] = 0;
        }
    }
}
void CINIFileHandle::read_COMPreloaderNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size)
{
    AnsiString strNo;
    AnsiString strProp = "Channel";
    AnsiString tmp;

    for(int i = 0; i < size; i++)
    {
        tmp = IntToStr(i);
        strNo = read_field(asSetupFile, asExeName, "PreloaderPorts", strProp + tmp, /*tmp*/0);
        comArray[i] = strNo.ToIntDef(0);
        if(strNo < 0)
        {
            comArray[i] = 0;
        }
    }
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_COMNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size)
{
    AnsiString strNo;
    AnsiString strProp = "Channel";
    AnsiString tmp;

    for(int i = 0; i < size; i++)
    {
        strNo = IntToStr(comArray[i]);
        tmp = IntToStr(i);
        write_field(asSetupFile, asExeName, "Ports", strProp + tmp, strNo);
    }
}

void CINIFileHandle::write_COMPreloaderNoArray(AnsiString asSetupFile, AnsiString asExeName, int * comArray, int size)
{
    AnsiString strNo;
    AnsiString strProp = "Channel";
    AnsiString tmp;

    for(int i = 0; i < size; i++)
    {
        strNo = IntToStr(comArray[i]);
        tmp = IntToStr(i);
        write_field(asSetupFile, asExeName, "PreloaderPorts", strProp + tmp, strNo);
    }
}
//==============================================================================
AnsiString CINIFileHandle::read_ComEnabled(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_ComEnabled = read_field(asSetupFile, asExeName, "form","ComportEnabled", "1");
    return  as_ComEnabled;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_ComEnabled(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_ComportEnabled)
{
    write_field(asSetupFile, asExeName, "form", "ComportEnabled", as_ComportEnabled);
}

//==============================================================================
AnsiString CINIFileHandle::read_AutoPollingEnabled(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_AutoPollingEnabled = read_field(asSetupFile, asExeName, "form", "Auto polling enabled", "0");
    return  as_AutoPollingEnabled;
}

//--------------------------------------------------------------------------
void CINIFileHandle::write_AutoPollingEnabled(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoPollingEnabled)
{
    write_field(asSetupFile, asExeName, "form", "Auto polling enabled", as_AutoPollingEnabled);
}


//==============================================================================
AnsiString CINIFileHandle::read_AutoPollingCount(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_AutoPollingCount = read_field(asSetupFile, asExeName, "form", "Auto polling count", "0");
    return  as_AutoPollingCount;
}

//--------------------------------------------------------------------------
void CINIFileHandle::write_AutoPollingCount(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoPollingCount)
{
    write_field(asSetupFile, asExeName, "form", "Auto polling count", as_AutoPollingCount);
}



//==============================================================================
AnsiString CINIFileHandle::read_LV_FAT_index( AnsiString asSetupFile, AnsiString asExeName, int idx )
{
    AnsiString as_KEY_NAME = "rom_info_FAT_index" + IntToStr( idx );
    AnsiString as_LV_FAT_index = read_field(asSetupFile, asExeName, "Download", as_KEY_NAME, "0");
    return  as_LV_FAT_index;
}

//-----------------------------------------------------------------------------
void CINIFileHandle::write_LV_FAT_index(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_LV_FAT_index)
{
    AnsiString as_KEY_NAME = "rom_info_FAT_index" + IntToStr( idx );
    write_field(asSetupFile, asExeName, "Download", as_KEY_NAME, as_LV_FAT_index);
}

//==============================================================================
AnsiString CINIFileHandle::read_LV_count(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_LV_count = read_field(asSetupFile, asExeName, "Download", "rom_info_count", "0");
    return  as_LV_count;
}

//--------------------------------------------------------------------------
void CINIFileHandle::write_LV_count(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_count)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_count);
}

//==============================================================================
AnsiString CINIFileHandle::read_LV_name(AnsiString asSetupFile,AnsiString asExeName, int index)
{
    AnsiString as_LV_name = read_field(asSetupFile, asExeName, "Download", "rom_info_name"+IntToStr(index), "");
    return  as_LV_name;
}

//--------------------------------------------------------------------------
void CINIFileHandle::write_LV_name(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_name)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_name);
}

//==============================================================================
void CINIFileHandle::write_LV_region_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_region_addr)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_region_addr);
}

//==============================================================================
void CINIFileHandle::write_LV_begin_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_begin_addr)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_begin_addr);
}

//==============================================================================
void CINIFileHandle::write_LV_end_addr(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_end_addr)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_end_addr);
}

//==============================================================================
void CINIFileHandle::write_LV_location(AnsiString asSetupFile, AnsiString asExeName, AnsiString asSecName, AnsiString as_LV_location)
{
    write_field(asSetupFile, asExeName, "Download", asSecName, as_LV_location);
}

//==============================================================================
AnsiString CINIFileHandle::read_Baseband_chip(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_Baseband_chip = read_field(asSetupFile, asExeName, "form","Baseband chip", "6205B");
    return  as_Baseband_chip;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Baseband_chip(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Baseband_chip)
{
    write_field(asSetupFile, asExeName, "form", "Baseband chip", as_Baseband_chip);
}

//==========================================================================================
AnsiString CINIFileHandle::read_External_clock(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_External_clock = read_field(asSetupFile, asExeName, "form","External clock", "26 mhz");
    return  as_External_clock;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_External_clock(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_External_clock)
{
    write_field(asSetupFile, asExeName, "form", "External clock", as_External_clock);
}

//==============================================================================
AnsiString CINIFileHandle::read_CheckECOVersion(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_CheckECOVersion = read_field(asSetupFile, asExeName, "form", "Check ECO version", "yes");
    return  as_CheckECOVersion;
}

//--------------------------------------------------------------------------
void CINIFileHandle::write_CheckECOVersion(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CheckECOVersion)
{
    write_field(asSetupFile, asExeName, "form", "Check ECO version", as_CheckECOVersion);
}

//==============================================================================
//////////////////////////     Multi-bin option     ////////////////////////////
//==============================================================================
AnsiString CINIFileHandle::read_CheckTgtResLayout(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_CheckTgtResLayout = read_field(asSetupFile, asExeName, "form", "Check target resource layout", "yes");
    return  as_CheckTgtResLayout;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_CheckTgtResLayout(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CheckTgtResLayout)
{
    write_field(asSetupFile, asExeName, "form", "Check target resource layout", as_CheckTgtResLayout);
}

//==============================================================================
AnsiString CINIFileHandle::read_AutoLoadResByScat(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_AutoLoadResByScat = read_field(asSetupFile, asExeName, "form", "Auto load resource by scat", "yes");
    return  as_AutoLoadResByScat;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_AutoLoadResByScat(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_AutoLoadResByScat)
{
    write_field(asSetupFile, asExeName, "form", "Auto load resource by scat", as_AutoLoadResByScat);
}

//==============================================================================
AnsiString CINIFileHandle::read_CompareResProjectId(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_CompareResProjectId = read_field(asSetupFile, asExeName, "form", "Compare resoure project ID", "yes");
    return  as_CompareResProjectId;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_CompareResProjectId(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_CompareResProjectId)
{
    write_field(asSetupFile, asExeName, "form", "Compare resoure project ID", as_CompareResProjectId);
}

//==============================================================================
AnsiString CINIFileHandle::read_BROMDllLog(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_BROMDllLog = read_field(asSetupFile, asExeName, "form", "BROM_DLL logging", "yes");
    return  as_BROMDllLog;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_BROMDllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_BROMDllLog)
{
    write_field(asSetupFile, asExeName, "form", "BROM_DLL logging", as_BROMDllLog);
}

//==============================================================================
AnsiString CINIFileHandle::read_METADllLog(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_METADllLog = read_field(asSetupFile, asExeName, "form", "META_DLL logging", "yes");
    return  as_METADllLog;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_METADllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_METADllLog)
{
    write_field(asSetupFile, asExeName, "form", "META_DLL logging", as_METADllLog);
}

//==============================================================================
AnsiString CINIFileHandle::read_EBootDllLog(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_EbootDllLog = read_field(asSetupFile, asExeName, "form", "Eboot_DLL logging", "yes");
    return  as_EbootDllLog;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_EBootDllLog(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_EbootDllLog)
{
    write_field(asSetupFile, asExeName, "form", "Eboot_DLL logging", as_EbootDllLog);
}

//==============================================================================
AnsiString CINIFileHandle::read_Baudrate(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_baudrate = read_field(asSetupFile, asExeName, "form", "Baudrate", "921600");
    return  as_baudrate;

}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Baudrate(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Baudrate)
{
    write_field(asSetupFile, asExeName, "form", "Baudrate", as_Baudrate );
}

//==========================================================================================
AnsiString CINIFileHandle::read_Type(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_Type = read_field(asSetupFile, asExeName, "form", "Type", "Format and Download All");
    return  as_Type;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Type(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Type)
{
    write_field(asSetupFile, asExeName, "form", "Type", as_Type );
}

void CINIFileHandle::write_OTPOption(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Type)
{
    write_field(asSetupFile, asExeName, "OTP Option", "Option", as_Type );
}


AnsiString CINIFileHandle::read_OTPOption(AnsiString asSetupFile, AnsiString asExeName)
{
    AnsiString as_OTPOption = read_field(asSetupFile, asExeName, "OTP Option", "Option", "NON OTP" );
	return as_OTPOption;
}

//==========================================================================================
AnsiString CINIFileHandle::read_Format_FAT_range(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_FormatFat = read_field(asSetupFile, asExeName, "form", "Format FAT range", "auto");
    return  as_FormatFat;

}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Format_FAT_range(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_range)
{
    write_field(asSetupFile, asExeName, "form", "Format FAT range", as_Format_FAT_range );
}

//==========================================================================================
AnsiString CINIFileHandle::read_Format_FAT_Begin_Address(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString Format_FAT_Begin_Address = read_field(asSetupFile, asExeName, "form", "Format FAT begin address", "0x00700000");
    return  Format_FAT_Begin_Address;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Format_FAT_Begin_Address(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_Begin_Address)
{
    write_field(asSetupFile, asExeName, "form", "Format FAT begin address", as_Format_FAT_Begin_Address );
}

//==========================================================================================
AnsiString CINIFileHandle::read_Format_FAT_length(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString Format_FAT_length = read_field(asSetupFile, asExeName, "form", "Format FAT length", "0x00100000");
    return  Format_FAT_length;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Format_FAT_length(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Format_FAT_length)
{
    write_field(asSetupFile, asExeName, "form", "Format FAT length", as_Format_FAT_length );
}

//==========================================================================================
AnsiString CINIFileHandle::read_NAND_Format_FAT_Begin_Address(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString NAND_Format_FAT_Begin_Address = read_field(asSetupFile, asExeName, "form", "NAND format FAT begin address", "0x00000000");
    return  NAND_Format_FAT_Begin_Address;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_NAND_Format_FAT_Begin_Address(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_NAND_Format_FAT_Begin_Address)
{
    write_field(asSetupFile, asExeName, "form", "NAND format FAT begin address", as_NAND_Format_FAT_Begin_Address );
}

//==========================================================================================
AnsiString CINIFileHandle::read_NAND_Format_FAT_length(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString NAND_Format_FAT_length = read_field(asSetupFile, asExeName, "form", "NAND format FAT length", "0x08000000");
    return  NAND_Format_FAT_length;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_NAND_Format_FAT_length(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_NAND_Format_FAT_length)
{
    write_field(asSetupFile, asExeName, "form", "NAND format FAT length", as_NAND_Format_FAT_length );
}


//=============================================================================
AnsiString CINIFileHandle::read_Erase_Validation(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_Erase_Validation = read_field(asSetupFile, asExeName, "FORM", "erase_validation", "no");
    return  as_Erase_Validation;
}


//==========================================================================================
AnsiString CINIFileHandle::read_Pull_down_GPIO17(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString Format_FAT_length = read_field(asSetupFile, asExeName, "form", "Pull down GPIO17", "yes");
    return  Format_FAT_length;
}

//=============================================================================
///////////////////////////          NFB           ////////////////////////////
//=============================================================================
AnsiString CINIFileHandle::read_BootLoader(AnsiString asSetupFile,AnsiString asExeName, int idx )
{
    AnsiString as_KEY_NAME = "bloader" + IntToStr(idx);
    AnsiString as_BootLoader = read_field(asSetupFile, asExeName, "NFB", as_KEY_NAME, "");
    return  as_BootLoader;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_BootLoader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_BootLoader)
{
    AnsiString as_KEY_NAME = "bloader" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "NFB", as_KEY_NAME, as_BootLoader );
}

//=============================================================================
AnsiString CINIFileHandle::read_BootLoaderEnable(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "bloader_enable" + IntToStr(idx);
    AnsiString as_BootLoaderEnable = read_field(asSetupFile, asExeName, "NFB", as_KEY_NAME, "yes");
    return  as_BootLoaderEnable;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_BootLoaderEnable(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_BootLoader_Enable)
{
    AnsiString as_KEY_NAME = "bloader_enable" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "NFB", as_KEY_NAME, as_BootLoader_Enable );
}

//=============================================================================
///////////////////////////        Access          ////////////////////////////
//=============================================================================
AnsiString CINIFileHandle::read_DataSource(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_DataSource = read_field(asSetupFile, asExeName, "Access", "data source", "MTK_DATABASE.mdb");
    return  as_DataSource;
}

//------------------------------------------------------------------------------
void CINIFileHandle::write_DataSource(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DataSource)
{
    write_field(asSetupFile, asExeName, "Access", "data source", as_DataSource);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_DbLogEnable(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_DataSource = read_field(asSetupFile, asExeName, "Access", "Database logging enable", "no");
    return  as_DataSource;
}

//------------------------------------------------------------------------------
void CINIFileHandle::write_DbLogEnable(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_DbLogEnable)
{
    write_field(asSetupFile, asExeName, "Access", "Database logging enable", as_DbLogEnable);
}

//=============================================================================
/////////////////////////////        EMI     //////////////////////////////////
//=============================================================================
AnsiString CINIFileHandle::read_bank0_auto(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_bank0_auto = read_field(asSetupFile, asExeName, "EMI", "bank0_auto", "yes");
    return  as_bank0_auto;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_bank0_cfg(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_bank0_cfg = read_field(asSetupFile, asExeName, "EMI", "bank0_cfg", "0x00004102");
    return  as_bank0_cfg;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_bank1_auto(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_bank1_auto = read_field(asSetupFile, asExeName, "EMI", "bank1_auto", "yes");
    return  as_bank1_auto;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_bank1_cfg(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_bank1_cfg = read_field(asSetupFile, asExeName, "EMI", "bank1_cfg", "0x00004102");
    return  as_bank1_cfg;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_nand_acccon(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_nand_acccon = read_field(asSetupFile, asExeName, "EMI", "nand_acccon", "0x000007FF");
    return  as_nand_acccon;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_gen_a(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_gen_a = read_field(asSetupFile, asExeName, "EMI", "emi_gen_a", "0x568996A2");
    return  as_emi_gen_a;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_gen_b(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_gen_b = read_field(asSetupFile, asExeName, "EMI", "emi_gen_b", "0x5B8A69DA");
    return  as_emi_gen_b;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_gen_c(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_gen_c = read_field(asSetupFile, asExeName, "EMI", "emi_gen_c", "0x00000000");
    return  as_emi_gen_c;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_dram_con_i_mode(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_dram_con_i_mode = read_field(asSetupFile, asExeName, "EMI", "emi_dram_con_i_mode", "0x03A90223");
    return  as_emi_dram_con_i_mode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_dram_con_i_ext_mode(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_dram_con_i_ext_mode = read_field(asSetupFile, asExeName, "EMI", "emi_dram_con_i_ext_mode", "0x03A94000");
    return  as_emi_dram_con_i_ext_mode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_dram_con_k(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_dram_con_k = read_field(asSetupFile, asExeName, "EMI", "emi_dram_con_k", "0x50004564");
    return  as_emi_dram_con_k;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnsiString CINIFileHandle::read_emi_dram_con_l(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_emi_dram_con_l = read_field(asSetupFile, asExeName, "EMI", "emi_dram_con_l", "0x04107777");
    return  as_emi_dram_con_l;
}

int CINIFileHandle::read_BkRs_Action(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_Action", "0");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_BkRs_Action(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_Action", as_check);
}

int CINIFileHandle::read_BkRs_FolderName(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_folderName", "11");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

void CINIFileHandle::write_BkRs_FolderName(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_folderName", as_check);
}

bool CINIFileHandle::read_BkRs_Deletion(AnsiString asSetupFile,AnsiString asExeName)
{
    bool ret;
    AnsiString as_check = read_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_deletion", "yes");

    if(as_check == "yes")
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return  ret;
}

void CINIFileHandle::write_BkRs_Deletion(AnsiString asSetupFile, AnsiString asExeName, bool iCheck)
{
    AnsiString as_check;

    if(iCheck)
    {
        as_check = "yes";
    }
    else
    {
        as_check = "no";
    }
    write_field(asSetupFile, asExeName, "Backup_Restore", "BkRs_deletion", as_check);
}

//==============================================================================

int CINIFileHandle::read_TwoSite_Enable(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "TwoSite_Download", "Enable", "0");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_TwoSite_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "TwoSite_Download", "Enable", as_check);
}

//==============================================================================

int CINIFileHandle::read_TwoSite_Location(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "TwoSite_Download", "Location", "1");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_TwoSite_Location(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "TwoSite_Download", "Location", as_check);
}

//==============================================================================

int CINIFileHandle::read_DADL_Enable(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "DA_Download", "Enable", "1");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_DADL_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "DA_Download", "Enable", as_check);
}

//==============================================================================

int CINIFileHandle::read_DACksm_Enable(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "DA_Download", "Checksum", "1");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_DACksm_Enable(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "DA_Download", "Checksum", as_check);
}

//==============================================================================

int CINIFileHandle::read_DADL_Speed(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "DA_Download", "Speed", "1");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_DADL_Speed(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "DA_Download", "Speed", as_check);
}


//==============================================================================

int CINIFileHandle::read_DADL_Battery(AnsiString asSetupFile,AnsiString asExeName)
{
    int check;
    AnsiString as_check = read_field(asSetupFile, asExeName, "DA_Download", "Battery", "0");

    try
    {
       check = as_check.ToInt();
    }
    catch(EConvertError &e)
    {
       check = 0;
    }
    catch(...)
    {
       check = 0;
    }
    return  check;
}

//----------------------------------------------------------------------------
void CINIFileHandle::write_DADL_Battery(AnsiString asSetupFile, AnsiString asExeName, int iCheck)
{
    AnsiString as_check;
    as_check =  IntToStr(iCheck);
    write_field(asSetupFile, asExeName, "DA_Download", "Battery", as_check);
}



//==============================================================================
AnsiString CINIFileHandle::read_Preloader(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "Preloader" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Preloader(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Preloader" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}
//==============================================================================
void CINIFileHandle::write_RomInfo(AnsiString asSetupFile, AnsiString asExeName, AnsiString RomName, AnsiString as_Path,AnsiString SaveType)
{
    write_field(asSetupFile, asExeName, SaveType, RomName, as_Path);
}
AnsiString CINIFileHandle::read_RomInfo(AnsiString asSetupFile, AnsiString asExeName, AnsiString RomName, AnsiString SaveType)
{
    
    AnsiString as_Path=read_field(asSetupFile, asExeName, SaveType, RomName, "");
    return as_Path;
    
}
//==============================================================================
_BOOL CINIFileHandle::read_Preloader_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Preloader" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Preloader_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Preloader" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

AnsiString CINIFileHandle::read_DSPBL(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "DSPBL" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_DSPBL(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "DSPBL" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_DSPBL_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "DSPBL" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_DSPBL_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "DSPBL" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}



//==============================================================================
AnsiString CINIFileHandle::read_UBoot(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "UBoot" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_UBoot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "UBoot" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_UBoot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "UBoot" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_UBoot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "UBoot" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}



//==============================================================================
AnsiString CINIFileHandle::read_Boot(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "Boot" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Boot(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Boot" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_Boot_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Boot" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Boot_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Boot" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}


//==============================================================================
AnsiString CINIFileHandle::read_Logo(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "Logo" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Logo(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Logo" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_Logo_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Logo" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Logo_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Logo" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//=============================TEE1==============================================
AnsiString CINIFileHandle::read_TEE1(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "TEE1" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_TEE1(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "TEE1" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_TEE1_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "TEE1" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_TEE1_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "TEE1" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//=============================TEE2==============================================
AnsiString CINIFileHandle::read_TEE2(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "TEE2" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_TEE2(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "TEE2" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_TEE2_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "TEE2" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_TEE2_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "TEE2" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}


//==============================================================================
AnsiString CINIFileHandle::read_Secro(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "Secro" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Secro(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Secro" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
AnsiString CINIFileHandle::read_Secro4Reworking(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_KEY_NAME = "Secro4Reworking";
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Secro4Reworking(AnsiString asSetupFile, AnsiString asExeName, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Secro4Reworking";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}


//==============================================================================
_BOOL CINIFileHandle::read_Secro_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Secro" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Secro_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Secro" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//---------------------------------------------------------------------------
AnsiString CINIFileHandle::read_Recovery(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "Recovery" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}
//---------------------------------------------------------------------------
void CINIFileHandle::write_Recovery(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Recovery" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}
//==============================================================================
_BOOL CINIFileHandle::read_Recovery_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Recovery" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Recovery_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Recovery" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_AndroidSystem(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "AndroidSystem" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_AndroidSystem(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "AndroidSystem" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}


//==============================================================================
_BOOL CINIFileHandle::read_AndroidSystem_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "AndroidSystem" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_AndroidSystem_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "AndroidSystem" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_emmc_FAT(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "emmcFAT" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_emmc_FAT(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "emmcFAT" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_emmc_FAT_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "emmcFAT" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_emmc_FAT_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "emmcFAT" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}


//==============================================================================
AnsiString CINIFileHandle::read_cache(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "cache" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_cache(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "cache" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_cache_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "cache" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_cache_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "cache" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_EBR1(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "EBR1" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBR1(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "EBR1" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_EBR1_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "EBR1" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBR1_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "EBR1" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}
//================================================================
 _BOOL CINIFileHandle::read_Checksum_Ver(AnsiString asSetupFile,AnsiString asExeName)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Force_Checksum_Version" ;
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "V1");
    if(as_Checked == "V1")
    {
        ret = _FALSE;
    }
    else
    {
        ret = _TRUE;
    }

    return  ret;

}
void CINIFileHandle::write_Checksum_Ver(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked)
{
     AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "V2";
    }
    else
    {
        b_value ="V1";
    }
    AnsiString as_KEY_NAME = "Force_Checksum_Version" ;
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}
_BOOL CINIFileHandle::read_SCATTER(AnsiString asSetupFile,AnsiString asExeName)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Force_ALL_IMAGE_DL" ;
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");
    if(as_Checked == "yes")
    {
        ret = _FALSE;
    }
    else
    {
        ret = _TRUE;
    }

    return  ret;
}
void CINIFileHandle::write_SCATTER(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked)
{
     AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "no";
    }
    else
    {
        b_value ="yes";
    }
    AnsiString as_KEY_NAME = "Force_ALL_IMAGE_DL" ;
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}


_BOOL CINIFileHandle::read_SecroReworkingSupport(AnsiString asSetupFile,AnsiString asExeName)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Enable_Secro_Reworking_Feature" ;
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "no");
    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}
void CINIFileHandle::write_SecroReworkingSupport(AnsiString asSetupFile, AnsiString asExeName, _BOOL as_Checked)
{
     AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value ="no";
    }
    AnsiString as_KEY_NAME = "Enable_Secro_Reworking_Feature" ;
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_EBR2(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "EBR2" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBR2(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "EBR2" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_EBR2_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "EBR2" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_EBR2_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "EBR2" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_MBR(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "MBR" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_MBR(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "MBR" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_MBR_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "MBR" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_MBR_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "MBR" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}



//==============================================================================
AnsiString CINIFileHandle::read_UserData(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    AnsiString as_KEY_NAME = "UserData" + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_UserData(AnsiString asSetupFile, AnsiString asExeName, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "UserData" + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_UserData_Checked(AnsiString asSetupFile,AnsiString asExeName, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "UserData" + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_UserData_Checked(AnsiString asSetupFile, AnsiString asExeName, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "UserData" + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

//==============================================================================
AnsiString CINIFileHandle::read_Custpack(AnsiString asSetupFile,AnsiString asExeName, int sequence, int idx)
{
    AnsiString as_KEY_NAME = "Custpack" + IntToStr(sequence) + IntToStr(idx);
    AnsiString as_Path = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "");
    return  as_Path;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Custpack(AnsiString asSetupFile, AnsiString asExeName, int sequence, int idx, AnsiString as_Path)
{
    AnsiString as_KEY_NAME = "Custpack" + IntToStr(sequence) + IntToStr(idx);
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_Path);
}

//==============================================================================
_BOOL CINIFileHandle::read_Custpack_Checked(AnsiString asSetupFile,AnsiString asExeName, int sequence, int idx)
{
    _BOOL  ret;
    AnsiString as_KEY_NAME = "Custpack" + IntToStr(sequence) + IntToStr(idx) + " checked";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "yes");

    if(as_Checked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_Custpack_Checked(AnsiString asSetupFile, AnsiString asExeName, int sequence, int idx, _BOOL as_Checked)
{
    AnsiString  b_value;

    if(as_Checked)
    {
        b_value = "yes";
    }
    else
    {
        b_value = "no";
    }

    AnsiString as_KEY_NAME = "Custpack" + IntToStr(sequence) + IntToStr(idx) + " checked";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, b_value);
}

int CINIFileHandle::read_HostBufferEnable(AnsiString asSetupFile,AnsiString asExeName)
{
    int enable;
    AnsiString as_enable = read_field(asSetupFile, asExeName, "IsEnableChecksum", "CHECKSUM_SWITCH", "0");

    try
    {
       enable = as_enable.ToInt();
    }
    catch(EConvertError &e)
    {
       enable = 0;
    }
    catch(...)
    {
       enable = 0;
    }
    return  enable;
}

int CINIFileHandle::read_HostBufferStdCksm(AnsiString asCksmFile, AnsiString asExeName, AnsiString name)
{

    int check;
    AnsiString as_check = read_field(asCksmFile, asExeName, "Checksum", name, "-1");

    try
    {
       check = as_check.ToIntDef(-1);
    }
    catch(EConvertError &e)
    {
       check = -1;
    }
    catch(...)
    {
       check = -1;
    }

    return  check;
}


//==============================================================================
AnsiString CINIFileHandle::read_ScanComportType(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_KEY_NAME = "ScanComportType";
    AnsiString as_ScanComportType = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "0");
	if(as_ScanComportType.IsEmpty())
	{
	    as_ScanComportType = "0";
	}
    return  as_ScanComportType;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_ScanComportType(AnsiString asSetupFile, AnsiString asExeName,AnsiString as_ScanComportType)
{
    AnsiString as_KEY_NAME = "ScanComportType";
    write_field(asSetupFile, asExeName, "form", as_KEY_NAME, as_ScanComportType);
}

//==============================================================================
AnsiString CINIFileHandle::read_ResetKey(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_KEY_NAME = "ResetKey";
    AnsiString as_ResetKey = read_field(asSetupFile, asExeName, "Download", as_KEY_NAME, "h");
	if(as_ResetKey.IsEmpty())
	{
	    as_ResetKey = "h";
	}
    return  as_ResetKey;
}

//---------------------------------------------------------------------------
void CINIFileHandle::write_ResetKey(AnsiString asSetupFile, AnsiString asExeName,AnsiString as_ResetKey)
{
    AnsiString as_KEY_NAME = "ResetKey";
    write_field(asSetupFile, asExeName, "Download", as_KEY_NAME, as_ResetKey);
}

AnsiString CINIFileHandle::read_EfuseXmlFilePath(AnsiString asSetupFile,AnsiString asExeName)
{
    AnsiString as_KEY_NAME = "EfuseXmlPath";
    AnsiString as_XmlFilePath = read_field(asSetupFile, asExeName, "EfuseSettings", as_KEY_NAME, "");
    return  as_XmlFilePath;
}

_BOOL CINIFileHandle::read_EnableEnterMetaFlag(AnsiString asSetupFile,AnsiString asExeName)
{
    _BOOL  bRet;
    AnsiString as_KEY_NAME = "EnableEnterMetaAfterDownload";
    AnsiString as_Checked = read_field(asSetupFile, asExeName, "form", as_KEY_NAME, "no");

    if(as_Checked == "yes")
    {
        bRet = _TRUE;
    }
    else
    {
        bRet = _FALSE;
    }

    return  bRet;
}



