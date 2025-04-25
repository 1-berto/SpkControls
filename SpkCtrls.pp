(*
    SpkCtrls - Themed Controls.
    Copyright (c) 2025 Humberto Teófilo, All Rights Reserved.
    Licensed under Modified LGPL.

    CREDITS

    Some draw code are simply copied from the depending library, to high fidelity of visual aspects.
    The credit of this part of code is the third party authors.

    * All files included by code definition are subject to the same copyright terms.

    TERMS

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version with the following modification:

    As a special exception, the copyright holders of this library give you
    permission to link this library with independent modules to produce an
    executable, regardless of the license terms of these independent modules,and
    to copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the terms
    and conditions of the license of that module. An independent module is a
    module which is not derived from or based on this library. If you modify
    this library, you may extend this exception to your version of the library,
    but you are not obligated to do so. If you do not wish to do so, delete this
    exception statement from your version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public License
    for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*)
unit SpkCtrls;

{$MODE OBJFPC}
{$H+}

interface
uses
  Classes, SysUtils, Controls, Forms, Graphics, StdCtrls, ExtCtrls, Comctrls, ImgList, Types, FPCanvas,
  SpkToolbar, SpkMath, SpkGUITools, SpkGraphTools;

const
  SPKCTRLS_TRANSPARENT_COLOR: TColor = clWhite;
  SPKCTRLS_LARGEBTN_WIDTH    = 32;
  SPKCTRLS_SMALLBTN_WIDTH    = 16;
  SPKCTRLS_TRACK_SMALLCHANGE = 10;

type
  (*
      ISpkControl

      The ISpkControl is a common interface for any SpkCtrl, it forces the method to vinculate the
      Toolbar and to have  redraw. Note the redraw method is only used for redraw controls that keep the
      same geometry. For resized controls must recalculate all(See UpdateAppearance() for complete).
  *)
  ISpkControl = interface['{8AD3B49E-D562-48D8-9800-45911ADEE9FE}']
    procedure   SetToolbar(const AValue: TSpkToolbar);
    procedure   ReDraw();
  end;

  TCustomSpkControl = class(TCustomControl,ISpkControl)
  protected
    procedure   SetToolbar(const AValue: TSpkToolbar); virtual;
    procedure   DoEnter; override;
    procedure   DoExit; override;

    function    CalcRect(): T2DIntRect; virtual;
    procedure   InvalidateBuffer(); virtual;

    procedure   SetEnabled(Value: Boolean); override;
    procedure   RealSetText(const AValue: TCaption); override;
    procedure   DoOnResize; override;
    procedure   Paint(); override;

    procedure   DoDraw(const ABuffer: TBitmap); virtual; abstract;
  private
    FToolbar    : TSpkToolbar;
    FBuffer     : TBitmap;
    FRect       : T2DIntRect;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy(); override;
    procedure   ReDraw(); virtual;
    procedure   UpdateAppearance(); virtual;
    property    InternalRect: T2DIntRect read FRect;
  published
    property    SpkToolbar: TSpkToolbar read FToolbar write SetToolbar;
  end;

  TCommonSpkContainer = class(TCustomSpkControl)
  public
    procedure   InsertControl(AControl: TControl; Index: integer); override;
  end;

  TCustomSpkPanel = class(TCommonSpkContainer)
  protected
    procedure   SetToolbar(const AValue: TSpkToolbar); override;
    procedure   DoOnResize; override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure   ReDraw(); override;
  end;

  TSpkPanel = class(TCustomSpkPanel)
  protected
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetInset(Value: Boolean); virtual;
  private
    FInset      : Boolean;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property    Inset: Boolean read FInset write SetInset default False;
    { TControl }
    property    Align;
    property    Autosize;
    property    Anchors;
    property    BorderSpacing;
    property    Constraints;
    property    ChildSizing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TCustomSpkBackground = class(TCustomSpkPanel)
  protected
    procedure   DoDraw(const ABuffer: TBitmap); override;
  public
  end;

  TSpkBackground = class(TCustomSpkBackground)
  published
    { TControl }
    property    Align;
    property    Autosize;
    property    Anchors;
    property    BorderSpacing;
    property    Constraints;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TCustomSpkDetail = class(TGraphicControl, ISpkControl)
  protected
    function    CalcRect(): T2DIntRect; virtual; abstract;

    procedure   InvalidateBuffer(); virtual;
    procedure   SetToolbar(const AValue: TSpkToolbar); virtual;

    procedure   SetEnabled(Value: Boolean); override;
    procedure   DoOnResize; override;
    procedure   RealSetText(const AValue: TCaption); override;
    procedure   Paint(); override;

    procedure   DoDraw(const ABuffer: TBitmap); virtual; abstract;
  private
    FBuffer     : TBitmap;
    FRect       : T2DIntRect;
    FToolbar    : TSpkToolbar;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy(); override;
    procedure   ReDraw();
    procedure   UpdateAppearance(); virtual;
  published
    property    SpkToolbar: TSpkToolbar read FToolbar write SetToolbar;
  end;

  TCustomSpkButton = class(TCustomSpkControl)
  protected
    procedure   DispatchUserClick({%H-}Shift: TShiftState; {%H-}X, {%H-}Y: Integer); virtual;
  private
    FButtonState: TSpkButtonState;
    FOnUserClick: TNotifyEvent;
  protected
    procedure   MouseLeave(); override;
    procedure   MouseDown(Button: TMouseButton; {%H-}Shift: TShiftState; {%H-}X, {%H-}Y: Integer); override;
    procedure   MouseMove({%H-}Shift: TShiftState; {%H-}X, {%H-}Y: Integer); override;
    procedure   MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
  public
    property    OnUserClick: TNotifyEvent read FOnUserClick write FOnUserClick;
  end;

  TCustomSpkAutoSizeButton = class(TCustomSpkButton)
  protected
    class
    function    GetControlClassDefaultSize(): TSize; override;
    procedure   CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; {%H-}WithThemeSpace: Boolean); override;
    procedure   TextChanged; override;
    procedure   DoSetBounds(ALeft, ATop, AWidth, AHeight: integer); override;
    procedure   Loaded(); override;
  public
    constructor Create(AOwner: TComponent); override;
  end;

  TSpkButtonType     = (btRegular, btButtonDropdown, btDropdown, btToggle, btSeparator);
  TSpkTableBehaviour = (tbBeginsRow, tbBeginsColumn, tbContinuesRow);
  TSpkGroupBehaviour = (gbSingleItem, gbBeginsGroup, gbContinuesGroup, gbEndsGroup);

  TCustomSpkPushButton = class(TCustomSpkAutoSizeButton)
  protected
    procedure    SetImageIndex(AValue: TImageIndex); virtual;
    procedure    SetImgsWidth(AValue: Integer); virtual;
    procedure    SetImages(const AValue: TImageList); virtual;
    procedure    SetDisabledImages(const AValue: TImageList); virtual;
    procedure    SetButtonType(AValue: TSpkButtonType); virtual;
    procedure    SetPushed(AValue: Boolean); virtual;
    procedure    SetToggleMode(AValue: Boolean); virtual;
    procedure    KeyDown(var Key: Word; Shift: TShiftState); override;
  private
    FToggleMode  ,
    FPushed      : Boolean;
    FDropdownRect: T2DIntRect;
    FButtonType  : TSpkButtonType;
    FImageIndex  : TImageIndex;
    FImgsWidth   : Integer;
    FDisImages   ,
    FImages      : TImageList;
    procedure    DrawDropdownArrow(ABuffer: TBitmap; ARect: TRect; AColor: TColor);
  public
    constructor  Create(AOwner: TComponent); override;
    property     ImagesWidth: Integer read FImgsWidth write SetImgsWidth;
    property     ToggleMode: Boolean read FToggleMode write SetToggleMode;
    property     Pushed: Boolean read FPushed write SetPushed;
  published
    property     ImageIndex: TImageIndex read FImageIndex write SetImageIndex default -1;
    property     Images: TImageList read FImages write SetImages;
    property     DisabledImages: TImageList read FDisImages write SetDisabledImages;
  end;

  TSpkLargePushButton = class(TCustomSpkPushButton)
  protected
    class
    function    GetControlClassDefaultSize(): TSize; override;

    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
  public
    constructor Create(AOwner: TComponent); override;
  private
    procedure   FindBreakPlace(s: string; out Position: integer; out AWidth: integer);
  published
    property    ToggleMode default False;
    property    Pushed default False;
    property    ImagesWidth default SPKCTRLS_LARGEBTN_WIDTH;
    property    OnUserClick;
    property    Caption;
    { TControl }
    property    Align;
    property    Anchors;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TCustomSpkSmallPushButton = class(TCustomSpkPushButton)
  protected
    function    RealGetText: TCaption; override;

    procedure   SetGroupBehaviour(const AValue: TSpkGroupBehaviour); virtual;
    procedure   SetUseFrame(const AValue: Boolean); virtual;
    procedure   SetShowCaption(const AValue: Boolean); virtual;
    procedure   SetTableBehaviour(const AValue: TSpkTableBehaviour); virtual;
    function    GetGroupBehaviour(): TSpkGroupBehaviour; virtual;
    function    GetTableBehaviour(): TSpkTableBehaviour; virtual;

    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
  private
    FTableBehaviour: TSpkTableBehaviour;
    FGroupBehaviour: TSPkGroupBehaviour;
    FUseFrame      : Boolean;
    FShowCaption   : Boolean;
    procedure   ConstructRects(out BtnRect, DropRect: T2DIntRect);
  public
    constructor Create(AOwner: TComponent); override;
    function    GeItemtWidth: Integer;
    property    GroupBehaviour: TSpkGroupBehaviour read FGroupBehaviour write SetGroupBehaviour;
    property    UseFrame: Boolean read FUseFrame write SetUseFrame;
    property    ShowCaption: Boolean read FShowCaption write SetShowCaption;
    property    TableBehaviour: TSpkTableBehaviour read FTableBehaviour write SetTableBehaviour;
  end;

  TSpkPushButton = class(TCustomSpkSmallPushButton)
  published
    property    ShowCaption default True;
    property    UseFrame default False;
    property    Pushed default False;
    property    ToggleMode default False;
    property    ImagesWidth default SPKCTRLS_SMALLBTN_WIDTH;
    property    OnUserClick;
    property    Caption;
    { TControl }
    property    Align;
    property    Anchors;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TSpkSegmentButton = class(TSpkPushButton)
  published
    property    GroupBehaviour;
    property    TableBehaviour;
  end;

  TSpkUserCheckSwapEvent = function(const AChecked: Boolean): Boolean of object;

  TSpkCheckButton = class(TCustomSpkAutoSizeButton)
  protected
    procedure   SetupIntf(); virtual;
    procedure   DispatchUserClick(Shift: TShiftState; X, Y: Integer); override;

    function    AllowChangeState(): Boolean; virtual;
    procedure   SetShowBorder(AValue: Boolean); virtual;
    procedure   SetChecked(const AValue: Boolean); virtual;
    function    GetChecked(): Boolean; virtual;
    procedure   SetCheckState(AValue: TCheckBoxState); virtual;

    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   KeyDown(var Key: Word; {%H-}Shift: TShiftState); override;
  private
    FOnUserCheck: TSpkUserCheckSwapEvent;
    FShowBorder : Boolean;
    FCheckState : TCheckboxState;
    FCheckStyle : TSpkCheckboxStyle;
    FOnChanged  : TNotifyEvent;
    function    DefaultUserCheckSwap(const AChecked: Boolean): Boolean;
  public
    constructor Create(AOwner: TComponent); override;
    property    UserCheckSwapEvent: TSpkUserCheckSwapEvent read FOnUserCheck write FOnUserCheck;
  published
    property    ShowBorder: Boolean read FShowBorder write SetShowBorder default False;
    property    Checked: Boolean read GetChecked write SetChecked;
    property    State: TCheckboxState read FCheckState write SetCheckState default cbUnchecked;
    property    OnChanged: TNotifyEvent read FOnChanged write FOnChanged;
    property    OnUserClick;
    { TControl }
    property    AutoSize;
    property    Caption;
    property    Align;
    property    Anchors;
    property    BidiMode;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TCustomSpkRadioCheck = class(TSpkCheckButton)
  protected
    procedure   SetupIntf(); override;
    procedure   SetCheckState(AValue: TCheckBoxState); override;
  private
    FGroupIndex : Integer;
  public
    procedure   UnckeckGroup(AGroupIndex: Integer); virtual;
    property    GroupIndex: Integer read FGroupIndex write FGroupIndex;
  end;

  TSpkRadioCheck = class(TCustomSpkRadioCheck)
  published
    property    GroupIndex default 0;
  end;

  TSpkFontStyle = (fsElement, fsMenu, fsPane, fsTab);
  TSpkTextRole  = (trLabel, trExtra, trDetail, trTitle);

  TCustomSpkLabel = class(TCustomSpkControl)
  protected
    class
    function    GetControlClassDefaultSize(): TSize; override;
    procedure   CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; {%H-}WithThemeSpace: Boolean); override;
    function    RealGetText: TCaption; override;
    procedure   TextChanged; override;
    procedure   DoSetBounds(ALeft, ATop, AWidth, AHeight: integer); override;
    procedure   Loaded(); override;
    procedure   LoadedAll(); override;
    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetAlignment(AValue: TAlignment); virtual;
    procedure   SetLayout(AValue: TTextLayout); virtual;
    procedure   SetFontStyle(AValue: TSpkFontStyle); virtual;
    procedure   SetTextRole(AValue: TSpkTextRole); virtual;
  private
    FTextRect   : T2DIntRect;
    FLPad       ,
    FRPad       : Integer;
    FRole       : TSpkTextRole;
    FAlignment  : TAlignment;
    FLayout     : TTextLayout;
    FFontStyle  : TSpkFontStyle;
    FHighlighted: Boolean;
    procedure   UpdateFontByStyle();
  public
    constructor Create(AOwner: TComponent); override;
    property    Alignment: TAlignment read FAlignment write SetAlignment;
    property    Layout: TTextLayout read FLayout write SetLayout;
    property    FontStyle: TSpkFontStyle read FFontStyle write SetFontStyle;
    property    TextRole: TSpkTextRole read FRole write SetTextRole;
    property    Highlighted: Boolean read FHighlighted write FHighlighted;
  end;

  TSpkLabel = class(TCustomSpkLabel)
  published
    property    Alignment default taLeftJustify;
    property    Layout default tlTop;
    property    FontStyle default fsElement;
    property    TextRole default trLabel;
    { TControl }
    property    AutoSize;
    property    Caption;
    property    Align;
    property    Anchors;
    property    BidiMode;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TSpkOrientation = (orHorizontal, orVertical);

  TCustomSpkTrack = class(TCustomSpkControl)
  protected
    procedure   SetMin(const AValue: Integer); virtual;
    procedure   SetMax(const AValue: Integer); virtual;
    procedure   SetPos(const AValue: Integer); virtual;
    procedure   SetSmallChange(const AValue: Integer); virtual;
    procedure   DoChange(); virtual;
    procedure   SetReversed(AValue: Boolean); virtual;

    procedure   SetupIntf(); virtual; abstract;
    procedure   DoRepos(); virtual; abstract;
  private
    FMin        ,
    FMax        ,
    FPos        ,
    FSmallChange: Integer;
    FTrack      : T2DIntRect;
    FOnChange   : TNotifyEvent;
    FReversed   : Boolean;
  public
    constructor Create(AOwner: TComponent); override;
    property    Min: Integer read FMin write SetMin;
    property    Max: Integer read FMax write SetMax;
    property    Position: Integer read FPos write SetPos;
    property    Reversed: Boolean read FReversed write SetReversed;
    property    SmallChange: Integer read FSmallChange write SetSmallChange;
    property    OnChange: TNotifyEvent read FOnChange write FOnChange;
  end;

  TCustomSpkTrackbar = class(TCustomSpkTrack)
  protected
    function    CalcRect(): T2DIntRect; override;
    procedure   SetOrientation(AValue: TSpkOrientation); virtual;

    procedure   SetupIntf(); override;
    procedure   DoOnResize; override;
    procedure   DoRepos(); override;
    procedure   DoDraw(const ABuffer: TBitmap); override;

    function    AllowChange(): Boolean; virtual;
    procedure   SetPos(const AValue: Integer); override;
  protected
    procedure   KeyDown(var Key: Word; {%H-}Shift: TShiftState); override;
    procedure   MouseLeave(); override;
    procedure   MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   MouseMove(Shift: TShiftState; X, Y: Integer); override;
    procedure   MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    function    DoMouseWheelDown({%H-}Shift: TShiftState; {%H-}MousePos: TPoint): Boolean; override;
    function    DoMouseWheelUp({%H-}Shift: TShiftState; {%H-}MousePos: TPoint): Boolean; override;
  private
    FOnUChange  : TNotifyEvent;
    FTrackLen   : Integer;
    FSlider     : T2DIntRect;
    FSliderState: TSpkButtonState;
    FOrientation: TSpkOrientation;
    procedure   UserSmallChange(AInc: Boolean);
  public
    property    Orientation: TSpkOrientation read FOrientation write SetOrientation;
    property    OnUserChange: TNotifyEvent read FOnUChange write FOnUChange;
  end;

  TSpkTrackbar = class(TCustomSpkTrackbar)
  published
    property    Min;
    property    Max;
    property    Position;
    property    SmallChange;
    property    Orientation;
    property    Reversed default False;
    property    OnChange;
    property    OnUserChange;
    { TControl }
    property    Align;
    property    Anchors;
    property    BidiMode;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnResize;
    property    OnStartDrag;
  end;

  TCustomSpkProgressbar = class(TCustomSpkTrack)
  protected
    procedure   Loaded(); override;
    function    CalcRect(): T2DIntRect; override;

    procedure   SetupIntf(); override;
    procedure   DoOnResize; override;
    procedure   DoRepos(); override;

    procedure   SetPos(const AValue: Integer); override;
  private
    FFiller     : T2DIntRect;
  public
  end;

  TSpkProgressbar = class(TCustomSpkProgressbar)
  protected
    procedure   DoDraw(const ABuffer: TBitmap); override;
  published
    property    Min;
    property    Max;
    property    Position;
    property    SmallChange;
    property    Reversed default False;
    property    OnChange;
    { TControl }
    property    Align;
    property    Anchors;
    property    BidiMode;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnResize;
    property    OnStartDrag;
  end;

  TSpkSwitch = class(TCustomSpkLabel)
  protected
    procedure   CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; WithThemeSpace: Boolean); override;

    function    DoAllowChange(): Boolean; virtual;
    procedure   SetChecked(const AValue: Boolean); virtual;

    procedure   MouseLeave(); override;
    procedure   MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   KeyDown(var Key: Word; {%H-}Shift: TShiftState); override;

    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetToolbar(const AValue: TSpkToolbar); override;
  private
    FSwitcher   ,
    FTrack      : T2DIntRect;
    FChecked    : Boolean;
    FOnUserClick,
    FOnChange   : TNotifyEvent;
    procedure   SetCheckChanges();
    function    GetCalcPad(): Integer;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property    Checked: Boolean read FChecked write SetChecked default False;
    property    OnUserClick: TNotifyEvent read FOnUserClick write FOnUserClick;
    property    OnChange: TNotifyEvent read FOnChange write FOnChange;
    { TControl }
    property    AutoSize;
    property    Caption;
    property    Align;
    property    Anchors;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TSpkStyledBorder = (sbNone, sbEtched, sbFlat, sbRaised);

  TCustomSpkCard =  class(TCustomSpkPanel)
  protected
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetConcave(AValue: Boolean); virtual;
    procedure   SetBorder(AValue: TSpkStyledBorder); virtual;
    procedure   SetToolbar(const AValue: TSpkToolbar); override;
  private
    FConcave    : Boolean;
    FBorder     : TSpkStyledBorder;
  public
    constructor Create(AOwner: TComponent); override;
    property    Concave: Boolean read FConcave write SetConcave;
    property    Border: TSpkStyledBorder read FBorder write SetBorder;
  end;

  TSpkCard = class(TCustomSpkCard)
  published
    property Concave default True;
    property Border default sbEtched;
    { TWinControl }
    property Align;
    property Anchors;
    property AutoSize;
    property BorderSpacing;
    property ChildSizing;
    property ClientHeight;
    property ClientWidth;
    property Constraints;
    property DockSite;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChangeBounds;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDockDrop;
    property OnDockOver;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
    property OnUTF8KeyPress;
  end;

  TCustomSpkGroupBox = class(TCustomSpkPanel)
  protected
    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetBorder(AValue: TSpkStyledBorder); virtual;
    procedure   SetToolbar(const AValue: TSpkToolbar); override;
  private
    FTitleAlign : Boolean;
    FTitlebar   ,
    FTitle      : T2DIntRect;
    FBorder     : TSpkStyledBorder;
  public
    constructor Create(AOwner: TComponent); override;
    property    Border: TSpkStyledBorder read FBorder write SetBorder;
  end;

  TSpkGroupBox = class(TCustomSpkGroupBox)
  published
    property Border;
    { TWinControl }
    property Align;
    property Anchors;
    property AutoSize;
    property BorderSpacing;
    property Caption;
    property ChildSizing;
    property ClientHeight;
    property ClientWidth;
    property Constraints;
    property DockSite;
    property DoubleBuffered;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChangeBounds;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDockDrop;
    property OnDockOver;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
    property OnUTF8KeyPress;
  end;

  TCustomSpkExpander = class(TCustomSpkGroupBox)
  protected
    function    AllowUserChangeExpansion(): Boolean; virtual;
    procedure   MouseMove(Shift: TShiftState; X,Y: Integer); override;
    procedure   MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   MouseLeave; override;
    procedure   MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   DoOnResize; override;
    procedure   KeyDown(var Key: Word; {%H-}Shift: TShiftState); override;

    procedure   SetExpanded(const AValue: Boolean); virtual;
    procedure   DoCollapse();
    procedure   DoExpand();

    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
  private
    FExpandSize : Integer;
    FExpanded   : Boolean;
    FButtonState: TSpkButtonState;
    FButtonRect : T2DIntRect;
    FOnExpanded ,
    FOnCollapsed: TNotifyEvent;
  public
    constructor Create(AOwner: TComponent); override;
    procedure   Collapse(); virtual;
    property    Expanded: Boolean read FExpanded write SetExpanded;
    property    OnExpanded: TNotifyEvent read FOnExpanded write FOnExpanded;
    property    OnCollapsed: TNotifyEvent read FOnCollapsed write FOnCollapsed;
  end;

  TSpkExpander = class(TCustomSpkExpander)
  published
    property Expanded default true;
    property OnExpanded;
    property OnCollapsed;
    property Border;
    { TWinControl }
    property Align;
    property Anchors;
    property AutoSize;
    property BorderSpacing;
    property Caption;
    property ChildSizing;
    property ClientHeight;
    property ClientWidth;
    property Constraints;
    property DockSite;
    property DoubleBuffered;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChangeBounds;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDockDrop;
    property OnDockOver;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
    property OnUTF8KeyPress;
  end;

  TCustomSpkCombobox = class(TCustomSpkButton)
  protected
    procedure     MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure     MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    function      DoMouseWheelDown({%H-}Shift: TShiftState; {%H-}MousePos: TPoint): Boolean; override;
    function      DoMouseWheelUp({%H-}Shift: TShiftState; {%H-}MousePos: TPoint): Boolean; override;

    function      CalcRect(): T2DIntRect; override;
    procedure     DoDraw(const ABuffer: TBitmap); override;

    procedure     SetAllowAdd(AValue: Boolean); virtual;
    procedure     SetItemIndex(AValue: Integer); virtual;
    function      GetItems(): TStrings; virtual;
    procedure     SetItems(const AValue: TStrings); virtual;
    procedure     KeyDown(var Key: Word; {%H-}Shift: TShiftState); override;

    procedure     DoUserAdd(); virtual;
    procedure     DoPopupCtrl(); virtual;
    procedure     DoUpdateItem(const ANewItem: Integer); virtual;
    function      CreateStrings(): TStrings; dynamic;
    function      CreateListItemsControl(APopup: TWinControl): TWinControl; virtual;
    procedure     InitPopupItemIndex(); virtual;
    function      CalcListHeight(var AX, AY: Integer): Integer; virtual;
  strict private
    FListControl  : TWinControl;
    procedure     ListKeyDown(Sender: TObject; var Key: Word; {%H-}Shift: TShiftState);
    procedure     ListMouseMove(Sender: TObject; {%H-}Shift: TShiftState; X, Y: Integer);
    procedure     ListMouseUp(Sender: TObject; {%H-}Button: TMouseButton; {%H-}Shift: TShiftState; {%H-}X, {%H-}Y: Integer);
    procedure     ListDrawItem(Control: TWinControl; Index: Integer; ARect: TRect; State: TOwnerDrawState);
  private
    FMaxListHeight: Integer;
    FItems        : TStrings;
    FPushed       ,
    FAllowAdd     : Boolean;
    FTextRect     ,
    FAddRect      : T2DIntRect;
    FOnUserAdd    ,
    FOnChangeItem : TNotifyEvent;
    FItemIndex    : Integer;
    procedure     DoOnClosedPopup(Sender: TObject);
    procedure     BeginPopup();
    procedure     EndPopup();
  public
    constructor   Create(AOwner: TComponent); override;
    destructor    Destroy(); override;
    function      IsPushed(): Boolean;
    procedure     Assign(Source: TPersistent); override;
    procedure     Clear(); virtual;
    property      MaxListHeight: Integer read FMaxListHeight write FMaxListHeight;
    property      AllowAdd: Boolean read FAllowAdd write SetAllowAdd;
    property      ItemIndex: Integer read FItemIndex write SetItemIndex;
    property      Items: TStrings read GetItems write SetItems;
    property      OnUserAdd: TNotifyEvent read FOnUserAdd write FOnUserAdd;
    property      OnChangeItem: TNotifyEvent read FOnChangeItem write FOnChangeItem;
  end;

  TSpkCombobox = class(TCustomSpkCombobox)
  published
    property    MaxListHeight default 0;
    property    AllowAdd;
    property    ItemIndex default -1;
    property    Items;
    property    OnUserClick;
    property    OnUserAdd;
    property    OnChangeItem;
    { TControl }
    property    Caption;
    property    Align;
    property    Anchors;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelDown;
    property    OnMouseWheelUp;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnResize;
    property    OnStartDrag;
  end;

  TSpkSideAlign = (saLeft, saRight, saNone);

  TCustomSpkSideTool = class(TCustomSpkDetail)
  protected
    procedure   MouseEnter; override;
    procedure   MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure   MouseLeave; override;
    procedure   MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;

    procedure   DispatchMouseDown(Shift: TShiftState; X, Y: Integer); virtual; abstract;
    procedure   DispatchMouseUp(Shift: TShiftState; X, Y: Integer); virtual; abstract;

    procedure   SetAssociate(const ACtrl: TControl); virtual;
    procedure   SetSideAlign(AValue: TSpkSideAlign); virtual;
    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;
  private
    FAssociate  : TControl;
    FState      : TSpkButtonState;
    FSideAlign  : TSpkSideAlign;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy(); override;
    property    Associate: TControl read FAssociate write SetAssociate;
  end;

  TCustomSpkUpDown = class(TCustomSpkSideTool)
  protected
    function    DoMouseWheelUp(Shift: TShiftState; MousePos: TPoint): Boolean; override;
    function    DoMouseWheelDown(Shift: TShiftState; MousePos: TPoint): Boolean; override;

    function    AllowUserChange(const {%H-}ANewValue: Single): Boolean; virtual;
    procedure   DispatchMouseDown({%H-}Shift: TShiftState; X, Y: Integer); override;
    procedure   DispatchMouseUp({%H-}Shift: TShiftState; {%H-}X, {%H-}Y: Integer); override;
    procedure   UpButtonTrigger(Sender: TObject); virtual;
    procedure   DownButtonTrigger(Sender: TObject); virtual;

    procedure   SetCurrValue(const AValue: Single); virtual;
    function    GetIntPosition(): Integer; virtual;
    procedure   SetIntPosition(const AValue: Integer); virtual;
    procedure   UpdateAssociateControl(); virtual;

    function    CalcRect(): T2DIntRect; override;
    procedure   DoDraw(const ABuffer: TBitmap); override;

    procedure   Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure   SetAssociate(const ACtrl: TControl); override;
  private
    FThousands   ,
    FUpPressed   ,
    FPressed     : Boolean;
    FOnUserChange,
    FOnAftChange : TNotifyEvent;
    FUpRect      ,
    FDownRect    : T2DIntRect;
    FInterval    : Longint;
    FIncValue    ,
    FMinValue    ,
    FMaxValue    ,
    FCurrValue   : Single;
  public
    constructor  Create(AOwner: TComponent); override;
    property     Thousands: Boolean read FThousands write FThousands;
    property     Increment: Single read FIncValue write FIncValue;
    property     MinRepeatInterval: Longint read FInterval write FInterval;
    property     Min: Single read FMinValue write FMinValue;
    property     Max: Single read FMaxValue write FMaxValue;
    property     Position: Single read FCurrValue write SetCurrValue;
    property     IntPosition: Integer read GetIntPosition write SetIntPosition;
    property     OnUserChange: TNotifyEvent read FOnUserChange write FOnUserChange;
    property     OnAfterChange: TNotifyEvent read FOnAftChange write FOnAftChange;
  end;

  TSpkUpDown = class(TCustomSpkUpDown)
  published
    property     Associate;
    property     Thousands default True;
    property     Increment default 1;
    property     MinRepeatInterval default 100;
    property     Min default 0;
    property     Max default 100;
    property     Position default 0;
    property     IntPosition;
    property     OnUserChange;
    property     OnAfterChange;
    { TControl }
    property    Align;
    property    Anchors;
    property    BorderSpacing;
    property    Enabled;
    property    Visible;
    property    ShowHint;
    property    Hint;
    property    DragCursor;
    property    DragKind;
    property    DragMode;
    property    PopupMenu;
    property    OnChangeBounds;
    property    OnContextPopup;
    property    OnDragDrop;
    property    OnDragOver;
    property    OnEndDrag;
    property    OnMouseWheel;
    property    OnMouseWheelHorz;
    property    OnMouseWheelLeft;
    property    OnMouseWheelRight;
    property    OnStartDrag;
  end;

  TCustomSpkListbox = class(TCustomListBox,ISpkControl)
  protected
    procedure     SetToolbar(const AValue: TSpkToolbar); virtual;
  private
    FCurItemHeight: Integer;
    FToolbar      : TSpkToolbar;
    procedure     SpkDrawItem(Control: TWinControl; Index: Integer; ARect: TRect; State: TOwnerDrawState);
  public
    constructor   Create(AOwner: TComponent); override;
    procedure     ReDraw();
    property      SpkToolbar: TSpkToolbar read FToolbar write SetToolbar;
  end;

  TSpkListbox = class(TCustomSpkListbox)
  published
    property SpkToolbar;
    { TListbox }
    property Align;
    property Anchors;
    property BorderSpacing;
    property ClickOnSelChange;
    property Columns;
    property Constraints;
    property DoubleBuffered;
    property DragCursor;
    property DragKind;
    property DragMode;
    property ExtendedSelect;
    property Enabled;
    property IntegralHeight;
    property Items;
    property ItemHeight;
    property ItemIndex;
    property MultiSelect;
    property OnChangeBounds;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEnter;
    property OnEndDrag;
    property OnExit;
    property OnKeyPress;
    property OnKeyDown;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnMouseWheelHorz;
    property OnMouseWheelLeft;
    property OnMouseWheelRight;
    property OnResize;
    property OnSelectionChange;
    property OnShowHint;
    property OnStartDrag;
    property OnUTF8KeyPress;
    property Options;
    property ParentShowHint;
    property ParentFont;
    property PopupMenu;
    property ScrollWidth;
    property ShowHint;
    property Sorted;
    property TabOrder;
    property TabStop;
    property TopIndex;
    property Visible;
  end;

  TCustomSpkStatusbar = class(TCustomSpkPanel)
  protected
    procedure   DoDraw(const ABuffer: TBitmap); override;
    procedure   SetSimplePanel(const AValue: Boolean); virtual;
    procedure   SetShowGrip(const AValue: Boolean); virtual;
    procedure   RealSetText(const AValue: TCaption); override;
  private
    FShowGrip   ,
    FSimplePanel: Boolean;
    FOnTxtChange: TNotifyEvent;
  public
    constructor Create(AOwner: TComponent); override;
    procedure   InsertControl(AControl: TControl; Index: integer); override;
    property    SimplePanel: Boolean read FSimplePanel write SetSimplePanel;
    property    SimpleText: TCaption read RealGetText write RealSetText;
    property    ShowSizeGrip: Boolean read FShowGrip write SetShowGrip;
    property    OnSimpleTextChange: TNotifyEvent read FOnTxtChange write FOnTxtChange;
  end;

  TSpkStatusbar = class(TCustomSpkStatusbar)
  published
    property SimplePanel default true;
    property SimpleText;
    property ShowSizeGrip default true;
    property OnSimpleTextChange;
    { TWinControl }
    property Align;
    property Anchors;
    property ClientHeight;
    property ClientWidth;
    property Constraints;
    property DockSite;
    property DoubleBuffered;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property UseDockManager default True;
    property Visible;
    property OnClick;
    property OnContextPopup;
    property OnDockDrop;
    property OnDockOver;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnGetDockCaption;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnMouseWheelHorz;
    property OnMouseWheelLeft;
    property OnMouseWheelRight;
    property OnPaint;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;

  TCustomSpkSplitter = class(TCustomSplitter,ISpkControl)
  protected
    procedure     SetToolbar(const AValue: TSpkToolbar); virtual;
  private
    FToolbar      : TSpkToolbar;
    procedure     SpkPaintEvent(Sender: TObject);
  public
    constructor   Create(AOwner: TComponent); override;
    procedure     ReDraw();
    property      SpkToolbar: TSpkToolbar read FToolbar write SetToolbar;
  end;

  TSpkSplitter = class(TCustomSpkSplitter)
  published
    property SpkToolbar;
    { TWinControl }
    property Align;
    property Anchors;
    property AutoSnap;
    property Beveled;
    property Color;
    property Constraints;
    property Cursor;
    property DoubleBuffered;
    property Height;
    property MinSize;
    property OnCanOffset;
    property OnCanResize;
    property OnChangeBounds;
    property OnMoved;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnMouseWheelHorz;
    property OnMouseWheelLeft;
    property OnMouseWheelRight;
    property PopupMenu;
    property ResizeAnchor;
    property ResizeStyle;
    property ShowHint;
    property Visible;
    property Width;
  end;

  TCustomSpkRadioGroup = class(TCustomSpkGroupBox)
  protected
    procedure   Loaded; override;
    function    AllowChangeIndex(): Boolean;
    procedure   NotifyItemIndexChange(); virtual;
    function    NewItemsList(): TStrings; virtual;
    function    NewItem(const ACaption: TCaption; AIdx: Integer): TCustomSpkRadioCheck; virtual;
    procedure   UpdateItems(); virtual;
    procedure   SetItems(const AItems: TStrings); virtual;
    function    GetItemIndex(): Integer; virtual;
    procedure   SetItemIndex(const AValue: Integer); virtual;
    function    GetColumns(): Integer; virtual;
    procedure   SetColumns(const AValue: Integer); virtual;
  private
    FAllowUnsel : Boolean;
    FItemIndex  : Integer;
    FItems      : TStrings;
    FOnUserClick: TNotifyEvent;
    FOnItemEnter: TNotifyEvent;
    FOnItemExit : TNotifyEvent;
    FOnSChanged : TNotifyEvent;
    procedure   DoRadioItemEnter(Sender: TObject);
    procedure   DoRadioItemExit(Sender: TObject);
    procedure   DoRadioItemUserClick(Sender: TObject);
    function    ItemUserCheckSwap(const AChecked: Boolean): Boolean;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy(); override;
    property    AllowUnselectItem: Boolean read FAllowUnsel write FAllowUnsel;
    property    Columns: Integer read GetColumns write SetColumns;
    property    ItemIndex: Integer read GetItemIndex write SetItemIndex;
    property    Items: TStrings read FItems write SetItems;
    property    OnItemUserClick: TNotifyEvent read FOnUserClick write FOnUserClick;
    property    OnItemEnter: TNotifyEvent read FOnItemEnter write FOnItemEnter;
    property    OnItemExit: TNotifyEvent read FOnItemExit write FOnItemExit;
    property    OnSelectionChanged: TNotifyEvent read FOnSChanged write FOnSChanged;
  end;

  TSpkRadioGroup = class(TCustomSpkRadioGroup)
  published
    property Border;
    property AllowUnselectItem default False;
    property Columns default 1;
    property ItemIndex default -1;
    property Items;
    property OnItemUserClick;
    property OnItemEnter;
    property OnItemExit;
    property OnSelectionChanged;
    { TWinControl }
    property Align;
    property Anchors;
    property AutoSize;
    property BorderSpacing;
    property Caption;
    property ChildSizing;
    property ClientHeight;
    property ClientWidth;
    property Constraints;
    property DockSite;
    property DoubleBuffered;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChangeBounds;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDockDrop;
    property OnDockOver;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
    property OnUTF8KeyPress;
  end;

  TSpkPopupWindow = class(TForm)
  protected
    procedure    Deactivate; override;
  private
    FAutoDestroy: Boolean;
    FCaller     : TWinControl;
  public
    constructor  CreateNew(AOwner: TComponent; Num: Integer=0); override;
    procedure    ShowAt(const AX, AY: Integer; AAutoDestroy: Boolean = False); virtual;
    procedure    ShowMe(ADeactiveEvent: TNotifyEvent; AAutoDestroy: Boolean = False); virtual;
    procedure    Finishit(); virtual;
    property     Caller: TWinControl read FCaller write FCaller;
  end;

  TSpkCtrlsAux = class
    class procedure DrawControlFocus(const AControl: TControl; const ABitmap: TBitmap; const AColor: TColor);
    class procedure DrawControlCustomFocus(const {%H-}AControl: TControl; const ABitmap: TBitmap; const ARect: TRect; const AColor: TColor);
    class procedure DrawControlLineFocus(const {%H-}AControl: TControl; const ABitmap: TBitmap; const AX, AY, AWidth: Integer; const AColor: TColor);
    class procedure DrawCrossIcon(ARect: TRect; const ABitmap: TBitmap; const AColor: TColor);
    class procedure DrawStyledBorder(const ARect: T2DIntRect; const ABitmap: TBitmap; ABorder: TSpkStyledBorder;
                                     const ACornerRadius: Integer; const ABorderDarkColor, ABorderLightColor: TColor);
    class procedure DrawStyledChar(const ARect: T2DIntRect; const ABitmap: TBitmap; const AFont: TFont;
                                   const AColor: TColor; const AChr: String);

    class procedure DrawDropdownArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor);
    class procedure DrawUpArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor);
    class procedure DrawUpAndDownArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor; AIsUp: Boolean; const ADisp: Integer = 0);
  end;

  function GetUpDownRepeaterTimer(const ACtrl: TCustomSpkUpDown; AEvent: TNotifyEvent): TTimer;

{$I SpkOptions.inc}

const
  SPK_VERSION     = '0.1.1';
  SPK_PALLETE     = 'SpkControls';
  { Must be the same of TSpkPane.DrawMoreOptionsButton(..MOB_SIGNS[mobsArrow]..). }
  SPK_ARROW_CHAR  = String(#$E2#$87#$B2);
  SPK_SIZE_GRIP   = SPK_ARROW_CHAR;
  SPK_SUBFOCUS_TAB: Word = 9;
  SPK_ENTER_KEY   : Word = 13;
  SPK_ACTIVATE_KEY: Word = 32;
  SPK_SELECT_KEY  : Word = 41;
  RELEASE = 'SpkCtrls ' + SPK_VERSION;

var
  PopupFrm   : TForm  = nil;
  UpDownTimer: TTimer = nil;

implementation
uses
  LCLType, LCLIntf, LCLStrConsts, LCLVersion, Math, Themes, GraphUtil,
  spkt_Buttons, spkt_Tools, spkt_Const, spkt_Appearance;

function GetUpDownRepeaterTimer(const ACtrl: TCustomSpkUpDown; AEvent: TNotifyEvent): TTimer;
begin
  UpDownTimer := TTimer.Create(Application);
  UpDownTimer.Interval := ACtrl.FInterval;
  UpDownTimer.OnTimer  := AEvent;

  Result := UpDownTimer;
end;

function GetButtonKindByType(AType: TSpkButtonType): TSpkButtonKind;
begin
  case AType of
    btRegular        : Result := bkButton;
    btButtonDropdown : Result := bkButtonDropdown;
    btDropdown       : Result := bkDropdown;
    btToggle         : Result := bkToggle;
    btSeparator      : Result := bkSeparator;
  end;
end;

{$I CustomSpkControl.inc}
{$I CustomSpkDetail.inc}
{$I SpkContainers.inc}
{$I SpkPanel.inc}
{$I SpkCheckButtons.inc}
{$I SpkButtons.inc}
{$I SpkLabels.inc}
{$I SpkTracks.inc}
{$I SpkSwitch.inc}
{$I SpkCards.inc}
{$I SpkGroups.inc}
{$I SpkExpander.inc}
{$I SpkCombobox.inc}
{$I SpkSideTool.inc}
{$I SpkUpDown.inc}
{$I SpkListbox.inc}
{$I SpkSplitter.inc}
{$I SpkRadioGroup.inc}
{$I SpkStatusbar.inc}
{$I SpkPopupWindow.inc}
{$I SpkCtrlsAux.inc}

end.

