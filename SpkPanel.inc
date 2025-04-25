{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkPanel
*)

(*
    PUBLIC
*)
constructor TCustomSpkPanel.Create(AOwner: TComponent);
begin
  inherited     Create(AOwner);
  FCompStyle    := csPanel;
  ControlStyle  := ControlStyle + [csAcceptsControls, csCaptureMouse, csClickEvents, csDoubleClicks,
                                   csReplicatable, {csNoFocus,}csAutoSize0x0, csOpaque];
  SetInitialBounds(0, 0, 300, 200);
  AccessibleRole := larGroup;
  AccessibleDescription := rsTPanelAccessibilityDescription;
end;

procedure TCustomSpkPanel.ReDraw();
var
  Loop: Integer;
begin
  inherited ReDraw();

  for Loop := 0 to Self.ControlCount -1 do
    if (Self.Controls[Loop] is ISpkControl) then
      (Self.Controls[Loop] as ISpkControl).ReDraw();
end;

(*
    PROTECTED
*)

procedure TCustomSpkPanel.SetToolbar(const AValue: TSpkToolbar);
var
  Loop: Integer;
begin
  inherited SetToolbar(AValue);

  for Loop := 0 to Self.ControlCount -1 do
    if (Self.Controls[Loop] is ISpkControl) then
      (Self.Controls[Loop] as ISpkControl).SetToolbar(Self.SpkToolbar);
end;

procedure TCustomSpkPanel.DoOnResize;
var
  Loop: Integer;
begin
  inherited DoOnResize;

  for Loop := 0 to Self.ControlCount - 1 do
    if (Self.Controls[Loop] is ISpkControl) and (Self.Controls[Loop].Align = alNone) then
      (Self.Controls[Loop] as ISpkControl).ReDraw();
end;

(*
    TSpkPanel
*)

(*
    PROTECTED
*)

procedure TSpkPanel.DoDraw(const ABuffer: TBitmap);
begin
  if (FToolbar = nil) then
    Exit;

  with FToolbar.Appearance do
    if FInset then
      TGUITools.RenderBackground(ABuffer, FRect, Element.IdleInnerLightColor, Element.IdleInnerDarkColor, bkVerticalGradient)
    else
      TGUITools.RenderBackground(ABuffer, FRect, Element.IdleGradientFromColor, Element.IdleGradientToColor, bkVerticalGradient);
end;

procedure TSpkPanel.SetInset(Value: Boolean);
begin
  FInset := Value;
  Self.ReDraw();
end;

(*
    PUBLIC
*)
constructor TSpkPanel.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FInset := False;
end;

