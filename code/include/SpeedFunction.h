#ifndef SPEEDFUNCTION_H_INCLUDED
#define SPEEDFUNCTION_H_INCLUDED

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <limits.h>
#define pi 3.141592653589793
using namespace std;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
double f(double x, double y){
    double z;
    z = 1.0;
    // z = 1 + 0.5*sin(4*pi*x)*sin(4*pi*y);
    // z = 0.5*x + 0.5;
    // z = 12*x + 2;
    // z = 5*x + 20*y + 2;
    return(z);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void speedData(int N, double h, double *F)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            double x = (double)j*h;
            double y = (double)i*h;
            F[i*N+j] = f(x, y);
        }
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif
