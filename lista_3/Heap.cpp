#include <iostream>
#include <climits>
#include "Heap.hpp"

void swap(Heap::Node *x, Heap::Node *y) {
    Heap::Node tmp = *x;
    *x = *y;
    *y = tmp;
}
Heap::Heap(int cap) {
    size = 0;
    capacity = cap;
    arr = (Node*)malloc(cap*sizeof(Node));;
}
void Heap::MinHeapify(int i) {
    int l = left(i);
    int r = right(i); 
    int smallest = i;
    if (l < size && arr[l].value < arr[i].value) {
        smallest = l;
    }
    if (r < size && arr[r].value < arr[smallest].value) {
        smallest = r;
    }
    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        MinHeapify(smallest);
    }
}
int Heap::parent(int i) {
    return (i - 1)/2;
}
int Heap::left(int i) {
    return 2*i + 1;
}
int Heap::right(int i) {
    return 2*i + 2;
}
int Heap::extractMin() {
    if (size <= 0) {
        return INT_MAX;
    }
    if (size == 1) {
        size--;
        return arr[0].index;
    }
    int root = arr[0].index;
    arr[0] = arr[size - 1];
    size--;
    MinHeapify(0);
    return root;
}
void Heap::decreaseKey(int ind, int new_val) {
    int i = 0;
    while (arr[i].index != ind && i < size) {
        i++;
    }
    arr[i].value = new_val;
    while (i != 0 && arr[parent(i)].value > arr[i].value) {
        swap(&arr[i], &arr[parent(i)]);
        i = parent(i);
    }
}
int Heap::getMin() {
    return arr[0].index;
}
void Heap::deleteKey(int i) {
    decreaseKey(i, INT_MIN);
    extractMin();
}
void Heap::insertKey(int index, int value) {
    if (size == capacity) {
        std::cout << "Overflow" << std::endl;
        return;
    }
    size++;
    int i = size - 1;
    arr[i].index = index;
    arr[i].value = value;
    while (i != 0 && arr[parent(i)].value > arr[i].value) {
        swap(&arr[i], &arr[parent(i)]);
        i = parent(i);
    }
}
