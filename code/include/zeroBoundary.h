/**
 * @file zeroBoundary.h
 * @author Dongping Qi ()
 * @brief 
 *      Set up zero boundary condition for a square domain [0,1] x [0,1]
 * @date 2022-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

void zeroBoundary(int N, double *u, int *status, MinHeap *minHeap) {
    // Set the initial value function as infinity
    for (int i = 0; i < N * N; i++) {
        u[i] = DBL_MAX;
    }
    // lower edge
    for (int i = 0; i < N - 1; i++) {
        u[i] = 0.0;  
        status[i] = 2;
        minHeap -> array[minHeap -> hsize] = newMinHeapNode(i, 0.0);
        minHeap -> pos[i] = 0;
        minHeap -> hsize = minHeap -> hsize + 1;
    }
    // left edge
    for (int i = 1; i < N; i++) {
        u[i * N] = 0.0;  
        status[i * N] = 2;
        minHeap -> array[minHeap -> hsize] = newMinHeapNode(i * N, 0.0);
        minHeap -> pos[i * N] = 0;
        minHeap -> hsize = minHeap -> hsize + 1;
    }
    // upper edge
    for (int i = 1; i < N; i++) {
        u[(N - 1) * N + i] = 0.0;  
        status[(N - 1) * N + i] = 2;
        minHeap -> array[minHeap -> hsize] = newMinHeapNode((N - 1) * N + i, 0.0);
        minHeap -> pos[(N - 1) * N + i] = 0;
        minHeap -> hsize = minHeap -> hsize + 1;
    }
    // right edge
    for (int i = 0; i < N - 1; i++) {
        u[(i + 1) * N - 1] = 0.0;  
        status[(i + 1) * N - 1] = 2;
        minHeap -> array[minHeap -> hsize] = newMinHeapNode((i + 1) * N - 1, 0.0);
        minHeap -> pos[(i + 1) * N - 1] = 0;
        minHeap -> hsize = minHeap -> hsize + 1;
    }
}