unit MultiRBTree;

interface

uses
  Classes, RBTree;

type
  TStrSortCompare = function (const Item1: String; const Item2: String): Integer;
  TMultiColor = (clRed, clBlack);

  TMultiRBNodeP = ^TMultiRBNode;
  TMultiRBNode = record
    Key: String;
    Left, Right, Parent: TMultiRBNodeP;
    Color: TMultiColor;
    Duplicates: TRBTree;
//    Data: Pointer;
  end;

  TMultiRBTree = class
	private
  	Root: TMultiRBNodeP;
    Leftmost: TMultiRBNodeP;
    Rightmost: TMultiRBNodeP;
    CompareFunc: TStrSortCompare;
    procedure RotateLeft(var x: TMultiRBNodeP);
    procedure RotateRight(var x: TMultiRBNodeP);
    function Minimum(var x: TMultiRBNodeP): TMultiRBNodeP;
    function Maximum(var x: TMultiRBNodeP): TMultiRBNodeP;
	public
  	constructor Create;
    destructor Destroy; override;
    procedure Clear;
    procedure Delete(Z: TMultiRBNodeP; Data: Pointer);
    function Find(Key: String): TMultiRBNodeP;
    function Add(Key: String; Data: Pointer): TMultiRBNodeP;
  public
    property First: TMultiRBNodeP read leftmost;
    property Last: TMultiRBNodeP read rightmost;
	end;

  procedure RBInc(var x: TMultiRBNodeP);  
implementation

procedure RBInc(var x: TMultiRBNodeP);
var
  y: TMultiRBNodeP;
begin
  if (x^.Right <> nil) then
  begin
    x := x^.Right;
    while (x^.Left <> nil) do
    begin
      x := x^.Left;
    end;
  end
  else
  begin
    y := x^.Parent;
    while (x = y^.Right) do
    begin
      x := y;
      y := y^.Parent;
    end;
    if (x^.Right <> y) then
      x := y;
  end
end;

{ Pre: x <> first }
procedure RBDec(var x: TMultiRBNodeP);
var
  y: TMultiRBNodeP;
begin
  if (x^.left <> nil) then
  begin
    y := x^.Left;
    while (y^.Right <> nil) do
    begin
      y := y^.Right;
    end;
    x := y;
  end
  else
  begin
    y := x^.Parent;
    while (x = y^.Left) do
    begin
      x := y;
      y := y^.Parent;
    end;
    x := y;
  end
end;

function CompareStr(const Item1: String; const Item2: String): Integer;
begin
  if (Item1 < Item2) then begin
    Result := -1;
  end
  else if (Item1 = Item2) then
  begin
    Result := 0;
  end
  else
  begin
    Result := 1;
  end
end;

constructor TMultiRBTree.Create;
begin
  inherited Create;
  CompareFunc := CompareStr;
  Root := nil;
  Leftmost  := nil;
  Rightmost := nil;
end;

destructor TMultiRBTree.Destroy;
begin
  Clear;
  inherited Destroy;
end;

procedure fast_erase(x: TMultiRBNodeP);
begin
  if (x^.Left <> nil) then  fast_erase(x^.Left);
  if (x^.Right <> nil) then fast_erase(x^.Right);
  x^.Duplicates.Free;
  dispose(x);
end;

procedure TMultiRBTree.Clear;
begin
  if (Root <> nil) then
    fast_erase(Root);
  Root := nil;
  Leftmost  := nil;
  Rightmost := nil;
end;

function TMultiRBTree.Find(Key: String): TMultiRBNodeP;
var
  cmp: integer;
begin
  Result := Root;
  while (Result <> nil) do
  begin
    cmp := CompareFunc(Result^.Key, key);
    if cmp < 0 then
    begin
      Result := Result^.right;
    end
    else if cmp > 0 then
    begin
      Result := Result^.left;
    end
    else
    begin
      break;
    end;
  end;
end;

procedure TMultiRBTree.RotateLeft(var x: TMultiRBNodeP);
var
  y: TMultiRBNodeP;
begin
  y := x^.right;
  x^.right := y^.left;
  if (y^.left <> nil) then
  begin
    y^.left^.parent := x;
  end;
  y^.parent := x^.parent;
  if (x = Root) then
  begin
    Root := y;
  end
  else if (x = x^.parent^.left) then
  begin
    x^.parent^.left := y;
  end
  else
  begin
    x^.parent^.right := y;
  end;
  y^.left := x;
  x^.parent := y;
end;

procedure TMultiRBTree.RotateRight(var x: TMultiRBNodeP);
var
  y: TMultiRBNodeP;
begin
  y := x^.left;
  x^.left := y^.right;
  if (y^.right <> nil) then
  begin
    y^.right^.parent := x;
  end;
  y^.parent := x^.parent;
  if (x = Root) then
  begin
    Root := y;
  end
  else if (x = x^.parent^.right) then
  begin
    x^.parent^.right := y;
  end
  else
  begin
    x^.parent^.left := y;
  end;
  y^.right := x;
  x^.parent := y;
end;

function TMultiRBTree.Minimum(var x: TMultiRBNodeP): TMultiRBNodeP;
begin
  Result := x;
  while (Result^.left <> nil) do
    Result := Result^.left;
end;

function TMultiRBTree.Maximum(var x: TMultiRBNodeP): TMultiRBNodeP;
begin
  Result := x;
  while (Result^.right <> nil) do
    Result := Result^.right;
end;

function TMultiRBTree.Add(Key: String; Data: Pointer): TMultiRBNodeP;
var
  x, y, z, zpp: TMultiRBNodeP;
  cmp: Integer;
begin
  z := New(TMultiRBNodeP);
  { Initialize fields in new node z }
  z^.Key 		:= key;
  z^.Left 	:= nil;
  z^.Right 	:= nil;
  z^.Color 	:= clRed;
  z^.Duplicates := TRBTree.Create;
  z^.Duplicates.Add(data, data);
  Result := z;
  
  { Maintain leftmost and rightmost nodes }
  if ((leftmost = nil) or (compareFunc(key, leftmost^.Key) < 0)) then
  begin
    leftmost := z;
  end;

  if ((rightmost = nil) or (compareFunc(rightmost^.Key, key) < 0)) then
  begin
    rightmost := z;
  end;
  
  { Insert node z }
  y := nil;
  x := Root;
  while (x <> nil) do
  begin
    y := x;
    cmp := compareFunc(key, x^.Key);
    if (cmp < 0) then
    begin
      x := x^.left;
    end
    else if (cmp > 0) then
    begin
      x := x^.right;
    end
    else
    begin
      { Value already exists in tree. }
      x^.Duplicates.Add(data, data);
      Result := x;

      z^.Duplicates.Free;
      dispose(z);
      exit;
    end;
  end;

  z^.parent := y;
  if (y = nil) then
  begin
    Root := z;
  end
  else if (compareFunc(key, y^.Key) < 0) then
  begin
    y^.left := z;
  end
  else
  begin
    y^.right := z;
  end;

  { Rebalance tree }
  while ((z <> Root) and (z^.parent^.color = clRed)) do
	begin
  	zpp := z^.parent^.parent;
    if (z^.parent = zpp^.left) then
    begin
      y := zpp^.right;
      if ((y <> nil) and (y^.color = clRed)) then
      begin
        z^.parent^.color := clBlack;
        y^.color := clBlack;
        zpp^.color := clRed;
      	z := zpp;
      end
      else
      begin
        if (z = z^.parent^.right) then
        begin
          z := z^.parent;
          rotateLeft(z);
        end;
        z^.parent^.color := clBlack;
        zpp^.color := clRed;
        rotateRight(zpp);
      end;
    end
    else
    begin
      y := zpp^.left;
      if ((y <> nil) and (y^.color = clRed)) then
      begin
        z^.parent^.color := clBlack;
        y^.color := clBlack;
        zpp^.color := clRed;
        z := zpp;
      end
      else
      begin
        if (z = z^.parent^.left) then
        begin
          z := z^.parent;
          rotateRight(z);
        end;
        z^.parent^.color := clBlack;
        zpp^.color := clRed;
        rotateLeft(zpp);
      end;
    end;
  end;
  Root^.color := clBlack;
end;

procedure TMultiRBTree.Delete(Z: TMultiRBNodeP; Data: Pointer);
var
  w, x, y, x_parent: TMultiRBNodeP;
  tmpcol: TMultiColor;
begin
	if Z = NIL then Exit;

	if Z^.Duplicates.Count > 1 then
  begin
  	Z^.Duplicates.Delete(Z^.Duplicates.Find(Data));
		Exit;
  end;

  y := z;
  x := nil;
  x_parent := nil;

  if (y^.left = nil) then
  begin    { z has at most one non-null child. y = z. }
    x := y^.right;     { x might be null. }
  end
  else
  begin
    if (y^.right = nil) then
    begin { z has exactly one non-null child. y = z. }
      x := y^.left;    { x is not null. }
    end
    else
    begin
      { z has two non-null children.  Set y to }
      y := y^.right;   {   z's successor.  x might be null. }
      while (y^.left <> nil) do begin
        y := y^.left;
      end;
      x := y^.right;
    end;
  end;

  if (y <> z) then
  begin
    { "copy y's sattelite data into z" }
    { relink y in place of z.  y is z's successor }
    z^.left^.parent := y;
    y^.left := z^.left;
    if (y <> z^.right) then
    begin
      x_parent := y^.parent;
      if (x <> nil) then
      begin
        x^.parent := y^.parent;
      end;
      y^.parent^.left := x;   { y must be a child of left }
      y^.right := z^.right;
      z^.right^.parent := y;
    end
    else
    begin
      x_parent := y;
    end;
    if (Root = z) then
    begin
      Root := y;
    end
    else if (z^.parent^.left = z) then
    begin
      z^.parent^.left := y;
    end else begin
      z^.parent^.right := y;
    end;
    y^.parent := z^.parent;
    tmpcol := y^.color;
    y^.color := z^.color;
    z^.color := tmpcol;
    y := z;
    { y now points to node to be actually deleted }
  end
  else
  begin                        { y = z }
    x_parent := y^.parent;
    if (x <> nil) then
    begin
      x^.parent := y^.parent;
    end;
    if (Root = z) then
    begin
      Root := x;
    end
    else
    begin
      if (z^.parent^.left = z) then
      begin
        z^.parent^.left := x;
      end
      else
      begin
        z^.parent^.right := x;
      end;
    end;
	  if (leftmost = z) then
    begin
	    if (z^.right = nil) then
      begin      { z^.left must be null also }
	      leftmost := z^.parent;
	    end else
      begin
	      leftmost := minimum(x);
      end;
    end;
	  if (rightmost = z) then
    begin
	    if (z^.left = nil) then
      begin       { z^.right must be null also }
	      rightmost := z^.parent;
	    end
      else
      begin                     { x == z^.left }
	      rightmost := maximum(x);
      end;
    end;
  end;

  { Rebalance tree }
  if (y^.color = clBlack) then
  begin
    while ((x <> Root) and ((x = nil) or (x^.color = clBlack))) do
    begin
      if (x = x_parent^.left) then
      begin
          w := x_parent^.right;
          if (w^.color = clRed) then
          begin
            w^.color := clBlack;
            x_parent^.color := clRed;
            rotateLeft(x_parent);
            w := x_parent^.right;
          end;
          if (((w^.left = nil) or
               (w^.left^.color = clBlack)) and
              ((w^.right = nil) or
               (w^.right^.color = clBlack)))  then begin
            w^.color := clRed;
            x := x_parent;
            x_parent := x_parent^.parent;
          end
          else
          begin
            if ((w^.right = nil) or (w^.right^.color = clBlack)) then
            begin
              w^.left^.color := clBlack;
              w^.color := clRed;
              rotateRight(w);
              w := x_parent^.right;
            end;
            w^.color := x_parent^.color;
            x_parent^.color := clBlack;
            if (w^.right <> nil) then
            begin
              w^.right^.color := clBlack;
            end;
            rotateLeft(x_parent);
            x := Root; { break; }
         end
      end
      else
      begin
        { same as above, with right <^. left. }
        w := x_parent^.left;
        if (w^.color = clRed)  then begin
          w^.color := clBlack;
          x_parent^.color := clRed;
          rotateRight(x_parent);
          w := x_parent^.left;
        end;
        if (((w^.right = nil) or
             (w^.right^.color = clBlack)) and
            ((w^.left = nil) or
             (w^.left^.color = clBlack)))  then begin
          w^.color := clRed;
          x := x_parent;
          x_parent := x_parent^.parent;
        end else begin
          if ((w^.left = nil) or (w^.left^.color = clBlack)) then begin
            w^.right^.color := clBlack;
            w^.color := clRed;
            rotateLeft(w);
            w := x_parent^.left;
          end;
          w^.color := x_parent^.color;
          x_parent^.color := clBlack;
          if (w^.left <> nil) then begin
            w^.left^.color := clBlack;
          end;
          rotateRight(x_parent);
          x := Root; { break; }
        end;
      end;
    end;
    if (x <> nil) then begin
      x^.color := clBlack;
    end;
  end;
  y^.Duplicates.Free;
  dispose(y);
end;


end.

