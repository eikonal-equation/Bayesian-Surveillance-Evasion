/**
 * @file UCTGrid.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief 
 * @date 2022-07-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Grid.h"

#include <math.h>
#include <iostream>
#include <random>
#include <chrono>

class UCTGrid : public Grid {
 public:
    /*  Inherited from Grid
    int n1d;                             // number of grid points in one direction
    double h;                            // grid size
    vector<GridNode*> grid;              // grid matrix
    vector<vector<int>> obstacles;       // lower & upper indices of several rectangles
    MinHeap* minHeap;                    // a general 1d min heap
    */
    UCTGrid(int _n1d, vector<double> _source, int _nobs = 0);

    vector<vector<int>> neighbor;

    int max_episode;
    int cur_episode;
    vector<double> source;
    vector<int> capture_vec;
    int total_captures;

    vector<vector<int>> nFirstActions;

    bool atBoundary(int i, int j);

    int bestDirection(int i, int j);
    
    bool simulateCapture(int i, int j, int next_i, int next_j);
    double uctSearch(int i, int j);

    vector<vector<double>> optimalPath;
    void findUCBPath();

    void uctLearning();
};