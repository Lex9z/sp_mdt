object formPCMemoryChecksum: TformPCMemoryChecksum
  Left = 331
  Top = 268
  Width = 441
  Height = 193
  BorderIcons = [biMaximize]
  Caption = 'PC memory Checksum'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object checksumImageNameLabel: TLabel
    Left = 16
    Top = 56
    Width = 105
    Height = 13
    Caption = '                                   '
  end
  object PCMemoryChecksumProgressBar: TCGauge
    Left = 48
    Top = 80
    Width = 361
    Height = 25
  end
  object checksumTipLabel: TLabel
    Left = 16
    Top = 32
    Width = 400
    Height = 13
    Caption = 
      'It is verifing PC memory Checksum, Please wait for a minute  pat' +
      'iently.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clTeal
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object MD5ChecksumLabel1: TLabel
    Left = 24
    Top = 8
    Width = 105
    Height = 13
    Caption = '                                   '
  end
end
