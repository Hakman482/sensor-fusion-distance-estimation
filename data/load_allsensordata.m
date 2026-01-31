clear; clc; close all;

% Select folder
folder = uigetdir('.', 'Select folder containing CSV sensor files');

% Get all CSV files
files = dir(fullfile(folder, '*.csv'));

disp("FILES MATLAB SEES:");
{files.name}'

%% ---- Process each file ----
for i = 1:length(files)

    filename = files(i).name;
    filepath = fullfile(folder, filename);

    % Load data
    T = readtable(filepath);

    %% ---- Parse filename safely ----
    nameOnly = erase(filename, '.csv');  
    parts = split(nameOnly, '_');        

    if numel(parts) < 3
        error("Filename '%s' does not follow format: sensor_surface_distance.csv", filename);
    end

    sensor  = lower(string(parts{1}));
    surface = lower(string(parts{2}));

    dist_str = lower(string(parts{3}));
    dist_str = erase(dist_str, 'cm');
    distance = str2double(dist_str);

    if isnan(distance)
        error("Could not extract distance from filename '%s'", filename);
    end

    %% ---- Store data ----
    dataStruct.(sensor).(surface).(sprintf("d%d", distance)) = T;

    fprintf("Loaded: %-25s | Sensor: %-6s | Surface: %-8s | Dist: %d cm\n", ...
            filename, sensor, surface, distance);
end
