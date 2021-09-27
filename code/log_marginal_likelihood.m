function [L,dL] = log_marginal_likelihood(y,mx,K_dist,D,a,l)
% Compute the value & gradients of log marginal likelihood of a GP regression problem.
% y:        input function values;
% mx:       m(x) at input x locations;
% K_dist:   ||x-x'|| as a matrix
% D:        x-dependent variance
% (a,l):    hyperparameters
% ************ Attention: minimize instead of maximize! ************
    K_obs = a*exp(-K_dist/l) + diag(D);
    v = K_obs\(y - mx);
    d_sig_a = exp(-K_dist/l);
    d_sig_l = a*exp(-K_dist/l).*K_dist/l^2;
    T_a = K_obs\d_sig_a;
    T_l = K_obs\d_sig_l;
    da = v'*d_sig_a*v/2 - trace(T_a)/2;
    dl = v'*d_sig_l*v/2 - trace(T_l)/2;
    C = chol(K_obs);
    % max(L) <=> min(-L)
    L = (y - mx)'*v/2 + log(prod(diag(C))^2)/2;
    dL = [-da -dl];
end

