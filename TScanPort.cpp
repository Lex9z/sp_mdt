
#include "TScanPort.h"
#include "GCTrace.h"
#ifndef _MUTLIPORTFLASHDOWNLOADTOOL_H_
#include "MultiPortFlashDownloadTool.h"
#endif


extern TMainForm *MainForm;
extern struct DA_HANDLE	*g_DA_HANDLE;
extern AUTH_HANDLE_T	 g_AUTH_HANDLE ;
extern SCERT_HANDLE_T  g_SCERT_HANDLE;

TScanPort::TScanPort(bool CreateSuspended, bool * pIndexArray, unsigned int arraySize, E_ScanComportType eScanType,bool * stopFlag)
        : TThread(CreateSuspended), mpIndexArray(pIndexArray), mArraySize(arraySize), m_eScanComportType(eScanType),mpStopFlag(stopFlag)
{

}

void __fastcall TScanPort::Execute()
{
    switch(m_eScanComportType)
	{
        case SCAN_BROM_AND_PRELOADER:
			GC_TRACE("<TScanPort> Execute(): Scan brom and prealoder begin...");
			ScanBromAndPreloader();
			break;
		case SCAN_PRELOADER_ONLY:
			GC_TRACE("<TScanPort> Execute(): Scan prealoder begin...");
		    ScanPrealoderOnly();
			break;
		default:
			break;
	}
	
    
}

void __fastcall TScanPort::ScanBromAndPreloader()
{
	bool * chnChecked = new bool[mArraySize];
    m_bIsDaConnecting = false;
	
    memcpy(chnChecked, mpIndexArray, mArraySize * sizeof(bool));
    //=====================Step1 Scan Brom Comport========================
    for(unsigned int iCycle = 0; (iCycle < mArraySize) && (!(*mpStopFlag));iCycle++){
        if(chnChecked[iCycle]){
            m_BromComNum = 0;
            m_PreLoaderComNum = 0;
	        mIndex = iCycle;
            //Scan brom comport success
            if(S_DONE == ScanBRomComPort(iCycle) && m_BromComNum != 0){
                GC_TRACE("<TScanPort> ScanBromAndPreloader(): ScanBRomComPort success!");
                //Step2 Scan Preloader comport
                if(S_DONE == ScanPreLoaderComPort(m_BromComNum,iCycle)){
                    SyncUpdateScanSuccess();
                    GC_TRACE("<TScanPort> ScanBromAndPreloader(): ScanPreLoaderComPort Success!");
                }
                else{
                    SyncUpdateScanFail();
                    GC_TRACE("<TScanPort> ScanBromAndPreloader(): ScanPreLoaderComPort fail!");
					break;
                }
            }
            else{
                SyncUpdateScanFail();
                GC_TRACE("<TScanPort> ScanBromAndPreloader(): ScanBRomComPort fail!");
				break;
            }
        }
    }
	
    delete [] chnChecked;
    SyncEnableUIForScan();
}

void __fastcall TScanPort::ScanPrealoderOnly()
{
    bool * chnChecked = new bool[mArraySize];
    COMPortUtils comFinder;
    int comportType;
    string friendlyName;
    int number;
	int iRet = S_STOP;
	
    memcpy(chnChecked, mpIndexArray, mArraySize * sizeof(bool));
    //=====================Step1 Scan Brom Comport========================
    for(unsigned int iCycle = 0; (iCycle < mArraySize) && (!(*mpStopFlag));iCycle++){
        if(chnChecked[iCycle]){
            m_BromComNum = 0;
            m_PreLoaderComNum = 0;
			mIndex = iCycle;
			
			//First scan and make comport list
			GC_TRACE("<TScanPort> ScanPrealoderOnly(): First scan...");
			comportType = PRELOADER_COMPORT;
			comFinder.ScanFriendlyNameByDDK(true, comportType);

			//Now we find the new comport and compare with the comport list created by first scan
			GC_TRACE("<TScanPort> ScanPrealoderOnly(): Scan Prealoader COM port for Channel[%d]...", mIndex);
			iRet = S_STOP;
			while(!(*mpStopFlag))
			{
			    if(comFinder.GetNewPortFriendlyName(friendlyName, comportType)){
			        if(comFinder.ExtractCOMNumber(friendlyName, number)){
			            GC_TRACE("<TScanPort> ScanPrealoderOnly(): Find new Prealoader COM port for Channel[%d]! COM%d, comportType = %d", mIndex, number, comportType);
			            mIndex = iCycle;
			            m_PreLoaderComNum = number;
			            SyncUpdateCOMPort();
						iRet = S_DONE;
			            break;
			        }
			    }
				Sleep(500);
			 }

            if(S_DONE == iRet && m_PreLoaderComNum != 0){
				SyncUpdateScanSuccess();
                GC_TRACE("<TScanPort> ScanPrealoderOnly(): ScanPrealoderOnly success!");
            }
            else{
                SyncUpdateScanFail();
                GC_TRACE("<TScanPort> ScanPrealoderOnly(): ScanPrealoderOnly fail!");
				break;
            }
        }
    }
	
    delete [] chnChecked;
    SyncEnableUIForScan();

}

int _fastcall TScanPort::ScanBRomComPort(int index)
{
    string friendlyName;
    string label;
    int number;
    COMPortUtils comFinder;
    int comportType;

    //First scan
    GC_TRACE("<TScanPort> Execute(): First scan...");
    comportType = BOOTROM_COMPORT;
    comFinder.ScanFriendlyNameByDDK(true, comportType);


    GC_TRACE("<TScanPort> Execute(): Scan Bootrom COM port for Channel[%d]...", index);
    while(!(*mpStopFlag))
    {
        if(comFinder.GetNewPortFriendlyName(friendlyName, comportType)){
            if(comFinder.ExtractCOMNumber(friendlyName, number)){
                GC_TRACE("<TScanPort> Execute(): Find new Bootrom COM port for Channel[%d]! COM%d, comportType = %d", index, number, comportType);
                mIndex = index;
                m_BromComNum = number;
                SyncUpdateCOMPort();
                break;
            }
        }
        Sleep(500);
    }
    return S_DONE;
}

int _fastcall TScanPort::ScanPreLoaderComPort(unsigned int iComNum, int index)
{
    int iRet = S_DONE;
    string friendlyName;
    string label;
    int number;
    COMPortUtils comFinder;
    int comportType;

    //============Step1 Download Da and connect dA===============================
    GC_TRACE("[iComNum%d] ScanPreLoaderComPort():Da Connect begin.", iComNum);
    iRet = DAConnect(iComNum);
    if(S_DONE != iRet){
        GC_TRACE("[iComNum%d] ScanPreLoaderComPort(): [ERROR] DAConnect fail!ret=%d", iComNum, iRet);
        return iRet;
    }
    //============Step2 change brom pid & vid to preloader pid & vid=============

    //Step2.1 First scan to get comport list before change preloader pid & vid
    GC_TRACE("<TScanPort> ScanPreLoaderComPort(): First scan...");
    comportType = PRELOADER_COMPORT;
    comFinder.ScanFriendlyNameByDDK(true, comportType);

    //Step2.2 change Brom vid & pid to Preloader pid&vid
    iRet = SwitchToHighSpeedComPort(comFinder.GetAndroidVid(), comFinder.GetPreloaderPid());
    if(S_DONE != iRet)
    {
        DADisConnect();
        return iRet;
    }

    //=============Step3 Scan Preloader comport==================================
    GC_TRACE("<TScanPort> Execute(): Scan preloader COM port for Channel[%d]...", index);
    while(!(*mpStopFlag))
    {
        if(comFinder.GetNewPortFriendlyName(friendlyName, comportType)){
            if( comFinder.ExtractCOMNumber(friendlyName, number)){
                GC_TRACE("<TScanPort> Execute(): Find new preloader COM port for Channel[%d]! COM%d, comportType = %d", index, number, comportType);
                mIndex = index;
                m_PreLoaderComNum = number;
                SyncUpdateCOMPort();
                break;
            }
        }
        Sleep(500);
    }

    //=============Step4 change comport from full speed to high speed comport=================
    iRet = FlashTool_ChangeCOM(&m_ft_api_handle, m_PreLoaderComNum);
	if(iRet != S_DONE)
	{
	    GC_TRACE("<TScanPort> Execute():: [ERROR] Change COM port fail!");
		return iRet;
	}
    DADisConnect();
    return iRet;
}

int __fastcall TScanPort::DAConnect(const int iComport)
{
    int iRet = S_DONE;
    unsigned char iConnetComNum = iComport;
    int force_charge = FORCE_CHARGE_ON;

    if(MainForm->m_miDADLNoBattery->Checked)
    {
        force_charge = FORCE_CHARGE_ON;
    }
    else if(MainForm->m_miDADLBattery->Checked)
    {
        force_charge = FORCE_CHARGE_OFF;
    }
    else if(MainForm->m_miDADLAutoDetect->Checked)
    {
        force_charge = FORCE_CHARGE_AUTO;
    }

    /* set FlashTool_Connect_Arg */
    m_sConnArg.m_com_ms_read_timeout  =  COM_DEFAULT_TIMEOUT;
    m_sConnArg.m_com_ms_write_timeout =  COM_DEFAULT_TIMEOUT;
    BootArgSetting(m_sConnArg.m_boot_arg);
    m_sConnArg.m_cb_security_pre_process_notify = NULL;
    m_sConnArg.m_cb_security_pre_process_notify_arg = NULL;
    m_sConnArg.m_nor_chip_select[0] = CS_0;
    m_sConnArg.m_nor_chip_select[1] = CS_WITH_DECODER;
    m_sConnArg.m_nand_chip_select   = m_sDnParam.s_bbchip_op.b_enable_nfi_cs1?CS_1:CS_0;
    m_sConnArg.m_p_nand_acccon      = NULL  ;
    m_sConnArg.m_boot_arg.m_bmt_block_count = frmMultiLoad->m_BMT_blocks;
    m_sConnArg.m_storage_type = frmMultiLoad->m_Storage_Type;// m_storage_type
    m_sConnArg.m_force_charge = force_charge;
    DL_HANDLE_T *p_dl_handle = MainForm->Get_DlHandle(0);
    m_sConnArg.m_p_dl_handle = *p_dl_handle;

    /* connect to target */
    iRet = FlashTool_Connect(
                            /*index + m_sDnParam.i_FirstComNo,*/
                            iConnetComNum,
                            &m_sConnArg,
                            &m_sConnResult,
                            NULL,
                            MainForm->_t_manager.Get_stop_flag_addr(mIndex),
                            &m_ft_api_handle);
    if(S_DONE != iRet)
    {
        GC_TRACE("[COM%d] DAConnect(): [ERROR] DA connect fail!", iConnetComNum);
        return iRet;
    }
	
    m_bIsDaConnecting = true;
    FlashTool_ChangeBaudrate_Arg  ft_cb_arg;
    ft_cb_arg.m_baudrate                       = MainForm->Get_Baudrate();
    ft_cb_arg.m_baudrate_full_sync_count       = 1;
    ft_cb_arg.m_cb_set_high_speed_baudrate     = NULL;
    ft_cb_arg.m_cb_set_high_speed_baudrate_arg = NULL;

    iRet = FlashTool_ChangeBaudrate(m_ft_api_handle, &ft_cb_arg);
    if(S_DONE != iRet)
    {
        GC_TRACE("[iComNum%d] DAConnect(): [ERROR] FlashTool_ChangeBaudrate fail!", iConnetComNum);
        DADisConnect();
        return iRet;
    }

    return iRet;
}

void __fastcall TScanPort::DADisConnect()
{
    if(m_bIsDaConnecting){
        if(S_DONE == FlashTool_Disconnect(&m_ft_api_handle)){
            m_bIsDaConnecting = false;
        }
    }
}


void __fastcall TScanPort::BootArgSetting(BOOT_FLASHTOOL_ARG	&boot_arg)
{
    if(MT6516_AP == frmMultiLoad->m_chip_type)
    {
        boot_arg.m_bbchip_type = MT6516_AP;
    }
    else
    {
        boot_arg.m_bbchip_type = AUTO_DETECT_BBCHIP;
    }

    //boot_arg.m_bbchip_type = m_sDnParam.e_bbchip_type;
    boot_arg.m_ext_clock = AUTO_DETECT_EXT_CLOCK; //m_sDnParam.e_ext_clock;
    boot_arg.m_ms_boot_timeout = BOOT_INFINITE;
    boot_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;
    boot_arg.m_p_bank0_mem_cfg = m_sDnParam.s_emi_param.b_bank0_auto?NULL:&(m_sDnParam.s_emi_param.ui_bank0_cfg);
    boot_arg.m_p_bank1_mem_cfg = m_sDnParam.s_emi_param.b_bank1_auto?NULL:&(m_sDnParam.s_emi_param.ui_bank1_cfg);
    boot_arg.m_da_handle = g_DA_HANDLE;

    boot_arg.m_cb_download_da_init = cb_scancom_download_da_init;
    boot_arg.m_cb_download_da_init_arg = this;
    boot_arg.m_cb_download_da = cb_scancom_download_da;
    boot_arg.m_cb_download_da_arg = this;

    boot_arg.m_cb_in_brom_stage = cb_scancom_in_brom_stage;
    boot_arg.m_cb_in_brom_stage_arg = this;
    boot_arg.m_speedup_brom_baudrate = _TRUE;

    // WM_BROM_READY_TO_POWER_ON_TGT
    boot_arg.m_ready_power_on_wnd_handle	= NULL;
    boot_arg.m_ready_power_on_wparam		= NULL;
    boot_arg.m_ready_power_on_lparam		= NULL;

    // serial link authentication arg
    boot_arg.m_auth_handle              = g_AUTH_HANDLE;
    boot_arg.m_scert_handle              = g_SCERT_HANDLE;
    boot_arg.m_cb_sla_challenge         = SLA_Challenge;
    boot_arg.m_cb_sla_challenge_arg     = this;
    boot_arg.m_cb_sla_challenge_end     = SLA_Challenge_END;
    boot_arg.m_cb_sla_challenge_end_arg = this;

    // EDGE card
    boot_arg.m_cb_com_init_stage = NULL;
    boot_arg.m_cb_com_init_stage_arg = NULL;

    // EMI Control for bank0 and bank1
    boot_arg.m_p_bank0_mem_cfg            = NULL;
    boot_arg.m_p_bank1_mem_cfg            = NULL;

    boot_arg.m_usb_enable = _FALSE;
}


void _fastcall TScanPort::UpdateCOMPort()
{
    MainForm->UpdateChannel(mIndex, m_BromComNum,m_PreLoaderComNum,true);
}

void TScanPort::SyncUpdateCOMPort()
{
    Synchronize(UpdateCOMPort);
}

void _fastcall TScanPort::EnableUIForScan()
{
    MainForm->EnableAllChannelUI();
    MainForm->Enable_UIComponent();
    MainForm->RefreshUIComponent();
    MainForm->btnStartAll->Enabled = true;
    MainForm->btnScan->Enabled = true;
}

void TScanPort::SyncEnableUIForScan()
{
    Synchronize(EnableUIForScan);
}

int _fastcall TScanPort::GetComportIndex()
{
    return mIndex;
}

int _fastcall TScanPort::SwitchToHighSpeedComPort(string strVid, string strPid)
{
    int iRet = S_DONE;
    unsigned short tempVidPid;
    USB_Setup_Info usb_setup_info;
    //FlashTool_CheckUSBStatus arguments
    FlashTool_USB_Status_Arg usb_status_arg;
    FlashTool_USB_Status_Result  usb_statue_result;
    usb_status_arg.m_cb_usb_status_init = NULL;
    usb_status_arg.cb_usb_status_init_arg = NULL;
    usb_statue_result.usb_speed_status = USB_STATUS_UNKNOWN;
	GC_TRACE("[iComNum%d] SwitchToHighSpeedComPort():enter enum preloader comport.", m_BromComNum);
    iRet = FlashTool_CheckUSBStatus(m_ft_api_handle, &usb_status_arg, &usb_statue_result);
    if (usb_statue_result.usb_speed_status != USB_FULL_SPEED) // If usb is not full speed status, do not switch.
    {
        GC_TRACE("[COM%d] ScanPreLoaderComPort(): The com port is in high speed, do not switch it.",m_BromComNum);
        return -1;
    }
	GC_TRACE("[iComNum%d] SwitchToHighSpeedComPort():FlashTool_CheckUSBStatus, usb_speed_status is %d", m_BromComNum, usb_statue_result.usb_speed_status);
   
    //get new comport of Preloader comport
    usb_setup_info.enable_hs = 1;
    //translate pid&vid str to hex
    sscanf(strVid.c_str(), "%x", &tempVidPid);
    usb_setup_info.vid = tempVidPid;
    sscanf(strPid.c_str(), "%x", &tempVidPid);
    usb_setup_info.pid = tempVidPid;
    if(usb_setup_info.pid == 0 || usb_setup_info.vid == 0)
    {
        usb_setup_info.pid = 0x2000;
        usb_setup_info.vid = 0x0E8D;
    }
    GC_TRACE("[iComNum%d] SwitchToHighSpeedComPort():Preloader pid = 0x%4X, vid =0x%4X", m_BromComNum, usb_setup_info.pid, usb_setup_info.vid);
    iRet = FlashTool_SetupUSBDL_EX(&m_ft_api_handle, &usb_setup_info);
    if( S_DONE != iRet )
    {
        GC_TRACE("[COM%d] SwitchComport(): [ERROR] Setup USB DL fail!,ret=%d",m_BromComNum,iRet);
        return iRet;
    }

    return iRet;
}

//---------------------------------------------------------------------------
void _fastcall TScanPort::SyncInitProgress()
{
    Synchronize(InitProgress);
}

//---------------------------------------------------------------------------
void _fastcall TScanPort::InitProgress()
{
    int i = GetComportIndex();

    MainForm->Set_COMPercentageCaption(i, prefix);
    MainForm->pbCOMArray[i]->Progress = 0;
}

//---------------------------------------------------------------------------
void _fastcall TScanPort::UpdateProgressColor()
{
   int i = GetComportIndex();
   MainForm->SetPbComBackColor(i, _bk_col);
   MainForm->SetPbComForeColor(i, _ft_col);
}

//---------------------------------------------------------------------------
void _fastcall TScanPort::SyncUpdateProgressColor(TColor bk, TColor ft)
{
    _bk_col =  bk;
    _ft_col =  ft;
    Synchronize(UpdateProgressColor);
}

void _fastcall TScanPort::SyncUpdateProgressPercentage(int precentage)
{
   m_ucProgressPercentage = precentage;
   Synchronize(UpdateProgressPercentage);
}

void _fastcall TScanPort::UpdateProgressPercentage()
{
    int i = GetComportIndex();
    MainForm->SetPbComProgress(i, m_ucProgressPercentage);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void _fastcall TScanPort::SyncUpdateScanStart()
{
    Synchronize(UpdateProgressScanStart);
}

void _fastcall TScanPort::UpdateProgressScanStart()
{
    prefix = "";
    _bk_col = clWhite;
    _ft_col = clBlue;
    for(int i=0; i <MAX_SUPPORT_PORT_NUM; i++)
    {
        MainForm->Set_COMPercentageCaption(i, prefix);
        MainForm->pbCOMArray[i]->Progress = 0;
        MainForm->SetPbComBackColor(i, _bk_col);
        MainForm->SetPbComForeColor(i, _ft_col);
    }
}
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void _fastcall TScanPort::SyncUpdateScanSuccess()
{
    Synchronize(UpdateProgressScanSuccess);
}

void _fastcall TScanPort::UpdateProgressScanSuccess()
{
    prefix = "Scan Success! ";
    SyncInitProgress();
    SyncUpdateProgressColor(clWhite, clGreen);
    SyncUpdateProgressPercentage(100);
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void _fastcall TScanPort::SyncUpdateScanFail()
{
    Synchronize(UpdateProgressScanFail);
}

void _fastcall TScanPort::UpdateProgressScanFail()
{
    prefix = "Scan Fail! ";
    SyncInitProgress();
    SyncUpdateProgressColor(clWhite, clGreen);
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*===============================================================================
Region: Callback function for using DA to set up high speed port from full speed
================================================================================*/
//---------------------------------------------------------------------------
static int __stdcall cb_scancom_download_da_init(void *usr_arg)
{
    TScanPort *t=(TScanPort *)usr_arg;
    int i = t->GetComportIndex();
    MainForm->DownLoadTimeSec[i]=0;

    // access
    if (MainForm->Get_EnableFactoryDatabaseLog())
    {
        TDateTime datetime;
        TDateTime date = datetime.CurrentDate();
        TDateTime time = datetime.CurrentTime();
        sTime     stime;
        time.DecodeTime( &stime.hour, &stime.min, &stime.sec, &stime.msec);
        MainForm->SetDnDate( i, date );
        MainForm->SetDnStartTime( i, time );
        MainForm->SetDnStartsTime( i, stime );
    }

    t->prefix = "DA ";
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clRed);

    return 0;
}

//---------------------------------------------------------------------------
static int __stdcall cb_scancom_download_da(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg)
{
    TScanPort *t=(TScanPort *)usr_arg;
    t->SyncUpdateProgressPercentage(percent);
    return 0;
}

static int __stdcall cb_scancom_in_brom_stage(unsigned int brom_handle, HANDLE hCOM, void *usr_arg)
{

  int ret;
  unsigned short	value16;
  TScanPort*t=(TScanPort *)usr_arg;

  if( t->m_sDnParam.b_PullDownGPIO17 && (MT6218B == t->m_sDnParam.e_bbchip_type) )
  {
      // configure GPIO17 mode to GPIO
      value16 = 0x000C;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x0120+0x08)/*GPIO_MODE3_CLR*/, &value16, 1);

      // configure GPIO17 direction as output
      value16 = 0x0002;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x0010+0x04)/*GPIO_DIR2_SET*/, &value16, 1);

      // pull down GPIO17 to disable vibrator
      value16 = 0x0002;
      ret = Brom_Write16Cmd(brom_handle, hCOM, (0x80120000+0x00a0+0x08)/*GPIO_DOUT2_CLR*/, &value16, 1);
      return ret;
  }
  return 0;

}


