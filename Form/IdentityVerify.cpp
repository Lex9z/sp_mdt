//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IdentityVerify.h"
#include "des.h"
#include "file_utils.h"
#include "multiport_common.h"
#include "Form_PasswordSetting.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIdentity *Identity;

//---------------------------------------------------------------------------
__fastcall TIdentity::TIdentity(TComponent* Owner)
        : TForm(Owner)
{
    asPasswordName = "MDTDATA";
    edtPassword->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TIdentity::m_btnEnterClick(TObject *Sender)
{
    if( VerifyPassWord(edtPassword->Text.c_str())){
        ModalResult = mrOk;
    }
    else{
        Application->MessageBox("Identity verify fails,password is not correct!.", "Fail", MB_OK);
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool __fastcall TIdentity::VerifyPassWord(AnsiString ansiPassword)
{
    bool bRet = false;
    BYTE key[10]="03055030";
    AnsiString asPath = asSetupPath+"\\"+asPasswordName;
    unsigned char password[LOGIN_PASSWORD_MAX];

    if(ReadDataFromFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str()) == S_DES_SUCCESS)
    {
        if(strcmp(password+2, ansiPassword.c_str())== 0)
        {
            bRet = true;
        }
    }

    return bRet;
}

void __fastcall TIdentity::FormCreate(TObject *Sender)
{
    AnsiString asPath;
    edtPassword->Text = "";
    asSetupPath = "";
    if (getPathFromStr(Application->ExeName, asPath) &&
         withPath(asPath)){
       asSetupPath = asPath;
    }
}
//---------------------------------------------------------------------------

void __fastcall TIdentity::m_btnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Login_Mode __fastcall TIdentity::GetLoginMode()
{
    Login_Mode loginMode = LM_LONGIN_MODE_UNKNOWN;
    BYTE key[10]="03055030";
    unsigned char password[LOGIN_PASSWORD_MAX];
    AnsiString asPath = asSetupPath+"\\"+asPasswordName;
    if(ReadDataFromFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str()) == S_DES_SUCCESS)
    {
         if(password[0] =='0' && password[1] =='0'){
            loginMode = LM_OPERATOR;
        }else if(password[0] =='0' && password[1] =='1'){
            loginMode = LM_ENGINEER;
        }else{
            loginMode = LM_LONGIN_MODE_UNKNOWN;
        }
    }

    return loginMode;
}

int __fastcall TIdentity::SetLoginMode(Login_Mode loginMode)
{
    BYTE key[10]="03055030";
    int iRet = -1;
    unsigned char password[LOGIN_PASSWORD_MAX];
    AnsiString asPath = asSetupPath+"\\"+asPasswordName;

    if(ReadDataFromFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str()) == S_DES_SUCCESS)
    {
        if(loginMode == LM_OPERATOR){
            password[0] = '0';
            password[1] = '0';
        }else if(loginMode == LM_ENGINEER){
            password[0] = '0';
            password[1] = '1';
        }else{
           goto Finish;
        }
        iRet = WriteDataToFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str());
    }

Finish:
    return iRet;
}


void __fastcall TIdentity::FormShow(TObject *Sender)
{
     edtPassword->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TIdentity::edtPasswordKeyPress(TObject *Sender, char &Key)
{
    if( Key == 0x0D )
    {
        m_btnEnterClick(Sender);
    }        
}
//---------------------------------------------------------------------------

