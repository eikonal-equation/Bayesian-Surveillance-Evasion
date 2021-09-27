function [mean_prd,stds] = gp_prediction(n,mean,x_acc,y_acc,x_prd,K_obs,K_o_p,K_prd)
% GP regression update function:
% - n x n: # of prediction pts;   mean: prior mean function;
% - x_acc: observation locations; y_acc: observed (noisy) value;
% - x_prd: prediction locations;  K_obs: K(x_acc,x_acc) + D;
% - K_o_p: K(x_prd,x_acc);        K_prd: K(x_prd,x_prd);
    mean_prd = mean(x_prd) + K_o_p*(K_obs\(y_acc - mean(x_acc)));
    Sigma_prd = K_prd - dot(K_o_p',K_obs\K_o_p');  
    
    mean_prd = reshape(mean_prd,[n n]);
    stds = reshape(sqrt(max(Sigma_prd,0.0)),[n n]);
end

