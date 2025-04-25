{%MainUnit SpkCtrls.pp} 
 
(*
    TCustomSpkStatusbar
*)

(*
    PROTECTED
*)
procedure TCustomSpkStatusbar.DoDraw(const ABuffer: TBitmap);
var
  fontColor: TColor;
  I ,
  LX: Integer;
  LR: T2DIntRect;
begin
  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  TGUITools.RenderBackground(ABuffer, FRect, FToolbar.Appearance.Tab.GradientFromColor,
                                  FToolbar.Appearance.Tab.GradientToColor, FToolbar.Appearance.Tab.GradientType);

  fontColor := FToolbar.Appearance.Tab.InactiveTabHeaderFontColor;

  if not Self.Enabled then
    fontColor := TColorTools.ColorToGrayscale(fontColor);

  if FSimplePanel then
    begin
      ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Tab.TabHeaderFont);
      LX := ABuffer.Canvas.TextHeight('Wy');

      TGUITools.DrawText(ABuffer, SmallButtonBorderWidth, (FRect.Bottom - LX) div 2, Self.Caption, fontColor);
    end;

  if FShowGrip then
    begin
      ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Tab.TabHeaderFont);
      LX := ABuffer.Canvas.TextWidth(SPK_ARROW_CHAR);


      I  := FRect.Right - (SmallButtonHalfBorderWidth + SmallButtonBorderWidth + LX + SmallButtonBorderWidth);
{$IFDEF EnhancedRecordSupport}
      LR := T2DIntRect.Create(
{$ELSE}
      LR := Create2DIntRect(
{$ENDIF}
      I, SmallButtonBorderWidth, I + LX, FRect.Bottom - SmallButtonHalfBorderWidth);

      TSpkCtrlsAux.DrawStyledChar(LR, ABuffer, FToolbar.Appearance.Tab.TabHeaderFont, fontColor, SPK_SIZE_GRIP);
    end;
end;

procedure TCustomSpkStatusbar.SetSimplePanel(const AValue: Boolean);
var
  I: Integer;
begin
  FSimplePanel := AValue;

  if not (csLoading in ComponentState) then
    UpdateAppearance();

  for I := 0 to Self.ControlCount -1 do
    Self.Controls[I].Visible := not FSimplePanel;
end;

procedure TCustomSpkStatusbar.SetShowGrip(const AValue: Boolean);
begin
  FShowGrip := AValue;

  if not (csLoading in ComponentState) then
    UpdateAppearance();
end;

procedure TCustomSpkStatusbar.RealSetText(const AValue: TCaption);
begin
  inherited RealSetText(AValue);

  if Assigned(FOnTxtChange) then
    FOnTxtChange(Self);
end;

(*
    PUBLIC
*)

constructor TCustomSpkStatusbar.Create(AOwner: TComponent);
var
  LH: Integer;
begin
  inherited   Create(AOwner);
  FShowGrip      := True;
  FSimplePanel   := True;
  FOnTxtChange   := nil;
  AccessibleRole := larUnknown;
  ShowHint       := False;
  TabStop        := False;

  LH := PaneRowHeight + (2 * SmallButtonBorderWidth);
  Constraints.SetInterfaceConstraints(SmallButtonMinWidth, LH, 0, LH);
  SetBounds(Left, Top, 5 * SmallButtonMinWidth, LH);

  with Self.ChildSizing do
    begin
      TopBottomSpacing := SmallButtonBorderWidth;
      HorizontalSpacing:= 4 * SmallButtonBorderWidth;
      LeftRightSpacing := PaneRowHeight;
      Layout := cclNone;
    end;

  if (csDesigning in Self.ComponentState) then
    Self.Align := alBottom;
end;

procedure TCustomSpkStatusbar.InsertControl(AControl: TControl; Index: integer);
begin
  inherited InsertControl(AControl, Index);

  if (csDesigning in Self.ComponentState) then
    begin
      AControl.Align := alLeft;
    end
  else
  if FSimplePanel then
    AControl.Visible := not FSimplePanel;

  if not (csLoading in ComponentState) then
    UpdateAppearance();
end;

