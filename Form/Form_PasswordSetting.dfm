object PasswordSetting: TPasswordSetting
  Left = 399
  Top = 190
  Width = 345
  Height = 256
  Caption = 'Password Setting'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 48
    Top = 40
    Width = 84
    Height = 16
    Caption = 'Old Password'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 42
    Top = 80
    Width = 90
    Height = 16
    Caption = 'New Password'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 24
    Top = 112
    Width = 108
    Height = 16
    Caption = 'Confirm Password'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 24
    Top = 184
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
  object edtOldPassword: TEdit
    Left = 144
    Top = 40
    Width = 153
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
    OnKeyPress = edtOldPasswordKeyPress
  end
  object edtNewPassword: TEdit
    Left = 144
    Top = 80
    Width = 153
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
    OnKeyPress = edtNewPasswordKeyPress
  end
  object edtConfirmPassword: TEdit
    Left = 144
    Top = 112
    Width = 153
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
    OnKeyPress = edtConfirmPasswordKeyPress
  end
  object btnSetPassword: TButton
    Left = 64
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Set'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = btnSetPasswordClick
  end
  object BtnCancel: TButton
    Left = 200
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = BtnCancelClick
  end
end
