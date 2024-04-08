# Patryk Piskorski

set P := {1..4};
set M := {1..3};

set Tab within P cross M;
set Prod within P;
set Mach within M;

param time{Tab} >= 0;
param demand{Prod} >= 0;
param sell{Prod} >= 0;
param cost{Prod} >= 0;
param cost_m{Mach} >= 0;

set Machine{i in M} := {j in P: (j, i) in Tab};
set Product{i in P} := {j in M: (i, j) in Tab};

var x{i in P} integer, >= 0; # ilosc wyprodukowanych kilogramow produktu i

# Funkcja celu
maximize profit: sum{i in Prod}(x[i]*(sell[i] - cost[i])) - sum{i in Mach}(sum{j in Machine[i]} cost_m[i]*x[j]*time[j,i]/60);

# Ograniczenia
subject to machine{i in M}: sum{j in Machine[i]} x[j]*time[j, i] <= 60*60;
subject to product{i in P}: x[i] <= demand[i];

solve;

display profit;
display{i in P} x[i];
display machine;

data;

param : Tab : time :=
        1 1     5
        1 2    10
        1 3     6
        2 1     3
        2 2     6
        2 3     4
        3 1     4
        3 2     5
        3 3     3
        4 1     4
        4 2     2
        4 3     1;

param : Prod : demand sell cost :=
          1     400    9     4
          2     100    7     1
          3     150    6     1
          4     500    5     1;

param : Mach : cost_m :=
         1       2
         2       2
         3       3;
end;