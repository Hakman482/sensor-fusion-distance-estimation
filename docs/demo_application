# Application Demo: RGB Distance Indicator

## What it demonstrates
A simple real-time “distance zone” indicator driven by the fused distance estimate.
Purpose: show how fusion output is usable for immediate decision logic.

## Logic
- The fused distance is continuously updated.
- A Kalman filter smooths the value to prevent flicker.
- RGB colour changes based on zones:

Example zones (tune for your build):
- Green: object far / safe
- Yellow: approaching / caution
- Red: very close / danger

## Why fusion matters here
If you drive RGB zones from a single sensor:
- LiDAR on black cloth can cause false “far/unknown” readings
- Sonar alone can jitter near edges
Fusion + filtering yields steadier, more reliable zone switching.
