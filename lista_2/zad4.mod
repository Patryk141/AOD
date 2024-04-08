# Patryk Piskorski

param m, integer, >= 2;
param n, integer, >= 2;
param k, integer, >= 1;

set Width := {1..n};
set Height := {1..m};

set Area within Height cross Width;

param container{Area} integer, >=0, <=1;

var camera{(i, j) in Area} integer, >= 0, <= 1;

# Funkcja celu
minimize quantity: sum{(i, j) in Area} camera[i, j];

# Ograniczenia
subject to check{(i, j) in Area: container[i, j] == 1}: sum{(x, y) in Area: x == i and abs(j - y) <= k} camera[x, y] + sum{(x, y) in Area: abs(i - x) <= k and y == j} camera[x, y] >= 1;
subject to camera_check{(i, j) in Area: container[i, j] == 1}: camera[i, j] == 0;

solve;

display {(i, j) in Area} camera[i, j];
display quantity;

data;

param m := 5;
param n := 5;
param k := 2;

param : Area : container :=
        1  1       0
        1  2       0
        1  3       0
        1  4       0
        1  5       0
        2  1       1
        2  2       1
        2  3       0
        2  4       1
        2  5       1
        3  1       0
        3  2       0
        3  3       0
        3  4       0
        3  5       0
        4  1       0
        4  2       0
        4  3       0
        4  4       0
        4  5       0
        5  1       0
        5  2       0
        5  3       0
        5  4       0
        5  5       0;

end;