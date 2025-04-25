{%MainUnit SpkCtrls.pp}

(*
    Internal options:
*)
var
  FocusOpt   : Boolean = True;   ///Define True if the controls must draw the visual focus.
  BtnFocusOpt: Boolean = True;   ///Define True if the buttons must draw the underline visual focus.
  MouseClick : TMouseButton = mbLeft; ///Define the Mouse button for click action.
  FocusStyle : TFPPenStyle = Graphics.psSolid; ///Define the kind of pen style for focus.

