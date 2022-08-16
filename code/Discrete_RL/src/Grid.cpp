#include "Grid.h"
#include "analyticFunctions.h"

/*
    Constructor of Grid:
          n1d:    number of grid points in one direction
    obstacles:    a vector of obstacle sw - ne corner coordinates
*/
Grid::Grid(
    int _n1d, const vector<vector<double>> &_obstacles) : 
    n1d(_n1d), h(1 / ((double)_n1d - 1)) {
    grid.resize(n1d * n1d);
    // Transform physical obstacles to index bounds
    for (const auto &obs : _obstacles) {
        vector<int> obs_index(4, 0);
        for (int i = 0; i < 4; ++i) {  
            obs_index[i] = static_cast<int>(obs[i] / h);
        }
        this->obstacles.push_back(obs_index);
    }

    // For loop to construct gridnode matrix 
    for (int i = 0; i < n1d; ++i) {
        for (int j = 0; j < n1d; ++j) {
            grid[i * n1d + j] = new GridNode;
            double x = (double)j * h;  
            double y = (double)i * h;  
            grid[i * n1d + j]->fValue = f(x, y);
            grid[i * n1d + j]->kValue = K(x, y);
            grid[i * n1d + j]->uValue = INFTY;
            grid[i * n1d + j]->status = inObstacle(j, i) 
                ? ACCEPTED : FAR;
        }
    }
    // Construct min heap: a total of n1d * n1d nodes on grid 
    minHeap = new MinHeap(n1d * n1d);
}

Grid::Grid(
    int _n1d, const int _cost_index) : 
    n1d(_n1d), h(1 / ((double)_n1d - 1)), 
    cost_index(_cost_index) {
    grid.resize(n1d * n1d);
    // For loop to construct gridnode matrix 
    for (int i = 0; i < n1d; ++i) {
        for (int j = 0; j < n1d; ++j) {
            grid[i * n1d + j] = new GridNode;
            double x = (double)j * h;  
            double y = (double)i * h;  
            grid[i * n1d + j]->fValue = f(x, y);
            grid[i * n1d + j]->kValue = differentK(x, y, cost_index);
            grid[i * n1d + j]->uValue = INFTY;
            grid[i * n1d + j]->status = inObstacle(j, i) 
                ? ACCEPTED : FAR;
        }
    }
    // Construct min heap: a total of n1d * n1d nodes on grid 
    minHeap = new MinHeap(n1d * n1d);
}

// destructor of Grid:
Grid::~Grid() {
    for (int i = 0; i < n1d; ++i) {
        for (int j = 0; j < n1d; ++j) {
            delete grid[i * n1d + j];
        }
    }
    delete minHeap;
}

// Check if (x, y) is inside one of the obstacles
bool Grid::inObstacle(int j, int i) {
    for (const auto &obs_index : obstacles) {
        /*
            Obstacles are considered as open sets
            - Pay attention especially when obstacle 
              boundaries adhere to the domain boundaries!!!
        */
        if (j > obs_index[0] && j < obs_index[2] &&
            i > obs_index[1] && i < obs_index[3]) {
            return true;
        }
    }
    return false;
}

double Grid::analytic_cost(double x, double y) {
    return differentK(x, y, cost_index);
}