{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkTrack
*)

(*
    PROTECTED
*)
procedure TCustomSpkTrack.SetMin(const AValue: Integer);
begin
  FMin := AValue;
  if not (csLoading in ComponentState) then
    DoRepos();
end;

procedure TCustomSpkTrack.SetMax(const AValue: Integer);
begin
  FMax := AValue;
  if not (csLoading in ComponentState) then
    DoRepos();
end;

procedure TCustomSpkTrack.SetPos(const AValue: Integer);
begin
  FPos := AValue;
  Self.Hint := IntToStr(FPos);
  if not (csLoading in ComponentState) then
    DoChange();
end;

procedure TCustomSpkTrack.SetSmallChange(const AValue: Integer);
begin
  FSmallChange := AValue;
end;

procedure TCustomSpkTrack.DoChange();
begin
  Repaint();
  if Assigned(FOnChange) then
    FOnChange(Self);
end;

procedure TCustomSpkTrack.SetReversed(AValue: Boolean);
begin
  FReversed := AValue;

  DoRepos();
  InvalidateBuffer();
  Repaint();
end;


(*
    PUBLIC
*)
constructor TCustomSpkTrack.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FMin         := 0;
  FMax         := 100;
  FPos         := 0;
  FSmallChange := SPKCTRLS_TRACK_SMALLCHANGE;
  FOnChange    := nil;
  FReversed    := False;
  SetupIntf();
end;

(*
    TCustomSpkTrackbar
*)

(*
    PROTECTED
*)

function TCustomSpkTrackbar.CalcRect(): T2DIntRect;
var
  Thickness: Integer;
begin
  Thickness := PaneRowHeight div 3;
  if (FOrientation = orHorizontal) then
    begin
{$IFDEF EnhancedRecordSupport}
      Result := T2DIntRect.Create(0, 0, Self.Width, Self.Height);
      FTrack := T2DIntRect.Create(2, Thickness, Self.Width -4, 2 * Thickness);
{$ELSE}
      Result.Create(0, 0, Self.Width, Self.Height);
      FTrack.Create(2, Thickness, Self.Width -4, 2 * Thickness);
{$ENDIF}
    end
  else
    begin
{$IFDEF EnhancedRecordSupport}
      Result := T2DIntRect.Create(0, 0, Self.Width, Self.Height);
      FTrack := T2DIntRect.Create(Thickness, 2, 2 * Thickness, Self.Height -4);
{$ELSE}
      Result.Create(0, 0, Self.Width, Self.Height);
      FTrack.Create(Thickness, 2, 2 * Thickness, Self.Height -4);
{$ENDIF}
    end;
end;

procedure TCustomSpkTrackbar.SetOrientation(AValue: TSpkOrientation);
var
  Len: Integer;
begin
  if (AValue <> FOrientation) then
    begin
      if (FOrientation = orHorizontal) then
        Len := Self.Width
      else
        Len := Self.Height;

      FOrientation := AValue;

      if (FOrientation = orHorizontal) then
        begin
          Self.Constraints.SetInterfaceConstraints(5 * PaneRowHeight, PaneRowHeight, 0, PaneRowHeight);
          Self.Width := Len;
        end
      else
        begin
          Self.Constraints.SetInterfaceConstraints(PaneRowHeight, 5 * PaneRowHeight, PaneRowHeight, 0);
          Self.Height := Len;
        end;
      DoRepos();
      InvalidateBuffer();
      Repaint();
    end;
end;

procedure TCustomSpkTrackbar.SetupIntf();
begin
  {$IFDEF EnhancedRecordSupport}
   FSlider := T2DIntRect.Create(0, 0, 0, 0);
  {$ELSE}
   FSlider.Create(0, 0, 0, 0);
  {$ENDIF}
  FReversed      := False;
  FOnUChange     := nil;
  FOrientation   := orVertical;
  FSliderState   := bsIdle;
  AccessibleRole := larTrackBar;
  SetOrientation(orHorizontal);

  SetInitialBounds(0, 0, 5 * PaneRowHeight, PaneRowHeight);
end;

procedure TCustomSpkTrackbar.DoOnResize();
begin
  inherited DoOnResize();
  DoRepos();
end;

procedure TCustomSpkTrackbar.DoRepos();
var
  CurPos: Integer;
begin
  FRect := Self.CalcRect();
  if (FOrientation = orHorizontal) then
    begin
      FTrackLen := FRect.Width - (PaneRowHeight + 2);

      if FReversed then
        CurPos := (FTrackLen * (FMax - FPos)) div (FMax - FMin)
      else
        CurPos := (FTrackLen * FPos) div (FMax - FMin);
{$IFDEF EnhancedRecordSupport}
      FSlider := T2DIntRect.Create(CurPos, 1, CurPos + PaneRowHeight, PaneRowHeight -2);
{$ELSE}
      FSlider.Create(CurPos, 1, CurPos + PaneRowHeight, PaneRowHeight -2);
{$ENDIF}
    end
  else
    begin
      FTrackLen := FRect.Height - (PaneRowHeight + 2);

      if FReversed then
        CurPos := (FTrackLen * (FMax - FPos)) div (FMax - FMin)
      else
        CurPos := (FTrackLen * FPos) div (FMax - FMin);
{$IFDEF EnhancedRecordSupport}
      FSlider := T2DIntRect.Create(1, CurPos, PaneRowHeight -2, CurPos + PaneRowHeight);
{$ELSE}
      FSlider.Create(1, CurPos, PaneRowHeight -2, CurPos + PaneRowHeight);
{$ENDIF}
    end;
end;

procedure TCustomSpkTrackbar.DoDraw(const ABuffer: TBitmap);
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

  if (FToolbar.Appearance.Element.Style = esRounded) then
    Radius := SmallButtonRadius
  else
    Radius := 0;

  if FocusOpt and (Screen.ActiveControl = Self) then
    TSpkCtrlsAux.DrawControlFocus(Self, ABuffer, FToolbar.Appearance.Element.IdleFrameColor);

  ClipRect := FRect;

  with FToolbar.Appearance.Element do
    if (FOrientation = orHorizontal) then
      TGuiTools.DrawRoundRect(ABuffer.Canvas, FTrack, Radius, IdleGradientFromColor, IdleGradientToColor, bkVerticalGradient, ClipRect)
    else
      TGuiTools.DrawRoundRect(ABuffer.Canvas, FTrack, Radius, IdleGradientFromColor, IdleGradientToColor, bkHorizontalGradient, ClipRect);

  if (FSliderState = bsIdle) then
    TGuiTools.DrawAARoundFrame(ABuffer.Canvas, FTrack, Radius,
                               FToolbar.Appearance.Element.IdleInnerDarkColor, ClipRect)
  else
    TGuiTools.DrawAARoundFrame(ABuffer.Canvas, FTrack, Radius,
                               FToolbar.Appearance.Element.IdleFrameColor, ClipRect);

  if (FSliderState = bsDropdownPressed) then
    FToolbar.Appearance.Element.GetActiveColors(True, FontColor, FrameColor, InnerLightColor, InnerDarkColor,
                                                GradientFromColor, GradientToColor, GradientKind)
  else
    FToolbar.Appearance.Element.GetHotTrackColors(not (FSliderState = bsIdle), FontColor, FrameColor, InnerLightColor,
                                                  InnerDarkColor, GradientFromColor, GradientToColor, GradientKind);

  TButtonTools.DrawButton(
    ABuffer,
    FSlider,
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

function TCustomSpkTrackbar.AllowChange(): Boolean;
begin
  Result := True;
end;

procedure TCustomSpkTrackbar.SetPos(const AValue: Integer);
begin
  if Self.AllowChange() then
    begin
      if (AValue < FMin) then
        inherited SetPos(FMin)
      else
      if (AValue > FMax) then
        inherited SetPos(FMax)
      else
        inherited SetPos(AValue);
      DoRepos();
      InvalidateBuffer();
      Repaint();
    end;
end;

procedure TCustomSpkTrackbar.KeyDown(var Key: Word; Shift: TShiftState);
begin
  case Key of
    VK_RIGHT: begin Key := 0; Self.UserSmallChange((FOrientation = orHorizontal)) end;
    VK_LEFT : begin Key := 0; Self.UserSmallChange((FOrientation <> orHorizontal)) end;
    VK_DOWN : begin Key := 0; Self.UserSmallChange((FOrientation = orVertical)) end;
    VK_UP   : begin Key := 0; Self.UserSmallChange((FOrientation = orHorizontal)) end;
    VK_ADD  : begin Key := 0; Self.UserSmallChange(True) end;
    VK_SUBTRACT: begin Key:= 0; Self.UserSmallChange(False) end;
  end;
end;

procedure TCustomSpkTrackbar.MouseLeave();
begin
  inherited MouseLeave();
  FSliderState := bsIdle;
  InvalidateBuffer();
  Repaint();
end;

procedure TCustomSpkTrackbar.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  inherited MouseDown(Button, Shift, X, Y);

  if (Button <> MouseClick) then
    Exit;

  if (FOrientation = orHorizontal) then
    begin
      if (FSlider.Contains(X, Y)) then
        begin
          FSliderState := bsBtnPressed;
          Self.Position:= FPos;
        end
      else
      if (ssShift in Shift) then
        begin
          if (X > FSlider.Right) then
            Self.Position := FPos + FSmallChange
          else
          if ( X < FSlider.Left) then
            Self.Position := FPos - FSmallChange;
        end
      else
        begin
          FSliderState := bsBtnPressed;

          X := X - (FSlider.Width div 2);

          if FReversed then
            Self.Position := (FMax - X * (FMax - FMin) div FTrackLen)
          else
            Self.Position := X * (FMax - FMin) div FTrackLen;
        end;
    end
  else
    begin
      if (FSlider.Contains(X, Y)) then
        begin
          FSliderState := bsBtnPressed;
          Self.Position:= FPos;
        end
      else
      if (ssShift in Shift) then
        begin
          if (Y > FSlider.Bottom) then
            Self.Position := FPos - FSmallChange
          else
          if ( Y < FSlider.Top) then
            Self.Position := FPos + FSmallChange;
        end
      else
        begin
          FSliderState := bsBtnPressed;

          Y := Y - (FSlider.Height div 2);

          if FReversed then
            Self.Position := (FMax - Y * (FMax - FMin) div FTrackLen)
          else
            Self.Position := Y * (FMax + FMin) div FTrackLen;
        end;
    end;

  if Assigned(FOnUChange) then
    FOnUChange(Self);

  if FocusOpt and (Screen.ActiveControl <> Self) then
    Self.SetFocus;
end;

procedure TCustomSpkTrackbar.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
  inherited MouseMove(Shift, X, Y);

  if not (FSliderState = bsBtnPressed) then
    Exit;

  if (FOrientation = orHorizontal) then
    begin
      X := X - (FSlider.Width div 2);

      if FReversed then
        Self.Position := (FMax - X * (FMax - FMin) div FTrackLen)
      else
        Self.Position := X * (FMax - FMin) div FTrackLen;
    end
  else
    begin
      Y := Y - (FSlider.Height div 2);

      if FReversed then
        Self.Position := (FMax - Y * (FMax - FMin) div FTrackLen)
      else
        Self.Position := Y * (FMax + FMin) div FTrackLen;
    end;

  if Assigned(FOnUChange) then
    FOnUChange(Self);
end;

procedure TCustomSpkTrackbar.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) then
    begin
      FSliderState := bsIdle;
      InvalidateBuffer();
      Repaint();
    end;
  inherited MouseUp(Button, Shift, X, Y);
end;

function TCustomSpkTrackbar.DoMouseWheelDown(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  Result  := False;
  Self.UserSmallChange((FOrientation = orVertical));
end;

function TCustomSpkTrackbar.DoMouseWheelUp(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  Result  := False;
  Self.UserSmallChange((FOrientation = orHorizontal));
end;

procedure TCustomSpkTrackbar.UserSmallChange(AInc: Boolean);
begin
  if AInc then
    Self.Position := Self.Position + FSmallChange
  else
    Self.Position := Self.Position - FSmallChange;

  if Assigned(FOnUChange) then
    FOnUChange(Self);

  if FocusOpt and (Screen.ActiveControl <> Self) then
    Self.SetFocus();
end;

(*
    PUBLIC
*)

(*
    TCustomSpkProgressbar
*)

(*
    PROTECTED
*)

procedure TCustomSpkProgressbar.Loaded();
begin
  inherited Loaded();
  SetPos(FPos);
end;

function TCustomSpkProgressbar.CalcRect(): T2DIntRect;
begin
{$IFDEF EnhancedRecordSupport}
  Result := T2DIntRect.Create(0, 0, Self.Width, Self.Height);
  FTrack := T2DIntRect.Create(2, 2, Self.Width -4, Self.Height -4);
{$ELSE}
  Result.Create(0, 0, Self.Width, Self.Height);
  FTrack.Create(2, 2, Self.Width -4, Self.Height -4);
{$ENDIF}
end;

procedure TCustomSpkProgressbar.SetupIntf();
begin
  TabStop        := False;
  FReversed      := False;
  AccessibleRole := larProgressIndicator;

  SetInitialBounds(0, 0, 5 * PaneRowHeight, PaneRowHeight);
  Constraints.SetInterfaceConstraints(2 * PaneRowHeight, PaneRowHeight, 0, PaneRowHeight);
end;

procedure TCustomSpkProgressbar.DoOnResize();
begin
  inherited DoOnResize;
  DoRepos();
end;

procedure TCustomSpkProgressbar.DoRepos();
var
  NewLeft,
  PixPos : Integer;
begin
  PixPos := (FTrack.Width * FPos) div (FMax - FMin);

  if FReversed then
    NewLeft := FTrack.Width - PixPos + 2
  else
    NewLeft := 2;

{$IFDEF EnhancedRecordSupport}
  FFiller := T2DIntRect.Create(NewLeft, 2, NewLeft + PixPos, FTrack.Height +1);
{$ELSE}
  FFiller.Create(NewLeft, 2, NewLeft + PixPos, FTrack.Height +1);
{$ENDIF}
end;

procedure TCustomSpkProgressbar.SetPos(const AValue: Integer);
begin
  if (AValue < FMin) then
    inherited SetPos(FMin)
  else
  if (AValue > FMax) then
    inherited SetPos(FMax)
  else
    inherited SetPos(AValue);

  DoRepos();
  InvalidateBuffer();
  Repaint();
end;

(*
    TSpkProgressbar
*)

(*
    PROTECTED
*)
procedure TSpkProgressbar.DoDraw(const ABuffer: TBitmap);
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

  if (FToolbar.Appearance.Element.Style = esRounded) then
    Radius := SmallButtonRadius
  else
    Radius := 0;

  ClipRect := FRect;

  with FToolbar.Appearance.Element do
    TGuiTools.DrawRoundRect(ABuffer.Canvas, FTrack, Radius, IdleGradientFromColor, IdleGradientToColor,
                            bkVerticalGradient, ClipRect);

  TGuiTools.DrawAARoundFrame(ABuffer.Canvas, FTrack, Radius,
                             FToolbar.Appearance.Element.IdleInnerDarkColor, ClipRect);

  FToolbar.Appearance.Element.GetHotTrackColors((FPos = FMax), FontColor, FrameColor, InnerLightColor, InnerDarkColor,
                                                GradientFromColor, GradientToColor, GradientKind);

  TButtonTools.DrawButton(
    ABuffer,
    FFiller,
    FrameColor,
    InnerLightColor,
    InnerDarkColor,
    GradientFromColor,
    GradientToColor,
    GradientKind,
    FReversed,
    not FReversed,
    False,
    False,
    Radius,
    ClipRect
  );
end;

