function ucb_K = ucb_update_pwc_K(n,h,kn,kh,K_var,K_est,ucb_K,ucb_factor,K_min)
% use a coarser estimate of K (cgrid/tgrid) to update guess_K    
    % stride size from a coarse grid to a finer grid
    r = (n-1)/kn;

    for i = 1:n
        for j = 1:n
            ki = floor((i-1)/r) + 1;
            kj = floor((j-1)/r) + 1;
            if ki > kn
                ki = ki - 1;
            end
            if kj > kn
                kj = kj - 1;
            end
            ucb_K(i,j) = K_est(ki,kj) - ucb_factor * sqrt(K_var(ki,kj));
        end
    end
    % truncation to K_min
    ucb_K = max(K_min,ucb_K);
end

