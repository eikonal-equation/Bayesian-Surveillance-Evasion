function pwc_K = piecewise_constant_cost_xy(kn,kh,pwc_cost,x,y)
% - pwc_cost is a 2D array of size kn x kn
%   integrate over each cell to find an average K
% - This function can be viewed as a handle with 
%   variables (x,y) and value according to pwc_cost
    nx = min(floor(x*(1/kh))+1, kn);
    ny = min(floor(y*(1/kh))+1, kn);

    pwc_K = pwc_cost(sub2ind(size(pwc_cost),ny,nx));
end

