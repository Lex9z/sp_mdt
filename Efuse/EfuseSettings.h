#ifndef EFUSE_SETTINGS_H
#define EFUSE_SETTINGS_H
#include<string>
#include "GCTrace.h"

using namespace std;

class EfuseSettings
{
public:
	EfuseSettings();
	~EfuseSettings();

	const int GetEfuseEnable(void) const{ return mEfuseEnable; };
	const int GetEfuseSettingsEnable(void) const{ return mEfuseSettingsEnable; };
	const int GetEfuseLockEnable(void) const{ return mEfuseLockEnable; };
    const int GetEfuseReadBackEnable(void) const{ return mEfuseReadBackEnable; };

	void ReadEfuseSettings(void);

private:
	int mEfuseEnable;
	int mEfuseSettingsEnable;
	int mEfuseLockEnable;
	int mEfuseReadBackEnable;
};

#endif