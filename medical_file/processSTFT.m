function processSTFT(Data, time, f)
    B=1;
    y1=recSTFT(Data,time,f,B); 
    %y2 = wdf(x,t,f);

    figure;
    imagesc(transpose(abs(y1)),'Xdata',time,'Ydata',f)
    colormap(gray);

%figure;
%imagesc(transpose(abs(y2)),'Xdata',t,'Ydata',f)
%colormap(gray);