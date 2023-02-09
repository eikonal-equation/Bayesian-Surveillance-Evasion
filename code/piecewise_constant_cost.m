function pwc_cost = piecewise_constant_cost(kn,kh,real_cost)
% - pwc_cost is a 2D array of size kn x kn
%   integrate over each cell to find an average K
    pwc_cost = zeros(kn,kn);

    for i = 1:kn
        for j = 1:kn
            xmin = (j-1)*kh;    xmax = xmin + kh;
            ymin = (i-1)*kh;    ymax = ymin + kh;
            pwc_cost(i,j) = integral2(real_cost,xmin,xmax,ymin,ymax)/(kh^2);
        end
    end
end

