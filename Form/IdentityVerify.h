//---------------------------------------------------------------------------

#ifndef IdentityVerifyH
#define IdentityVerifyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "multiport_common.h"

//---------------------------------------------------------------------------
class TIdentity : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
		TLabel *Label2;
        TEdit *edtPassword;
        TButton *m_btnEnter;
        TButton *m_btnCancel;
        void __fastcall m_btnEnterClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall m_btnCancelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall edtPasswordKeyPress(TObject *Sender, char &Key);
private:	// User declarations
        AnsiString asSetupPath;
        AnsiString asPasswordName;
public:		// User declarations
        __fastcall TIdentity(TComponent* Owner);
		bool __fastcall VerifyPassWord(AnsiString ansiPassword);
		int __fastcall SetLoginMode(Login_Mode loginMode);
		Login_Mode __fastcall GetLoginMode();
};
//---------------------------------------------------------------------------
extern PACKAGE TIdentity *Identity;
//---------------------------------------------------------------------------
#endif
