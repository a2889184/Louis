clc;close all; clear;

load ('./data_directory/Control', 'data_directory')

%for i = 1: size(data_directory, 1) - 1
    
%end
root_name = '../data/Control/';
% iterate all folder and file
% size(data_directory, 1)
for p = 1:size(data_directory, 1)
    folder_name = strcat(data_directory{p, 1}, '/');

    %s = sprintf('%s',data_directory{i, 2}{j});
    for o = 1: length(data_directory{p, 2})
        test_name = char(data_directory{p, 2}(o));
        
        save_folder = strcat(root_name, 'output/', folder_name, test_name, '/');
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
        
        lack_EMGorY = 0;
        for i = 1:5
            if(RMG_A_column(i) == 0)
                lack_EMGorY = 1;
            end
        end
        
        if(lack_EMGorY == 1)
            fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
            fprintf(fileID,'lack of some EMG/Y\n');
            fclose(fileID);
            continue
        end
        if(ankle_column == -1)
            fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
            fprintf(fileID,'no ankle data\n');
            fclose(fileID);
            continue
        end


        %for i = 1: size(time)
            %if Data{ankle_column}(i, 1) ~= Data{ankle_column}(i + 1, 1),
                %breakpoint = i;
                %break
            %end
        %end
        if(ankle_column ~= -1)
            Max_pos = findmax(Data{ankle_column}, time, 0, time(end)); %compute the max degree of ankle
            Min_pos = findmin(Data{ankle_column}, time, time(Max_pos(1)), time(Max_pos(end))); %compute the min degree of ankle


        % add the data between (max - 250ms) and (max + 250ms) to a cell (for TA, SU, MG, LG)

            MaxCell_TA = CollectDataAroundExtremeValue(Max_pos, Data{RMG_A_column(1)}, samplingRate);
            MinCell_TA = CollectDataAroundExtremeValue(Min_pos, Data{RMG_A_column(1)}, samplingRate);
            MaxCell_SU = CollectDataAroundExtremeValue(Max_pos, Data{RMG_A_column(2)}, samplingRate);
            MinCell_SU = CollectDataAroundExtremeValue(Min_pos, Data{RMG_A_column(2)}, samplingRate);
            MaxCell_MG = CollectDataAroundExtremeValue(Max_pos, Data{RMG_A_column(3)}, samplingRate);
            MinCell_MG = CollectDataAroundExtremeValue(Min_pos, Data{RMG_A_column(3)}, samplingRate);
            MaxCell_LG = CollectDataAroundExtremeValue(Max_pos, Data{RMG_A_column(4)}, samplingRate);
            MinCell_LG = CollectDataAroundExtremeValue(Min_pos, Data{RMG_A_column(4)}, samplingRate);
            

            rms_Max_TA = compute_rms(MaxCell_TA);
            rms_Min_TA = compute_rms(MinCell_TA);
            rms_Max_SU = compute_rms(MaxCell_SU);
            rms_Min_SU = compute_rms(MinCell_SU);
            rms_Max_MG = compute_rms(MaxCell_MG);
            rms_Min_MG = compute_rms(MinCell_MG);
            rms_Max_LG = compute_rms(MaxCell_LG);
            rms_Min_LG = compute_rms(MinCell_LG);

            rms_total = cell(8, 4);
            rms_name = ['Max_TA'; 'Min_TA'; 'Max_SU'; 'Min_SU'; 'Max_MG'; 'Min_MG'; 'Max_LG'; 'Min_LG'];

            for i = 1:8
                rms_total{i, 1} = rms_name(i, :);
            end

            for i = 1:3
                rms_total{1, i + 1} = rms_Max_TA(i);
            end
            for i = 1:3
                rms_total{2, i + 1} = rms_Min_TA(i);
            end
            for i = 1:3
                rms_total{3, i + 1} = rms_Max_SU(i);
            end
            for i = 1:3
                rms_total{4, i + 1} = rms_Min_SU(i);
            end
            for i = 1:3
                rms_total{5, i + 1} = rms_Max_MG(i);
            end
            for i = 1:3
                rms_total{6, i + 1} = rms_Min_MG(i);
            end
            for i = 1:3
                rms_total{7, i + 1} = rms_Max_LG(i);
            end
            for i = 1:3
                rms_total{8, i + 1} = rms_Min_LG(i);
            end   
            % accel Y rms
            MaxCell_Y = CollectDataAroundExtremeValue(Max_pos, Data{RMG_A_column(5)}, samplingRate);
            MinCell_Y = CollectDataAroundExtremeValue(Min_pos, Data{RMG_A_column(5)}, samplingRate);

            rms_Max_Y = compute_rms(MaxCell_Y);
            rms_Min_Y = compute_rms(MinCell_Y);

            rms_Y = cell(2, 4);
            rms_name = ['Max_accel Y'; 'Min_accel Y'];
            for i = 1:2
                rms_Y{i, 1} = rms_name(i, :);
            end

            for i = 1:3
                rms_Y{1, i + 1} = rms_Max_Y(i);
            end  
            for i = 1:3
                rms_Y{2, i + 1} = rms_Min_Y(i);
            end  
            if (z_force_find ~= -1)
                Z_force = computeZforce(Data{z_force_column(3)}, Data{z_force_column(4)});
                MaxCell_Z_force = CollectDataAroundExtremeValue(Max_pos, Z_force, samplingRate);
                MinCell_Z_force = CollectDataAroundExtremeValue(Min_pos, Z_force, samplingRate);

                rms_Max_Z_force = compute_rms(MaxCell_Z_force);
                rms_Min_Z_force = compute_rms(MinCell_Z_force);

                rms_Z_force = cell(2, 4);
                rms_name = ['Max_Z_force'; 'Min_Z_force'];
                for i = 1:2
                    rms_Z_force{i, 1} = rms_name(i, :);
                end

                for i = 1:3
                    rms_Z_force{1, i + 1} = rms_Max_Z_force(i);
                end  
                for i = 1:3
                    rms_Z_force{2, i + 1} = rms_Min_Z_force(i);
                end  

                [Z_force_UP, Z_force_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Z_force, time);
                if(size(Z_force_UP{1, 1}, 1) == 0)
                    fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                    fprintf(fileID,'Z_force_UP1 problem\n');
                    fclose(fileID);
                    continue;
                elseif(size(Z_force_UP{2, 1}, 1) == 0)
                    fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                    fprintf(fileID,'Z_force_UP2 problem\n');
                    fclose(fileID);
                    continue;
                end
                if(size(Z_force_DOWN{1, 1}, 1) == 0)
                    fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                    fprintf(fileID,'Z_force_DOWN1 problem\n');
                    fclose(fileID);
                    continue;
                elseif(size(Z_force_DOWN{2, 1}, 1) == 0)
                    fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                    fprintf(fileID,'Z_force_DOWN2 problem\n');
                    fclose(fileID);
                    continue;
                end
                
                
                f_5_8_Z_force = cell(1, 3);
                f_5_8_Z_force{1, 1} = 'f_Z_force';
                f_5_8_Z_force_s = cell(1, 3);
                f_5_8_Z_force_s{1, 1} = 'f_Z_F_S';
                [f_5_8_Z_force{1, 2}, f_5_8_Z_force{1, 3}] = GetFourier_50_80(Z_force, samplingRate, 1, 20.5, 'Z_force_all', save_folder);
                [f_5_8_Z_force_s{1, 2}, f_5_8_Z_force_s{1, 3}] = smooth_fft(Z_force, samplingRate, 1, 20.5, 'Z_force_all', save_folder);

                f_5_8_Z_force_partial = cell(2, 3);
                f_5_8_Z_force_partial{1, 1} = 'Z_force UP';
                f_5_8_Z_force_partial{2, 1} = 'Z_force DN';

                % original
                for i = 1:2
                    [f5, f8] = GetFourier_50_80(Z_force_UP{i, 1}, samplingRate, 1, 20.5, strcat('Z_force_UP ', int2str(i)), save_folder);
                    f_5_8_Z_force_partial{1, i + 1} = [f5; f8];
                end
                for i = 1:2
                    [f5, f8] = GetFourier_50_80(Z_force_DOWN{i, 1}, samplingRate, 1, 20.5, strcat('Z_force_DN ', int2str(i)), save_folder);
                    f_5_8_Z_force_partial{2, i + 1} = [f5; f8];
                end
                % smooth

                f_5_8_Z_force_partial_s = cell(2, 3);
                f_5_8_Z_force_partial_s{1, 1} = 'Z_force_s UP';
                f_5_8_Z_force_partial_s{2, 1} = 'Z_force_s DN';
                for i = 1:2
                    [f5, f8] = smooth_fft(Z_force_UP{i, 1}, samplingRate, 1, 20.5, strcat('Z_force_UP ', int2str(i)), save_folder);
                    f_5_8_Z_force_partial_s{1, i + 1} = [f5; f8];
                end
                for i = 1:2
                    [f5, f8] = smooth_fft(Z_force_DOWN{i, 1}, samplingRate, 1, 20.5, strcat('Z_force_DN ', int2str(i)), save_folder);
                    f_5_8_Z_force_partial_s{2, i + 1} = [f5; f8];
                end
                save(strcat(save_folder, 'rms_data_Z_force'), 'rms_Z_force')
                save (strcat(save_folder, 'f_5_8_Z_force'), 'f_5_8_Z_force', 'f_5_8_Z_force_partial')
                save (strcat(save_folder, 'f_5_8_Z_force_s'), 'f_5_8_Z_force_s', 'f_5_8_Z_force_partial_s')
            end
            % get duration data from EMG
            [TA_UP, TA_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data{RMG_A_column(1)}, time);
            [SU_UP, SU_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data{RMG_A_column(2)}, time);
            [MG_UP, MG_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data{RMG_A_column(3)}, time);
            [LG_UP, LG_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data{RMG_A_column(4)}, time);
            [Y_UP, Y_DOWN] = ProcessDurationEMG(Max_pos, Min_pos, Data{RMG_A_column(5)}, time);
            if(size(TA_UP{1, 1}, 1) == 0)
                fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                fprintf(fileID,'EMG_UP1 problem\n');
                fclose(fileID);
                continue;
            elseif(size(TA_UP{2, 1}, 1) == 0)
                fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                fprintf(fileID,'EMG_UP2 problem\n');
                fclose(fileID);
                continue;
            end
            
            if(size(TA_DOWN{1, 1}, 1) == 0)
                fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                fprintf(fileID,'EMG_DOWN1 problem\n');
                fclose(fileID);
                continue;
            elseif(size(TA_DOWN{2, 1}, 1) == 0)
                fileID = fopen(strcat(save_folder, 'Error_code.txt'),'w');
                fprintf(fileID,'EMG_DOWN2 problem\n');
                fclose(fileID);
                continue;
            end

            f_5_8 = cell(5, 3);
            f_names = ['f_TA';'f_SU'; 'f_MG'; 'f_LG'; 'f_Y '];
            data_type = ['TA_all'; 'SU_all'; 'MG_all'; 'LG_all'];
            for i = 1:size(f_names, 1)
                f_5_8{i, 1} = f_names(i, :);
            end
            % EMG f5 f8
            for i = 1:4
                [f_5_8{i, 2}, f_5_8{i, 3}] = GetFourier_50_80(Data{RMG_A_column(i)}, samplingRate, 10, 500, data_type(i, :), save_folder);
            end
            % Y f5 f8
            [f_5_8{5, 2}, f_5_8{5, 3}] = GetFourier_50_80(Data{RMG_A_column(5)}, samplingRate, 1, 100, 'Accel Y', save_folder);

            % smooth 
            f_5_8_s = cell(5, 3);
            f_names = ['f_TA_s';'f_SU_s'; 'f_MG_s'; 'f_LG_s'; 'f_Y_s '];
            data_type = ['TA_all'; 'SU_all'; 'MG_all'; 'LG_all'];
            for i = 1:size(f_names, 1)
                f_5_8_s{i, 1} = f_names(i, :);
            end
            % EMG f5 f8
            for i = 1:4
                [f_5_8_s{i, 2}, f_5_8_s{i, 3}] = smooth_fft(Data{RMG_A_column(i)}, samplingRate, 10, 500, data_type(i, :), save_folder);
            end
            % Y f5 f8
            [f_5_8_s{5, 2}, f_5_8_s{5, 3}] = smooth_fft(Data{RMG_A_column(5)}, samplingRate, 1, 100, 'Accel Y', save_folder);

            %partial

            f_5_8_partial = cell(10, 3);
            f_names = ['TA_UP'; 'TA_DN'; 'SU_UP'; 'SU_DN'; 'MG_UP'; 'MG_DN'; 'LG_UP'; 'LG_DN'; 'Y_UP '; 'Y_DN '];

            for i = 1:10
                f_5_8_partial{i, 1} = f_names(i, :);
            end

            for i = 1:2
                [f5, f8] = GetFourier_50_80(TA_UP{i, 1}, samplingRate, 10, 500, strcat('TA_UP ', int2str(i)), save_folder);
                f_5_8_partial{1, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(TA_DOWN{i, 1}, samplingRate, 10, 500, strcat('TA_DOWN ', int2str(i)), save_folder);
                f_5_8_partial{2, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(SU_UP{i, 1}, samplingRate, 10, 500, strcat('SU_UP ', int2str(i)), save_folder);
                f_5_8_partial{3, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(SU_DOWN{i, 1}, samplingRate, 10, 500, strcat('SU_DOWN ', int2str(i)), save_folder);
                f_5_8_partial{4, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(MG_UP{i, 1}, samplingRate, 10, 500, strcat('MG_UP ', int2str(i)), save_folder);
                f_5_8_partial{5, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(MG_DOWN{i, 1}, samplingRate, 10, 500, strcat('MG_DOWN ', int2str(i)), save_folder);
                f_5_8_partial{6, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(LG_UP{i, 1}, samplingRate, 10, 500, strcat('LG_UP ', int2str(i)), save_folder);
                f_5_8_partial{7, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = GetFourier_50_80(LG_DOWN{i, 1}, samplingRate, 10, 500, strcat('LG_DOWN ', int2str(i)), save_folder);
                f_5_8_partial{8, i + 1} = [f5; f8];
            end
            % Accel Y partial
            for i = 1:2
                [f5, f8] = GetFourier_50_80(Y_UP{i, 1}, samplingRate, 1, 100, strcat('Accel Y_UP ', int2str(i)), save_folder);
                f_5_8_partial{9, i + 1} = [f5; f8];
            end

            for i = 1:2
                [f5, f8] = GetFourier_50_80(Y_DOWN{i, 1}, samplingRate, 1, 100, strcat('Y_DOWN ', int2str(i)), save_folder);
                f_5_8_partial{10, i + 1} = [f5; f8];
            end

            % smooth
            f_5_8_partial_s = cell(10, 3);
            f_names = ['TA_UP_s'; 'TA_DN_s'; 'SU_UP_s'; 'SU_DN_s'; 'MG_UP_s'; 'MG_DN_s'; 'LG_UP_s'; 'LG_DN_s'; 'Y_UP_s '; 'Y_DN_s '];

            for i = 1:10
                f_5_8_partial_s{i, 1} = f_names(i, :);
            end

            for i = 1:2
                [f5, f8] = smooth_fft(TA_UP{i, 1}, samplingRate, 10, 500, strcat('TA_UP ', int2str(i)), save_folder);
                f_5_8_partial_s{1, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(TA_DOWN{i, 1}, samplingRate, 10, 500, strcat('TA_DOWN ', int2str(i)), save_folder);
                f_5_8_partial_s{2, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(SU_UP{i, 1}, samplingRate, 10, 500, strcat('SU_UP ', int2str(i)), save_folder);
                f_5_8_partial_s{3, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(SU_DOWN{i, 1}, samplingRate, 10, 500, strcat('SU_DOWN ', int2str(i)), save_folder);
                f_5_8_partial_s{4, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(MG_UP{i, 1}, samplingRate, 10, 500, strcat('MG_UP ', int2str(i)), save_folder);
                f_5_8_partial_s{5, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(MG_DOWN{i, 1}, samplingRate, 10, 500, strcat('MG_DOWN ', int2str(i)), save_folder);
                f_5_8_partial_s{6, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(LG_UP{i, 1}, samplingRate, 10, 500, strcat('LG_UP ', int2str(i)), save_folder);
                f_5_8_partial_s{7, i + 1} = [f5; f8];
            end
            for i = 1:2
                [f5, f8] = smooth_fft(LG_DOWN{i, 1}, samplingRate, 10, 500, strcat('LG_DOWN ', int2str(i)), save_folder);
                f_5_8_partial_s{8, i + 1} = [f5; f8];
            end
            % Accel Y partial
            for i = 1:2
                [f5, f8] = smooth_fft(Y_UP{i, 1}, samplingRate, 1, 100, strcat('Accel Y_UP ', int2str(i)), save_folder);
                f_5_8_partial_s{9, i + 1} = [f5; f8];
            end

            for i = 1:2
                [f5, f8] = smooth_fft(Y_DOWN{i, 1}, samplingRate, 1, 100, strcat('Y_DOWN ', int2str(i)), save_folder);
                f_5_8_partial_s{10, i + 1} = [f5; f8];
            end



            save (strcat(save_folder, 'f_5_8'), 'f_5_8', 'f_5_8_partial')
            save (strcat(save_folder, 'f_5_8_s'), 'f_5_8_s', 'f_5_8_partial_s')
            save(strcat(save_folder, 'rms_data'), 'rms_Y', 'rms_total')
        end
        
    end
    %test_name = sprintf('%s',data_directory{p, 2});
    
end
%root_name = '../data/Control/';
%folder_name = 'c134_20160831/';
%test_name = 'L_SSC2';
%save_folder = strcat(root_name, 'output/', folder_name, test_name, '/');
%ab_folder_name = strcat(root_name, folder_name);
%load (strcat(ab_folder_name , test_name, '.mat')) %load data
% Data{i} correspond to what data. (in SSC)
% i      |     Data
% 1      |     TA
% 2      |     SU
% 3      |     MG
% 4      |     LG
% 5~7    |     Accel x, y, z
% 8~15   |     channel 1~8
% 16     |     belt receiver
% 17     |     knee's degree
% 18     |     ankle's degree
%compute time




%save('Processed_Data.mat')

%subplot(1, 2, 1);
%plot(time, Z_force);
%subplot(1, 2, 2);
%plot(time, Data{18});
%subplot(1, 3, 3);
%plot(time, Data{7});
%subplot(2, 4, 4);
%plot(time, Data{11});
%subplot(2, 4, 5);
%plot(time, Data{12});
%subplot(2, 4, 6);
%plot(time, Data{13});
%subplot(2, 4, 7);
%plot(time, Data{14});
%subplot(2, 4, 8);
%plot(time, Data{15});
