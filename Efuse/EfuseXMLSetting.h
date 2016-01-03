#ifndef _H_EFUSESETTING_H
#define _H_EFUSESETTING_H
#include <string>
#include "..\brom\brom.h"
#include "..\TinyXml\header\tinyxml.h"
#include "..\TinyXml\header\tinystr.h"
using namespace std;

typedef enum {
	S_EFUSE_SUCCESS = 0,
	S_GET_XML_FILE_LOAD_FAIL,
	S_GET_XML_CHILD_FAIL,
	S_GET_XML_COMMON_CTRL_FAIL,
	S_GET_XML_SECURE_CTRL_FAIL,
	S_GET_XML_COMMON_LOCK_FAIL,
	S_GET_XML_SECURE_LOCK_FAIL,
	S_GET_XML_SBC_KEY_FAIL
	
}S_EFUSE_SETTING_STATUS;

#define EFUSE_STK_KEY_NUMBER        16

class CEfuseXMLSetting
{
public:
	   CEfuseXMLSetting();
	   CEfuseXMLSetting(string filepath);
	   ~CEfuseXMLSetting();
	  
	   int ParseXML();
	   void SetLoadFilePath(string filepath);
	   bool LoadFile(string filepath);
	   int  ParseCommonCtrl();
	   int  ParseSecureCtrl();
	   int  ParseSbcKey();
	   int  ParseCommonLock();
	   int  ParseSecureLock();
	   int  ParseUsbID();
	   int  ParseAcKey();
	   int  ParseSpare();

       int  ParseSTBLock();
       int  ParseSTBID();
       int  ParseSTBKey();
	   
	   bool  IsXmlParseReady();

	   string GetXmlFilePath()
	   {
	      return m_filepath;
	   }

	   void ReleaseArgs();
private:
       string m_filepath;
	   TiXmlDocument *m_pXmlDoc;
	   bool m_isLoadFile;
	   bool m_isParseXmlReady;
public:
	   Efuse_Common_Arg common_arg;
	   Efuse_Secure_Arg secure_arg;
	   Efuse_Lock_Arg lock_arg;
	   Efuse_STB_Key_Arg   stb_key_arg_;
	   //Efuse_Extra_Arg     extra_arg_;
	   
};

#endif