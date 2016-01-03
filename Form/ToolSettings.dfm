object Form_tool_settings: TForm_tool_settings
  Left = 310
  Top = 151
  Width = 429
  Height = 496
  Caption = 'Tool Settings'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 16
    Top = 16
    Width = 369
    Height = 225
    ActivePage = TabSheet1
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Backup/Restore'
      object RadioGroup1: TRadioGroup
        Left = 8
        Top = 8
        Width = 345
        Height = 97
        Caption = 'Backup folder naming'
        TabOrder = 0
      end
      object m_rbCOMAsName: TRadioButton
        Left = 24
        Top = 32
        Width = 281
        Height = 25
        Caption = 'USB COM port as folder name'
        TabOrder = 1
      end
      object m_rbRIDAsName: TRadioButton
        Left = 24
        Top = 64
        Width = 265
        Height = 25
        Caption = 'RID as folder name'
        TabOrder = 2
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 112
        Width = 345
        Height = 73
        Caption = 'NVRAM backup deletion'
        TabOrder = 3
        object m_cbBackupDel: TCheckBox
          Left = 16
          Top = 32
          Width = 297
          Height = 17
          Caption = 'Delete NVRAM backup data (Only for RID)'
          Checked = True
          State = cbChecked
          TabOrder = 0
        end
        object CheckBox1: TCheckBox
          Left = 32
          Top = 72
          Width = 297
          Height = 17
          Caption = 'Delete NVRAM backup data (Only for RID)'
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
      end
    end
  end
  object m_btToolSettingsOK: TButton
    Left = 256
    Top = 424
    Width = 65
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = m_btToolSettingsOKClick
  end
  object m_btToolSettingsCancel: TButton
    Left = 336
    Top = 424
    Width = 65
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = m_btToolSettingsCancelClick
  end
  object PageControl2: TPageControl
    Left = 16
    Top = 256
    Width = 369
    Height = 153
    ActivePage = TabSheet2
    TabIndex = 0
    TabOrder = 3
    object TabSheet2: TTabSheet
      Caption = 'OTP Option (emmc only)'
      object GroupBox2: TGroupBox
        Left = 8
        Top = 8
        Width = 345
        Height = 105
        TabOrder = 0
        object m_cbOTPLock: TCheckBox
          Left = 16
          Top = 24
          Width = 297
          Height = 17
          Caption = 'OTP Lock'
          TabOrder = 0
          OnClick = m_cbOTPLockClick
        end
        object m_cbOTPFormat: TCheckBox
          Left = 16
          Top = 48
          Width = 297
          Height = 17
          Caption = 'OTP Format'
          TabOrder = 1
          OnClick = m_cbOTPFormatClick
        end
        object m_cbFirmwareUpgradeAndOTPLock: TCheckBox
          Left = 16
          Top = 72
          Width = 297
          Height = 17
          Caption = 'FirmwareUpgrade+OTP Lock'
          TabOrder = 2
          OnClick = m_cbFirmwareUpgradeAndOTPLockClick
        end
      end
    end
  end
end
