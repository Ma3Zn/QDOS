clear

Sz = SZ(1/2);

Sy = SY(1/2);

% Tensore di connessione
J = [ 0     4*pi    4*2*pi;
      0      0      4*2*pi;
      0      0        0     ];

J = J * 10;
  
% Hamiltoniane
mu_b = 14 * 2 * pi;
g_1  = [2.2, 2.2, 2.2];
g_2  = [2, 2, 2];
g_3  = [1.8, 1.8, 1.8];
D    = [7.2 5 9] * 2 * pi;
B = 4;

B1 = 0.01;

H1 = D(1) * Sz * Sz + g_1(3) * mu_b * B * Sz;
H2 = D(2) * Sz * Sz + g_2(3) * mu_b * B * Sz;
H3 = D(3) * Sz * Sz + g_3(3) * mu_b * B * Sz;

H =   kron(kron(H1,eye(2)),eye(2)) ...
    + kron(kron(eye(2),H2),eye(2)) ...
    + kron(eye(2),kron(eye(2),H3));

% H = kron(H1, eye(4)) + kron(eye(4), H1);

[aut, w] = eig(H);
% aut = [ aut(:,1),aut(:,3),aut(:,6),...
%         aut(:,10),aut(:,2),aut(:,4),...
%         aut(:,8),aut(:,12),aut(:,5),...
%         aut(:,7),aut(:,11),aut(:,15),...
%         aut(:,9),aut(:,13),aut(:,14),aut(:,16)]
% 0 1 4 8 2 3 6 10 5 7 11 9 13 12 14 15
% asp = [diag(aut'*kron(Sz,eye(4))*aut), diag(aut'*kron(eye(4),Sz)*aut)]

w = diag(w);

for i = 1:size(H)
    for j = 1:size(H)
        W(i,j) = w(i)-w(j);
    end
end
W;

% Matrice di connessione
B1 = B1;
Hy1 = B1 * mu_b * g_1(2) * Sy;
Hy2 = B1 * mu_b * g_2(2) * Sy;
Hy3 = B1 * mu_b * g_3(2) * Sy;

% Parametri (avviamento)
params = 0;

% aggiungiamo ora una matrice di densita "stato iniziale" del sistema
% alpha = 3.8753271;
% beta  = 0.8643292;
% gamma = 2.974528;
% delta = 1.973529;
% 
% alpha = 3/4 * exp(1i*alpha);
% beta  = 1/2 * exp(1i*beta);
% gamma = sqrt(1/8) * exp(1i*gamma);
% delta = 1/4 * exp(1i*delta);

% v = [0 0 0 0 alpha beta gamma delta].';
% v = random_superposition(8);
v = [1 0 0 0 0 0 0 0].';
% v = 1/sqrt(2) * [1 0 1 0 0 0 0 0].';
% v = 1/sqrt(8) * [1 1 1 1 1 1 1 1].';
% v = [1 0 0 0 0 0 0 0].';
% v = [0 0 0 0 0 0 0 1].';
% v = 1/sqrt(4) * [1 0 1 0 1 0 1 0].';
% v = [1 0 0 0].';
% v = 1/sqrt(4) * [1 1 1 1].';
% v = [1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0].';
% v = 1/sqrt(16) * [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1].';

rho_in = v*v';

%%

% riempiamo file parametri
fileID = fopen('../input/in.P.bin','w');
fwrite(fileID, params, 'uint64');

Y(1,:,:) = real(rho_in).'; 
Y(2,:,:) = imag(rho_in).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%

% riempiamo file hamiltoniane
fileID = fopen('../input/in.H.bin','w');

y(1,:,:) = real(H1).'; 
y(2,:,:) = imag(H1).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H2).'; 
y(2,:,:) = imag(H2).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H3).'; 
y(2,:,:) = imag(H3).';

fwrite(fileID, y, 'double');
fclose(fileID);


%%

% riempiamo file tensore di connessione
fileID = fopen('../input/in.J.bin','w');

yy(1,:,:) = real(J).'; 
yy(2,:,:) = imag(J).';

fwrite(fileID, yy, 'double');
fclose(fileID);

%%

% riempiamo file matrice di connessione
fileID = fopen('../input/in.M.bin','w');

clear Y

Y(1,:,:) = real(Hy1).'; 
Y(2,:,:) = imag(Hy1).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy2).'; 
Y(2,:,:) = imag(Hy2).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy3).'; 
Y(2,:,:) = imag(Hy3).';

fwrite(fileID, Y, 'double');
fclose(fileID);
