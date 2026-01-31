clc; clear; close all;

%% -------------------------------
%  Load CSV file
%  (change filename here)
%% -------------------------------
file = 'weight_balck_15cm';
T = readtable(file);

% Extract columns
sample = T.sample;
sonar  = T.sonar;
lidar  = T.lidar;
ir     = T.ir;
fused  = T.fused;

%% -------------------------------
%  Create a single subplot
%% -------------------------------
figure;
plot(sample, sonar, 'b', 'LineWidth', 1.4); hold on;
plot(sample, lidar, 'r', 'LineWidth', 1.4);
plot(sample, fused, 'k', 'LineWidth', 1.6);

% IR is digital → scale to show visibly (optional)
plot(sample, ir * max([sonar; lidar; fused]) * 0.1, ...
     'g--', 'LineWidth', 1.2);  

grid on;
xlabel('Sample Number');
ylabel('Distance (cm)');
title('Sonar, LIDAR, IR Trigger, and Fused Distance (Black)');
legend('Sonar', 'LIDAR', 'Fused', 'IR (scaled digital)');
set(gca, 'FontSize', 12);
