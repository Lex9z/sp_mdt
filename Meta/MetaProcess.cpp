
#include "MetaProcess.h"
#include "GCTrace.h"
#include "SP_META_Wrapper.h"
#include <stdio.h>

#define META_CONNECT_TIMEOUT 10*1000
static const GUID GUID_PORT_CLASS_USB2SER = {0x4D36E978L,0xE325,0x11CE,{0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}};


MetaProcess::MetaProcess(S_Meta_PARAM_T sMetaParam)
{
    m_eMetaConnectStatus = false;
    m_sMetaParam = sMetaParam;
}

MetaProcess::MetaProcess()
{
    m_eMetaConnectStatus = false;
    memset(&m_sMetaParam, 0, sizeof(m_sMetaParam));
}


MetaProcess::~MetaProcess()
{

}

void MetaProcess::InitMetaProcessParam(S_Meta_PARAM_T sMetaParam)
{
    m_sMetaParam = sMetaParam;
}

META_RESULT MetaProcess::EnterAPMetaMode()
{
    SP_Brom_DebugOn();
	SP_META_Wrapper_TraceOn();
    GC_TRACE ( "EnterAPMetaMode() : Enter...");
    META_RESULT spMetaResult = META_SUCCESS;
    int bootResult = 0;
	m_hSPMetaHandle = INVALID_META_HANDLE;

	spMetaResult = MetaHandle_Init();
    if (spMetaResult != META_SUCCESS)
    {       
        return -1;
    }

	InitMetaModeParameters();

	if(CheckPreloaderPortAvalible())
    {
       return -1;
    }
	
    bootResult = ConnectWithPreloader();
	
    if (bootResult != 0)
    {
        spMetaResult = bootResult;
    }
    else
    {
        bootResult = ConnectWithKernelPort(AP_META_MODE);
       
        if(bootResult == META_SUCCESS)
        {
            m_eMetaConnectMode = AP_META_MODE;
            m_eMetaConnectStatus = true;
			GC_TRACE ( "EnterAPMetaMode() : ConnectWithKernelPort() success!");
        }
		else
        {
            m_eMetaConnectMode = UNKNOWN_MODE;
            m_eMetaConnectStatus = false;
			GC_TRACE ( "EnterAPMetaMode() : ConnectWithKernelPort() fail, ret(%d)!", bootResult);
        }
    }
    
    GC_TRACE ("EnterAPMetaMode() : End!");
    spMetaResult = bootResult;
	
	SP_META_Deinit_r(&m_hSPMetaHandle);
    return spMetaResult;
}

META_RESULT MetaProcess::MetaHandle_Init()
{
    META_RESULT meta_result; 
    META_RESULT spMetaResult;
        
    if (m_hSPMetaHandle == INVALID_META_HANDLE)
    {
        spMetaResult = APMetaHandle_Init();
        if (spMetaResult != META_SUCCESS)
        {
            meta_result = (META_RESULT)spMetaResult;
            return meta_result;
        }
    }
     
    return META_SUCCESS;
}

META_RESULT MetaProcess::APMetaHandle_Init()
{
    META_RESULT spMetaResult =  META_SUCCESS;
    spMetaResult = SP_META_GetAvailableHandle ( &m_hSPMetaHandle );
    if (spMetaResult != META_SUCCESS )
    {     
       
        return spMetaResult;
    } 

    
    spMetaResult = SP_META_Init_r (m_hSPMetaHandle , NULL);
    if ( spMetaResult != META_SUCCESS)
    {     
        return spMetaResult;
    }
    
    return META_SUCCESS;
}

void MetaProcess::InitMetaModeParameters()
{
    GC_TRACE ( "InitMetaModeParameters() : Enter...");
	//For preloader handshake parameters
    memset (&m_SpBootArgs, 0x0, sizeof(SP_BOOT_ARG_S));

    m_iKernelComport = m_sMetaParam.iCOMPort;
		
    //Old parameters
    m_SpBootArgs.m_bbchip_type=SP_AUTO_DETECT_BBCHIP;
    m_SpBootArgs.m_ext_clock=SP_AUTO_DETECT_EXT_CLOCK;
    m_SpBootArgs.m_ms_boot_timeout=SP_BOOT_INFINITE;
    //m_stModeArg.m_max_start_cmd_retry_count = SP_DEFAULT_BROM_START_CMD_RETRY_COUNT;
    m_SpBootArgs.m_max_start_cmd_retry_count = 0;   
    //New parameters
    m_SpBootArgs.m_uTimeout =   20000;
    m_SpBootArgs.m_uRetryTime = 2000;
    m_SpBootArgs.m_uInterval =  10;
    m_SpBootArgs.m_uBaudrate =  CBR_115200;
    m_SpBootArgs.m_pStopFlag  = &m_pMetaStopFlag;
    
 
	
    m_SpBootArgs.m_bIsUSBEnable = m_sMetaParam.bUsbEnable;   
    m_SpBootArgs.m_bIsSymbolicEnable = false;
    m_SpBootArgs.m_bIsCompositeDeviceEnable = false;
    m_SpBootArgs.m_euBootMode = SP_META_BOOT;
    m_SpBootArgs.m_uPortNumber = 0;
    m_SpBootArgs.m_uMDMode = 0;

	/*if (g_sMetaComm.bSecurityUSB)
    {
        m_SpBootArgs.m_auth_handle = SPATE_Get_AuthHandle();
        m_SpBootArgs.m_scert_handle = SPATE_Get_ScertHandle();
        m_SpBootArgs.m_cb_sla_challenge = SLA_Challenge;
        m_SpBootArgs.m_cb_sla_challenge_arg = NULL;
        m_stModeArg.m_cb_sla_challenge_end = SLA_Challenge_END;
        m_SpBootArgs.m_cb_sla_challenge_end_arg = NULL; 
    }
    else*/
    {
        m_SpBootArgs.m_auth_handle = NULL;
        m_SpBootArgs.m_scert_handle = NULL;
        m_SpBootArgs.m_cb_sla_challenge = NULL;
        m_SpBootArgs.m_cb_sla_challenge_arg = NULL;
        m_SpBootArgs.m_cb_sla_challenge_end = NULL;
        m_SpBootArgs.m_cb_sla_challenge_end_arg = NULL;
    }      

/*
    //Common
    m_tMetaReq_Ex.com_port = m_iKernelComport;
    m_tMetaReq_Ex.ms_connect_timeout = g_sMetaComm.iKernel_Connect_Timeout;
    m_tMetaReq_Ex.close_com_port = 1;
    m_tMetaReq_Ex.InMetaMode = 1;
    m_tMetaReq_Ex.protocol = 1;          // DHL
    m_tMetaReq_Ex.channel_type = 1;      // Native channel
    m_tMetaReq_Ex.boot_meta_arg.m_bbchip_type = AUTO_DETECT_BBCHIP; 
    m_tMetaReq_Ex.boot_meta_arg.m_ext_clock = AUTO_DETECT_EXT_CLOCK;
    //UART parameters
    m_tMetaReq_Ex.baudrate[0] = META_BAUD115200;
    m_tMetaReq_Ex.baudrate[1] = META_BAUD230400;
    m_tMetaReq_Ex.baudrate[2] = META_BAUD460800;
    m_tMetaReq_Ex.baudrate[3] = META_BAUD921600;
    m_tMetaReq_Ex.baudrate[4] = META_BAUD_END;   
    m_tMetaReq_Ex.flowctrl = META_SW_FLOWCTRL;
    //USB
    if (g_sMetaComm.bUsbEnable)
    {
        MTRACE (g_hEBOOT_DEBUG, "SmartPhoneSN::SetupMetaModeParameters(): Target enter meta mode by [USB].");
        m_tMetaReq_Ex.usb_enable = 1;
        m_tMetaReq_Ex.boot_meta_arg.m_usb_enable = _TRUE;       
    } 
    else
    {
        MTRACE (g_hEBOOT_DEBUG, "SmartPhoneSN::SetupMetaModeParameters(): Target enter meta mode by [UART].");
        m_tMetaReq_Ex.usb_enable = 0;
        m_tMetaReq_Ex.boot_meta_arg.m_usb_enable = _FALSE;
        m_tMetaReq_Ex.escape = 1;
    }                    
    
    
    MTRACE (g_hEBOOT_DEBUG, "SmartPhoneSN::SetupMetaModeParameters() end...");*/
}

int MetaProcess::CheckPreloaderPortAvalible()
{
	HANDLE hCOM = NULL;
	
	int iPreLoaderComNum = m_sMetaParam.iCOMPort;
	//int * p_stopflag = MainForm->_t_manager.Get_stop_flag_addr(i);
	char pPreLoaderComStr[128];
	
	sprintf(pPreLoaderComStr,"\\\\.\\COM%d", iPreLoaderComNum);
	int Int_tryTimes=0;
	//GC_TRACE("[COM%d] CheckPortAvailable(): Try to open COM%d port...(Polling),comStr = %s", m_sComPortSetting.com.number, iBromComNum, pBromComStr);
	
	do
	{
		/*
		if(BOOT_STOP == *p_stopflag)
		{
			GC_TRACE("[COM%d] CheckPortAvailable(): User press stop button!", m_sComPortSetting.com.number);
			return -1;
		}
	*/
			//Try to open preloader comport
		if(iPreLoaderComNum >0){
			hCOM = CreateFile( pPreLoaderComStr, GENERIC_READ |GENERIC_WRITE, 0, NULL,
															OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if(INVALID_HANDLE_VALUE == hCOM){
				//GC_TRACE("[COM%d] CheckPortAvailable(): Cannot open preloader USB port!", m_sComPortSetting.com.number);
			}
			else{
				GC_TRACE("[COM%d] CheckPortAvailable(): ++++++Open preloader USB succeed!++++++", m_sMetaParam.iCOMPort);
				CloseHandle(hCOM);
				return 0;
			}
		}
		Sleep(10);
		Int_tryTimes++;
		if(Int_tryTimes>3000)
			{
			GC_TRACE("check preloader port %d times fail", Int_tryTimes);
			return -1;
			}
			
	}while(1);

}


int MetaProcess::ConnectWithPreloader()
{
    GC_TRACE("ConnectWithPreloader(): Enter...");
	GC_TRACE("ConnectWithPreloader(): Comport number is %d!", m_sMetaParam.iCOMPort);

    int iRet = 0;
    unsigned int eType;   //0:BoorROMUSB,1:PreloaderUSB

    if (m_sMetaParam.bUsbEnable == false)
    {
         GC_TRACE("ConnectWithPreloader(): Using Uart Port!");
    } 
    else
    {
		GC_TRACE("ConnectWithPreloader(): Using USB Port!!");
		m_SpBootArgs.m_uPortNumber = m_sMetaParam.iCOMPort;
		m_SpBootArgs.m_bIsUSBEnable = true;
		iRet = SP_Preloader_BootMode(&m_SpBootArgs);
    }
   
    Sleep (0.01);
   
    return iRet;
}

int MetaProcess::ConnectWithKernelPort(E_META_MODE eMetaMode)
{
    GC_TRACE("ConnectWithKernelPort(): Start to get dynamic kernel comport...");
    
    int iRet = -1;  
    char *pFuncName = NULL;
    unsigned short kernelComport = 0;  
    META_RESULT MetaResult = META_SUCCESS;
	
	META_ConnectByUSB_Req     spUSBConnReq;
    META_ConnectByUSB_Report  spUSBConnReport;  
    memset(&spUSBConnReq, 0, sizeof(spUSBConnReq));
    memset(&spUSBConnReport, 0, sizeof(spUSBConnReport));
	  
    if (m_sMetaParam.bUsbEnable)
    {
        if(m_sMetaParam.iKernelComPort == 0)
        {
            kernelComport  = ScanSpKernelComport();
            if(kernelComport == 0)
            {
                GC_TRACE("ConnectWithKernelPort(): Get Kernel com fail!");
                return iRet; 
            }
		
            GC_TRACE("ConnectWithKernelPort(): Get Kernel com success! Kernel comport is %d",kernelComport);
            m_iKernelComport = kernelComport;   
		}
		else
		{
		   kernelComport  = m_sMetaParam.iKernelComPort;
		}
		   

        iRet = TryToOpenSPKernelComport(kernelComport);
		
        if(iRet == S_DONE)
        {

			spUSBConnReq.com_port = kernelComport;
            spUSBConnReq.ms_connect_timeout = 5000;
            iRet = SP_META_ConnectInMetaModeByUSB_r(m_hSPMetaHandle, &spUSBConnReq, &m_pMetaStopFlag, &spUSBConnReport);;
            if (iRet == META_SUCCESS )
            {                         
                GC_TRACE("ConnectWithKernelPort(): SP_META_ConnectInMetaModeByUSB_r() Success!");
                #if 0
				iRet =  SP_META_RebootTarget_r(m_hSPMetaHandle,3);
				/*
				if(iRet != META_SUCCESS)
				{
				    GC_TRACE("[COM%d] ConnectWithKernelPort(): [ERROR] SP_META_RebootTarget_r() Fail! m_hSPMetaHandle = %d",m_iPreloaderComport, m_hSPMetaHandle);
				}
				else
				{
				    GC_TRACE("[COM%d] ConnectWithKernelPort(): SP_META_RebootTarget_r() Success! m_hSPMetaHandle = %d",m_iPreloaderComport, m_hSPMetaHandle);
				}
				*/
				#else
                iRet = SP_META_DisconnectInMetaMode_r(m_hSPMetaHandle);
				#endif
            }
            else
            {
                GC_TRACE("ConnectWithKernelPort(): SP_META_ConnectInMetaModeByUSB_r Fail!");
                goto Err;
            }
        }//end if(TryToOpenSPKernelComport(kernelComport) == OK)
                  
   
    } //end if (g_sMetaComm.bUsbEnable)
    #if 0
    else
    {
        kernelComport = m_sMetaParam.iCOMPort;
		m_iKernelComport = kernelComport;
       
        iRet = SP_META_Wrapper_ConnectTargetByUart_r(m_hSPMetaHandle, 15000, kernelComport);
        if (iRet == META_SUCCESS)
        {                          
           
           
        }//end if (iRet == META_SUCCESS)
        else
        {
            goto Err;
        } 
    }
    #endif
	
    return iRet;
    
Err:
    
    return iRet;
}

int MetaProcess::GetKernelComport()
{
    return m_iKernelComport;
}

int MetaProcess::ScanSpKernelComport()
{
    int iRet;
	SP_COM_FILTER_LIST_S sCOMFilter;
	SP_COM_PROPERTY_S sCOMProperty;   
	memset(&sCOMFilter, 0, sizeof(SP_COM_FILTER_LIST_S));
	memset(&sCOMProperty, 0, sizeof(SP_COM_PROPERTY_S));

	const UINT filterNums = 2;
	char *ppFilter[filterNums];
    for (int i = 0; i < filterNums; i++)
    {
        ppFilter[i] = NULL;
    }

	char *asKernelFilter = "VID_0E8D&PID_0003";
    char *asPreloaderPortFilter = "VID_0E8D&PID_2000";
    // Block kernel comport filter list
    ppFilter[0] = asPreloaderPortFilter;
    ppFilter[1] = asKernelFilter;
    
	sCOMFilter.m_uCount = filterNums;
    sCOMFilter.m_ppFilterID = ppFilter;
	sCOMFilter.m_eType = SP_BLACK_LIST;

	bool SCANFLAG = true;
	while(SCANFLAG)
		{
			memset(&sCOMProperty, 0, sizeof(SP_COM_PROPERTY_S));
			iRet = SP_GetIncrementCOMPortWithFilter(&sCOMFilter, &sCOMProperty, &GUID_PORT_CLASS_USB2SER, _FALSE, &m_pMetaStopFlag, META_CONNECT_TIMEOUT); 
			if(META_SUCCESS != iRet)
				{ 
				GC_TRACE("ScanSpKernelComport(): Get Kernel com fail!");
				return 0; 
				}
			int icycle= 0;
			for(icycle=0; icycle<MAX_SUPPORT_PORT_NUM; icycle++)
				{
				if(m_sMetaParam.KernelComPort[icycle] == sCOMProperty.m_uNumber)
					break;
				}
				
			if(icycle == MAX_SUPPORT_PORT_NUM)
				SCANFLAG= false;	
		}
	
	m_iKernelComport = sCOMProperty.m_uNumber;
	return sCOMProperty.m_uNumber;
	
}

//Fix connect with kernel comport probabilistic fail bug
int MetaProcess::TryToOpenSPKernelComport(int KernelCom_Num)
{
  
    HANDLE Hcom;
    char tmp_com[30] = {0};
    sprintf(tmp_com, "\\\\.\\COM%d", KernelCom_Num);
    int retryTime = 0;

	GC_TRACE("TryToOpenSPKernelComport(): Enter...");
    //try to open kernel comport until it ready
    do
    {
        retryTime += 1;
	
       // MTRACE(g_hEBOOT_DEBUG, "SmartPhoneSN::TryToOpenSPKernelComport(): Try to open kernel comport until it ready, retryTime = %d...",retryTime);
        Hcom = CreateFile(tmp_com, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE != Hcom)
        {
            CloseHandle(Hcom);
            GC_TRACE("TryToOpenSPKernelComport(): Success!");
            return S_DONE;
        }
        Sleep(500);
    }while(retryTime <= 100);
    
   // MTRACE_ERR(g_hEBOOT_DEBUG, "SmartPhoneSN::TryToOpenSPKernelComport(): Open kernel comport fail, retryTime = %d...", retryTime);
    return -1;
}



