#include "NewIniHandle.h"

#ifndef  _FILE_UTILS_H_
#include "file_utils.h"
#endif

NewIniHandle::NewIniHandle()
{

}

NewIniHandle::~NewIniHandle()
{

}

AnsiString NewIniHandle::ReadStringFromIni(AnsiString iniFileName,
										   AnsiString applicationName,
										   int idx,
										   AnsiString asSecName,
										   AnsiString asFieldName,
										   AnsiString asDefStr)
{
	AnsiString as_KEY_NAME = asFieldName + IntToStr(idx);
	AnsiString strValue = ReadField(iniFileName, applicationName, asSecName, as_KEY_NAME, asDefStr);
    return  strValue;
}

void NewIniHandle::WriteStringToIni(AnsiString iniFileName, 
								    AnsiString applicationName,
									int idx, 
									AnsiString asSecName,
									AnsiString asFieldName,
									AnsiString asValue)
{
	AnsiString as_KEY_NAME = asFieldName + IntToStr(idx);
    WriteField(iniFileName, applicationName, asSecName, as_KEY_NAME, asValue);

}

_BOOL NewIniHandle::ReadBoolFromIni(AnsiString iniFileName,
								   AnsiString applicationName,
								   int idx,
								   AnsiString asSecName,
								   AnsiString asFieldName,// key
								   AnsiString asDefaultValue)
{
	_BOOL  ret;
	AnsiString as_KEY_NAME = asFieldName + IntToStr(idx) + " checked";
    AnsiString asChecked = ReadField(iniFileName, applicationName, asSecName, as_KEY_NAME, asDefaultValue);

    if(asChecked == "yes")
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }

    return  ret;

}

void NewIniHandle::WriteBoolToIni(AnsiString iniFileName, 
								  AnsiString applicationName,
							      int idx, 
								  AnsiString asSecName,
								  AnsiString asFieldName,
								  _BOOL bValue)
{
	AnsiString  asValue;

    if(bValue)
    {
        asValue = "yes";
    }
    else
    {
        asValue = "no";
    }

    AnsiString as_KEY_NAME = asFieldName + IntToStr(idx) + " checked";
    WriteField(iniFileName, applicationName, asSecName, as_KEY_NAME, asValue);


}

AnsiString NewIniHandle::ReadField(AnsiString iniFileName,
								   AnsiString applicationName,
								   AnsiString asSecName,
								   AnsiString asFieldName,
								   AnsiString asDefaultValue)
{

	AnsiString asPath;
    AnsiString as_str;
    if ( getPathFromStr(applicationName, asPath) && withPath( asPath) && !withPath(iniFileName) )
    {
       iniFileName = asPath + iniFileName;
    }

    TIniFile *ini;

    try
    {
       ini = new TIniFile(iniFileName);
       if(ini != NULL)
       {
           as_str  = ini->ReadString(asSecName, asFieldName, asDefaultValue);
           delete ini;
       }
    }
    catch (...)
    {
        ShowMessage( " read setup file " + iniFileName + " error: " + asFieldName);
    }
    return as_str;
}

void NewIniHandle::WriteField(AnsiString iniFileName, 
							  AnsiString applicationName, 
							  AnsiString asSecName, 
							  AnsiString asFieldName, 
							  AnsiString asDefaultValue)
{
	AnsiString asPath;
    if ( getPathFromStr(applicationName, asPath) &&
         withPath( asPath) &&
         !withPath( iniFileName)
       )
    {
       iniFileName = asPath + iniFileName;
    }

    TIniFile *ini;

    try
    {
        ini = new TIniFile( iniFileName );

        if(ini != NULL)
        {
            ini->WriteString(asSecName, asFieldName, asDefaultValue );
            delete ini;
        }

    }
    catch (...)
    {
        ShowMessage( " write setup file : " + iniFileName + " error" + asDefaultValue);
    }
}