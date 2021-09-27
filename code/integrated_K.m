function int_K = integrated_K(path_x,path_y,real_K,tau)
% Integrate along a path to find total K using trapezoid rule
    path_K = real_K(path_x,path_y);
    int_K = sum(tau*(path_K(1:end-1)+path_K(2:end))/2);
end

 