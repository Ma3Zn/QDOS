function [S_p] = S_plus(spin)
    dim = 2*spin + 1;
    S_p = zeros(dim, dim);
    
    for i = 1:(dim-1)
        S_p(i, i+1) = spin*(spin+1) - (spin - (i-1))*(spin - i);
        S_p(i, i+1) = sqrt(S_p(i,i+1));
    end
end