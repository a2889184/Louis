function [f5, f8] = GetFourier_50_80(Data, samplingRate, freq_min, freq_max, data_type, save_folder)
    N = length(Data);
    T = 1/samplingRate;
    k = 0:N-1;

    f = k*(1/(N*T));
    Y = abs(fft(Data)) / N * 2;

    start = 1;
    ending = 2;

    error1 = 100000;
    error2 = 100000;
    for i = 1:length(f)
        if(abs(f(i) - freq_min)/freq_min < error1)
            start = i;
            error1 = abs(f(i) - freq_min)/freq_min;
        end

        if(abs(f(i) - freq_max)/freq_max < error2)
            ending = i;
            error2 = abs(f(i) - freq_max)/freq_max;
        end
    end
    %f(start)
   % f(ending)
    
    place5 = 1;
    place8 = 2;
    
    All_area = trapz(f(start:ending), Y(start:ending));
    All_area_5 = All_area / 2;
    All_area_8 = All_area * 8 / 10;
    find5 = 10000000;
    find8 = 10000000;
    for i = start + 1 : ending
        temp_area = trapz(f(start:i), Y(start:i));
        if abs((temp_area - All_area_5) / All_area_5) < find5
            place5 = i;
            find5 = abs((temp_area - All_area_5) / All_area_5);
        end
        if abs((temp_area - All_area_8) / All_area_8) < find8
            place8 = i;
            find8 = abs((temp_area - All_area_8) / All_area_8);
        end
    end
    %All_area
   % temp_area = trapz(f(start:place5), Y(start:place5))
    f5 = f(place5);
    f8 = f(place8);
    fig = figure;
    plot(f(start:ending), Y(start:ending));
    title_name = strcat(data_type, ' in freqency domain( ', int2str(freq_min), ' Hz to ', int2str(freq_max), ' Hz)');
    title(title_name);
    ylabel(data_type);
    xlabel('freqency');

    axis([f(start), f(ending), 0, prctile(Y(start:ending), 99.8) + 0.1]);
    plot_name = strcat(save_folder, data_type, '_fft');
    print(fig, plot_name, '-dpng');
    close
end