close all
clear
clc

% Generiamo l'opportuno l'input per QDOS
generazione_input;

T2_S = T2(2);
N = 500;

flag_valore_misurato = uint8([0,0]);

iterazione_max = 19;
indice_iterazione = 1;

ff = zeros(1,iterazione_max);

bck_p_oss = zeros(2,iterazione_max);
bck_err = zeros(2,iterazione_max);

t_T2 = zeros(1,iterazione_max);
errore_totale = zeros(1,iterazione_max);

p_osservazione = zeros(1,2);
errore = zeros(1,2);

% tval = [2500, 3000, 3500];
tval = [500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, ...
        5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, ...
        50000];
% tval = linspace(500,50000,iterazione_max)

% Cicliamo sui vari tempi di IDLE
for t = tval
%   Inizializzazione variabili    
    p_osservazione = zeros(1,2);
    errore = zeros(1,2);

%   Scriviamo il circuito QDOS con l'appropriato tempo di evoluzione libera
%   tra la fase di encoding e quella di detection
    scrivi_circuito_QDOS;
    
%   Eseguiamo il circuito al più N volte fino a che non otteniamo una
%   misurazione dell'ancilla sia top che down
    for n = 1:N
        clc
        somma = 0;
        
%       Teniamo traccia dell'avanzamento della simulazione
        t        
        n
        
%       Eseguiamo il circuito QDOS
        !cd .. && qdos --simulation_progress=1 --random_seed=true --errors_enabled=true EQEC_grafico.q
        
%       Leggiamo gli esiti del circuito
        verifica_esito_circuito;

%       Verifichiamo se dobbiamo continuare il ciclo oppure se abbiamo
%       ottenuto già entrambi i risultati.
        somma = flag_valore_misurato(1) + flag_valore_misurato(2);
        
        if (somma == 2)
            ff(indice_iterazione) = 1;
            break;
        end
    end
%   Ripristiniamo il flag
    flag_valore_misurato = uint8([0,0]);
    
%   Salviamo statistiche per un possibile debug
    bck_p_oss(:,indice_iterazione) = p_osservazione.';
    bck_err(:,indice_iterazione) = errore.';
    
%   Calcoliamo l'errore per il rapporto t/T2_S attuale
	errore_totale(indice_iterazione) = p_osservazione(1) * errore(1) ...
                                     + p_osservazione(2) * errore(2);
    t_T2(indice_iterazione) = t/T2_S;
    
    indice_iterazione = indice_iterazione + 1;
end

%%

%   Stampiamo il grafico dell'errore
figure(1)
loglog(t_T2, errore_totale);

figure(2)
plot(t_T2, errore_totale);