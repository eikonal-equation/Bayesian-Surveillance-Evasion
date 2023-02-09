function kernel = kernel_setup(kernel_type,shape,scale)

dist = @(x,y) (x(:,1)-y(:,1)').^2 + (x(:,2)-y(:,2)').^2;

if strcmp(kernel_type,'gauss')
    kernel = @(x,y) shape*exp(-dist(x,y)/scale);
elseif strcmp(kernel_type,'matern1')
    kernel = @(x,y) shape*exp(-sqrt(dist(x,y))/scale);
elseif strcmp(kernel_type,'matern3')
    kernel = @(x,y) shape*(1+sqrt(3*dist(x,y))/scale).*exp(-sqrt(3*dist(x,y))/scale);
elseif strcmp(kernel_type,'matern5')
    kernel = @(x,y) shape*(1+sqrt(5*dist(x,y))/scale+5*dist(x,y)/scale/scale/3).*exp(-sqrt(5*dist(x,y))/scale);
else
    disp('Unknown kernel type!')
    return 
end

end

