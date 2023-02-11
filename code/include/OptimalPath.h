/**
 * @file optimalPath.h
 * @author Dongping Qi ()
 * @brief 
 *      A path tracing function for Eikonal value function
 * @date 2022-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

void optimalNode(int N, double h, vector<double> &current, double *u, double tau) {
    // number of directions
    int W = 100;

    // angle spacing
    double dw = 2 * pi / (double)W;

    // center of all the testing locations
    double x = current[0];  
    double y = current[1];
    double xtau, ytau;

    /* 
        Coordinate: 
            xtau = Nx * h + x1, ytau = Ny * h + y1
        If (xtau, ytau) belongs to a cell, then the lower left node has index: 
            Nx * N + Ny + 1
    */
    int Nx, Ny;
    double xmin, ymin, xres, yres;

    double umin = DBL_MAX;
    double usw, unw, une, use, ubilinear;

    // bi-linear interpolation to approximate the minimum value
    for (int i = 0; i < W; i++)
    {
        xtau = x + tau * cos((double)i * dw);
        ytau = y + tau * sin((double)i * dw);
        Nx = xtau / h;  
        Ny = ytau / h;
        xres = xtau - (double)Nx * h;
        yres = ytau - (double)Ny * h;

        usw = u[Nx * N + Ny];           unw = u[Nx * N + Ny + 1];
        use = u[(Nx + 1) * N + Ny];     une = u[(Nx + 1) * N + Ny + 1];

        ubilinear = (1 - yres / h) * ((1 - xres / h) * usw + (xres / h) * use) + 
                        (yres / h) * ((1 - xres / h) * unw + (xres / h) * une);
        if(ubilinear < umin)
        {
            xmin = xtau;
            ymin = ytau;
            umin = ubilinear;
        }
    }
    current[0] = xmin;
    current[1] = ymin;
}

void optimalPath(int N, double h, double *u, double x0, double y0, vector<double> &pathX, vector<double> &pathY) {
    pathX.push_back(x0);  
    pathY.push_back(y0);
    vector<double> current{x0, y0};

    double tau = 0.1 * h;  
    int count = 0;
    // domain: [0,1] x [0,1]; stop if within the thin "strip" at boundary
    while (current[0] > tau && current[0] < 1 - tau && current[1] > tau && current[1] < 1 - tau && count < 5 / tau)
    {
        optimalNode(N, h, current, u, tau);
        pathX.push_back(current[0]);
        pathY.push_back(current[1]);
        count = count + 1;
    }
}