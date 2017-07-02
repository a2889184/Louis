function rms_array = compute_rms(Data)
    rms_array = [];
    for i = 1: length(Data)
       rms_array = [rms_array; rms(Data{i})]; 
    end
end