function opt_int_K = accurate_optimal_integrated_K(real_cost,x0,y0)
% - To compute the integrated K along optimal path
%   on a finer grid
    n = 801;  h = 1/(n-1);  tau = 0.1*h;
    x_fine = linspace(0.0,1.0,n);  [X,Y] = meshgrid(x_fine);
    [~,path_x,path_y] = fastMarching(n,x0,y0,real_cost(X,Y));
    opt_int_K = integrated_K(path_x,path_y,real_cost,tau);
end

