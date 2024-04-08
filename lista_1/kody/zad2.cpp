#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

struct graph {
    std::list<int> neighbors;
    int degree;
};

graph addEdge(graph G, int edge) {
    G.neighbors.push_back(edge);
    return G;
};


void topologicalSorting(graph *G, int n) {
    int vertex[n];
    int k = 0;
    int tmp;

    for (int i = 0 ; i < n ; i++) {
        tmp = i;
        if (G[i].degree == 0) {
            vertex[k] = i;
            G[i].degree--; // jezeli stopien = -1 to wierzcholek jest sprawdzony
            for (std::list<int>::iterator j = G[i].neighbors.begin() ; j != G[i].neighbors.end() ; ++j) {
                G[*j].degree--;
                if (G[*j].degree == 0 && *j < i) {
                    tmp = *j - 1;
                }
            }
            i = tmp;
            k++;
        }
    }

    // std::cout << "k " << k << std::endl;
    // std::cout << "n " << n << std::endl;

    if (n == k) {
        std::cout << "Graf nie jest cykliczny" << std::endl;
        if (n < 200) {
            for (int i = 0 ; i < n ; i++) {
                std::cout << vertex[i] + 1 << std::endl;
            }
        }
    } else {
        std::cout << "Graf jest cykliczny" << std::endl;
    }
}

int main(int argc, char const *argv[]) {

    std::string type; // U - nieskierowany, D - skierowany
    int n; // liczba wierzcholkow
    int m; // liczba krawedzi

    std::ifstream file(argv[1]);
    std::string text;
    getline(file, text);
    type = text;
    getline(file, text);
    n = std::stoi(text);
    getline(file, text);
    m = std::stoi(text);

    graph *myGraph = new graph[n];

    for (int i = 0 ; i < n ; i++) {
        myGraph[i].degree = 0;
    }

    int a,b;
    
    if (!strcmp(type.c_str(), "U")) {
        std::cout << "Graf musi byc skierowany" << std::endl;
    } else {
        for (int i = 0 ; i < m ; i++) {
            getline(file, text, ' ');
            a = std::stoi(text) - 1;
            getline(file, text, '\n');
            b = std::stoi(text) - 1;
            myGraph[a] = addEdge(myGraph[a], b);
            myGraph[b].degree += 1;
        }
    }

    clock_t t = clock();
    double time_taken;
    t = clock();

    topologicalSorting(myGraph, n);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "Execution time: " << time_taken << " sec" <<std::endl;

    file.close();
}