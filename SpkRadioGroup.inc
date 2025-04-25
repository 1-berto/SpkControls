{%MainUnit SpkCtrls.pp}

(*
    TCustomSpkRadioGroup
*)

(*
    PROTECTED
*)
procedure TCustomSpkRadioGroup.Loaded;
begin
  inherited Loaded();
  UpdateItems();
end;

function TCustomSpkRadioGroup.AllowChangeIndex(): Boolean;
begin
  Result := True;
end;

procedure TCustomSpkRadioGroup.NotifyItemIndexChange();
begin
  if Assigned(FOnSChanged) then
    FOnSChanged(Self);
end;

function TCustomSpkRadioGroup.NewItemsList(): TStrings;
begin
  Result := TStringList.Create;
  TStringList(Result).OwnsObjects := False;
end;

function TCustomSpkRadioGroup.NewItem(const ACaption: TCaption; AIdx: Integer): TCustomSpkRadioCheck;
begin
  Result := TCustomSpkRadioCheck.Create(Self);
  with Result do
    begin
      Parent:= Self;
      Name  := 'GroupedRadioButton' + IntToStr(AIdx);
      ControlStyle:= ControlStyle + [csNoDesignSelectable];
      OnUserClick := @DoRadioItemUserClick;
      OnEnter := @DoRadioItemEnter;
      OnExit  := @DoRadioItemExit;
      UserCheckSwapEvent := @ItemUserCheckSwap;
      SpkToolbar := Self.SpkToolbar;
      Caption    := ACaption;
      Tag := AIdx;
    end;
end;

procedure TCustomSpkRadioGroup.UpdateItems();
var
  KeepIdx: Boolean;
  I: Integer;
  R: TCustomSpkRadioCheck;
begin
  KeepIdx := (FItemIndex > -1);

  for I := Self.ControlCount -1 downto 0 do
    Self.Controls[I].Destroy();
        
  for I := 0 to FItems.Count -1 do
    if (FItems.Strings[I] <> '') then
      begin
        R := TCustomSpkRadioCheck(Self.NewItem(FItems.Strings[I], I));
        FItems.Objects[I] := R;
        
        if KeepIdx and (I = FItemIndex) then
          if AllowChangeIndex() then
            begin
              R.FCheckState := cbChecked;
            end;
      end;

  if not KeepIdx then      
    FItemIndex := -1;
end;

procedure TCustomSpkRadioGroup.SetItems(const AItems: TStrings);
begin
  FItems.Assign(AItems);
  UpdateItems(); 
end;

function TCustomSpkRadioGroup.GetItemIndex(): Integer;
begin
  Result := FItemIndex;
end;

procedure TCustomSpkRadioGroup.SetItemIndex(const AValue: Integer);
begin
  if not AllowChangeIndex() then
    Exit;

  FItemIndex := AValue;
  if (FItemIndex > -1) and (FItems.Count > 0) then
    begin
      TCustomSpkRadioCheck(FItems.Objects[FItemIndex]).Checked := True;
      NotifyItemIndexChange();
    end;
end;

function TCustomSpkRadioGroup.GetColumns(): Integer;
begin
  Result := Self.ChildSizing.ControlsPerLine;
end;

procedure TCustomSpkRadioGroup.SetColumns(const AValue: Integer);
begin
  Self.ChildSizing.ControlsPerLine := Max(AValue, 1);
end;

(*
    PRIVATE
*)
procedure TCustomSpkRadioGroup.DoRadioItemEnter(Sender: TObject);
begin
  if Assigned(FOnItemEnter) then
    FOnItemEnter(Sender);
end;

procedure TCustomSpkRadioGroup.DoRadioItemExit(Sender: TObject);
begin
  if Assigned(FOnItemExit) then
    FOnItemExit(Sender);
end;

procedure TCustomSpkRadioGroup.DoRadioItemUserClick(Sender: TObject);
begin    
  SetItemIndex(TCustomSpkRadioCheck(Sender).Tag);
  
  if Assigned(FOnUserClick) then
    FOnUserClick(Sender);
end;

function TCustomSpkRadioGroup.ItemUserCheckSwap(const AChecked: Boolean): Boolean;
begin
  if FAllowUnsel then
    Result := not AChecked
  else
  //Prevents the user unselect by click.
    Result := True;
end;

(*
    PUBLIC
*)
constructor TCustomSpkRadioGroup.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  ControlStyle:= ControlStyle - [csAcceptsControls];
  FAllowUnsel := False;
  FItemIndex  := -1;
  FItems      := NewItemsList();
  FOnUserClick:= nil;
  FOnItemEnter:= nil;
  FOnItemExit := nil;
  FOnSChanged := nil;

  with ChildSizing do
    begin
      Layout           := cclLeftToRightThenTopToBottom;
      ControlsPerLine  := 1;
      TopBottomSpacing := PaneCaptionHeight + (PaneCaptionHeight div 2) + PaneBorderSize;
      EnlargeHorizontal:= crsHomogenousChildResize;
      EnlargeVertical  := crsHomogenousChildResize;
      ShrinkHorizontal := crsScaleChilds;
      ShrinkVertical   := crsScaleChilds;
      LeftRightSpacing := PaneCaptionHeight div 2;
    end;
end;

destructor TCustomSpkRadioGroup.Destroy();
begin
  if (FItems <> nil) then
    FItems.Destroy();
  inherited Destroy();
end;

