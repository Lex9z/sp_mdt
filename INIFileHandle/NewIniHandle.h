#ifndef NEW_INI_HANDLE_H
#define NEW_INI_HANDLE_H

#include<string>
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#include "flashtool_handle.h"

using namespace std;

class NewIniHandle
{
public:
	NewIniHandle();
	~NewIniHandle();

	static AnsiString ReadStringFromIni(AnsiString iniFileName, 
										AnsiString applicationName, 
										int idx, 
									    AnsiString asSecName,
										AnsiString asFieldName,
										AnsiString asDefaultValue);

	static void WriteStringToIni(AnsiString iniFileName, 
								 AnsiString applicationName, 
								 int idx, 
								 AnsiString asSecName,
							     AnsiString asFieldName,
							     AnsiString asValue);

	static _BOOL ReadBoolFromIni(AnsiString iniFileName, 
								AnsiString applicationName, 
								int idx, 
								AnsiString asSecName,
								AnsiString asFieldName,
								AnsiString asDefaultValue);

	static void WriteBoolToIni(AnsiString iniFileName, 
							   AnsiString applicationName, 
							   int idx, 
							   AnsiString asSecName,
							   AnsiString asFieldName,
							   _BOOL bValue);

	static AnsiString ReadField(AnsiString iniFileName,
								AnsiString applicationName, 
								AnsiString asSecName,
								AnsiString asFieldName,
								AnsiString asDefaultValue);

	static void WriteField(AnsiString iniFileName, 
						   AnsiString applicationName, 
						   AnsiString asSecName,
						   AnsiString asFieldName,
						   AnsiString asDefaultValue);

	
};

#endif