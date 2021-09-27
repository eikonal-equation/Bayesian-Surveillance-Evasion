#ifndef SINGLESRC_F_H_INCLUDED
#define SINGLESRC_F_H_INCLUDED

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <limits.h>
#include <vector>

#include "MinHeap.h"
#include "SpeedFunction.h"
#include "InitialCondition.h"
#include "ReEvaluateNeighbor.h"
#include "OptimalPath.h"

using namespace std;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void fastMarching(int N, double x0, double y0, double *K, double *U_int, vector<double> &pathX, vector<double> &pathY)
{
    double h = 1/((double)N - 3);
    int n = N*N;
    double *U = new double [n];

    // Generate the speed function on each node ------------------------------------
    double *F = new double [n];
    speedData(N, h, F);

    // Generate the status indicator on each node ----------------------------------
    int *status = new int [n];    // 0:Far  1:Consider  2:Accept

    // Initialize the heap  --------------------------------------------------------
    struct MinHeap* minHeap = createMinHeap(n);
    minHeap->hsize = 0;

    // Initialize the boundary: ----------------------------------------------------
    initialCondition(N, n, U, status, minHeap);

    // MAIN LOOP -------------------------------------------------------------
    int minnode;
    while(!isEmpty(minHeap)){
        // Step one: Extract the root node of the heap and set its status as Accept
        struct MinHeapNode* minNode = extractMin(minHeap);
        minnode = minNode->node;
        status[minnode] = 2;

        int i;
        // Step two: Re-evaluate the neighbors of the minnode
        if (status[minnode+1] != 2) {
            i = (minnode+1)/N;
            reEvaluateNeighbor(minnode+1, N, h, U, status, minHeap, F[minnode+1]/K[minnode+1-2*i-N+1]);
        }
        if (status[minnode-1] != 2) {
            i = (minnode-1)/N;
            reEvaluateNeighbor(minnode-1, N, h, U, status, minHeap, F[minnode-1]/K[minnode-1-2*i-N+1]);
        }
        if (status[minnode-N] != 2) {
            i = (minnode-N)/N;
            reEvaluateNeighbor(minnode-N, N, h, U, status, minHeap, F[minnode-N]/K[minnode-N-2*i-N+1]);
        }
        if (status[minnode+N] != 2) {
            i = (minnode+N)/N;
            reEvaluateNeighbor(minnode+N, N, h, U, status, minHeap, F[minnode+N]/K[minnode+N-2*i-N+1]);
        }
    }// End of while
    
    //
    for(int i = 0; i < N-2; i++){
        for(int j = 0; j < N-2; j++){
            U_int[j*(N-2)+i] = U[(j+1)*N+(i+1)];
        }
    }

    optimalPath(N-2, h, U_int, x0, y0, pathX, pathY);

    // Free all the heap allocations 
    delete [] U;
    delete [] F;
    delete [] status;
    deleteHeap(minHeap);
}
// //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // SINGLESRC_F_H_INCLUDED
