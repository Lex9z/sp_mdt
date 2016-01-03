object frmMultiLoad: TfrmMultiLoad
  Left = 281
  Top = 317
  Width = 799
  Height = 365
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = '  Scatter and Load Setting'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lv1: TListView
    Left = 6
    Top = 48
    Width = 746
    Height = 177
    Checkboxes = True
    Columns = <
      item
        Caption = 'Name'
        Width = 100
      end
      item
        Caption = 'Region address'
        Width = 100
      end
      item
        Caption = 'Begin Address'
        Width = 100
      end
      item
        Caption = 'End Address'
        Width = 100
      end
      item
        Caption = 'Location'
        Width = 500
      end>
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = lvChange
    OnChanging = lvChanging
    OnClick = lvClick
  end
  object lblScatFileName1: TStaticText
    Left = 88
    Top = 18
    Width = 585
    Height = 17
    AutoSize = False
    BorderStyle = sbsSunken
    Color = clSkyBlue
    ParentColor = False
    TabOrder = 1
  end
  object sbMultiLoad: TStatusBar
    Left = 0
    Top = 298
    Width = 791
    Height = 23
    Panels = <>
    SimplePanel = False
  end
  object btnScatFile1: TButton
    Left = 8
    Top = 18
    Width = 75
    Height = 19
    Caption = 'Scatter File 1'
    TabOrder = 3
    OnClick = btnScatFileClick
  end
  object btnDelLoad1: TButton
    Left = 680
    Top = 16
    Width = 73
    Height = 17
    Caption = 'Delete Load 1'
    TabOrder = 4
    Visible = False
    OnClick = btnDelLoadClick
  end
  object lvPar1: TListView
    Left = 6
    Top = 224
    Width = 745
    Height = 73
    Checkboxes = True
    Columns = <
      item
        Caption = 'Parameter'
        Width = 100
      end
      item
        Caption = 'Information'
        Width = 700
      end>
    TabOrder = 5
    ViewStyle = vsReport
    Visible = False
    OnChange = lvParamChange
    OnChanging = lvParamChanging
    OnClick = lvParamClick
  end
  object dlgOpenScatter1: TOpenDialog
    Title = 'Open'
    Left = 72
    Top = 280
  end
  object dlgOpenROM1: TOpenDialog
    Title = 'Open'
    Left = 208
    Top = 280
  end
  object dlgOpenFAT1: TOpenDialog
    Title = 'Open'
    Left = 344
    Top = 280
  end
  object dlgOpenScatter2: TOpenDialog
    Tag = 1
    Title = 'Open'
    Left = 104
    Top = 280
  end
  object dlgOpenScatter3: TOpenDialog
    Tag = 2
    Title = 'Open'
    Left = 136
    Top = 280
  end
  object dlgOpenScatter4: TOpenDialog
    Tag = 3
    Title = 'Open'
    Left = 168
    Top = 280
  end
  object dlgOpenROM2: TOpenDialog
    Tag = 1
    Title = 'Open'
    Left = 240
    Top = 280
  end
  object dlgOpenROM3: TOpenDialog
    Tag = 2
    Title = 'Open'
    Left = 272
    Top = 280
  end
  object dlgOpenROM4: TOpenDialog
    Tag = 3
    Title = 'Open'
    Left = 304
    Top = 280
  end
  object dlgOpenFAT2: TOpenDialog
    Tag = 1
    Title = 'Open'
    Left = 376
    Top = 280
  end
  object dlgOpenFAT3: TOpenDialog
    Tag = 2
    Title = 'Open'
    Left = 408
    Top = 280
  end
  object dlgOpenFAT4: TOpenDialog
    Tag = 3
    Title = 'Open'
    Left = 440
    Top = 280
  end
  object dlgOpenSecRo: TOpenDialog
    Title = 'Open SEC_RO file ...'
    Left = 472
    Top = 280
  end
  object m_dlgOpenGPS: TOpenDialog
    Title = 'Open GPS ROM'
    Left = 504
    Top = 280
  end
end
