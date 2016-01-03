//---------------------------------------------------------------------------

#ifndef ToolSettingsH
#define ToolSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_tool_settings : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TRadioGroup *RadioGroup1;
        TRadioButton *m_rbCOMAsName;
        TRadioButton *m_rbRIDAsName;
        TButton *m_btToolSettingsOK;
        TButton *m_btToolSettingsCancel;
        TGroupBox *GroupBox1;
        TCheckBox *m_cbBackupDel;
        TCheckBox *CheckBox1;
        TPageControl *PageControl2;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox2;
        TCheckBox *m_cbOTPLock;
        TCheckBox *m_cbOTPFormat;
        TCheckBox *m_cbFirmwareUpgradeAndOTPLock;
        void __fastcall m_btToolSettingsOKClick(TObject *Sender);
        void __fastcall m_btToolSettingsCancelClick(TObject *Sender);
        void __fastcall m_cbOTPLockClick(TObject *Sender);
        void __fastcall m_cbOTPFormatClick(TObject *Sender);
        void __fastcall m_cbFirmwareUpgradeAndOTPLockClick(
          TObject *Sender);
private:	// User declarations
	void __fastcall UpdateBkRsOption();
	void __fastcall UpdateOTPOption();
public:		// User declarations
        __fastcall TForm_tool_settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_tool_settings *Form_tool_settings;
//---------------------------------------------------------------------------
#endif
