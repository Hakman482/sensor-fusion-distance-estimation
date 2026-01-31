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


---

## How to run (end-to-end)

### A) Arduino: capture distance streams
1. Wire sensors to the UNO (see `hardware/wiring.md`).
2. Upload `src/arduino/main.ino`.
3. Open Serial Monitor / log serial output to CSV.

### B) MATLAB: visualise + analyse
1. Put captured CSVs into `data/`
2. Run:
   - `src/matlab/plot_raw_vs_fused.m`
   - `src/matlab/kalman_filter_1d.m`
   - `src/matlab/rmse_comparison.m`
3. Export plots to `results/`.

---

## Engineering notes (important details)
### Weighted fusion (noise-based)
For sensors `i ∈ {lidar, sonar}`, fused distance:
\[
\hat{x} = \frac{\sum_i w_i x_i}{\sum_i w_i}
\]
Where weights reflect reliability (commonly inverse variance):
\[
w_i = \frac{1}{\sigma_i^2}
\]

### Why fused RMSE can be higher (a real failure case)
Fusion does **not** automatically cancel error if sensors share similar **bias** or if weights are mis-specified for a given surface/range. In that case the fused estimate can “inherit” systematic error from both sensors.

---

## Limitations
- Fixed noise-based weights do not fully adapt to changing surfaces/angles.
- Hardware alignment and mounting can introduce repeatable bias.
- Range tested was narrow (5–30 cm); wider ranges and moving targets were not evaluated here.

## Roadmap
- Adaptive weighting (surface-aware / innovation-based)
- Wider distance range + more materials + angle sweeps
- Moving-target tests
- Replace analogue IR with digital ToF or use IR strictly as trigger

---

## Quick resume bullets (paste into CV)
- Built an embedded **distance-estimation pipeline** using **LiDAR + sonar fusion** with **multipoint calibration**, **noise-based weighting**, and **1-D Kalman filtering** for stable real-time output.
- Designed controlled experiments across **three surfaces** and **three ground-truth distances**, logging and analysing sensor behaviour with MATLAB.
- Implemented an **RGB distance indicator** demo driven by fused distance zones for obstacle awareness.

---

## References
- STMicroelectronics VL53-series ToF sensor datasheet (VL53L0X class)
- HC-SR04 ultrasonic ranging module datasheet
- Grewal & Andrews, *Kalman Filtering: Theory and Practice Using MATLAB*



## Hardware build & test evidence

**Bench prototype (Arduino + sensors):**
<img src="hardware/photos/sensors_upclose.png" alt="ground distance" width="400">


**Repeatable distance testing using tape measure (ground truth):**
<img src="hardware/photos/calc_dist_black.png" alt="ground distance" width="400">
<img src="hardware/photos/calc_dist_black.png" alt="ground distance" width="400">
<img src="hardware/photos/calc_dist_black.png" alt="ground distance" width="400">


