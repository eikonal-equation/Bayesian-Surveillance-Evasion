#include "EvaderGrid.h"
#include "analyticFunctions.h"

EvaderGrid::EvaderGrid(
    const int _n1d, 
    const int _cost_index,
    const int _episodes)
        : DijkstraGrid(_n1d, _cost_index),
          episodes(_episodes),
          captured(false) { }

bool EvaderGrid::simulate_capture_along_edge(
    const std::vector<double> &emit,
    const std::vector<double> &receive) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::uniform_real_distribution<double> unif(0,1);

    double tau = h / 50.0;
    double dx = receive[0] - emit[0];
    double dy = receive[1] - emit[1];
    double edge_length = sqrt(dx * dx + dy * dy);
    dx = dx / edge_length;
    dy = dy / edge_length;

    double path_length = 0;

    while (!captured && path_length < edge_length) {
        double r = unif(rng); 
        if (r < tau * analytic_cost(emit[0] + path_length * dx, 
                                    emit[1] + path_length * dy)) {
            captured = true;
        }
        path_length += tau;
    }
    return captured;
}

void EvaderGrid::simulate_capture_along_path(
    const std::shared_ptr<DijkstraPathTracer> &tracer,
    std::vector<double> &visit_num,
    std::vector<double> &capture_num) {
    for (int k = 0; k < tracer->optimalPath.size() - 1; ++k) {
        double x = tracer->optimalPath[k][0];
        double y = tracer->optimalPath[k][1];
        int ix = static_cast<int>(x / h);
        int iy = static_cast<int>(y / h);
        visit_num[iy * n1d + ix] += 1.0;
        bool capture = simulate_capture_along_edge(
            tracer->optimalPath[k], tracer->optimalPath[k + 1]);
        if (capture) {
            capture_num[iy * n1d + ix] += 1.0;
            break;
        } 
    }
}

void EvaderGrid::upper_confidence_update(
    const std::vector<double> &visit_num,
    const std::vector<double> &capture_num) {
    for (int i = 0; i < n1d; ++i) {
        for (int j = 0; j < n1d; ++j) {
            double capture_ratio = 
                capture_num[i * n1d + j] / visit_num[i * n1d + j];
            double confidence_cost = std::max(minK, 
                capture_ratio - 0.5 * confidence_factor / sqrt(visit_num[i * n1d + j]));
            grid[i * n1d + j]->kValue = -log(1 - confidence_cost);
        }
    }
}