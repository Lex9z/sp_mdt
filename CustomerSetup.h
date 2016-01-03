#ifndef CUSTOMER_SETUP_H
#define CUSTOMER_SETUP_H
#include<string>
#include <initguid.h>
#include "GCTrace.h"


using namespace std;

typedef enum {
	CLASS_GUID = 0,
	INTERFACE_GUID
}GUIDTYPE;

// for android usb port find
//
DEFINE_GUID(Interface_Guid_ANDROID_USB2Ser,0xA5DCBF10L,0x6530,0x11D2,0x90,0x1F,0x00,0xC0,0x4F,
                            0xB9,0x51,0xED);

DEFINE_GUID(Class_Guid_ANDROID_USB2Ser, 0x4D36E978L,0xE325,0x11CE,0xBF,0xC1,0x08,0x00,0x2B,
			0xE1,0x03,0x18);

class CustomerSetup
{
public:
	CustomerSetup();
	~CustomerSetup();

	const string& GetPreloaderPid(void) const;
	const string& GetDAHighSpeedPid(void) const;
    const string& GetBromPid(void) const;
	const string& GetAndroidVid(void) const;

	const GUID& GetGUID(void) const;

	void ReadCustomerSettings(void);

private:
	string mPreloaderPid;
	string mDAHighSpeedPid;
	string mBromPid;
	string mAndroidVid;

	int mGUIDFlag;

};

#endif