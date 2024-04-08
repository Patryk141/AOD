class Heap {
public:
    struct Node {
        int index;
        int value;
    };
    Node *arr;
    int capacity;
    int size;
    Heap(int cap);
    void MinHeapify(int i);
    int parent(int i);
    int left(int i);
    int right(int i);
    int extractMin();
    void decreaseKey(int ind, int new_val);
    int getMin();
    void deleteKey(int i);
    void insertKey(int index, int value);
};