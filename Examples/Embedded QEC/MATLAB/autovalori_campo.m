% Generazione hamiltoniana
% Elettrone - Elettrone
% A_z  = -1 * 2 * pi;
% A_   = 0.5 * 2 * pi;
clear

mu_b = 14 * 2 * pi;
g_1  = [2,2,2];
g_2  = [2.9,2.9,4.2];
D    = 7.2 * 2 * pi;
B    = 1;

% 1 G = 1e-4 T
B1 = 0.005;

% Operatori di Spin
spin1 = 3/2;
spin2 = 1/2;

Sz1 = SZ(spin1);
Sz2 = SZ(spin2);

Sy1 = SY(spin1);
Sy2 = SY(spin2);

k = 1

for B = 0 : 0.025 : 4
    % Hamiltoniane
    H1 = g_1(3) * mu_b * B * Sz1 + D * Sz1 * Sz1;
    H2 = g_2(3) * mu_b * B * Sz2;
    
    H = kron(H1,eye(2)) + kron(eye(4),H2);

    aut(:, k) = eig(H);
    k = k + 1;
end

plot([0:0.025: 4], aut);

[v,m] = eig(H);
m
v