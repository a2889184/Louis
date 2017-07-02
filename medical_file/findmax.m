function pos = findmax(Data, time, threshold1, threshold2)
    pos = zeros(3, 1);
    max1 = 0;
    max2 = 0;
    time1 = 0;
    time2 = 0;
    diff_time = 2.5;
    for j = 1:3
        max = 0;
        for i = 1:size(time)
            if(time(i) < threshold2 - 3 && time(i) > threshold1 && Data(i) > max && Data(i) ~= max1 && Data(i) ~= max2 && abs(time(i) - time1) > diff_time && abs(time(i) - time2) > diff_time), 
                max = Data(i);
                pos(j) = i;
            end
        end
        if j == 1, 
            max1 = max;
            time1 = time(pos(j));
        elseif j == 2, 
            max2 = max;
            time2 = time(pos(j));
        end
    end
    
    for i = 1: length(pos)
        for j = i:-1:length(pos)-1
            if pos(j) < pos(j - 1),
                temp = pos(j);
                pos(j) = pos(j - 1);
                pos(j - 1) = temp;
            end
        end
    end
end