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
    for(int i=0; i<n; i++){
        status[i] = 0;
    }

    // Set the initial value function as infty
    for(int i=0; i<n; i++){
        U[i] = DBL_MAX;
    }
    
    // Outside domain is infinity
    for(int i=0; i<N; i++){
        status[i] = 2;
        status[i*N] = 2;
        status[N*N-N+i] = 2;
        status[(i+1)*N-1] = 2;
    }
    
    // Zero boundary condition
    for(int i=1; i<N-1; i++){
        U[i+N] = 0.0;  status[i+N] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode(i+N, 0.0);
        minHeap->pos[i+N] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    
    for(int i=1; i<N-1; i++){
        U[i*N+1] = 0.0;  status[i*N+1] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode(i*N+1, 0.0);
        minHeap->pos[i*N+1] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    
    for(int i=1; i<N-1; i++){
        U[N*N-2*N+i] = 0.0;  status[N*N-2*N+i] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode(N*N-2*N+i, 0.0);
        minHeap->pos[N*N-2*N+i] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
    
    for(int i=1; i<N-1; i++){
        U[(i+1)*N-2] = 0.0;  status[(i+1)*N-2] = 2;
        minHeap->array[minHeap->hsize] = newMinHeapNode((i+1)*N-2, 0.0);
        minHeap->pos[(i+1)*N-2] = 0;
        minHeap->hsize = minHeap->hsize + 1;
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // INITIALCONDITION_H_INCLUDED
