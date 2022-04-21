mex fastMarching.cpp

%% parameters
episode = 50;
kn = 20;  gamma = 0.01;
ucb_factor = sqrt(log(episode*kn^2/gamma));
nobs = 4;
tune_param = true;


%% variables
n = 101;  h = 1/(n-1);

% G_pd: fine prediction grid
x_prd_1d = linspace(0.0,1.0,n);  [X,Y] = meshgrid(x_prd_1d);
x_prd = [reshape(X,[n^2 1]) reshape(Y,[n^2 1])];

% speed function: set to be 1 for simplicity
f = @(x,y) 1.0;  F = f(X,Y);

% intensity function & sources
[real_cost,x0,y0,B] = intensity_setup(nobs);

% G_ob: coarse grid to store estimate
kh = 1/kn;   
x_obs_1d = linspace(1/kn/2,1-1/kn/2,kn);  [xx,yy] = meshgrid(x_obs_1d);
x_obs = [reshape(xx,[kn^2 1]) reshape(yy,[kn^2 1])];

% statistics on G_ob
cgrid = zeros(kn,kn);  
tgrid = zeros(kn,kn);  
ngrid = zeros(kn,kn); 
agrid = zeros(kn,kn);

% path step size 
tau = 0.1*h;

% minimum integrated K along optimal path
opt_int_K = optimal_integrated_K(real_cost,x0,y0);


%% gaussian process parameters
mean = @(x) log(B)*ones(size(x,1),1);
scale = 0.02;  shape = 1.0;
dist = @(x,y) (x(:,1)-y(:,1)').^2 + (x(:,2)-y(:,2)').^2;
kernel = @(x,y) shape*exp(-dist(x,y)/scale);

% initialize Ktilde
K_est = zeros(kn,kn);

% prediction kernel matrix (fixed since G_pd is fixed)
K_prd = diag(kernel(x_prd,x_prd))';

% workspaces
mean_prd = reshape(mean(x_prd),[n n]);
stds_prd = zeros(n,n);
ctime = zeros(episode,1);  cn = 0;  % capture counts at run time
int_K = zeros(episode,1);  % a vector contains integrated K at each episode
xcaught = zeros(episode,1);  ycaught = zeros(episode,1);


%% main learning loop, an optimal path is found by FMM in each episode
for m = 1:episode
    % use ucb factor to modify gp predicted K
    ucb_K = exp(mean_prd - ucb_factor*stds_prd);
    
    % planning based on ucb modified K
    [u,path_x,path_y] = fastMarching(n,x0,y0,ucb_K);
    
    % integrate K along currently used path
    int_K(m) = integrated_K(path_x,path_y,real_cost,tau);
    
    % simulate the capture event 
    [xfin,yfin,caught,cgrid,tgrid,ngrid] = simluate_capture(path_x,path_y,real_cost,kn,kh,cgrid,tgrid,ngrid,tau);
    if caught
        xcaught(m) = xfin;    ycaught(m) = yfin;
        cn = cn + 1;
    else
        xcaught(m) = NaN;    ycaught(m) = NaN;
    end
    ctime(m) = cn;
    
    % update the estimated K & variance
    K_est = cgrid./tgrid;
    K_std = 1./cgrid;
    
    % locate cells with small variance
    agrid = locate_acceptable_cells(kn,kh,cgrid,tgrid,ngrid,agrid);
    idgrid = find(agrid == 1);
    if isempty(idgrid)  % use prior if none accepted
        mean_prd = reshape(mean(x_prd),[n n]);
        stds_prd = zeros(n,n);
    else
        x_acc = x_obs(idgrid,:);   
        y_acc = log(K_est(idgrid));
        std_acc = K_std(idgrid);
        K_obs = kernel(x_acc,x_acc) + diag(std_acc);
        K_o_p = kernel(x_prd,x_acc);
        [mean_prd,stds_prd] = gp_prediction(n,mean,x_acc,y_acc,x_prd,K_obs,K_o_p,K_prd);
    end
    
    % tune the hyperparameters every 1000 episode
    if tune_param  &&  m > 1000  &&  mod(m,1000) == 1  &&  length(idgrid) > 10
        lb = [1e-10,1e-10];
        lml = @(p) log_marginal_likelihood(y_acc,mean(x_acc),dist(x_acc,x_acc),std_acc,p(1),p(2));
        options = optimoptions('fmincon','SpecifyObjectiveGradient',true);
        [pm,~] = fmincon(lml,[shape,scale],[],[],[],[],lb,[],[],options);
        shape = pm(1);  scale = pm(2);
        kernel = @(x,y) shape*exp(-dist(x,y)/scale);
        K_prd = diag(kernel(x_prd,x_prd))';
    end
end


%% learning results: 3 difference paths
mean_prd_zero_ucb = exp(mean_prd);
[u_zero_ucb,path_x_zero_ucb,path_y_zero_ucb] = fastMarching(n,x0,y0,mean_prd_zero_ucb);
[u_ucb,path_x_ucb,path_y_ucb] = fastMarching(n,x0,y0,ucb_K);
[u_free,path_x_free,path_y_free] = fastMarching(n,x0,y0,real_cost(X,Y));


%% plottings 
figure
set(gcf, 'Position', get(0, 'Screensize'));
subplot(2,4,1)
hold on
% imagesc(x_prd_1d,x_prd_1d,real_cost(X,Y))
imagesc(x_prd_1d,x_prd_1d,u_free)
line(path_x_free,path_y_free,'Linewidth',3,'Color','r','Linestyle', '-')
scatter(x0,y0,50,'c','o','filled')
colorbar
cllim = caxis;
hold off
axis image
title('true $K$','fontsize',15,'interpreter','latex')

subplot(2,4,2)
hold on
imagesc(x_prd_1d,x_prd_1d,mean_prd_zero_ucb);
caxis(cllim)
colorbar
scatter(xcaught,ycaught,2.5,'y','o','filled')
scatter(x0,y0,50,'c','o','filled')
line(path_x_zero_ucb,path_y_zero_ucb,'Linewidth',3,'Color','k','Linestyle', '-')
line(path_x_ucb,path_y_ucb,'Linewidth',3,'Color','g','Linestyle', '-')
line(path_x_free,path_y_free,'Linewidth',3,'Color','r','Linestyle', '-')
hold off
set(gca,'xtick',0.0:0.2:1)
set(gca,'ytick',0.0:0.2:1)
axis image
title(['$\exp(M)$, $l$=' num2str(scale) ', $a$=' num2str(shape)],'fontsize',15,'interpreter','latex')


subplot(2,4,3)
[~,hc] = contour(x_prd_1d,x_prd_1d,u_free,25);
colorbar
cllim_u = caxis;
set(hc,'LineWidth',2);
hold on
line(path_x_free,path_y_free,'Linewidth',3,'Color','r','Linestyle', '-')
scatter(x0,y0,50,'c','o','filled')
hold off
axis image
title('true $u$; optimal path','fontsize',15,'interpreter','latex')


subplot(2,4,4)
[~,hc] = contour(x_prd_1d,x_prd_1d,u_zero_ucb,20);
colorbar
% caxis(cllim_u)
set(hc,'LineWidth',2);
hold on
scatter(x0,y0,50,'c','o','filled')
line(path_x_zero_ucb,path_y_zero_ucb,'Linewidth',3,'Color','k','Linestyle', '-')
line(path_x_ucb,path_y_ucb,'Linewidth',3,'Color','g','Linestyle', '-')
line(path_x_free,path_y_free,'Linewidth',3,'Color','r','Linestyle', '-')
hold off
set(gca,'xtick',0.0:0.2:1)
set(gca,'ytick',0.0:0.2:1)
axis image
title('predicted $u$','fontsize',15,'interpreter','latex')


subplot(2,4,5)
hold on
imagesc(x_prd_1d,x_prd_1d,stds_prd)
colorbar
scatter(x0,y0,50,'c','o','filled')
line(path_x_zero_ucb,path_y_zero_ucb,'Linewidth',3,'Color','k','Linestyle', '-')
line(path_x_ucb,path_y_ucb,'Linewidth',3,'Color','g','Linestyle', '-')
line(path_x_free,path_y_free,'Linewidth',3,'Color','r','Linestyle', '-')
hold off
set(gca,'xtick',0.0:0.2:1)
set(gca,'ytick',0.0:0.2:1) 
axis image
title(['$\rho$, $\gamma=$' num2str(ucb_factor)],'fontsize',15,'interpreter','latex')


subplot(2,4,7)
expect_risk = 1 - exp(-opt_int_K);
actual_risk = 1 - exp(-int_K);
averag_risk = zeros(episode,1);
for i = 1:length(int_K)
     averag_risk(i) = sum(actual_risk(1:i))/i - expect_risk;
end   
hold on
plot(1:episode,averag_risk,'LineWidth',3)
hold off
xlabel('episode')
title('averaged excess risk','fontsize',15,'interpreter','latex')


int_K_zero_ucb = integrated_K(path_x_zero_ucb,path_y_zero_ucb,real_cost,tau);
actual_risk_zero_ucb = 1 - exp(-int_K_zero_ucb);
subplot(2,4,6)
hold on
plot(1:episode,log10((actual_risk - expect_risk)/expect_risk),'LineWidth',3)
plot(1:episode,log10((actual_risk_zero_ucb - expect_risk)/expect_risk)*ones(1,episode),'Color','k','LineWidth',2)
plot(1:episode,log10((actual_risk(end) - expect_risk)/expect_risk)*ones(1,episode),'Color','g','LineWidth',2)
hold off
xlabel('episode')
legend('episodic','zero ucb','ucb')
title('$\log_{10}$(marginal excess risk)','fontsize',15,'interpreter','latex')


subplot(2,4,8)
hold on
plot(1:episode,ctime'./(1:episode),'LineWidth',3)
plot(1:episode,expect_risk*ones(1,episode),'Color','r','LineWidth',3)
hold off
ylim([0.0 1.0])
xlabel('episode')
legend('actual','expected')
title('observed capture probabilities','fontsize',15,'interpreter','latex')