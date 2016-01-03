//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ToolSettings.h"
#include "MultiportFlashDownloadTool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_tool_settings *Form_tool_settings;
//---------------------------------------------------------------------------
__fastcall TForm_tool_settings::TForm_tool_settings(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TForm_tool_settings::m_btToolSettingsOKClick(
      TObject *Sender)
{
	UpdateBkRsOption();
	UpdateOTPOption();
	Close();
}

void __fastcall TForm_tool_settings::m_btToolSettingsCancelClick(
      TObject *Sender)
{
        Close();        
}



void __fastcall TForm_tool_settings::UpdateBkRsOption()
{
	if(m_rbCOMAsName->Checked)
	{
		MainForm->Get_DNParameter()->e_BkRs_folderName = OP_COM_FOLDER_NAME;
	}
	else if(m_rbRIDAsName->Checked)
	{
		MainForm->Get_DNParameter()->e_BkRs_folderName = OP_RID_FOLDER_NAME;
	}

	MainForm->Get_DNParameter()->b_BkRs_deletion = m_cbBackupDel->Checked;
}
//---------------------------------------------------------------------------


//mtk71145 add for otp
void __fastcall TForm_tool_settings::UpdateOTPOption()
{
	if(m_cbOTPLock->Checked||m_cbOTPFormat->Checked||m_cbFirmwareUpgradeAndOTPLock->Checked)
	{
		MainForm->cbType->Enabled = false;
	}
	else
	{
		MainForm->cbType->Enabled = true;
	}
}







void __fastcall TForm_tool_settings::m_cbOTPLockClick(TObject *Sender)
{
    if(m_cbOTPLock->Checked == true)
    {
       m_cbOTPFormat->Checked=false;
       m_cbFirmwareUpgradeAndOTPLock->Checked=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_tool_settings::m_cbOTPFormatClick(TObject *Sender)
{
    if(m_cbOTPFormat->Checked == true)
    {
       m_cbOTPLock->Checked = false;
       m_cbFirmwareUpgradeAndOTPLock ->Checked = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_tool_settings::m_cbFirmwareUpgradeAndOTPLockClick(
      TObject *Sender)
{
     if(m_cbFirmwareUpgradeAndOTPLock->Checked == true)
     {
        m_cbOTPLock->Checked = false;
        m_cbOTPFormat->Checked = false;
     }

}
//---------------------------------------------------------------------------



