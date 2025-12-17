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

idx = 0;
base(1) = 0;
j = 2;

% post processing del tempo
for i = 1:n-1
    if (time(i) > time(i+1))
        idx(j-1) = i;
        base(j) = base(j-1) + time(i);
        j = j+1;
    end
end
idx(end+1) = n;
[k,nn] = size(idx);

for j = 2:nn
    for i = idx(j-1)+1:idx(j)
        time(i) = base(j) + time(i);
    end
end


%%

% psi_init = v;
% 
% err_init = 1 - psi_init'*rho(:,:,1).'*psi_init
% 
% %%
% % U_ = kron(eye(2), U);
% % psi_fin = aut'*U_*aut*v;
% % psi_fin = psi_fin(end:-1:1);
e_0 = [1, 0].';
e_1 = [0, 1].';
v = 1/sqrt(2)*(e_0 + e_1);
psi_fin = kron(kron(kron(v,v),v),v);

% Errore := 1 - F^2 con F^2 := <psi|rho|psi>
err_fin = 1 - psi_fin'*rho(:,:,end).'*psi_fin

%%
prob = zeros(n,dim);
prob_tot = zeros(n,1);

for i = 1:n
    for j = 1:dim
        prob(i,j) = rho(j,j,i);
        prob_tot(i) = prob_tot(i) + prob(i,j);
    end
end

%%
diag(psi_fin*psi_fin')
diag(rho(:,:,end).')

%%
psi_fin*psi_fin'
rho(:,:,end)
%%

figure(1)
hold on
grid on

plot(time, prob, time, prob_tot);
legend();
