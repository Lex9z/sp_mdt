//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "GCTrace.h"
#include "formChecksumProgress.h"
#include "PCMemoryChecksumProgress.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"


TformPCMemoryChecksum * formPCMemoryChecksum;
//PCMemoryChecksumProgress *mChecksumProgress;

//---------------------------------------------------------------------------
__fastcall TformPCMemoryChecksum::TformPCMemoryChecksum(TComponent* Owner)
        : TForm(Owner)
{
    //PCMemoryChecksumProgress *checksumProgress = new PCMemoryChecksumProgress(FALSE);
    InitializeCriticalSection(&g_cs);
    mChecksumProgress = NULL;
}

//---------------------------------------------------------------------------
__fastcall TformPCMemoryChecksum::~TformPCMemoryChecksum()
{

    DeleteCriticalSection(&g_cs);
    
}

//---------------------------------------------------------------------------
void __fastcall TformPCMemoryChecksum::FormShow(TObject *Sender)
{

    GC_TRACE("TformPCMemoryChecksum::FormShow() before new PCMemoryChecksumProgress");
    //InitializeCriticalSection(&g_cs);
    EnterCriticalSection(&g_cs);

    mChecksumProgress = new PCMemoryChecksumProgress(FALSE);
    LeaveCriticalSection(&g_cs);
    mChecksumProgress->FreeOnTerminate = true;
}

//---------------------------------------------------------------------------
void __fastcall TformPCMemoryChecksum::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caHide;
    GC_TRACE("TformPCMemoryChecksum::FormClose()");
    EnterCriticalSection(&g_cs);

    try
    {
        if(NULL !=  mChecksumProgress)
        {
             mChecksumProgress->Terminate();
             mChecksumProgress = NULL;
        }

    }
    catch (...)
    {
       GC_TRACE("TformPCMemoryChecksum::FormClose() exception.");
    }


    LeaveCriticalSection(&g_cs);
    //DeleteCriticalSection(&g_cs);
        

}
