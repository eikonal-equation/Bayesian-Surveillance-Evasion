#include "UCTGrid.h"


UCTGrid::UCTGrid(
    int _n1d, vector<double> _source, int _nobs)
        : Grid(_n1d, _nobs), source(_source) {
    max_episode = 15000;
    cur_episode = 0;
    neighbor = {{1, 0},  {0, 1},  {-1, 0}, {0, -1},
                {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

    for (int i = 0; i < n1d; i++) {
        for (int j = 0; j < n1d; j++) {
            grid[i * n1d + j]->qValue.reserve(8);
            grid[i * n1d + j]->nVisit = 1;
            grid[i * n1d + j]->nAction.reserve(8);
            for (int k = 0; k < 8; k++) {
                grid[i * n1d + j]->qValue[k] = 0;
                grid[i * n1d + j]->nAction[k] = 0;
            }
            if (inObstacle(j, i))    grid[i * n1d + j]->status = ACCEPTED;
            else    grid[i * n1d + j]->status = FAR;
        }
    }
    capture_vec = vector<int>(max_episode, 0);
    total_captures = 0;

    nFirstActions = vector<vector<int>>(max_episode, vector<int>(8, 0));
}

bool UCTGrid::atBoundary(int i, int j) {
    if (i == 0) {
        return true;
    } else if (j == 0) {
        return true;
    } else if (i == n1d - 1) {
        return true;
    } else if (j == n1d - 1) {
        return true;
    } else {
        return false;
    }
}

void UCTGrid::uctLearning() {
    int i = source[1] / h, j = source[0] / h;
    while (cur_episode < max_episode) {
        double q = uctSearch(i, j);
        for (int k = 0; k < 8; k++) {
            nFirstActions[cur_episode - 1][k] = grid[i * n1d + j]->nAction[k];
        }
    }
    for (int i = 0; i < n1d; i++) {
        for (int j = 0; j < n1d; j++) {
            double v = INFTY;
            for (int k = 0 ; k < neighbor.size(); k++) {
                if (grid[i * n1d + j]->qValue[k] < v) {
                    v = grid[i * n1d + j]->qValue[k];
                }
            }
            grid[i * n1d + j]->uValue = v;
        }
    }
}

bool UCTGrid::simulateCapture(int i, int j, int next_i, int next_j) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::uniform_real_distribution<double> unif(0,1);

    double tau = 0.001;

    double x = (double)j * h, y = (double)i * h;
    double dx = (double)next_j - (double)j, dy = (double)next_i - (double)i;
    double d = sqrt(dx * dx + dy * dy);
    double edge_length = sqrt(dx * dx + dy * dy) * h;
    dx = dx / d;
    dy = dy / d;

    bool capture = false;
    double path_length = 0;

    while (!capture && path_length < edge_length) {
        double r = unif(rng);
        if (r < tau * analytic_cost(x + path_length * dx, y + path_length * dy)) {
            capture = true;
        }
        path_length += tau;
    }

    return capture;
}

double UCTGrid::uctSearch(int i, int j) {
    if (atBoundary(i, j)) {
        capture_vec[cur_episode] = total_captures;
        cur_episode++;
        return 0;
    }

    int best_action = bestDirection(i, j);

    int curr_node = i * n1d + j;
    grid[curr_node]->nVisit += 1;

    int next_i = i + neighbor[best_action][0];
    int next_j = j + neighbor[best_action][1];

    grid[curr_node]->nAction[best_action] += 1;

    double q_tail;
    if (simulateCapture(i, j, next_i, next_j)) {
        ++total_captures;
        capture_vec[cur_episode] = total_captures;
        ++cur_episode;
        q_tail = 1.0;
        grid[curr_node]->qValue[best_action] += 
            (q_tail - grid[curr_node]->qValue[best_action]) /
             grid[curr_node]->nAction[best_action];
    } else {
        q_tail = uctSearch(next_i, next_j);
        grid[curr_node]->qValue[best_action] += 
            (q_tail - grid[curr_node]->qValue[best_action]) /
             grid[curr_node]->nAction[best_action];
    }
    return q_tail;
}

int UCTGrid::bestDirection(int i, int j) {
    int best_action = 0;
    double minQ = INFTY;
    // double Kij = grid[i * n1d + j]->kValue;
    double Kij = 0.0;
    double half_sq_two = sqrt(2.0);
    double alpha = 1.0;  // scaling factor for Q(s,a)

    double union_bound_factor = sqrt(10 * log(grid[i * n1d + j]->nVisit));
    int q_id = 0;
    if (i + 1 < n1d) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = union_bound_factor / sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 1;
    if (j + 1 < n1d) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = union_bound_factor / sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 2;
    if (i - 1 >= 0) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = union_bound_factor / sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 3;
    if (j - 1 >= 0) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = union_bound_factor / sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    // 8-stencil with 4 diagonal neighbors
    q_id = 4;
    if (i + 1 < n1d && j + 1 < n1d) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            return q_id;
        }
        double ucb_factor = 
            union_bound_factor / 
            sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = half_sq_two * h * Kij + 
                   grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 5;
    if (i - 1 >= 0 && j + 1 < n1d) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = 
            union_bound_factor / 
            sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = half_sq_two * h * Kij + 
                   grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 6;
    if (i - 1 >= 0 && j - 1 >= 0) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            return q_id;
        }
        double ucb_factor = 
            union_bound_factor / 
            sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = half_sq_two * h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }

    q_id = 7;
    if (i + 1 < n1d && j - 1 >= 0) {
        if (grid[i * n1d + j]->nAction[q_id] < 0.5) {
            // grid[i * n1d + j]->ucbValue = -100;
            return q_id;
        }
        double ucb_factor = union_bound_factor / sqrt(grid[i * n1d + j]->nAction[q_id]);
        double q = half_sq_two * h * Kij + grid[i * n1d + j]->qValue[q_id] - ucb_factor;
        if (q < minQ) {
            minQ = q;
            best_action = q_id;
            grid[i * n1d + j]->ucbValue = q;
        }
    }
    return best_action;
}

void UCTGrid::findUCBPath() {
    int i = source[1] / h, j = source[0] / h;
    optimalPath.push_back({(double)j * h, (double)i * h});
    int count = 0;
    while (!atBoundary(i, j) && count < 1000) {
        int best_action = bestDirection(i, j);
        int next_i = i + neighbor[best_action][0];
        int next_j = j + neighbor[best_action][1];
        i = next_i;
        j = next_j;
        optimalPath.push_back({(double)j * h, (double)i * h});
        count++;
    }
}