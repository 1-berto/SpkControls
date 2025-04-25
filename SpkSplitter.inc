{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkSplitter
*)

(*
    PROTECTED
*)
procedure TCustomSpkSplitter.SetToolbar(const AValue: TSpkToolbar);
begin
  FToolbar := AValue;
  Repaint();
end;

(*
    PRIVATE
*)

procedure TCustomSpkSplitter.SpkPaintEvent(Sender: TObject);
var
  I: Integer;
  Clip: T2DIntRect;
begin
  if (FToolbar = nil) then
    Exit;

{$IFDEF EnhancedRecordSupport}
  Clip := T2DIntRect.Create(0, 0, Width, Height);
{$ELSE}
  Clip.Create(0, 0, Width, Height);
{$ENDIF}
  Canvas.Brush.Color := FToolbar.Appearance.Pane.CaptionBgColor;
  Canvas.FillRect(Self.GetClientRect());

  if (Self.Cursor = crHSplit) then
    begin
      I := (Self.Height - PaneRowHeight)  div 2;
      TGUITools.DrawVLine(Canvas, 2, I, I+PaneRowHeight, FToolbar.Appearance.Pane.BorderDarkColor, Clip);
      TGUITools.DrawVLine(Canvas, 5, I, I+ PaneRowHeight, FToolbar.Appearance.Pane.BorderDarkColor, Clip);
    end
  else
  if (Self.Cursor = crVSplit) then
    begin
      I := (Self.Width - PaneRowHeight) div 2;
      TGUITools.DrawHLine(Canvas, I, I+PaneRowHeight, 2, FToolbar.Appearance.Pane.BorderDarkColor, Clip);
      TGUITools.DrawHLine(Canvas, I, I+PaneRowHeight, 5, FToolbar.Appearance.Pane.BorderDarkColor, Clip);
    end;
end;

(*
    PUBLIC
*)
constructor TCustomSpkSplitter.Create(AOwner: TComponent);
begin
  inherited  Create(AOwner);
  FToolbar   := nil;
  OnPaint    := @SpkPaintEvent;
  Color      := SPKCTRLS_TRANSPARENT_COLOR;
  Constraints.MinWidth := (PaneRowHeight div 3) +1;
  Constraints.MinHeight:= (PaneRowHeight div 3) +1;
end;

procedure TCustomSpkSplitter.ReDraw();
begin
  Invalidate;
end;
