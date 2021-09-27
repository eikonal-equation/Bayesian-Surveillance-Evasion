function [real_cost,x0,y0,B] = intensity_setup(nobs)
% a function generating the intensity function K(x,y) according to
% different numbers of observers
    % a general gaussian-peaked analytic running cost
    gauss_cost = @(x,y,x0,y0,sig,a) a*exp(-((x-x0).^2+(y-y0).^2)/(2*sig^2))/(2*pi*sig^2);
    
    if nobs == 1
        % single-peak cost
        xhat = 0.05;  yhat = 0.5;  sigma = 0.4;  a = 4;
        real_cost = @(x,y) gauss_cost(x,y,xhat,yhat,sigma,a);  
        B = integral2(real_cost,0.0,1.0,0.0,1.0);
        x0 = 0.39;  y0 = 0.61;
    elseif nobs == 2
        % double-peak cost
        xhat  = 0.05;  yhat  = 0.50;  sigma  = 0.3;  a = 2;
        xhat2 = 0.80;  yhat2 = 0.90;  sigma2 = 0.3;  a2 = 2;
        real_cost = @(x,y) gauss_cost(x,y,xhat,yhat,sigma,a) + gauss_cost(x,y,xhat2,yhat2,sigma2,a2);  
        B = integral2(real_cost,0.0,1.0,0.0,1.0);
        x0 = 0.39;  y0 = 0.61;
    elseif nobs == 3
        % 8-peak cost, with one weak observer
        sigma_id = 0.078;  a_id = 0.35;
        xhat1 = 0.92;  yhat1 = 0.50;  sigma1 = sigma_id;  a1 = a_id;
        xhat2 = 0.83;  yhat2 = 0.70;  sigma2 = sigma_id;  a2 = a_id;
        xhat3 = 0.83;  yhat3 = 0.30;  sigma3 = sigma_id;  a3 = a_id;
        xhat4 = 0.63;  yhat4 = 0.75;  sigma4 = sigma_id;  a4 = a_id;
        xhat5 = 0.63;  yhat5 = 0.25;  sigma5 = sigma_id;  a5 = a_id;
        xhat6 = 0.43;  yhat6 = 0.81;  sigma6 = sigma_id;  a6 = a_id;
        xhat7 = 0.43;  yhat7 = 0.19;  sigma7 = sigma_id;  a7 = a_id;
        xhat8 = 0.20;  yhat8 = 0.50;  sigma8 = 1.3*sigma_id;  a8 = a_id;
        real_cost = @(x,y) gauss_cost(x,y,xhat1,yhat1,sigma1,a1) + gauss_cost(x,y,xhat2,yhat2,sigma2,a2) +...
            gauss_cost(x,y,xhat3,yhat3,sigma3,a3) + gauss_cost(x,y,xhat4,yhat4,sigma4,a4) +...
            gauss_cost(x,y,xhat5,yhat5,sigma5,a5) + gauss_cost(x,y,xhat6,yhat6,sigma6,a6) +...
            gauss_cost(x,y,xhat7,yhat7,sigma7,a7) + gauss_cost(x,y,xhat8,yhat8,sigma8,a8);  
        B = integral2(real_cost,0.0,1.0,0.0,1.0);
        x0 = 0.82;  y0 = 0.59;
    else
        % 8-peak cost, with one displaced observer
        sigma_id = 0.10;  a_id = 0.42;
        xhat1 = 0.15;  yhat1 = 0.15;  sigma1 = sigma_id;  a1 = a_id;
        xhat2 = 0.15;  yhat2 = 0.50;  sigma2 = sigma_id;  a2 = a_id;
        xhat3 = 0.15;  yhat3 = 0.85;  sigma3 = sigma_id;  a3 = a_id;
        xhat4 = 0.50;  yhat4 = 0.15;  sigma4 = sigma_id;  a4 = a_id;
        xhat5 = 0.50;  yhat5 = 0.50;  sigma5 = 0.10;      a5 = 0.0;
        xhat6 = 0.50;  yhat6 = 0.85;  sigma6 = sigma_id;  a6 = a_id;
        xhat7 = 0.88;  yhat7 = 0.20;  sigma7 = 0.10;      a7 = a_id*sigma7^2/sigma_id^2;
        xhat8 = 0.85;  yhat8 = 0.50;  sigma8 = sigma_id;  a8 = a_id;
        xhat9 = 0.85;  yhat9 = 0.85;  sigma9 = sigma_id;  a9 = a_id;
        real_cost = @(x,y) gauss_cost(x,y,xhat1,yhat1,sigma1,a1) + gauss_cost(x,y,xhat2,yhat2,sigma2,a2) + ...
            gauss_cost(x,y,xhat3,yhat3,sigma3,a3) + gauss_cost(x,y,xhat4,yhat4,sigma4,a4) + ...
            gauss_cost(x,y,xhat5,yhat5,sigma5,a5) + gauss_cost(x,y,xhat6,yhat6,sigma6,a6) + ...
            gauss_cost(x,y,xhat7,yhat7,sigma7,a7) + gauss_cost(x,y,xhat8,yhat8,sigma8,a8) + ...
            gauss_cost(x,y,xhat9,yhat9,sigma9,a9);  
        B = integral2(real_cost,0.0,1.0,0.0,1.0);
        x0 = 0.37;  y0 = 0.37;
    end
end

