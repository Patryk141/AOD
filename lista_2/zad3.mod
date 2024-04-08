# Patryk Piskorski

set Districts := {1..3}; # zbior dzielnic
set Changes := {1..3}; # zbior zmian

set Tab within Districts cross Changes; # tablica wielkosci liczba zmian x liczba dzielnic
set D within Districts;
set C within Changes;

set District{i in Districts} := {j in Changes: (i, j) in Tab};
set Change{i in Changes} := {j in Districts: (j, i) in Tab};

param min{Tab} >= 0;
param max{Tab} >= 0;

param min_d{D} >= 0;
param min_c{C} >= 0;

var x{(i, j) in Tab} integer, >= min[i, j], <= max[i, j];

# Funkcja celu
minimize quantity: sum{(i, j) in Tab} x[i, j];

# Ograniczenia
subject to district_{i in D}: sum{j in District[i]} x[i, j] >= min_d[i];
subject to change_{i in C}: sum{j in Change[i]} x[j, i] >= min_c[i];

solve;

display {(i, j) in Tab}: i,j,x[i,j];
display sum{(i, j) in Tab} x[i, j];

data;

param : Tab : min max :=
        1 1    2   3
        1 2    4   7
        1 3    3   5
        2 1    3   5
        2 2    6   7
        2 3    5  10
        3 1    5   8
        3 2    7  12
        3 3    6  10;

param : D : min_d :=
        1     10
        2     14
        3     13;

param : C : min_c :=
        1     10
        2     20
        3     18;
end;