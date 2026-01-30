# Experiment Design

## Goal
Quantify how surface reflectivity and material properties affect distance sensors and demonstrate a fusion pipeline that remains stable under sensor disagreement/dropouts.

## Variables
**Independent variables**
- Surface: white cardboard, black cloth, metal
- Distance: 5 cm, 15 cm, 30 cm

**Dependent variables**
- Raw sensor distance (LiDAR, sonar)
- Fused distance
- Filtered distance (Kalman)
- Error metrics (RMSE), dispersion (variance)

## Sampling plan
- 100 samples per sensor per condition
- Logged via Arduino serial → CSV
- Analysed in MATLAB

## What “good” looks like
- Low variance under stable conditions
- Minimal dropouts/spikes
- Fusion output stable when one sensor degrades
- Kalman output smoother than raw fused stream without lag becoming problematic for demo logic
