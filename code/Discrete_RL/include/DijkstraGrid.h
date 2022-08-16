/**
 * @file DijkstraGrid.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief This class is the standard Dijkstra's method to solve 
 * 		  the discrete shortest path problem on grid
 * @date 2022-07-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Grid.h"

#include <math.h>
#include <iostream>

// This Dijkstra's method does not maintain a mapping of edge cost
// Instead, edges emitting from the same node have the same cost
class DijkstraGrid : public Grid {
 public:
    /*  Inherited from Grid
    int n1d;                             // number of grid points in one direction
    double h;                            // grid size
    vector<GridNode *> grid;             // grid matrix
    vector<vector<int>> obstacles;       // lower & upper indices of several rectangles
    MinHeap* minHeap;                    // a general 1d min heap
    */
    DijkstraGrid(int _n1d, int _cost_index = 0);

    void initializeZeroBoundary();

    void updateNode(int i, int j, int ri, int rj);

    void gridMarching();
};