object MainForm: TMainForm
  Left = 328
  Top = 127
  Width = 882
  Height = 712
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Smart Phone Multiport Download Tool'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 18
    Width = 60
    Height = 13
    Caption = 'DA file name'
    Layout = tlCenter
  end
  object Label3: TLabel
    Left = 16
    Top = 184
    Width = 74
    Height = 13
    Caption = 'Data source file'
    Layout = tlCenter
    Visible = False
  end
  object Label2: TLabel
    Left = 8
    Top = 52
    Width = 84
    Height = 13
    Caption = 'Authentication file'
    Layout = tlCenter
  end
  object Label5: TLabel
    Left = 16
    Top = 86
    Width = 74
    Height = 13
    Caption = 'Certification File'
    Layout = tlCenter
  end
  object gbFormatFATRange: TGroupBox
    Left = 256
    Top = 184
    Width = 209
    Height = 41
    Caption = '  Format FAT Option  '
    TabOrder = 0
    Visible = False
    object rbManual: TRadioButton
      Left = 8
      Top = 16
      Width = 89
      Height = 17
      Caption = 'Manual'
      Checked = True
      Enabled = False
      TabOrder = 0
      TabStop = True
    end
    object rbAuto: TRadioButton
      Left = 112
      Top = 16
      Width = 89
      Height = 17
      Caption = 'Auto'
      Enabled = False
      TabOrder = 1
    end
  end
  object btnStartAll: TButton
    Left = 648
    Top = 472
    Width = 209
    Height = 41
    Caption = 'Start all'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -25
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btnStartAllClick
  end
  object btnStopAll: TButton
    Left = 648
    Top = 520
    Width = 209
    Height = 41
    Caption = 'Stop all'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -25
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = btnStopAllClick
  end
  object sbMPDT: TStatusBar
    Left = 0
    Top = 623
    Width = 874
    Height = 23
    Panels = <
      item
        Width = 500
      end>
    SimplePanel = False
    SizeGrip = False
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 112
    Width = 833
    Height = 49
    Caption = 'Scatter Files'
    TabOrder = 4
    object ScatterFileButton1: TButton
      Left = 8
      Top = 16
      Width = 81
      Height = 21
      Caption = 'Scatter File'
      TabOrder = 0
      OnClick = ScatterFileButtonClick
    end
    object btDeleteScatterFile1: TButton
      Left = 784
      Top = 16
      Width = 49
      Height = 21
      Caption = 'Delete'
      TabOrder = 1
      Visible = False
      OnClick = btDeleteScatterFileClick
    end
  end
  object stScatterFile1: TStaticText
    Left = 104
    Top = 128
    Width = 681
    Height = 21
    AutoSize = False
    BorderStyle = sbsSunken
    Color = clSkyBlue
    ParentColor = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
  end
  object edit_da_path: TEdit
    Left = 104
    Top = 16
    Width = 737
    Height = 21
    Color = clSkyBlue
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ReadOnly = True
    TabOrder = 6
  end
  object edit_auth_path: TEdit
    Left = 104
    Top = 50
    Width = 737
    Height = 21
    Color = clSkyBlue
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ReadOnly = True
    TabOrder = 7
  end
  object edit_database_path: TEdit
    Left = 72
    Top = 182
    Width = 737
    Height = 21
    Color = clSkyBlue
    Enabled = False
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ReadOnly = True
    TabOrder = 8
    Visible = False
  end
  object gbDLCable: TGroupBox
    Left = 40
    Top = 184
    Width = 209
    Height = 57
    Caption = ' Two-site download '
    TabOrder = 9
    Visible = False
    object rbUART: TRadioButton
      Left = 8
      Top = 17
      Width = 113
      Height = 17
      Caption = 'First-site (UART)'
      Checked = True
      Enabled = False
      TabOrder = 0
      TabStop = True
    end
    object rbUSB: TRadioButton
      Left = 8
      Top = 36
      Width = 113
      Height = 17
      Caption = 'Second-site (USB)'
      Enabled = False
      TabOrder = 1
    end
  end
  object edit_cert_path: TEdit
    Left = 104
    Top = 84
    Width = 737
    Height = 21
    Color = clSkyBlue
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 10
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 168
    Width = 633
    Height = 433
    TabOrder = 11
    object COM1Percentage: TLabel
      Left = 160
      Top = 56
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM2Percentage: TLabel
      Tag = 1
      Left = 160
      Top = 78
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM3Percentage: TLabel
      Tag = 2
      Left = 160
      Top = 100
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM4Percentage: TLabel
      Tag = 3
      Left = 160
      Top = 122
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM5Percentage: TLabel
      Tag = 4
      Left = 160
      Top = 144
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM6Percentage: TLabel
      Tag = 5
      Left = 160
      Top = 167
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -7
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object COM7Percentage: TLabel
      Tag = 6
      Left = 160
      Top = 189
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM8Percentage: TLabel
      Tag = 7
      Left = 160
      Top = 211
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object lblCOM1Time: TLabel
      Left = 471
      Top = 56
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM2Time: TLabel
      Tag = 1
      Left = 471
      Top = 78
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM3Time: TLabel
      Tag = 2
      Left = 471
      Top = 100
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM4Time: TLabel
      Tag = 3
      Left = 471
      Top = 122
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM5Time: TLabel
      Tag = 4
      Left = 471
      Top = 144
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM6Time: TLabel
      Tag = 5
      Left = 471
      Top = 167
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM7Time: TLabel
      Tag = 6
      Left = 471
      Top = 189
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM8Time: TLabel
      Tag = 7
      Left = 471
      Top = 211
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object pbCOM1: TCGauge
      Left = 264
      Top = 56
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM2: TCGauge
      Tag = 1
      Left = 264
      Top = 78
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM3: TCGauge
      Tag = 2
      Left = 264
      Top = 100
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM4: TCGauge
      Tag = 3
      Left = 264
      Top = 122
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM5: TCGauge
      Tag = 4
      Left = 264
      Top = 144
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM6: TCGauge
      Tag = 5
      Left = 264
      Top = 167
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM7: TCGauge
      Tag = 6
      Left = 264
      Top = 189
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM8: TCGauge
      Tag = 7
      Left = 264
      Top = 211
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object shpCOM1: TShape
      Left = 138
      Top = 56
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM2: TShape
      Tag = 1
      Left = 138
      Top = 78
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM4: TShape
      Tag = 3
      Left = 138
      Top = 122
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM5: TShape
      Tag = 4
      Left = 138
      Top = 144
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM6: TShape
      Tag = 5
      Left = 138
      Top = 167
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM7: TShape
      Tag = 6
      Left = 138
      Top = 189
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM8: TShape
      Tag = 7
      Left = 138
      Top = 211
      Width = 17
      Height = 17
      Brush.Color = clGreen
      ParentShowHint = False
      Shape = stCircle
      ShowHint = False
    end
    object shpCOM3: TShape
      Tag = 2
      Left = 138
      Top = 100
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM9: TShape
      Tag = 8
      Left = 138
      Top = 233
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM10: TShape
      Tag = 9
      Left = 138
      Top = 255
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM11: TShape
      Tag = 10
      Left = 138
      Top = 278
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM12: TShape
      Tag = 11
      Left = 138
      Top = 300
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM13: TShape
      Tag = 12
      Left = 138
      Top = 322
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM14: TShape
      Tag = 13
      Left = 138
      Top = 344
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM15: TShape
      Tag = 14
      Left = 138
      Top = 366
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object shpCOM16: TShape
      Tag = 15
      Left = 138
      Top = 389
      Width = 17
      Height = 17
      Brush.Color = clGreen
      Shape = stCircle
    end
    object COM9Percentage: TLabel
      Tag = 8
      Left = 160
      Top = 233
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM10Percentage: TLabel
      Tag = 9
      Left = 160
      Top = 255
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM11Percentage: TLabel
      Tag = 10
      Left = 160
      Top = 278
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM12Percentage: TLabel
      Tag = 11
      Left = 160
      Top = 300
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM13Percentage: TLabel
      Tag = 12
      Left = 160
      Top = 322
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM14Percentage: TLabel
      Tag = 13
      Left = 160
      Top = 344
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM15Percentage: TLabel
      Tag = 14
      Left = 160
      Top = 366
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object COM16Percentage: TLabel
      Tag = 15
      Left = 160
      Top = 389
      Width = 100
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
    end
    object pbCOM9: TCGauge
      Tag = 8
      Left = 264
      Top = 233
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM10: TCGauge
      Tag = 9
      Left = 264
      Top = 255
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM11: TCGauge
      Tag = 10
      Left = 264
      Top = 278
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM12: TCGauge
      Tag = 11
      Left = 264
      Top = 300
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM13: TCGauge
      Tag = 12
      Left = 264
      Top = 322
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM14: TCGauge
      Tag = 13
      Left = 264
      Top = 344
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM15: TCGauge
      Tag = 14
      Left = 264
      Top = 366
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object pbCOM16: TCGauge
      Tag = 15
      Left = 264
      Top = 389
      Width = 185
      Height = 17
      Color = clWhite
      ForeColor = clWhite
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
    end
    object lblCOM9Time: TLabel
      Tag = 8
      Left = 471
      Top = 233
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM10Time: TLabel
      Tag = 9
      Left = 471
      Top = 255
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM11Time: TLabel
      Tag = 10
      Left = 471
      Top = 278
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM12Time: TLabel
      Tag = 11
      Left = 471
      Top = 300
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM13Time: TLabel
      Tag = 12
      Left = 471
      Top = 322
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM14Time: TLabel
      Tag = 13
      Left = 471
      Top = 344
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM15Time: TLabel
      Tag = 14
      Left = 471
      Top = 366
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object lblCOM16Time: TLabel
      Tag = 15
      Left = 471
      Top = 389
      Width = 16
      Height = 13
      Alignment = taRightJustify
      Caption = '0 S'
      Layout = tlCenter
    end
    object Label4: TLabel
      Left = 8
      Top = 56
      Width = 6
      Height = 13
      Caption = '1'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBackground
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object Label7: TLabel
      Left = 8
      Top = 78
      Width = 6
      Height = 13
      Caption = '2'
    end
    object Label8: TLabel
      Left = 8
      Top = 100
      Width = 6
      Height = 13
      Caption = '3'
    end
    object Label10: TLabel
      Left = 8
      Top = 122
      Width = 6
      Height = 13
      Caption = '4'
    end
    object Label9: TLabel
      Left = 8
      Top = 144
      Width = 6
      Height = 13
      Caption = '5'
    end
    object Label11: TLabel
      Left = 8
      Top = 167
      Width = 6
      Height = 13
      Caption = '6'
    end
    object Label12: TLabel
      Left = 8
      Top = 189
      Width = 6
      Height = 13
      Caption = '7'
    end
    object Label13: TLabel
      Left = 8
      Top = 211
      Width = 6
      Height = 13
      Caption = '8'
    end
    object Label14: TLabel
      Left = 8
      Top = 233
      Width = 6
      Height = 13
      Caption = '9'
    end
    object Label15: TLabel
      Left = 8
      Top = 255
      Width = 12
      Height = 13
      Caption = '10'
    end
    object Label16: TLabel
      Left = 8
      Top = 278
      Width = 12
      Height = 13
      Caption = '11'
    end
    object Label17: TLabel
      Left = 8
      Top = 300
      Width = 12
      Height = 13
      Caption = '12'
    end
    object Label18: TLabel
      Left = 8
      Top = 322
      Width = 12
      Height = 13
      Caption = '13'
    end
    object Label19: TLabel
      Left = 8
      Top = 344
      Width = 12
      Height = 13
      Caption = '14'
    end
    object Label20: TLabel
      Left = 8
      Top = 366
      Width = 12
      Height = 13
      Caption = '15'
    end
    object Label21: TLabel
      Left = 8
      Top = 389
      Width = 12
      Height = 13
      Caption = '16'
    end
    object btnCOM1Start: TButton
      Left = 496
      Top = 56
      Width = 40
      Height = 17
      Caption = 'Start'
      ParentShowHint = False
      ShowHint = False
      TabOrder = 0
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM2Start: TButton
      Tag = 1
      Left = 496
      Top = 78
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 1
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM3Start: TButton
      Tag = 2
      Left = 496
      Top = 100
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 2
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM4Start: TButton
      Tag = 3
      Left = 496
      Top = 122
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 3
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM5Start: TButton
      Tag = 4
      Left = 496
      Top = 144
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 4
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM6Start: TButton
      Tag = 5
      Left = 496
      Top = 167
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 5
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM7Start: TButton
      Tag = 6
      Left = 496
      Top = 189
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 6
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM8Start: TButton
      Tag = 7
      Left = 496
      Top = 211
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 7
      OnClick = btnSeperateDownloadClick
    end
    object cbCOM1: TCheckBox
      Left = 24
      Top = 56
      Width = 113
      Height = 17
      Caption = 'COM1+N/A'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
      OnClick = cbCOMClick
    end
    object cbCOM2: TCheckBox
      Tag = 1
      Left = 24
      Top = 78
      Width = 113
      Height = 17
      Caption = 'COM2'
      TabOrder = 9
      OnClick = cbCOMClick
    end
    object cbCOM3: TCheckBox
      Tag = 2
      Left = 24
      Top = 100
      Width = 113
      Height = 17
      Caption = 'COM3'
      TabOrder = 10
      OnClick = cbCOMClick
    end
    object cbCOM4: TCheckBox
      Tag = 3
      Left = 24
      Top = 122
      Width = 113
      Height = 17
      Caption = 'COM4'
      TabOrder = 11
      OnClick = cbCOMClick
    end
    object cbCOM5: TCheckBox
      Tag = 4
      Left = 24
      Top = 144
      Width = 113
      Height = 17
      Caption = 'COM5'
      TabOrder = 12
      OnClick = cbCOMClick
    end
    object cbCOM6: TCheckBox
      Tag = 5
      Left = 24
      Top = 167
      Width = 113
      Height = 17
      Caption = 'COM6'
      TabOrder = 13
      OnClick = cbCOMClick
    end
    object cbCOM7: TCheckBox
      Tag = 6
      Left = 24
      Top = 189
      Width = 113
      Height = 17
      Caption = 'COM7'
      TabOrder = 14
      OnClick = cbCOMClick
    end
    object cbCOM8: TCheckBox
      Tag = 7
      Left = 24
      Top = 211
      Width = 113
      Height = 17
      Caption = 'COM8'
      TabOrder = 15
      OnClick = cbCOMClick
    end
    object btnCOM1Stop: TButton
      Left = 536
      Top = 56
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 16
      OnClick = btnSeperateStopClick
    end
    object btnCOM2Stop: TButton
      Tag = 1
      Left = 536
      Top = 78
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 17
      OnClick = btnSeperateStopClick
    end
    object btnCOM3Stop: TButton
      Tag = 2
      Left = 536
      Top = 100
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 18
      OnClick = btnSeperateStopClick
    end
    object btnCOM4Stop: TButton
      Tag = 3
      Left = 536
      Top = 122
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 19
      OnClick = btnSeperateStopClick
    end
    object btnCOM5Stop: TButton
      Tag = 4
      Left = 536
      Top = 144
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 20
      OnClick = btnSeperateStopClick
    end
    object btnCOM6Stop: TButton
      Tag = 5
      Left = 536
      Top = 167
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 21
      OnClick = btnSeperateStopClick
    end
    object btnCOM7Stop: TButton
      Tag = 6
      Left = 536
      Top = 189
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 22
      OnClick = btnSeperateStopClick
    end
    object btnCOM8Stop: TButton
      Tag = 7
      Left = 536
      Top = 211
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 23
      OnClick = btnSeperateStopClick
    end
    object m_stBkStatus0: TStaticText
      Left = 584
      Top = 56
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 24
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus1: TStaticText
      Tag = 1
      Left = 584
      Top = 78
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 25
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus2: TStaticText
      Tag = 2
      Left = 584
      Top = 100
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 26
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus3: TStaticText
      Tag = 3
      Left = 584
      Top = 122
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 27
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus4: TStaticText
      Tag = 4
      Left = 584
      Top = 144
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 28
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus5: TStaticText
      Tag = 5
      Left = 584
      Top = 167
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 29
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus6: TStaticText
      Tag = 6
      Left = 584
      Top = 189
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 30
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus7: TStaticText
      Tag = 7
      Left = 584
      Top = 211
      Width = 33
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 31
      OnDblClick = m_stBkStatusDbClick
    end
    object cbSelAllCOM: TCheckBox
      Left = 24
      Top = 32
      Width = 209
      Height = 17
      Caption = 'BootRom+PreLoader COM Sel All'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 32
      OnClick = cbSelAllCOMClick
    end
    object cbCOM9: TCheckBox
      Tag = 8
      Left = 24
      Top = 233
      Width = 113
      Height = 17
      Caption = 'COM9'
      TabOrder = 33
      OnClick = cbCOMClick
    end
    object cbCOM10: TCheckBox
      Tag = 9
      Left = 24
      Top = 255
      Width = 113
      Height = 17
      Caption = 'COM10'
      TabOrder = 34
      OnClick = cbCOMClick
    end
    object cbCOM11: TCheckBox
      Tag = 10
      Left = 24
      Top = 278
      Width = 113
      Height = 17
      Caption = 'COM11'
      TabOrder = 35
      OnClick = cbCOMClick
    end
    object cbCOM12: TCheckBox
      Tag = 11
      Left = 24
      Top = 300
      Width = 113
      Height = 17
      Caption = 'COM12'
      TabOrder = 36
      OnClick = cbCOMClick
    end
    object cbCOM13: TCheckBox
      Tag = 12
      Left = 24
      Top = 322
      Width = 113
      Height = 17
      Caption = 'COM13'
      TabOrder = 37
      OnClick = cbCOMClick
    end
    object cbCOM14: TCheckBox
      Tag = 13
      Left = 24
      Top = 344
      Width = 113
      Height = 17
      Caption = 'COM14'
      TabOrder = 38
      OnClick = cbCOMClick
    end
    object cbCOM15: TCheckBox
      Tag = 14
      Left = 24
      Top = 366
      Width = 113
      Height = 17
      Caption = 'COM15'
      TabOrder = 39
      OnClick = cbCOMClick
    end
    object cbCOM16: TCheckBox
      Tag = 15
      Left = 24
      Top = 389
      Width = 113
      Height = 17
      Caption = 'COM16'
      TabOrder = 40
      OnClick = cbCOMClick
    end
    object btnCOM9Start: TButton
      Tag = 8
      Left = 496
      Top = 233
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 41
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM10Start: TButton
      Tag = 9
      Left = 496
      Top = 255
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 42
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM11Start: TButton
      Tag = 10
      Left = 496
      Top = 278
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 43
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM12Start: TButton
      Tag = 11
      Left = 496
      Top = 300
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 44
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM13Start: TButton
      Tag = 12
      Left = 496
      Top = 322
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 45
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM14Start: TButton
      Tag = 13
      Left = 496
      Top = 344
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 46
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM15Start: TButton
      Tag = 14
      Left = 496
      Top = 366
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 47
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM16Start: TButton
      Tag = 15
      Left = 496
      Top = 389
      Width = 40
      Height = 17
      Caption = 'Start'
      TabOrder = 48
      OnClick = btnSeperateDownloadClick
    end
    object btnCOM9Stop: TButton
      Tag = 8
      Left = 536
      Top = 233
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 49
      OnClick = btnSeperateStopClick
    end
    object btnCOM10Stop: TButton
      Tag = 9
      Left = 536
      Top = 255
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 50
      OnClick = btnSeperateStopClick
    end
    object btnCOM11Stop: TButton
      Tag = 10
      Left = 536
      Top = 278
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 51
      OnClick = btnSeperateStopClick
    end
    object btnCOM12Stop: TButton
      Tag = 11
      Left = 536
      Top = 300
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 52
      OnClick = btnSeperateStopClick
    end
    object btnCOM13Stop: TButton
      Tag = 12
      Left = 536
      Top = 322
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 53
      OnClick = btnSeperateStopClick
    end
    object btnCOM14Stop: TButton
      Tag = 13
      Left = 536
      Top = 344
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 54
      OnClick = btnSeperateStopClick
    end
    object btnCOM15Stop: TButton
      Tag = 14
      Left = 536
      Top = 366
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 55
      OnClick = btnSeperateStopClick
    end
    object btnCOM16Stop: TButton
      Tag = 15
      Left = 536
      Top = 389
      Width = 40
      Height = 17
      Caption = 'Stop'
      TabOrder = 56
      OnClick = btnSeperateStopClick
    end
    object m_stBkStatus8: TStaticText
      Tag = 8
      Left = 584
      Top = 233
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 57
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus9: TStaticText
      Tag = 9
      Left = 584
      Top = 255
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 58
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus10: TStaticText
      Tag = 10
      Left = 584
      Top = 278
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 59
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus11: TStaticText
      Tag = 11
      Left = 584
      Top = 300
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 60
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus12: TStaticText
      Tag = 12
      Left = 584
      Top = 322
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 61
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus13: TStaticText
      Tag = 13
      Left = 584
      Top = 344
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 62
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus14: TStaticText
      Tag = 14
      Left = 584
      Top = 366
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 63
      OnDblClick = m_stBkStatusDbClick
    end
    object m_stBkStatus15: TStaticText
      Tag = 15
      Left = 584
      Top = 389
      Width = 33
      Height = 17
      Hint = 'Backup or restore status: current number / total number'
      Alignment = taCenter
      AutoSize = False
      Color = clMoneyGreen
      ParentColor = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 64
      OnDblClick = m_stBkStatusDbClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 648
    Top = 168
    Width = 209
    Height = 89
    TabOrder = 12
    object Label6: TLabel
      Left = 9
      Top = 20
      Width = 24
      Height = 13
      Caption = 'Type'
      Layout = tlCenter
    end
    object m_lblBaudRate: TLabel
      Left = 11
      Top = 52
      Width = 46
      Height = 13
      Caption = 'Baud rate'
      Layout = tlCenter
    end
    object cbBaudrate: TComboBox
      Left = 72
      Top = 52
      Width = 129
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 0
      Text = '115200'
      OnChange = cbBaudrateChange
      Items.Strings = (
        '115200'
        '230400'
        '460800'
        '921600')
    end
    object cbType: TComboBox
      Left = 48
      Top = 20
      Width = 153
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 1
      OnChange = cbTypeChange
    end
  end
  object GroupBox4: TGroupBox
    Left = 648
    Top = 264
    Width = 209
    Height = 121
    Caption = ' DA download all '
    TabOrder = 13
    object Panel1: TPanel
      Left = 8
      Top = 20
      Width = 193
      Height = 33
      BevelOuter = bvLowered
      TabOrder = 0
      object m_miDADLFullSpeed: TRadioButton
        Left = 8
        Top = 8
        Width = 89
        Height = 17
        Caption = 'Full speed'
        Checked = True
        Enabled = False
        TabOrder = 0
        TabStop = True
      end
      object m_miDADLHighSpeed: TRadioButton
        Left = 104
        Top = 8
        Width = 81
        Height = 17
        Caption = 'High speed'
        Enabled = False
        TabOrder = 1
      end
    end
    object Panel2: TPanel
      Left = 8
      Top = 60
      Width = 193
      Height = 53
      BevelOuter = bvLowered
      TabOrder = 1
      object m_miDADLBattery: TRadioButton
        Left = 8
        Top = 8
        Width = 73
        Height = 17
        Caption = 'Battery'
        Checked = True
        Enabled = False
        TabOrder = 0
        TabStop = True
      end
      object m_miDADLNoBattery: TRadioButton
        Left = 104
        Top = 8
        Width = 81
        Height = 17
        Caption = 'No battery'
        Enabled = False
        TabOrder = 1
      end
      object m_miDADLAutoDetect: TRadioButton
        Left = 8
        Top = 32
        Width = 81
        Height = 17
        Caption = 'Auto detect'
        Enabled = False
        TabOrder = 2
      end
    end
  end
  object btnScan: TButton
    Left = 648
    Top = 568
    Width = 81
    Height = 33
    Caption = 'Scan'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    OnClick = btnScanClick
  end
  object GroupBox5: TGroupBox
    Left = 648
    Top = 392
    Width = 209
    Height = 65
    Caption = 'AutoPulling'
    TabOrder = 15
    object m_cbEnableAutoPulling: TCheckBox
      Left = 16
      Top = 24
      Width = 137
      Height = 17
      Caption = 'EnableAutoPulling'
      TabOrder = 0
    end
  end
  object TimerCOM1: TTimer
    Enabled = False
    OnTimer = TimerCOM1Timer
    Left = 248
    Top = 48
  end
  object TimerCOM2: TTimer
    Tag = 1
    Enabled = False
    OnTimer = TimerCOM2Timer
    Left = 280
    Top = 48
  end
  object TimerCOM3: TTimer
    Tag = 2
    Enabled = False
    OnTimer = TimerCOM3Timer
    Left = 312
    Top = 48
  end
  object TimerCOM4: TTimer
    Tag = 3
    Enabled = False
    OnTimer = TimerCOM4Timer
    Left = 344
    Top = 48
  end
  object TimerCOM5: TTimer
    Tag = 4
    Enabled = False
    OnTimer = TimerCOM5Timer
    Left = 376
    Top = 48
  end
  object TimerCOM6: TTimer
    Tag = 5
    Enabled = False
    OnTimer = TimerCOM6Timer
    Left = 408
    Top = 48
  end
  object dlgOpenDA: TOpenDialog
    DefaultExt = '*.bin'
    Filter = 'Bin File (*.bin)|*.bin|All File (*.*)|*.*'
    Title = 'Open'
    Left = 632
    Top = 16
  end
  object TimerCOM7: TTimer
    Tag = 6
    Enabled = False
    OnTimer = TimerCOM7Timer
    Left = 504
    Top = 48
  end
  object TimerCOM8: TTimer
    Tag = 7
    Enabled = False
    OnTimer = TimerCOM8Timer
    Left = 536
    Top = 48
  end
  object MainMenu1: TMainMenu
    Left = 600
    Top = 16
    object miFile: TMenuItem
      Caption = '&File'
      ShortCut = 16454
      object OpenDownloadAgentFile1: TMenuItem
        Caption = 'Open Download Agent File'
        OnClick = OpenDownloadAgentFile1Click
      end
      object OpenScatterFile1: TMenuItem
        Caption = 'Scatter and Load Setting ...'
        OnClick = OpenScatterFile1Click
      end
      object miOpenAuthFile: TMenuItem
        Caption = 'Open Authentication File'
        OnClick = miOpenAuthFileClick
      end
      object miOpenCertFile: TMenuItem
        Caption = 'Open Certification File'
        OnClick = miOpenCertFileClick
      end
      object OpenMemoryVerificationListFile1: TMenuItem
        Caption = 'Open Memory Verification List Excel File'
        OnClick = OpenMemoryVerificationListFile1Click
      end
    end
    object miFactoryDatabase: TMenuItem
      Caption = 'Factory &database'
      ShortCut = 16452
      Visible = False
      object miSetupDataSrc: TMenuItem
        Caption = 'Setup data source file'
        OnClick = miSetupDataSrcClick
      end
      object miEnableFDLog: TMenuItem
        Caption = 'Enable factory database logging'
        OnClick = miEnableFDLogClick
      end
    end
    object miOption: TMenuItem
      Caption = '&Option'
      ShortCut = 16463
      object miBasebandchip: TMenuItem
        Caption = 'Baseband chip'
        object miAutodetectBB: TMenuItem
          Caption = 'Auto detect'
          Checked = True
          OnClick = miAutodetectBBClick
        end
      end
      object miExternalclock: TMenuItem
        Caption = 'External clock'
        object miAutodetectExtClock: TMenuItem
          Caption = 'Auto detect'
          Checked = True
          Default = True
          OnClick = miAutodetectExtClockClick
        end
        object mi13MHz: TMenuItem
          Caption = '13 MHz'
          OnClick = mi13MHzClick
        end
        object mi26MHz: TMenuItem
          Caption = '26 MHz'
          OnClick = mi26MHzClick
        end
      end
      object miBasebandchipoption: TMenuItem
        Caption = 'Baseband chip option'
        Enabled = False
        Visible = False
        object miCheckECOversion: TMenuItem
          Caption = 'Check ECO version'
          Enabled = False
          OnClick = miCheckECOversionClick
        end
        object miEnableNFICS1: TMenuItem
          Caption = 'Enable NFI CS1'
          Enabled = False
          OnClick = miEnableNFICS1Click
        end
      end
      object MultiBindownloadoption1: TMenuItem
        Caption = 'Multi-Bin download option'
        Enabled = False
        Visible = False
        object miCheckTargetResourceLayout: TMenuItem
          Caption = 'Check target resource layout'
          Checked = True
          Enabled = False
          OnClick = miCheckTargetResourceLayoutClick
        end
        object miAutoLoadResourcefilesByScatter: TMenuItem
          Caption = 'Auto load resource files by scatter'
          Checked = True
          Enabled = False
          OnClick = miAutoLoadResourcefilesByScatterClick
        end
        object miCompareResourceProjectId: TMenuItem
          Caption = 'Compare resource project id'
          Checked = True
          Enabled = False
          OnClick = miCompareResourceProjectIdClick
        end
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object m_miEnableDADL: TMenuItem
        Caption = 'Enable DA download all'
        Checked = True
        Enabled = False
        OnClick = m_miEnableDADLClick
      end
      object m_miEnableDACksm: TMenuItem
        Caption = 'Enable DA checksum'
        OnClick = m_miEnableDACksmClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object m_miSettings: TMenuItem
        Caption = 'Settings...'
        OnClick = m_miSettingsClick
      end
    end
    object miDebuglog: TMenuItem
      Caption = 'Debug &Logging'
      ShortCut = 16460
      object miBromDllLog: TMenuItem
        Caption = 'Enable BROM_DLL logging'
        OnClick = miBromDllLogClick
      end
      object miClearBromDllLog: TMenuItem
        Caption = 'Clear BROM_DLL log'
        OnClick = miClearBromDllLogClick
      end
      object miMETADllLog: TMenuItem
        Caption = 'Enable META_DLL logging'
        OnClick = miMETADllLogClick
      end
      object miClearMETADllLog: TMenuItem
        Caption = 'Clear META_DLL log'
        OnClick = miClearMETADllLogClick
      end
      object miEbootDllLog: TMenuItem
        Caption = 'Enable EBOOT_DLL logging'
        OnClick = miEbootDllLogClick
      end
      object miCllearEbootDllLog: TMenuItem
        Caption = 'Clear EBOOT_DLL logging'
        OnClick = miCllearEbootDllLogClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object miClearAlllogs: TMenuItem
        Caption = 'Clear All logs'
        ShortCut = 49219
        OnClick = miClearAlllogsClick
      end
    end
    object Identity1: TMenuItem
      Caption = 'Identity'
      object m_EngeerMode: TMenuItem
        Caption = 'EngineerMode'
        object SwithToOperator1: TMenuItem
          Caption = 'SwithToOperator'
          OnClick = SwithToOperator1Click
        end
        object ChangePassword1: TMenuItem
          Caption = 'Change Password'
          OnClick = ChangePassword1Click
        end
      end
      object m_OperatorMode: TMenuItem
        Caption = 'OperatorMode'
        object SwitchToEngineer1: TMenuItem
          Caption = 'SwitchToEngineer'
          OnClick = SwitchToEngineer1Click
        end
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      ShortCut = 16456
      object DeviceManager1: TMenuItem
        Caption = 'Open Device Manager'
        OnClick = DeviceManager1Click
      end
      object miOpenLogFolder: TMenuItem
        Caption = 'Open Log Folder'
        OnClick = miOpenLogFolderClick
      end
      object miAbout: TMenuItem
        Caption = '&About'
        ShortCut = 16449
        OnClick = miAboutClick
      end
    end
  end
  object TimerCOM9: TTimer
    Tag = 8
    Enabled = False
    OnTimer = TimerCOM9Timer
    Left = 568
    Top = 48
  end
  object TimerCOM10: TTimer
    Tag = 9
    Enabled = False
    OnTimer = TimerCOM10Timer
    Left = 600
    Top = 48
  end
  object TimerCOM11: TTimer
    Tag = 10
    Enabled = False
    OnTimer = TimerCOM11Timer
    Left = 632
    Top = 48
  end
  object TimerCOM12: TTimer
    Tag = 11
    Enabled = False
    OnTimer = TimerCOM12Timer
    Left = 664
    Top = 48
  end
  object TimerCOM13: TTimer
    Tag = 12
    Enabled = False
    OnTimer = TimerCOM13Timer
    Left = 696
    Top = 48
  end
  object TimerCOM14: TTimer
    Tag = 13
    Enabled = False
    OnTimer = TimerCOM14Timer
    Left = 440
    Top = 48
  end
  object TimerCOM15: TTimer
    Tag = 14
    Enabled = False
    OnTimer = TimerCOM15Timer
    Left = 728
    Top = 48
  end
  object TimerCOM16: TTimer
    Tag = 15
    Enabled = False
    OnTimer = TimerCOM16Timer
    Left = 472
    Top = 48
  end
  object DataSrc: TDataSource
    Left = 504
    Top = 16
  end
  object ADOTab: TADOTable
    Connection = ADOConn
    Left = 536
    Top = 16
  end
  object ADOConn: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Multiport downlo' +
      'ad source\Multiport_DL source v2.4.1013.1\output\MTK_DATABASE.md' +
      'b;Persist Security Info=False'
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 568
    Top = 16
  end
  object dlgOpenDataSrc: TOpenDialog
    Filter = 'mdb files (*.mdb)|*.mdb|All files (*.*)|*.*'
    Left = 664
    Top = 16
  end
  object dlgOpenAuthFile: TOpenDialog
    DefaultExt = '*.auth'
    Filter = 'Authentication File (*.auth)|*.auth|All File (*.*)|*.*'
    InitialDir = '.'
    Title = 'Open AUTH File'
    Left = 697
    Top = 16
  end
  object dlgOpenFlashBinFile: TOpenDialog
    DefaultExt = '*.bin'
    Filter = 'Flash Bin File (*.bin)|*.bin|All File (*.*)|*.*'
    InitialDir = '.'
    Title = 'Open Flash Bin File'
    Left = 729
    Top = 16
  end
  object dlgOpenCertFile: TOpenDialog
    Left = 248
    Top = 80
  end
  object ActionList1: TActionList
    Left = 280
    Top = 80
    object ManualScan: TAction
      Caption = 'ManualScan'
      ShortCut = 49235
      OnExecute = ManualScanExecute
    end
  end
end
