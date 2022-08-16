/**
 * @file DijkstraPathTracer.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief This class is graph path tracer for dijkstra's method.
 * @date 2022-07-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "DijkstraGrid.h"

#include <memory>
#include <math.h>
using namespace std;

class DijkstraPathTracer {
 public:
    // Members in Grid, need to be initialized from Grid
    int n1d;                             // number of grid points in one direction
    double h;                            // grid size
    vector<double> val;                  // tracing val's gradient
    shared_ptr<DijkstraGrid> gr;

    // optimal path
    vector<vector<double>> optimalPath;
    double accurate_cost;

    // initial point
    double xinit;
    double yinit;

    // constructor
    DijkstraPathTracer(shared_ptr<DijkstraGrid> _gr, double _xinit, double _yinit);

    double fromNeighborValue(int ri, int rj, int i, int j);

    // Find the minimum next point around curr 
    void minNextPoint(vector<int> &curr);
    
    void findOptimalPath();

    // a stopping condition for DijkstraPathTracer
    bool stoppingCondition(const vector<int> &curr);
};