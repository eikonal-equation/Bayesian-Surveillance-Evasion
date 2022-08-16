#include "DijkstraGrid.h"

DijkstraGrid::DijkstraGrid(int _n1d, int _cost_index)
    : Grid(_n1d, _cost_index) { }

// Initialize the boundary of [0,1] x [0,1] to be zero & ACCEPTED
// and push them into min heap
void DijkstraGrid::initializeZeroBoundary() {   
    for (int k = 0; k < n1d - 1; ++k) {
        grid[k]->uValue = 0.0;
        grid[k]->status = ACCEPTED;
        minHeap->insertNode(k, 0.0);

        grid[(k + 1) * n1d]->uValue = 0.0;
        grid[(k + 1) * n1d]->status = ACCEPTED;
        minHeap->insertNode((k + 1) * n1d, 0.0);

        grid[(k + 1) * n1d - 1]->uValue = 0.0;
        grid[(k + 1) * n1d - 1]->status = ACCEPTED;
        minHeap->insertNode((k + 1) * n1d - 1, 0.0);

        grid[(n1d - 1) * n1d + k + 1]->uValue = 0.0;
        grid[(n1d - 1) * n1d + k + 1]->status = ACCEPTED;
        minHeap->insertNode((n1d - 1) * n1d + k + 1, 0.0);
    }
}   

// A Dijkstra-like node update Method
// Only update CONSIDERED or FAR
// (ri, rj) is the root node 
// (i, j) is the to-be-updated node
void DijkstraGrid::updateNode(int i, int j, int ri, int rj) {
    if (grid[i * n1d + j]->status == ACCEPTED)  
        return;

    int rad = (ri - i) * (ri - i) + (rj - j) * (rj - j);
    double dK = sqrt(static_cast<double>(rad)) * h * 
                grid[i * n1d + j]->kValue;

    double unew = grid[ri * n1d + rj]->uValue + dK;
    grid[i * n1d + j]->uValue = std::min(grid[i * n1d + j]->uValue, unew);

    if (grid[i * n1d + j]->status == FAR) {
        grid[i * n1d + j]->status = CONSIDERED;
        minHeap->insertNode(i * n1d + j, grid[i * n1d + j]->uValue);
    } else {
        minHeap->decreaseNode(i * n1d + j, grid[i * n1d + j]->uValue);
    }
}

// main loop of marching, extract min as ACCEPTED each time
// assuming an 8-neighbor stencil
// some GridNode may not have 8 neighbors
void DijkstraGrid::gridMarching() {
    while (!minHeap->isEmpty()) {
        // Step one: extract the root of the heap and 
        // set its status as ACCEPTED 
        int root = minHeap->extractMin();
        int i = root / n1d, j = root - i * n1d;
        grid[root]->status = ACCEPTED;
        
        // Step two: update the neighbors of the minnode
        // Notice: need to check if the index is in domain
        if (i - 1 >= 0)
            updateNode(i - 1, j, i, j);
        if (i + 1 < n1d)
            updateNode(i + 1, j, i, j);
        if (j - 1 >= 0)
            updateNode(i, j - 1, i, j);
        if (j + 1 < n1d)
            updateNode(i, j + 1, i, j);
        if (i - 1 >= 0  && j - 1 >= 0)
            updateNode(i - 1, j - 1, i, j);
        if (i + 1 < n1d && j - 1 >= 0)
            updateNode(i + 1, j - 1, i, j);
        if (i - 1 >= 0  && j + 1 < n1d)
            updateNode(i - 1, j + 1, i, j);
        if (i + 1 < n1d && j + 1 < n1d)
            updateNode(i + 1, j + 1, i, j);
    }
}  