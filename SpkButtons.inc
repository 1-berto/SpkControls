{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkButton
*)

(*
    PROTECTED
*)
procedure TCustomSpkButton.DispatchUserClick(Shift: TShiftState; X, Y: Integer);
begin
  if Assigned(FOnUserClick) then
    FOnUserClick(Self);
end;

procedure TCustomSpkButton.MouseLeave();
begin
  FButtonState := bsIdle;
  InvalidateBuffer();
  Repaint();
end;

procedure TCustomSpkButton.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) then
    begin
      FButtonState := bsBtnPressed;

      if (Screen.ActiveControl = Self) then
        begin
          InvalidateBuffer();
          Repaint();
        end
      else
        Self.SetFocus();
    end;
end;

procedure TCustomSpkButton.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
  if (FButtonState = bsIdle) then
    begin
      FButtonState := bsBtnHottrack;
      InvalidateBuffer();
      Repaint();
    end;
end;

procedure TCustomSpkButton.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Button = MouseClick) then
    begin
      Self.DispatchUserClick(Shift, X, Y);

      FButtonState := bsBtnHottrack;
      InvalidateBuffer();
      Repaint();
    end;
end;

(*
    TCustomSpkPushButton
*)

(*
    PROTECTED
*)
procedure TCustomSpkPushButton.SetImageIndex(AValue: TImageIndex);
begin
  FImageIndex := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkPushButton.SetImgsWidth(AValue: Integer);
begin
  FImgsWidth := AValue;
end;

procedure TCustomSpkPushButton.SetImages(const AValue: TImageList);
begin
  FImages := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkPushButton.SetDisabledImages(const AValue: TImageList);
begin
  FDisImages := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkPushButton.SetButtonType(AValue: TSpkButtonType);
begin
  FButtonType := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkPushButton.SetPushed(AValue: Boolean);
begin
  FPushed := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkPushButton.SetToggleMode(AValue: Boolean);
begin
  if (AValue = FToggleMode) then
    Exit;

  FToggleMode := AValue;

  if (FToggleMode) then
    FButtonType := btToggle
  else
    FButtonType := btRegular;

  UpdateAppearance();
end;

procedure TCustomSpkPushButton.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if (Key = SPK_ENTER_KEY) or (Key = SPK_ACTIVATE_KEY) then
    begin
      Self.MouseDown(MouseClick, [], 1, 1);
      Sleep(120);
      Self.MouseUp(MouseClick, [], 1, 1);
    end
  else
    inherited;
end;

(*
    PRIVATE
*)

procedure TCustomSpkPushButton.DrawDropdownArrow(ABuffer: TBitmap; ARect: TRect; AColor: TColor);
var
  P: array[0..3] of TPoint;
begin
  P[2].x := ARect.Left + (ARect.Right - ARect.Left) div 2;
  P[2].y := ARect.Top + (ARect.Bottom - ARect.Top + DropDownArrowHeight) div 2 - 1;
  P[0] := Point(P[2].x - DropDownArrowWidth div 2, P[2].y - DropDownArrowHeight div 2);
  P[1] := Point(P[2].x + DropDownArrowWidth div 2, P[0].y);
  P[3] := P[0];
  ABuffer.Canvas.Brush.Color := AColor;
  ABuffer.Canvas.Pen.Style := psClear;
  ABuffer.Canvas.Polygon(P);
end;

(*
    PUBLIC
*)

constructor TCustomSpkPushButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FToggleMode    := False;
  FPushed        := False;
  FButtonType    := btRegular;
  FImageIndex    := -1;
  FImages        := nil;
  FDisImages     := nil;
  AccessibleRole := larButton;
end;

(*
    TSpkLargePushButton
*)

(*
    PROTECTED
*)
class
function TSpkLargePushButton.GetControlClassDefaultSize(): TSize;
begin
  with Result do
    begin
      CX := 2 * LargeButtonMinWidth;
      CY := PaneFullRowHeight;
    end;
end;

function TSpkLargePushButton.CalcRect(): T2DIntRect;
var
  LBtnRect: T2DIntRect;
begin
  {$IFDEF EnhancedRecordSupport}
   FRect := T2DIntRect.Create(0, 0, Self.Width -1, Self.Height -1);
   if FButtonType = btDropdown then
   begin
     LBtnRect := T2DIntRect.Create(FRect.Left, FRect.Top, FRect.Right, FRect.Bottom - LargeButtonDropdownFieldSize);
     FDropdownRect := T2DIntRect.Create(FRect.Left, FRect.Bottom - LargeButtonDropdownFieldSize, FRect.Right, FRect.Bottom);
   end else
   begin
     LBtnRect := FRect;
     FDropdownRect := T2DIntRect.Create(0, 0, 0, 0);
   end;
  {$ELSE}
   FRect.Create(0, 0, Self.Width -1, Self.Height -1);
   if FButtonType = btDropdown then
   begin
     LBtnRect.Create(FRect.Left, FRect.Top, FRect.Right, FRect.Bottom - LargeButtonDropdownFieldSize);
     FDropdownRect.Create(FRect.Left, FRect.Bottom - LargeButtonDropdownFieldSize, FRect.Right, FRect.Bottom);
   end else
   begin
     LBtnRect := FRect;
     FDropdownRect.Create(0, 0, 0, 0);
   end;
  {$ENDIF}
  Result := LBtnRect;
end;

procedure TSpkLargePushButton.DoDraw(const ABuffer: TBitmap);
var
  fontColor, frameColor: TColor;
  gradientFromColor, gradientToColor: TColor;
  innerLightColor, innerDarkColor: TColor;
  gradientKind: TBackgroundKind;
  x: Integer;
  y: Integer;
  delta: Integer;
  cornerRadius: Integer;
  imgList: TImageList;
  imgSize: TSize;
  txtHeight: Integer;
  breakPos, breakWidth: Integer;
  s: String;
  P: T2DIntPoint;
  drawBtn: Boolean;
  ppi: Integer;
  R: TRect;
  ClipRect: T2DIntRect;
  BtnKind : TSpkButtonKind;
begin
   if (FToolbar = nil) then
     Exit;

  ClipRect := FRect;

  delta := FToolbar.Appearance.Element.HotTrackBrightnessChange;
  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := LargeButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  BtnKind :=  GetButtonKindByType(FButtonType);

  // Prepare text color
  fontColor := clNone;
  case FButtonState of
    bsIdle:
      fontColor := FToolbar.Appearance.Element.IdleCaptionColor;
    bsBtnHottrack,
    bsDropdownHottrack:
      fontColor := FToolbar.Appearance.Element.HotTrackCaptionColor;
    bsBtnPressed,
    bsDropdownPressed:
      fontColor := FToolbar.Appearance.Element.ActiveCaptionColor;
  end;

  if not Self.Enabled then
    fontColor := TColorTools.ColorToGrayscale(fontColor);

  // Dropdown button
  // Draw full rect, otherwise the DropDownRect will contain the full gradient
  if BtnKind = bkButtonDropdown then
  begin
    drawBtn := true;
    if (FButtonState in [bsBtnHotTrack, bsBtnPressed]) then
    begin
      FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientKind,
        delta);
    end else
    if (FButtonState = bsDropdownHottrack) then
    begin
      FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientKind);
    end else
    if (FButtonState = bsDropdownPressed) then
    begin
      FToolbar.Appearance.Element.GetActiveColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientKind);
    end else
      drawBtn := false;

    if drawBtn then begin
      TButtonTools.DrawButton(
        ABuffer,
        FRect,
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
      );
    end;
  end;

  // Button (Background and frame)
  drawBtn := true;
  if FButtonState = bsBtnHottrack then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind);
  end else
  if FButtonState = bsBtnPressed then
  begin
    FToolbar.Appearance.Element.GetActiveColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientkind);
  end else
  if (FButtonState in [bsDropdownHotTrack, bsDropdownPressed]) then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind,
      delta);
  end else
  if (FButtonState = bsIdle) and FPushed then
  begin
    FToolbar.Appearance.Element.GetActiveColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
    drawBtn := false;

  if drawBtn then
  begin
    TButtonTools.DrawButton(
      ABuffer,
      FRect,       // draw button part only
      frameColor,
      innerLightColor,
      innerDarkColor,
      gradientFromColor,
      gradientToColor,
      gradientKind,
      false,
      false,
      false,
      BtnKind = bkButtonDropdown,
      cornerRadius,
      ClipRect
    );
  end;

  // Dropdown button - draw horizontal dividing line
  if BtnKind = bkButtonDropdown then
  begin
    drawBtn := true;
    if (FButtonState in [bsDropdownHotTrack, bsBtnHotTrack]) then
      frameColor := FToolbar.Appearance.element.HotTrackFrameColor
    else
    if (FButtonState in [bsDropDownPressed, bsBtnPressed]) then
      frameColor := FToolbar.Appearance.Element.ActiveFrameColor
    else
      drawBtn := false;
    if drawBtn then
      TGuiTools.DrawHLine(
        ABuffer,
        FDropDownRect.Left,
        FDropDownRect.Right,
        FDropDownRect.Top,
        frameColor,
        ClipRect
     );
  end;

  // Icon
  if not Self.Enabled and (FDisImages <> nil) then
    imgList := FDisImages
  else
    imgList := FImages;

  if (imgList <> nil) and (FImageIndex >= 0) and (FImageIndex < imgList.Count) then
  begin
    ppi := FToolbar.Appearance.Element.CaptionFont.PixelsPerInch;
    {$IF LCL_FULLVERSION >= 1090000}
    imgSize := imgList.SizeForPPI[FImgsWidth, ppi];
    {$ELSE}
    imgSize := Size(imgList.Width, imgList.Height);
    {$ENDIF}

    P := {$IFDEF EnhancedRecordSupport}T2DIntPoint.Create{$ELSE}Create2DIntPoint{$ENDIF}(
      FRect.Left + (FRect.Width - imgSize.CX) div 2,
      FRect.Top + LargeButtonBorderSize + LargeButtonGlyphMargin
    );
    TGUITools.DrawImage(ABuffer.Canvas, imgList, FImageIndex, P, ClipRect,
      FImgsWidth, ppi, 1.0);
  end;

  // Text
  ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
  ABuffer.Canvas.Font.Color := fontColor;

  if BtnKind in [bkButton, bkToggle] then
    FindBreakPlace(Self.Caption, breakPos, breakWidth)
  else
    breakPos := 0;
  txtHeight := ABuffer.Canvas.TextHeight('Wy');

  if breakPos > 0 then
  begin
    s := Copy(Self.Caption, 1, breakPos - 1);
    x := FRect.Left + (FRect.Width - ABuffer.Canvas.Textwidth(s)) div 2;
    y := FRect.Top + LargeButtonCaptionTopRail - txtHeight div 2;
    TGUITools.DrawText(ABuffer.Canvas, x, y, s, fontColor, ClipRect);

    s := Copy(Self.Caption, breakPos+1, Length(Self.Caption) - breakPos);
    x := FRect.Left + (FRect.Width - ABuffer.Canvas.Textwidth(s)) div 2;
    y := FRect.Top + LargeButtonCaptionButtomRail - txtHeight div 2;
    TGUITools.DrawText(ABuffer.Canvas, x, y, s, fontColor, ClipRect);
  end else
  begin
    // The text is not broken
    x := FRect.Left + (FRect.Width - ABuffer.Canvas.Textwidth(Self.Caption)) div 2;
    y := FRect.Top + LargeButtonCaptionTopRail - txtHeight div 2;
    TGUITools.DrawText(ABuffer.Canvas, x, y, Self.Caption, FontColor, ClipRect);
  end;

  //Added later, a focus for button.
  if (Screen.ActiveControl = Self) and (FocusOpt) and (BtnFocusOpt) then
    TSpkCtrlsAux.DrawControlLineFocus(Self, ABuffer, x, y + txtHeight, ClipRect.Right-x, fontColor);

  // Dropdown arrow
  if BtnKind = bkDropdown then
  begin
    y := FRect.Bottom - ABuffer.Canvas.TextHeight('Tg') - 1;
    R := Classes.Rect(FRect.Left, y, FRect.Right, FRect.Bottom);
    DrawDropdownArrow(ABuffer, R, fontcolor);
  end else
  if BtnKind = bkButtonDropdown then
  begin
    y := FDropdownRect.Bottom - ABuffer.Canvas.TextHeight('Tg') - 1;
    R := Classes.Rect(FDropdownRect.Left, y, FDropDownRect.Right, FDropdownRect.Bottom);
    DrawDropdownArrow(ABuffer, R, fontcolor);
  end;
end;

(*
    PRIVATE
*)

procedure TSpkLargePushButton.FindBreakPlace(s: string; out Position: integer; out AWidth: integer);
var
  i: integer;
  Bitmap: TBitmap;
  BeforeWidth, AfterWidth: integer;
begin
  Position := -1;
  AWidth := -1;

  if (FToolbar.Appearance = nil) then
     exit;

  Bitmap := FToolbar.GetTempBitmap;
  if (Bitmap = nil) then
    exit;

  Bitmap.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);

  AWidth := Bitmap.Canvas.TextWidth(Self.Caption);

  for i := 1 to Length(s) do
    if s[i] = ' ' then
    begin
      if i > 1 then
        BeforeWidth := Bitmap.Canvas.TextWidth(Copy(s, 1, i-1))
      else
        BeforeWidth := 0;

      if i < Length(s) then
        AfterWidth := Bitmap.Canvas.TextWidth(Copy(s, i+1, Length(s)-i))
      else
        AfterWidth := 0;

      if (Position = -1) or (Max(BeforeWidth, AfterWidth) < Width) then
      begin
        AWidth := Max(BeforeWidth, AfterWidth);
        Position := i;
      end;
    end;
end;

(*
    PUBLIC
*)

constructor TSpkLargePushButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FToggleMode:= False;
  FImgsWidth := SPKCTRLS_LARGEBTN_WIDTH;
  FOnUserClick := nil;
  FButtonState := bsIdle;
end;

(*
    TCustomSpkAutoSizeButton
*)

(*
    PROTECTED
*)

class
function TCustomSpkAutoSizeButton.GetControlClassDefaultSize: TSize;
begin
  Result.CX := 3 * SmallButtonMinWidth;
  Result.CY := PaneCaptionHeight;
end;

procedure TCustomSpkAutoSizeButton.CalculatePreferredSize(var PreferredWidth, PreferredHeight: integer; WithThemeSpace: Boolean);
begin
  PreferredWidth := FRect.Width;
  PreferredHeight:= Max(FRect.Height, PaneCaptionHeight);
end;

procedure TCustomSpkAutoSizeButton.TextChanged;
begin
  if Self.AutoSize then
    begin
      InvalidatePreferredSize;
      AdjustSize;
    end;
  AccessibleValue := Caption;
  UpdateAppearance();
end;

procedure TCustomSpkAutoSizeButton.DoSetBounds(ALeft, ATop, AWidth, AHeight: integer);
var
  LMod: Boolean;
begin
  LMod := (AWidth <> Self.Width) or (AHeight <> Self.Height);
  inherited DoSetBounds(ALeft, ATop, AWidth, AHeight);

  if LMod then
    InvalidatePreferredSize;
end;

procedure TCustomSpkAutoSizeButton.Loaded;
begin
  inherited Loaded;
  UpdateAppearance();
  //AdjustSize;
end;

(*
    PUBLIC
*)

constructor TCustomSpkAutoSizeButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  with GetControlClassDefaultSize do
    SetInitialBounds(0, 0, CX, CY);
end;

(*
    TCustomSpkSmallPushButton
*)

(*
    PROTECTED
*)

function TCustomSpkSmallPushButton.RealGetText: TCaption;
begin
  //
  //    NOTE: Necessary for design-time.
  //
  if inherited RealGetText = '' then
    Result := 'Button'
  else
    Result := inherited RealGetText;
end;

procedure TCustomSpkSmallPushButton.SetGroupBehaviour(const AValue: TSpkGroupBehaviour);
begin
  FGroupBehaviour := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkSmallPushButton.SetUseFrame(const AValue: Boolean);
begin
  FUseFrame := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkSmallPushButton.SetShowCaption(const AValue: Boolean);
begin
  FShowCaption := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkSmallPushButton.SetTableBehaviour(const AValue: TSpkTableBehaviour);
begin
  FTableBehaviour := AValue;
  UpdateAppearance();
end;

function TCustomSpkSmallPushButton.GetGroupBehaviour(): TSpkGroupBehaviour;
begin
  Result := FGroupBehaviour;
end;

function TCustomSpkSmallPushButton.GetTableBehaviour(): TSpkTableBehaviour;
begin
  Result := FTableBehaviour;
end;

function TCustomSpkSmallPushButton.CalcRect(): T2DIntRect;
var
  LRect : T2DIntRect;
  RectVector: T2DIntVector;
begin
  if FButtonType = btSeparator then
  begin
    LRect.Create(0, 0, SmallButtonSeparatorWidth, LargeButtonDropdownFieldSize);
    FDropdownRect.Create(0, 0, 0, 0);
  end else
  begin
    ConstructRects(LRect, FDropdownRect);
   {$IFDEF EnhancedRecordSupport}
    RectVector := T2DIntVector.Create(0, 0);
   {$ELSE}
    RectVector.Create(0, 0);
   {$ENDIF}
    LRect := LRect + RectVector;
    FDropdownRect := FDropdownRect + RectVector;
   end;
  Result := LRect;
end;

procedure TCustomSpkSmallPushButton.DoDraw(const ABuffer: TBitmap);
var
  fontColor: TColor;
  frameColor, innerLightColor, innerDarkColor: TColor;
  gradientFromColor, gradientToColor: TColor;
  gradientKind: TBackgroundKind;
  P: T2DIntPoint;
  x, y: Integer;
  delta: Integer;
  cornerRadius: Integer;
  imgList: TImageList;
  imgSize: TSize;
  drawBtn: Boolean;
  R: TRect;
  dx: Integer;
  ppi: Integer;
  SeparatorRect: TRect;
  SeparatorLineColor: TColor;
  ClipRect: T2DIntRect;
begin
  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  if (FButtonType = btSeparator) then
  begin
    SeparatorLineColor := FToolbar.Appearance.Pane.BorderDarkColor;
    SeparatorRect.Create(FRect.Left + (SmallButtonSeparatorWidth div 2),
      FRect.Top,FRect.Left + (SmallButtonSeparatorWidth div 2) + 1,
      FRect.Bottom);
    TGUITools.DrawVLine(
      ABuffer,
      SeparatorRect.Left,
      SeparatorRect.Top + SmallButtonSeparatorTopMargin,
      SeparatorRect.Bottom - SmallButtonSeparatorBottomMargin,
      SeparatorLineColor
    );
    exit;
  end;

  if (FRect.Width < 2*SmallButtonRadius) or (FRect.Height < 2*SmallButtonRadius) then
    exit;

  delta := FToolbar.Appearance.Element.HotTrackBrightnessChange;
  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := SmallButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  ClipRect := FRect;

  // Button (Background and frame)
  drawBtn := true;
  if (FButtonState = bsIdle) and (FUseFrame) then
  begin
    FToolbar.Appearance.Element.GetIdleColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if FButtonState = bsBtnHottrack then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if FButtonState = bsBtnPressed then
  begin
    FToolbar.Appearance.Element.GetActiveColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FButtonState in [bsDropdownHotTrack, bsDropdownPressed]) then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind,
      delta
    );
  end else
    drawBtn := false;

  if drawBtn then
  begin
    TButtonTools.DrawButton(
      ABuffer,
      FRect,       // draw button part only
      frameColor,
      innerLightColor,
      innerDarkColor,
      gradientFromColor,
      gradientToColor,
      gradientKind,
      (FGroupBehaviour in [gbContinuesGroup, gbEndsGroup]),
      (FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup]) or (FButtontype = btButtonDropdown),
      false,
      false,
      cornerRadius,
      ClipRect
    );
  end;

  // Icon
  if not Self.Enabled and (FDisImages <> nil) then
    imgList := FDisImages
  else
    imgList := FImages;

  if (imgList <> nil) and (FImageIndex >= 0) and (FImageIndex < imgList.Count) then
  begin
    ppi := FToolbar.Appearance.Element.CaptionFont.PixelsPerInch;
    {$IF LCL_FULLVERSION >= 1090000}
    imgSize := imgList.SizeForPPI[FImgsWidth, ppi];
    {$ELSE}
    imgSize := Size(imgList.Width, imgList.Height);
    {$ENDIF}

    if (FGroupBehaviour in [gbContinuesGroup, gbEndsGroup]) then
      x := FRect.Left + SmallButtonHalfBorderWidth + SmallButtonPadding
    else
      x := FRect.Left + SmallButtonBorderWidth + SmallButtonPadding;
    y := FRect.top + (FRect.height - imgSize.CY) div 2;
    P := {$IFDEF EnhancedRecordSupport}T2DIntPoint.Create{$ELSE}Create2DIntPoint{$ENDIF}(x, y);
    TGUITools.DrawImage(
      ABuffer.Canvas,
      imgList,
      FImageIndex,
      P,
      ClipRect,
      FImgsWidth,
      ppi, 1.0
    );
  end;

  // Prepare font and chevron color
  fontColor := clNone;
  case FButtonState of
    bsIdle:
      fontColor := FToolbar.Appearance.Element.IdleCaptionColor;
    bsBtnHottrack,
    bsDropdownHottrack:
      fontColor := FToolbar.Appearance.Element.HotTrackCaptionColor;
    bsBtnPressed,
    bsDropdownPressed:
      fontColor := FToolbar.Appearance.ELement.ActiveCaptionColor;
  end;

  if not Self.Enabled then
    fontColor := TColorTools.ColorToGrayscale(fontColor);

  // Text
  if FShowCaption then
  begin
    ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
    ABuffer.Canvas.Font.Color := fontColor;

    if (FGroupBehaviour in [gbContinuesGroup, gbEndsGroup]) then
      x := FRect.Left + SmallButtonHalfBorderWidth
    else
      x := FRect.Left + SmallButtonBorderWidth;

    if FImageIndex <> -1 then
      x := x + 2 * SmallButtonPadding + SmallButtonGlyphWidth
    else
      x := x + SmallButtonPadding;
    y := FRect.Top + (FRect.Height - ABuffer.Canvas.TextHeight('Wy')) div 2;

    TGUITools.DrawText(ABuffer.Canvas, x, y, Self.Caption, fontColor, ClipRect);

    // Added later, a focus for button.
    if (Screen.ActiveControl = Self) and (FocusOpt) and (BtnFocusOpt) then
      TSpkCtrlsAux.DrawControlLineFocus(Self, ABuffer, x, y + ABuffer.Canvas.TextHeight('Wy'), ClipRect.Right-x, fontColor);
  end
  else
  begin
    // NOTE: When dont show text then we suppose the user is using an image so we draw the focus around.
    if (Screen.ActiveControl = Self) and (FocusOpt) and (BtnFocusOpt) then
      TSpkCtrlsAux.DrawControlCustomFocus(Self, ABuffer, Bounds(x-1,y-1,imgSize.CX+2,imgSize.CY+2), fontColor);
  end;

  // Dropdown button
  if FButtonType = btButtonDropdown then
  begin
    drawBtn := true;
    if (FButtonState = bsIdle) and (FUseFrame) then
    begin
      FToolbar.Appearance.Element.GetIdleColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientkind
      );
    end else
    if (FButtonState in [bsBtnHottrack, bsBtnPressed]) then
    begin
      FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientKind,
        delta
      );
    end else
    if (FButtonState = bsDropdownHottrack) then
    begin
      FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientkind
      );
    end else
    if (FButtonState = bsDropdownPressed) then
    begin
      FToolbar.Appearance.Element.GetActiveColors(FPushed,
        fontColor, frameColor, innerLightColor, innerDarkColor,
        gradientFromColor, gradientToColor, gradientKind
      );
    end else
      drawBtn := false;

    if drawBtn then begin
      TButtonTools.DrawButton(
        ABuffer,
        FDropdownRect,
        frameColor,
        innerLightColor,
        innerDarkColor,
        gradientFromColor,
        gradientToColor,
        gradientKind,
        true,
        (FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup]),
        false,
        false,
        cornerRadius,
        ClipRect
      );
    end;
  end;

  // Dropdown arrow
  if FButtonType in [btDropdown, btButtonDropdown] then begin
    dx := SmallButtonDropdownWidth;
    if FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup] then
      inc(dx, SmallButtonHalfBorderWidth)
    else
      inc(dx, SmallButtonBorderWidth);
    if FButtonType = btDropdown then
      R := Classes.Rect(FRect.Right-dx, FRect.Top, FRect.Right, FRect.Bottom)
    else
      R := Classes.Rect(FDropdownRect.Right-dx, FDropdownRect.Top, FDropdownRect.Right, FDropdownRect.Bottom);
    DrawdropdownArrow(ABuffer, R, fontcolor);
  end;
end;

(*
    PUBLIC
*)
constructor TCustomSpkSmallPushButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  //FToggleMode     := False;
  FImgsWidth      := SPKCTRLS_SMALLBTN_WIDTH;
  //FOnUserClick    := nil;
  //FButtonState    := bsIdle;
  FTableBehaviour := TSpkTableBehaviour.tbContinuesRow;
  FGroupBehaviour := TSpkGroupBehaviour.gbSingleItem;
  FUseFrame       := True;
  FShowCaption    := True;

  AutoSize        := True;
end;

function TCustomSpkSmallPushButton.GeItemtWidth(): Integer;
var
  BtnRect ,
  DropRect: T2DIntRect;
begin
  Result := -1;

  if (FToolbar.Appearance = nil) then
    Exit;

  if (FButtonType = btSeparator) then
  begin
    Result := SmallButtonSeparatorWidth;
    Exit;
  end;

  ConstructRects(BtnRect, DropRect);

  if (FButtonType = btButtonDropdown) then
    Result := DropRect.Right + 1
  else
    Result := BtnRect.Right + 1;
end;

(*
    PRIVATE
*)
procedure TCustomSpkSmallPushButton.ConstructRects(out BtnRect, DropRect: T2DIntRect);
var
  BtnWidth: integer;
  DropdownWidth: Integer;
  Bitmap: TBitmap;
  TextWidth: Integer;
  AdditionalPadding: Boolean;
begin
  {$IFDEF EnhancedRecordSupport}
  BtnRect := T2DIntRect.Create(0, 0, 0, 0);
  DropRect := T2DIntRect.Create(0, 0, 0, 0);
  {$ELSE}
  BtnRect.Create(0, 0, 0, 0);
  DropRect.Create(0, 0, 0, 0);
  {$ENDIF}

  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  Bitmap := FToolbar.GetTempBitmap;
  if not Assigned(Bitmap) then
    Exit;

  // *** Regardless of the type, there must be room for the icon and / or text ***

  BtnWidth := 0;
  AdditionalPadding := false;

  // Icon
  if FImageIndex <> -1 then
  begin
    BtnWidth := BtnWidth + SmallButtonPadding + SmallButtonGlyphWidth;
    AdditionalPadding := true;
  end;

  // Text
  if FShowCaption then
  begin
    Bitmap.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
    TextWidth := Bitmap.Canvas.TextWidth(Self.Caption);

    BtnWidth := BtnWidth + SmallButtonPadding + TextWidth;
    AdditionalPadding := true;
  end;

  // Padding behind the text or icon
  if AdditionalPadding then
    BtnWidth := BtnWidth + SmallButtonPadding;

  // The width of the button content must be at least SMALLBUTTON_MIN_WIDTH
  BtnWidth := Max(SmallButtonMinWidth, BtnWidth);

  // *** Dropdown ***
  case FButtonType of
    btRegular, btToggle:
      begin
        // Left edge of the button
        if FGroupBehaviour in [gbContinuesGroup, gbEndsGroup] then
          BtnWidth := BtnWidth + SmallButtonHalfBorderWidth
        else
          BtnWidth := BtnWidth + SmallButtonBorderWidth;

        // Right edge of the button
        if (FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup]) then
          BtnWidth := BtnWidth + SmallButtonHalfBorderWidth
        else
          BtnWidth := BtnWidth + SmallButtonBorderWidth;

        {$IFDEF EnhancedRecordSupport}
        BtnRect := T2DIntRect.Create(0, 0, BtnWidth - 1, SpkLayoutSizes.PANE_ROW_HEIGHT - 1);
        DropRect := T2DIntRect.Create(0, 0, 0, 0);
        {$ELSE}
        BtnRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
        DropRect.Create(0, 0, 0, 0);
        {$ENDIF}
      end;

    btButtonDropdown:
      begin
        // Left edge of the button
        if FGroupBehaviour in [gbContinuesGroup, gbEndsGroup] then
          BtnWidth := BtnWidth + SmallButtonHalfBorderWidth
        else
          BtnWidth := BtnWidth + SmallButtonBorderWidth;

        // Right edge of the button
        BtnWidth := BtnWidth + SmallButtonHalfBorderWidth;

        // Left edge and dropdown field content
        DropdownWidth := SmallButtonHalfBorderWidth + SmallButtonDropdownWidth;

        // Right edge of the dropdown field
        if (FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup]) then
          DropdownWidth := DropdownWidth + SmallButtonHalfBorderWidth
        else
          DropdownWidth := DropdownWidth + SmallButtonBorderWidth;

        {$IFDEF EnhancedRecordSupport}
        BtnRect := T2DIntRect.Create(0, 0, BtnWidth - 1, PaneRowHeightT - 1);
        DropRect := T2DIntRect.Create(BtnRect.Right+1, 0, BtnRect.Right+DropdownWidth, PaneRowHeight - 1);
        {$ELSE}
        BtnRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
        DropRect.Create(BtnRect.Right+1,  0, BtnRect.Right+DropdownWidth, PaneRowHeight - 1);
        {$ENDIF}
      end;

    btDropdown:
      begin
        // Left edge of the button
        if FGroupBehaviour in [gbContinuesGroup, gbEndsGroup] then
          BtnWidth := BtnWidth + SmallButtonHalfBorderWidth
        else
          BtnWidth := BtnWidth + SmallButtonBorderWidth;

        // Right edge of the button
        if (FGroupBehaviour in [gbBeginsGroup, gbContinuesGroup]) then
          BtnWidth := BtnWidth + SmallButtonHalfBorderWidth
        else
          BtnWidth := BtnWidth + SmallButtonBorderWidth;

        // Additional area for dropdown + place for the central edge,
        // for dimensional compatibility with dkButtonDropdown
        BtnWidth := BtnWidth + SmallButtonBorderWidth + SmallButtonDropdownWidth;

        {$IFDEF EnhancedRecordSupport}
        BtnRect := T2DIntRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
        DropRect := T2DIntRect.Create(0, 0, 0, 0);
        {$ELSE}
        BtnRect.Create(0, 0, BtnWidth - 1, PaneRowHeight - 1);
        DropRect.Create(0, 0, 0, 0);
        {$ENDIF}
      end;
  end;
end;

