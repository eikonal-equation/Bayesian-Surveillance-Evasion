#include <vector>
#include "mex.h"
#include "include/fastMarching.h"
using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nlhs != 3) 
        mexErrMsgTxt("Wrong number of output arguments.\n");
    if (nrhs != 4)
        mexErrMsgTxt("Wrong number of input arguments.\n");
    // gather inputs
    int N;
    N = mxGetScalar(prhs[0]);
    double x0;
    x0 = mxGetScalar(prhs[1]);
    double y0;
    y0 = mxGetScalar(prhs[2]);
    double *K;
    K = mxGetPr(prhs[3]);
    
    // output pointers
    double *u;
    double *path_x;  
    double *path_y;

    // allocate value function as a matrix
    plhs[0] = mxCreateDoubleMatrix(N, N, mxREAL);

    // get a pointer to the output data
    u = mxGetPr(plhs[0]);

    // length of optimal path is unknown when compiling
    // use vector to hold optimal path first
    vector<double> pathX;  
    vector<double> pathY;
    fastMarching(N, x0, y0, K, u, pathX, pathY);
    
    // allocate optimal path
    plhs[1] = mxCreateDoubleMatrix(1, pathX.size(), mxREAL);
    plhs[2] = mxCreateDoubleMatrix(1, pathY.size(), mxREAL);

    path_x = mxGetPr(plhs[1]);
    path_y = mxGetPr(plhs[2]);

    copy(pathX.begin(), pathX.end(), path_x);
    copy(pathY.begin(), pathY.end(), path_y);
}
