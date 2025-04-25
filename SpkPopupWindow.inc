{%MainUnit SpkCtrls.pp}

(*
    TSpkPopupWindow
*)

(*
    PROTECTED
*)
procedure TSpkPopupWindow.Deactivate;
begin
  inherited Deactivate;
  if FAutoDestroy then
    begin
      PopupFrm := nil;
      Self.Destroy();
    end
  else
    Self.Hide;
end;

(*
    PUBLIC
*)
constructor TSpkPopupWindow.CreateNew(AOwner: TComponent; Num: Integer=0);
begin
  inherited CreateNew(AOwner,Num);
  Visible      := False;
  ShowInTaskBar:= stNever;
  FormStyle    := fsStayOnTop;
  BorderStyle  := bsNone;
  PopupFrm     := Self;
end;

procedure TSpkPopupWindow.ShowAt(const AX, AY: Integer; AAutoDestroy: Boolean = False);
begin
  FAutoDestroy := AAutoDestroy;
  Self.SetBounds(AX, AY, Width, Height);
  Self.ShowOnTop;
end;

procedure TSpkPopupWindow.ShowMe(ADeactiveEvent: TNotifyEvent; AAutoDestroy: Boolean = False);
begin
  FAutoDestroy := AAutoDestroy;
  OnDeactivate := ADeactiveEvent;
  Self.ShowOnTop;
end;

procedure TSpkPopupWindow.Finishit();
begin
  Self.Deactivate;
end;      

