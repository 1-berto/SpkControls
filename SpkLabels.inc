{%MainUnit SpkCtrls.pp}

(*
     TCustomSpkLabel
*)

(*
    PROTECTED
*)

class
function TCustomSpkLabel.GetControlClassDefaultSize: TSize;
begin
  Result.CX := SmallButtonMinWidth;
  Result.CY := PaneCaptionHeight;
end;

procedure TCustomSpkLabel.CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; WithThemeSpace: Boolean);
begin
  PreferredWidth := FTextRect.Width;
  PreferredHeight:= Max(FTextRect.Height, PaneCaptionHeight);
end;

function TCustomSpkLabel.RealGetText: TCaption;
begin
  //
  //    NOTE: Necessary for design-time.
  //
  if inherited RealGetText = '' then
    Result := 'Label'
  else
    Result := inherited RealGetText;
end;

procedure TCustomSpkLabel.TextChanged;
begin
  if Self.AutoSize then
    begin
      InvalidatePreferredSize;
      AdjustSize;
    end;
  AccessibleValue := Caption;
end;

procedure TCustomSpkLabel.DoSetBounds(ALeft, ATop, AWidth, AHeight: integer);
var
  LMod: Boolean;
begin
  LMod := (AWidth <> Self.Width) or (AHeight <> Self.Height);
  inherited DoSetBounds(ALeft, ATop, AWidth, AHeight);

  if LMod then
    InvalidatePreferredSize;
end;

procedure TCustomSpkLabel.Loaded;
begin
  inherited Loaded;
  //AdjustSize;
  UpdateFontByStyle();
  UpdateAppearance();
end;

procedure TCustomSpkLabel.LoadedAll();
begin
  inherited LoadedAll();
  Self.InvalidateBuffer();
end;

function TCustomSpkLabel.CalcRect(): T2DIntRect;
var
  Bitmap    : TBitmap;
  TextWidth ,
  TextHeight: Integer;
begin
  if (Self.Caption = '') then
    Exit;

  if (FToolbar = nil) then
    Exit;

  Bitmap := FToolbar.GetTempBitmap;

  if not Assigned(Bitmap) then
    Exit;

  Bitmap.Canvas.Font.Assign(Self.Font);
  TextWidth := Bitmap.Canvas.TextWidth(Self.Caption);
  TextHeight:= Bitmap.Canvas.TextHeight(Self.Caption);

 {$IFDEF EnhancedRecordSupport}
  FTextRect := T2DIntRect.Create(0, 0, FLPad + TextWidth + FRPad, TextHeight);
  Result    := T2DIntRect.Create(0, 0, Self.Width, Self.Height);
 {$ELSE}
  FTextRect.Create(0, 0, FLPad + TextWidth + FRPad, TextHeight);
  Result.Create(0, 0, Self.Width, Self.Height);
 {$ENDIF}
end;

procedure TCustomSpkLabel.DoDraw(const ABuffer: TBitmap);
var
  x, y: Integer;
  FontColor: TColor;
begin
  FontColor := Self.Font.Color;
  if not Self.Enabled then
    FontColor := TColorTools.ColorToGrayscale(FontColor);

  case FAlignment of
    taLeftJustify : x := FLPad;
    taRightJustify: x :=  Self.Width - (FTextRect.Width + FRPad);
    taCenter      : x := ((Self.Width - FTextRect.Width) + FLPad + FRPad) div 2;
  end;

  case FLayout of
    tlTop   : y := 0;
    tlCenter: y := (Self.Height - FTextRect.Height) div 2;
    tlBottom: y := Self.Height - FTextRect.Height;
  end;

  if (FRole = trTitle) then
    TGUITools.DrawOutlinedText(ABuffer.Canvas, x, y, Self.Caption, FontColor, TColorTools.Brighten(FontColor, 65))
  else
    begin
      ABuffer.Canvas.Font.Assign(Self.Font);

      ABuffer.Canvas.Font.Color := FontColor;
      ABuffer.Canvas.TextOut(x, y, Self.Caption);
    end;
end;

procedure TCustomSpkLabel.SetAlignment(AValue: TAlignment);
begin
  FAlignment := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkLabel.SetLayout(AValue: TTextLayout);
begin
  FLayout := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkLabel.SetFontStyle(AValue: TSpkFontStyle);
begin
  FFontStyle := AValue;
  UpdateFontByStyle();
  UpdateAppearance();
end;

procedure TCustomSpkLabel.SetTextRole(AValue: TSpkTextRole);
begin
  FRole := AValue;
  UpdateFontByStyle();
  UpdateAppearance();
end;

(*
    PRIVATE
*)
procedure TCustomSpkLabel.UpdateFontByStyle();
begin
  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  case FFontStyle of
    fsElement : Self.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
    fsMenu    : Self.Font.Assign(FToolbar.Appearance.MenuButton.CaptionFont);
    fsPane    : Self.Font.Assign(FToolbar.Appearance.Pane.CaptionFont);
    fsTab     : Self.Font.Assign(FToolbar.Appearance.Tab.TabHeaderFont);
  end;

  if (FFontStyle = fsElement) or (FFontStyle = fsMenu) then
    begin
      if FHighlighted then
        Self.Font.Color := FToolbar.Appearance.Element.ActiveCaptionColor
      else
        Self.Font.Color := FToolbar.Appearance.Element.IdleCaptionColor;
    end;

  case FRole of
    trExtra : Self.Font.Style := Self.Font.Style + [fsBold];
    trDetail: Self.Font.Color := TColorTools.Brighten(Self.Font.Color, 25);
  end;

  if Self.AutoSize then
    begin
      InvalidatePreferredSize;
      AdjustSize;
    end;
end;

(*
    PUBLIC
*)
constructor TCustomSpkLabel.Create(AOwner: TComponent);
begin
  inherited   Create(AOwner);
  FLPad       := 0;
  FRPad       := 0;
  FRole       := trLabel;
  FAlignment  := taLeftJustify;
  FLayout     := tlTop;
  FFontStyle  := fsElement;
  FHighlighted:= False;
  TabStop     := False;
  AccessibleRole := larLabel;

  with GetControlClassDefaultSize do
    SetInitialBounds(0, 0, CX, CY);
end;

