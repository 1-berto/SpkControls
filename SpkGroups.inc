{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkGroupBox
*)

(*
    PROTECTED
*)
function TCustomSpkGroupBox.CalcRect(): T2DIntRect;
var
  L,W,H: Integer;
  Bmp: TBitmap;
begin
  Result := inherited CalcRect();

  if (FToolbar = nil) then
    Exit;

  Bmp := FToolbar.GetTempBitmap();
  Bmp.Canvas.Font.Assign(FToolbar.Appearance.Pane.CaptionFont);

  W := Bmp.Canvas.TextWidth(Self.Caption);
  H := Max(Bmp.Canvas.TextHeight('Wy'), PaneCaptionHeight);

  {$IFDEF EnhancedRecordSupport}
  FTitlebar := T2DIntRect.Create(
  {$ELSE}
  FTitlebar := Create2DIntRect(
  {$ENDIF}
      FRect.Left,
      FRect.Top ,
      FRect.Right - PaneBorderHalfSize,
      FRect.Top + H + PaneBorderHalfSize
    );

  if FTitleAlign then
    begin
      if Self.IsRightToLeft then
        L := FRect.Left + (FRect.Width - W)
      else
        L := FRect.Left;
      L += PaneBorderSize;
    end
  else
    L := FRect.Left + (FRect.Width - W) div 2;

{$IFDEF EnhancedRecordSupport}
    FTitle := T2DIntRect.Create(
{$ELSE}
    FTitle := Create2DIntRect(
{$ENDIF}
      L,
      1,
      FRect.Right - PaneBorderSize,
      H + PaneBorderSize
    );
end;

procedure TCustomSpkGroupBox.DoDraw(const ABuffer: TBitmap);
var
  BgFromColor, BgToColor, CaptionColor: TColor;
  FontColor, BorderLightColor, BorderDarkColor: TColor;
  R: T2DIntRect;
  ClipRect: T2DIntRect;
begin
  if (FToolbar = nil) then
    Exit;

  with FToolbar do
    begin
      BgFromColor := Appearance.Pane.GradientFromColor;
      BgToColor := Appearance.Pane.GradientToColor;
      CaptionColor := Appearance.Pane.CaptionBgColor;
      FontColor := Appearance.Pane.CaptionFont.Color;
      BorderLightColor := Appearance.Pane.BorderLightColor;
      BorderDarkColor := Appearance.Pane.BorderDarkColor;
    end;

{$IFDEF EnhancedRecordSupport}
  R := T2DIntRect.Create(
{$ELSE}
  R := Create2DIntRect(
{$ENDIF}
    FRect.Left,
    FRect.Top,
    FRect.Right - PaneBorderHalfSize,
    FRect.Bottom - PaneBorderHalfSize
  );

  ClipRect := FRect;

  TGuiTools.DrawRoundRect(
    ABuffer.Canvas,
    R,
    PaneCornerRadius,
    BgFromColor,
    BgToColor,
    bkSolid,
    ClipRect
  );

  TGuiTools.DrawRoundRect(
    ABuffer.Canvas,
    FTitlebar,
    PaneCornerRadius,
    CaptionColor,
    clNone,
    bkSolid,
    ClipRect,
    True,
    True,
    False,
    False
  );

  if not Self.Enabled then
    FontColor := TColorTools.ColorToGrayscale(fontColor);

  ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Pane.CaptionFont);
  TGUITools.DrawText(ABuffer.Canvas, FTitle.Left, FTitle.Top, Self.Caption, FontColor, ClipRect);

  TSpkCtrlsAux.DrawStyledBorder(FRect, ABuffer, FBorder, PaneCornerRadius, BorderDarkColor, BorderLightColor);
end;

procedure TCustomSpkGroupBox.SetBorder(AValue: TSpkStyledBorder);
begin
  FBorder := AValue;

  Self.UpdateAppearance();
end;

procedure TCustomSpkGroupBox.SetToolbar(const AValue: TSpkToolbar);
begin
  inherited SetToolbar(AValue);

  if (FToolbar <> nil) then
    case FToolbar.Appearance.Pane.Style of
      psDividerEtched, psDividerFlat, psDividerRaised: SetBorder(sbNone);
      psRectangleEtched: SetBorder(sbEtched);
      psRectangleFlat: SetBorder(sbFlat);
      psRectangleRaised: SetBorder(sbRaised);
    end;
end;

(*
    PUBLIC
*)

constructor TCustomSpkGroupBox.Create(AOwner: TComponent);
begin
  inherited   Create(AOwner);
  FTitleAlign := False;
end;

