unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ComCtrls, StdCtrls,
  Grids, Buttons, SpkCtrls, SpkContainers, SpkToolbar, spkt_Tab, spkt_Pane,
  spkt_Buttons;

type

  { TForm1 }

  TForm1 = class(TForm)
    Edit1: TEdit;
    ImageList1: TImageList;
    ImageList2: TImageList;
    SpkAnyGridContainer1: TSpkAnyGridContainer;
    SpkBackground1: TSpkBackground;
    SpkCard1: TSpkCard;
    SpkCheckButton1: TSpkCheckButton;
    SpkCombobox1: TSpkCombobox;
    SpkExpander1: TSpkExpander;
    SpkGroupBox1: TSpkGroupBox;
    SpkLabel1: TSpkLabel;
    SpkLabel2: TSpkLabel;
    SpkLabel3: TSpkLabel;
    SpkLabel4: TSpkLabel;
    SpkLabel5: TSpkLabel;
    SpkLabel6: TSpkLabel;
    SpkLargeButton1: TSpkLargeButton;
    SpkLargePushButton1: TSpkLargePushButton;
    SpkListbox1: TSpkListbox;
    SpkPane1: TSpkPane;
    SpkPanel1: TSpkPanel;
    SpkProgressbar1: TSpkProgressbar;
    SpkProgressbar2: TSpkProgressbar;
    SpkPushButton1: TSpkPushButton;
    SpkPushButton2: TSpkPushButton;
    SpkPushButton3: TSpkPushButton;
    SpkRadioCheck1: TSpkRadioCheck;
    SpkRadioCheck2: TSpkRadioCheck;
    SpkRadioGroup1: TSpkRadioGroup;
    SpkSplitter1: TSpkSplitter;
    SpkStatusbar1: TSpkStatusbar;
    SpkSwitch1: TSpkSwitch;
    SpkTab1: TSpkTab;
    SpkToolbar1: TSpkToolbar;
    SpkTrackbar1: TSpkTrackbar;
    SpkTreeViewContainer1: TSpkTreeViewContainer;
    SpkUpDown1: TSpkUpDown;
    StringGrid1: TStringGrid;
    TreeView1: TTreeView;
    procedure SpkCombobox1UserAdd(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.SpkCombobox1UserAdd(Sender: TObject);
begin
  ShowMessage('Test add click...');
end;


end.

