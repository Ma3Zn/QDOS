% Nucleo - Elettrone
J  = [ 0      -(0.033 * 30) * 2 * pi;
       0                 0          ];

% Elettrone - Elettrone
mu_b = 14 * 2 * pi;
g_1  = [1.98,1.98,1.98];
g_2  = [2.9,2.9,4.2];
D    = - (0.24 * 30) * 2 * pi;
B    = 1;

% 1 G = 1e-4 T
B1   = 0.0100;
B1_A = 0.0045;

% Operatori di Spin
spin1 = 3/2;
spin2 = 1/2;

Sz1 = SZ(spin1);
Sz2 = SZ(spin2);

Sy1 = SY(spin1);
Sy2 = SY(spin2);

% Hamiltoniane
H1 = g_1(3) * mu_b * B * Sz1 + D * Sz1 * Sz1;
H2 = g_2(3) * mu_b * B * Sz2;

% Matrici di connessione
Hy1 = B1  *g_1(2) * mu_b * Sy1;
Hy2 = B1_A*g_2(2) * mu_b * Sy2;

% Ratei Dephasing
T2 = [3000, 50000];
ratei = 1./T2;

% Operatori Errore
E1 = Sz1;
E2 = Sz2;

% Condizione iniziale
alpha = exp( 1i * 1.93741501847)/2;
beta  = exp(-1i * 2.08461593485)*sqrt(3)/2;

v = zeros(8,1);
v(1) = beta;
v(2) = alpha;

rho_in = v*v';

%%

avviamento = 0;

% riempiamo file parametri
fileID = fopen('../input/in.P.bin','w');
fwrite(fileID, avviamento, 'double');

clear Y

Y(1,:,:) = real(rho_in).'; 
Y(2,:,:) = imag(rho_in).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%

% riempiamo file hamiltoniane
fileID = fopen('../input/in.H.bin','w');

clear y

y(1,:,:) = real(H2).'; 
y(2,:,:) = imag(H2).';

fwrite(fileID, y, 'double');

clear y

y(1,:,:) = real(H1).'; 
y(2,:,:) = imag(H1).';

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

Y(1,:,:) = real(Hy2).'; 
Y(2,:,:) = imag(Hy2).';

fwrite(fileID, Y, 'double');

clear Y

Y(1,:,:) = real(Hy1).'; 
Y(2,:,:) = imag(Hy1).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%

% Riempiamo file operatori errore
fileID = fopen('../input/in.E.bin','w');

clear Y

Y(1,:,:) = real(E2).'; 
Y(2,:,:) = imag(E2).';

fwrite(fileID, Y, 'double');

clear Y

Y(1,:,:) = real(E1).'; 
Y(2,:,:) = imag(E1).';

fwrite(fileID, Y, 'double');
fclose(fileID);

%%


% Riempiamo file parametri errore
fileID = fopen('../input/in.R.bin','w');

fwrite(fileID, ratei, 'double');
fclose(fileID);

%%

% Definiamo e scriiamo su File il custom gate $ACE
fileID = fopen('../custom_gate/ACE.bin','w');

% Sequenza di rotazioni
ID = 1;
fwrite(fileID, ID, 'uint64');

% Numero di rotazioni
N = 2;
fwrite(fileID, N, 'uint64');

% Prima Rotazione
% Rotazione Piana
IDR = 0;
fwrite(fileID, IDR, 'uint64');

% Livelli
lvs = [1, 5];
fwrite(fileID, lvs, 'uint64');

% Parametri rotazione
param = [pi, 0];
fwrite(fileID, param, 'double');

% Seconda Rotazione
% Rotazione Piana
IDR = 0;
fwrite(fileID, IDR, 'uint64');

% Livelli
lvs = [3, 7];
fwrite(fileID, lvs, 'uint64');

% Parametri rotazione
param = [pi, 0];
fwrite(fileID, param, 'double'); 

%%

% Definiamo e scriiamo su File il custom gate $ACE
fileID = fopen('../custom_gate/ADE.bin','w');

% Sequenza di rotazioni
ID = 1;
fwrite(fileID, ID, 'uint64');

% Numero di rotazioni
N = 2;
fwrite(fileID, N, 'uint64');

% Prima Rotazione
% Rotazione Piana
IDR = 0;
fwrite(fileID, IDR, 'uint64');

% Livelli
lvs = [1, 5];
fwrite(fileID, lvs, 'uint64');

% Parametri rotazione
param = [3*pi, 0];
fwrite(fileID, param, 'double');

% Seconda Rotazione
% Rotazione Piana
IDR = 0;
fwrite(fileID, IDR, 'uint64');

% Livelli
lvs = [3, 7];
fwrite(fileID, lvs, 'uint64');

% Parametri rotazione
param = [3*pi, 0];
fwrite(fileID, param, 'double'); 

%%

dim = 8;

% Generazione stato finale di output
S = spin1;

zero_logico = zeros(dim,1);

zero_logico(2) = sqrt((factorial(2*S)/(factorial(1) * factorial(2*S - 1))));
zero_logico(4) = 1;

zero_logico = 1/sqrt(2^(2*S - 1)) * zero_logico;

% zero_logico' * zero_logico

uno_logico = zeros(dim, 1);

uno_logico(1) = 1;
uno_logico(3) = sqrt( (factorial(2*S)/(factorial(2) * factorial(2*S - 2))));

uno_logico = 1/sqrt(2^(2*S - 1)) * uno_logico;

% uno_logico' * uno_logico

% psi_init = [beta alpha 0 0 0 0 0 0].';
% 
% err_init = 1 - psi_init'*rho(:,:,1).'*psi_init

psi = alpha * zero_logico + beta * uno_logico;