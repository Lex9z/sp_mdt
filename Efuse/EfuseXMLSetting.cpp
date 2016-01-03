#include"EfuseXMLSetting.h"

#define BoolToEfuseOpt(txt) (txt ? EFUSE_ENABLE : EFUSE_DISABLE)
#define _sbc_key_E  \
    sbc_pub_key_u.w_sbc_pub_key.key_e

#define _sbc_key_N  \
    sbc_pub_key_u.w_sbc_pub_key.key_n


static bool Str2Buf(_BUFFER *buf, const string &str)
{
    const U32 length = str.length();

    if (length > 0 && length%2 == 0)
    {
        buf->buf_len = length/2;
        buf->buf = new char[length+1];
		memset(buf->buf, 0, length+1);
        strcpy(buf->buf, str.c_str());
        return true;
    }
    return false;
}

CEfuseXMLSetting::CEfuseXMLSetting()
{
    m_pXmlDoc = NULL;
    m_isParseXmlReady = false;
    memset(&common_arg, 0, sizeof(Efuse_Common_Arg));
    memset(&secure_arg, 0, sizeof (Efuse_Secure_Arg));
    memset(&lock_arg, 0, sizeof(Efuse_Lock_Arg));
    memset(&stb_key_arg_, 0, sizeof(Efuse_Lock_Arg));
    //memset(&extra_arg_, 0, sizeof(extra_arg_));
}


CEfuseXMLSetting::CEfuseXMLSetting(string filepath):m_filepath (filepath)
{
     m_isParseXmlReady = false;
     m_pXmlDoc = NULL;
}

CEfuseXMLSetting::~CEfuseXMLSetting()
{
    ReleaseArgs();
}

bool CEfuseXMLSetting::LoadFile(string filepath)
{
    m_pXmlDoc = new TiXmlDocument(m_filepath.c_str());
    m_isLoadFile= m_pXmlDoc->LoadFile();
    //if loadfile success, it will return true
    return m_isLoadFile;
}

//Change the filepath for another xml file
void CEfuseXMLSetting::SetLoadFilePath(string filepath)
{
	m_filepath = filepath;
	m_isParseXmlReady = false;
}

int CEfuseXMLSetting::ParseXML()
{
   
    if(m_isParseXmlReady ==  true)
    {
	    ReleaseArgs();
	}

	m_isParseXmlReady = false;
	memset(&common_arg, 0, sizeof(Efuse_Common_Arg));
	memset(&secure_arg, 0, sizeof (Efuse_Secure_Arg));
	memset(&lock_arg, 0, sizeof(Efuse_Lock_Arg));
	memset(&stb_key_arg_, 0, sizeof(Efuse_Lock_Arg));
        //memset(&extra_arg_, 0, sizeof(extra_arg_));

    LoadFile(m_filepath);

	if(m_isLoadFile == false){
		return S_GET_XML_FILE_LOAD_FAIL;
	}
    
    ParseCommonCtrl();
	ParseSecureCtrl();
	ParseSbcKey();
	ParseCommonLock();
	ParseSecureLock();
	ParseUsbID();
	ParseAcKey();
    ParseSpare();

	//STB
	ParseSTBLock();
	ParseSTBID();
	ParseSTBKey();
	
	m_isParseXmlReady = true;
	return S_EFUSE_SUCCESS;
}

int CEfuseXMLSetting::ParseCommonCtrl()
{
    TiXmlHandle  docHandle( m_pXmlDoc);
    bool pBool;
	int iTemp;
	
	//1.Parse common-ctrl
	TiXmlElement* childCommonCtrl = docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("common-ctrl").ToElement();
	if(childCommonCtrl == NULL){
		return S_GET_XML_CHILD_FAIL;
	}
	//1.1Parse common-ctrl -- Disable_EMMC_boot
	if(childCommonCtrl->QueryBoolAttribute("Disable_EMMC_boot", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_COMMON_CTRL_FAIL;
	}
	common_arg.emmc_boot_dis = BoolToEfuseOpt(pBool);
	
	//1.2Parse common-ctrl -- Disable_EMMC_boot
	if(childCommonCtrl->QueryBoolAttribute("Disable_NAND_boot", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_COMMON_CTRL_FAIL;
	}
    common_arg.nand_boot_dis = BoolToEfuseOpt(pBool);
	
	//1.3Parse common-ctrl -- Disable_EMMC_boot
	if(childCommonCtrl->QueryBoolAttribute("Disable_NAND_boot_speedup", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_COMMON_CTRL_FAIL;
	}
	common_arg.nand_boot_speedup_dis = BoolToEfuseOpt(pBool);;

    //1.4Parse common-ctrl -- USB_download_type
	if(childCommonCtrl->Attribute("USB_download_type",&iTemp) == NULL){
		return S_GET_XML_COMMON_CTRL_FAIL;
	}
	common_arg.usbdl_type = (unsigned int)iTemp;
	if (common_arg.usbdl_type != 0)
    {
        common_arg.usbdl_type_blow = EFUSE_ENABLE;
    }
    else
    {
        common_arg.usbdl_type_blow = EFUSE_DISABLE;
    }
	
	return S_EFUSE_SUCCESS;
}


int CEfuseXMLSetting::ParseSecureCtrl()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    bool pBool;

	//1.Parse secure-ctrl
	TiXmlElement* childSecureCtrl= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("secure-ctrl").ToElement();
    if(childSecureCtrl == NULL){
        return S_GET_XML_CHILD_FAIL;
    }

	//1.1Parse secure-ctrl -- Enable_ACC
	if(childSecureCtrl->QueryBoolAttribute("Enable_ACC", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.acc_en = BoolToEfuseOpt(pBool);

    //1.2Parse secure-ctrl -- Enable_ACK
	if(childSecureCtrl->QueryBoolAttribute("Enable_ACK", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
    secure_arg.ack_en= BoolToEfuseOpt(pBool);

	//1.3 Parse secure-ctrl -- Enable_SBC
	if(childSecureCtrl->QueryBoolAttribute("Enable_SBC", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.sbc_en= BoolToEfuseOpt(pBool);

	//1.4 Parse secure-ctrl -- Enable_SLA
	if(childSecureCtrl->QueryBoolAttribute("Enable_SLA", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
    secure_arg.sla_en = BoolToEfuseOpt(pBool);

	//1.5 Parse secure-ctrl -- Enable_DAA
	if(childSecureCtrl->QueryBoolAttribute("Enable_DAA", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.daa_en= BoolToEfuseOpt(pBool);
	
	//1.6 Parse secure-ctrl -- Enable_SW_JTAG_CON
	if(childSecureCtrl->QueryBoolAttribute("Enable_SW_JTAG_CON", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.jtag_sw_con = BoolToEfuseOpt(pBool);

    //1.8 Parse secure-ctrl -- Enable_Root_Cert
	if(childSecureCtrl->QueryBoolAttribute("Enable_Root_Cert", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.root_cert_en= BoolToEfuseOpt(pBool);
	
	//1.9 Parse secure-ctrl -- Disable_JTAG
	if(childSecureCtrl->QueryBoolAttribute("Disable_JTAG", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.jtag_dis= BoolToEfuseOpt(pBool);

        //2.0 Parse -disable dbgport lock -- Disable_DBGPORT_LOCK
	if(childSecureCtrl->QueryBoolAttribute("Disable_DBGPORT_LOCK", &pBool) != TIXML_SUCCESS){
		return S_GET_XML_SECURE_CTRL_FAIL;
	}
	secure_arg.dbgport_lock_dis= BoolToEfuseOpt(pBool);

	return S_EFUSE_SUCCESS;

}

int CEfuseXMLSetting::ParseSbcKey()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    string strKeyE;
	string strKeyN;
	//1.Parse sbc-pub-key
    TiXmlElement* childSbcKey= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("sbc-pub-key").ToElement();
	if(childSbcKey){
	    TiXmlElement  *childSbcKeyE = childSbcKey->FirstChildElement("pub-key-e");
		strKeyE = childSbcKeyE->GetText();
		TiXmlElement  *childSbcKeyN = childSbcKey->FirstChildElement("pub-key-n");
		strKeyN = childSbcKeyN->GetText();
	    if(childSbcKeyE && childSbcKeyN){
		    if (Str2Buf(&secure_arg._sbc_key_E, strKeyE) &&
		        Str2Buf(&secure_arg._sbc_key_N, strKeyN)){
                secure_arg.sbc_pubk_blow = EFUSE_ENABLE;
		    }
		}else{
		    return S_GET_XML_SBC_KEY_FAIL;
		}
	}else{
	    return S_GET_XML_SBC_KEY_FAIL;
	}
	
	return S_EFUSE_SUCCESS;

}

int CEfuseXMLSetting::ParseCommonLock()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    bool pBool;

	//1.Parse common-lock
    TiXmlElement* childCommonLock= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("common-lock").ToElement();
    if(childCommonLock == NULL){
        return S_GET_XML_CHILD_FAIL;
    }
	
	//1.1 Parse common-lock -- com_ctrl_lock
	if(childCommonLock->QueryBoolAttribute("com_ctrl_lock", &pBool) != TIXML_SUCCESS){
			return S_GET_XML_COMMON_LOCK_FAIL;
	}
	lock_arg.common_ctrl_lock = BoolToEfuseOpt(pBool);

    //1.2 Parse common-lock -- com_ctrl_lock
	if(childCommonLock->QueryBoolAttribute("usb_id_lock", &pBool) != TIXML_SUCCESS){
	    return S_GET_XML_COMMON_LOCK_FAIL;
	}
	lock_arg.usb_id_lock = BoolToEfuseOpt(pBool);

	//1.3 Parse common-lock -- com_ctrl_lock
	if(childCommonLock->QueryBoolAttribute("spare_lock", &pBool) != TIXML_SUCCESS){
	    return S_GET_XML_COMMON_LOCK_FAIL;
	}
	lock_arg.spare_lock= BoolToEfuseOpt(pBool);

	return S_EFUSE_SUCCESS;

}

int CEfuseXMLSetting::ParseSecureLock()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    bool pBool;

	//1.Parse secure-lock
    TiXmlElement* childSecureLock= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("secure-lock").ToElement();
    if(childSecureLock == NULL){
        return S_GET_XML_CHILD_FAIL;
    }
	
	//1.1 Parse secure-lock -- sec_attr_lock
	if(childSecureLock->QueryBoolAttribute("sec_attr_lock", &pBool) != TIXML_SUCCESS){
			return S_GET_XML_SECURE_LOCK_FAIL;
	}
	lock_arg.sec_ctrl_lock= BoolToEfuseOpt(pBool);

    //1.2 Parse secure-lock -- ackey_lock
	if(childSecureLock->QueryBoolAttribute("ackey_lock", &pBool) != TIXML_SUCCESS){
	    return S_GET_XML_SECURE_LOCK_FAIL;
	}
	lock_arg.ackey_lock = BoolToEfuseOpt(pBool);

	//1.3 Parse secure-lock -- sbc_pubk_hash_lock
	if(childSecureLock->QueryBoolAttribute("sbc_pubk_hash_lock", &pBool) != TIXML_SUCCESS){
	    return S_GET_XML_SECURE_LOCK_FAIL;
	}
	lock_arg.sbc_pubk_hash_lock= BoolToEfuseOpt(pBool);

	return S_EFUSE_SUCCESS;

}

int CEfuseXMLSetting::ParseUsbID()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    int iTmp = 0;
	//1.Parse usb-id
    TiXmlElement* childUsbID= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("usb-id").ToElement();
    if(childUsbID == NULL){
        return S_GET_XML_CHILD_FAIL;
    }
	
	//1.1 Parse usb-id -- vid
	if(childUsbID->Attribute("vid", &iTmp ) != NULL ){
		common_arg.usb_vid= (unsigned short)iTmp;
	}

	//1.2 Parse usb-id -- pid
	if(childUsbID->Attribute("pid", &iTmp) != NULL ){
		common_arg.usb_pid = (unsigned short)iTmp;
	}

	if (common_arg.usb_vid != 0 ||
		common_arg.usb_pid != 0 )
	{
		common_arg.usb_id_blow = EFUSE_ENABLE;
	}

	return S_EFUSE_SUCCESS;
}

int CEfuseXMLSetting::ParseAcKey()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
	string text;
	//1.Parse ac-key
    TiXmlElement* childAcKey= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("ac-key").ToElement();
    if(childAcKey == NULL){
        return S_GET_XML_CHILD_FAIL;
    }

	text = childAcKey->GetText();
	if (Str2Buf(&secure_arg.ac_key, text))
    {
        secure_arg.ac_key_blow = EFUSE_ENABLE;
    }
	
	return S_EFUSE_SUCCESS;
}

int CEfuseXMLSetting::ParseSpare()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
	string text;
	//1.Parse spare
    TiXmlElement* childSpare = docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("spare").ToElement();
    if(childSpare == NULL){
        return S_GET_XML_CHILD_FAIL;
    }

	text = childSpare->GetText();
	if (Str2Buf(&common_arg.spare, text))
    {
        common_arg.spare_blow= EFUSE_ENABLE;
    }
	
	return S_EFUSE_SUCCESS;
}

int CEfuseXMLSetting::ParseSTBLock()
{
    /*<stb-lock
              stb_key_g7_lock="false" 
 		stb_key_g6_lock="false"
		stb_key_g5_lock="false"
		stb_key_g4_lock="false"
		stb_key_g3_lock="false"
		stb_key_g2_lock="false"
		stb_key_g1_lock="false"
		stb_key_g0_lock="false"
		stb_key_operatorid_lock="false"
		stb_key_chipid_lock="false"
		stb_key_sn_lock="false"/>
	*/

    TiXmlHandle  docHandle(m_pXmlDoc);
    bool pBool;
	
	//1.Parse common-lock
    TiXmlElement* childStdLock= docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("stb-lock").ToElement();
    if(childStdLock == NULL){
        return S_GET_XML_CHILD_FAIL;
    }
	
	//1.1 Parse stb_key_g7_lock-- stb_key_g7_lock
    if(childStdLock->QueryBoolAttribute("stb_key_g7_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g7_lock = BoolToEfuseOpt(pBool);
    }
	

    if(childStdLock->QueryBoolAttribute("stb_key_g6_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g6_lock = BoolToEfuseOpt(pBool);
    }
	

    if(childStdLock->QueryBoolAttribute("stb_key_g5_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g5_lock = BoolToEfuseOpt(pBool);
    }
	

    if(childStdLock->QueryBoolAttribute("stb_key_g4_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g4_lock = BoolToEfuseOpt(pBool);
    }
	

	
    if(childStdLock->QueryBoolAttribute("stb_key_g3_lock", &pBool) == TIXML_SUCCESS){
		stb_key_arg_.stb_lock.stb_key_g3_lock = BoolToEfuseOpt(pBool);
    }
	

    if(childStdLock->QueryBoolAttribute("stb_key_g2_lock", &pBool) == TIXML_SUCCESS){
		stb_key_arg_.stb_lock.stb_key_g2_lock = BoolToEfuseOpt(pBool);
	}
	

    if(childStdLock->QueryBoolAttribute("stb_key_g1_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g1_lock = BoolToEfuseOpt(pBool);
    }
	

    if(childStdLock->QueryBoolAttribute("stb_key_g0_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_g0_lock = BoolToEfuseOpt(pBool);
    }
    

    if(childStdLock->QueryBoolAttribute("stb_key_operatorid_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_operatorid_lock= BoolToEfuseOpt(pBool);
    }
	
    if(childStdLock->QueryBoolAttribute("stb_key_chipid_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_chipid_lock = BoolToEfuseOpt(pBool);
    }
	
    if(childStdLock->QueryBoolAttribute("stb_key_sn_lock", &pBool) == TIXML_SUCCESS){
        stb_key_arg_.stb_lock.stb_key_sn_lock= BoolToEfuseOpt(pBool);
    }
	
    return S_EFUSE_SUCCESS;
}


int CEfuseXMLSetting::ParseSTBID()
{
	TiXmlHandle  docHandle(m_pXmlDoc);
	int iTmp = 0;
	//1.Parse STBID
    TiXmlElement* childStbId = docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("stb-id").ToElement();
    if(childStbId == NULL){
        return S_GET_XML_CHILD_FAIL;
    }
	
	//<stb-id stb_key_chipid="0000" stb_key_operatorid="0000"/> 
	
    if(childStbId->Attribute("stb_key_chipid", &iTmp ) != NULL ){
        stb_key_arg_.stb_key_chipID = (unsigned int)iTmp;
    }
	
	if(childStbId->Attribute("stb_key_operatorid", &iTmp) != NULL ){
        stb_key_arg_.stb_key_operatorid = (unsigned short)iTmp;
	}


    if (stb_key_arg_.stb_key_chipID  != 0)
    {
        stb_key_arg_.stb_key_chipid_blow = EFUSE_ENABLE;
    }

	
    if(stb_key_arg_.stb_key_operatorid != 0)
    {
        stb_key_arg_.stb_key_operatorid_blow = EFUSE_ENABLE;
    }

    return S_EFUSE_SUCCESS;

}

int CEfuseXMLSetting::ParseSTBKey()
{
    TiXmlHandle  docHandle(m_pXmlDoc);
    STB_KEY_PARAM *param;
    string node_tag;
    string key;
	int i = 0;

    TiXmlElement* childstbkey =  docHandle.FirstChild("flashtool-config").FirstChild("commands").FirstChild("efuse").FirstChild("stb-key-group").FirstChild().ToElement();

    i = 0;
    while(childstbkey != NULL && i < EFUSE_STK_KEY_NUMBER)
    {
        param = &stb_key_arg_.stb_blow_keys[i];

        node_tag = childstbkey->Value();  
        const U32 length = node_tag.length();

        if (length > 0 )
        {
            param->key_name = new char[length+1];
            strcpy(param->key_name, node_tag.c_str());
        }

        key = childstbkey->GetText();
        if(Str2Buf(&param->stb_key, key))
            param->key_blow = EFUSE_ENABLE;

        childstbkey = childstbkey->NextSiblingElement();
        i++;
    }
	
    return S_EFUSE_SUCCESS;
}


/*
void CEfuseXMLSetting::LoadUnitValue(EFUSE_KEY key,
                                 const XML::Node &node)
{
    std::string text;

    text = node.GetText();

    extra_arg_.items[key].key = key;
    extra_arg_.items[key].type = T_INT;
    extra_arg_.items[key].data.iPair.value = strtoul(text.c_str(), NULL, 16);

    if(extra_arg_.items[key].data.iPair.value != 0)
        extra_arg_.items[key].data.iPair.blow = EFUSE_ENABLE;
}

void CEfuseXMLSetting::LoadKey(EFUSE_KEY key,
                           const XML::Node &node)
{
    std::string text;

    text = node.GetText();

    extra_arg_.items[key].key = key;
    extra_arg_.items[key].type = T_BUF;

    if(Str2Buf(&extra_arg_.items[key].data.key_pair.key, text))
    {
        extra_arg_.items[key].data.key_pair.key_blow = EFUSE_ENABLE;
    }
}*/


void CEfuseXMLSetting::ReleaseArgs()
{
    if(m_pXmlDoc != NULL)
    {
        delete m_pXmlDoc;
        m_pXmlDoc = NULL;
    }

   
    if (common_arg.spare.buf != NULL)
    {
        delete common_arg.spare.buf;
        common_arg.spare.buf = NULL;
    }

    if (secure_arg.ac_key.buf != NULL)
    {
        delete secure_arg.ac_key.buf;
		secure_arg.ac_key.buf = NULL;
   	}
	
   	if (secure_arg._sbc_key_E.buf != NULL)
   	{
        delete secure_arg._sbc_key_E.buf;
		secure_arg._sbc_key_E.buf = NULL;
   	}

	if (secure_arg._sbc_key_N.buf != NULL)
	{
        delete secure_arg._sbc_key_N.buf; 
		secure_arg._sbc_key_N.buf = NULL;
	}

	for(int i = 0; i < EFUSE_STK_KEY_NUMBER; i++)
    {
        if(stb_key_arg_.stb_blow_keys[i].key_name != NULL)
        {
            delete stb_key_arg_.stb_blow_keys[i].key_name;
			stb_key_arg_.stb_blow_keys[i].key_name = NULL;
        }

        if(stb_key_arg_.stb_blow_keys[i].stb_key.buf != NULL)
        {
            delete stb_key_arg_.stb_blow_keys[i].stb_key.buf;
			stb_key_arg_.stb_blow_keys[i].stb_key.buf = NULL;
        }
    }

       /*	for(int i = 0; i < extra_arg_.item_count; i++)
    {
        if(extra_arg_.items[i].type == T_BUF && extra_arg_.items[i].data.key_pair.key.buf != NULL)
        {
            delete extra_arg_.items[i].data.key_pair.key.buf;
			extra_arg_.items[i].data.key_pair.key.buf = NULL;
        }
    }   */
	
}

bool CEfuseXMLSetting::IsXmlParseReady()
{
   return m_isParseXmlReady;
}





