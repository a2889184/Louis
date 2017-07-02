function createfolder(data_root_directory, source_directory)
    % data_root_directory: where to put the folder
    % source_directory: the process target directory file
    % ex: createfolder('../data/Control', './data_directory/Control')
    load (source_directory, 'data_directory')
    folder_name = strcat(data_root_directory, '/output');
    mkdir (folder_name);
    for i = 1: size(data_directory, 1)
       test_folder_name = strcat(folder_name, '/', data_directory{i, 1});
       mkdir(test_folder_name);
       for j = 1:length(data_directory{i, 2})
           s = sprintf('%s',data_directory{i, 2}{j});

           mkdir(strcat(test_folder_name, '/', s));
       end
           
    end
end