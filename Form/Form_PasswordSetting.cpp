//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form_PasswordSetting.h"
#include "des.h"
#include "file_utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPasswordSetting *PasswordSetting;
//---------------------------------------------------------------------------
__fastcall TPasswordSetting::TPasswordSetting(TComponent* Owner)
        : TForm(Owner)
{
    AnsiString asPath;
    asPasswordName = "MDTDATA";
    asSetupPath = "";
    if (getPathFromStr(Application->ExeName, asPath) &&
         withPath(asPath)){
       asSetupPath = asPath;
    }
}
//---------------------------------------------------------------------------

void __fastcall TPasswordSetting::FormShow(TObject *Sender)
{
    edtOldPassword->Text = "";
    edtNewPassword->Text = "";
    edtConfirmPassword->Text = "";      
}
//---------------------------------------------------------------------------


void __fastcall TPasswordSetting::BtnCancelClick(TObject *Sender)
{
     
    ModalResult = mrCancel; 
}
//---------------------------------------------------------------------------

void __fastcall TPasswordSetting::btnSetPasswordClick(TObject *Sender)
{
    
    BYTE key[10]="03055030";
    AnsiString asPath = asSetupPath+"\\"+asPasswordName;
    unsigned char password[LOGIN_PASSWORD_MAX];
  
    if(edtOldPassword->Text.IsEmpty() || edtNewPassword->Text.IsEmpty() || edtConfirmPassword->Text.IsEmpty()){
	   ShowMessage(AnsiString("The password is null,please input password!")); 
	   return;
	}

	if(edtNewPassword->Text.AnsiCompare(edtConfirmPassword->Text.c_str()) != 0){
	   ShowMessage(AnsiString("New password and Confirm password are not the same!")); 
	   return;
	}

    if(ReadDataFromFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str()) != S_DES_SUCCESS){
       ShowMessage(AnsiString("Read file error, please try again!")); 
	   return;
        
    }

    if(strcmp(password+2, edtOldPassword->Text.c_str()) != 0)
    {
       ShowMessage(AnsiString("Old Password is not correct, Please input password again!")); 
	   return;
    }

	strcpy(password+2, edtNewPassword->Text.c_str());

    if(WriteDataToFile(password,LOGIN_PASSWORD_MAX, key, asPath.c_str())!= S_DES_SUCCESS){
		ShowMessage(AnsiString("Write file error, please try again!")); 
	    return;
	}

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TPasswordSetting::edtOldPasswordKeyPress(TObject *Sender,
      char &Key)
{
    if( Key == 0x0D )
    {
        btnSetPasswordClick(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TPasswordSetting::edtNewPasswordKeyPress(TObject *Sender,
      char &Key)
{
    if( Key == 0x0D )
    {
        btnSetPasswordClick(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TPasswordSetting::edtConfirmPasswordKeyPress(
      TObject *Sender, char &Key)
{
    if( Key == 0x0D )
    {
        btnSetPasswordClick(Sender);
    }
}
//---------------------------------------------------------------------------

