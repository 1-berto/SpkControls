{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkCombobox
*)

(*
    PROTECTED
*)

procedure TCustomSpkCombobox.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if FAllowAdd and (Button = MouseClick) and (X <= FAddRect.Right) then
    begin
      FButtonState := bsDropdownPressed;
      InvalidateBuffer();
      Repaint();
    end
  else
    begin
      inherited MouseDown(Button, Shift, X, Y);

      if (Button = MouseClick) then
        DoPopupCtrl();
    end;
end;

procedure TCustomSpkCombobox.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if FAllowAdd and (Button = MouseClick) and (X <= FAddRect.Right) then
    begin
      inherited MouseUp(Button, Shift, X, Y);
      DoUserAdd();
    end;
end;

function TCustomSpkCombobox.DoMouseWheelDown(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  if (FItemIndex < FItems.Count-1) then
    begin
      Sleep(50);
      Self.ItemIndex := FItemIndex + 1;
      Self.SetFocus;
    end;
  Result := True;
end;

function TCustomSpkCombobox.DoMouseWheelUp(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  if (FItemIndex > 0) then
    begin
      Sleep(50);
      Self.ItemIndex := FItemIndex -1;
      Self.SetFocus;
    end;
  Result := True;
end;

function TCustomSpkCombobox.CalcRect(): T2DIntRect;
begin
  {$IFDEF EnhancedRecordSupport}
    FRect := T2DIntRect.Create(
  {$ELSE}
    FRect.Create(
  {$ENDIF}
      0, 0, Self.Width - SmallButtonHalfBorderWidth, Self.Height - SmallButtonHalfBorderWidth
    );

  if (FToolbar = nil) then
    Exit;

  if FAllowAdd then
    begin
      {$IFDEF EnhancedRecordSupport}
        FAddRect := T2DIntRect.Create(
      {$ELSE}
        FAddRect.Create(
      {$ENDIF}
          FRect.Left + SmallButtonPadding,
          FRect.Top + SmallButtonPadding,
          FRect.Left + (3 * SmallButtonPadding) + SmallButtonDropdownWidth,
          FRect.Bottom - SmallButtonPadding
        );
    end;

  Result := FRect;
end;

procedure TCustomSpkCombobox.DoDraw(const ABuffer: TBitmap);
var
  fontColor, frameColor,
  innerLightColor, innerDarkColor,
  gradientFromColor, gradientToColor: TColor;
  gradientKind: TBackgroundKind;
  w, h, x, y: Integer;
  delta: Integer;
  cornerRadius: Integer;
  SepRect: TRect;
  SepLineColor: TColor;
  TmpRect, ClipRect: T2DIntRect;
begin
  if (FToolbar = nil) then
    Exit;

  delta := FToolbar.Appearance.Element.HotTrackBrightnessChange;

  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := SmallButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  if (FButtonState = bsIdle) then
  begin
    FToolbar.Appearance.Element.GetIdleColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FButtonState = bsBtnHottrack) then
  begin
    FToolbar.Appearance.Element.GetHotTrackColors(FPushed,
      fontColor, frameColor, innerLightColor, innerDarkColor,
      gradientFromColor, gradientToColor, gradientKind
    );
  end else
  if (FButtonState = bsBtnPressed) then
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
  end;

  ClipRect := FRect;

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

  if FAllowAdd then
    begin
      if (FButtonState in [bsDropdownHotTrack, bsDropdownPressed]) then
        begin
          FToolbar.Appearance.Element.GetActiveColors(FPushed,
            fontColor, frameColor, innerLightColor, innerDarkColor,
            gradientFromColor, gradientToColor, gradientKind
          );

          TmpRect       := FRect;
          TmpRect.Right := FAddRect.Right;
          TButtonTools.DrawButton(
            ABuffer,
            TmpRect,
            frameColor,
            innerLightColor,
            innerDarkColor,
            gradientFromColor,
            gradientToColor,
            gradientKind,
            false,
            true,
            false,
            false,
            cornerRadius,
            ClipRect
          );
        end;

      SepLineColor := FToolbar.Appearance.Pane.BorderDarkColor;
      {$IFDEF EnhancedRecordSupport}
        SepRect := T2DIntRect.Create(
      {$ELSE}
        SepRect.Create(
      {$ENDIF}
        (FRect.Left + FAddRect.Width + (SmallButtonSeparatorWidth div 2)),
        FRect.Top,
        FRect.Left + (SmallButtonSeparatorWidth div 2) + 1,
        FRect.Bottom);

      TGUITools.DrawVLine(
        ABuffer,
        SepRect.Left,
        SepRect.Top + SmallButtonSeparatorTopMargin,
        SepRect.Bottom - SmallButtonSeparatorBottomMargin,
        SepLineColor);

      TSpkCtrlsAux.DrawStyledChar(FAddRect, ABuffer, FToolbar.Appearance.Pane.CaptionFont, fontColor, '+');
    end;

  if (Self.Caption <> '') then
    begin
      ABuffer.Canvas.Font.Assign(FToolbar.Appearance.Element.CaptionFont);
      ABuffer.Canvas.Font.Color := fontColor;

      if FAllowAdd then
        begin
          x := FRect.Left + (2 * SmallButtonPadding) + SmallButtonDropdownWidth + SmallButtonSeparatorWidth;
        end
      else
        begin
          x := FRect.Left + SmallButtonPadding;
        end;

      w := (FRect.Left + FRect.Width) - (SmallButtonPadding + SmallButtonDropdownWidth);
      h := ABuffer.Canvas.TextHeight('Wy');
      y := FRect.Top + (FRect.Height - h) div 2;

    {$IFDEF EnhancedRecordSupport}
      FTextRect := T2DIntRect.Create(
    {$ELSE}
      FTextRect.Create(
    {$ENDIF}
        x,
        y,
        w,
        h
      );

      TGUITools.DrawText(ABuffer.Canvas, x, y, Self.Caption, fontColor, FTextRect);

     if (FocusOpt) and (Screen.ActiveControl = Self) then
       TSpkCtrlsAux.DrawControlLineFocus(Self, ABuffer, FTextRect.Left, FTextRect.Bottom, FTextRect.Right, fontColor);

   end;

  TSpkCtrlsAux.DrawDropdownArrow(ABuffer,
                                 Classes.Rect(FRect.Right-SmallButtonDropdownWidth,FRect.Top,FRect.Right,FRect.Bottom),
                                 fontColor);
end;

procedure TCustomSpkCombobox.SetAllowAdd(AValue: Boolean);
begin
  FAllowAdd := AValue;
  UpdateAppearance();
end;

procedure TCustomSpkCombobox.SetItemIndex(AValue: Integer);
begin
  try
    if (AValue > -1) and (AValue < FItems.Count) then
      Self.Caption := FItems.Strings[AValue];
  finally
    DoUpdateItem(AValue);
  end;
end;

function TCustomSpkCombobox.GetItems(): TStrings;
begin
  Result := FItems;
end;

procedure TCustomSpkCombobox.SetItems(const AValue: TStrings);
begin
  FItems.Assign(AValue);
  if not (csLoading in Self.ComponentState) then
    begin
      FItemIndex   := -1;
      Self.Caption := '';
    end;
end;

procedure TCustomSpkCombobox.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if (Key = 43) and FAllowAdd then
    Self.DoUserAdd()
  else
  if (Key = SPK_ACTIVATE_KEY) or (Key = SPK_SELECT_KEY) or (Key = SPK_ENTER_KEY) then
    Self.DoPopupCtrl();
end;


procedure TCustomSpkCombobox.DoUserAdd();
begin
  if Assigned(FOnUserAdd) then
    FOnUserAdd(Self);
end;

procedure TCustomSpkCombobox.DoPopupCtrl();
begin
  Sleep(100);
  if (FItems.Count > 0) then
    BeginPopup();
end;

procedure TCustomSpkCombobox.DoUpdateItem(const ANewItem: Integer);
begin
  FItemIndex := ANewItem;

  if Assigned(FOnChangeItem) then
    FOnChangeItem(Self);
end;

function TCustomSpkCombobox.CreateStrings(): TStrings;
begin
  Result := TStringList.Create();
end;

function TCustomSpkCombobox.CreateListItemsControl(APopup: TWinControl): TWinControl;
begin
  Result := TListBox.Create(APopup);
  with (Result as TListBox) do
    begin
      Parent       := APopup;
      OnKeyDown    := @ListKeyDown;
      OnMouseMove  := @ListMouseMove;
      OnMouseUp    := @ListMouseUp;
      OnDrawItem   := @ListDrawItem;
    end;
end;

procedure TCustomSpkCombobox.InitPopupItemIndex();
begin
  with (FListControl as TListBox) do
    begin
      Align   := alClient;
      ItemHeight:= PaneRowHeight + 2;
      Options := [];
      BorderStyle := bsNone;
      Style := lbOwnerDrawFixed;
      Color := FToolbar.Appearance.Element.IdleFrameColor;
      Font  := FToolbar.Appearance.Element.CaptionFont;
      Items.Assign(Self.FItems);
      ItemIndex := Self.ItemIndex;
    end;
end;

function TCustomSpkCombobox.CalcListHeight(var AX, AY: Integer): Integer;
var
  P: TPoint;
  L: Integer;
begin
  //
  //    NOTE: We add 2 pixels per item for perfect fit.
  //
  L := (FItems.Count * PaneRowHeight) + (FItems.Count * 2);

  if (FMaxListHeight > 0) then
    begin
      if (L > FMaxListHeight) then
        Result := FMaxListHeight
      else
        Result := L;
    end
  else
    Result := L;

  if (FMaxListHeight > 0) and (Result = FMaxListHeight) then
    P := Self.ControlToScreen(Point(0, 0))
  else
    P := Self.ControlToScreen(Point(0, -(FItemIndex * PaneRowHeight)));

  AX := P.X;
  AY := P.Y;
end;

(*
    PUBLIC
*)
constructor TCustomSpkCombobox.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  FMaxListHeight:= 0;
  FAllowAdd     := True;
  FPushed       := False;
  FOnUserAdd    := nil;
  FOnChangeItem := nil;
  FItemIndex    := -1;
  FItems        := Self.CreateStrings();
  AccessibleRole:= larComboBox;

  Constraints.SetInterfaceConstraints(PaneRowHeight *4, PaneRowHeight, 0, PaneRowHeight);
end;

destructor TCustomSpkCombobox.Destroy();
begin
  if (FItems <> nil) then
    FItems.Destroy();

  inherited Destroy();
end;

function TCustomSpkCombobox.IsPushed(): Boolean;
begin
  Result := FPushed;
end;

procedure TCustomSpkCombobox.Assign(Source: TPersistent);
begin
  inherited Assign(Source);

  if Source is TCustomSpkCombobox then
    begin
      FMaxListHeight  := TCustomSpkCombobox(Source).MaxListHeight;
      FAllowAdd       := TCustomSpkCombobox(Source).AllowAdd;
      FItemIndex      := TCustomSpkCombobox(Source).ItemIndex;
      FItems.Assign(TCustomSpkCombobox(Source).Items);
    end;
end;

procedure TCustomSpkCombobox.Clear();
begin
  FItems.Clear();
  FItemIndex  := -1;
  Self.Caption:= '';
end;


procedure TCustomSpkCombobox.DoOnClosedPopup(Sender: TObject);
begin
  FPushed:= False;
  FButtonState := bsIdle;
  InvalidateBuffer();
  Repaint();
end;

procedure TCustomSpkCombobox.BeginPopup();
var
  LX,LY,LH: Integer;
begin
  if (PopupFrm <> nil) then
    PopupFrm.Destroy();

  PopupFrm := TSpkPopupWindow.CreateNew(nil);
  try
    FListControl := Self.CreateListItemsControl(PopupFrm);
    Self.InitPopupItemIndex();
    LH := Self.CalcListHeight(LX, LY);
    PopupFrm.SetBounds(LX, LY, Self.Width, LH);
    TSpkPopupWindow(PopupFrm).Caller := Self;
    FPushed:= True;
  finally
    TSpkPopupWindow(PopupFrm).ShowMe(@DoOnClosedPopup, True);
  end;
end;

procedure TCustomSpkCombobox.EndPopup();
begin
  FPushed := False;
  TSpkPopupWindow(PopupFrm).Finishit();
end;

(*
    STRICT PRIVATE
*)
procedure TCustomSpkCombobox.ListKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
  if (Key = VK_SPACE) or (Key = VK_RETURN) then
    begin
      Key := 0;
      Self.ListMouseUp(Self, mbLeft, [], 0,0);
    end;
end;

procedure TCustomSpkCombobox.ListMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
begin
  TListBox(FListControl).ItemIndex := TListBox(FListControl).GetIndexAtXY(X, Y);
end;

procedure TCustomSpkCombobox.ListMouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  try
    FButtonState   := bsBtnHottrack;
    Self.ItemIndex := TListBox(FListControl).ItemIndex;
    if Assigned(FOnUserClick) then
      FOnUserClick(Self);
  finally
    Self.EndPopup();
  end;
end;

procedure TCustomSpkCombobox.ListDrawItem(Control: TWinControl; Index: Integer; ARect: TRect; State: TOwnerDrawState);
begin
  with TListBox(Control) do
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

      if (Index = FItemIndex) then
        Canvas.Font.Color := FToolbar.Appearance.Element.ActiveCaptionColor;

      Canvas.Pen.Style := psClear;
      Canvas.FillRect(ARect);
      Canvas.TextRect(ARect, ARect.Left + SmallButtonBorderWidth,
                      ARect.Top + (ARect.Height - Canvas.TextHeight('Wy')) div 2,
                      Items[Index]);
    end;
end;

