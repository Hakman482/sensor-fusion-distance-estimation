%% ================================================
%   SENSOR vs KALMAN VISUALIZATION (Workspace Data)
%   Assumes your CSV is already imported as a table in the Workspace.
%   Table must contain columns: sample, sonar, lidar, ir, fused
%% ================================================

clearvars -except data sense_data T; clc; close all;

%% ---- Pick the table already in Workspace ----
% If your imported table variable is named "data", this will work immediately.
% Otherwise set tbl = <yourTableName>;
if exist('data','var') && istable(data)
    tbl = data;
elseif exist('sense_data','var') && istable(sense_data)
    tbl = sense_data;
elseif exist('T','var') && istable(T)
    tbl = T;
else
    error('No table found. Import your CSV as a table (e.g., variable name: data), then run again.');
end

%% ---- Extract columns ----
sample = tbl.sample;
sonar  = tbl.sonar;
lidar  = tbl.lidar;
ir     = tbl.ir;
kalman = tbl.fused;   % fused column is your Kalman-filtered distance

%% ---- Time vector ----
dt = 0.12;            % set to match Arduino delay; e.g., delay(120) -> 0.12 s
t  = sample * dt;

%% ---- Plot 1: Sensors vs Kalman ----
figure('Name','Sensors vs Kalman Output','NumberTitle','off');
plot(t, sonar, 'LineWidth', 1.2); hold on;
plot(t, lidar, 'LineWidth', 1.2);
plot(t, kalman,'LineWidth', 2.0);
grid on;
xlabel('Time (s)');
ylabel('Distance (cm)');
title('Sonar and LiDAR Compared to Kalman Output');
legend('Sonar','LiDAR','Kalman','Location','best');

%% ---- Plot 2: Residuals (Sensor - Kalman) ----
figure('Name','Residuals Relative to Kalman','NumberTitle','off');
res_sonar = sonar - kalman;
res_lidar = lidar - kalman;

plot(t, res_sonar, 'LineWidth', 1.2); hold on;
plot(t, res_lidar, 'LineWidth', 1.2);
yline(0, '--', 'LineWidth', 1.0);
grid on;
xlabel('Time (s)');
ylabel('Residual (cm)');
title('Residuals: Sensor - Kalman');
legend('Sonar - Kalman','LiDAR - Kalman','Zero','Location','best');

%% ---- Plot 3: IR events (logged only) ----
figure('Name','IR Obstacle Events','NumberTitle','off');
stem(t, ir == 0, 'filled');   % IR=0 means obstacle
grid on;
xlabel('Time (s)');
ylabel('Obstacle (1=yes)');
title('IR Obstacle Events');

%% ---- Metrics: RMSE vs Kalman ----
rmse = @(e) sqrt(mean(e.^2, 'omitnan'));
fprintf('RMSE relative to Kalman:\n');
fprintf('  Sonar RMSE: %.4f cm\n', rmse(res_sonar));
fprintf('  LiDAR RMSE: %.4f cm\n', rmse(res_lidar));
