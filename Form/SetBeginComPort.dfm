object dlgSetBeginComPort: TdlgSetBeginComPort
  Left = 513
  Top = 383
  Width = 282
  Height = 99
  Caption = 'SetBeginComPort'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object laSetBeginComPort: TLabel
    Left = 24
    Top = 24
    Width = 82
    Height = 13
    Caption = 'Begin COM Port :'
  end
  object ed_begin_com_port: TEdit
    Left = 112
    Top = 16
    Width = 65
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 0
  end
  object cbBCPApply: TButton
    Left = 192
    Top = 16
    Width = 65
    Height = 25
    Caption = 'Apply'
    TabOrder = 1
    OnClick = cbBCPApplyClick
  end
end
