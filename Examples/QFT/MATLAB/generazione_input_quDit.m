clear

spin = 15/2;

Sz = SZ(spin);
Sy = SY(spin);

% Tensore di connessione
% J = [ 0     4*2*pi  4*2*pi  4*2*pi  4*2*pi  4*2*pi;
%       0      0      4*2*pi  4*2*pi  4*2*pi  4*2*pi;
%       0      0        0     4*2*pi  4*2*pi  4*2*pi;
%       0      0        0       0     4*2*pi  4*2*pi;
%       0      0        0       0       0     4*2*pi;
%       0      0        0       0       0       0   ;
%       0      0        0       0       0       0   ];

% Tensore di connessione
J = [ 0     4*2*pi ;
      0      0    ];

J = J*10;

% Hamiltoniane
mu_b = 14 * 2 * pi;
g_1  = [2.6, 2.6, 2.6];
g_2  = [2.4, 2.4, 2.4];
g_3  = [2.2, 2.2, 2.2];
g_4  = [2, 2, 2];
g_5  = [1.8, 1.8, 1.8];
g_6  = [1.6, 1.6, 1.6];
D    = [9 8.3 7.5 7 6.2 5.8 5] * 2 * pi;
B = 8;

B1 = 0.001;

H1 = D(1) * Sz * Sz + g_1(3) * mu_b * B * Sz;
H2 = D(2) * Sz * Sz + g_2(3) * mu_b * B * Sz;
H3 = D(3) * Sz * Sz + g_3(3) * mu_b * B * Sz;
H4 = D(4) * Sz * Sz + g_4(3) * mu_b * B * Sz;
H5 = D(5) * Sz * Sz + g_5(3) * mu_b * B * Sz;
H6 = D(6) * Sz * Sz + g_6(3) * mu_b * B * Sz;

% H = kron(H4,eye(4)) + kron(eye(4),H5);
% H = H + J(1,2) * kron(Sz, Sz);

% H = kron(H1, eye(4)) + kron(eye(4), H1);

% [aut, w] = eig(H);
% aut = [ aut(:,1),aut(:,3),aut(:,6),...
%         aut(:,10),aut(:,2),aut(:,4),...
%         aut(:,8),aut(:,12),aut(:,5),...
%         aut(:,7),aut(:,11),aut(:,15),...
%         aut(:,9),aut(:,13),aut(:,14),aut(:,16)]
% 0 1 4 8 2 3 6 10 5 7 11 9 13 12 14 15
% asp = [diag(aut'*kron(Sz,eye(4))*aut), diag(aut'*kron(eye(4),Sz)*aut)]

% w = diag(w);
% 
% for i = 1:size(H)
%     for j = 1:size(H)
%         W(i,j) = w(i)-w(j);
%     end
% end
% W;

% Matrice di connessione
Hy1 = B1 * mu_b * g_1(2) * Sy;
Hy2 = B1 * mu_b * g_2(2) * Sy;
Hy3 = B1 * mu_b * g_3(2) * Sy;
Hy4 = B1 * mu_b * g_4(2) * Sy;
Hy5 = B1 * mu_b * g_5(2) * Sy;
Hy6 = B1 * mu_b * g_6(2) * Sy;

% Parametri (avviamento)
avviamento = 0;

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
% v = random_superposition(16);
% v = [1 0 0 0 0 0 0 0].';
% v = 1/sqrt(2) * [1 0 1 0 0 0 0 0].';
% v = 1/sqrt(8) * [1 1 1 1 1 1 1 1].';
% v = [1 0 0 0 0 0 0 0].';
% v = [0 0 0 0 0 0 0 1].';
% v = 1/sqrt(4) * [1 0 1 0 1 0 1 0].';
% v = [1 0 0 0 0 0].';
% v = 1/sqrt(4) * [1 1 1 1].';
% v = [1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0].';
% v = 1/sqrt(16) * [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1].';

v = zeros(16,1);
v(1) = 1;

rho_in = v*v';

%%

% riempiamo file parametri
fileID = fopen('../input/in.P.bin','w');
fwrite(fileID, avviamento, 'double');

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

y(1,:,:) = real(H6).'; 
y(2,:,:) = imag(H6).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H2).'; 
y(2,:,:) = imag(H2).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H3).'; 
y(2,:,:) = imag(H3).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H4).'; 
y(2,:,:) = imag(H4).';

fwrite(fileID, y, 'double');

y(1,:,:) = real(H5).'; 
y(2,:,:) = imag(H5).';

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

Y(1,:,:) = real(Hy6).'; 
Y(2,:,:) = imag(Hy6).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy2).'; 
Y(2,:,:) = imag(Hy2).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy3).'; 
Y(2,:,:) = imag(Hy3).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy4).'; 
Y(2,:,:) = imag(Hy4).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(Hy5).'; 
Y(2,:,:) = imag(Hy5).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%
SWAP = [
    1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0;
    0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0;
    0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0;
    0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1];

d = det(SWAP);
X = exp(-1i*angle(d)/16) * SWAP;


% riempiamo file matrice di connessione
fileID = fopen('../custom_gate/SWAP.bin','w');

id = 0;

fwrite(fileID, id, 'uint64');

clear y
y(1,:,:) = real(X).';
y(2,:,:) = imag(X).';

fwrite(fileID, y, 'double');

fclose(fileID);

%%
E1 = Sz;
E2 = Sz;

% riempiamo file matrice di connessione
fileID = fopen('../input/in.E.bin','w');

clear Y

Y(1,:,:) = real(E1).'; 
Y(2,:,:) = imag(E1).';

fwrite(fileID, Y, 'double');

Y(1,:,:) = real(E2).'; 
Y(2,:,:) = imag(E2).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%
r = ones(1,2)/(100000);
% riempiamo file matrice di connessione
fileID = fopen('../input/in.R.bin','w');

fwrite(fileID, r, 'double');

fclose(fileID);