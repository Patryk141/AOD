#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include <stack>

struct graph {
    std::list<int> neighbors;
    bool visited;
};

graph addEdge(graph G, int edge) {
    // G.neighbors.push_back(edge);
    G.neighbors.push_front(edge);
    return G;
}

void DFS(graph *G, std::stack<int> *stos, int number) {
    std::stack<int> *_stack = new std::stack<int>;
    _stack -> push(number);
    int current;

    while (!_stack -> empty()) {
        current = _stack -> top();
        if (!G[current].visited) {
            G[current].visited = true;
            _stack -> pop();
            // std::cout << current << std::endl;

            for (std::list<int>::iterator i = G[current].neighbors.begin() ; i != G[current].neighbors.end() ; i++) {
                if (!G[*i].visited) {
                    _stack -> push(*i);
                }
            }
            stos -> push(current);
        } else {
            _stack -> pop();
        }
    }
    free(_stack);
}

void SCC(graph *G, graph *GT, int n) {
    std::stack<int> *stos = new std::stack<int>;
    std::stack<int> *stos1 = new std::stack<int>;
    int counter = 0;
    for (int i = n - 1 ; i >= 0 ; i--) {
        if (!G[i].visited) {
            DFS(G, stos, i);
        }
    }
    while (!stos -> empty() && n > 200) {
        std::stack<int> *stos1 = new std::stack<int>;
        DFS(GT, stos1, stos -> top());
        
        if (stos1 -> size() > 0) {
            counter++;
            std::cout << "Cykl nr: " << counter << " liczba elem: " << stos1 -> size() << std::endl;
        }
        // std::cout << stos -> top() << std::endl;
        stos -> pop();
        free(stos1);
    }
    while (!stos -> empty() && n < 200) {
        DFS(GT, stos1, stos -> top());
        stos -> pop();
        if (stos1 -> size() > 0) {
            std::cout << "Cykl: ";
            while (!stos1 -> empty()) {
                std::cout << stos1 -> top() << " ";
                stos1 -> pop();
            }
            std::cout << std::endl;
        }
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
    graph *myGraphT = new graph[n];

    for (int i = 0 ; i < n ; i++) {
        myGraph[i].visited = false;
        myGraphT[i].visited = false;
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
            myGraphT[b] = addEdge(myGraphT[b], a);
        }
    }

    clock_t t = clock();
    double time_taken;
    t = clock();

    SCC(myGraph, myGraphT, n);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "Execution time: " << time_taken << " sec" <<std::endl;
    file.close();
}
