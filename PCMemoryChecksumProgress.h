#ifndef  PC_MEMORY_CHECKSUM_PROGRESS_H
#define  PC_MEMORY_CHECKSUM_PROGRESS_H

#include "Brom\flashtool_handle.h"
#include "Form\formChecksumProgress.h"
#include <vcl.h>
#include <Classes.hpp>


class PCMemoryChecksumProgress : public TThread
{
private:
        

protected:
        void __fastcall Execute();

public:
        PCMemoryChecksumProgress(bool CreateSuspended);

        
        void InitChecksumArgs(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult);
        int VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult);
        bool VerifyHostBufferBeforeStart();

        void _fastcall Start();
        void _fastcall SyncStart();
        void _fastcall End();
        void _fastcall SyncEnd();


        void _fastcall InitProgress();
        void _fastcall SyncInitProgress();
        void _fastcall UpdateProgressColor();
        void _fastcall SyncUpdateProgressColor(TColor bk, TColor ft);
        void _fastcall UpdateProgressPercentage();
        void _fastcall SyncUpdateProgressPercentage(int precentage);




        AnsiString mPrefix;
		AnsiString mHint;
        int m_ucProgressPercentage;

        TColor _bk_col;
        TColor _ft_col;

};
#endif
