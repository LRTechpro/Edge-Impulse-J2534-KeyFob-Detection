# MS579 Computer Vision: Assignment 4.1
## Edge Impulse Object Detection: J2534 VCI and Key Fob Detection

**Student:** Harold L. R. Watkins  
**Course:** MS579 Computer Vision  
**Institution:** University of Advancing Technology (UAT)  
**Semester:** Summer 2026  

---

## Project Overview

This repository documents the complete data collection, labeling, training, and deployment pipeline for a custom Edge Impulse FOMO (Faster Objects, More Objects) object detection model targeting two automotive cybersecurity artifacts:

| Class Label | Object | Images |
|-------------|--------|--------|
| `keyfob` | Remote key fob (PEPS/passive entry system) | 75 |
| `J2534VCI` | J2534 Vehicle Communication Interface adapter | 75 |
| *(none)* | Background (no object present) | 30 |
| **Total** | | **180** |

---

## Hardware & Software Stack

| Component | Details |
|-----------|---------|
| Camera Module | ESP32-CAM (AI Thinker), OV2640, 240×240 px |
| IDE | Arduino IDE 2.3.9 |
| Camera Library | [EloquentEsp32cam](https://github.com/eloquentarduino/EloquentEsp32cam) |
| ML Platform | [Edge Impulse Studio](https://edgeimpulse.com) |
| Model Architecture | FOMO MobileNetV2 0.35 |
| Input Resolution | 48 × 48 × 3 (RGB) |
| Quantization | int8 (EON Compiler) |
| PSRAM | Not required |

---

## Repository Structure

```
├── README.md                          # This file
├── Collect_Images_for_EdgeImpulse.ino # Arduino sketch for image collection
├── arduino-library/
│   └── ei-haroldlrwatkins-project-1-arduino-1.0.1-impulse-#1.zip
├── screenshots/
│   ├── ESP32_Espressif_Board_Package_Installed.png
│   └── ESP32CAM_Board_Selected_EloquentEsp32cam_Installed.png
├── contact-sheets/
│   ├── keyFob_selected_contact_sheet.jpg
│   ├── j2534VCI_selected_contact_sheet.jpg
│   └── background_selected_contact_sheet.jpg
└── report/
    └── MS579_Assignment4_1_Harold_Watkins_Final_Report.pdf
```

---

## Arduino Image Collection Sketch

The `Collect_Images_for_EdgeImpulse.ino` sketch turns the ESP32-CAM into a Wi-Fi-connected image capture server. Once flashed and powered:

1. Connect the ESP32-CAM to the same Wi-Fi network as your computer.
2. Open the Serial Monitor (115200 baud) to see the device IP address.
3. Navigate to the address in a browser to access the image capture interface.
4. Click "Capture" to save images directly to Edge Impulse.

> **Security Note:** Replace `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` in the sketch with your actual Wi-Fi credentials before flashing. Never commit real credentials to a public repository.

---

## Impulse Configuration (Edge Impulse)

```
Input Block:    Image — 48×48 RGB, Fit shortest axis
Processing:     Image (normalize to [0, 1])
Learning Block: Object Detection (FOMO MobileNetV2 0.35)
Training:       30 cycles, LR = 0.005, data augmentation enabled
Augmentation:   Enabled (flip, brightness jitter, rotation)
Quantization:   int8 post-training (EON Compiler)
```

**Dataset Split:** 141 training / 39 testing (79% / 21%)

---

## Key Design Decisions

**Why FOMO over YOLO?**  
FOMO eliminates the anchor regression head and NMS post-processing, reducing MACs by ~100× versus SSD/YOLO-Nano. This enables real-time inference on the ESP32's Xtensa LX6 cores without PSRAM.

**Why manual bounding boxes?**  
The Edge Impulse Gemini AI-labeling tool was tested but encountered API rate-limit quota errors. Manual annotation was preferred for ground-truth quality — tight, rule-governed boxes produce better FOMO convergence than variable API-generated boxes.

**Why 75 images per class?**  
Meets the assignment target while remaining practical for manual annotation. Background images (30) provide approximately 14% of the dataset — within the recommended 10–20% range for FOMO false-positive suppression.

**Label casing:**  
- `keyfob` — all lowercase, matches Edge Impulse output tensor exactly  
- `J2534VCI` — capital J, capital VCI, no separators — avoids tokenization ambiguity in generated C++ headers

---

## Security Considerations

This project targets automotive cybersecurity artifacts. The J2534 VCI adapter is a high-value attack surface for ECU reflashing and diagnostic replay attacks. Visual detection of these devices in a facility context supports:

- Physical access control enforcement
- Unauthorized diagnostic session alerting
- Inventory and chain-of-custody monitoring

No proprietary calibration data, vehicle PII, or ECU firmware is included in this repository.

---

## References

- Dronebot Workshop. (2023, June 25). *Simple ESP32-CAM object detection* [Video]. YouTube. https://www.youtube.com/watch?v=HDRvZ_BYd08
- Edge Impulse. (2024). *FOMO: Faster objects, more objects object detection algorithm*. https://docs.edgeimpulse.com/docs/edge-impulse-studio/learning-blocks/object-detection/fomo-object-detection-algorithm
- Eloquent Arduino. (2024). *EloquentEsp32cam* [Software]. GitHub. https://github.com/eloquentarduino/EloquentEsp32cam
- SAE International. (2015). *SAE J2534: Recommended practice for pass-thru vehicle programming*. https://www.sae.org/standards/content/j2534/
- Zanardi, A., Bourdoukan, A., & Shafahi, A. (2022). *FOMO: Detection of small objects on embedded devices*. Edge Impulse Research. https://edgeimpulse.com/blog/fomo

---

*Report PDF and full assignment submission: see `/report/MS579_Assignment4_1_Harold_Watkins_Final_Report.pdf`*
