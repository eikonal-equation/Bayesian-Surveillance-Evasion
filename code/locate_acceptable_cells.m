function agrid = locate_acceptable_cells(kn,kh,cgrid,tgrid,ngrid,agrid)
% - To make sure we accept an estimate in a cell, we need to check:
% 1) whether the trial time span is large enough:  tgrid > t_min      
% 2) whether there are enough trials:              ngrid > n_mim
% 3) whether a capture already took place:         cgrid > 1
% - log-button depends on the model assumption
    for i = 1:kn
        for j = 1:kn
            if cgrid(i,j) < 1 || tgrid(i,j) < kh || ngrid(i,j) < 20
                agrid(i,j) = 0;
            else
                agrid(i,j) = 1;
            end
        end
    end
end

