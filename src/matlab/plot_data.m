%% ================================================
%   MATLAB SCRIPT FOR SENSOR FUSION VISUALIZATION
%   Plots Sonar, LIDAR, IR, and Fused Distance
%   Author: (You)
%% ================================================

clear; clc; close all;

%% ---- Load Data ----


data = readtable('sense_data.csv');

% Extract columns
sample = data.sample;
sonar  = data.sonar;
lidar  = data.lidar;
ir     = data.ir;
fused  = data.fused;

%% ---- Create Time Vector (assuming 100 ms sampling) ----
dt = 0.1;   % 100 ms between samples (delay(100))
t = sample * dt;


%% ---- Plot 1: Raw Sensor Distances ----
figure('Name','Raw Sensor Readings','NumberTitle','off');
plot(t, sonar, 'b', 'LineWidth', 1.5); hold on;
plot(t, lidar, 'r', 'LineWidth', 1.5);

xlabel('Time (s)');
ylabel('Distance (cm)');
title('Sonar vs LIDAR Raw Distances');
legend('Sonar','LIDAR');
grid on;


%% ---- Plot 2: Fused Distance ----
figure('Name','Fused Distance','NumberTitle','off');
plot(t, fused, 'k', 'LineWidth', 2);
xlabel('Time (s)');
ylabel('Fused Distance (cm)');
title('Weighted Fused Distance Output');
grid on;


%% ---- Plot 3: All Sensors + Fusion ----
figure('Name','Full Sensor Comparison','NumberTitle','off');
plot(t, sonar, '--b', 'LineWidth', 1.2); hold on;
plot(t, lidar, '--r', 'LineWidth', 1.2);
plot(t, fused, 'k', 'LineWidth', 2);

xlabel('Time (s)');
ylabel('Distance (cm)');
title('Sonar, LIDAR, and Fused Distance');
legend('Sonar','LIDAR','Fused');
grid on;


%% ---- Plot 4: IR Detection Events ----
figure('Name','IR Obstacle Detection','NumberTitle','off');
stem(t, ir == 0, 'filled');   % IR = 0 means OBSTACLE
xlabel('Time (s)');
ylabel('IR Detection');
title('IR Obstacle Events (1 = Obstacle)');
grid on;

