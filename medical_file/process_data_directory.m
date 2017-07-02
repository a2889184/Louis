function process_data_directory(data_root_directory)
    % ex:  process_data_directory('../data/Control') 
    mkdir('./data_directory')
    foldername1 = dir(data_root_directory);
    data_directory = cell(length(foldername1) - 2, 3);
    foldername1 = foldername1(3:end);
    targetnames = ['/R_S*.mat'; '/R_U*.mat'; '/L_S*.mat'; '/L_U*.mat'];
    
    for i = 1:length(foldername1)
        data_name = strcat(data_root_directory, '/', foldername1(i).name);
        data_directory{i, 1} = foldername1(i).name;
        for k = 1: size(targetnames, 1)
            temp_name = strcat(data_name, targetnames(k, :));
            temp = dir(temp_name);
            for j = 1:length(temp)
                token = strtok(temp(j).name, '.');
                
                data_directory{i, 2} = [data_directory{i, 2}; cellstr(token)];
                data_directory{i, 3} = [data_directory{i, 3}; cellstr(strcat(data_name, '/', token))];
            end
        end
        
        %for j = 1:length(foldername2{i})
            %s = sprintf('%s',foldername2{i}{j});
            %temp = dir(strcat(name3, '/', s, '/R_S*.mat'));
            %temp.name
        %end
     
    end
    
    folder = '';
    num = 0;
    for i = 1:length(data_root_directory)
        if (data_root_directory(i) == '/')
            num = num + 1;
            if(num == 2)
                folder = data_root_directory(i + 1:end);
                break
            end
        end
    end
    targetfolder = strcat('./data_directory/', folder);
    save (targetfolder, 'data_directory');
   % for i = 1:length(foldername1)
        %temp = dir(strcat(data_root_directory, '/', foldername1(i).name));
        
        %for j = 3:length(temp)
            %foldername2{i, 1} = [foldername2{i, 1};temp(j).name];
        %end
        %foldername2{i, 1} = cellstr(foldername2{i, 1});
    %end
    
    %foldername3 = cell(length(foldername2), 1);
    %targetnames = ['/R_S*.mat', '/R_U*.mat', '/L_S*.mat', '/L_U*.mat'];
    %for i = 3:length(foldername2)
        %name3 = strcat(data_root_directory, '/', foldername1(i).name);
        %for j = 1:length(foldername2{i})
           % s = sprintf('%s',foldername2{i}{j});
            %temp = dir(strcat(name3, '/', s, '/R_S*.mat'));
           % temp.name
       % end
   % end
    
    %data_directory = foldername3;
end