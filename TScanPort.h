#ifndef __SCAN_PORT_H
#define __SCAN_PORT_H

#include <Classes.hpp>

#ifndef __COM_PORT_UTILS_H
#include "COMPortUtils.h"
#endif

#ifndef	_FLASHTOOL_HANDLE_H_
#include "flashtool_handle.h"
#endif

#ifndef _MULTIPORT_COMMON_H_
#include "multiport_common.h"
#endif

#ifndef _FORM_MULTILOAD_H_
#include "Form\form_MultiLoad.h"
#endif

#ifndef _SLA_CHALLENGE_H_
#include "sla_challenge.h"
#endif
static int __stdcall cb_scancom_download_da_init(void *usr_arg);
static int __stdcall cb_scancom_in_brom_stage(unsigned int brom_handle, HANDLE hCOM, void *usr_arg);
static int __stdcall cb_scancom_download_da(unsigned char percent, unsigned int sent_bytes,
    unsigned int total_bytes, void *usr_arg);


typedef enum {
	 SCAN_BROM_AND_PRELOADER = 0	 // scan brom and preloader comport
    ,SCAN_PRELOADER_ONLY			// scan preloader comport only
    ,UNKNOWN_SCAN_TYPE = 0xFFFF
} E_ScanComportType;

class TScanPort : public TThread
{
public:
    TScanPort(bool CreateSuspended, bool * pIndexArray, unsigned int arraySize, E_ScanComportType eScanType,bool * stopFlag);
    int _fastcall GetComportIndex();

    AnsiString prefix;
    S_DN_PARAM_T   m_sDnParam;
    FlashTool_Connect_Arg  m_sConnArg;
    FlashTool_Connect_Result  m_sConnResult;
    FLASHTOOL_API_HANDLE_T     m_ft_api_handle;


    //progress of call back function
    void _fastcall SyncUpdateProgressColor(TColor bk, TColor ft);
    void _fastcall UpdateProgressColor();
    void _fastcall InitProgress();
    void _fastcall SyncInitProgress();
    void _fastcall SyncUpdateProgressPercentage(int precentage);
    void _fastcall UpdateProgressPercentage();
	

protected:
    void __fastcall Execute();

private:
	//===================common function==============================
    void _fastcall UpdateCOMPort();
    void SyncUpdateCOMPort();
    void _fastcall EnableUIForScan();
    void SyncEnableUIForScan();
   
    void _fastcall UpdateProgressScanFail();
    void _fastcall SyncUpdateScanFail();
    void _fastcall SyncUpdateScanSuccess();
    void _fastcall UpdateProgressScanSuccess();
	
	//==================Scan Brom and preloader comport=====================
    void __fastcall ScanBromAndPreloader();
	int _fastcall   ScanBRomComPort(int index);
    int _fastcall   ScanPreLoaderComPort(unsigned int iComNum, int index);
	int __fastcall  DAConnect(const int iComport);
    void __fastcall DADisConnect();
	void __fastcall BootArgSetting(BOOT_FLASHTOOL_ARG	&boot_arg);
    int _fastcall   SwitchToHighSpeedComPort(string strVid, string strPid);

	//==================Scan preloader only comport=========================
	void __fastcall ScanPrealoderOnly();

    //COMPortUtils mCOMFinder;
    bool * mpIndexArray;
    unsigned int mArraySize;
    bool * mpStopFlag;
    bool m_bIsDaConnecting;
    //For update UI only
    int mIndex;
    string mLabel;
    int m_BromComNum;
    int m_PreLoaderComNum;
	E_ScanComportType m_eScanComportType;

    unsigned char m_ucProgressPercentage;
    TColor _bk_col;
    TColor _ft_col;

};

#endif