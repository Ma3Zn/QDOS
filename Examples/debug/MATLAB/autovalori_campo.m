clear
clc

Sz1 = [0.5 0; 0 -0.5];
Sz2 = [0.5 0; 0 -0.5];
Sz3 = [0.5 0; 0 -0.5];

% Hamiltoniane
mu_b = 14 * 2 * pi;
g_1  = [2,2,2];
g_2  = [1.8, 1.8, 1.8];
g_3  = [2.2 2.2 2.2];
D    = [7.2 5 9] * 2 * pi;
B = 4;

k = 1;

for B = 0.25 : 0.025 : 4
    H1 = D(1) * Sz1 * Sz1 + g_1(3) * mu_b * B * Sz1;
    H2 = D(2) * Sz2 * Sz2 + g_2(3) * mu_b * B * Sz2;
    H3 = D(3) * Sz3 * Sz3 + g_3(3) * mu_b * B * Sz3;

    H =   kron(kron(H1,eye(2)),eye(2)) ...
        + kron(kron(eye(2),H2),eye(2)) ...
        + kron(eye(2),kron(eye(2),H3));

    aut(:, k) = eig(H);
    k = k + 1;
    
end


plot([0.25:0.025: 4], aut)