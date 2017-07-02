%This function will return a cell which contain value round extreme point(+- 250ms)
%Input:  *MaxOrMin_pos: input the position of Max or Min points
%        *Data: contain the data which you want to collect  
%        *samplingRate: how many time will sample a point, this parameter
%                       will decide range. 
%Output:  *cell_data: a size(3, 2*range + 1) cell, 3 is the number of extreme points. 
function cell_data = CollectDataAroundExtremeValue(MaxOrMin_pos, Data, samplingRate) 
    range = ceil(0.250 / (1 / samplingRate));
    cell_data = cell(1, 3);
    threshold_min = 1;
    threshold_max = length(Data);

    for i = 1:size(MaxOrMin_pos)
        if(MaxOrMin_pos(i) - range < 1)
            if(MaxOrMin_pos(i) + range > threshold_max)
                cell_data{i} = Data(threshold_min: threshold_max);
            else
                cell_data{i} = Data(threshold_min: MaxOrMin_pos(i) + range);
            end
        else
            if(MaxOrMin_pos(i) + range > threshold_max)
                cell_data{i} = Data(MaxOrMin_pos(i) - range: threshold_max);
            else
                cell_data{i} = Data(MaxOrMin_pos(i) - range: MaxOrMin_pos(i) + range);
            end
        end       
    end
end