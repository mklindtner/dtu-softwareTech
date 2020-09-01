type Lid = string
type Flight = string
type Airport = string
type Route = (Flight * Airport) list
type LuggageCatalogue = (Lid * Route) list

let lc: LuggageCatalogue = [
    ("DL 016-914", [
        ("DL 189", "ATL");
        ("DL 124", "BRU");
        ("SN 733", "CPH")
    ]);
    ("SK 222-142", [
        ("SK 208", "ATL");
        ("DL 124", "BRU");
        ("SK 122", "JFK")
    ])
 ]

// Task 2, Problem 1
let rec findRoute (lId: Lid, lc: LuggageCatalogue) =
    match lc with
    | [] -> failwith ("There is no route matching the given Luggage Id")
    | (currentLid, route) :: _ when currentLid = lId -> route
    | (_, _) :: tail -> findRoute (lId, tail)

// Task 2, Problem 2
let rec inRoute (flight: Flight) = function
    | [] -> false
    | (currentFlight, _) :: _ when currentFlight = flight -> true
    | (_, _) :: tail -> inRoute flight tail

// Task 2, Problem 3
let rec withFlight (f: Flight) (lc: LuggageCatalogue) =
    match lc with
    | [] -> []
    | (lId, route) :: tail when inRoute f route -> lId :: withFlight f tail
    | (_, _) :: tail -> withFlight f tail

// Task 2, Problem 4
type ArrivalCatalogue = (Airport * Lid list) list

let rec arrivalCatalogueAirportWithLid (lId: Lid, airport: Airport, arrivalCatalogue: ArrivalCatalogue): ArrivalCatalogue =
    match arrivalCatalogue with
    | [] -> [ (airport, [ lId ]) ]
    | (cAirport, lIds) :: tail when airport = cAirport -> (cAirport, lId :: lIds) :: tail
    | (cAirport, lIds) :: tail -> (cAirport, lIds) :: arrivalCatalogueAirportWithLid (lId, airport, tail);

let rec extend (lId: Lid, route: Route, arrivalCatalogue: ArrivalCatalogue): ArrivalCatalogue =
    match route with
    | [] -> arrivalCatalogue
    | (_, airport) :: tail ->
        extend (lId, tail, arrivalCatalogueAirportWithLid (lId, airport, arrivalCatalogue))


// Task 2, Problem 5
let toArrivalCatalog (lc: LuggageCatalogue): ArrivalCatalogue = List.fold (fun acc (lId, route) -> extend(lId, route, acc)) [] lc

// Task 3, Problem 2
// We want to find a boolean value for whether a flight EXISTS in a route - the exists function serves this purpose.
// We would have used List.contains, but we couldn't seem to use it in our environment.
let inRoute2 (flight: Flight) (route: Route) = List.exists (fun (f, _) -> f = flight) route

// Task 3, Problem 3
// List.filter gives us a list, with only the (Lid*Route) elements we want to keep. We only want to return the Lid's however, therefore we
// can use List.map to map the output to the first element of the tuple (Lid)
let withFlight2 (f: Flight) (lc: LuggageCatalogue) = List.map (fun (lId, _) -> lId) (List.filter (fun (_, route) -> inRoute2 f route) lc)