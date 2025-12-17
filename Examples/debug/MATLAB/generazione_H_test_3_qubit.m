spin = 1/2;
dim  = 2 * spin + 1;

X = Hadamard(spin);
    
d = det(X)

if (abs(d - 1) > 1e-12)
    U = exp(-1i*angle(d)/dim) * X;
    d = det(U)
else
    U = X;
    d = det(U)
end

U

% riempiamo file hdamard
fileID = fopen('../input/H.gate','w');

fwrite(fileID, 0, 'uint64');

XX(1,:,:) = real(U).'; 
XX(2,:,:) = imag(U).';

fwrite(fileID, XX, 'double');
fclose(fileID);