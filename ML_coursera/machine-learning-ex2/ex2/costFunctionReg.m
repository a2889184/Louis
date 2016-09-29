function [J, grad] = costFunctionReg(theta, X, y, lambda)
%COSTFUNCTIONREG Compute cost and gradient for logistic regression with regularization
%   J = COSTFUNCTIONREG(theta, X, y, lambda) computes the cost of using
%   theta as the parameter for regularized logistic regression and the
%   gradient of the cost w.r.t. to the parameters. 

% Initialize some useful values
m = length(y); % number of training examples

% You need to return the following variables correctly 
J = 0;
grad = zeros(size(theta));

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the cost of a particular choice of theta.
%               You should set J to the cost.
%               Compute the partial derivatives and set grad to the partial
%               derivatives of the cost w.r.t. each parameter in theta

y_and_one_substract_y = [y [1 - y]];
h_of_X = X * theta;
log_h_of_x_and_one_substract_h_of_x = log([h_of_X 1 - h_of_X]); 

J = (-1 / m) .* sum(y_and_one_substract_y * log_h_of_x_and_one_substract_h_of_x') + (lambda / (2 * m)) * sum(theta .^ 2);

theta_index0_is0 = theta;
theta_index0_is0(1) = 0;
grad = (1 / m) .* X' * [h_of_X-y] + (lambda / m) * theta_index0_is0;

% =============================================================

end
