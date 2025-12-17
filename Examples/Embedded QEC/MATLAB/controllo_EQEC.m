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

prob = zeros(n,dim);
prob_tot = zeros(n,1);

for i = 1:n
    for j = 1:dim
        prob(i,j) = rho(j,j,i);
        prob_tot(i) = prob_tot(i) + prob(i,j);
    end
end

%%

figure(1)
hold on
grid on

plot(time, prob, time, prob_tot);
legend();
%%
S = spin1;

zero_logico = zeros(dim,1);

zero_logico(2) = sqrt((factorial(2*S)/(factorial(1) * factorial(2*S - 1))));
zero_logico(4) = 1;

zero_logico = 1/sqrt(2^(2*S - 1)) * zero_logico;

% zero_logico' * zero_logico

%%

uno_logico = zeros(dim, 1);

uno_logico(1) = 1;
uno_logico(3) = sqrt( (factorial(2*S)/(factorial(2) * factorial(2*S - 2))));

uno_logico = 1/sqrt(2^(2*S - 1)) * uno_logico;

% uno_logico' * uno_logico

%%
psi_init = [beta alpha 0 0 0 0 0 0].';

err_init = 1 - psi_init'*rho(:,:,1).'*psi_init

%%
psi = alpha * zero_logico + beta * uno_logico;

% psi'*psi

% Errore := 1 - F^2 con F^2 := <psi|rho|psi>
err_fin = 1 - psi'*rho(:,:,end).'*psi