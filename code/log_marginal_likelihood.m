function [L,dL] = log_marginal_likelihood(y,mx,K_dist,D,a,l,kernel_type)
% Compute the value & gradients of log marginal likelihood of a GP regression problem.
% y:        input function values;
% mx:       m(x) at input x locations;
% K_dist:   ||x-x'|| as a matrix
% D:        x-dependent variance
% (a,l):    hyperparameters
% ************ Attention: minimize instead of maximize! ************
    if strcmp(kernel_type,'gauss')
        d_sig_a = exp(-K_dist/l);
        K_obs = a*d_sig_a + diag(D);
        d_sig_l = a*d_sig_a.*K_dist/l^2;
    elseif strcmp(kernel_type,'matern1')
        dist = sqrt(K_dist);
        d_sig_a = exp(-dist/l);
        K_obs = a*d_sig_a + diag(D);
        d_sig_l = a*d_sig_a.*dist/l^2;
    elseif strcmp(kernel_type,'matern3')
        dist = sqrt(K_dist);
        d_sig_a = (1+sqrt(3)*dist/l).*exp(-sqrt(3)*dist/l);
        K_obs = a*d_sig_a + diag(D); 
        d_sig_l = 3*a*exp(-sqrt(3)*dist/l).*K_dist/l^3;
    elseif strcmp(kernel_type,'matern5')
        dist = sqrt(K_dist);
        d_sig_a = (1+sqrt(5)*dist/l+5*K_dist/l/l/3).*exp(-sqrt(5)*dist/l);
        K_obs = a*d_sig_a + diag(D); 
        d_sig_l = 5*a*(1+sqrt(5)*dist/l).*exp(-sqrt(5)*dist/l).*K_dist/l^3/3;
    else
        disp('Unknown kernel type!')
        return 
    end
    
    
    v = K_obs\(y - mx);
    
    T_a = K_obs\d_sig_a;
    T_l = K_obs\d_sig_l;
    
    da = v'*d_sig_a*v/2 - trace(T_a)/2;
    dl = v'*d_sig_l*v/2 - trace(T_l)/2;

    C = chol(K_obs);
    % max(L) is equivalent to min(-L)
    L = (y - mx)'*v/2 + log(prod(diag(C))^2)/2;
    
    dL = [-da -dl];
end

