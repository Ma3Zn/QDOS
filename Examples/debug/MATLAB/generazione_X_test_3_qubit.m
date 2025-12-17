X = [0 1; 1 0];
dim = 2;
    
d = det(X)

if (abs(d - 1) > 1e-12)
    U = exp(-1i*angle(d)/dim) * X;
    d = det(U)
else
    U = X;
    d = det(U)
end

% riempiamo file hdamard
fileID = fopen('../input/X.gate','w');

fwrite(fileID, 0, 'uint64');

XX(1,:,:) = real(U).'; 
XX(2,:,:) = imag(U).';

fwrite(fileID, XX, 'double');
fclose(fileID);