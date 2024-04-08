#include <iostream>
#include <climits>
#include <vector>
#include <fstream>
#include <queue>
#include <cstring>
#include <chrono>
#include <list>
#include <math.h>

struct Edge {
    unsigned int neighbors;
    unsigned int cost;
};

struct A {
    std::list<Edge> edge;
};

long long *radixheap(int n, int s, A *edges, int maxCost) {
    unsigned int K = (unsigned int)ceil(log2(n) + log2(maxCost));
    long long *d = (long long*)malloc(n* sizeof(long long));

    for (int i = 1; i <= n; i++) {
        d[i] = LLONG_MAX;
    }
    d[s] = 0;
    std::vector<std::list<long long>> buckets(K + 1);
    long long *range = (long long*)malloc((K + 1)* sizeof(long long));
    range[0] = 0;
    long long temp_value = 1;
    for (int i = 1; i <= K; i++) {
        range[i] = temp_value;
        temp_value *= 2;
    }
    buckets[0].push_back(s);
// range 0 1 2 4 8 16 32

    int counter = 1;
    int bucket_inx = 0;
    int index;

    while (counter < n) {
        bucket_inx = 0;
        while (buckets[bucket_inx].empty()) {
            bucket_inx++;
        }
        if (bucket_inx >= (K + 1)) {
            break;
        }
        if (bucket_inx < 2) {
            while (!buckets[bucket_inx].empty()) {
                int current_vertex = buckets[bucket_inx].back(); // pobieram z niego element
                buckets[bucket_inx].pop_back();
                counter++;

                for (std::list<Edge>::iterator j = edges[current_vertex].edge.begin(); j != edges[current_vertex].edge.end(); j++) {
                    if (d[j->neighbors] > d[current_vertex] + j->cost) {
                        int index;
                        
                        for (index = 0; index <= K; index++) {
                            if (d[j->neighbors] < range[index]) {
                                break;
                            }
                        }

                        if (index != K) {
                            index--;
                        }

                        if (d[j->neighbors] < LLONG_MAX) {
                            buckets[index].remove(j->neighbors);
                        }

                        d[j->neighbors] = d[current_vertex] + j->cost;
                        
                        for (index; index >= 0; index--) {
                            if (d[j->neighbors] >= range[index]) {
                                break;
                            }
                        }
                        buckets[index].push_back(j->neighbors);

                    }
                }
            }       
        } else {
            long long min_value = LLONG_MAX;
            long long max_value;
            int count = 0;
            for (auto bucket = buckets[bucket_inx].begin(); bucket != buckets[bucket_inx].end(); bucket++) { // znajdowanie najmniejszej wartosci w wiaderkach
                count++;
                if (d[*bucket] < min_value) {
                    min_value = d[*bucket];
                }
            }
            range[0] = min_value; // w kubelkach od 0 do bucket_int powiny byc wartosci od min_value do range[bucket_inx + 1] - 1
            range[1] = min_value + 1;
            range[2] = min_value + 2;
            // min_value min_value + 1 min_value + 2 .... max_value ....
            // 8 9 10 12 14 16
            if (bucket_inx < K) {
                max_value = range[bucket_inx + 1] - 1;
            } else {
                max_value = maxCost;
            }
            if (bucket_inx > 2) {
                int value = max_value - min_value - 2;
                int empty = bucket_inx - 2;
                int tmp = floor(value/empty);

                for (int i = 3; i <= bucket_inx; i++) {
                    range[i] = range[i - 1] + tmp; 
                }
            }
                    

            // rozdzielanie z kubelka
            for (int i = 0; i < count; i++) { // znajdowanie najmniejszej wartosci w wiaderkach
                int bucket_tmp = buckets[bucket_inx].front();
                buckets[bucket_inx].pop_front();
                for (int j = 0; j <= bucket_inx; j++) {
                    if (d[bucket_tmp] < range[j]) {
                        buckets[j - 1].push_back(bucket_tmp);
                        break;
                    } else if (j == bucket_inx) {
                        buckets[j].push_back(bucket_tmp);
                    }
                }
            }

        }
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
    // std::cout << n << std::endl;
    end = text.find(" ", start);
    start = end + 1;
    int k = stoi(text.substr(start, end - start));
    // std::cout << k << std::endl;

    A *edges = new A[n];
    // int *c = (int *)malloc(n*n*sizeof(int));

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
                outputFile << "f " << argv[index_d + 1] << " " << argv[index_problem + 1] << std::endl;
                outputFile << "g " << n << " " << k << " " << min_cost << " " << max_cost << std::endl;
                auto start = std::chrono::high_resolution_clock::now();
                // for (int i = 0; i < numberOfSource; i++) {
                    radixheap(n, source[0] - 1, edges, max_cost);
                // }
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                outputFile << "t " << duration << std::endl;
                // outputFile << "t " << duration / numberOfSource << std::endl;
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
                    wynik = radixheap(n, source[0] - 1, edges, max_cost);
                    outputFile << "d " << source[i] << " " << ends[i] << " " << wynik[i] << std::endl;
                    
                }
                outputFile.close();
            }    
            break;
        }
    }

    return 0;
}
