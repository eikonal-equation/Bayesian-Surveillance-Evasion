#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>

#include "saveOutput.h"
#include "DijkstraGrid.h"
#include "DijkstraPathTracer.h"
#include "EvaderGrid.h"
#include "UCTGrid.h"

using namespace std;

void test_details() {
    int n1d = 21;
    int episodes = 15000;
    std::vector<double> source;
    int cost_index = 3;
    if (cost_index == 2) {
        source = {0.39, 0.61};
    } else if (cost_index == 3) {
        source = {0.82, 0.59};
    } else if (cost_index == 4) {
        source = {0.37, 0.37};
    }
    
    std::vector<double> visit_num(n1d * n1d, 1.0);
    std::vector<double> capture_num(n1d * n1d, 1.0);

    std::vector<double> actual_induced_cost(episodes, 0.0);
    int total_capture = 0;
    std::vector<int> capture_vec(episodes, 0);

    for (int i = 0; i < episodes; ++i) {
        auto eva = std::make_shared<EvaderGrid>(n1d, cost_index, episodes);
        eva->initializeZeroBoundary();
        eva->upper_confidence_update(visit_num, capture_num);

        eva->gridMarching();

        auto path = make_shared<DijkstraPathTracer>(eva, source[0], source[1]);
        path->findOptimalPath();
        actual_induced_cost[i] = path->accurate_cost;

        eva->simulate_capture_along_path(path, visit_num, capture_num);

        total_capture += eva->captured ? 1 : 0;
        capture_vec[i] = total_capture;

    }
    saveVector(actual_induced_cost, "Graph/actual_cost");
    saveVector(capture_vec, "Graph/capture_times");


    // UCT 
    auto gd = make_shared<UCTGrid>(n1d, source, cost_index);
    gd->uctLearning();
    saveVector(gd->capture_vec, "UCT/capture_times");

    gd->findUCBPath();
    saveVector(gd->optimalPath, "UCT/ucb_optimal_path");

    // vector<double> rough_source = {0.35, 0.35};
    // saveVector(rough_source, "source");
    // vector<vector<double>> ucbValue(n1d, vector<double>(n1d, 0));
    // for (int i = 0; i < n1d; i++) {
    //     for (int j = 0; j < n1d; j++) {
    //         ucbValue[i][j] = gd->grid[i * n1d + j] -> ucbValue;
    //     }
    // }
    // saveMatrix(gd->nFirstActions, "number_of_action");
    // saveMatrix(ucbValue, "ucb_value");
}

int main() { 
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    test_details();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Task takes " 
         << duration.count() / 1000000.0 << " seconds!" << endl;

    return 0;
} 