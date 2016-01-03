//---------------------------------------------------------------------------

#ifndef FORM_CHECKSUM_PROGRESS_H
#define FORM_CHECKSUM_PROGRESS_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CGAUGES.h"


class PCMemoryChecksumProgress;
//---------------------------------------------------------------------------
class TformPCMemoryChecksum : public TForm
{
__published:	// IDE-managed Components
        TLabel *checksumImageNameLabel;
        TCGauge *PCMemoryChecksumProgressBar;
        TLabel *checksumTipLabel;
        TLabel *MD5ChecksumLabel1;

        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        CRITICAL_SECTION g_cs;
public:		// User declarations

        __fastcall TformPCMemoryChecksum(TComponent* Owner);
        __fastcall ~TformPCMemoryChecksum();
        
        PCMemoryChecksumProgress *mChecksumProgress;
        static bool mChecksumFlag;


};

bool TformPCMemoryChecksum::mChecksumFlag = false;
//---------------------------------------------------------------------------
extern PACKAGE TformPCMemoryChecksum *formPCMemoryChecksum;
//---------------------------------------------------------------------------
#endif
