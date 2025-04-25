{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkControl
*)

(*
    PROTECTED
*)

procedure TCustomSpkControl.SetToolbar(const AValue: TSpkToolbar);
begin
  FToolbar := AValue;
  if (FToolbar <> nil) then
    Self.Repaint();
end;

procedure TCustomSpkControl.DoEnter;
begin
  if FocusOpt then
    Self.ReDraw();
end;

procedure TCustomSpkControl.DoExit;
begin
  if FocusOpt then
    Self.ReDraw();
end;

function TCustomSpkControl.CalcRect(): T2DIntRect;
begin
{$IFDEF EnhancedRecordSupport}
  Result := T2DIntRect.Create(0, 0, Width, Height);
{$ELSE}
  Result.Create(0, 0, Width, Height);
{$ENDIF}
end;

procedure TCustomSpkControl.InvalidateBuffer();
begin
  if (FBuffer <> nil) then
    FBuffer.Free();

  FBuffer := TBitmap.Create();
  FBuffer.PixelFormat := pf24bit;
  FBuffer.SetSize(Self.Width, Self.Height);
  FBuffer.Canvas.Brush.Style:=bsClear;

  if (Self.Parent is TCustomSpkControl) and Assigned(TCustomSpkControl(Self.Parent).FBuffer) then
    begin
      FBuffer.Canvas.CopyRect(Bounds(0, 0, Self.Width, Self.Height),
                              TCustomSpkControl(Self.Parent).FBuffer.Canvas,
                              Bounds(Self.Left, Self.Top, Self.Width, Self.Height));
    end
  else
    begin
      FBuffer.Canvas.Brush.Color := SPKCTRLS_TRANSPARENT_COLOR;
      FBuffer.Canvas.FillRect(Self.GetClientRect);
    end;

  Self.DoDraw(FBuffer);
end;

procedure TCustomSpkControl.SetEnabled(Value: Boolean);
begin
  inherited SetEnabled(Value);
  Self.ReDraw();
end;

procedure TCustomSpkControl.DoOnResize;
begin
  inherited DoOnResize;
  UpdateAppearance();
end;

procedure TCustomSpkControl.RealSetText(const AValue: TCaption);
begin
  inherited RealSetText(AValue);
  UpdateAppearance();
end;

procedure TCustomSpkControl.Paint();
begin
  Self.Canvas.Draw(0, 0, FBuffer);
end;

(*
    PUBLIC
*)
constructor TCustomSpkControl.Create(AOwner: TComponent);
begin
  inherited     Create(AOwner);
  DoubleBuffered:= True;
  FToolbar      := nil;
  FBuffer       := nil;
  TabStop       := True;
end;

destructor TCustomSpkControl.Destroy();
begin
  FToolbar := nil;
  FBuffer.Free();
  inherited Destroy();
end;

procedure TCustomSpkControl.ReDraw();
begin
  InvalidateBuffer();
  Repaint();
end;

procedure TCustomSpkControl.UpdateAppearance();
begin
  FRect := CalcRect();
  InvalidateBuffer();
  Repaint();
end;

