#include "DijkstraPathTracer.h"

DijkstraPathTracer::DijkstraPathTracer(
    shared_ptr<DijkstraGrid> _gr, double _xinit, double _yinit)
        : gr(_gr), n1d(_gr->n1d), h(_gr->h), 
          xinit(_xinit), yinit(_yinit) { 
    val.resize(n1d * n1d);
    for (int i = 0; i < n1d; ++i) {
        for (int j = 0; j < n1d; ++j) {
            val[i * n1d + j] = _gr->grid[i * n1d + j]->uValue;
        }
    }
}

double DijkstraPathTracer::fromNeighborValue(
    int ri, int rj, int i, int j) {
    int rad = (ri - i) * (ri - i) + (rj - j) * (rj - j);
    double dK = sqrt(static_cast<double>(rad)) * h * 
                gr->grid[i * n1d + j]->kValue;
    double unew = val[ri * n1d + rj] + dK;

    return unew;
}

// Bellman equation, check eight neighbors
void DijkstraPathTracer::minNextPoint(vector<int> &curr) {
    int i = curr[0], j = curr[1];
    double inext, jnext;
    double vmin = INFTY;
    if (i - 1 >= 0) {
        double vnew = fromNeighborValue(i - 1, j, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i - 1;
            curr[1] = j;
        }
    }
    if (i + 1 < n1d) {
        double vnew = fromNeighborValue(i + 1, j, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i + 1;
            curr[1] = j;
        }
    }
    if (j - 1 >= 0) {
        double vnew = fromNeighborValue(i, j - 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i;
            curr[1] = j - 1;
        }
    }
    if (j + 1 < n1d) {
        double vnew = fromNeighborValue(i, j + 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i;
            curr[1] = j + 1;
        }
    }
    if (i - 1 >= 0  && j - 1 >= 0) {
        double vnew = fromNeighborValue(i - 1, j - 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i - 1;
            curr[1] = j - 1;
        }
    }
    if (i + 1 < n1d && j - 1 >= 0) {
        double vnew = fromNeighborValue(i + 1, j - 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i + 1;
            curr[1] = j - 1;
        }
    }
    if (i - 1 >= 0  && j + 1 < n1d) {
        double vnew = fromNeighborValue(i - 1, j + 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i - 1;
            curr[1] = j + 1;
        }
    }
    if (i + 1 < n1d && j + 1 < n1d) {
        double vnew = fromNeighborValue(i + 1, j + 1, i, j);
        if (vnew < vmin) {
            vmin = vnew;
            curr[0] = i + 1;
            curr[1] = j + 1;
        }
    }
}

void DijkstraPathTracer::findOptimalPath() {
    int _xinit = xinit / h, _yinit = yinit / h;
    optimalPath.push_back({_xinit * h, _yinit * h});
    vector<int> curr{_yinit, _xinit};

    // In case the stopping condition is never satisfied
    int stepCount = 0, maxCount = n1d * n1d;
    
    while(!stoppingCondition(curr) && stepCount < maxCount) {   
        minNextPoint(curr);
        double xcurr = h * (double)curr[1], ycurr = h * (double)curr[0];
        optimalPath.push_back({xcurr, ycurr});
        stepCount++;
    }

    this->accurate_cost = 0;
    for (int i = 0; i < optimalPath.size() - 1; ++i) {
        double x = optimalPath[i][0], y = optimalPath[i][1];
        double dx = optimalPath[i + 1][0] - optimalPath[i][0];
        double dy = optimalPath[i + 1][1] - optimalPath[i][1];
        double d = sqrt(dx * dx + dy * dy);
        double edge_length = sqrt(dx * dx + dy * dy);
        dx = dx / d;
        dy = dy / d;
        double path_length = 0;
        double dK = 0, tau = 0.001;
        while (path_length < edge_length) {
            dK += tau * gr->analytic_cost(x + path_length * dx, 
                                          y + path_length * dy);
            path_length += tau;
        }
        accurate_cost += dK;
    }
}

bool DijkstraPathTracer::stoppingCondition(const vector<int> &curr) {
    return (curr[0] == 0 || curr[0] == n1d - 1 ||
            curr[1] == 0 || curr[1] == n1d - 1);
}