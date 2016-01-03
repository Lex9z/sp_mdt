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
 *   COMPortUtils.cpp
 *
 * Project:
 * --------
 *   Multiport download
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime$
 * $Log$

 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <vcl.h>
#pragma hdrstop

#include <algorithm>
#include <objbase.h>


#include "COMPortUtils.h"
#include "GCTrace.h"

#ifndef _EBOOT_INTERFACE_H_
#include "..\Eboot\interface.h"
#endif

#if 0
// for android usb port find
DEFINE_GUID(Guid_ANDROID_USB2Ser,0xA5DCBF10L,0x6530,0x11D2,0x90,0x1F,0x00,0xC0,0x4F,
                            0xB9,0x51,0xED);


#define     PID_PROLOADER       "2000"
#define     PID_DA_HS      "2001"
#define     PID_BOOTROM        "0003"
#define     VID_ANDROID       "0E8D"

#define  InterfaceClassGuidConstant  Guid_ANDROID_USB2Ser
#endif

COMPortUtils::COMPortUtils()
{
	mCustomerSetup.ReadCustomerSettings();
	mPreloaderPid = mCustomerSetup.GetPreloaderPid();
	mBromPid = mCustomerSetup.GetBromPid();
	mAndroidVid = mCustomerSetup.GetAndroidVid();
	mDAHighSpeedPid = mCustomerSetup.GetDAHighSpeedPid();

	mGuid = mCustomerSetup.GetGUID();

}

COMPortUtils::~COMPortUtils()
{
}

bool COMPortUtils::GetNewPortSymbolicName(string & strNewPort, int comportType)
{
    MySet setDiffPort;

    //Get current com port
    ScanSymbolicNameByDDK(false, comportType);

    //Compare original COM port with current
    COMPortSetDiff(mSetCurPortSym.begin(), mSetCurPortSym.end(), mSetOriPortSym.begin(), mSetOriPortSym.end(), setDiffPort);
    if(0 == setDiffPort.size())
    {
        GC_TRACE( "<COMPortUtils> GetNewPortSymbolicName(): No new COM port is found!");
        return false;
    }

    if(1 < setDiffPort.size() )
    {
        GC_TRACE( "<COMPortUtils> GetNewPortSymbolicName(): [Warning] Not only one new COM port is found! Num:%d", setDiffPort.size());
    }

    strNewPort = *(setDiffPort.begin());

    mSetOriPortSym.clear();
    COMPortSetCopy(mSetCurPortSym.begin(), mSetCurPortSym.end(), mSetOriPortSym);

    return true;
}

void COMPortUtils::ScanSymbolicNameByDDK(bool isFirstScan, int comportType)
{
    unsigned long	devNum = 0;
    char			symName[512];
    unsigned long	symNameLen;
    char			friName[512];
    unsigned long	friNameLen;

    //Clean set
    if(isFirstScan)
    {
        mSetOriPortSym.clear();
    }
    else
    {
        mSetCurPortSym.clear();
    }

	if (BOOTROM_COMPORT == comportType)
	{
		devNum =  GetDeviceNumber(mGuid, mBromPid.c_str(), mAndroidVid.c_str());
	}
	else if (PRELOADER_COMPORT == comportType)
	{
		devNum = GetDeviceNumber(mGuid, mPreloaderPid.c_str(), mAndroidVid.c_str());
	}

    //GC_TRACE( "<COMPortUtils> ScanSymbolicNameByDDK(): There is %d devices on PC.", devNum);
    if( devNum == 0)
    {
        return;
    }

    for(unsigned int i = 0; i < devNum; i++)
    {
		if (BOOTROM_COMPORT == comportType)
		{
			 if(GetPresentDevicePath(mGuid, mBromPid.c_str(), mAndroidVid.c_str(), i,
				friName, 512, (unsigned long *) &symNameLen,
				symName, 512, (unsigned long *) &symNameLen))
			{
				if(isFirstScan)
				{
					mSetOriPortSym.insert(symName);
				}
				else
				{
					mSetCurPortSym.insert(symName);
				}

			}
		}
		else if (PRELOADER_COMPORT == comportType)
		{
			if(GetPresentDevicePath(mGuid, mPreloaderPid.c_str(), mAndroidVid.c_str(), i,
				friName, 512, (unsigned long *) &symNameLen,
				symName, 512, (unsigned long *) &symNameLen))
			{
				if(isFirstScan)
				{
					mSetOriPortSym.insert(symName);
				}
				else
				{
					mSetCurPortSym.insert(symName);
				}
			}
		}
        else
        {
            GC_TRACE("<COMPortUtils> ScanSymbolicNameByDDK() fail!");
        }
    }
}


bool COMPortUtils::GetNewPortFriendlyName(string & strNewPort, const int comportType)
{
    MySet setDiffPort;

    //Get current com port
    ScanFriendlyNameByDDK(false, comportType);

    //Compare original COM port with current
    COMPortSetDiff(mSetCurPortFri.begin(), mSetCurPortFri.end(), mSetOriPortFri.begin(), mSetOriPortFri.end(), setDiffPort);
    if(0 == setDiffPort.size())
    {
        GC_TRACE( "<COMPortUtils> GetNewPortFriendlyName(): No new COM port is found!");
        return false;
    }

    if(1 < setDiffPort.size())
    {
        GC_TRACE( "<COMPortUtils> GetNewPortFriendlyName(): [Warning] Not only one new COM port is found! Num:%d", setDiffPort.size());
    }

    strNewPort = *(setDiffPort.begin());

    mSetOriPortFri.clear();
    COMPortSetCopy(mSetCurPortFri.begin(), mSetCurPortFri.end(), mSetOriPortFri);

    return true;
}

void COMPortUtils::ScanFriendlyNameByDDK(bool isFirstScan, const int comportType)
{
    unsigned long	devNum = 0;
    char			symName[512];
    unsigned long	symNameLen;
    char			friName[512];
    unsigned long	friNameLen;

    //Clean set
    if(isFirstScan)
    {
        mSetOriPortFri.clear();
    }
    else
    {
        mSetCurPortFri.clear();
    }

    if (BOOTROM_COMPORT == comportType){
        devNum =  GetDeviceNumber(mGuid, mBromPid.c_str(), mAndroidVid.c_str());
    }
    else if (PRELOADER_COMPORT == comportType){
        devNum = GetDeviceNumber(mGuid, mPreloaderPid.c_str(), mAndroidVid.c_str());
    }

    //GC_TRACE( "<COMPortUtils> ScanFriendlyNameByDDK(): There is %d devices on PC.", devNum);
    if( devNum == 0)
    {
        return;
    }

    for(unsigned int i = 0; i < devNum; i++){
        if (BOOTROM_COMPORT == comportType){
            if(GetPresentDevicePath(mGuid, mBromPid.c_str(), mAndroidVid.c_str(), i,
               friName, 512, (unsigned long *) &symNameLen,
               symName, 512, (unsigned long *) &symNameLen)){
                if(isFirstScan){
                    mSetOriPortFri.insert(friName);
                }
                else{
                mSetCurPortFri.insert(friName);
                }
            }
		    }
		    else if (PRELOADER_COMPORT == comportType){
            if(GetPresentDevicePath(mGuid, mPreloaderPid.c_str(), mAndroidVid.c_str(), i,
               friName, 512, (unsigned long *) &symNameLen,
               symName, 512, (unsigned long *) &symNameLen)){
                if(isFirstScan){
                    mSetOriPortFri.insert(friName);
                }
                else{
                    mSetCurPortFri.insert(friName);
                }
            }
        }
        else
        {
            GC_TRACE("<COMPortUtils> ScanFriendlyNameByDDK() fail!");
        }
    }
}

bool COMPortUtils::ExtractCOMLabel(string & friendly, string & label)
{
    string tagHead("COM");
    string tagTail(")");
    size_t psHead, psTail;

    psHead = friendly.rfind(tagHead);
    psTail = friendly.rfind(tagTail);
    if ((psHead != string::npos) && (psTail != string::npos))
    {
        label = friendly.substr(psHead, psTail - psHead);
        return true;
    }
    else
    {
        return false;
    }
}

bool COMPortUtils::ExtractCOMNumber(string & friendly, int & number)
{
    string tagHead("COM");
    string tagTail(")");
    string tmp;
    size_t psHead, psTail;

    psHead = friendly.rfind(tagHead);
    psTail = friendly.rfind(tagTail);
    if ((psHead != string::npos) && (psTail != string::npos))
    {
        tmp = friendly.substr(psHead + tagHead.size(), psTail - psHead);
        number = atoi(tmp.c_str());
        if(0 == number)
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

void COMPortUtils::COMPortSetDiff(MyIter curBegin, MyIter curEnd, MyIter oriBegin, MyIter oriEnd, MySet & diff)
{
    MyIter curIt, oriIt;
    string tmp;
    for(curIt = curBegin; curIt != curEnd; curIt++)
    {
        tmp = *curIt;
        for(oriIt = oriBegin; oriIt != oriEnd; oriIt++)
        {
            if(0 == tmp.compare(*oriIt))
            {
                break;
            }
        }

        if(oriIt == oriEnd)
        {
            diff.insert(tmp);
        }
    }
}

void COMPortUtils::COMPortSetCopy(MyIter srcBegin, MyIter srcEnd, MySet & dest)
{
    MyIter it;
    for(it = srcBegin; it != srcEnd; it++)
    {
        dest.insert(*it);
    }
}

string COMPortUtils::GetAndroidVid(void)
{
    return mAndroidVid;
}
string COMPortUtils::GetPreloaderPid(void)
{
    return mPreloaderPid;
}





