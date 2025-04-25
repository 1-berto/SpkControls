{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkExpander 
*)

(*
    PROTECTED
*)

function TCustomSpkExpander.AllowUserChangeExpansion(): Boolean;
begin
  Result := True;
end;

procedure TCustomSpkExpander.MouseMove(Shift: TShiftState; X,Y: Integer);
begin
  if Self.FTitlebar.Contains(X, Y) then
    begin
      if FButtonRect.Contains(X, Y) then
        begin
          FButtonState:= bsBtnHottrack;
          UpdateAppearance();
        end
      else
        begin
          FButtonState := bsIdle;
          UpdateAppearance();
        end;
    end
  else
    inherited MouseMove(Shift, X,Y);
end;

procedure TCustomSpkExpander.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) and FButtonRect.Contains(X, Y) then
    begin
      FButtonState:= bsBtnPressed;
      UpdateAppearance();
    end
  else
    inherited MouseDown(Button, Shift, X, Y);
end;

procedure TCustomSpkExpander.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) and FButtonRect.Contains(X, Y) then
    begin
      FButtonState := bsBtnHottrack;
      UpdateAppearance();
      if Self.AllowUserChangeExpansion() then
        SetExpanded( not FExpanded );
    end
  else
    inherited MouseUp(Button, Shift, X, Y);
end;

procedure TCustomSpkExpander.MouseLeave;
begin
  if (FButtonState <> bsIdle) then
    begin
      FButtonState := bsIdle;
      UpdateAppearance();
    end;
end;

procedure TCustomSpkExpander.DoOnResize;
begin
  inherited DoOnResize();

  if FExpanded then
    FExpandSize := Self.Height;
end;

procedure TCustomSpkExpander.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if (Key = SPK_ACTIVATE_KEY) or (Key = SPK_SELECT_KEY) or (Key = SPK_ENTER_KEY) then
    Self.Expanded := not Self.Expanded;
end;

procedure TCustomSpkExpander.SetExpanded(const AValue: Boolean);
begin
  if (AValue = FExpanded) then
    Exit;

  if AValue then
    DoExpand()
  else
    DoCollapse();
end;

procedure TCustomSpkExpander.DoCollapse();
begin
  FExpanded := False;

  Self.Height := FTitle.Bottom + PaneBorderHalfSize;
  if Assigned(FOnCollapsed) then
    FOnCollapsed(Self);

  UpdateAppearance();
end;

procedure TCustomSpkExpander.DoExpand();
begin
  Self.Height := FExpandSize;
  FExpanded := True;

  if Assigned(FOnExpanded) then
    FOnExpanded(Self);

  UpdateAppearance();
end;

function TCustomSpkExpander.CalcRect(): T2DIntRect;
var
  L,H: Integer;
begin
  Result := inherited CalcRect();
  if Self.IsRightToLeft then
    L := PaneBorderSize
  else
    L := Self.Width - ((2 * DropdownArrowWidth) + (2 * PaneBorderSize));
  H := (FTitlebar.Bottom - (2 * DropdownArrowHeight)) div 2;

  {$IFDEF EnhancedRecordSupport}
    FButtonRect := T2DIntRect.Create(
  {$ELSE}
    FButtonRect.Create(
  {$ENDIF}
    L,
    H,
    L + (2 * DropdownArrowWidth),
    H + (2 * DropdownArrowHeight));
end;

procedure TCustomSpkExpander.DoDraw(const ABuffer: TBitmap);
var
  fontColor: TColor;
  frameColor, innerLightColor, innerDarkColor: TColor;
  gradientFromColor, gradientToColor: TColor;
  gradientKind: TBackgroundKind;
  cornerRadius: Integer;
  ClipRect: T2DIntRect;
begin
  inherited DoDraw(ABuffer);

  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  ClipRect := FRect;

  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := SmallButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  if (FButtonState = bsIdle) and ( (FBorder <> sbNone) or (FBorder <> sbFlat) ) then
  begin
    FToolbar.Appearance.Element.GetIdleColors(False,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FButtonState = bsBtnHottrack) then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(False,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FButtonState = bsBtnPressed) then
  begin
    FToolbar.Appearance.Element.GetActiveColors(True,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end;

  if (FButtonState <> bsIdle) then
    TButtonTools.DrawButton(
      ABuffer,
      FButtonRect,
      frameColor,
      innerLightColor,
      innerDarkColor,
      gradientFromColor,
      gradientToColor,
      gradientKind,
      false,
      false,
      false,
      false,
      cornerRadius,
      ClipRect
    )
  else
    begin
      if FocusOpt and (Screen.ActiveControl = Self) then
        TSpkCtrlsAux.DrawControlCustomFocus(Self, ABuffer, FButtonRect.ForWinAPI, fontColor);
    end;

  TSpkCtrlsAux.DrawUpAndDownArrow(ABuffer, FButtonRect.ForWinAPI, fontColor, FExpanded, 2);
end;

(*
    PUBLIC
*)

constructor TCustomSpkExpander.Create(AOwner: TComponent);
begin
  FExpanded := True;
  inherited Create(AOwner);
  FTitleAlign := True;
  FOnExpanded := nil;
  FOnCollapsed:= nil;

  TabStop := True;
end;

procedure TCustomSpkExpander.Collapse();
begin
  if FExpanded then
    DoCollapse();
end;

