function [UP, DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data, time)
    UP = cell(2, 2);
    DOWN = cell(2, 2);
    %row is different test, column(1) is data, column(2) is time
    
    for i = 1:2
       DOWN{i, 1} = Data(Max_pos(i): Min_pos(i)); 
       DOWN{i, 2} = time(Max_pos(i): Min_pos(i)); 
       UP{i, 1} = Data(Min_pos(i): Max_pos(i + 1));
       UP{i, 2} = time(Min_pos(i): Max_pos(i + 1));
    end
end