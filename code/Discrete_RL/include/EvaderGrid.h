/**
 * @file EvaderGrid.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief This class implements the graph UCB algorithm for
 *        Surveillance Avoidance RL problem
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <cmath>
#include <vector>
#include <random>
#include <chrono>

#include "DijkstraGrid.h"
#include "DijkstraPathTracer.h"

class EvaderGrid : public DijkstraGrid {
 public:
    EvaderGrid(
        const int _n1d, 
        const int _cost_index,
        const int _episodes);
    const double minK = 1E-10;

    int episodes;
    const int node_num = (n1d - 2) * (n1d - 2);
    const double gamma = 0.1;
    const double confidence_factor = sqrt(
        log(static_cast<double>(episodes) *
            static_cast<double>(node_num)
            / gamma));
    bool captured;

    void upper_confidence_update(
        const std::vector<double> &visit_num,
        const std::vector<double> &capture_num);
    bool simulate_capture_along_edge(
        const std::vector<double> &emit,
        const std::vector<double> &receive);
    void simulate_capture_along_path(
        const std::shared_ptr<DijkstraPathTracer> &tracer,
        std::vector<double> &visit_num,
        std::vector<double> &capture_num);
};