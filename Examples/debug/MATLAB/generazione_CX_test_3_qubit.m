X = [1 0 0 0;
      0 1 0 0;
      0 0 0 1;
      0 0 1 0];
dim = 4;
    
d = det(X)

if (abs(d - 1) > 1e-12)
    U = exp(-1i*angle(d)/dim) * X;
    d = det(U)
else
    U = X;
    d = det(U)
end

% riempiamo file CX
fileID = fopen('../input/CX.gate','w');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fwrite(fileID, 0, 'uint64');

XX(1,:,:) = real(U).'; 
XX(2,:,:) = imag(U).';

fwrite(fileID, XX, 'double');
fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% fwrite(fileID, 1, 'uint64');
% 
% % Scriviamo il numero di rotazioni da eseguire
% N_rot = 4;
% fwrite(fileID, N_rot, 'uint64');
% 
% % Rotazione piana
% id = 0;
% livelli = [2,3];
% theta = pi;
% beta1 = pi;
% 
% fwrite(fileID, id, 'uint64');
% fwrite(fileID, livelli, 'uint64');
% fwrite(fileID, theta, 'double');
% fwrite(fileID, beta1, 'double');
% 
% % Rotazione Z
% id = 1
% livelli = [0,1];
% phi = pi/4;
% 
% fwrite(fileID, id, 'uint64');
% fwrite(fileID, livelli, 'uint64');
% fwrite(fileID, phi, 'double');
% 
% % Rotazione Z
% id = 1
% livelli = [1,2];
% phi = pi/2;
% 
% fwrite(fileID, id, 'uint64');
% fwrite(fileID, livelli, 'uint64');
% fwrite(fileID, phi, 'double');
% 
% % Rotazione Z
% id = 1
% livelli = [2,3];
% phi = (pi/2 + pi/4);
% 
% fwrite(fileID, id, 'uint64');
% fwrite(fileID, livelli, 'uint64');
% fwrite(fileID, phi, 'double');
% 
% fclose(fileID);