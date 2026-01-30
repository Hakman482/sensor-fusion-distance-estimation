# Methodology

This project implements a practical sensor fusion pipeline for short-range distance estimation using LiDAR + ultrasonic sonar, with IR repurposed as a binary trigger.

## 1) Calibration: offset, bias, noise
**Offset**: constant shift in measurements  
**Bias**: systematic scaling/shift that persists across time  
**Noise**: random fluctuations around the true value

### Multipoint calibration procedure
Reference distances: **5 cm, 15 cm, 30 cm**
1. Place target at reference distance.
2. Record multiple readings (e.g., 5 repeats) per sensor.
3. Compute average measured value and compare to ground truth.
4. Apply calibration mapping (offset/bias correction).
5. Re-test to confirm error reduction.

> Output: calibrated LiDAR and sonar streams used downstream for fusion.

## 2) Individual sensor profiling (surface + range)
Sensors are tested across:
- Surfaces: **white cardboard**, **black cloth**, **metal**
- Distances: **5 cm**, **15 cm**, **30 cm**
- Sampling: **100 readings** per condition

Purpose:
- Identify dropout patterns and surface dependence
- Estimate noise levels for weighting

## 3) Weighted fusion (LiDAR + sonar)
Weighted average fuses both sensors:

\[
\hat{x} = \frac{w_L x_L + w_S x_S}{w_L + w_S}
\]

Common choice for weights:
\[
w_i = \frac{1}{\sigma_i^2}
\]
where \(\sigma_i^2\) is the measured variance of sensor `i` in the current test condition.

### Practical behaviour
- If LiDAR becomes unstable on black cloth, its estimated variance increases → weight decreases → fusion becomes sonar-dominant.
- If both sensors are valid, fusion lies between them.

## 4) 1-D Kalman filter on fused output
Kalman filter smooths the fused output and provides a running uncertainty estimate.

State: distance \(x\)

**Predict**
\[
x^- = x
\]
\[
P^- = P + Q
\]

**Update**
\[
K = \frac{P^-}{P^- + R}
\]
\[
x = x^- + K(z - x^-)
\]
\[
P = (1-K)P^-
\]

Where:
- \(z\) = fused measurement
- \(Q\) = process noise (expected motion/variation)
- \(R\) = measurement noise (from fused stream)
- \(P\) = estimate uncertainty

## 5) IR repurposing
The analogue IR distance behaviour was nonlinear/unreliable across 1–30 cm in this setup.
Therefore, IR is used as a **digital trigger** (presence/absence), improving system stability for demonstration logic.
