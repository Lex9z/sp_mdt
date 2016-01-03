#ifndef __COM_PORT_UTILS_H
#define __COM_PORT_UTILS_H

//#include <string>
#include <set>
#include <vector>
#include <string>
#include <initguid.h>
#include "CustomerSetup.h"

using std::string;
using std::set;
using std::vector;

//typedef pair<string, string> NamePair;
typedef set<string> MySet;
typedef set<string>::iterator MyIter;



typedef enum
{
    BOOTROM_COMPORT = 0,
    PRELOADER_COMPORT = 1

}COMPORT_TYPE;

class COMPortUtils
{
public:
    COMPortUtils();
    ~COMPortUtils();

    bool GetNewPortSymbolicName(string & strNewPort, int comportType);
    void ScanSymbolicNameByDDK(bool isFirstScan, int comportType);
    bool GetNewPortFriendlyName(string & strNewPort, const int comportType);
    void ScanFriendlyNameByDDK(bool isFirstScan, int comportType);
    bool ExtractCOMLabel(string & friendly, string & label);
    bool ExtractCOMNumber(string & friendly, int & num);

    void COMPortSetDiff(MyIter curBegin, MyIter curEnd, MyIter oriBegin, MyIter oriEnd, MySet & diff);
    void COMPortSetCopy(MyIter srcBegin, MyIter srcEnd, MySet & dest);
    string GetAndroidVid(void);
    string GetPreloaderPid(void);

private:
    MySet  mSetOriPortSym;
    MySet  mSetCurPortSym;
    MySet  mSetOriPortFri;
    MySet  mSetCurPortFri;

	CustomerSetup mCustomerSetup;
	string mPreloaderPid;
	string mBromPid;
	string mAndroidVid;
	string mDAHighSpeedPid;

	GUID mGuid;

};

#endif //__COM_PORT_UTILS_H


