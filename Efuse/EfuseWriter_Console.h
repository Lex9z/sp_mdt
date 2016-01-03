// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <algorithm>

#include "..\Brom\flashtool_api.h"
#include "..\Brom\com_enum.h"
#include "..\Brom\mtk_status.h"
#include "..\Brom\brom.h"

#include "com_util.h"

using namespace std;

extern int  EfuseWriter(string& str_setting_file, string& str_lock_file, string& str_readback_file, unsigned char comPortNum);
extern int  GetEfuseCommonArgFromINI(const string & str_ini_file, Efuse_Common_Arg &common_arg);
extern int  GetEfuseSecureArgFromINI(const string & str_ini_file, Efuse_Secure_Arg & secure_arg);
extern int  GetEfuseLockArgFromINI(const string & str_lock_file, Efuse_Lock_Arg & lock_arg);
extern void DeleteReadBackEfuseSecureArg( Efuse_Secure_Arg &bft);
extern void DeleteReadBackEfuseCommonArg( Efuse_Common_Arg &bft);
extern void InitReadBackEfuseCommonArg( Efuse_Common_Arg &bft);
extern void InitReadBackEfuseSecureArg( Efuse_Secure_Arg &bft);

extern void DumpEfuseCommonArg(Efuse_Common_Arg bft, unsigned int comportNum, bool isReadBack = false);
extern void DumpEfuseSecureArg(Efuse_Secure_Arg bft, unsigned int comportNum, bool isReadBack = false);
extern void DumpEfuseSecureArgReadback(Efuse_Secure_Arg bft, unsigned int comportNum,ofstream &fout);
extern void DumpEfuseCommonArgReadback(Efuse_Common_Arg bft, unsigned int comportNum,ofstream &fout);
extern void DumpEfuseLockArgReadback(Efuse_Lock_Arg bft, unsigned int comportNum,ofstream &fout);
extern void DumpEfuseLockArg(Efuse_Lock_Arg bft, unsigned int comportNum);
extern void DumpSTBArg(Efuse_STB_Key_Arg bft, unsigned int comportNum, bool bIsWrite = true);
extern string Binary2Str(const _BUFFER &ref);









// TODO: reference additional headers your program requires here
