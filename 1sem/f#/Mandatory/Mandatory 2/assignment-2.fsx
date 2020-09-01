// Balance bikes
// AUTHORS:
// Daniel Kuzin, s184225
// Mads Kähler, s184206

(* ########################### *)
(* ## D E F I N I T I O N S ## *)
(* ########################### *)

type Part = string
type Task = string
type Cost = int(* can be assumed to be positive *)
type Duration = int(* can be assumed to be positive *)
type PartReg = Map<Part, Cost>
type TaskReg = Map<Task, Duration * Cost>

(* Part and task registers for balance bikes *)
(* Part and task registers for balance bikes *)
let preg1 = Map.ofList [ ("wheel", 50); ("saddle", 10); ("handlebars", 75); ("frame", 100); ("screw bolt", 5); ("nut", 3) ];
let treg1 = Map.ofList [ ("addWheels", (10, 2)); ("addSaddle", (5, 2)); ("addHandlebars", (6, 1)) ]


type WorkStation = Task * (Part * int) list
type AssemblyLine = WorkStation list

let ws1 = ("addWheels", [ ("wheel", 2); ("frame", 1); ("screw bolt", 2); ("nut", 2) ])
let ws2 = ("addSaddle", [ ("saddle", 1); ("screw bolt", 1); ("nut", 1) ])
let ws3 = ("addHandlebars", [ ("handlebars", 1); ("screw bolt", 1); ("nut", 1) ])

(* Not well defined in terms of treg1 because it does not contain the task "addPeddles" *)
let ws4 = ("addPeddles", [ ("handlebars", 1); ("screw bolt", 1); ("nut", 1) ])

(* Not well defined in terms preg1 because it does not contain the part "rivet" *)
let ws5 = ("addHandlebars", [ ("handlebars", 1); ("rivet", 1); ("nut", 1) ])

let al1 = [ ws1; ws2; ws3 ];

(* Not well defined because it contains ws4 *)
let al2 = [ ws1; ws2; ws3; ws4 ];

let failIfFalse (b: bool) = if (b = false) then failwith ("Test failed");


(* ################################### *)
(* ## S O L U T I O N S   B E G I N ## *)
(* ################################### *)

// ~~~ 2.1 ~~~
let wellDefWS pr tr (t, ps) = Map.containsKey t tr && List.forall (fun (p, a) -> Map.containsKey p pr && a > 0) ps

// Test
[ws1; ws2; ws3] |> List.map (fun ws -> wellDefWS preg1 treg1 ws)|> List.forall (fun b -> b)|> failIfFalse
[ws4; ws5] |> List.map (fun ws -> wellDefWS preg1 treg1 ws)|> List.forall (fun b -> not b)|> failIfFalse

// ~~~ 2.2 ~~~
let wellDefAL pr tr al = List.forall (fun ws -> wellDefWS pr tr ws) al

// Test
wellDefAL preg1 treg1 al1 |> failIfFalse
wellDefAL preg1 treg1 al2|> (fun b -> not b)|> failIfFalse

// ~~~ 2.3 ~~~
// longestDuration has the type: AssemblyLine * TaskReg -> Duration

// Maps the AssemblyLine to the cost of the task associated with each workstation and finds the max
let longestDuration (al, tr) = List.map (fun (t, _) -> Map.find t tr) al |> List.map (fun (d, _) -> d) |> List.max

// Test
longestDuration (al1, treg1)|> (fun r -> r = 10)|> failIfFalse

// ~~~ 2.4 ~~~

// Maps the AssemblyLine to the combined part costs of each WorkStation and then finds the sum
let partCostAL pr al = List.map (fun (_, pl) -> List.sumBy (fun (p, a) -> Map.find p pr * a) pl) al |> List.sum

// Test
partCostAL preg1 al1|> (fun r -> r = 317)|> failIfFalse

// ~~~ 2.5 ~~~

// Define infix operator for adding tuples
let (+~) (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)
let prodDurCost tr al = List.fold (fun acc (t, _) -> (Map.find t tr) +~ acc) (0, 0) al


// Test
prodDurCost treg1 al1|> (fun r -> r = (21, 5))|> failIfFalse

// ~~~ 2.6 ~~~

// If the key already exists then the value is replaced by the sum of the given value and the existing value, otherwise
// the key and value are added without modifications
let accumulateBySum map k v =
    match map with
        | map when Map.containsKey k map -> Map.add k (Map.find k map + v) map
        | map -> Map.add k v map

// Since a WorkStation's required parts are defined by a list and not a map, a required part and amount could be listed
// twice. Therefore, the list must be turned into map where duplicates are dealt with by finding the sum of the values
let getStockWS (_, pl) = List.fold (fun acc (k, v) -> accumulateBySum acc k v) Map.empty pl

// The maps from getStockWS are combined by once again finding the sum of the values of duplicate keys
let toStock al = List.fold (fun acc ws -> Map.fold accumulateBySum (getStockWS ws) acc) Map.empty al


// Test
toStock al1|>
    (fun r ->r = Map.ofList [ ("frame", 1); ("handlebars", 1); ("nut", 4); ("saddle", 1); ("screw bolt", 4); ("wheel", 2) ])|>
    failIfFalse
