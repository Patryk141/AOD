#include <bits/stdc++.h>

struct Edge {
    int v;
    int flow;
    int C;
    int rev;
};

struct node {
    std::string label;
    int Hamming_weight;

    node(std::string label) {
        this->label = label;
    }

    node() {}
};

int counter_ = 0;

struct Graph {
    int V;
    int* level;
    std::vector<Edge>* adj;
 
    Graph(int V) {
        adj = new std::vector<Edge>[V];
        this->V = V;
        level = new int[V];
    }
 
    void addEdge(int u, int v, int C) {
        Edge a{v, 0, C, (int)adj[v].size()};
        Edge b{u, 0, 0, (int)adj[u].size()};
 
        adj[u].push_back(a);
        adj[v].push_back(b);
    }
 
    bool BFS(int s, int t) {
        for (int i = 0; i < V; i++)
            level[i] = -1;
    
        level[s] = 0;

        std::list<int> q;
        q.push_back(s);
    
        std::vector<Edge>::iterator i;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            for (i = adj[u].begin(); i != adj[u].end(); i++) {
                Edge& e = *i;
                if (level[e.v] < 0 && e.flow < e.C) {
                    level[e.v] = level[u] + 1;
    
                    q.push_back(e.v);
                }
            }
        }

        return level[t] < 0 ? false : true;
    }

    int sendFlow(int u, int flow, int t, int start[]) {
        if (u == t)
            return flow;
    
        for (; start[u] < adj[u].size(); start[u]++) {
            Edge& e = adj[u][start[u]];
    
            if (level[e.v] == level[u] + 1 && e.flow < e.C) {
                int curr_flow = std::min(flow, e.C - e.flow);
                int temp_flow = sendFlow(e.v, curr_flow, t, start);
    
                if (temp_flow > 0) {
                    e.flow += temp_flow;
                    adj[e.v][e.rev].flow -= temp_flow;
                    return temp_flow;
                }
            }
        }
    
        return 0;
    }

    int DinicMaxflow(int s, int t) {
        if (s == t)
            return -1;
    
        int total = 0;
        while (BFS(s, t) == true) {
            int* start = new int[V + 1]{ 0 };
            while (int flow = sendFlow(s, INT_MAX, t, start)) {
                total += flow;
                counter_++;
            }
            delete[] start;
        }
        return total;
    }
};

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

int main(int argc, char const *argv[]) {
    int k = 0;
    

    if (!strcmp(argv[1], "--size")) {
        k = std::stoi(argv[2]);
    } else {
        return -1;
    }

    int number_v = 1 << k;

    Graph g(number_v);
    node *nodes = new node[number_v];

    // Tworzenie wirzchołków
    for (int i = 0; i < number_v; i++) {
        nodes[i] = node(intToBinary(i, k));
        nodes[i].Hamming_weight = Hamming_weight(nodes[i].label, k);
    }

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
                g.addEdge(i, binaryNumber.to_ulong(), max(nodes[i].Hamming_weight, nodes[binaryNumber.to_ulong()].Hamming_weight, k));
            }
        }
    }

    auto start_time = std::chrono::steady_clock::now();
    int result = g.DinicMaxflow(0, number_v - 1);
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
