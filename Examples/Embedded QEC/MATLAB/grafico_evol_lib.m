close all
clear
clc

%%

generazione_input;

!cd .. && qdos --frequency_output=400 --errors_enabled=true --simulation_progres=1 EQEC_evol_libera.q

%%

time_ = load('../output/t.out');
A = load('../output/rho.out');

time = time_;

[N,M] = size(A);
[n,m] = size(time);

%%

dim = M / 2;

C_ = A(:,1 : dim);
D_ = A(:,dim + 1 : end);

%%

% vanno trasposte nei primi due indici per avere quelle corrette!!
C = reshape(C_', dim, dim, n);
D = reshape(D_', dim, dim, n);

%%

rho = C + 1i*D;

%%

[~,~,N_rho] = size(rho);

errors = zeros(1,N_rho-1);
t_T2 = zeros(1,N_rho-1);

psi_fin = v;

for i = 2:N_rho
   errors(i-1) = 1 - psi_fin'*rho(:,:,i).'*psi_fin;
   t_T2(i-1) = time(i)/T2(2);
end

openfig("../Images/E_t_T2.fig");
hold on
grid on

loglog(t_T2, errors);