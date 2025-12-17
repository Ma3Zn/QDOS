function [Sy] = SY(spin)
    Sy = -1i/2 * (S_plus(spin) - S_minus(spin));
end

