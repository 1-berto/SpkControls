{%MainUnit SpkCtrls.pp}

(*
    TCommonSpkContainer
*)

(*
    PUBLIC
*)
procedure TCommonSpkContainer.InsertControl(AControl: TControl; Index: integer);
begin
  inherited InsertControl(AControl, Index);
  if (AControl is ISpkControl) then
    begin
      (AControl as ISpkControl).SetToolbar(Self.FToolbar);
    end;
end;

(*
    TCustomSpkBackground
*)

(*
    PUBLIC
*)

(*
    PROTECTED
*)

procedure TCustomSpkBackground.DoDraw(const ABuffer: TBitmap);
begin
  if (FToolbar = nil) then
    Exit;

  with FToolbar.Appearance do
    if (Pane.GradientType = bkSolid) then
      TGUITools.RenderBackground(ABuffer, FRect, Pane.GradientFromColor, Pane.GradientToColor, bkSolid)
    else
      TGUITools.RenderBackground(ABuffer, FRect, Pane.GradientFromColor, Pane.GradientToColor, bkVerticalGradient);
end;

