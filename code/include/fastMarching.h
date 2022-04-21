/**
 * @file fastMarching.h
 * @author Dongping Qi ()
 * @brief 
 *      A succinct Fast Marching Method implementation
 *      - MinHeap is implemented as a struct, not a class
 *      - Support only zero boundary condition now
 *      - Designed to be compiled by Matlab "mex" function 
 * @date 2022-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cmath>
#include <iostream>
#include <memory>
#include <limits>
#include <vector>

#define pi 3.141592653589793
using namespace std;

#include "MinHeap.h"
#include "zeroBoundary.h"
#include "updateNeighbor.h"
#include "optimalPath.h"



double f(double x, double y){
    double z;
    z = 1 + 0.0 * sin(4 * pi * x) * sin(4 * pi * y);
    return z;
}

void generateSpeed(int N, double h, double *F)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double x = (double)j * h;
            double y = (double)i * h;
            F[i * N + j] = f(x, y);
        }
    }
}

void fastMarching(int N, double x0, double y0, double *K, double *U, vector<double> &pathX, vector<double> &pathY)
{
    double h = 1 / ((double)N - 1);

    // Generate the speed value on each node
    double *F = new double[N * N];
    generateSpeed(N, h, F);

    // Generate the status indicator on each node 
    int *status = new int[N * N]{0};    // 0:FAR  1:CONSIDER  2:ACCEPT

    // Initialize the MinHeap  
    MinHeap* minHeap = createMinHeap(N * N);
    minHeap -> hsize = 0;

    // Initialize zero boundary
    zeroBoundary(N, U, status, minHeap);

    /* 
        Main loop starts 
    */
    int minnode;
    while (!isEmpty(minHeap)) 
    {
        // Step one: Extract the root node of minHeap and set its status as ACCEPT
        shared_ptr<MinHeapNode> minNode = extractMin(minHeap);
        minnode = minNode -> node;
        status[minnode] = 2;

        int i = minnode / N;  int j = minnode - i * N;
        // Step two: update the neighbors of the minnode
        //     Warn: make sure node is inside domain [0,1]^2
        if (j < N - 1) 
            updateNeighbor(minnode + 1, N, h, U, status, minHeap, F[minnode + 1] / K[minnode + 1]);
        if (j > 0) 
            updateNeighbor(minnode - 1, N, h, U, status, minHeap, F[minnode - 1] / K[minnode - 1]);
        if (i > 0) 
            updateNeighbor(minnode - N, N, h, U, status, minHeap, F[minnode - N] / K[minnode - N]);
        if (i < N - 1)
            updateNeighbor(minnode + N, N, h, U, status, minHeap, F[minnode + N] / K[minnode + N]);
    }

    optimalPath(N, h, U, x0, y0, pathX, pathY);

    // Free dynamically allocated memory
    delete [] F;
    delete [] status;
    deleteHeap(minHeap);
}