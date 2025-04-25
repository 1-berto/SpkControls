(*
    SpkCtrls - Themed Controls.
    Copyright (c) 2025 Humberto Teófilo, All Rights Reserved.
    Licensed under Modified LGPL.

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
unit SpkContainers;

{$MODE OBJFPC}
{$H+}

interface
uses
  Classes, SysUtils, Controls, Forms, Graphics, Grids, DBGrids, StdCtrls, ExtCtrls, Comctrls, ImgList,
  Types, LCLType,
  SpkToolbar,
  SpkCtrls;

type

  TCustomSpkExtension =  class(TCommonSpkContainer)
  protected
    procedure   Loaded(); override;
    procedure   SetBorder(AValue: TSpkStyledBorder); virtual;
    procedure   SetToolbar(const AValue: TSpkToolbar); override;
    procedure   ApplyOptions(const AWrapped: TControl); virtual; abstract;
    procedure   ApplyAppearance(const AWrapped: TControl); virtual; abstract;
    procedure   DoDraw(const ABuffer: TBitmap); override;
  private
    FTarget     : TControl;
    FBorder     : TSpkStyledBorder;
  public
    constructor Create(AOwner: TComponent); override;
    property    Target: TControl read FTarget;
  published
    property    Border: TSpkStyledBorder read FBorder write SetBorder;
  end;

  TSpkAnyGridContainer = class(TCustomSpkExtension)
  private
  protected
    procedure   ApplyOptions(const AWrapped: TControl); override;
    procedure   ApplyAppearance(const AWrapped: TControl); override;
  public
    procedure   InsertControl(AControl: TControl; Index: integer); override;
  published
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

  TSpkTreeViewContainer = class(TCustomSpkExtension)
  protected
    procedure   ApplyOptions(const AWrapped: TControl); override;
    procedure   ApplyAppearance(const AWrapped: TControl); override;
  public
    procedure   InsertControl(AControl: TControl; Index: integer); override;
  published
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

implementation
uses
  LCLStrConsts, Interfaces, Dialogs, LResources,
  spkt_Const, spkt_Appearance, SpkGraphTools, SpkMath;

(*
    TCustomSpkExtension
*)

(*
    PROTECTED
*)
procedure TCustomSpkExtension.Loaded();
begin
  inherited Loaded();

  if (FTarget <> nil) and (Self.SpkToolbar <> nil) then
    begin
      ApplyOptions(FTarget);
      ApplyAppearance(FTarget);
    end;
end;

procedure TCustomSpkExtension.SetBorder(AValue: TSpkStyledBorder);
begin
  FBorder := AValue;

  if not (csloading in Self.ComponentState) then
    Self.Repaint();
end;

procedure TCustomSpkExtension.SetToolbar(const AValue: TSpkToolbar);
begin
  inherited SetToolbar(AValue);
  if (Self.SpkToolbar = nil) then
    Exit;

  if (csDesigning in Self.ComponentState) then
    case Self.SpkToolbar.Appearance.Pane.Style of
      psDividerEtched, psDividerFlat, psDividerRaised: SetBorder(sbNone);
      psRectangleEtched: SetBorder(sbEtched);
      psRectangleFlat: SetBorder(sbFlat);
      psRectangleRaised: SetBorder(sbRaised);
    end;
end;

procedure TCustomSpkExtension.DoDraw(const ABuffer: TBitmap);
begin
  if (FBorder <> sbNone) then
    with SpkToolbar do
      TSpkCtrlsAux.DrawStyledBorder(Self.InternalRect, ABuffer, FBorder, PaneCornerRadius,
                                    Appearance.Pane.BorderDarkColor, Appearance.Pane.BorderLightColor);
end;

(*
    PUBLIC
*)
constructor TCustomSpkExtension.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FTarget       := nil;
  FCompStyle    := csPanel;
  ControlStyle  := ControlStyle + [csAcceptsControls, csCaptureMouse, csClickEvents, csDoubleClicks, csReplicatable,
                                   csNoFocus, csAutoSize0x0, csParentBackground] - [csOpaque];
  SetInitialBounds(0, 0, 300, 200);
  AccessibleRole := larGroup;
  AccessibleDescription := rsTPanelAccessibilityDescription;
//  BorderWidth := PaneBorderSize;
end;

(*
    TSpkAnyGridContainer
*)

(*
    PUBLIC
*)
procedure TSpkAnyGridContainer.InsertControl(AControl: TControl; Index: integer);
begin
  inherited InsertControl(AControl, Index);
  if (AControl is TDrawGrid) or (AControl is TDBGrid) or (AControl is TStringGrid) then
    begin
      FTarget := AControl;
      FTarget.Align := alCLient;
      FTarget.BorderSpacing.Around := 1;
    end
  else
    if (csDesigning in ComponentState) then
      ShowMessage('Design-time: The container require TStringGrid, TDrawGrid or TDBGrid class descendent.');
end;

(*
    PROTECTED
*)
procedure TSpkAnyGridContainer.ApplyOptions(const AWrapped: TControl);
begin
  if (AWrapped is TStringGrid) then
    with TStringGrid(AWrapped) do
      begin
        Flat              := True;
        BorderStyle       := bsNone;
        Scrollbars        := ssAutoBoth;
        ParentFont        := False;
        ParentColor       := False;
        ParentShowHint    := False;
        DefaultColWidth   := 3 * PaneRowHeight;
        DefaultRowHeight  := PaneRowHeight;
        TitleStyle        := tsStandard;
        EditorBorderStyle := bsNone;
        Options           := Options + [goDrawFocusSelected];
      end;

  if (AWrapped is TDrawGrid) then
    with TDrawGrid(AWrapped) do
      begin
        Flat              := True;
        BorderStyle       := bsNone;
        Scrollbars        := ssAutoBoth;
        ParentFont        := False;
        ParentColor       := False;
        ParentShowHint    := False;
        DefaultColWidth   := 3 * PaneRowHeight;
        DefaultRowHeight  := PaneRowHeight;
        TitleStyle        := tsStandard;
        EditorBorderStyle := bsNone;
        Options           := Options + [goDrawFocusSelected];
      end;

  if (AWrapped is TDBGrid) then
    with TDBGrid(AWrapped) do
      begin
        Flat              := True;
        BorderStyle       := bsNone;
        Scrollbars        := ssAutoBoth;
        ParentFont        := False;
        ParentColor       := False;
        ParentShowHint    := False;
        DefaultRowHeight  := PaneRowHeight;
        TitleStyle        := tsStandard;
        EditorBorderStyle := bsNone;
      end;
end;

procedure TSpkAnyGridContainer.ApplyAppearance(const AWrapped: TControl);
begin
  if (AWrapped is TStringGrid) then
    with TStringGrid(AWrapped) do
      begin
        SelectedColor      := SpkToolbar.Appearance.Element.IdleFrameColor;
        TitleFont          := SpkToolbar.Appearance.Pane.CaptionFont;
        FixedColor         := SpkToolbar.Appearance.Pane.CaptionBgColor;
        GridLineColor      := SpkToolbar.Appearance.Pane.BorderDarkColor;
        Font               := SpkToolbar.Appearance.Element.CaptionFont;
        DisabledFontColor  := TColorTools.ColorToGrayscale(SpkToolbar.Appearance.Element.CaptionFont.Color);
        Color              := SpkToolbar.Appearance.Element.IdleInnerLightColor;
        AlternateColor     := SpkToolbar.Appearance.Element.IdleInnerDarkColor;
        FocusColor         := SpkToolbar.Appearance.Element.ActiveCaptionColor;
        ColRowDragIndicatorColor := SpkToolbar.Appearance.Element.HotTrackCaptionColor;
      end;

  if (AWrapped is TDrawGrid) then
    with TDrawGrid(AWrapped) do
      begin
        SelectedColor      := SpkToolbar.Appearance.Element.IdleFrameColor;
        TitleFont          := SpkToolbar.Appearance.Pane.CaptionFont;
        FixedColor         := SpkToolbar.Appearance.Pane.CaptionBgColor;
        GridLineColor      := SpkToolbar.Appearance.Pane.BorderDarkColor;
        Font               := SpkToolbar.Appearance.Element.CaptionFont;
        DisabledFontColor  := TColorTools.ColorToGrayscale(SpkToolbar.Appearance.Element.CaptionFont.Color);
        Color              := SpkToolbar.Appearance.Element.IdleInnerLightColor;
        AlternateColor     := SpkToolbar.Appearance.Element.IdleInnerDarkColor;
        FocusColor         := SpkToolbar.Appearance.Element.ActiveCaptionColor;
        ColRowDragIndicatorColor := SpkToolbar.Appearance.Element.HotTrackCaptionColor;
      end;

  if (AWrapped is TDBGrid) then
    with TDBGrid(AWrapped) do
      begin
        SelectedColor      := SpkToolbar.Appearance.Element.IdleFrameColor;
        TitleFont          := SpkToolbar.Appearance.Pane.CaptionFont;
        FixedColor         := SpkToolbar.Appearance.Pane.CaptionBgColor;
        GridLineColor      := SpkToolbar.Appearance.Pane.BorderDarkColor;
        Font               := SpkToolbar.Appearance.Element.CaptionFont;
        Color              := SpkToolbar.Appearance.Element.IdleInnerLightColor;
        AlternateColor     := SpkToolbar.Appearance.Element.IdleInnerDarkColor;
        FocusColor         := SpkToolbar.Appearance.Element.ActiveCaptionColor;
        ColRowDragIndicatorColor := SpkToolbar.Appearance.Element.HotTrackCaptionColor;
      end;
end;

(*
    TSpkTreeViewContainer
*)

(*
    PUBLIC
*)
procedure TSpkTreeViewContainer.InsertControl(AControl: TControl; Index: integer);
begin
  inherited InsertControl(AControl, Index);
  if (AControl is TTreeView) then
    begin
      FTarget := AControl;
      FTarget.Align := alCLient;
      FTarget.BorderSpacing.Around := 1;
    end
  else
    if (csDesigning in ComponentState) then
      ShowMessage('Design-time: The container require a TTreeView class descendent.');
end;

(*
    PROTECTED
*)

procedure TSpkTreeViewContainer.ApplyOptions(const AWrapped: TControl);
begin
  with TTreeView(AWrapped) do
    begin
      BorderStyle       := bsNone;
      Scrollbars        := ssNone;
      ParentFont        := False;
      ParentColor       := False;
      ParentShowHint    := False;
      Options           := [tvoHideSelection,tvoKeepCollapsedNodes,tvoRowSelect,tvoShowButtons,tvoShowRoot,tvoToolTips];
      TreeLinePenStyle  := psClear;
      DefaultItemHeight := PaneRowHeight;
      ExpandSignType    := tvestArrowFill;
      ExpandSignSize    := DropdownArrowWidth;
    end;
end;

procedure TSpkTreeViewContainer.ApplyAppearance(const AWrapped: TControl);
begin
  with TTreeView(AWrapped) do
    begin
      SelectionFontColorUsed := False;
      Font := SpkToolbar.Appearance.Element.CaptionFont;
      { NOTE: The Font.Color is ONLY used when option [tvoThemedDraw] is not included. }
      Font.Color         := SpkToolbar.Appearance.Element.IdleCaptionColor;
      DisabledFontColor  := TColorTools.ColorToGrayscale(SpkToolbar.Appearance.Element.IdleCaptionColor);
      BackgroundColor    := SpkToolbar.Appearance.Element.IdleGradientFromColor;
      SeparatorColor     := SpkToolbar.Appearance.Element.IdleGradientToColor;
      ExpandSignColor    := SpkToolbar.Appearance.Element.IdleCaptionColor;
      SelectionColor     := SpkToolbar.Appearance.Element.IdleFrameColor;
      SelectionFontColor := SpkToolbar.Appearance.Element.IdleCaptionColor;
      HotTrackColor      := SpkToolbar.Appearance.Element.HotTrackCaptionColor;
    end;
end;

end.     
