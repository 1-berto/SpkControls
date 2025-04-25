{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkUpDown
*)

(*
    PROTECTED
*)
function TCustomSpkUpDown.DoMouseWheelUp(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  Result := inherited DoMouseWheelUp(Shift, MousePos);
  FPressed := False;
  UpButtonTrigger(Self);
end;

function TCustomSpkUpDown.DoMouseWheelDown(Shift: TShiftState; MousePos: TPoint): Boolean;
begin
  Result := inherited DoMouseWheelDown(Shift, MousePos);
  FPressed := False;
  DownButtonTrigger(Self);
end;

function TCustomSpkUpDown.AllowUserChange(const ANewValue: Single): Boolean;
begin
  Result := True;
end;

procedure TCustomSpkUpDown.DispatchMouseDown(Shift: TShiftState; X, Y: Integer);
begin
  FPressed  := True;
  FUpPressed:= FUpRect.Contains(X, Y);

  if FUpPressed then
    UpButtonTrigger(Self)
  else
    DownButtonTrigger(Self);
end;

procedure TCustomSpkUpDown.DispatchMouseUp(Shift: TShiftState; X, Y: Integer);
begin
  FPressed := False;
  FreeAndNil(UpDownTimer);
end;

procedure TCustomSpkUpDown.UpButtonTrigger(Sender: TObject);
begin
  if not AllowUserChange(FCurrValue + FIncValue) then
    Exit;

  if (FCurrValue < FMaxValue) then
    begin
      SetCurrValue(FCurrValue + FIncValue);

      if Assigned(FOnUserChange) then
        FOnUserChange(Self);
    end;

  if FPressed and (UpDownTimer = nil) then
    GetUpDownRepeaterTimer(Self, @UpButtonTrigger).Enabled := True;
end;

procedure TCustomSpkUpDown.DownButtonTrigger(Sender: TObject);
begin
  if not AllowUserChange(FCurrValue - FIncValue) then
    Exit;

  if (FCurrValue > FMinValue) then
    begin
      SetCurrValue(FCurrValue - FIncValue);

      if Assigned(FOnUserChange) then
        FOnUserChange(Self);
    end;

  if FPressed and (UpDownTimer = nil) then
    GetUpDownRepeaterTimer(Self, @DownButtonTrigger).Enabled := True;
end;

procedure TCustomSpkUpDown.SetCurrValue(const AValue: Single);
begin
  if (AValue < FMinValue) then
    FCurrValue := FMinValue
  else
  if (AValue > FMaxValue) then
    FCurrValue := FMaxValue
  else
    FCurrValue := AValue;

  if Assigned(FOnAftChange) then
    FOnAftChange(Self);

  if (FAssociate <> nil) then
    UpdateAssociateControl();
end;

function TCustomSpkUpDown.GetIntPosition(): Integer;
begin
  Result := Round(FCurrValue);
end;

procedure TCustomSpkUpDown.SetIntPosition(const AValue: Integer);
begin
  if (AValue < FMinValue) then
    FCurrValue := FMinValue
  else
  if (AValue > FMaxValue) then
    FCurrValue := FMaxValue
  else
    FCurrValue := AValue;

  if Assigned(FOnAftChange) then
    FOnAftChange(Self);

  if (FAssociate <> nil) then
    UpdateAssociateControl();
end;

procedure TCustomSpkUpDown.UpdateAssociateControl();
begin
  if not (csDesigning in ComponentState) and (FAssociate <> nil) then
    begin
      if FThousands then
        FAssociate.Caption := FloatToStrF(FCurrValue, ffNumber, 0, 0)
      else
        FAssociate.Caption := IntToStr(Integer(FCurrValue));
    end;
end;

function TCustomSpkUpDown.CalcRect(): T2DIntRect;
var
  Mid: Integer;
begin
  Result := inherited CalcRect();
  Mid := FRect.Bottom div 2;
{$IFDEF EnhancedRecordSupport}
  FUpRect  := T2DIntRect.Create(0, 0, FRect.Width-1, Mid);
  FDownRect:= T2DIntRect.Create(0, Mid, FRect.Width-1, Mid);
{$ELSE}
  FUpRect.Create(0, 0, FRect.Width-1, Mid);
  FDownRect.Create(0, Mid, FRect.Width-1, FRect.Bottom);
{$ENDIF}
end;

procedure TCustomSpkUpDown.DoDraw(const ABuffer: TBitmap);
var
  sepColor,
  fontColor: TColor;
  frameColor, innerLightColor, innerDarkColor: TColor;
  gradientFromColor, gradientToColor: TColor;
  gradientKind: TBackgroundKind;
  delta, cornerRadius: Integer;
  ClipRect: T2DIntRect;
begin
  inherited DoDraw(ABuffer);

  if (FToolbar = nil) or (FToolbar.Appearance = nil) then
    Exit;

  ClipRect := FRect;

  delta := FToolbar.Appearance.Element.HotTrackBrightnessChange;
  case FToolbar.Appearance.Element.Style of
    esRounded:
      cornerRadius := SmallButtonRadius;
    esRectangle:
      cornerRadius := 0;
  end;

  FToolbar.Appearance.Element.GetActiveColors(True,
   fontColor, frameColor, innerLightColor, innerDarkColor,
   gradientFromColor, gradientToColor, gradientKind,
   delta
  );

  if not Self.Enabled then
    fontColor := TColorTools.ColorToGrayscale(fontColor);

  if FPressed then
    begin
      if FUpPressed then
        TButtonTools.DrawButton(
          ABuffer,
          FUpRect,
          frameColor,
          innerLightColor,
          innerDarkColor,
          gradientFromColor,
          gradientToColor,
          gradientKind,
          (FSideAlign=saLeft),
          (FSideAlign=saRight),
          false,
          true,
          cornerRadius,
          ClipRect
        )
      else
        TButtonTools.DrawButton(
          ABuffer,
          FDownRect,
          frameColor,
          innerLightColor,
          innerDarkColor,
          gradientFromColor,
          gradientToColor,
          gradientKind,
          (FSideAlign=saLeft),
          (FSideAlign=saRight),
          true,
          false,
          cornerRadius,
          ClipRect
        );
    end;

  SepColor := FToolbar.Appearance.Pane.BorderDarkColor;
  TGUITools.DrawHLine(ABuffer,2 , FRect.Width-3, (FRect.Height+1) div 2, SepColor);

  TSpkCtrlsAux.DrawUpAndDownArrow(ABuffer, FUpRect.ForWinAPI, fontColor, True);
  TSpkCtrlsAux.DrawUpAndDownArrow(ABuffer, FDownRect.ForWinAPI, fontColor, False);
end;

procedure TCustomSpkUpDown.Notification(AComponent: TComponent; Operation: TOperation);
begin
  inherited Notification(AComponent, Operation);

  if (Operation = opRemove) and (AComponent = FAssociate) then
    SetAssociate(nil);
end;

procedure TCustomSpkUpDown.SetAssociate(const ACtrl: TControl);
begin
  inherited SetAssociate(ACtrl);

  if (ACtrl <> nil) then
    begin
      if not (ACtrl is ISpkControl) then
        FSideAlign := saNone
      else
      if Self.IsRightToLeft then
        begin
          FSideAlign := saRight;
          SetBounds(ACtrl.Left - Self.Width, ACtrl.Top, Width, Height);
        end
      else
        begin
          FSideAlign := saLeft;
          SetBounds(ACtrl.Left + ACtrl.Width, ACtrl.Top, Width, Height);
        end;

      if not (csLoading in Self.ComponentState) then
        Self.UpdateAppearance();
    end;
end;

(*
    PUBLIC
*)
constructor TCustomSpkUpDown.Create(AOwner: TComponent);
begin
  inherited      Create(AOwner);
  ControlStyle   := ControlStyle - csMultiClicks;
  FThousands     := True;
  FPressed       := False;
  FOnUserChange  := nil;
  FOnAftChange   := nil;
  FInterval      := 200;//150;
  FIncValue      := 1;
  FMinValue      := 0;
  FMaxValue      := 100;
  FCurrValue     := 0;
  AccessibleRole := larSpinner;

  Constraints.SetInterfaceConstraints(PaneRowHeight, PaneRowHeight, PaneRowHeight, PaneRowHeight);
  SetBounds(Left, Top, PaneRowHeight, PaneRowHeight);
end;

