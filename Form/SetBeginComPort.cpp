//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "..\INIFileHandle\INIFileHandle.h"
#include "SetBeginComPort.h"
#include "..\MultiPortFlashDownloadTool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdlgSetBeginComPort *dlgSetBeginComPort;

//extern PACKAGE TMainForm *MainForm;

//---------------------------------------------------------------------------
__fastcall TdlgSetBeginComPort::TdlgSetBeginComPort(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TdlgSetBeginComPort::FormCreate(TObject *Sender)
{
   ed_begin_com_port->Text = MainForm->INIFileH.read_TheFirstComPortNo( "SPMultiPortFlashDownloadProject.ini", Application->ExeName );
}
//---------------------------------------------------------------------------
void __fastcall TdlgSetBeginComPort::cbBCPApplyClick(TObject *Sender)
{

   try
   {
       int old_begin_com_port, new_begin_com_port;

       old_begin_com_port = MainForm->INIFileH.read_TheFirstComPortNo( "SPMultiPortFlashDownloadProject.ini", Application->ExeName ).ToInt();;

       new_begin_com_port = ed_begin_com_port->Text.ToInt();

       if((new_begin_com_port <= 0) ||(new_begin_com_port > 200))
       {
          throw EConvertError("Please input an integer which is between 1-200!");
       }

       if(new_begin_com_port == old_begin_com_port)
       {
          dlgSetBeginComPort->Close();
          return;
       }

       MainForm->INIFileH.write_TheFirstComPortNo( "SPMultiPortFlashDownloadProject.ini", Application->ExeName,  ed_begin_com_port->Text);

       Application->MessageBox("Setting have been applied, please reopen tool.", "Set begin COM port", MB_OK);

       dlgSetBeginComPort->Close();
   }
   catch(...)
   {
       Application->MessageBox("Please input an integer which is between 1-200!", "Set begin COM port", MB_OK);
       return;
   }
}
//---------------------------------------------------------------------------
