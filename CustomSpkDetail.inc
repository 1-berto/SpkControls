{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkDetail
*)

(*
    PROTECTED
*)

procedure TCustomSpkDetail.InvalidateBuffer();
begin
  if (FBuffer <> nil) then
    FBuffer.Free();

  FBuffer := TBitmap.Create();
  FBuffer.PixelFormat := pf24bit;
  FBuffer.Canvas.AntialiasingMode := amOn;
  FBuffer.SetSize(Self.Width, Self.Height);
  FBuffer.Canvas.Brush.Style:=bsClear;

  if (Self.Parent is TCustomSpkPanel) and Assigned(TCustomSpkPanel(Self.Parent).FBuffer) then
    begin
      FBuffer.Canvas.CopyRect(Bounds(0, 0, Self.Width, Self.Height),
                              TCustomSpkPanel(Self.Parent).FBuffer.Canvas,
                              Bounds(Self.Left, Self.Top, Self.Width, Self.Height));
    end
  else
    begin
      FBuffer.Transparent := True;
      FBuffer.TransparentColor := SPKCTRLS_TRANSPARENT_COLOR;
      FBuffer.Canvas.Brush.Color := SPKCTRLS_TRANSPARENT_COLOR;
      FBuffer.Canvas.FillRect(Self.GetClientRect);
    end;

  Self.DoDraw(FBuffer);
end;

procedure TCustomSpkDetail.SetToolbar(const AValue: TSpkToolbar);
begin
  FToolbar := AValue;

  if (FToolbar <> nil) then
    UpdateAppearance();
end;

procedure TCustomSpkDetail.SetEnabled(Value: Boolean);
begin
  inherited SetEnabled(Value);
  UpdateAppearance();
end;

procedure TCustomSpkDetail.DoOnResize;
begin
  inherited DoOnResize;
  UpdateAppearance();
end;

procedure TCustomSpkDetail.RealSetText(const AValue: TCaption);
begin
  inherited RealSetText(AValue);
  UpdateAppearance();
end;

procedure TCustomSpkDetail.Paint();
begin
  Self.Canvas.Draw(0, 0, FBuffer);
end;

(*
    PUBLIC
*)
constructor TCustomSpkDetail.Create(AOwner: TComponent);
begin
  inherited    Create(AOwner);
  FToolbar     := nil;
  FBuffer      := nil;
  ControlStyle := [csCaptureMouse, csSetCaption, csClickEvents, csDoubleClicks, csReplicatable] - [csOpaque];
end;

destructor TCustomSpkDetail.Destroy();
begin
  FToolbar := nil;
  FBuffer.Free();
  inherited Destroy();
end;

procedure TCustomSpkDetail.ReDraw();
begin
  InvalidateBuffer();
  Repaint();
end;

procedure TCustomSpkDetail.UpdateAppearance();
begin
  FRect := CalcRect();
  InvalidateBuffer();
  Repaint();
end;

