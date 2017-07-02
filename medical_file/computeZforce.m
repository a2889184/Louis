function Z_force = computeZforce(Fz3,Fz4)
    % Fz1~4 are force from the force plate.
    
    % compute the Z_force, * 1000 means switch V to mV, / 3.XXX is correct the value
    %Z_force = (Fz1 / 3.826 + Fz2 / 3.819 + Fz3 / 3.817 + Fz4 / 3.810) * 1000; 
    Z_force = (Fz3 / 3.817 + Fz4 / 3.810) * 1000; 
end