# Patryk Piskorski

param n, integer, >= 2; # liczba wierzcholkow

set N := {1..n}; # zbior wierzcholkow od 1 do n
set Start within N; # poczatkowy wierzcholek
set Finish within N; # koncowy wierzcholek
check: (Start inter Finish) within {}; # sprawdzenie czy poczatkowy wierzcholek jest rozny od koncowego

set N2 := N diff Start diff Finish; # zbior wierzcholkow bez poczatkowego i koncowego

set A within N cross N; # zbior zawierajacy krawedzie

set Outgoing{i in N} := {j in N: (i, j) in A}; # krawedzie wychodzace z i
set Incoming{i in N} := {j in N: (j, i) in A}; # krawedzie wchodzace do i

param c{A} >= 0; # koszt przejscia z i do j
param t{A} >= 0; # czas przejscia z i do j

param T >= 0; # maksymalny mozliwy czas

var x{(i, j) in A} integer, >= 0, <= 1; # zmienna przyjmujaca 1 gdy wybierzemy sciezke z i do j

# Funkcja celu
minimize cost: sum{(i, j) in A} c[i, j] * x[i, j]; # funkcja celu

# Ograniczenia
subject to start{i in Start}: sum{j in Outgoing[i]} x[i,j] = 1; # z poczatkowego wierzcholka musimy wybrac jedna krawedz
subject to equals{i in N2}:   sum{j in Outgoing[i]} x[i,j] - sum{j in Incoming[i]}x[j,i] = 0; # ograniczenie jak wybralismy krawedzie do wierzcholka i do musimy zniego wyjsc 
subject to time: sum{(i, j) in A} t[i, j] * x[i, j] <= T; # ograniczenie na czas

solve;

display{(i, j) in A: x[i, j] > 0}: x[i,j];

data;

param n := 10;

set Start := 1;
set Finish := 10;

param T := 8;

param : A : c  t :=
       1 2  4  1
       1 3  2  2
       2 4  3  1
       3 4  1  4
       3 5  2  2
       3 6  2  2
       4 7  6  1
       5 6  1  2
       6 7  2  2
       7 8  1  1
       8 9  1  1
       9 10 1  1;
end;