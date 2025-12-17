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

% One qudit
H = Hadamard(spin);
d = det(H);
dimension = 2*spin+1;

U = exp(-1i*angle(d)/dimension) * H;

psi_fin = U*v;

% % two qudit
% e_0 = 1/2 * [1 0 0 0].';
% e_1 = 1/2 * [0 1 0 0].';
% e_2 = 1/2 * [0 0 1 0].';
% e_3 = 1/2 * [0 0 0 1].';
% 
% v = e_0 + e_1 + e_2 + e_3;
% psi_fin = kron(v,v);


%%
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