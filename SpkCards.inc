{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkCard
*)

(*
    PROTECTED
*)

procedure TCustomSpkCard.DoDraw(const ABuffer: TBitmap);
var
  BgFromColor, BgToColor, CaptionColor: TColor;
  FontColor, BorderLightColor, BorderDarkColor: TColor;
  R: T2DIntRect;
  ClipRect: T2DIntRect;
  Bk: TBackgroundKind;
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
  if FConcave then
    Bk := bkConcave
  else
    Bk := bkVerticalGradient;

  TGuiTools.DrawRoundRect(
    ABuffer.Canvas,
    R,
    PaneCornerRadius,
    BgFromColor,
    BgToColor,
    Bk,
    ClipRect
  );

  TSpkCtrlsAux.DrawStyledBorder(FRect, ABuffer, FBorder, PaneCornerRadius, BorderDarkColor, BorderLightColor);
end;

procedure TCustomSpkCard.SetConcave(AValue: Boolean);
begin
  FConcave := AValue;

  Self.UpdateAppearance();
end;

procedure TCustomSpkCard.SetBorder(AValue: TSpkStyledBorder);
begin
  FBorder := AValue;

  Self.UpdateAppearance();
end;

procedure TCustomSpkCard.SetToolbar(const AValue: TSpkToolbar);
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

constructor TCustomSpkCard.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FConcave  := True;
  FBorder   := sbEtched;
end;

