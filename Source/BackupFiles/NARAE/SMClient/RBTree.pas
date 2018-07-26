unit RBTree;

interface

uses
  Classes;

type
  TColor = (clRed, clBlack);
  TRBNodeP = ^TRBNode;
  TRBNode = record
    Key: Pointer;
    Left, Right, Parent: TRBNodeP;
    Color: TColor;
    Data: Pointer;
  end;
  
  TRBTree = class
  private
    FCount: Integer;
  	Root: TRBNodeP;
    Leftmost: TRBNodeP;
    Rightmost: TRBNodeP;
    CompareFunc: TListSortCompare;
    procedure RotateLeft(var x: TRBNodeP);
    procedure RotateRight(var x: TRBNodeP);
    function Minimum(var x: TRBNodeP): TRBNodeP;
    function Maximum(var x: TRBNodeP): TRBNodeP;
    procedure fast_erase(x: TRBNodeP);    
	public
  	constructor Create;
    destructor Destroy; override;
    procedure Clear;
    function Find(Key: Pointer): TRBNodeP;
    function Add(Key: Pointer; Data: Pointer): TRBNodeP;
    procedure Delete(z: TRBNodeP);
		procedure RBInc(var x: TRBNodeP);
		procedure RBDec(var x: TRBNodeP);
  public
    property First: TRBNodeP read leftmost;
    property Last: TRBNodeP read rightmost;
    property Count: Integer read FCount;
   end;

implementation

function ComparePointer(Item1, Item2: Pointer): Integer;
begin
  if Longword	(Item1) < Longword(Item2) then
  begin
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

procedure TRBTree.RBInc(var x: TRBNodeP);
var
  y: TRBNodeP;
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

procedure TRBTree.RBDec(var x: TRBNodeP);
var
  y: TRBNodeP;
begin
  if (x^.Left <> nil) then
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

constructor TRBTree.Create;
begin
  inherited Create;
  FCount := 0;
  CompareFunc := ComparePointer;
  Root := nil;
  Leftmost := nil;
  Rightmost := nil;
end;

destructor TRBTree.Destroy();
begin
  Clear;
  inherited Destroy;
end;

procedure TRBTree.fast_erase(x: TRBNodeP);
begin
  if (x^.Left <> nil) then  fast_erase(x^.Left);
  if (x^.Right <> nil) then fast_erase(x^.Right);
  dispose(x);
end;

procedure TRBTree.Clear;
begin
  if (Root <> nil) then
    fast_erase(Root);
  Root := nil;
  Leftmost := nil;
  Rightmost := nil;
  FCount := 0;
end;

function TRBTree.Find(Key: Pointer): TRBNodeP;
var
  cmp: Integer;
begin
  Result := root;
  while (Result <> nil) do
  begin
    cmp := CompareFunc(Result^.Key, Key);
    if cmp < 0 then
    begin
      Result := Result^.Right;
    end
    else if cmp > 0 then
    begin
      Result := Result^.Left;
    end
    else
    begin
      break;
    end;
  end;
end;

procedure TRBTree.RotateLeft(var x: TRBNodeP);
var
  y: TRBNodeP;
begin
  y := x^.Right;
  x^.Right := y^.Left;
  if (y^.Left <> nil) then
  begin
    y^.Left^.Parent := x;
  end;

  y^.Parent := x^.Parent;
  if (x = Root) then
  begin
    Root := y;
  end
  else if (x = x^.Parent^.Left) then
  begin
    x^.Parent^.Left := y;
  end
  else
  begin
    x^.Parent^.Right := y;
  end;
  y^.Left := x;
  x^.Parent := y;
end;

procedure TRBTree.RotateRight(var x: TRBNodeP);
var
  y: TRBNodeP;
begin
  y := x^.Left;
  x^.Left := y^.Right;
  if (y^.Right <> nil) then
  begin
    y^.Right^.Parent := x;
  end;

  y^.Parent := x^.Parent;
  if (x = Root) then
  begin
    root := y;
  end
  else if (x = x^.Parent^.Right) then
  begin
    x^.Parent^.Right := y;
  end
  else
  begin
    x^.Parent^.Left := y;
  end;
  y^.Right := x;
  x^.Parent := y;
end;

function TRBTree.Minimum(var x: TRBNodeP): TRBNodeP;
begin
  Result := x;
  while (Result^.Left <> nil) do
    Result := Result^.Left;
end;

function TRBTree.Maximum(var x: TRBNodeP): TRBNodeP;
begin
  Result := x;
  while (Result^.Right <> nil) do
    Result := Result^.Right;
end;

function TRBTree.Add(key: Pointer; Data: Pointer): TRBNodeP;
var
  x, y, z, zpp: TRBNodeP;
  cmp: Integer;
begin
  z := New(TRBNodeP);
  { Initialize fields in new node z }
  z^.Key 	 	:= key;
  z^.Left 	:= nil;
  z^.Right 	:= nil;
  z^.Color 	:= clRed;
  z^.Data   := Data;
  Inc(FCount);
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
  x := root;
  while (x <> nil) do
  begin
    y := x;
    cmp := compareFunc(key, x^.Key);
    if (cmp < 0) then
    begin
      x := x^.Left;
    end
    else if (cmp > 0) then
    begin
      x := x^.Right;
    end
    else
    begin
      { Value already exists in tree. }
      Result := x;
      Dec(FCount);
      dispose(z);
      exit;
    end;
  end;

  z^.Parent := y;
  if (y = nil) then
  begin
    Root := z;
  end
  else if (compareFunc(key, y^.Key) < 0) then
  begin
    y^.Left := z;
  end
  else
  begin
    y^.Right := z;
  end;

  { Rebalance tree }
  while ((z <> Root) and (z^.Parent^.Color = clRed)) do
  begin
    zpp := z^.Parent^.Parent;
    if (z^.Parent = zpp^.Left) then
    begin
      y := zpp^.Right;
      if ((y <> nil) and (y^.color = clRed)) then
      begin
        z^.Parent^.Color := clBlack;
        y^.Color := clBlack;
        zpp^.Color := clRed;
        z := zpp;
      end
      else
      begin
        if (z = z^.Parent^.Right) then
        begin
          z := z^.Parent;
          rotateLeft(z);
        end;
        z^.Parent^.Color := clBlack;
        zpp^.Color := clRed;
        rotateRight(zpp);
      end;
    end
    else
    begin
      y := zpp^.Left;
      if ((y <> nil) and (y^.Color = clRed)) then
      begin
        z^.Parent^.Color := clBlack;
        y^.Color   := clBlack;
        zpp^.Color := clRed;
        z := zpp;
      end
      else
      begin
        if (z = z^.Parent^.Left) then
        begin
          z := z^.Parent;
          rotateRight(z);
        end;
        z^.Parent^.Color := clBlack;
        zpp^.Color := clRed;
        rotateLeft(zpp);
      end;
    end;
  end;
  root^.Color := clBlack;
end;

procedure TRBTree.Delete(Z: TRBNodeP);
var
  w, x, y, x_parent: TRBNodeP;
  tmpcol: TColor;
begin
	if Z = NIL then Exit;
  
  y := z;
  x := nil;
  x_parent := nil;

  if (y^.Left = nil) then
  begin    { z has at most one non-null child. y = z. }
    x := y^.Right;     { x might be null. }
  end
  else
  begin
    if (y^.Right = nil) then
    begin { z has exactly one non-null child. y = z. }
      x := y^.Left;    { x is not null. }
    end
    else
    begin
      { z has two non-null children.  Set y to }
      y := y^.Right;   {   z's successor.  x might be null. }
      while (y^.Left <> nil) do
      begin
        y := y^.Left;
      end;
      x := y^.Right;
    end;
  end;

  if (y <> z) then
  begin
    { "copy y's sattelite data into z" }
    { relink y in place of z.  y is z's successor }
    z^.Left^.Parent := y;
    y^.Left := z^.Left;
    if (y <> z^.Right) then
    begin
      x_parent := y^.Parent;
      if (x <> nil) then
      begin
        x^.Parent := y^.Parent;
      end;
      y^.Parent^.Left := x;   { y must be a child of left }
      y^.Right := z^.Right;
      z^.Right^.Parent := y;
    end
    else
    begin
      x_parent := y;
    end;

    if (Root = z) then
    begin
      Root := y;
    end
    else
    if (z^.Parent^.Left = z) then
    begin
      z^.Parent^.Left := y;
    end
    else
    begin
      z^.Parent^.Right := y;
    end;
    y^.Parent := z^.Parent;
    tmpcol := y^.Color;
    y^.Color := z^.Color;
    z^.Color := tmpcol;
    y := z;
    { y now points to node to be actually deleted }
  end
  else
  begin                        { y = z }
    x_parent := y^.Parent;
    if (x <> nil) then
    begin
      x^.Parent := y^.Parent;
    end;
    if (root = z) then
    begin
      root := x;
    end
    else
    begin
      if (z^.Parent^.Left = z) then
      begin
        z^.Parent^.Left := x;
      end
      else
      begin
        z^.Parent^.Right := x;
      end;
    end;
	  if (leftmost = z) then
    begin
	    if (z^.Right = nil) then
      begin      { z^.left must be null also }
	      leftmost := z^.Parent;
	    end
      else
      begin
	      leftmost := minimum(x);
      end;
    end;
	  if (rightmost = z) then
    begin
	    if (z^.Left = nil) then
      begin       { z^.right must be null also }
	      rightmost := z^.Parent;
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
    while ((x <> Root) and ((x = nil) or (x^.Color = clBlack))) do
    begin
      if (x = x_parent^.Left) then
      begin
          w := x_parent^.Right;
          if (w^.color = clRed) then
          begin
            w^.color := clBlack;
            x_parent^.color := clRed;
            rotateLeft(x_parent);
            w := x_parent^.Right;
          end;
          if (((w^.left = nil) or
               (w^.left^.color = clBlack)) and
              ((w^.right = nil) or
               (w^.right^.color = clBlack))) then
          begin
            w^.Color := clRed;
            x := x_parent;
            x_parent := x_parent^.Parent;
          end
          else
          begin
            if ((w^.Right = nil) or (w^.Right^.Color = clBlack)) then
            begin
              w^.Left^.Color := clBlack;
              w^.Color := clRed;
              rotateRight(w);
              w := x_parent^.Right;
            end;
            w^.Color := x_parent^.Color;
            x_parent^.Color := clBlack;
            if (w^.Right <> nil) then
            begin
              w^.Right^.Color := clBlack;
            end;
            rotateLeft(x_parent);
            x := Root; { break; }
         end
      end
      else
      begin
        { same as above, with right <^. left. }
        w := x_parent^.Left;
        if (w^.color = clRed) then
        begin
          w^.Color := clBlack;
          x_parent^.Color := clRed;
          rotateRight(x_parent);
          w := x_parent^.Left;
        end;
        if (((w^.right = nil) or
             (w^.right^.color = clBlack)) and
            ((w^.left = nil) or
             (w^.left^.color = clBlack))) then
        begin
          w^.Color := clRed;
          x := x_parent;
          x_parent := x_parent^.Parent;
        end
        else
        begin
          if ((w^.Left = nil) or (w^.Left^.Color = clBlack)) then
          begin
            w^.Right^.Color := clBlack;
            w^.Color := clRed;
            rotateLeft(w);
            w := x_parent^.Left;
          end;
          w^.Color := x_parent^.Color;
          x_parent^.Color := clBlack;
          if (w^.Left <> nil) then
          begin
            w^.Left^.Color := clBlack;
          end;
          rotateRight(x_parent);
          x := Root; { break; }
        end;
      end;
    end;
    if (x <> nil) then
    begin
      x^.Color := clBlack;
    end;
  end;
  Dec(FCount);
  dispose(y);
end;

end.

