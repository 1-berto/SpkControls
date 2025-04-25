{%MainUnit SpkCtrls.pp}

(*
    TSpkCtrlsAux
*)
class
procedure TSpkCtrlsAux.DrawControlFocus(const AControl: TControl; const ABitmap: TBitmap; const AColor: TColor);
begin
  ABitmap.Canvas.Pen.Color := AColor;
  ABitmap.Canvas.Pen.Style := FocusStyle;
  ABitmap.Canvas.Frame(AControl.ClientRect);
end;

class
procedure TSpkCtrlsAux.DrawControlCustomFocus(const AControl: TControl; const ABitmap: TBitmap;
  const ARect: TRect; const AColor: TColor);
begin
  ABitmap.Canvas.Pen.Color := AColor;
  ABitmap.Canvas.Pen.Style := FocusStyle;
  ABitmap.Canvas.Frame(ARect);
end;

class
procedure TSpkCtrlsAux.DrawControlLineFocus(const AControl: TControl; const ABitmap: TBitmap;
  const AX, AY, AWidth: Integer; const AColor: TColor);
begin
  ABitmap.Canvas.Pen.Color := AColor;
  ABitmap.Canvas.Pen.Style := FocusStyle;
  ABitmap.Canvas.Line(AX, AY, AWidth, AY);
end;

class
procedure TSpkCtrlsAux.DrawCrossIcon(ARect: TRect; const ABitmap: TBitmap; const AColor: TColor);
const
  Pad = 5;
begin
  ABitmap.Canvas.Pen.Width := 3;
  ABitmap.Canvas.Pen.Color := AColor;
  ABitmap.Canvas.Pen.Mode  := pmCopy;
  ABitmap.Canvas.Pen.Style := psSolid;
  ABitmap.Canvas.MoveTo(ARect.Left + Pad, ARect.Top + Pad);
  ABitmap.Canvas.LineTo(ARect.Right - Pad, ARect.Bottom - Pad);
  ABitmap.Canvas.MoveTo(ARect.Right - Pad, ARect.Top + Pad);
  ABitmap.Canvas.LineTo(ARect.Left + Pad, ARect.Bottom - Pad);
end;

class
procedure TSpkCtrlsAux.DrawStyledBorder(const ARect: T2DIntRect; const ABitmap: TBitmap; ABorder: TSpkStyledBorder;
                                        const ACornerRadius: Integer; const ABorderDarkColor, ABorderLightColor: TColor);
var
  C : TColor;
  CR,
  R : T2DIntRect;
begin
  CR := ARect;
  if (ABorder <> sbNone) then
    begin
      if (ABorder = sbFlat) then
        begin
        {$IFDEF EnhancedRecordSupport}
          R := T2DIntRect.Create(
        {$ELSE}
          R := Create2DIntRect(
        {$ENDIF}
            ARect.Left,
            ARect.Top,
            ARect.Right-1,
            ARect.bottom-1
          );
          TGUITools.DrawAARoundFrame(
            ABitmap,
            R,
            ACornerRadius,
            ABorderDarkColor,
            CR
          );
        end
      else
        begin
        {$IFDEF EnhancedRecordSupport}
          R := T2DIntRect.Create(
        {$ELSE}
          R := Create2DIntRect(
        {$ENDIF}
            ARect.Left + 1,
            ARect.Top + 1,
            ARect.Right -1,
            ARect.bottom -1
          );

          if (ABorder = sbEtched) then
            C := ABorderLightColor
          else
            C := ABorderDarkColor;

          TGUITools.DrawAARoundFrame(
            ABitmap,
            R,
            ACornerRadius,
            C,
            CR
          );

        {$IFDEF EnhancedRecordSupport}
          R := T2DIntRect.Create(
        {$ELSE}
          R := Create2DIntRect(
        {$ENDIF}
            ARect.Left,
            ARect.Top,
            ARect.Right-2,
            ARect.Bottom-2
          );

          if (ABorder = sbEtched) then
            C := ABorderDarkColor
          else
            C := ABorderLightColor;

          TGUITools.DrawAARoundFrame(
            ABitmap,
            R,
            ACornerRadius,
            C,
            CR
          );
        end;
     end;
end;

class
procedure TSpkCtrlsAux.DrawStyledChar(const ARect: T2DIntRect; const ABitmap: TBitmap;
                                      const AFont: TFont; const AColor: TColor; const AChr: String);
var
  X, Y: Integer;
begin
  ABitmap.Canvas.Font.Assign(AFont);
  ABitmap.Canvas.Font.Style := ABitmap.Canvas.Font.Style + [fsBold];
  X := ARect.Left + (ARect.Width - ABitmap.Canvas.TextWidth(AChr)) div 2;
  Y := ARect.Bottom - PaneBorderSize - PaneCaptionHeight + 2 + (PaneCaptionHeight-ABitmap.Canvas.TextHeight('Wy')) div 2;

  TGUITools.DrawText(ABitmap.Canvas, X, Y, AChr, AColor, ARect);
end;

class
procedure TSpkCtrlsAux.DrawDropdownArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor);
var
  P: array[0..3] of TPoint;
begin
  P[2].x := ARect.Left + (ARect.Right - ARect.Left) div 2;
  P[2].y := ARect.Top + (ARect.Bottom - ARect.Top + DropDownArrowHeight) div 2 - 1;
  P[0] := Point(P[2].x - DropDownArrowWidth div 2, P[2].y - DropDownArrowHeight div 2);
  P[1] := Point(P[2].x + DropDownArrowWidth div 2, P[0].y);
  P[3] := P[0];

  ABitmap.Canvas.Brush.Color := AColor;
  ABitmap.Canvas.Pen.Style := psClear;
  ABitmap.Canvas.Polygon(P);
end;

class
procedure TSpkCtrlsAux.DrawUpArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor);
var
  P: array[0..3] of TPoint;
begin
  P[2].x := ARect.Left + (ARect.Right - ARect.Left) div 2;
  P[2].y := ARect.Bottom - (DropDownArrowHeight) div 2 - 1;
  P[0] := Point(P[2].x - DropDownArrowWidth div 2, P[2].y - DropDownArrowHeight div 2);
  P[1] := Point(P[2].x + DropDownArrowWidth div 2, P[0].y);
  P[3] := P[0];

  ABitmap.Canvas.Brush.Color := AColor;
  ABitmap.Canvas.Pen.Style := psClear;
  ABitmap.Canvas.Polygon(P);
end;

class
procedure TSpkCtrlsAux.DrawUpAndDownArrow(const ABitmap: TBitmap; ARect: TRect; const AColor: TColor; AIsUp: Boolean; const ADisp: Integer = 0);
var
  P: array[0..3] of TPoint;
begin
  if (ADisp <> 0) then
    begin
      ARect.Top += 2;
      ARect.Left+= 2;
    end;

  P[2].x := ARect.Left + (ARect.Right - ARect.Left) div 2;
  if AIsUp then
    begin
      P[2].y := ARect.Top + (DropDownArrowHeight) div 2 -1;
      P[0] := Point(P[2].x - DropDownArrowWidth div 2 -1, P[2].y + DropDownArrowHeight div 2 +1);
      P[1] := Point(P[2].x + DropDownArrowWidth div 2, P[0].y);
    end
  else
    begin
      P[2].y := ARect.Bottom - (DropDownArrowHeight) div 2;
      P[0] := Point(P[2].x - DropDownArrowWidth div 2, P[2].y - DropDownArrowHeight div 2);
      P[1] := Point(P[2].x + DropDownArrowWidth div 2, P[0].y);
    end;
  P[3] := P[0];

  ABitmap.Canvas.Brush.Color := AColor;
  ABitmap.Canvas.Pen.Style := psClear;
  ABitmap.Canvas.Polygon(P);
end;

