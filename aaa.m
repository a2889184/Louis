v = zeros(3, 1);

A = [1 2 3; 4 5 6; 7 8 9];
x = [8 5 2];

for i = 1:3
  for j = 1:3
    v(i) = v(i) + A(i, j) * x(j);
  end
end
v