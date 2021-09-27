#ifndef OPTIMALPATH_H_INCLUDED
#define OPTIMALPATH_H_INCLUDED

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <limits.h>
#include <vector>
#define pi 3.141592653589793
using namespace std;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void optimalNode(int N, double h, vector<double> &current, double *U_int, double tau)
{
    // number of directions
    int W = 100;

    // angle spacing
    double dw = 2*pi/((double)W);

    // center of all the testing locations
    double x = current[0];  double y = current[1];
    double x_tau;  double y_tau;

    // node coordinates are x_tau = Nx*h + x1, y_tau = Ny*h + y1
    // if (x_tau,y_tau) belongs to a cell, then the lower left node has index: Nx*N + Ny+1
    int Nx, Ny;
    double x_min, y_min, x_res, y_res;
    
    double min_U = DBL_MAX;
    double uSW, uNW, uNE, uSE, bl_U;

    // bi-linear interpolation to approximate the minimum value
    for(int i = 0; i < W; i++)
    {
        x_tau = x + tau * cos((double)i*dw);
        y_tau = y + tau * sin((double)i*dw);
        Nx = x_tau/h;  Ny = y_tau/h;
        x_res = x_tau - (double)Nx*h;
        y_res = y_tau - (double)Ny*h;
        
        uSW = U_int[Nx*N+Ny];        uNW = U_int[Nx*N+Ny+1];
        uSE = U_int[(Nx+1)*N+Ny];    uNE = U_int[(Nx+1)*N+Ny+1];
        
        bl_U = (1 - y_res/h)*((1 - x_res/h)*uSW + (x_res/h)*uSE) + (y_res/h)*((1 - x_res/h)*uNW + (x_res/h)*uNE);
        if(bl_U < min_U)
        {
            x_min = x_tau;
            y_min = y_tau;
            min_U = bl_U;
        }
    }
    current[0] = x_min;
    current[1] = y_min;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void optimalPath(int N, double h, double *U_int, double x0, double y0, vector<double> &pathX, vector<double> &pathY)
{
    pathX.push_back(x0);  pathY.push_back(y0);
    vector<double> current{x0, y0};
    
    double tau = 0.1*h;  int count = 0;
    // domain: [0,1] x [0,1]; stop if within the thin "strip" at boundary
    while(current[0] > tau && current[0] < 1-tau && current[1] > tau && current[1] < 1-tau && count < 5/tau)
    {
        optimalNode(N, h, current, U_int, tau);
        pathX.push_back(current[0]);
        pathY.push_back(current[1]);
    
        count = count + 1;
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif // OPTIMALPATH_H_INCLUDED
