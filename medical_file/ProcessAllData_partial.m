clc;close all; clear;

load ('./data_directory/Reliability', 'data_directory')

%for i = 1: size(data_directory, 1) - 1
    
%end
root_name = '../data/Reliability/';
% iterate all folder and file
% size(data_directory, 1)
for p = 1:size(data_directory, 1)
    folder_name = strcat(data_directory{p, 1}, '/');

    %s = sprintf('%s',data_directory{i, 2}{j});
    for o = 1: length(data_directory{p, 2})
        test_name = char(data_directory{p, 2}(o));
        
        save_folder = strcat(root_name, 'ankle_output/', folder_name, test_name, '/');
        ab_folder_name = strcat(root_name, folder_name);
        disp (strcat(ab_folder_name, test_name))
        load (strcat(ab_folder_name, test_name, '.mat')) %load data
        % start process the data
        time = 0: 1 / samplingRate: length_sec;
        time = time';
        time = time(1:end - 1, 1);

        f = 0:1/length_sec:samplingRate - 1/length_sec;
        f = f';

        %plot graph of relationship between time and ankle's degree
        % find ankle column

        ankle_column = -1;
        RMG_A_column = zeros(5, 1);
        target = 'ankle';
        z_force_find = -1;
        z_force_column = zeros(4, 1);
        for i = 1: length(channelNames)
            name = lower(channelNames(i));
            %name = lname;
            while ~strcmp(name, '')
                [token, remain] = strtok(name, ' ,.-?!\/');
                if(strcmp(token, target))
                    ankle_column = i;
                end
                if(strcmp(token, 'tib'))
                    RMG_A_column(1) = i;
                end
                if(strcmp(token, 'soleus'))
                    RMG_A_column(2) = i;
                end
                if(strcmp(token, 'med'))
                    RMG_A_column(3) = i;
                end
                if(strcmp(token, 'lat'))
                    RMG_A_column(4) = i;
                end
                if(strcmp(token, 'y'))
                    RMG_A_column(5) = i;
                end

                if(strcmp(token, '5'))
                    z_force_column(1) = i;
                end
                if(strcmp(token, '6'))
                    z_force_column(2) = i;
                end
                if(strcmp(token, '7'))
                    z_force_column(3) = i;
                end
                if(strcmp(token, '8'))
                    z_force_column(4) = i;
                    z_force_find = 100;
                end
                name = remain;
            end
        end
        
        for i = 1:4
            RMG_A_column(i) = i;
        end

        if(ankle_column == -1)
            fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
            fprintf(fileID,'no ankle data\n');
            fclose(fileID);
            continue
        else
            max_min_ankle = cell(2, 2);
            max_min_ankle{1, 1} = 'max_ankle';
            max_min_ankle{2, 1} = 'min_ankle';
            Max_pos = findmax(Data{ankle_column}, time, 0, time(end)); %compute the max degree of ankle
            Min_pos = findmin(Data{ankle_column}, time, time(Max_pos(1)), time(Max_pos(end))); %compute the min degree of ankle
            
            max_min_ankle{1, 2} = Data{ankle_column}(Max_pos);
            max_min_ankle{2, 2} = Data{ankle_column}(Min_pos);
            save (strcat(save_folder, 'max_min_ankle'), 'max_min_ankle')
        end
    end

end

