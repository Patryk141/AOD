# Patryk Piskorski

set Companies := {1..3};
set Airports := {1..4};

set Tab within Airports cross Companies;
set Fuel within Companies;
set Demand within Airports;

set Airport{i in Airports} := {j in Companies: (i, j) in Tab};
set Company{i in Companies} := {j in Airports: (j, i) in Tab};

param unit_cost{Tab} >= 0;
param fuel{Fuel} >= 0;
param demand{Demand} >= 0;

var quantity{(i, j) in Tab} integer, >= 0;

# Funkcja celu
minimize cost: sum{(i, j) in Tab} unit_cost[i, j] * quantity[i, j];

# Ograniczenia
subject to airpo{i in Airports}: sum{j in Airport[i]} quantity[i, j] >= demand[i];
subject to com{i in Companies}: sum{j in Company[i]} quantity[j, i] <= fuel[i];

solve;

display sum{(i, j) in Tab} unit_cost[i, j] * quantity[i, j];
display airpo;
display com;

data;

param: Tab : unit_cost :=
       1 1       10
       1 2        7
       1 3        8
       2 1       10
       2 2       11
       2 3       14
       3 1        9
       3 2       12
       3 3        4
       4 1       11
       4 2       13
       4 3        9;

param: Fuel : fuel :=
        1    275000
        2    550000
        3    660000;

param: Demand : demand :=
         1      110000
         2      220000
         3      330000
         4      440000;
end;