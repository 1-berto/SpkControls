{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkListbox
*)

(*
    PROTECTED
*)
procedure TCustomSpkListbox.SetToolbar(const AValue: TSpkToolbar);
begin
  FToolbar := AValue;
  if (FToolbar <> nil) then
    begin
      FToolbar.GetTempBitmap.Canvas.Font := FToolbar.Appearance.Element.CaptionFont;
      FCurItemHeight := FToolbar.GetTempBitmap.Canvas.TextHeight('Wy');
      Font := FToolbar.Appearance.Element.CaptionFont;
      Color:= FToolbar.Appearance.Element.IdleGradientFromColor;
      ItemHeight := Max(PaneRowHeight, FCurItemHeight);
    end;
  Repaint();
end;

(*
    PRIVATE
*)

procedure TCustomSpkListbox.SpkDrawItem(Control: TWinControl; Index: Integer; ARect: TRect; State: TOwnerDrawState);
begin
  if (FToolbar = nil) then
    Exit;

  with TCustomListBox(Control) do
    begin
      if (odSelected in State) then
        begin
          Canvas.Brush.Color := FToolbar.Appearance.Element.IdleFrameColor;
          Canvas.Font.Color := FToolbar.Appearance.Element.CaptionFont.Color;
        end
      else
        begin
          Canvas.Brush.Color := FToolbar.Appearance.Element.IdleGradientFromColor;
          Canvas.Font.Color := FToolbar.Appearance.Element.IdleCaptionColor;
        end;

      if not Self.Enabled then
        Canvas.Font.Color := TColorTools.ColorToGrayscale(Canvas.Font.Color);

      Canvas.Pen.Style := psClear;
      Canvas.FillRect(ARect);

      Canvas.TextRect(ARect,ARect.Left+SmallButtonBorderWidth,ARect.Top+(ARect.Height-FCurItemHeight) div 2, Items[Index]);
    end;
end;

(*
    PUBLIC
*)
constructor TCustomSpkListbox.Create(AOwner: TComponent);
begin
  inherited  Create(AOwner);
  FToolbar   := nil;
  OnDrawItem := @SpkDrawItem;
  BorderStyle:= bsNone;
  Style := lbOwnerDrawFixed;
end;

procedure TCustomSpkListbox.ReDraw();
begin
  Invalidate;
end;

