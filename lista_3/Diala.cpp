#include <iostream>
#include <climits>
#include <vector>
#include <fstream>
#include <queue>
#include <cstring>
#include <chrono>
#include <list>

struct Edge {
    long long neighbors;
    long long cost;
};

struct A {
    std::list<Edge> edge;
};

long long *dial(int n, int s, A *edges, int maxCost) { // n - liczba wierzcholkow s - pierwszy wierzcholek
    long long *d = (long long*)malloc(n*sizeof(long long));
    for (int i = 0 ; i < n ; i++) {
        d[i] = LLONG_MAX;
    }
    d[s] = 0;

    std::vector<std::list<int>> buckets(maxCost + 1);
    buckets[0].push_back(s);

    int counter = 1;
    int bucket_inx = 0;
    int curr = 0;
    
    while (counter < n) {
        while (!buckets[bucket_inx].empty()) {
            int current_vertex = buckets[bucket_inx].back();
            buckets[bucket_inx].pop_back();
            counter++;
            for (std::list<Edge>::iterator j = edges[current_vertex].edge.begin(); j != edges[current_vertex].edge.end(); j++) {
                if (d[j->neighbors] > curr + j->cost) {
                    if (d[j->neighbors] < LLONG_MAX) {
                        buckets[d[j->neighbors]%(maxCost + 1)].remove(j->neighbors);
                    }

                    d[j->neighbors] = curr + j->cost;
                    buckets[d[j->neighbors]%(maxCost + 1)].push_back(j->neighbors);
                }
            }
        }
        curr++;
        bucket_inx = curr%(maxCost + 1);
    }
    return d;
}

int main(int argc, char const *argv[]) {
    
    int index_d;
    for (index_d = 1; index_d < argc; index_d++) {
        if (!strcmp(argv[index_d], "-d")) {
            break;
        }
    }

    if (index_d + 1 >= argc) {
        return 1;
    }

// Wczytywanie danych z pliku
    std::ifstream file(argv[index_d + 1]);
    std::string text;

    int min_cost = INT_MAX;
    int max_cost = INT_MIN;

    while (1) {
        getline(file, text, '\n');
        if (text[0] == 'p') {
            break;
        }
    }

    int start = 0;
    int end = text.find(" ");
    start = end + 1;
    end = text.find(" ", start);
    start = end + 1;
    int n = stoi(text.substr(start, end - start));

    end = text.find(" ", start);
    start = end + 1;
    int k = stoi(text.substr(start, end - start));


    A *edges = new A[n];


    int counter = 0;
    while (counter < k) {
        getline(file, text, '\n');
        if (text[0] == 'a') {
            start = 0;
            end = text.find(" ");
            start = end + 1;
            end = text.find(" ", start);
            int index_i = stoi(text.substr(start, end - start)) - 1;
            start = end + 1;
            end = text.find(" ", start);
            int index_j = stoi(text.substr(start, end - start)) - 1;
            start = end + 1;
            end = text.find(" ", start);
            int cost = stoi(text.substr(start, end - start));
            Edge *edge = new Edge();
            edge->neighbors = index_j;
            edge->cost = cost;
            edges[index_i].edge.push_front(*edge);
            counter++;
            if (cost < min_cost) {
                min_cost = cost;
            }
            if (cost > max_cost) {
                max_cost = cost;
            }
        }
    }
    
// Sprawdzenie problemu
    int problem = 0;
    int index_problem;
    for (index_problem = 1; index_problem < argc; index_problem++) {
        if (!strcmp(argv[index_problem], "-ss")) {
            problem = 1;
            break;
        } else if (!strcmp(argv[index_problem], "-p2p")) {
            problem = 2;
            break;
        }
    }
    
    int index_save;
    for (index_save = 1; index_save < argc; index_save++) {
        if (!strcmp(argv[index_save], "-oss")) {
            break;
        } else if (!strcmp(argv[index_save], "-op2p")) {
            break;
        }
    }

    std::ofstream outputFile(argv[index_save + 1]);

    switch (problem) {
        case (1): { // ss 
            std::ifstream file_p(argv[index_problem + 1]);
            std::string data_line;
            while (1) {
                getline(file_p, data_line, '\n');
                if (data_line[0] == 'p') {
                    break;
                }
            }
            start = data_line.find("ss");
            end = data_line.find(" ", start);
            start = end + 1;
            end = data_line.find(" ", start);
            int numberOfSource = stoi(data_line.substr(start, end - start));
            int source[numberOfSource];

            counter = 0;
            while (counter < numberOfSource) {
                getline(file_p, data_line, '\n');
                if (data_line[0] == 's') {
                    start = 0;
                    end = data_line.find(" ");
                    start = end + 1;
                    end = data_line.find(" ", start);
                    source[counter] = stoi(data_line.substr(start, end - start));
                    counter++;
                }
            }

            if (outputFile.is_open()) {
                outputFile << "p res sp ss dial" << std::endl;
                outputFile << "f " << argv[index_d + 1] << " " << argv[index_problem + 1] << std::endl;
                outputFile << "g " << n << " " << k << " " << min_cost << " " << max_cost << std::endl;
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < numberOfSource; i++) {
                    dial(n, source[i] - 1, edges, max_cost);
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                outputFile << "t " << duration / numberOfSource << std::endl;
            }
            
            break;
        }
        case (2): { // p2p
            std::ifstream file_p(argv[index_problem + 1]);
            std::string data_line;
            while (1) {
                getline(file_p, data_line, '\n');
                if (data_line[0] == 'p') {
                    break;
                }
            }
            start = data_line.find("p2p");
            end = data_line.find(" ", start);
            start = end + 1;
            end = data_line.find(" ", start);
            int numberOfPairs = stoi(data_line.substr(start, end - start));
            int source[numberOfPairs];
            int ends[numberOfPairs];

            counter = 0;
            while (counter < numberOfPairs) {
                getline(file_p, data_line, '\n');
                if (data_line[0] == 'q') {
                    start = 0;
                    end = data_line.find(" ");
                    start = end + 1;
                    end = data_line.find(" ", start);
                    source[counter] = stoi(data_line.substr(start, end - start));
                    start = end + 1;
                    end = data_line.find(" ", start);
                    ends[counter] = stoi(data_line.substr(start, end - start));
                    counter++;
                }
            }

            if (outputFile.is_open()) {
                outputFile << "f " << argv[index_d + 1] << " " << argv[index_problem + 1] << std::endl;
                outputFile << "g " << n << " " << k << " " << min_cost << " " << max_cost << std::endl;

                for (int i = 0; i < numberOfPairs; i++) {
                    long long *wynik = new long long[n];
                    wynik = dial(n, source[i] - 1, edges, max_cost);
                    outputFile << "d " << source[i] << " " << ends[i] << " " << wynik[i] << std::endl;
                }
                outputFile.close();
            }    
            break;
        }
    }

    return 0;
}
