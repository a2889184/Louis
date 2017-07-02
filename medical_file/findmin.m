function pos = findmin(Data, time, threshold1, threshold2)
    pos = zeros(3, 1);
    min1 = 0;
    min2 = 0;
    time1 = 0;
    time2 = 0;
    diff_time = 2.5;
    for j = 1:3
        min = 100000;
        for i = 1:size(time)
            if(time(i) < threshold2 + 3.2 && time(i) > threshold1 && Data(i) < min && Data(i) ~= min1 && Data(i) ~= min2 && abs(time(i) - time1) > diff_time && abs(time(i) - time2) > diff_time), 
                min = Data(i);
                pos(j) = i;
            end
        end
        if j == 1, 
            min1 = min;
            time1 = time(pos(j));
        elseif j == 2, 
            min2 = min;
            time2 = time(pos(j));
        end
    end
    for i = 1: length(pos)
        for j = i:-1:length(pos) - 1
            if pos(j) < pos(j - 1),
                temp = pos(j);
                pos(j) = pos(j - 1);
                pos(j - 1) = temp;
            end
        end
    end
end