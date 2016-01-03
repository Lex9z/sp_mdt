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
 *   form_About.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *  About form source
 *
 * Author:
 * -------
 *  Andy Ueng (mtk00490)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:   1.9  $
 * $Modtime:   Sep 25 2005 11:31:54  $
 * $Log:   //mtkvs01/vmdata/Multiport_Download/archives/Multiport_Download/Form/form_About.cpp-arc  $
 *
 * Nov 29 2006 mtk00490
 * [STP100001239] [Multiport download tool] Multiport download tool ver 3.1.02
 * 
 * 
 *    Rev 1.9   Sep 25 2005 12:02:46   mtk00490
 * Support NFB download and NAND flash format
 * 
 *    Rev 1.8   Sep 02 2005 10:22:38   mtk00490
 * Support MT6227, MT6228 baseband chip
 * 
 *    Rev 1.7   Jul 11 2005 14:45:22   mtk00490
 * Add set to factory default
 * 
 *    Rev 1.6   Jul 04 2005 20:56:00   mtk00490
 * Add download time, throughput and fail reason to Microsoft Access
 * 
 *    Rev 1.5   Jun 24 2005 15:24:56   mtk00490
 * Support maximum 16 port download, format only type, multi-bin option
 * 
 *    Rev 1.4   Apr 14 2005 16:08:30   mtk00490
 * add stop button to stop specific or all mobile downloading and formatting
 * 
 *    Rev 1.3   Feb 18 2005 11:18:10   mtk00490
 * Update brom.dll and download agent ver 2.4.1009 to resolve INTEL W18/W30 series flash doesn't support Buffered-Program
 * 
 *    Rev 1.2   Dec 01 2004 10:00:02   mtk00490
 * Multiport download tool ver 2.4.1007 (Add status indicator--download successful:green light, download fail:red light)
 * 
 *    Rev 1.1   Nov 05 2004 17:15:08   mtk00490
 * Multiport download tool ver 2.4.1007 (Support automatic baseband chip and external clock rate detection)
 * 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "form_About.h"

#include "..\eboot\interface.h"

#include "brom.h"

#ifndef _FLASHDOWNLOAD_H_
#include "FlashDownLoad.h"
#endif

#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_AboutMD *Form_AboutMD;

//---------------------------------------------------------------------------
__fastcall TForm_AboutMD::TForm_AboutMD(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TForm_AboutMD::show_brom_dll_version()
{    
    AnsiString as;
    const char *p_version, *p_release_type, *p_build_date;
    
    // get the brom dll information
    Brom_GetDLLInfo(&p_version, &p_release_type, &p_build_date, NULL);
    
    as.printf("%s", p_version);
    lblBromDllVer->Caption = as;
    as.printf("(Build date: %s)", p_build_date);
    lblBromDllBuild->Caption = as;
}

void __fastcall TForm_AboutMD::show_DA_version()
{
    AnsiString  as;
    DA_INFO da_info;

    if( !DA_GetInfo( g_DA_HANDLE, &da_info) )
    {
        if( strlen(da_info.version) != 0 )
        {
            as.printf("DA_v%s",da_info.version);
            lblDAVer->Caption = as;

            if( strlen( da_info.last_modified_date ) != 0 )
            {
                as.printf( "(Build date: %s)", da_info.last_modified_date );
                lblDABuild->Caption = as;
            }
            else
            {
                lblDAVer->Caption = "";
            }
        }
        else{
            lblDAVer->Caption    = "";
            lblDABuild->Caption = "";
        }
    }
    else
    {
        lblDAVer->Caption    = "";
        lblDABuild->Caption = "";
    }
    
}

//---------------------------------------------------------------------------
void __fastcall TForm_AboutMD::show_eboot_dll_version()
{
    AnsiString  as;
    const char *p_version, *p_release_type, *p_build_date;

    Eboot_GetDLLInfo(&p_version, &p_release_type, &p_build_date, NULL);
    
    as.printf("%s", p_version);
    lblEbootDllVer->Caption = as;
    as.printf("(Build-Date: %s)", p_build_date);
    lblEbootDllBuild->Caption = as;
}

void __fastcall TForm_AboutMD::show_SPMETA_dll_version()
{
    AnsiString  as;
    const char *p_version, *p_release_type, *p_build_date, *p_local_path;
/*    
    HINSTANCE h_SPMeta = LoadLibrary("WMMETA_DLL.dll");
    typedef void (*SP_META_GETDLLINFO)(const char **pp_version, const char **pp_release_type, const char **pp_build_date, const char **pp_load_path);
    SP_META_GETDLLINFO SPMetaGetDLLInfo = (SP_META_GETDLLINFO)GetProcAddress(h_SPMeta, "SP_META_GetDLLInfo");
    
    SPMetaGetDLLInfo(&p_version, &p_release_type, &p_build_date, NULL);
*/
    SP_META_GetDLLInfo(&p_version, &p_release_type, &p_build_date, &p_local_path);
    as.printf("%s", p_version);
    lblSPMETAVer->Caption = as;
    as.printf("(Build date: %s)", p_build_date);
    lblSPMETABuild->Caption = as;

//    FreeLibrary(h_SPMeta);
}

//---------------------------------------------------------------------------
void __fastcall TForm_AboutMD::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_AboutMD::from_show(TObject * Sender)
{
    //TODO: Add your source code here
    show_brom_dll_version();
    show_DA_version();
    show_eboot_dll_version();
    show_SPMETA_dll_version();
}

