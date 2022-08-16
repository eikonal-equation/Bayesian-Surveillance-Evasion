// Author & copyright to: Dongping Qi 

#pragma once

#include <math.h>

#include "specialConstants.h"

// analytic speed function
inline double f(double x, double y) {
    return 1.4 + 0.6 * cos(2 * PI * x) * sin(2 * PI * y);
}

// analytic cost functions
inline double K(double x, double y) {
    return 1.0 + 0.0 * sin(2 * PI * x) * sin(2 * PI * y);
}

inline double gauss(
    double x, double y, 
    double x0, double y0, 
    double sigma, double a) {
    double dist = (x - x0) * (x - x0) + (y - y0) * (y - y0);
    return a * exp( - dist / 2 / sigma / sigma ) 
        / (2.0 * M_PI * sigma * sigma);
}

// analytic surveillance intensity
inline double differentK(double x, double y, int nobs) {
    double realCost = 1.0 + 0.0 * sin(2 * PI * x) * sin(2 * PI * y);
    if (nobs == 2) {
        double xhat1 = 0.05, yhat1 = 0.5,  sigma1 = 0.3,  a1 = 2;
        double xhat2 = 0.8,  yhat2 = 0.9,  sigma2 = 0.3,  a2 = 2;
        realCost = gauss(x, y, xhat1, yhat1, sigma1, a1) 
                 + gauss(x, y, xhat2, yhat2, sigma2, a2);
    } else if (nobs == 3) {
        double sigma_id = 0.080, a_id = 0.35;
        double xhat1 = 0.92,  yhat1 = 0.50,  sigma1 = sigma_id,  a1 = a_id;
        double xhat2 = 0.82,  yhat2 = 0.71,  sigma2 = sigma_id,  a2 = a_id;
        double xhat3 = 0.82,  yhat3 = 0.29,  sigma3 = sigma_id,  a3 = a_id;
        double xhat4 = 0.615, yhat4 = 0.77,  sigma4 = sigma_id,  a4 = a_id;
        double xhat5 = 0.615, yhat5 = 0.23,  sigma5 = sigma_id,  a5 = a_id;
        double xhat6 = 0.40,  yhat6 = 0.81,  sigma6 = sigma_id,  a6 = a_id;
        double xhat7 = 0.40,  yhat7 = 0.19,  sigma7 = sigma_id,  a7 = a_id;
        double xhat8 = 0.20,  yhat8 = 0.50,  sigma8 = 1.2 * sigma_id;  
        double a8 = a_id;
        double xhat9 = 0.05,   yhat9 = 0.85,  sigma9 = 10 * sigma_id;
        double a9 = 0.02 * a_id;
        realCost = gauss(x, y, xhat1, yhat1, sigma1, a1) 
                 + gauss(x, y, xhat2, yhat2, sigma2, a2)
                 + gauss(x, y, xhat3, yhat3, sigma3, a3) 
                 + gauss(x, y, xhat4, yhat4, sigma4, a4)
                 + gauss(x, y, xhat5, yhat5, sigma5, a5) 
                 + gauss(x, y, xhat6, yhat6, sigma6, a6)
                 + gauss(x, y, xhat7, yhat7, sigma7, a7) 
                 + gauss(x, y, xhat8, yhat8, sigma8, a8)
                 + gauss(x, y, xhat9, yhat9, sigma9, a9);
    } else if (nobs == 4) {
        double sigma_id = 0.10,  a_id = 0.42;
        double xhat1 = 0.15,  yhat1 = 0.15,  sigma1 = sigma_id,  a1 = a_id;
        double xhat2 = 0.15,  yhat2 = 0.50,  sigma2 = sigma_id,  a2 = a_id;
        double xhat3 = 0.15,  yhat3 = 0.85,  sigma3 = sigma_id,  a3 = a_id;
        double xhat4 = 0.50,  yhat4 = 0.15,  sigma4 = sigma_id,  a4 = a_id;
        double xhat5 = 0.50,  yhat5 = 0.50,  sigma5 = 0.10,      a5 = 0.0;
        double xhat6 = 0.50,  yhat6 = 0.85,  sigma6 = sigma_id,  a6 = a_id;
        double xhat7 = 0.88,  yhat7 = 0.20,  sigma7 = 0.10;
        double a7 = a_id * sigma7 * sigma7 / sigma_id / sigma_id;
        double xhat8 = 0.85,  yhat8 = 0.50,  sigma8 = sigma_id,  a8 = a_id;
        double xhat9 = 0.85,  yhat9 = 0.85,  sigma9 = sigma_id,  a9 = a_id;
        realCost = gauss(x, y, xhat1, yhat1, sigma1, a1) 
                 + gauss(x, y, xhat2, yhat2, sigma2, a2)
                 + gauss(x, y, xhat3, yhat3, sigma3, a3) 
                 + gauss(x, y, xhat4, yhat4, sigma4, a4)
                 + gauss(x, y, xhat5, yhat5, sigma5, a5) 
                 + gauss(x, y, xhat6, yhat6, sigma6, a6)
                 + gauss(x, y, xhat7, yhat7, sigma7, a7) 
                 + gauss(x, y, xhat8, yhat8, sigma8, a8)
                 + gauss(x, y, xhat9, yhat9, sigma9, a9);
    }
    return realCost;
}