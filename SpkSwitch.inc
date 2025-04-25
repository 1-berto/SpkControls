{%MainUnit SpkCtrls.pp}

(*
    TSpkSwitch
*)

(*
    PROTECTED
*)

procedure TSpkSwitch.CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; WithThemeSpace: Boolean);
begin
  inherited CalculatePreferredSize(PreferredWidth, PreferredHeight, WithThemeSpace);
  PreferredHeight := Max(FTextRect.Height, PaneRowHeight);
  PreferredWidth  := FTextRect.Width;
end;

function TSpkSwitch.DoAllowChange(): Boolean;
begin
  Result := True;
end;

procedure TSpkSwitch.SetChecked(const AValue: Boolean);
begin
  FChecked := AValue;

  if (csLoading in ComponentState) then
    Exit;

  UpdateAppearance();

  if Assigned(FOnChange) then
    FOnChange(Self);
end;

procedure TSpkSwitch.MouseLeave();
begin
  inherited MouseLeave();
  UpdateAppearance();
end;

procedure TSpkSwitch.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  inherited MouseDown(Button, Shift, X, Y);

  if (Button <> MouseClick) then
    Exit;

  if FocusOpt and (Screen.ActiveControl <> Self) then
    Self.SetFocus
  else
    Repaint();
end;

procedure TSpkSwitch.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  inherited MouseUp(Button, Shift, X, Y);

  if (Button = MouseClick) then
    begin
      if Self.DoAllowChange() then
        begin
          SetChecked(not FChecked);
          if Assigned(FOnUserClick) then
            FOnUserClick(Self);
        end;
    end;
end;

procedure TSpkSwitch.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if (Key = SPK_ACTIVATE_KEY) or (Key = SPK_SELECT_KEY) then
    Self.Checked := not Self.Checked;
end;

procedure TSpkSwitch.DoDraw(const ABuffer: TBitmap);
var
  FontColor, FrameColor,
  GradientFromColor, GradientToColor,
  InnerLightColor, InnerDarkColor: TColor;
  GradientKind: TBackgroundKind;
  ClipRect : T2DIntRect;
  Radius: Integer;
begin
  if (FToolbar = nil) then
    Exit;

  inherited DoDraw(ABuffer);
  SetCheckChanges();

  if (FToolbar.Appearance.Element.Style = esRounded) then
    Radius := SmallButtonRadius
  else
    Radius := 0;

  if FocusOpt and (Self = Screen.ActiveControl) then
    TSpkCtrlsAux.DrawControlFocus(Self, ABuffer, FToolbar.Appearance.Element.IdleFrameColor);

  ClipRect := FRect;

  with FToolbar.Appearance.Element do
    TGuiTools.DrawRoundRect(ABuffer.Canvas, FTrack, Radius, IdleGradientFromColor, IdleGradientToColor,
                            bkVerticalGradient, ClipRect);

  TGuiTools.DrawAARoundFrame(ABuffer.Canvas, FTrack, Radius,
                             FToolbar.Appearance.Element.IdleInnerDarkColor, ClipRect);

  if FChecked then
    FToolbar.Appearance.Element.GetActiveColors(False, FontColor, FrameColor, InnerLightColor, InnerDarkColor,
                                                GradientFromColor, GradientToColor, GradientKind)
  else
    FToolbar.Appearance.Element.GetHotTrackColors(False, FontColor, FrameColor, InnerLightColor, InnerDarkColor,
                                                  GradientFromColor, GradientToColor, GradientKind);

  TButtonTools.DrawButton(
    ABuffer,
    FSwitcher,
    FrameColor,
    InnerLightColor,
    InnerDarkColor,
    GradientFromColor,
    GradientToColor,
    GradientKind,
    False,
    False,
    False,
    False,
    Radius,
    ClipRect
  );
end;

procedure TSpkSwitch.SetToolbar(const AValue: TSpkToolbar);
begin
  inherited SetToolbar(AValue);
  UpdateFontByStyle();
end;

(*
    PRIVATE
*)

procedure TSpkSwitch.SetCheckChanges();
var
  LSwt ,
  LTop ,
  LLeft: Integer;
begin
  if IsRightToLeft() then
    LLeft  := 2
  else
    LLeft  := FRect.Width - ((2 * PaneRowHeight) + SmallButtonPadding);
  LTop   := (FRect.Height + 2 - PaneRowHeight) div 2;

  if FChecked then
    LSwt := LLeft + PaneRowHeight
  else
    LSwt := LLeft;

{$IFDEF EnhancedRecordSupport}
  FTrack   := T2DIntRect.Create(LLeft, LTop, LLeft + (2 * PaneRowHeight), LTop + PaneRowHeight -3);
  FSwitcher:= T2DIntRect.Create(LSwt, LTop + 1, LSwt + PaneRowHeight, LTop + PaneRowHeight -4);
{$ELSE}
  FTrack.Create(LLeft, LTop, LLeft + (2 * PaneRowHeight), LTop + PaneRowHeight -3);
  FSwitcher.Create(LSwt, LTop + 1, LSwt + PaneRowHeight, LTop + PaneRowHeight -4);
{$ENDIF}
end;

function TSpkSwitch.GetCalcPad(): Integer;
begin
  Result := (2 * PaneRowHeight) + (2 * SmallButtonPadding);
end;

(*
    PUBLIC
*)
constructor TSpkSwitch.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  FChecked    := False;
  FOnUserClick:= nil;
  FOnChange   := nil;
  FFontStyle  := fsPane;

  if IsRightToLeft() then
    begin
      FLPad := GetCalcPad();
      SetAlignment(taRightJustify);
    end
  else
    begin
      FRPad := GetCalcPad();
      SetAlignment(taLeftJustify);
    end;
  Constraints.SetInterfaceConstraints(0, PaneRowHeight, 0, 0);
  SetLayout(tlCenter);

  TabStop        := True;
  AccessibleRole := larCheckBox;
end;

