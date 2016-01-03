#ifndef PROTECT_REGION_H
#define PROTECT_REGION_H
#include <string>
#include <inifiles.hpp>
#include <vcl.h>
#include <stdio.h>
#include <map>
#include "GCTrace.h"
#include "ProtectRegionInfo.h"
using namespace std;

class ProtectRegion
{
public:
	ProtectRegion();
	~ProtectRegion();
	void ReadProtectRegion(void);
	map<string, ProtectRegionInfo*>* GetProtectRegionInfoMap(void);

private:
    TIniFile* mProtectRegionIni;
    TStringList* mProtectRegionNameList;
    std::map<string, ProtectRegionInfo*> mProtectRegionMap;
};

#endif