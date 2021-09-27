#ifndef MINHEAP_H_INCLUDED
#define MINHEAP_H_INCLUDED

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <limits.h>

using namespace std;

// Structure to represent a MinHeap node
struct MinHeapNode
{
    int node;
    double value;
};
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Structure to represent a MinHeap
struct MinHeap
{
    int hsize;      // Number of heap nodes present currently
    int capacity;   // Capacity of MinHeap
    int *pos;       // This is needed for decreaseKey()
    MinHeapNode **array;
};
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A utility function to create a new MinHeap Node
struct MinHeapNode* newMinHeapNode(int node, double value)
{
    // struct MinHeapNode* minHeapNode =
    //        (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    MinHeapNode* minHeapNode = new MinHeapNode;
    minHeapNode->node = node;
    minHeapNode->value = value;
    return minHeapNode;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A utility function to create a MinHeap
struct MinHeap* createMinHeap(int capacity)
{
    // struct MinHeap* minHeap =
    //      (struct MinHeap*) malloc(sizeof(struct MinHeap));
    MinHeap* minHeap = new MinHeap;
    // minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->pos = new int [capacity];
    minHeap->hsize = 0;
    minHeap->capacity = capacity;
    // minHeap->array =
    //      (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    minHeap->array = new MinHeapNode* [capacity];
    return minHeap;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A utility function to swap two nodes of MinHeap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2*idx + 1;
    right = 2*idx + 2;

    if (left < minHeap->hsize &&
        minHeap->array[left]->value < minHeap->array[smallest]->value )
      smallest = left;

    if (right < minHeap->hsize &&
        minHeap->array[right]->value < minHeap->array[smallest]->value )
      smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in MinHeap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->node] = idx;
        minHeap->pos[idxNode->node] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A utility function to check if the given MinHeap is empty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->hsize == 0;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->hsize - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->node] = minHeap->hsize-1;
    minHeap->pos[lastNode->node] = 0;

    // Reduce heap size and heapify root
    --minHeap->hsize;
    minHeapify(minHeap, 0);

    return root;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Function to decrease value value of a given vertex v. This function
// uses pos[] of MinHeap to get the current index of node in MinHeap
void decreaseKey(struct MinHeap* minHeap, int v, double value)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its value value
    minHeap->array[i]->value = value;

    // Travel up while the complete tree is not heapified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->value < minHeap->array[(i - 1)/2]->value)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->node] = (i - 1)/2;
        minHeap->pos[minHeap->array[(i - 1)/2]->node] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1)/2]);

        // move to parent index
        i = (i - 1)/2;
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// A utility function to check if a given vertex is in MinHeap
bool isInMinHeap(struct MinHeap *minHeap, int node)
{
    if (minHeap->pos[node] < minHeap->hsize)
        return true;
    return false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void deleteHeap(struct MinHeap *minHeap)
{
    delete [] minHeap->pos;
    delete [] minHeap->array;
    delete minHeap;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // MINHEAP_H_INCLUDED
