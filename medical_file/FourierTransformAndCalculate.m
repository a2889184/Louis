function FourierTransformAndCalculate(Data)
    % freqency
    f = 0:1/length_sec:samplingRate - 1/length_sec;
    Y = abs(fft(Data))/(length_sec * samplingRate) * 2;
    
    plot(f, Y);
end