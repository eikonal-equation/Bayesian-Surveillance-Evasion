#ifndef INITIALCONDITION_H_INCLUDED
#define INITIALCONDITION_H_INCLUDED

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <limits.h>

using namespace std;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initialCondition(int N, int n, double *U, int *status, struct MinHeap *minHeap){

    // Set the initial status as "Far"
    for(int i = 0; i < n; i++){
        status[i] = 0;
    }

    // Set the initial value function as infinity
    for(int i = 0; i < n; i++){
        U[i] = DBL_MAX;
    }

    // lower edge
    for(int i = 0; i < N-1; i++){
        U[i] = 0.0;  status[i] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode(i, 0.0);
        minHeap->pos[i] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    // left edge
    for(int i = 1; i < N; i++){
        U[i*N] = 0.0;  status[i*N] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode(i*N, 0.0);
        minHeap->pos[i*N] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    // upper edge
    for(int i = 1; i < N; i++){
        U[(N-1)*N+i] = 0.0;  status[(N-1)*N+i] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode((N-1)*N+i, 0.0);
        minHeap->pos[(N-1)*N+i] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    // right edge
    for(int i = 0; i < N-1; i++){
        U[(i+1)*N-1] = 0.0;  status[(i+1)*N-1] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode((i+1)*N-1, 0.0);
        minHeap->pos[(i+1)*N-1] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // INITIALCONDITION_H_INCLUDED
