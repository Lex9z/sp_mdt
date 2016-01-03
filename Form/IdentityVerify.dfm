object Identity: TIdentity
  Left = 326
  Top = 224
  Width = 285
  Height = 215
  Caption = 'Identity'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 24
    Width = 163
    Height = 20
    Caption = 'Please input password:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 32
    Top = 136
    Width = 128
    Height = 16
    Caption = 'Default password:000'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object edtPassword: TEdit
    Left = 32
    Top = 48
    Width = 209
    Height = 21
    TabOrder = 0
    OnKeyPress = edtPasswordKeyPress
  end
  object m_btnEnter: TButton
    Left = 32
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Enter'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = m_btnEnterClick
  end
  object m_btnCancel: TButton
    Left = 168
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = m_btnCancelClick
  end
end
