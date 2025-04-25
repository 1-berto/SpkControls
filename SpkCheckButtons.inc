{%MainUnit SpkCtrls.pp}

(*
    TSpkCheckButton
*)

(*
    PROTECTED
*)

procedure TSpkCheckButton.SetupIntf();
begin
  FCheckStyle    := cbsCheckbox;
  AccessibleRole := larCheckBox;

  Constraints.SetInterfaceConstraints(3 * LargeButtonRadius, PaneRowHeight, 0, 0);
  SetBounds(Left, Top, 60, PaneRowHeight);
end;

procedure TSpkCheckButton.DispatchUserClick(Shift: TShiftState; X, Y: Integer);
begin
  SetChecked( FOnUserCheck( GetChecked() ) );

  inherited DispatchUserClick(Shift, X, Y);
end;

function TSpkCheckButton.AllowChangeState(): Boolean;
begin
  Result := True;
end;

procedure TSpkCheckButton.SetShowBorder(AValue: Boolean);
begin
  FShowBorder := AValue;
  if not (csLoading in Self.ComponentState) then
      UpdateAppearance();
end;

function TSpkCheckButton.GetChecked(): Boolean;
begin
  Result := (FCheckState = cbChecked);
end;

procedure TSpkCheckButton.SetCheckState(AValue: TCheckBoxState);
begin
  if AllowChangeState()  then
    begin
      FCheckState := AValue;
      if Assigned(FOnChanged) then
        FOnChanged(Self);

      InvalidateBuffer();
      Repaint();
    end;
end;

procedure TSpkCheckButton.SetChecked(const AValue: Boolean);
begin
  if AValue then
    SetCheckState(cbChecked)
  else
    SetCheckState(cbUnChecked);
end;

function TSpkCheckButton.CalcRect(): T2DIntRect;
var
  BtnWidth: integer;
  Bitmap: TBitmap;
  TextWidth: Integer;
  BtnRect: T2DIntRect;
begin
 {$IFDEF EnhancedRecordSupport}
  BtnRect := T2DIntRect.Create(0, 0, 0, 0);
 {$ELSE}
  BtnRect.Create(0, 0, 0, 0);
 {$ENDIF}

 if (FToolbar = nil) then
   Exit;

  Bitmap := FToolbar.GetTempBitmap;

  if not Assigned(Bitmap) then
    Exit;

  Bitmap.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
  TextWidth := Bitmap.Canvas.TextWidth(Self.Caption);

  BtnWidth := SmallButtonPadding + SmallButtonGlyphWidth +
    SmallButtonPadding + TextWidth + SmallButtonPadding;
  BtnWidth := Max(SmallButtonMinWidth, BtnWidth);

  BtnWidth := BtnWidth + (2 * SmallButtonBorderWidth);

 {$IFDEF EnhancedRecordSupport}
  BtnRect := T2DIntRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
 {$ELSE}
  BtnRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
 {$ENDIF}

  Result := BtnRect;
end;

procedure TSpkCheckButton.DoDraw(const ABuffer: TBitmap);
//
//  NOTE: This draw is the same code of original one, must be the same for same results.
//        Keep it up to date with original code to get the same control appearance.
//
var
  ClipRect: T2DIntRect;
  fontColor: TColor;
  x, y: Integer;
  h: Integer;
  te: TThemedElementDetails;
  cornerRadius: Integer;
begin
  if (FToolbar = nil) then
    Exit;

  ClipRect := FRect;

  with FToolbar do
  begin
    case Appearance.Element.Style of
      esRounded:
        cornerRadius := SmallButtonRadius;
      esRectangle:
        cornerRadius := 0;
    end;

  if FocusOpt and (Screen.ActiveControl = Self) then
    TSpkCtrlsAux.DrawControlFocus(Self, ABuffer, FToolbar.Appearance.Element.IdleFrameColor);

    if (FButtonState = bsIdle) and FShowBorder then
    begin
      with Appearance.Element do
        TButtonTools.DrawButton(
          ABuffer,
          FRect,
          IdleFrameColor,
          IdleInnerLightColor,
          IdleInnerDarkColor,
          IdleGradientFromColor,
          IdleGradientToColor,
          IdleGradientType,
          False,
          False,
          False,
          False,
          cornerRadius,
          ClipRect
        );
    end else
    if (FButtonState = bsBtnHottrack) then
    begin
      with Appearance.Element do
        TButtonTools.DrawButton(
          ABuffer,
          FRect,
          HotTrackFrameColor,
          HotTrackInnerLightColor,
          HotTrackInnerDarkColor,
          HotTrackGradientFromColor,
          HotTrackGradientToColor,
          HotTrackGradientType,
          False,
          False,
          False,
          False,
          cornerRadius,
          ClipRect
        );
    end else
    if (FButtonState = bsBtnPressed) then
    begin
      with Appearance.Element do
        TButtonTools.DrawButton(
          ABuffer,
          FRect,
          ActiveFrameColor,
          ActiveInnerLightColor,
          ActiveInnerDarkColor,
          ActiveGradientFromColor,
          ActiveGradientToColor,
          ActiveGradientType,
          False,
          False,
          False,
          False,
          cornerRadius,
          ClipRect
        );
    end;
  end;

  // Checkbox
  if ThemeServices.ThemesEnabled then
    begin
      te := ThemeServices.GetElementDetails(tbCheckboxCheckedNormal);
      h  := ThemeServices.GetDetailSize(te).cy;
    end
  else
    h := GetSystemMetrics(SM_CYMENUCHECK);

  x := FRect.Left + SmallButtonBorderWidth + SmallButtonPadding;
  y := FRect.Top + (FRect.Height - h) div 2;

  TGUITools.DrawCheckbox(
    ABuffer.Canvas,
    x, y,
    FCheckState,
    FButtonState,
    FCheckStyle,
    ClipRect
  );

  // Text
  with FToolbar do
    begin
      ABuffer.Canvas.Font.Assign(Appearance.Element.CaptionFont);

      case FButtonState of
        bsIdle             : fontColor := Appearance.Element.IdleCaptionColor;
        bsBtnHottrack,
        bsDropdownHottrack : fontColor := Appearance.Element.HotTrackCaptionColor;
        bsBtnPressed,
        bsDropdownPressed  : fontColor := Appearance.ELement.ActiveCaptionColor;
      end;

      if not Self.Enabled then
        fontColor := TColorTools.ColorToGrayscale(fontColor);
    end;


  x := FRect.Left + SmallButtonBorderWidth;
  x := x + 2 * SmallButtonPadding + SmallButtonGlyphWidth;
  y := FRect.Top + (FRect.Height - ABuffer.Canvas.TextHeight('Wy')) div 2;

  TGUITools.DrawText(ABuffer.Canvas, x, y, Self.Caption, fontColor, ClipRect);
end;

procedure TSpkCheckButton.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if (Key = SPK_ACTIVATE_KEY) or (Key = SPK_SELECT_KEY) then
    Self.Checked := not Self.Checked;
end;

(*
    PUBLIC
*)
constructor TSpkCheckButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  FCheckState  := cbUnchecked;
  FOnChanged   := nil;
  FOnUserClick := nil;
  FShowBorder  := False;
  FButtonState := bsIdle;
  FOnUserCheck := @DefaultUserCheckSwap;

  SetupIntf();
end;

(*
    PRIVATE
*)
function TSpkCheckButton.DefaultUserCheckSwap(const AChecked: Boolean): Boolean;
begin
  Result := not AChecked;
end;

(*
    TCustomSpkRadioCheck
*)

(*
    PROTECTED
*)
procedure TCustomSpkRadioCheck.SetupIntf();
begin
  FGroupIndex    := 0;
  FCheckStyle    := cbsRadioButton;
  AccessibleRole := larRadioButton;

  Constraints.SetInterfaceConstraints(3 * LargeButtonRadius, PaneRowHeight, 0, 0);
  SetBounds(Left, Top, 60, PaneRowHeight);
end;

procedure TCustomSpkRadioCheck.SetCheckState(AValue: TCheckboxState);
begin
  inherited SetCheckState(AValue);

  if (Self.FCheckState = cbChecked) then
    Self.UnckeckGroup(Self.GroupIndex);
end;

(*
    PUBLIC
*)
procedure TCustomSpkRadioCheck.UnckeckGroup(AGroupIndex: Integer);
var
  Loop : Integer;
  Ctrl : TCommonSpkContainer;
  Radio: TCustomSpkRadioCheck;
begin
  if (Self.Parent is TCommonSpkContainer) then
    begin
      Ctrl := TCommonSpkContainer(Self.Parent);

      for Loop := 0 to Ctrl.ControlCount -1 do
        if (Ctrl.Controls[Loop] is TCustomSpkRadioCheck) then
          begin
            Radio := TCustomSpkRadioCheck(Ctrl.Controls[Loop]);
            if (Radio <> self) and (Radio.GroupIndex = AGroupIndex) and (Radio.FCheckState = cbChecked) then
              Radio.Checked := False;
          end;
    end;
end;

