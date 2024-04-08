#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>
#include <chrono>

// Struktura reprezentująca krawędź w grafie
struct Edge {
    int u;
    int v;

    Edge(int vertexU, int vertexV) : u(vertexU), v(vertexV) {}
};

// Generowanie grafu
std::vector<Edge> generateGraph(int k, int i) {
    std::vector<Edge> edges;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (1 << k) - 1);

    for (int u = 0; u < (1 << k); u++) {
        std::vector<int> verticesV;
        for (int j = 0; j < i; j++) {
            int v = dist(gen);
            while (std::find(verticesV.begin(), verticesV.end(), v) != verticesV.end()) {
                v = dist(gen);
            }
            verticesV.push_back(v);
            edges.emplace_back(u, v);
        }
    }

    return edges;
}

// Funkcja pomocnicza do znajdowania rozszerzającej ścieżki w algorytmie Hopcrofta-Karpa
bool augmentPath(int u, const std::vector<Edge>& graph, std::vector<int>& match, std::vector<bool>& visited) {
    if (u == -1) {
        return true;
    }

    for (int i = 0; i < graph.size(); i++) {
        const Edge& edge = graph[i];

        if (edge.u == u && !visited[i]) {
            visited[i] = true;

            if (augmentPath(match[edge.v], graph, match, visited)) {
                match[edge.v] = edge.u;
                return true;
            }
        }
    }

    return false;
}

// Obliczanie wielkości maksymalnego skojarzenia
int calculateMaxMatching(const std::vector<Edge>& graph) {
    std::vector<int> match(graph.size(), -1);
    std::vector<bool> visited(graph.size(), false);
    int matchingSize = 0;

    for (const Edge& edge : graph) {
        if (match[edge.v] == -1) {
            match[edge.v] = edge.u;
            matchingSize++;
        } else {
            visited.assign(graph.size(), false);

            if (augmentPath(edge.u, graph, match, visited)) {
                matchingSize++;
            }
        }
    }

    return matchingSize;
}

// Wypisywanie skojarzenia (opcjonalne)
void printMatching(const std::vector<Edge>& graph, const std::vector<int>& match) {
    std::cout << "Matching Edges:" << std::endl;
    for (int i = 0; i < match.size(); i++) {
        if (match[i] != -1) {
            const Edge& edge = graph[i];
            std::cout << "(" << edge.u << ", " << edge.v << ")" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    int k = 0;
    int i = 0;

    if (argc > 4 && !strcmp(argv[1], "--size") && !strcmp(argv[3], "--degree")) {
        k = std::stoi(argv[2]);
        i = std::stoi(argv[4]);
    } else {
        return -1;
    }

    auto start_time = std::chrono::steady_clock::now();

    std::vector<Edge> graph = generateGraph(k, i);
    int maxMatchingSize = calculateMaxMatching(graph);

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    if (argc > 5 && !strcmp(argv[5], "--save")) {
        std::cout << k << " " << i << " " << maxMatchingSize << " " << elapsed_time << std::endl;
        return 1;
    }

    std::cout << "Wielkość maksymalnego skojarzenia: " << maxMatchingSize << std::endl;
    std::cout << "Czas działania programu: " << elapsed_time << " ms" << std::endl;

    if (argc > 5 && std::string(argv[5]) == "--printMatching") {
        std::vector<int> match(graph.size(), -1);
        for (int i = 0; i < graph.size(); i++) {
            if (match[graph[i].v] == -1) {
                match[graph[i].v] = graph[i].u;
            }
        }
        printMatching(graph, match);
    }
    
    return 0;
}
