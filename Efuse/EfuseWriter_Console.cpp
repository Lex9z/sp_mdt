/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2011
*
*****************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*  EfuseWriter_Console.cpp
*
* Project:
* --------
*   Flash tool
*
* Description:
* ------------
*   Efuse Writer Console
*
* Author:
* -------
*  MediaTek
*
*******************************************************************************/
//---------------------------------------------------------------------------


#include "EfuseWriter_Console.h"
#include "cust_sec_ctrl.h"
#include "GCTrace.h"


using namespace std;
using std::string;

#define MAX_PATH_LEN 260

// define Efuse Arg magic number
#define EFUSE_ACTIVE    0xA59914B3
#define EFUSE_NONE      0x241C8F6D





//
// Strcuture Setup Function
//

// Default BOOT_FLASHTOOL_ARG setup (no use now)
int FillArgBootFlashTool( BOOT_FLASHTOOL_ARG &bft)
{
    memset( &bft, 0, sizeof(bft));

    bft.m_bbchip_type                = AUTO_DETECT_BBCHIP;
    bft.m_ext_clock                  = EXT_26M;

    bft.m_ms_boot_timeout            = BOOT_INFINITE;
    bft.m_max_start_cmd_retry_count  = 1;

    // Initialization for Edge Moudle
    // This callback function will be invoke after COM port is opened
    // You can do some initialization by using this callback function.
    bft.m_cb_com_init_stage          = NULL;
    bft.m_cb_com_init_stage_arg      = NULL;

    // This callback function will be invoke after BootROM start cmd is passed.
    // You can issue other BootROM command by brom_handle and hCOM which
    // provides callback arguments,or do whatever you want otherwise.
    bft.m_cb_in_brom_stage           = NULL;
    bft.m_cb_in_brom_stage_arg       = NULL;

    // speed-up BootROM stage baudrate
    bft.m_speedup_brom_baudrate      = _TRUE;

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message
    bft.m_ready_power_on_wnd_handle  = NULL;
    bft.m_ready_power_on_wparam      = NULL;
    bft.m_ready_power_on_lparam      = NULL;

    // No DA usage for BootROM Efuse write behavior
    bft.m_da_handle = NULL;


    // download by USB
    bft.m_usb_enable                 = (_BOOL) FALSE;

    return 0;
}

// Default FlashTool_Connect_Arg setup (no use now)
int FillArgFlashToolConnect( FlashTool_Connect_Arg &ftConnect)
{

    memset( &ftConnect, 0, sizeof(ftConnect));
    // com port timeout setting
    ftConnect.m_com_ms_read_timeout  = COM_DEFAULT_TIMEOUT;
    ftConnect.m_com_ms_write_timeout = COM_DEFAULT_TIMEOUT;

    ftConnect.m_nor_chip_select[0] = CS_0;
    ftConnect.m_nor_chip_select[1] = CS_WITH_DECODER;
    ftConnect.m_nand_chip_select   = CS_0;

    ftConnect.m_cb_security_pre_process_notify = NULL;
    ftConnect.m_cb_security_pre_process_notify_arg = NULL;

    ftConnect.m_p_nand_acccon = NULL;

    //ftConnect.m_enable_download_without_battery = _TRUE;   // if blow Efuse withour battery, please setup _TRUE

    // fill necessary arguments for BOOT_FLASHTOOL_ARG to get ME ID
    FillArgBootFlashTool(ftConnect.m_boot_arg);
    return 0;
}

// For testing, DIY assign Efuse_Write_Common_Arg
void FillArgEfuseCommon( Efuse_Common_Arg &bft){

    bft.emmc_boot_dis = EFUSE_ACTIVE;
    bft.nand_boot_dis = EFUSE_ACTIVE;
    bft.nand_boot_speedup_dis = EFUSE_ACTIVE;
    bft.spare_blow = EFUSE_ACTIVE;
    bft.spare_num_of_byte = 6;
    //memcpy(bft.spare, temp, 6);
    bft.usb_id_blow = EFUSE_ACTIVE;
    bft.usb_pid = 0x1234;
    bft.usb_vid = 0x5678;

}

// For testing, DIY assign Efuse_Write_Secure_Arg
void FillArgEfuseSecure( Efuse_Secure_Arg &bft){


    bft.acc_en = EFUSE_ACTIVE;
    bft.sbc_en = EFUSE_ACTIVE;
    bft.daa_en = EFUSE_ACTIVE;
    bft.sla_en = EFUSE_ACTIVE;
    bft.ack_en = EFUSE_ACTIVE;
    /* Enable DIS JTAG feature */
#if DIS_JTAG_OPTION
    bft.jtag_dis = EFUSE_ACTIVE;
#endif
    bft.ac_key_blow = EFUSE_ACTIVE;
    //bft.sbc_pubk_blow = EFUSE_ACTIVE;

}

bool checkEnable(unsigned int value)
{
    if(value == EFUSE_ACTIVE)
        return true;
    else
        return false;
}

string Binary2Str(const _BUFFER &ref)
{
    string str;
    char tmp[32] = {0};

    for (U32 i=0; i<ref.buf_len; i+=sizeof(U32))
    {
        sprintf(tmp, "%08X", *(U32*)(ref.buf+i));
        str += tmp;
    }

    return str;
}


// For debugging, print Efuse_Write_Common_Arg
void DumpEfuseCommonArg(Efuse_Common_Arg bft,unsigned int comportNum,bool isReadBack /* = false*/){

    GC_TRACE("[COM%d] ========== Efuse Common Setting ========== ", comportNum);
    GC_TRACE("[COM%d] emmc_boot_dis = %s " , comportNum,(checkEnable(bft.emmc_boot_dis)?"On":"Off"));
    GC_TRACE("[COM%d] nand_boot_dis = %s " , comportNum,(checkEnable(bft.nand_boot_dis)?"On":"Off"));
    GC_TRACE("[COM%d] nand_boot_speedup_dis = %s " , comportNum,(checkEnable(bft.nand_boot_speedup_dis)?"On":"Off"));
    GC_TRACE("[COM%d] spare_blow    = %s " , comportNum,(checkEnable(bft.spare_blow)?"On":"Off"));

    if(bft.spare_blow == EFUSE_ACTIVE)
    {
        if(isReadBack){ //for efuse readback dump
            GC_TRACE("[COM%d] spare_num_of_byte = %d    ", comportNum,bft.spare_num_of_byte) ;
		    GC_TRACE("[COM%d] spare buf len = %d    ", comportNum,bft.spare.buf_len);
            GC_TRACE("[COM%d] spare data string = %s    ",comportNum, Binary2Str(bft.spare).c_str());
        }else{  //for efuse write dump
            GC_TRACE("[COM%d] spare_num_of_byte = %d    ", comportNum,bft.spare_num_of_byte) ;
		    GC_TRACE("[COM%d] spare buf len = %d    ", comportNum,bft.spare.buf_len);
            GC_TRACE("[COM%d] spare data string = %s    ", comportNum,bft.spare.buf);
        }
    }

    GC_TRACE("[COM%d] usb_type      = %d ", comportNum,bft.usbdl_type);
    GC_TRACE("[COM%d] usb_id_blow   = %s ", comportNum,(checkEnable(bft.usb_id_blow)?"On":"Off"));

    if(bft.usb_id_blow == EFUSE_ACTIVE)
    {
       GC_TRACE("[COM%d] usb_pid       = %d ", comportNum,bft.usb_pid);
       GC_TRACE("[COM%d] usb_vid       = %d ", comportNum,bft.usb_vid);
    }

	GC_TRACE("[COM%d] ================End======================= ", comportNum);
}

// For debugging, print Efuse_Write_Secure_Arg
void DumpEfuseSecureArg(Efuse_Secure_Arg bft,unsigned int comportNum,bool isReadBack /* = false*/){
    GC_TRACE("[COM%d] ========== Efuse Secure Setting ========== ", comportNum);
    GC_TRACE("[COM%d] acc_en               = %s ", comportNum,(checkEnable(bft.acc_en)?"On":"Off"));
    GC_TRACE("[COM%d] sbc_en               = %s ", comportNum,(checkEnable(bft.sbc_en)?"On":"Off"));
    GC_TRACE("[COM%d] daa_en               = %s ", comportNum,(checkEnable(bft.daa_en)?"On":"Off"));
    GC_TRACE("[COM%d] sla_en               = %s ", comportNum,(checkEnable(bft.sla_en)?"On":"Off"));
    GC_TRACE("[COM%d] ack_en               = %s ", comportNum,(checkEnable(bft.ack_en)?"On":"Off"));
    GC_TRACE("[COM%d] root_cert_en         = %s ", comportNum,(checkEnable(bft.root_cert_en)?"On":"Off"));
    GC_TRACE("[COM%d] jtag_dis             = %s ", comportNum,(checkEnable(bft.jtag_dis)?"On":"Off"));
	GC_TRACE("[COM%d] Enable_SW_JTAG_CON   = %s ", comportNum,(checkEnable(bft.jtag_sw_con)?"On":"Off"));
	GC_TRACE("[COM%d] Disable_DBGPORT_LOCK = %s ", comportNum,(checkEnable(bft.dbgport_lock_dis)?"On":"Off"));
    GC_TRACE("[COM%d] ack_key_blow         = %s ", comportNum,(checkEnable(bft.ac_key_blow)?"On":"Off"));
	GC_TRACE("[COM%d] sbc_publicK_blow     = %s ", comportNum,(checkEnable(bft.sbc_pubk_blow)?"On":"Off"));
    if(bft.ac_key_blow == EFUSE_ACTIVE){;   
		if(isReadBack){
			GC_TRACE("[COM%d] ack_key len= %d", comportNum,bft.ac_key.buf_len);
		    GC_TRACE("[COM%d] ack_key = %s ", comportNum,Binary2Str(bft.ac_key).c_str());
		}else{
		    GC_TRACE("[COM%d] ack_key len= %d ", comportNum,bft.ac_key.buf_len);
		    GC_TRACE("[COM%d] ack_key = %s ", comportNum,bft.ac_key.buf);
		}
	}
       
    if(bft.sbc_pubk_blow == EFUSE_ACTIVE)
    {
        if(isReadBack){ //for efuse readback dump
            GC_TRACE("[COM%d] SBC Public Key hash len= %d", comportNum,bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len);
            GC_TRACE("[COM%d] SBC Public Key hash = %s ", comportNum,Binary2Str(bft.sbc_pub_key_u.r_sbc_pub_key_hash).c_str());
		}else{
		    GC_TRACE("[COM%d] sbc_public_key_e = %s " , comportNum,bft.sbc_pub_key_u.w_sbc_pub_key.key_e.buf);
            GC_TRACE("[COM%d] sbc_public_key_n = %s " , comportNum,bft.sbc_pub_key_u.w_sbc_pub_key.key_n.buf);
		}
    }

	GC_TRACE("[COM%d] ================End======================= ", comportNum);
}


// For debugging, print Efuse_Write_Secure_Arg
void DumpEfuseSecureArgReadback(Efuse_Secure_Arg bft, unsigned int comportNum,ofstream &fout){

    // dump to scream
    GC_TRACE("[COM%d] ========== ReadBack Efsue Secure Setting  ========== ", comportNum);
    GC_TRACE("[COM%d] acc_en        = %s ",  comportNum,(checkEnable(bft.acc_en)?"On":"Off"));
    GC_TRACE("[COM%d] sbc_en        = %s ",  comportNum,(checkEnable(bft.sbc_en)?"On":"Off"));
    GC_TRACE("[COM%d] daa_en        = %s ",  comportNum,(checkEnable(bft.daa_en)?"On":"Off"));
    GC_TRACE("[COM%d] sla_en        = %s ",  comportNum,(checkEnable(bft.sla_en)?"On":"Off"));
    GC_TRACE("[COM%d] ack_en        = %s ",  comportNum,(checkEnable(bft.ack_en)?"On":"Off"));
    GC_TRACE("[COM%d] root_cert_en  = %s ",  comportNum,(checkEnable(bft.root_cert_en)?"On":"Off"));
    GC_TRACE("[COM%d] jtag_dis      = %s ",  comportNum,(checkEnable(bft.jtag_dis)?"On":"Off"));
	GC_TRACE("[COM%d] Enable_SW_JTAG_CON   = %s ", comportNum,(checkEnable(bft.jtag_sw_con)?"On":"Off"));
	GC_TRACE("[COM%d] Disable_DBGPORT_LOCK = %s ", comportNum,(checkEnable(bft.dbgport_lock_dis)?"On":"Off"));

    // dump ac key
    int *acKey = (int *)bft.ac_key.buf;
    for(unsigned int i =0 ; i < (bft.ac_key.buf_len / sizeof(int)) ; i ++)
        GC_TRACE("AC Key<EfuseWriter> %08x ", *(acKey+i));
    //printf("\n");

    GC_TRACE("<EfuseWriter> SBC Public Key hash   =  0x");

    // dump hash
    int *hash = (int *)bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf;
    for(unsigned int i =0 ; i < (bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len / sizeof(int)) ; i ++)
       GC_TRACE("<EfuseWriter> %08x ", *(hash+i));
    //printf("\n");

    // ==== dump to file
    fout << "========== ReadBack Efsue Secure Setting  ========== " << endl;
    fout << "acc_en        =          " << (checkEnable(bft.acc_en)?"On":"Off") << endl;
    fout << "sbc_en        =          " << (checkEnable(bft.sbc_en)?"On":"Off") << endl;
    fout << "daa_en        =          " << (checkEnable(bft.daa_en)?"On":"Off") << endl;
    fout << "sla_en        =          " << (checkEnable(bft.sla_en)?"On":"Off") << endl;
    fout << "ack_en        =          " <<  (checkEnable(bft.ack_en)?"On":"Off") << endl;
    fout << "root_cert_en  =          " << (checkEnable(bft.root_cert_en)?"On":"Off") << endl;
    fout << "jtag_dis      =          " << (checkEnable(bft.jtag_dis)?"On":"Off") << endl;

    fout << "AC Key        =          0x";

    // dump ac key
    acKey = (int *)bft.ac_key.buf;
    for(unsigned int i =0 ; i < (bft.ac_key.buf_len / sizeof(int)) ; i ++)
        fout << hex << setw(8) <<*(acKey+i) << " ";
    fout << endl;

    fout << "SBC Public Key hash   =  0x";

    // dump hash
    hash = (int *)bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf;
    for(unsigned int i =0 ; i < (bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len / sizeof(int)) ; i ++)
        fout << setw(8) << *(hash+i) << " ";
    fout << endl;

}

void DumpEfuseCommonArgReadback(Efuse_Common_Arg bft, unsigned int comportNum, ofstream &fout){
    GC_TRACE("[COM%d] ========= Readback Efuse CommonSetting ========== ", comportNum);
    GC_TRACE("[COM%d] emmc_boot_dis = %s       ", comportNum,(checkEnable(bft.emmc_boot_dis)?"On":"Off"));
    GC_TRACE("[COM%d] nand_boot_dis = %s       ", comportNum,(checkEnable(bft.nand_boot_dis)?"On":"Off"));
    GC_TRACE("[COM%d] nand_boot_speedup_dis= %s", comportNum,(checkEnable(bft.nand_boot_speedup_dis)?"On":"Off"));

    // dump spare
    int *spare_iptr = (int *)bft.spare.buf;
    unsigned char *spare_cptr = (unsigned char *)(bft.spare.buf + 4);
    GC_TRACE("[COM%d] Spare         =        0x%08x", *spare_iptr);

    GC_TRACE("[COM%d] %02x%02x ", *spare_cptr, *(spare_cptr+1));
    //printf("\n");

    GC_TRACE("[COM%d] usb_type      =  %d", bft.usbdl_type);
    //cout << "usb_pid       =        0x" << hex << setw(4) << bft.usb_pid << endl;
    //cout << "usb_vid       =        0x" << hex << setw(4) << bft.usb_vid << endl;

    // dump to file

    fout << "========= Readback Efuse CommonSetting ========== " << endl;
    fout << "emmc_boot_dis =        " << (checkEnable(bft.emmc_boot_dis)?"On":"Off") << endl;
    fout << "nand_boot_dis =        " << (checkEnable(bft.nand_boot_dis)?"On":"Off") << endl;
    fout << "nand_boot_speedup_dis= " << (checkEnable(bft.nand_boot_speedup_dis)?"On":"Off") << endl;
    //fout << "spare_num_of_byte =    " << bft.spare_num_of_byte << endl ;

    // dump spare
    spare_iptr = (int *)bft.spare.buf;
    spare_cptr = (unsigned char *)(bft.spare.buf + 4);
    fout << "Spare          =       0x" << setw(8) <<*spare_iptr;

    fout << setw(2) << hex << (int)(*spare_cptr) << setw(2) << hex << (int)(*(spare_cptr+1)) << endl;

    fout << "usb_type      =        " << bft.usbdl_type << endl;
    fout << "usb_pid       =        0x" << hex << setw(4) << bft.usb_pid << endl;
    fout << "usb_vid       =        0x" << hex << setw(4) << bft.usb_vid << endl;
}

void DumpEfuseLockArgReadback(Efuse_Lock_Arg bft, unsigned int comportNum, ofstream &fout){
    GC_TRACE("[COM%d]  ========== Efuse Lock Setting =========== ", comportNum);
    GC_TRACE("[COM%d] common_ctrl_lock   = %s", comportNum,(checkEnable(bft.common_ctrl_lock)?"On":"Off"));
    GC_TRACE("[COM%d] usb_id_lock        = %s", comportNum,(checkEnable(bft.usb_id_lock)?"On":"Off"));
    GC_TRACE("[COM%d] spare_lock         = %s", comportNum,(checkEnable(bft.spare_lock)?"On":"Off"));
    GC_TRACE("[COM%d] sec_ctrl_lock      = %s", comportNum,(checkEnable(bft.sec_ctrl_lock)?"On":"Off"));
    GC_TRACE("[COM%d] ackey_lock         = %s", comportNum,(checkEnable(bft.ackey_lock)?"On":"Off"));
    GC_TRACE("[COM%d] sbc_pubk_hash_lock = %s", comportNum,(checkEnable(bft.sbc_pubk_hash_lock)?"On":"Off"));


    // dump to file
    fout << "========== Efuse Lock Setting =========== " << endl;
    fout << "common_ctrl_lock   =   " << (checkEnable(bft.common_ctrl_lock)?"On":"Off") << endl;
    fout << "usb_id_lock        =   " << (checkEnable(bft.usb_id_lock)?"On":"Off") << endl;
    fout << "spare_lock         =   " << (checkEnable(bft.spare_lock)?"On":"Off") << endl;
    fout << "sec_ctrl_lock      =   " << (checkEnable(bft.sec_ctrl_lock)?"On":"Off") << endl;
    fout << "ackey_lock         =   " << (checkEnable(bft.ackey_lock)?"On":"Off") << endl;
    fout << "sbc_pubk_hash_lock =   " << (checkEnable(bft.sbc_pubk_hash_lock)?"On":"Off") << endl;
}


// For debugging, print Efuse_Register_lock_Arg
void DumpEfuseLockArg(Efuse_Lock_Arg bft, unsigned int comportNum){
    GC_TRACE("[COM%d] ========== Efuse Lock Setting =========== ", comportNum);
    GC_TRACE("[COM%d] common_ctrl_lock   = %s", comportNum,(checkEnable(bft.common_ctrl_lock)?"On":"Off"));
    GC_TRACE("[COM%d] usb_id_lock        = %s", comportNum,(checkEnable(bft.usb_id_lock)?"On":"Off"));
    GC_TRACE("[COM%d] spare_lock         = %s", comportNum,(checkEnable(bft.spare_lock)?"On":"Off"));
    GC_TRACE("[COM%d] sec_ctrl_lock      = %s", comportNum,(checkEnable(bft.sec_ctrl_lock)?"On":"Off"));
    GC_TRACE("[COM%d] ackey_lock         = %s", comportNum,(checkEnable(bft.ackey_lock)?"On":"Off"));
    GC_TRACE("[COM%d] sbc_pubk_hash_lock = %s", comportNum,(checkEnable(bft.sbc_pubk_hash_lock)?"On":"Off"));
	GC_TRACE("[COM%d] ================End======================= ", comportNum);
}

// For debugging, print Efuse_STB_Key_Arg
void DumpSTBArg(Efuse_STB_Key_Arg bft, unsigned int comportNum, bool bIsWrite){
	
#define EFUSE_STK_KEY_NUMBER 16
    GC_TRACE("[COM%d] ========== STB Args Setting =========== ", comportNum);
    GC_TRACE("[COM%d] [Part1] STB lock:", comportNum);
    GC_TRACE("[COM%d]   stb_key_g7_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g7_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g6_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g6_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g5_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g5_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g4_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g4_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g3_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g3_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g2_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g2_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g1_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g1_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_g0_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_g0_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_operatorid_lock = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_operatorid_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_chipid_lock     = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_chipid_lock)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_sn_lock         = %s", comportNum,(checkEnable(bft.stb_lock.stb_key_sn_lock)?"On":"Off"));

    GC_TRACE("[COM%d] [Part2] STB ID:", comportNum);
    GC_TRACE("[COM%d]   stb_key_chipid_blow     = %s",     comportNum,(checkEnable(bft.stb_key_chipid_blow)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_chipID          = 0x%04x", comportNum, bft.stb_key_chipID);
    GC_TRACE("[COM%d]   stb_key_operatorid_blow = %s",     comportNum,(checkEnable(bft.stb_key_operatorid_blow)?"On":"Off"));
    GC_TRACE("[COM%d]   stb_key_operatorid      = 0x%04x", comportNum, bft.stb_key_operatorid);

    GC_TRACE("[COM%d] [Part3] STB key:", comportNum);
    for(int i = 0; i < EFUSE_STK_KEY_NUMBER; i++)
    {
        if(bft.stb_blow_keys[i].stb_key.buf_len > 0 && bft.stb_blow_keys[i].key_name != NULL &&strlen(bft.stb_blow_keys[i].key_name) > 0)
        {
            if(bIsWrite)
            {
                GC_TRACE("[COM%d]   %-15s = %s", comportNum, bft.stb_blow_keys[i].key_name, bft.stb_blow_keys[i].stb_key.buf);
            }
			else
			{
			    GC_TRACE("[COM%d]   %-15s = %s", comportNum, bft.stb_blow_keys[i].key_name, Binary2Str(bft.stb_blow_keys[i].stb_key));
			}
        }
    }
	
    GC_TRACE("[COM%d] ================End======================= ", comportNum);
}


// Init buffer for Efuse readback
void InitReadBackEfuseCommonArg( Efuse_Common_Arg &bft){

    // allocate 12 byte buffer for readback spare data (MT6276 only 6 byte spare)
    bft.spare.buf = new char [12];
    bft.spare.buf_len =12;
    memset(bft.spare.buf, 0, bft.spare.buf_len);

}

// Init buffer for Efsue readback
void InitReadBackEfuseSecureArg( Efuse_Secure_Arg &bft){

    // allocate 16 byte for readback AC key
    bft.ac_key.buf = new char [16];
    bft.ac_key.buf_len = 16;
    memset(bft.ac_key.buf, 0, bft.ac_key.buf_len);

    // allocate 32 byte for readback SBC public key hash value
    //KeyHashValue &hash = bft.sbc_pub_key_u.r_sbc_pub_key_hash;

    bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf = new char [32];
    bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len = 32;
    memset(bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf, 0, bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf_len);

}

// Delete buffer for Efuse readback
void DeleteReadBackEfuseCommonArg( Efuse_Common_Arg &bft){

    if(bft.spare.buf != NULL)
        delete [] bft.spare.buf;
}

// Delete buffer for Efsue readback
void DeleteReadBackEfuseSecureArg( Efuse_Secure_Arg &bft){

    if(bft.ac_key.buf != NULL)
        delete [] bft.ac_key.buf;

    //KeyHashValue &hash = bft.sbc_pub_key_u.r_sbc_pub_key_hash;
    if(bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf != NULL)
        delete [] bft.sbc_pub_key_u.r_sbc_pub_key_hash.buf;
}


//
// Targrt Connect Function
//
bool IsUSBCOMPortReady(unsigned short comPortNum)
{
    for (int i=0; i<10; ++i)
    {
        // Wait for 200ms
        Sleep(200);

        // Try to open the COM port to see if it's ready to be used
        HANDLE hCOM = INVALID_HANDLE_VALUE;

        if (open_com_port(hCOM, comPortNum, 115200))
        {
            if (hCOM != NULL)
            {
                close_com_port(hCOM);
            }

            return true;
        }

        if (hCOM != NULL)
        {
            close_com_port(hCOM);
        }
    }

    return false;
}

bool GetDynamicComPort(unsigned short *com_port)
{
    int i, ret;
    unsigned short com_port_list_org[255];
    unsigned short com_port_count_org = sizeof(com_port_list_org)/sizeof(unsigned short);
    unsigned short com_port_list[255];
    unsigned short com_port_count = sizeof(com_port_list)/sizeof(unsigned short);
    bool bFindComPort = false;
    bool b_com_port_list[255];


    if( COM_ENUM_OK != ( ret = ComPortEnumerate(com_port_list_org, &com_port_count_org)) )
    {
        if( COM_ENUM_NO_COM_PORT_FOUND == ret )
        {
            for (int i=0; i<sizeof(com_port_list_org)/sizeof(com_port_list_org[0]); ++i)
            {
                com_port_list_org[i] = 0;
            }

            com_port_count_org =0;
        }
        else {
            GC_TRACE("<EfuseWriter> Failed to enumerate a certain COM port! ");
            return false;
        }
    }

    while(1)
    {
        com_port_count = sizeof(com_port_list)/sizeof(com_port_list[0]);

        if( COM_ENUM_OK != ( ret = ComPortEnumerate(com_port_list, &com_port_count) ) )
        {
            if( COM_ENUM_NO_COM_PORT_FOUND == ret )
            {
                for (int i=0; i<sizeof(com_port_list)/sizeof(com_port_list[0]); ++i)
                {
                    com_port_list[i] = 0;
                }

                com_port_count=0;
                continue;
            }
            else
            {
                continue;
            }
        }

        if( com_port_count == com_port_count_org && !memcmp((void *)com_port_list, (void *)com_port_list_org, com_port_count_org*sizeof(unsigned short)) )
        {
            // COM ports are the same!
            continue;
        }

        for( i = 0; i < 255; i++ ) b_com_port_list[i] = false;
        for( i = 0; i < com_port_count_org; i++ ) b_com_port_list[com_port_list_org[i]] = true;

        for( i = 0; i < com_port_count; i++ )
        {
            if( b_com_port_list[com_port_list[i]] == false )
            {
                // Find a new com port : COM%d",com_port_list[i]);
                *com_port = com_port_list[i];
                bFindComPort = true;
                break;
            }
        }

        if( bFindComPort ) break;

        com_port_count_org = com_port_count;
        memcpy((void*)com_port_list_org, (void*)com_port_list, sizeof(com_port_list)/ sizeof(unsigned short));
    }

    return IsUSBCOMPortReady(*com_port);
}

// Connect to Target BootROM
bool ConnectToTargetBootROM(unsigned int *brom_base, unsigned char ComPort, HANDLE *hCOM, FLASHTOOL_API_HANDLE_T *ft_handle)
{
    int stopFlag = 0;

    // setup bootup argument
    BBCHIP_TYPE bbchip_type = AUTO_DETECT_BBCHIP;
    EXT_CLOCK ext_clock = AUTO_DETECT_EXT_CLOCK;
    unsigned int boot_timeout = 10 * 1000;
    unsigned int max_start_cmd_retry_count = 1;

#if 0
    Brom_Debug_SetLogFilename("EfuseWriter.log");
    Brom_DebugOn();  // open BROM debug mode
#endif


    // Connect to Target
    int ret = Brom_Connect(brom_base, ft_handle, ComPort, bbchip_type, ext_clock, &stopFlag, boot_timeout, max_start_cmd_retry_count);
    GC_TRACE("<EfuseWriter> Finish connect target  ret = [%d]  Get brom_base object = %d" , ret, *brom_base);

    if(S_DONE != ret){
        GC_TRACE("<EfuseWriter> ERROR: FlashTool_Connect() fail [%s]", StatusToString(ret));
        return 1;
    }

    return 0;

}

//
// Command line Parsing Function
//
int AddModulePath(std::string  &ini_filepath)
{
    // if filename without path, append path
    if( '.'==ini_filepath[0] // ".\ABCD.txt"
        || (isalnum(ini_filepath[0]) && strncmp(ini_filepath.c_str()+1, ":\\", 2)) // "ABCD.txt"
    ) {
        char szExeName[MAX_PATH_LEN];
        char *p_last_backslash;

        // get module name
        GetModuleFileName(NULL, szExeName, sizeof (szExeName));
        //MTRACE(g_hBROM_DEBUG, "szExeName=\"%s\".", szExeName);

        // retrieve the path only
        std::string     tmp;
        if( NULL != (p_last_backslash=strrchr(szExeName,'\\')) ) {
            *p_last_backslash = '\0';
            tmp = szExeName;
            tmp += "\\";
            tmp += ini_filepath;
            ini_filepath = tmp;
        }
    }

    return 0;
}

bool DoesFileExist(const string& filename)
{
    DWORD attrib = GetFileAttributes(filename.c_str());
    return (attrib != 0xFFFFFFFF);
}

//
//  Secure File Parsing Function
//
int SecGetProfileInt(const string & pattern, const string & str_ini_file)
{
    FILE *file;
    char *buf = NULL;
    //int value = 0;
    int ret = 0;

    char *r0 = NULL;
    //char *r1 = NULL;
    char *r2 = NULL;

    /* ============================ */
    /* allocate buffer              */
    /* ============================ */
    buf = (char*) malloc(INI_FILE_MAX_SIZE);

    /* ============================ */
    /* read INI file                */
    /* ============================ */
    file = fopen(str_ini_file.c_str(), "rb");
    fread((char*)buf,INI_FILE_MAX_SIZE,1,file);

    /* ============================ */
    /* decrypt INI file if needed   */
    /* ============================ */
    ret = FlashTool_DecryptINI((unsigned char*)buf,(unsigned char*)INI_AES_KEY,AES_KEY_LEN);

    /* ============================ */
    /* skip cipher header if needed */
    /* ============================ */
    if(0 == ret)
    {
        /* INI file is encrypted */
        buf = buf + GetCipherHeaderLen();
    }
    else
    {
#if INI_MUST_BE_ENC
        GC_TRACE("<EfuseWriter> Error. INI Is Not Encrypted");
        goto _fail;
#endif
    }
    GC_TRACE("<EfuseWriter> Dump INI File");
    GC_TRACE("<EfuseWriter> %s", buf);

    /* ============================ */
    /* search pattern               */
    /* ============================ */
    r0 = strtok(buf," ");
    strtok(NULL," ");
    r2 = strtok(NULL,"\n");

    while(r0!=NULL)
    {
        if(0 == strcmp(r0,pattern.c_str()))
        {
            GC_TRACE("<EfuseWriter> %s is found = %d", r0, atoi(r2));
            ret = atoi(r2);
            goto _exit;
        }

        r0 = strtok(NULL," ");
        strtok(NULL," ");
        r2 = strtok(NULL,"\n");
    }

    GC_TRACE("<EfuseWriter> %s is NOT found", pattern.c_str());
    ret = 0;

_exit:
    free(buf);
    fclose(file);
    return ret;

_fail:
    free(buf);
    fclose(file);
    //exit(0);
    return -1;
}

int SecGetProfileString(const string &  pattern, char * str, int len, const string & str_ini_file)
{
    FILE *file;
    char *buf = NULL;
    //int value = 0;
    int ret = 0;

    char *r0 = NULL;
    //char *r1 = NULL;
    char *r2 = NULL;

    /* ============================ */
    /* allocate buffer              */
    /* ============================ */
    buf = (char*) malloc(INI_FILE_MAX_SIZE);

    /* ============================ */
    /* read INI file                */
    /* ============================ */
    file = fopen(str_ini_file.c_str(), "rb");
    fread((char*)buf,INI_FILE_MAX_SIZE,1,file);

    /* ============================ */
    /* decrypt INI file if needed   */
    /* ============================ */
    ret = FlashTool_DecryptINI((unsigned char*)buf,(unsigned char*)INI_AES_KEY,AES_KEY_LEN);

    /* ============================ */
    /* skip cipher header if needed */
    /* ============================ */
    if(0 == ret)
    {
        /* INI file is encrypted */
        buf = buf + GetCipherHeaderLen();
    }
    else
    {
#if INI_MUST_BE_ENC
        GC_TRACE("<EfuseWriter>  Error. INI Is Not Encrypted");
        goto _fail;
#endif
    }
    //GC_TRACE("<EfuseWriter> Dump INI File");
    //GC_TRACE("<EfuseWriter> %s", buf);

    /* ============================ */
    /* search pattern               */
    /* ============================ */
    r0 = strtok(buf," ");
    strtok(NULL," ");
    r2 = strtok(NULL,"\n");

    while(r0!=NULL)
    {
        if(0 == strcmp(r0,pattern.c_str()))
        {
            strncpy(str,r2,len);
            GC_TRACE("<EfuseWriter> %s is found = %s", r0, str);
            ret = strlen(str);
            goto _exit;
        }

        r0 = strtok(NULL," ");
        strtok(NULL," ");
        r2 = strtok(NULL,"\n");
    }

    GC_TRACE("<EfuseWriter> %s is NOT found", pattern.c_str());
    ret = 0;

_exit:
    free(buf);
    return ret;

_fail:
    free(buf);
    //exit(0);
    return -1;
}

//
//  INI Parsing Function
//

int GetEfuseCommonArgFromINI(const string & str_ini_file, Efuse_Common_Arg &common_arg)
{
    // Common Attribute
    int attribute_value = SecGetProfileInt("Disable_EMMC_boot", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        common_arg.emmc_boot_dis = EFUSE_ACTIVE;
    }
    else
    {
        common_arg.emmc_boot_dis = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Disable_NAND_boot", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        common_arg.nand_boot_dis = EFUSE_ACTIVE;
    }
    else
    {
        common_arg.nand_boot_dis = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Disable_NAND_boot_speedup", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        common_arg.nand_boot_speedup_dis = EFUSE_ACTIVE;
    }
    else
    {
        common_arg.nand_boot_speedup_dis = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("USB_download_type", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        common_arg.usbdl_type_blow = EFUSE_ACTIVE;
        common_arg.usbdl_type = 1;
    }
    else
    {
        common_arg.usbdl_type_blow = EFUSE_NONE;
    }

    // USB ID
    char pid[512];
    char vid[512];
    int pid_valid = 0;
    int vid_valid = 0;
    memset(pid, 0, sizeof(pid));
    memset(vid, 0, sizeof(vid));

    // If blow USB_PID, get PID from INI
    pid_valid = SecGetProfileString("USB_PID", pid, sizeof(pid), str_ini_file.c_str());
    vid_valid = SecGetProfileString("USB_VID", vid, sizeof(vid), str_ini_file.c_str());

    if(pid_valid > 0 && vid_valid > 0)
    {
        if(!strncmp(pid, "0x", 2))
        {
            common_arg.usb_pid = (unsigned short)strtoul(pid, NULL, 16);
        }
        else
        {
            common_arg.usb_pid = (unsigned short)strtoul(pid, NULL, 10);
        }

        if(!strncmp(vid, "0x", 2))
        {
            common_arg.usb_vid = (unsigned short) strtoul(vid, NULL, 16);
        }
        else
        {
            common_arg.usb_vid = (unsigned short) strtoul(vid, NULL, 10);
        }

        common_arg.usb_id_blow = EFUSE_ACTIVE;
    }
    else if(pid_valid > 0 || vid_valid > 0) // one of pid or vid data hasn't value
    {
        GC_TRACE("<EfuseWriter> ERROR: input usb id needs both PID and VID");
        return 1;
    }
    else
    {
        common_arg.usb_id_blow = EFUSE_NONE;
    }

    // Spare data
    char spare[512];
    int spare_byte = 0;
    int spare_length =0;
    memset(spare, 0, sizeof(spare));

    // If blow SPARE, get spare size
    spare_byte = SecGetProfileInt("spare_byte", str_ini_file.c_str());
    spare_length = SecGetProfileString("spare_data", spare, sizeof(spare), str_ini_file.c_str());

    if(spare_byte > 0 && spare_length > 0)
    {
        common_arg.spare_num_of_byte = spare_byte;

        // change unaigned char* to char* by Roger
        char *pSpare = new char[spare_length + 1];
        memcpy(pSpare, spare, spare_length);
        pSpare[spare_length] = '\0';
        common_arg.spare.buf = pSpare;
        common_arg.spare.buf_len = spare_length + 1;

        common_arg.spare_blow = EFUSE_ACTIVE;
    }
    else if(spare_byte > 0 || spare_length > 0) // one of spare_byte or spare data hasn't value
    {
        GC_TRACE("<EfuseWriter> ERROR: input spare Data format invalid");
        return 1;
    }
    else
    {
        common_arg.spare_blow = EFUSE_NONE;
    }

    return 0;
}

int GetEfuseSecureArgFromINI(const string & str_ini_file, Efuse_Secure_Arg & secure_arg)
{
    // Secure Attribute
    int attribute_value = SecGetProfileInt("Enable_ACC", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.acc_en = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.acc_en = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Enable_SBC", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.sbc_en = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.sbc_en = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Enable_DAA", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.daa_en = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.daa_en = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Enable_SLA", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.sla_en = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.sla_en = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("Enable_ACK", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.ack_en = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.ack_en = EFUSE_NONE;
    }

#if DIS_JTAG_OPTION
    attribute_value = SecGetProfileInt("Disable_JTAG", str_ini_file.c_str());
    if(attribute_value == 1)
    {
        secure_arg.jtag_dis = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.jtag_dis = EFUSE_NONE;
    }
#endif

    // SBC Public Key
    char pubKey_e[2048];
    char pubKey_n[2048];
    int pubKey_e_length =0;
    int pubKey_n_length =0;
    memset(pubKey_e,0,sizeof(pubKey_e));
    memset(pubKey_n,0,sizeof(pubKey_n));

    pubKey_e_length = SecGetProfileString("public_key_e", pubKey_e,sizeof(pubKey_e), str_ini_file.c_str());
    pubKey_n_length = SecGetProfileString("public_key_n", pubKey_n,sizeof(pubKey_n), str_ini_file.c_str());

    if(pubKey_e_length > 0 && pubKey_n_length > 0)
    {
        // process key string
        char *pPubKeyE = new char[pubKey_e_length + 1];
        memcpy(pPubKeyE, pubKey_e, pubKey_e_length);
        pPubKeyE[pubKey_e_length] = '\0';
        secure_arg.sbc_pub_key_u.w_sbc_pub_key.key_e.buf = pPubKeyE;
        secure_arg.sbc_pub_key_u.w_sbc_pub_key.key_e.buf_len = pubKey_e_length + 1;

        char *pPubKeyN = new char[pubKey_n_length + 1];
        memcpy(pPubKeyN, pubKey_n, pubKey_n_length);
        pPubKeyN[pubKey_n_length] = '\0';
        secure_arg.sbc_pub_key_u.w_sbc_pub_key.key_n.buf = pPubKeyN;
        secure_arg.sbc_pub_key_u.w_sbc_pub_key.key_n.buf_len = pubKey_n_length + 1;

        secure_arg.sbc_pubk_blow = EFUSE_ACTIVE;
    }
    else if(pubKey_e_length > 0 || pubKey_n_length > 0)
    {
        GC_TRACE("<EfuseWriter> ERROR: input SBC public key format invalid");
        return 1;
    }
    else
    {
        secure_arg.sbc_pubk_blow = EFUSE_NONE;
    }

    // AC Key
    char acKey[512];
    int acKey_length;
    memset(acKey,0,sizeof(acKey));

    if( 0 != (acKey_length = SecGetProfileString("ac_key", acKey, sizeof(acKey), str_ini_file.c_str())))
    {
        // process key string
        char *pACKey = new char[acKey_length + 1];
        memcpy(pACKey, acKey, acKey_length);
        pACKey[acKey_length] = '\0';
        secure_arg.ac_key.buf = pACKey;
        secure_arg.ac_key.buf_len = acKey_length + 1;

        secure_arg.ac_key_blow = EFUSE_ACTIVE;
    }
    else
    {
        secure_arg.ac_key_blow = EFUSE_NONE;
    }

    return 0;
}

// Get Lock Argument from ini (lock.ini)
int GetEfuseLockArgFromINI(const string & str_lock_file, Efuse_Lock_Arg & lock_arg){
    int attribute_value;

    attribute_value = SecGetProfileInt("com_ctrl_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.common_ctrl_lock = EFUSE_ACTIVE;

    }
    else
    {
        lock_arg.common_ctrl_lock = EFUSE_NONE;

    }

    attribute_value = SecGetProfileInt("usb_id_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.usb_id_lock = EFUSE_ACTIVE;
    }
    else
    {
        lock_arg.usb_id_lock = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("spare_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.spare_lock = EFUSE_ACTIVE;
    }
    else
    {
        lock_arg.spare_lock = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("sec_attr_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.sec_ctrl_lock = EFUSE_ACTIVE;
    }
    else
    {
        lock_arg.sec_ctrl_lock = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("ackey_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.ackey_lock = EFUSE_ACTIVE;
    }
    else
    {
        lock_arg.ackey_lock = EFUSE_NONE;
    }

    attribute_value = SecGetProfileInt("sbc_pubk_hash_lock", str_lock_file.c_str());
    if(attribute_value == 1)
    {
        lock_arg.sbc_pubk_hash_lock = EFUSE_ACTIVE;
    }
    else
    {
        lock_arg.sbc_pubk_hash_lock = EFUSE_NONE;
    }

    return 0;
}

//
// Sample Efuse Writer Tool
//
//string str_setting_file;            // setting file name string
//string str_lock_file;               // lock file name string
//string str_readback_file;           // readback file name string
//char comPortNum = 0;                // usb or uart com port number
int EfuseWriter(string& str_setting_file, string& str_lock_file, string& str_readback_file, unsigned char comPortNum)
{
    int ret;                            // return value

    Efuse_Lock_Arg lock_arg;            // Arg for Brom_WriteEfuseSecure
    Efuse_Common_Arg common_arg;        // Arg for Brom_WriteEfuseCommon
    Efuse_Secure_Arg secure_arg;        // Arg for Brom_WriteEfuseSecure
    memset(&common_arg, 0, sizeof(common_arg));
    memset(&secure_arg, 0, sizeof(secure_arg));
    memset(&lock_arg, 0, sizeof(lock_arg));

    //
    // parse Efuse_Write_Common_Arg from INI
    //

    if(!str_setting_file.empty()){  // if enter setting file

        if( 0 != ( ret = GetEfuseCommonArgFromINI(str_setting_file, common_arg) ))
        {
            GC_TRACE("<EfuseWriter> ERROR: GetEfuseCommonArgFromINI fail! (code = %d)\n ", ret);
            return ret;
        }
        DumpEfuseCommonArg(common_arg, (unsigned int)comPortNum);     // Debug usage : Dump Efuse_Write_Common_Arg

        //
        // parse Efuse_Write_Common_Arg from INI
        //
        if( 0 != ( ret = GetEfuseSecureArgFromINI(str_setting_file, secure_arg) ))
        {
            GC_TRACE("<EfuseWriter> ERROR: GetEfuseSecureArgFromINI fail! (code = %d)\n ", ret);
            return ret;
        }
        DumpEfuseSecureArg(secure_arg, (unsigned int)comPortNum);     // Debug usage : Dump Efuse_Write_Common_Arg
    }

    //
    // parse Efuse_Write_Common_Arg from INI
    //
    if(!str_lock_file.empty()){     // if enter lock file

        if( 0 != ( ret = GetEfuseLockArgFromINI(str_lock_file, lock_arg) ))
        {
            GC_TRACE("<EfuseWriter> ERROR: GetEfuseLockArgFromINI fail! (code = %d)\n ", ret);
            return ret;
        }
        DumpEfuseLockArg(lock_arg, (unsigned int)comPortNum);     // Debug usage : Dump Efuse_Write_Common_Arg
    }

    //
    // connect to Target phone
    //
    HANDLE hCOM;
    FLASHTOOL_API_HANDLE_T ft_handle;

    unsigned int p_brom_base;  // brom object pointer

    //cout << "[Efuse Writer Console] Please power on the phone..." << endl;

    if(comPortNum == 0){
        // Get dynamic USB COM port by scanning new pop-up COM port
        GC_TRACE("<EfuseWriter>  Try to get dynamic USB COM port...");
        if (!GetDynamicComPort((unsigned short *)&comPortNum))
            return 1;
        int temp = (int) comPortNum;
        GC_TRACE("<EfuseWriter>  Dynamic USB COM port is %d", temp);
    }

    if(0 != ConnectToTargetBootROM( &p_brom_base, comPortNum, &hCOM, &ft_handle))
    {
        GC_TRACE("<EfuseWriter> Connect to Target Failed");
        return 1;
    }

    if(p_brom_base == NULL)
        GC_TRACE("<EfuseWriter> BRom Object get with ERROR");
    else
        GC_TRACE("<EfuseWriter> Brom_base = %x", p_brom_base);

    //
    // write Efuse
    //
    if(!str_setting_file.empty()){ // if enter setting file

        /////Write Efuse common register
        ret = Brom_WriteEfuseCommon(p_brom_base, ft_handle, &common_arg, true);
        if(0 != ret){
            //ERROT handle
            switch(ret)
            {
            case S_BROM_EFUSE_WRITE_TIMEOUT:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed : Efuse blowing timeout (target no response) [%s](%d)",StatusToString(ret),ret);
                break;
            case S_BROM_EFUSE_REG_NO_MATCH_WITH_TARGET:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed : Argument register is not match with target's Efuse [%s](%d)", StatusToString(ret), ret);
                break;
            case S_FT_PROTOCOL_EXCEPTION:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed : BootROM connect faild [%s](%d)",StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_DATA_PROCESS_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed : Key data processing error [%s](%d)",StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_BLOW_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed : Efuse blowing error (write data error) [%s](%d)", StatusToString(ret), ret);
                break;
            default:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Failed :  [%s](%d)", StatusToString(ret), ret);
            }

            goto finish;
        }
        else{
            GC_TRACE("<EfuseWriter> Brom_WriteEfuseCommon() Done");
        }

        /////Write Efuse secure register


        ret = Brom_WriteEfuseSecure(p_brom_base, ft_handle, &secure_arg, true);
        if(0 != ret){
            //ERROT handle
            switch(ret)
            {
            case S_BROM_EFUSE_WRITE_TIMEOUT:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed : Efuse blowing timeout (target no response) [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_REG_NO_MATCH_WITH_TARGET:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed : Argument register is not match with target's Efuse [%s](%d)", StatusToString(ret), ret);
                break;
            case S_FT_PROTOCOL_EXCEPTION:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed : BootROM connect faild [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_DATA_PROCESS_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed : Key data processing error [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_BLOW_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed : Efuse blowing error (write data error) [%s](%d)", StatusToString(ret), ret);
                break;
            default:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Failed :  [%s](%d)", StatusToString(ret), ret);
            }
            goto finish;
        }
        else{
            GC_TRACE("<EfuseWriter> Brom_WriteEfuseSecure() Done");
        }
    }

    if(!str_lock_file.empty()){ // if enter lock file

        /////Lock Efuse register

        ret = Brom_WriteEfuseLock(p_brom_base, ft_handle, &lock_arg, true);
        if(0 != ret){
            //ERROT handle
            switch(ret)
            {
            case S_BROM_EFUSE_WRITE_TIMEOUT:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed : Efuse blowing timeout (target no response) [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_REG_NO_MATCH_WITH_TARGET:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed : Argument register is not match with target's Efuse [%s](%d)", StatusToString(ret), ret);
                break;
            case S_FT_PROTOCOL_EXCEPTION:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed : BootROM connect faild [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_DATA_PROCESS_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed : Key data processing error [%s](%d)", StatusToString(ret), ret);
                break;
            case S_BROM_EFUSE_BLOW_ERROR:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed : Efuse blowing error (write data error) [%s](%d)", StatusToString(ret), ret);
                break;
            default:
                GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Failed :  [%s](%d)", StatusToString(ret), ret);
            }
            goto finish;
        }
        else{
            GC_TRACE("<EfuseWriter> Brom_WriteEfuseLock() Done");
        }
    }

    //
    // read Efuse
    //
    Efuse_Lock_Arg lock_return;          // Arg for Brom_WriteEfuseSecure
    Efuse_Common_Arg common_return;      // Arg for Brom_WriteEfuseCommon
    Efuse_Secure_Arg secure_return;      // Arg for Brom_WriteEfuseSecure
    memset(&common_return, 0, sizeof(common_return));
    memset(&secure_return, 0, sizeof(secure_return));
    memset(&lock_return, 0, sizeof(lock_return));

    // init buffer for using EfuseWriteAll API
    InitReadBackEfuseCommonArg(common_return);
    InitReadBackEfuseSecureArg(secure_return);

    if(!str_readback_file.empty()) // if enter lock file
    {
        ofstream fout(str_readback_file.c_str());
        if(!fout)
        {
            GC_TRACE("<EfuseWriter> Readback file open failed");
        }
        else
        {
            //BROM DLL not yet implement Brom_ReadEfuseAll API
            ret = Brom_ReadEfuseAll(p_brom_base, ft_handle, &common_return, &secure_return, &lock_return, true);
            if(0 != ret){
                switch(ret)
                {
                case S_BROM_EFUSE_WRITE_TIMEOUT:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed : Efuse blowing timeout (target no response) [%s](%d)", StatusToString(ret), ret);
                    break;
                case S_BROM_EFUSE_REG_NO_MATCH_WITH_TARGET:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed : Argument register is not match with target's Efuse [%s](%d)", StatusToString(ret), ret );
                    break;
                case S_FT_PROTOCOL_EXCEPTION:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed : BootROM connect faild [%s](%d)", StatusToString(ret), ret);
                    break;
                case S_BROM_EFUSE_DATA_PROCESS_ERROR:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed : Key data processing error [%s](%d)", StatusToString(ret), ret);
                    break;
                case S_BROM_EFUSE_BLOW_ERROR:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed : Efuse blowing error (write data error) [%s](%d)", StatusToString(ret), ret);
                    break;
                default:
                    GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Failed :  [%s](%d)", StatusToString(ret), ret);
                }

                DeleteReadBackEfuseCommonArg(common_return);
                DeleteReadBackEfuseSecureArg(secure_return);
                fout.close();
                goto finish;
            }
            else{
                GC_TRACE("<EfuseWriter> Brom_ReadEfuseAll() Done");
            }

            // Dump readback result
            fout.fill('0');
            DumpEfuseSecureArgReadback(secure_return, (unsigned int)comPortNum, fout);
            DumpEfuseCommonArgReadback(common_return, (unsigned int)comPortNum, fout);
            DumpEfuseLockArgReadback(lock_return, (unsigned int)comPortNum, fout);
            fout.close();
        }
    }

    DeleteReadBackEfuseCommonArg(common_return);
    DeleteReadBackEfuseSecureArg(secure_return);

finish:

    // Disappear the COM port.
    DEVICE_STATUS device_staus(DISCONNECT_USB_AND_RELEASE_POWERKEY);
    ret = Brom_Rehabilitate(p_brom_base, ft_handle, device_staus);

    //
    // Disconnect with target phone
    //
    int disconn_ret = Brom_DisConnect(&p_brom_base, &ft_handle);
  	Brom_Destroy(&p_brom_base);//Release brom object resource

    if(0 != disconn_ret)
    {
        GC_TRACE("<EfuseWriter> Brom_DisConnect() Failed :  [%s]", StatusToString(ret));
    }
    else
    {
        GC_TRACE("<EfuseWriter> Brom_DisConnect() Done");
    }

    if (0 != ret)
    {
        return ret;
    }
    else
    {
        return disconn_ret;
    }
}

