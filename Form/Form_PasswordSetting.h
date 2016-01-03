//---------------------------------------------------------------------------

#ifndef Form_PasswordSettingH
#define Form_PasswordSettingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#define LOGIN_PASSWORD_MAX 64

//---------------------------------------------------------------------------
class TPasswordSetting : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edtOldPassword;
        TEdit *edtNewPassword;
        TEdit *edtConfirmPassword;
        TButton *btnSetPassword;
        TButton *BtnCancel;
        TLabel *Label4;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnCancelClick(TObject *Sender);
        void __fastcall btnSetPasswordClick(TObject *Sender);
        void __fastcall edtOldPasswordKeyPress(TObject *Sender, char &Key);
        void __fastcall edtNewPasswordKeyPress(TObject *Sender, char &Key);
        void __fastcall edtConfirmPasswordKeyPress(TObject *Sender,
          char &Key);
private:	// User declarations
        AnsiString asSetupPath;
        AnsiString asPasswordName;
public:		// User declarations
        __fastcall TPasswordSetting(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPasswordSetting *PasswordSetting;
//---------------------------------------------------------------------------
#endif
