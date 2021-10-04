mex fastMarching.cpp

%% test fmm solver & plot the solution level sets
n = 401;  h = 1/(n-1);

% G_pd: fine prediction grid
x_prd_1d = linspace(0.0,1.0,n);  [X,Y] = meshgrid(x_prd_1d);
x_prd = [reshape(X,[n^2 1]) reshape(Y,[n^2 1])];

nobs = 4;
[real_cost,x0,y0,B] = intensity_setup(nobs);

[u,path_x,path_y] = fastMarching(n,x0,y0,real_cost(X,Y));

figure
contourf(x_prd_1d,x_prd_1d,u,25);
colorbar
hold on
line(path_x,path_y,'Linewidth',3,'Color','r','Linestyle', '-')
scatter(x0,y0,50,'c','o','filled')
hold off
axis image