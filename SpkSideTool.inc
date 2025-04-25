{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkSideTool
*)

(*
    PROTECTED
*)
procedure TCustomSpkSideTool.MouseEnter();
begin
  FState := bsBtnHottrack;
  InvalidateBuffer();
  Repaint();

  inherited MouseEnter();
end;

procedure TCustomSpkSideTool.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) then
    begin
      DispatchMouseDown(Shift, X, Y);
      FState := bsDropdownPressed;
      InvalidateBuffer();
      Repaint();
    end;

  inherited MouseDown(Button, Shift, X, Y);
end;

procedure TCustomSpkSideTool.MouseLeave();
begin
  FState := bsIdle;
  InvalidateBuffer();
  Repaint();

  inherited MouseLeave();
end;

procedure TCustomSpkSideTool.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) then
    begin
      DispatchMouseUp(Shift, X, Y);
      FState := bsBtnHottrack;
      InvalidateBuffer();
      Repaint();
    end;

  inherited MouseUp(Button, Shift, X, Y);
end;

procedure TCustomSpkSideTool.SetSideAlign(AValue: TSpkSideAlign);
begin
  FSideAlign := AValue;
  Self.UpdateAppearance();
end;

procedure TCustomSpkSideTool.SetAssociate(const ACtrl: TControl);
begin
  FAssociate := ACtrl;
end;

function TCustomSpkSideTool.CalcRect(): T2DIntRect;
begin
{$IFDEF EnhancedRecordSupport}
  Result := T2DIntRect.Create(0, 0, Self.Width -1, Self.Height -1);
{$ELSE}
  Result.Create(0, 0, Self.Width -1, Self.Height -1);
{$ENDIF}
end;
 
procedure TCustomSpkSideTool.DoDraw(const ABuffer: TBitmap);
var
  fontColor: TColor;
  frameColor, innerLightColor, innerDarkColor: TColor;
  gradientFromColor, gradientToColor: TColor;
  gradientKind: TBackgroundKind;
  delta, cornerRadius: Integer;
  ClipRect: T2DIntRect;
begin
  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  delta := FToolbar.Appearance.Element.HotTrackBrightnessChange;
  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := SmallButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  ClipRect := FRect;

  if (FState = bsIdle) then
  begin
    FToolbar.Appearance.Element.GetIdleColors(False,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if FState = bsBtnHottrack then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(False,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if FState = bsBtnPressed then
  begin
    FToolbar.Appearance.Element.GetActiveColors(False,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FState in [bsDropdownHotTrack, bsDropdownPressed]) then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(false,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind,
      delta
    );
  end;

  TButtonTools.DrawButton(
    ABuffer,
    FRect,
    frameColor,
    innerLightColor,
    innerDarkColor,
    gradientFromColor,
    gradientToColor,
    gradientKind,
    (FSideAlign = saLeft),
    (FSideAlign = saRight),
    false,
    false,
    cornerRadius,
    ClipRect
  );
end;

(*
    PUBLIC
*)
constructor TCustomSpkSideTool.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FState    := bsIdle;
  FAssociate:= nil;
end;

destructor TCustomSpkSideTool.Destroy();
begin
  FAssociate := nil;
  inherited Destroy();
end;

