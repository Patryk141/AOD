#include <iostream>
#include <fstream>
#include <list>
#include <string.h>


struct graph {
    std::list<int> neighbors;
    bool visited;
};

graph addEdge(graph G, int edge) {
    G.neighbors.push_back(edge);
    return G;
}

void DFS(graph *G, int number) {
    std::cout << number + 1 << " ";
    G[number].visited = true;
    for (std::list<int>::iterator i = G[number].neighbors.begin() ; i != G[number].neighbors.end() ; i++) {
        if (!G[*i].visited) {
            DFS(G, *i);
            // cout << "(" << number  << ")\n";
        }
    }
}

void BFS(graph *G, int number) {
    G[number].visited = true;
    std::list<int> queue;
    queue.push_back(number);

    while(!queue.empty()) {
        int current = queue.front();
        std::cout << current + 1 << " ";
        queue.pop_front();
        for (std::list<int>::iterator i = G[current].neighbors.begin() ; i != G[current].neighbors.end() ; i++) {
            if (!G[*i].visited) {
                G[*i].visited = true;
                queue.push_back(*i);
            }
        }
    }
    
}

void reset(graph *G, int n) {
    for (int i = 0 ; i < n ; i++) {
        G[i].visited = false;
    }
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

    std::cout << "DFS:" << std::endl;
    DFS(myGraph, 0);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "\nExecution time: " << time_taken << " sec" <<std::endl;

    reset(myGraph, n);
    t = clock();

    std::cout << "BFS:" << std::endl;
    BFS(myGraph, 0);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "\nExecution time: " << time_taken << " sec" <<std::endl;
    
    file.close();
}
