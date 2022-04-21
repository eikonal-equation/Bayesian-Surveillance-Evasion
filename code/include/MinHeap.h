/**
 * @file MinHeap.h
 * @author Dongping Qi ()
 * @brief 
 *      A customized minimum heap data structure
 * @date 2022-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

struct MinHeapNode
{
    int node;
    double value;
};

struct MinHeap
{
    // number of heap nodes present currently
    int hsize;                          
    // capacity of MinHeap, large enough to hold all nodes
    int capacity;                       
    // a map between node and its index in array
    int *pos;                           
    // an array to hold all nodes
    shared_ptr<MinHeapNode> *array;
};

// Construct a new MinHeapNode
shared_ptr<MinHeapNode> newMinHeapNode(int node, double value)
{
    auto minHeapNode = make_shared<MinHeapNode>();
    minHeapNode -> node = node;
    minHeapNode -> value = value;
    return minHeapNode;
}

// Construct a MinHeap
MinHeap* createMinHeap(int capacity)
{
    MinHeap *minHeap = new MinHeap;
    minHeap -> pos = new int[capacity];
    minHeap -> hsize = 0;
    minHeap -> capacity = capacity;
    minHeap -> array = new shared_ptr<MinHeapNode>[capacity];
    return minHeap;
}

// Swap two nodes of MinHeap. Needed for min heapify
void swapMinHeapNode(shared_ptr<MinHeapNode> & node_a, shared_ptr<MinHeapNode> & node_b)
{
    shared_ptr<MinHeapNode> temp = node_a;
    node_a = node_b;
    node_b = temp;
}

/*
    Heapify at a given idx
    - Need to updates pos of nodes when swapping
*/
void minHeapify(MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left =  2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap -> hsize &&
        minHeap -> array[left] -> value < minHeap -> array[smallest] -> value)
        smallest = left;

    if (right < minHeap -> hsize &&
        minHeap -> array[right] -> value < minHeap -> array[smallest] -> value)
        smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in MinHeap
        auto smallestNode = minHeap -> array[smallest];
        auto idxNode = minHeap -> array[idx];

        // Swap pos        
        minHeap -> pos[smallestNode -> node] = idx;
        minHeap -> pos[idxNode -> node] = smallest;

        // Swap nodes
        swapMinHeapNode(minHeap -> array[smallest], minHeap -> array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// Check if the given MinHeap is empty or not
bool isEmpty(MinHeap *minHeap)
{
    return minHeap -> hsize == 0;
}

// Extract minimum node from MinHeap
shared_ptr<MinHeapNode> extractMin(MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return nullptr;

    // Store the root node
    shared_ptr<MinHeapNode> root = minHeap -> array[0];

    // Replace root node with the last node
    shared_ptr<MinHeapNode> lastNode = minHeap -> array[minHeap -> hsize - 1];
    minHeap -> array[0] = lastNode;

    // Update position of the last node
    minHeap -> pos[root -> node] = minHeap -> hsize - 1;
    minHeap -> pos[lastNode -> node] = 0;

    // Reduce heap size and heapify root
    minHeap -> hsize -= 1;
    minHeapify(minHeap, 0);

    return root;
}

/* 
    Decrease value of a given node
    - Use pos[] to get the index of node in MinHeap array
*/
void decreaseKey(MinHeap *minHeap, int node, double value)
{
    // Get the index of node in MinHeap array
    int i = minHeap -> pos[node];

    // Get the node and update its value
    minHeap -> array[i] -> value = value;

    int p = (i - 1) / 2;
    // Travel up while the tree is not completely heapified
    while (i && minHeap -> array[i] -> value < minHeap -> array[p] -> value)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap -> array[i] -> node] = p;
        minHeap->pos[minHeap -> array[p] -> node] = i;
        swapMinHeapNode(minHeap->array[i], minHeap->array[p]);

        // Move to parent index
        i = p;
        p = (p - 1) / 2;
    }
}

// Destruct the MinHeap
void deleteHeap(MinHeap *minHeap)
{
    delete [] minHeap->pos;
    delete [] minHeap->array;
    delete minHeap;
}
