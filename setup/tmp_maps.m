%okrąg
r = 35;
n = 250;
a = r * cos([0:pi/n:2*pi]')+ 50;
b = r * sin([0:pi/n:2*pi]')+ 50;
% [a,b]

%IndianaPolis
a = 94 / 2;
b = 5 * a / 16;
r = 2 * b/4;
a = a - 2 * r;
b = b - 2 * r;
% d = 10;
%n = 
dt = 2/3;
rdt = -pi/80;
s = 1;

X = [0:dt:a];
Y = [(b + r) * ones(1,length(0:dt:a))];

alpha = 0;
X = [X, a + r * sin(alpha:-rdt:(alpha+pi/2))];
Y = [Y, b + r * cos(alpha:-rdt:(alpha+pi/2))];

% size(X)
% size(Y)
X = [X, (a + r) * ones(1,length(b:-dt:-b)) ];
Y = [Y, b:-dt:-b];
% size(X)
% size(Y)
alpha = alpha + pi /2;
X = [X, a + r * sin(alpha:-rdt:(alpha+pi/2))];
Y = [Y, -b + r * cos(alpha:-rdt:(alpha+pi/2))];
% size(X)
% size(Y)

X = [X, a:-dt:-a];
Y = [Y, -(b + r) * ones(1, length(a:-dt:-a))];
% size(X)
% size(Y)

alpha = alpha + pi /2;
X = [X, -a + r * sin(alpha:-rdt:(alpha+pi/2))];
Y = [Y, -b + r * cos(alpha:-rdt:(alpha+pi/2))];
% size(X)
% size(Y)

X = [X, -(a + r) * ones(1, length(-b:dt:b)) ];
Y = [Y, -b:dt:b];
% size(X)
% size(Y)

alpha = alpha + pi /2;
X = [X, -a + r * sin(alpha:-rdt:(alpha+pi/2))];
Y = [Y, b + r * cos(alpha:-rdt:(alpha+pi/2))];
% size(X)
% size(Y)

X = [X, -a:dt:0];
Y = [Y, (b + r) * ones(1, length(-a:dt:0))];
% size(X)
% size(Y)

% 50 + [X', Y']
N = length(X);
T = [linspace(0,s,N/4) linspace(s,0,N/4) 0  linspace(0,-s,N/4) linspace(-s,0,N/4)] ;

X = (X + T + 50)';
Y = (Y + T + 50)';
plot(X,Y);
axis("equal");
input("");
[X, Y]
