function [xfin,yfin,caught,cgrid,tgrid,ngrid,ntotal] = simluate_capture(path_x,path_y,real_cost,kn,kh,cgrid,tgrid,ngrid,tau)
% - This is a simulation of "capturing" along a path [path_x path_y] according to 'real_cost'
% - The function updates 3 statistical grids: 
%   cgrid: capture times in a cell
%   tgrid: total running time in a cell
%   ngrid: total visit times in a cell
% * The path is supposed to have step size tau 
    x0 = path_x(1);  y0 = path_y(1);  ntotal = 0;

    caught = false;
    
    % (nx,ny) is used for checking whether we moved across a grid cell. 
    % nstep keep track of how many steps we moved from last cell
    nx = floor(x0*(1/kh))+1;  ny = floor(y0*(1/kh))+1;
    nstep = 0;    ngrid(ny,nx) = ngrid(ny,nx) + 1;
    
    for i = 2:length(path_x)
        x_new = path_x(i);  y_new = path_y(i); ntotal = i;
        % - update tgrid if still inside the same cell
        %   or reset nstep if move across boundaries 
        nx_new = max(floor(x_new*(1/kh)),0)+1;  ny_new = max(floor(y_new*(1/kh)),0)+1;

        if nx_new == nx && ny_new == ny  % still inside the same cell
            nstep = nstep + 1;
        else  % run into a new cell
            tgrid(ny,nx) = tgrid(ny,nx) + nstep*tau;
            ngrid(ny,nx) = ngrid(ny,nx) + 1;
            nstep = 0;
            nx = nx_new;  ny = ny_new;
        end
        
        % P(capture) = K(x,y) * tau
        if rand < real_cost(x_new,y_new)*tau
            caught = true;
            xfin = x_new;  yfin = y_new;
            cgrid(ny_new,nx_new) = cgrid(ny_new,nx_new)+1;
            tgrid(ny_new,nx_new) = tgrid(ny_new,nx_new) + nstep*tau;
            break;
        end
    end 
    % reaching the boundary without being caught
    if ~caught
        tgrid(ny,nx) = tgrid(ny,nx) + nstep*tau;
        ngrid(ny,nx) = ngrid(ny,nx) + 1;
        xfin = path_x(end);
        yfin = path_y(end);
    end
end