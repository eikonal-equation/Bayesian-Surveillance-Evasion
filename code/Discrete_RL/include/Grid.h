/**
 * @file Grid.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief This class is an abstract grid structure for holding parameters
          the value functions, running cost, speed, obstacles etc.
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <vector>

#include "MinHeap.h"
#include "specialConstants.h"

using std::vector;

/*
    A grid node struct containing:
    - status indicator
    - speed at this node
    - cost at this node
    - value function at this node
*/
struct GridNode {
    int status;
    double fValue;
    double kValue;
    double uValue;

    double ucbValue;
    double nCapture;
    double runTime;
    vector<double> qValue;
    double nVisit;
    vector<double> nAction;
};

class Grid {
 public:
    int n1d;                             // number of grid points in one direction
    double h;                            // grid size
    vector<GridNode *> grid;             // grid matrix
    vector<vector<int>> obstacles;       // lower & upper indices of several rectangles
    MinHeap *minHeap;                    // a general min heap
    int cost_index;

    // constructor
    Grid(int _n1d, const vector<vector<double>> &_obstacles = {});
    Grid(int _n1d, const int _cost_index = 0);

    // destructor
    ~Grid();

    // Check if (x, y) is inside one of the obstacles
    bool inObstacle(int x, int y); 

    // Obtain physical coordinates of (i, j) 
    inline vector<double> index2Physical(int i, int j) {
        return {(double)j * h, (double)i * h};
    } 

    // Cost function
    double analytic_cost(double x, double y);
};