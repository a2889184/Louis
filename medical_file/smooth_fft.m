function [f5, f8] = smooth_fft(Data, samplingRate, freq_min, freq_max, data_type, save_folder)
    N = length(Data);
    T = 1/samplingRate;
    k = 0:N-1;
    f = k*(1/(N*T));
    
    Y = abs(fft(Data)) / N * 2;
    
    integer_f = 0:int32(samplingRate);
    iter = 1;
    ecumulate = 0.00001;
    unit_length = 0;

    smooth_y = zeros(int32(samplingRate) + 1, 1);

    integer_f = double(integer_f);

    for i = 1:length(f)
        if(abs(f(i) - integer_f(iter)) <= 0.5)
            ecumulate = ecumulate + Y(i);
            unit_length = unit_length + 1;
        elseif(abs(f(i) - integer_f(iter)) > 0.5)
            smooth_y(iter) = ecumulate / unit_length;
            ecumulate = Y(i);
            unit_length = 1;
            iter = iter + 1;
        elseif(i == length(f))
            smooth_y(iter - 1) = ecumulate / unit_length;
        end
    end
    
    start = 1;
    ending = 1;

    error1 = 100000;
    error2 = 100000;
  
    for i = 1:length(integer_f)
        if(abs(integer_f(i) - freq_min)/double(freq_min) < error1)
            start = i;
            error1 = abs(integer_f(i) - freq_min)/freq_min;
        end

        if(abs(integer_f(i) - freq_max)/double(freq_max) < error2)
            ending = i;
            error2 = abs(integer_f(i) - freq_max)/freq_max;
        end
    end
    %f(start)
   % f(ending)

    All_area = trapz(integer_f(start:ending), smooth_y(start:ending));
    All_area_5 = All_area / 2;
    All_area_8 = All_area * 8 / 10;
    find5 = 10000000;
    find8 = 10000000;
    %smooth_y
    place5 = 1;
    place8 = 2;

    for i = start + 1 : ending
        temp_area = trapz(integer_f(start:i), smooth_y(start:i));
        if abs((temp_area - All_area_5) / All_area_5) < find5
            place5 = i;
            find5 = abs((temp_area - All_area_5) / All_area_5);
        end
        if abs((temp_area - All_area_8) / All_area_8) < find8
            place8 = i;
            find8 = abs((temp_area - All_area_8) / All_area_8);
        end
    end
    
    f5 = integer_f(place5);
    f8 = integer_f(place8);
    fig = figure;
    plot(integer_f(start:ending), smooth_y(start:ending));
    title_name = strcat(data_type, '(smooth) in freqency domain( ', int2str(freq_min), ' Hz to ', int2str(freq_max), ' Hz)');
    title(title_name);
    ylabel(strcat(data_type, '(smooth)'));
    xlabel('freqency');
    axis([integer_f(start), integer_f(ending), 0, prctile(smooth_y(start:ending),99.8) + 0.1]);
    plot_name = strcat(save_folder, data_type, '(smooth)_fft');
    print(fig, plot_name, '-dpng');
    close
    
end