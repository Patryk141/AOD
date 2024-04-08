unsigned int *dial(int s, int &n, int &m,
                   std::unordered_map<int, std::unordered_set<int>> &arcs,
                   int **&cost, int &max_cost) {
    unsigned int *d = (unsigned int *)malloc((n + 1) * sizeof(unsigned int));
    for (int i = 1; i <= n; i++) {
        d[i] = std::numeric_limits<unsigned int>::max();
    }
    d[s] = 0;
    int S = 1;
    int curr = 0;
    int k = 0;
    std::vector<std::list<int>> content(max_cost + 1);
    content[0].push_back(s);
    int a;
    unsigned int old_val, new_val;

    while (S < n) {
        while (!content[k].empty()) {
            a = content[k].back();
            content[k].pop_back();
            S = S + 1;
            for (int b : arcs[a]) {
                old_val = d[b];
                new_val = curr + cost[a][b];

                if (new_val < old_val) {
                    if (old_val < std::numeric_limits<unsigned int>::max()) {
                        content[old_val % (max_cost + 1)].remove(b);
                    }
                    content[new_val % (max_cost + 1)].push_back(b);
                    d[b] = new_val;
                }
            }
        }
        curr = curr + 1;
        k = curr % (max_cost + 1);
    }

    return d;
}

unsigned int *dial(int n, int s, A *edges, int maxCost) { // n - liczba wierzcholkow s - pierwszy wierzcholek
    unsigned int *d = (unsigned int*)malloc(n*sizeof(unsigned int));
    for (int i = 0 ; i < n ; i++) {
        d[i] = UINT_MAX;
    }
    d[s] = 0;

    std::vector<std::vector<int>> buckets(maxCost + 1, std::vector<int>());
    buckets[0].push_back(s);

    for (int bucket_inx = 0; bucket_inx < (int)buckets.size(); bucket_inx++) {
        while (!buckets[bucket_inx].empty()) {
            int current_vertex = buckets[bucket_inx].back();
            buckets[bucket_inx].pop_back();

            if (d[current_vertex] < bucket_inx)
                continue;

            for (std::list<Edge>::iterator j = edges[current_vertex].edge.begin(); j != edges[current_vertex].edge.end(); j++) {
                if (d[j->neighbors] > bucket_inx + j->cost) {
                    d[j->neighbors] = bucket_inx + j->cost;
                    buckets[d[j->neighbors]%(maxCost + 1)].push_back(j->neighbors);
                }
            }
        }
        
    }
    return d;
}

long long *radixheap(int s, int &n, int &m,
                     std::unordered_map<int, std::unordered_set<int>> &arcs,
                     int **&cost, int &max_cost) {
    int K = (int)ceil(log2(n) + log2(max_cost));

    long long *d = (long long *)malloc((n + 1) * sizeof(long long));
    int *bucket_idx = (int *)malloc((n + 1) * sizeof(int));
    std::vector<std::list<long long>> content(K + 1);

    for (int i = 1; i <= n; i++) {
        d[i] = std::numeric_limits<long long>::max();
        bucket_idx[i] = K;
    }

    d[s] = 0;
    bucket_idx[s] = 0;
    content[0].push_back(s);

    int S = 1;
    int k = 0;
    long long *range_begin = (long long *)malloc((K + 1) * sizeof(long long));
    range_begin[0] = 0;
    long long x = 1;
    for (int i = 1; i <= K; i++) {
        range_begin[i] = x;
        x = 2 * x;
    }
    int a, old_bucket_idx, new_bucket_idx;
    long long old_val, new_val, min_val;

    while (S < n) {
        k = 0;
        while (content[k].empty()) {
            k = k + 1;
        }
        if (k <= 1) {
            while (!content[k].empty()) {
                a = content[k].back();
                content[k].pop_back();
                S = S + 1;

                for (int b : arcs[a]) {
                    old_val = d[b];
                    new_val = range_begin[k] + cost[a][b];

                    if (new_val < old_val) {
                        old_bucket_idx = bucket_idx[b];

                        if (old_val < std::numeric_limits<long long>::max()) {
                            content[old_bucket_idx].remove(b);
                        }

                        new_bucket_idx = old_bucket_idx;
                        while (range_begin[new_bucket_idx] > new_val) {
                            new_bucket_idx = new_bucket_idx - 1;
                        }

                        content[new_bucket_idx].push_back(b);
                        d[b] = new_val;
                        bucket_idx[b] = new_bucket_idx;
                    }
                }
            }
        } else {
            min_val = std::numeric_limits<long long>::max();
            for (auto it = content[k].begin(); it != content[k].end(); ++it) {
                if (d[*it] < min_val) {
                    min_val = d[*it];
                }
            }
            range_begin[0] = min_val;
            long long x = 1;
            for (int i = 1; i <= k; i++) {
                range_begin[i] = min_val + x;
                x = 2 * x;
            }
            while (!content[k].empty()) {
                a = content[k].back();
                content[k].pop_back();
                new_bucket_idx = k - 1;
                while (range_begin[new_bucket_idx] > d[a]) {
                    new_bucket_idx = new_bucket_idx - 1;
                }
                bucket_idx[a] = new_bucket_idx;
                content[new_bucket_idx].push_back(a);
            }
        }
    }

    free(range_begin);
    free(bucket_idx);
    return d;
}



unsigned int *radixheap(int n, int s, A *edges, int maxCost) {
    unsigned int *d = (unsigned int*)malloc(n*sizeof(unsigned int));
    int K = (int)ceil(log2(n) + log2(maxCost));

    int *bucket_idx = (int*)malloc(n* sizeof(int));

    for (int i = 0 ; i < n ; i++) {
        d[i] = UINT_MAX;
        bucket_idx[i] = K;
    }

    std::vector<std::vector<int>> buckets(K + 1, std::vector<int>()); // kubelki

    d[s] = 0;
    bucket_idx[s] = 0;
    buckets[0].push_back(s);

    int S = 1;
    int k = 0;
    unsigned int *range_begin = (unsigned int*)malloc((K + 1)*sizeof(unsigned int)); // zakresy kubelkow
    range_begin[0] = 0;
    unsigned int x = 1;
    for (int i = 1; i <= K; i++) {
        range_begin[i] = x;
        x *= 2;
    }


    int a, old_bucket_idx, new_bucket_idx;
    long long old_val, new_val, min_val;
    unsigned int min_value;

    for (int bucket_inx = 0; bucket_inx < (int)buckets.size(); bucket_inx++) {
        if (!buckets[bucket_inx].empty()) {
            if (bucket_inx < 2) {
                int current_vertex = buckets[bucket_inx].back();
                buckets[bucket_inx].pop_back();

                if (d[current_vertex] < bucket_inx)
                    continue;

                for (std::list<Edge>::iterator j = edges[current_vertex].edge.begin(); j != edges[current_vertex].edge.end(); j++) {
                    if (d[j->neighbors] > bucket_inx + j->cost) {
                        d[j->neighbors] = bucket_inx + j->cost;
                        buckets[d[j->neighbors]%(maxCost + 1)].push_back(j->neighbors);
                    }
                }

            } else {
                min_value = UINT_MAX;

                for (auto bucket = buckets[k].begin(); bucket != buckets[k].end(); bucket++) {
                    if (d[*bucket] < min_value) {
                        min_value = d[*bucket];
                    }
                }

                range_begin[0] = min_value;
                unsigned int x = 1;
                for (int i = 1; i <= bucket_inx; i++) {
                    range_begin[i] = min_value + x;
                    x = 2 * x;
                }

                while (!buckets[bucket_inx].empty()) {
                    a = buckets[bucket_inx].back();
                    buckets[bucket_inx].pop_back();
                    new_bucket_idx = bucket_inx - 1;
                    while (range_begin[new_bucket_idx] > d[a]) {
                        new_bucket_idx--;
                    }
                    bucket_idx[a] = new_bucket_idx;
                    buckets[new_bucket_idx].push_back(a);
                }
            }
        }
    }
}