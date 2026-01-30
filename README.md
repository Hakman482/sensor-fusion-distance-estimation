# Sensor Fusion for Distance Estimation (LiDAR + Sonar + Kalman)

Robust short-range distance estimation using **LiDAR + ultrasonic sonar fusion**, improved via **multipoint calibration**, **noise-based weighting**, and a **1-D Kalman filter** for real-time smoothing. Includes an **RGB distance indicator** demo for intuitive obstacle awareness.

> Built as a Sensing & Sensor Fusion course project (ARU). Focus: practical reliability under surface-dependent sensing failure modes.

---

## Why this matters (engineering problem)
Single sensors fail in predictable ways:
- **LiDAR (ToF)** can drop out or spike on **dark/absorptive** or **angle-dependent reflective** surfaces.
- **Sonar** is generally colour-independent but can jitter with geometry, edge effects, or multi-path.
- **Analogue IR distance** is often nonlinear and surface-dependent at short range (not reliable for quantitative fusion in this build).

This repo shows a **simple, implementable pipeline** that keeps distance estimation usable even when one sensor becomes unreliable.

---

## System overview
**Pipeline**
1. **Multipoint calibration** (reduce offset/bias)
2. **Sensor behaviour profiling** (surface + range)
3. **Weighted fusion** (reliability-based blending)
4. **1-D Kalman filtering** (smooth fused estimate + provide uncertainty)
5. **RGB indicator demo** (Green/Yellow/Red distance zones)

**Hardware**
- Arduino UNO
- LiDAR ToF sensor (VL53-series class)
- Ultrasonic sonar (HC-SR04 class)
- IR module (used as **digital trigger**)
- RGB LED module, breadboard, jumper wires, USB

**Software**
- Arduino IDE (sampling + serial logging + demo logic)
- MATLAB (visualisation, RMSE/analysis, plots)
- Excel (data logging/csv prep)

---

## Key features
- ✅ **Surface-aware robustness**: fusion falls back to the more reliable sensor when the other drops out.
- ✅ **Noise reduction**: Kalman filter improves stability for downstream logic (e.g., thresholds).
- ✅ **Reproducible experiment design**: controlled distances and surfaces with consistent sampling.
- ✅ **Demo-ready output**: RGB indicator based on fused distance zones.

---

## Results at a glance (what to look for)
Experiments were run at **5 cm, 15 cm, 30 cm** on **white cardboard, black cloth, metal**.

Typical observations:
- **Black cloth**: LiDAR often becomes unreliable (low reflectivity) → fusion becomes **sonar-dominant**; Kalman smooths jitter.
- **Metal**: both sensors usable but LiDAR can show angle-dependent artefacts → fusion + Kalman converges between sensors.
- **White**: both sensors strong; filter rejects early outliers/spikes and stabilises quickly.

> See `docs/results.md` and exported plots in `results/`.

---

## Repository structure


