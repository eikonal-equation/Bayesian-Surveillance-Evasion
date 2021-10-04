#include <vector>
#include "mex.h"
#include "include/Main.h"
using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // output pointers
    double *U;
    double *path_x;  double *path_y;

    int N;
    N = mxGetScalar(prhs[0]);
    double x0;
    x0 = mxGetScalar(prhs[1]);
    double y0;
    y0 = mxGetScalar(prhs[2]);
    double *K;
    K = mxGetPr(prhs[3]);

    if (nlhs != 3 || nrhs != 4) {
        cout << "Error: The number of variables does not fit!" << endl;
    }

    // create output matrix
    plhs[0] = mxCreateDoubleMatrix(N, N, mxREAL);

    // get a pointer to the output data
    U = mxGetPr(plhs[0]);

    // create vectors for path coordinates
    vector<double> pathX;  vector<double> pathY;
    fastMarching(N, x0, y0, K, U, pathX, pathY);

    plhs[1] = mxCreateDoubleMatrix(1, pathX.size(), mxREAL);
    plhs[2] = mxCreateDoubleMatrix(1, pathY.size(), mxREAL);

    path_x = mxGetPr(plhs[1]);
    path_y = mxGetPr(plhs[2]);

    copy(pathX.begin(), pathX.end(), path_x);
    copy(pathY.begin(), pathY.end(), path_y);
}
