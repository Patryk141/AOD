#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

struct graph {
    std::list<int> neighbors;
    bool visited;
    int type;
};

graph addEdge(graph G, int edge) {
    G.neighbors.push_back(edge);
    return G;
}

bool BFS(graph *G, int number, int type) {
    G[number].visited = true;
    G[number].type = type;
    std::list<int> queue;
    queue.push_back(number);

    while(!queue.empty()) {
        int current = queue.front();
        // cout << current + 1 << "\n";
        queue.pop_front();
        for (std::list<int>::iterator i = G[current].neighbors.begin() ; i != G[current].neighbors.end() ; i++) {
            if (!G[*i].visited) {
                G[*i].visited = true;
                G[*i].type = G[current].type * (-1);
                queue.push_back(*i);
            } else if(G[*i].type == G[current].type) {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char const *argv[]) {
    std::string type; // u - nieskierowany, d - skierowany
    int n; // liczba wierzcholkow 
    int m; // liczba krawedzi

    std::ifstream file(argv[1]);
    std::string text;
    getline(file, text);
    type = text;
    getline(file, text);
    n = stoi(text);
    getline(file, text);
    m = stoi(text);

    graph *myGraph = new graph[n];

    for (int i = 0 ; i < n ; i++) {
        myGraph[i].visited = false;
    }

    int a,b;

    for (int i = 0 ; i < m ; i++) {
        getline(file, text, ' ');
        a = stoi(text) - 1;
        getline(file, text, '\n');
        b = stoi(text) - 1;
        if (!strcmp(type.c_str(), "U")) {
            myGraph[a] = addEdge(myGraph[a], b);
            myGraph[b] = addEdge(myGraph[b], a);
        } else {
            myGraph[a] = addEdge(myGraph[a], b);
        }
    }


    clock_t t = clock();
    double time_taken;
    t = clock();

    if (BFS(myGraph, 0, 1)) {
        std::cout << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl; 
    }

    if (BFS(myGraph, 0, 1) == true && n < 200) {
        std::cout << "Subset 1:" << std::endl;
        for (int i = 0 ; i < n ; i++) {
            if (myGraph[i].type == 1) {
                std::cout << i + 1 << std::endl;
            }
        }
        std::cout << "Subset 2:" << std::endl;
        for (int i = 0 ; i < n ; i++) {
            if (myGraph[i].type == -1) {
                std::cout << i + 1 << std::endl;
            }
        }
    }

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "Execution time: " << time_taken << " sec" <<std::endl;

    file.close();
    return 0;
}
