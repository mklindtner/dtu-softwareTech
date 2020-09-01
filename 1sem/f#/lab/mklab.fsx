System.Math.PI;;
// let s r = r * r;;
// s 10;;
let circleArea r = System.Math.PI * r * r;;
let circleArea2 = fun r -> System.Math.PI * r * r;;


let f = function
| 2 -> 28 // February
| 4 -> 30 // April
| 6 -> 30 // June
| 9 -> 30 // September
| 11 -> 30 // November
| _ -> 31;;// All other months


let rec fact = function 
| 0 -> 1
| k -> k * fact(k-1);;
fact 5;;

let a = (2.0, 3);;
let (x,y) = a;;
x + 3.0;;

let rec power = function 
| (x,0) -> 1.0
| (x, n) -> x * power(x, n-1);;
power (2.0, 2);;