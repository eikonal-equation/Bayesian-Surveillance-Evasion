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
void fastMarching(int N, double x0, double y0, double *K, double *U, vector<double> &pathX, vector<double> &pathY)
{
    double h = 1/((double)N - 1);
    int n = N*N;

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
    while (!isEmpty(minHeap)) {
        // Step one: Extract the root node of the heap and set its status as Accept
        struct MinHeapNode* minNode = extractMin(minHeap);
        minnode = minNode->node;
        status[minnode] = 2;

        int i = minnode / N;  int j = minnode - i * N;
        // Step two: Re-evaluate the neighbors of the minnode
        //     Warn: Make sure node is INSIDE domain
        if (j < N - 1) {
            reEvaluateNeighbor(minnode+1, N, h, U, status, minHeap, F[minnode+1]/K[minnode+1]);
        }
        if (j > 0) {
            reEvaluateNeighbor(minnode-1, N, h, U, status, minHeap, F[minnode-1]/K[minnode-1]);
        }
        if (i > 0) {
            reEvaluateNeighbor(minnode-N, N, h, U, status, minHeap, F[minnode-N]/K[minnode-N]);
        }
        if (i < N - 1) {
            reEvaluateNeighbor(minnode+N, N, h, U, status, minHeap, F[minnode+N]/K[minnode+N]);
        }
    }// End of while

    optimalPath(N, h, U, x0, y0, pathX, pathY);

    // Free all the heap allocations
    delete [] F;
    delete [] status;
    deleteHeap(minHeap);
}
// //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // SINGLESRC_F_H_INCLUDED
