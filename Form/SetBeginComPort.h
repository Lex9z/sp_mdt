//---------------------------------------------------------------------------

#ifndef SetBeginComPortH
#define SetBeginComPortH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TdlgSetBeginComPort : public TForm
{
__published:	// IDE-managed Components
        TEdit *ed_begin_com_port;
        TLabel *laSetBeginComPort;
        TButton *cbBCPApply;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cbBCPApplyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TdlgSetBeginComPort(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdlgSetBeginComPort *dlgSetBeginComPort;
//---------------------------------------------------------------------------
#endif
