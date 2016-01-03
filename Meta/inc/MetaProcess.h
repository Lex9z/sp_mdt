
#ifndef _META_PROCESS_
#define _META_PROCESS_
#include "sp_brom.h"
#include "multiport_common.h"

typedef enum
{
    MODEM_META_MODE = 0,
    AP_META_MODE = 1,
    UNKNOWN_MODE = 256
}E_META_MODE;


class MetaProcess
{
public:
	
    MetaProcess();
	MetaProcess(S_Meta_PARAM_T sMetaParam);
	~MetaProcess();

public:
	void              InitMetaProcessParam(S_Meta_PARAM_T sMetaParam);
	META_RESULT       EnterAPMetaMode();
	int               ConnectWithPreloader();
	int               ConnectWithKernelPort(E_META_MODE eMetaMode);
	META_RESULT       MetaHandle_Init();
	META_RESULT       APMetaHandle_Init();
	void              InitMetaModeParameters();
	int               CheckPreloaderPortAvalible();
	int               TryToOpenSPKernelComport(int KernelCom_Num);
	int               ScanSpKernelComport();
	int               GetKernelComport();
public: 
	bool              bUseEnable;
private: 
	E_META_MODE       m_eMetaConnectMode;
	bool              m_eMetaConnectStatus;
	S_Meta_PARAM_T    m_sMetaParam;
	SP_BOOT_ARG_S     m_SpBootArgs;
	int               m_pMetaStopFlag;
	int               m_iKernelComport;
	int               m_hSPMetaHandle;
	
};

#endif