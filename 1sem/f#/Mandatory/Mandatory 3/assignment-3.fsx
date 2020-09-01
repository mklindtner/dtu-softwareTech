(* MANDATORY 3 *)
// AUTHORS:
// Daniel Kuzin, s184225
// Mads Kähler, s184206

(* ########################### *)
(* ## D E F I N I T I O N S ## *)
(* ########################### *)

let failIfFalse (b: bool) = if (b = false) then failwith ("Test failed");
type ExprTree =
 | Const of int
 | Ident of string
 | Sum of ExprTree * ExprTree
 | Let of string * ExprTree * ExprTree

(* ################################### *)
(* ## S O L U T I O N S   B E G I N ## *)
(* ################################### *)

// ~~~ 1 ~~~
Const 4
Let("x", Const 4, Sum(Const 5, Ident "x"))
Sum(Const 4, Const 3)
Let("y", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y"))

// ~~~ 2 ~~~

//ExprTree -> int list
let rec getConsts expr =
    match expr with
    | Const x -> [ x ]
    | Ident _ -> []
    | Sum(x, y) | Let(_, x, y) -> getConsts (x) @ getConsts (y)

// Test
getConsts (Const 4) |> fun x -> x = [ 4 ] |> failIfFalse
getConsts (Let("x", Const 4, Sum(Const 5, Ident "x"))) |> fun x -> x = [ 4; 5 ] |> failIfFalse
getConsts (Sum(Const 4, Const 3)) |> fun x -> x = [ 4; 3 ] |> failIfFalse
getConsts (Let("y", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = [ 4; 5; 4 ] |> failIfFalse

// ~~~ 3 ~~~
let rec extFreeIdentH expr s =
    match expr with
    | Const _ -> s
    | Ident x -> Set.add x s
    | Sum(x, y) -> Set.union (extFreeIdentH x s) (extFreeIdentH y s)
    | Let(x, y, z) -> extFreeIdentH z s |> Set.remove x |> Set.union (extFreeIdentH y s)

//ExprTree -> Set<string>
let extFreeIdent expr = extFreeIdentH expr Set.empty

// Test
extFreeIdent (Let("y", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = Set.ofList [] |> failIfFalse
extFreeIdent (Let("z", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = Set.ofList [ "y" ] |> failIfFalse
extFreeIdent (Let("x", Ident "x", Ident "x")) |> fun x -> x = Set.ofList [ "x" ] |> failIfFalse

// ~~~ 4 ~~~
let rec subst x n t =
    match t with
    | Ident x' when x' = x -> Const n
    | Sum(x', y') -> Sum(subst x n x', subst x n y')
    | Let(x', y', z') when x' = x -> Let(x', y', z')
    | Let(x', y', z') -> Let(x', subst x n y', subst x n z')
    | n -> n

// Test
subst "x" 5 (Sum(Const 5, Ident "x")) |> fun x -> x = (Sum(Const 5, Const 5)) |> failIfFalse
subst "x" 5 (Sum(Let("x", Const 1, Ident "x"), Ident "x"))|> fun x -> x = (Sum(Let("x", Const 1, Ident "x"), Const 5)) |> failIfFalse

//5)
type ExprTreeExt =
 | Const of int
 | Ident of string
 | Sum of ExprTreeExt * ExprTreeExt
 | Let of string * ExprTreeExt * ExprTreeExt
 | App of string * ExprTreeExt list

let rec getConstsExt expr =
    match expr with
    | Const x -> [ x ]
    | Ident _ -> []
    | Sum(x, y) | Let(_, x, y) -> getConstsExt (x) @ getConstsExt (y)
    | App(_, x) -> List.fold (fun acc x' -> (getConstsExt x') @ acc) [] x


// Test
getConstsExt (Const 4) |> fun x -> x = [ 4 ] |> failIfFalse
getConstsExt (Let("x", Const 4, Sum(Const 5, Ident "x"))) |> fun x -> x = [ 4; 5 ] |> failIfFalse
getConstsExt (Sum(Const 4, Const 3)) |> fun x -> x = [ 4; 3 ] |> failIfFalse
getConstsExt (Let("y", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = [ 4; 5; 4 ] |> failIfFalse
getConstsExt (App ("x", [ Const 4; App("y", [ Const 5 ]) ]))|> fun x -> x = [ 5; 4 ] |> failIfFalse

let rec extFreeIdentHExt expr s =
    match expr with
    | Const _ -> Set.empty
    | Ident x -> Set.add x s
    | Sum(x, y) -> Set.union (extFreeIdentHExt x s) (extFreeIdentHExt y s)
    | Let(x, y, z) -> extFreeIdentHExt z s |> Set.remove x |> Set.union (extFreeIdentHExt y s)
    | App(_, x) -> List.fold (fun acc x' -> Set.union (extFreeIdentHExt x' Set.empty) acc) s x

let rec extFreeIdentExt (expr: ExprTreeExt) = extFreeIdentHExt expr Set.empty


// Test
extFreeIdentExt (Let("y", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = Set.ofList [] |> failIfFalse
extFreeIdentExt (Let("z", Let("x", Const 4, Sum(Const 5, Ident "x")), Sum(Const 4, Ident "y")))|> fun x -> x = Set.ofList [ "y" ] |> failIfFalse
extFreeIdentExt (Let("x", Ident "x", Ident "x")) |> fun x -> x = Set.ofList [ "x" ] |> failIfFalse
extFreeIdentExt (Let ("x", Const 4, App ("x", [ Ident "x"; Ident "x" ])))|> fun x -> x = Set.ofList [] |> failIfFalse
extFreeIdentExt (App ("x", [ Ident "x"; Ident "x" ]))|> fun x -> x = Set.ofList [ "x" ] |> failIfFalse

let rec substExt x n t =
    match t with
    | Const x' -> Const x'
    | Ident x' when x' = x -> Const n
    | Ident x' -> Ident x'
    | Sum(x', y') -> Sum(substExt x n x', substExt x n y')
    | Let(x', y', z') when x' = x -> Let(x', y', z')
    | Let(x', y', z') -> Let(x', substExt x n y', substExt x n z')
    | App(s, x') -> App(s, List.map (fun y -> substExt x n y) x')


// Test
substExt "x" 5 (Sum(Const 5, Ident "x")) |> fun x -> x = (Sum(Const 5, Const 5)) |> failIfFalse
substExt "x" 5 (Sum(Let("x", Const 1, Ident "x"), Ident "x"))|> fun x -> x = (Sum(Let("x", Const 1, Ident "x"), Const 5)) |> failIfFalse
substExt "x" 5 (Sum (Const 5, App ("a", [ Ident "x"; Ident "x" ])))|> fun x -> x = (Sum (Const 5, App ("a", [ Const 5; Const 5 ]))) |> failIfFalse
substExt "x" 5 (Sum (Let ("x", Const 1, App ("a", [ Ident "x"; Ident "x" ])), App ("a", [ Ident "x"; Ident "x" ])))|> fun x -> x = (Sum (Let ("x", Const 1, App ("a", [ Ident "x"; Ident "x" ])), App ("a", [ Const 5; Const 5 ]))) |> failIfFalse
