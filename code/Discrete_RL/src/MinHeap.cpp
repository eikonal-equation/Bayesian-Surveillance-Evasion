#include <iostream>
#include <cfloat>

#include "MinHeap.h"

// Constructor: an empty min heap with tree & pos vectors initialized
MinHeap::MinHeap(int cap) {
    hsize = 0;
    capacity = cap;
    pos = new int[cap];
    tree = new Node*[cap];
}

// Destructor: destruct all created by new
MinHeap::~MinHeap() {
    delete[] pos;
    delete[] tree;
}

// Inserts a new node with index = id & value = val
void MinHeap::insertNode(int id, double val) {
    if (hsize == capacity) {
        std::cout << "Heap size overflow: cannot insert node." << std::endl;
        return;
    }
    // First insert the new node at the end
    tree[hsize] = new Node(id, INFTY);
    pos[id] = hsize;
    ++hsize;
    // Next push it down until tree structure satisfied
    decreaseNode(id, val);
}

// Decreases value of key at index 'i' to new_val
// It is assumed that new_val is smaller
void MinHeap::decreaseNode(int id, double new_val) {
    int i = pos[id];
    tree[i]->value = new_val;
    // Swap up the decreased Node
    while (i != 0 && tree[parent(i)]->value > tree[i]->value) {
        pos[tree[parent(i)]->index] = i;
        pos[tree[i]->index] = parent(i);
        std::swap(tree[i], tree[parent(i)]);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin() {
    if (hsize <= 0) {
        std::cout << "Empty heap: cannot extract root." << std::endl;
        return -1;
    }
    // Store the minimum value, i.e. tree root value
    int root = tree[0]->index;
    // Dijkstra guarantees that root never re-enter heap
    delete tree[0];

    // Replace root node with the last node
    Node* lastNode = tree[hsize - 1];
    tree[0] = lastNode;

    // Update position of the last node
    pos[lastNode->index] = 0;
    pos[root] = hsize - 1;

    // Reduce heap size and heapify root
    --hsize;
    minHeapify(0);

    return root;
}

// A recursive method to heapify a subtree with the root at index i
void MinHeap::minHeapify(int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    int smallest = i;
    if (l < hsize && tree[l]->value < tree[smallest]->value)
        smallest = l;
    if (r < hsize && tree[r]->value < tree[smallest]->value)
        smallest = r;
    if (smallest != i) {
        pos[tree[i]->index] = smallest;
        pos[tree[smallest]->index] = i;
        std::swap(tree[i], tree[smallest]);
        minHeapify(smallest);
    }
}

// check whether the min heap is empty
bool MinHeap::isEmpty() {
    return hsize == 0;
}