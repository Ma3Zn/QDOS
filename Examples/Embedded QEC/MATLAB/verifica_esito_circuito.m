clear A C_ D_ C D rho valore_ancilla
% Recuperiamo il valore dell'ancilla osservato
valore_ancilla = load('../measurement_outcome.txt');
valore_ancilla = valore_ancilla + 1;

if (flag_valore_misurato(valore_ancilla) == 0)
%   Aggiorniamo il valore del flag
    flag_valore_misurato(valore_ancilla) = 1;

%   Recuperiamo le rho prodotte dalla simulazione
    A = load('../output/rho_finale.out');

    [N,M] = size(A);

    dim = M / 2;

    C_ = A(:,1 : dim);
    D_ = A(:,dim + 1 : end);

    % vanno trasposte nei primi due indici per avere quelle corrette!!
    C = reshape(C_', dim, dim, 2);
    D = reshape(D_', dim, dim, 2);

    rho = C + 1i*D;
    
%   Calcoliamo la probabilità dell'esito della misura
    if (valore_ancilla == 1)
        p_osservazione(valore_ancilla) = real(rho(1,1,1) + rho(3,3,1));
    else
        p_osservazione(valore_ancilla) = real(rho(6,6,1) + rho(8,8,1));
    end
    
%   Calcoliamo l'errore := 1 - F^2 con F^2 := <psi|rho|psi>
    errore(valore_ancilla) = 1 - psi'*rho(:,:,end).'*psi;
end