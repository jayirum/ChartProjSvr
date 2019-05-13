object fmMain: TfmMain
  Left = 0
  Top = 0
  Caption = 'C++ Builder demo for TestMessage.dll'
  ClientHeight = 684
  ClientWidth = 645
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 645
    Height = 684
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = 'Prototype #2 with many instances'
      ImageIndex = 1
      object lblObjNumber: TLabel
        Left = 240
        Top = 37
        Width = 138
        Height = 13
        Caption = 'Objects number : UNKNOWN'
      end
      object lblMaxObjNumber: TLabel
        Left = 424
        Top = 37
        Width = 183
        Height = 13
        Caption = 'Maximum objects number : UNKNOWN'
      end
      object lblSelectedInstance: TLabel
        Left = 24
        Top = 128
        Width = 91
        Height = 13
        Caption = 'Selected instance :'
      end
      object lblConnState: TLabel
        Left = 439
        Top = 176
        Width = 144
        Height = 13
        Caption = 'Connection state : UNKNOWN'
      end
      object Label1: TLabel
        Left = 24
        Top = 216
        Width = 61
        Height = 13
        Caption = 'Unique Key :'
      end
      object Label2: TLabel
        Left = 135
        Top = 159
        Width = 41
        Height = 10
        Caption = 'Func. result'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 135
        Top = 22
        Width = 41
        Height = 10
        Caption = 'Func. result'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 135
        Top = 63
        Width = 41
        Height = 10
        Caption = 'Func. result'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 337
        Top = 159
        Width = 41
        Height = 10
        Caption = 'Func. result'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 24
        Top = 249
        Width = 47
        Height = 13
        Caption = 'Client IP :'
      end
      object Button9: TButton
        Left = 24
        Top = 32
        Width = 105
        Height = 25
        Caption = 'Initialize DLL'
        TabOrder = 0
        OnClick = Button9Click
      end
      object edInitRes2: TEdit
        Left = 135
        Top = 34
        Width = 50
        Height = 21
        TabOrder = 1
      end
      object Button11: TButton
        Left = 24
        Top = 72
        Width = 105
        Height = 25
        Caption = 'Create instance !'
        TabOrder = 2
        OnClick = Button11Click
      end
      object edCreateInst: TEdit
        Left = 135
        Top = 74
        Width = 50
        Height = 21
        TabOrder = 3
      end
      object Button12: TButton
        Left = 167
        Top = 123
        Width = 33
        Height = 25
        Caption = '0'
        TabOrder = 4
        OnClick = Button12Click
      end
      object Button13: TButton
        Tag = 1
        Left = 206
        Top = 123
        Width = 33
        Height = 25
        Caption = '1'
        TabOrder = 5
        OnClick = Button12Click
      end
      object Button14: TButton
        Tag = 2
        Left = 246
        Top = 123
        Width = 33
        Height = 25
        Caption = '2'
        TabOrder = 6
        OnClick = Button12Click
      end
      object Button15: TButton
        Tag = 3
        Left = 286
        Top = 123
        Width = 33
        Height = 25
        Caption = '3'
        TabOrder = 7
        OnClick = Button12Click
      end
      object Button16: TButton
        Tag = 4
        Left = 325
        Top = 123
        Width = 33
        Height = 25
        Caption = '4'
        TabOrder = 8
        OnClick = Button12Click
      end
      object Button17: TButton
        Tag = 5
        Left = 365
        Top = 123
        Width = 33
        Height = 25
        Caption = '5'
        TabOrder = 9
        OnClick = Button12Click
      end
      object Button18: TButton
        Tag = 6
        Left = 405
        Top = 123
        Width = 33
        Height = 25
        Caption = '6'
        TabOrder = 10
        OnClick = Button12Click
      end
      object Button19: TButton
        Tag = 7
        Left = 444
        Top = 123
        Width = 33
        Height = 25
        Caption = '7'
        TabOrder = 11
        OnClick = Button12Click
      end
      object Button20: TButton
        Tag = 8
        Left = 484
        Top = 123
        Width = 33
        Height = 25
        Caption = '8'
        TabOrder = 12
        OnClick = Button12Click
      end
      object Button21: TButton
        Tag = 9
        Left = 524
        Top = 123
        Width = 33
        Height = 25
        Cancel = True
        Caption = '9'
        TabOrder = 13
        OnClick = Button12Click
      end
      object Button22: TButton
        Left = 24
        Top = 171
        Width = 105
        Height = 25
        Caption = 'Connect !'
        TabOrder = 14
        OnClick = Button22Click
      end
      object edConnRes: TEdit
        Left = 135
        Top = 173
        Width = 50
        Height = 21
        TabOrder = 15
      end
      object edUnKey: TEdit
        Left = 135
        Top = 213
        Width = 466
        Height = 21
        TabOrder = 16
      end
      object Button23: TButton
        Left = 226
        Top = 171
        Width = 105
        Height = 25
        Caption = 'Disconnect !'
        TabOrder = 17
        OnClick = Button23Click
      end
      object edDisconnRes: TEdit
        Left = 337
        Top = 173
        Width = 50
        Height = 21
        MaxLength = 30
        TabOrder = 18
      end
      object GroupBox1: TGroupBox
        Left = 3
        Top = 273
        Width = 598
        Height = 104
        Caption = 'Send functions'
        TabOrder = 19
        object Label6: TLabel
          Left = 141
          Top = 21
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 360
          Top = 21
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 141
          Top = 61
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Button24: TButton
          Left = 16
          Top = 32
          Width = 113
          Height = 25
          Caption = 'Set Mess Params'
          TabOrder = 0
          OnClick = Button24Click
        end
        object edSetMess: TEdit
          Left = 141
          Top = 34
          Width = 50
          Height = 21
          TabOrder = 1
        end
        object Button26: TButton
          Left = 238
          Top = 32
          Width = 113
          Height = 25
          Caption = 'Set Mess Field'
          TabOrder = 2
          OnClick = Button26Click
        end
        object edSetMessageField2: TEdit
          Left = 360
          Top = 34
          Width = 50
          Height = 21
          TabOrder = 3
        end
        object Button27: TButton
          Left = 16
          Top = 72
          Width = 113
          Height = 25
          Caption = 'Send Message'
          TabOrder = 4
          OnClick = Button27Click
        end
        object edSendMessage2: TEdit
          Left = 141
          Top = 74
          Width = 50
          Height = 21
          TabOrder = 5
        end
      end
      object GroupBox2: TGroupBox
        Left = 3
        Top = 383
        Width = 598
        Height = 258
        Caption = 'Receive functions'
        TabOrder = 20
        object Label7: TLabel
          Left = 138
          Top = 19
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 404
          Top = 19
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 138
          Top = 59
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label13: TLabel
          Left = 404
          Top = 59
          Width = 41
          Height = 10
          Caption = 'Func. result'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Button25: TButton
          Left = 19
          Top = 32
          Width = 113
          Height = 25
          Caption = 'Add Event Dest.'
          TabOrder = 0
          OnClick = Button25Click
        end
        object edSMAddDest: TEdit
          Left = 138
          Top = 35
          Width = 50
          Height = 21
          TabOrder = 1
        end
        object Button28: TButton
          Left = 223
          Top = 33
          Width = 169
          Height = 25
          Caption = 'Set Work Thread CallBack'
          TabOrder = 2
          OnClick = Button28Click
        end
        object edCallBack2: TEdit
          Left = 404
          Top = 35
          Width = 50
          Height = 21
          TabOrder = 3
        end
        object ListBox2: TListBox
          Left = 15
          Top = 112
          Width = 564
          Height = 143
          ItemHeight = 13
          TabOrder = 4
        end
        object Button1: TButton
          Left = 15
          Top = 72
          Width = 113
          Height = 25
          Caption = 'Remove Event Dest.'
          TabOrder = 5
          OnClick = Button1Click
        end
        object edSMRemoveDest: TEdit
          Left = 138
          Top = 75
          Width = 50
          Height = 21
          TabOrder = 6
          OnClick = Button11Click
        end
        object Button2: TButton
          Left = 222
          Top = 72
          Width = 170
          Height = 25
          Caption = 'All Remove Dest.'
          TabOrder = 7
          OnClick = Button2Click
        end
        object edSMAllRemoveDest: TEdit
          Left = 404
          Top = 75
          Width = 50
          Height = 21
          TabOrder = 8
        end
        object Button3: TButton
          Left = 480
          Top = 72
          Width = 75
          Height = 25
          Caption = 'Clear List'
          TabOrder = 9
          OnClick = Button3Click
        end
      end
      object edClientIP: TEdit
        Left = 135
        Top = 246
        Width = 154
        Height = 21
        TabOrder = 21
      end
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 580
    Top = 88
  end
end
