#include <iostream>
#include <bitset>
#include <cstring>
#include <string>
#include <list>
#include <queue>
#include <climits>
#include <chrono>

struct node {
    std::string label;
    int Hamming_weight;

    node(std::string label) {
        this->label = label;
    }

    node() {}
};

int counter_ = 0;

int Hamming_weight(std::string label, int k) {
    int counter = 0;
    for (int i = 0; i < k; i++) {
        if (label[i] == '1') {
            counter++;
        }
    }
    return counter;
}

std::string intToBinary(int number, int k) {
    std::string result = "";  
    for (int i = k - 1; i >= 0; --i) {
        int bit = (number >> i) & 1;
        result = result + std::to_string(bit);
    }
    return result;
}

int max(int a, int b, int k) {
    int max = 0;
    if (max < a) {
        max = a;
    }
    if (max < b) {
        max = b;
    }
    if (max < abs(a - k)) {
        max = abs(a - k);
    }
    if (max < abs(b - k)) {
        max = abs(b - k);
    }
    return max;
}

unsigned short int *flow;
int *pre;
unsigned short int **capacity;

int BFS(node* nodes, int src, int des, int k) {
    std::queue<int> q;
    int size = 1 << k;
    for (int i = 0; i < size; i++) {
        pre[i] = -1;
    }
    pre[src] = 0;
    flow[src] = USHRT_MAX;
    q.push(src);
    while (!q.empty()) {
        int index = q.front();
        q.pop();
        if (index == des) {
            break;
        }
        for (int i = 0; i < size; i++) {
            if (i != src && capacity[index][i] > 0 && pre[i] == -1) {
                pre[i] = index;
                flow[i] = std::min(capacity[index][i], flow[index]);
                q.push(i);
            }
        }
    }

    if (pre[des] == -1) {
        return -1;
    }
    return flow[des];
}

int Edmonds_Karp(node* nodes, int src, int des, int k) {
    int increasement = BFS(nodes, src, des, k);
    int sumflow = 0;

    while (increasement != -1) {
        int temp = des;
        while (temp != src) {
            int last = pre[temp];
            capacity[last][temp] -= increasement;
            capacity[temp][last] += increasement;
            temp = last;
        }
        sumflow += increasement;
        counter_++;
        increasement = BFS(nodes, src, des, k);
    }
    
    return sumflow;
}

int main(int argc, char const *argv[]) {
    int k = 0;

    if (argv > 1 && !strcmp(argv[1], "--size")) {
        k = std::stoi(argv[2]);
    } else {
        return -1;
    }

    int number_v = 1 << k;

    node *nodes = new node[number_v];
    pre = new int[number_v];
    flow = new unsigned short int[number_v];
    capacity = new unsigned short int*[number_v];
    for (int i = 0; i < number_v; i++) {
        capacity[i] = new unsigned short int[number_v];
        for (int j = 0; j < number_v; j++) {
            capacity[i][j] = 0;
        }
    }

    // Tworzenie wierzchołków
    for (int i = 0; i < number_v; i++) {
        nodes[i] = node(intToBinary(i, k));
        nodes[i].Hamming_weight = Hamming_weight(nodes[i].label, k);
    }

    // Tworzenie krawędzi
    for (int i = 0; i < number_v; i++) {
        std::string acc_label = nodes[i].label;

        for (int j = 0; j < k; j++) {
            std::string new_label = acc_label;
            if (new_label[j] == '1') {
                new_label[j] = '0';
            } else {
                new_label[j] = '1';
            }
            std::bitset<16> binaryNumber(new_label);
            if (nodes[i].Hamming_weight < nodes[binaryNumber.to_ulong()].Hamming_weight) {
                capacity[i][binaryNumber.to_ulong()] = max(nodes[i].Hamming_weight, nodes[binaryNumber.to_ulong()].Hamming_weight, k);
            }
        }
    }

    auto start_time = std::chrono::steady_clock::now();
    int result = Edmonds_Karp(nodes, 0, number_v - 1, k);
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    if (argc > 3 && !strcmp(argv[3], "--save")) {
        std::cout << k << " " << result << " " << counter_ << " " << elapsed_time << std::endl;
    } else {
        std::cout << "Maximum flow: " << result << std::endl;
        std::cout << "Liczba ścieżek powiększających: " << counter_ << std::endl;
        std::cout << "Czas działania programu: " << elapsed_time << " ms" << std::endl;
    }

    return 0;
}
