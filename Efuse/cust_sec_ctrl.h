#ifndef CUST_SEC_CTRL_H
#define CUST_SEC_CTRL_H

//------------------------------------------------------------------------------
// INI AES KEY
//------------------------------------------------------------------------------
// Dear Customer. Please configure this AES key which is used to decrypt EFUSE INI file
#define INI_AES_KEY                 "B145620DDEE22541265DD89713235BDC"
#define AES_KEY_LEN                 (32)

//------------------------------------------------------------------------------
// INI SECURE CONTROL
//------------------------------------------------------------------------------
// If INI_MUST_BE_ENC is configured as TRUE, only encrypted INI can be loaded
#define INI_MUST_BE_ENC             (FALSE)

/* Warning, Please carefully enable this feature */
#define DIS_JTAG_OPTION             (FALSE)

//------------------------------------------------------------------------------
// INI AES KEY
//------------------------------------------------------------------------------
#define INI_FILE_MAX_SIZE           (16384)

#endif

