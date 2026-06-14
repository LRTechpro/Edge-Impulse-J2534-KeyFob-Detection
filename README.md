# MS579 Computer Vision — Assignment 4.1
## Edge Impulse FOMO Object Detection: J2534 VCI & Key Fob

**Student:** Harold L. R. Watkins | **Course:** MS579 | **Institution:** UAT | **Semester:** Summer 2026

---

## Project Overview

Custom Edge Impulse FOMO MobileNetV2 0.35 object detection model for two automotive cybersecurity artifacts deployed on ESP32-CAM (no PSRAM required).

| Label | Object | Images |
|-------|--------|--------|
| `keyfob` | Remote key fob | 75 |
| `J2534VCI` | J2534 VCI adapter | 75 |
| (none) | Background | 30 |
| **Total** | | **180** |

**Split:** 141 training (79%) / 39 testing (21%)

---

## Hardware & Software

- ESP32-CAM (AI Thinker, OV2640, 240x240 px)
- Arduino IDE 2.3.9 + EloquentEsp32cam library
- Edge Impulse Studio — FOMO MobileNetV2 0.35, 48x48 RGB, int8 EON Compiler

---

## Impulse Config

Input: 48x48 RGB | Training: 60 epochs, LR=0.005, Adam | Labels: `keyfob` / `J2534VCI`

---

## Files

- `Collect_Images_for_EdgeImpulse.ino` — Arduino sketch (credentials redacted)
- `screenshots/` — Arduino IDE setup
- `contact-sheets/` — Dataset contact sheets
- `report/` — Full PDF report (APA 7, graduate-level)

---

## References

- Dronebot Workshop. (2023). Simple ESP32-CAM object detection [Video]. YouTube.
- Edge Impulse. (2024). FOMO object detection algorithm. https://docs.edgeimpulse.com
- SAE International. (2015). SAE J2534. https://www.sae.org/standards/content/j2534/
