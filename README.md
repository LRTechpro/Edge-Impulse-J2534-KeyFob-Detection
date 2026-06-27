# ESP32-CAM FOMO Automotive Object Detection Research

An experimental embedded computer vision project that deploys an Edge Impulse FOMO object detector to an AI Thinker ESP32-CAM for recognizing two automotive artifacts:

- `keyfob`
- `J2534VCI`

The work focuses on model deployment, live-frame validation, controlled object detection, simultaneous multi-class detection, and a follow-on distance experiment using grid-based FOMO outputs.

## Why this project matters

Low-cost edge vision can support physical security, tool inventory, diagnostic-bay monitoring, and embedded AI prototyping without requiring a cloud-connected inference service. This repository documents a reproducible engineering workflow from trained model to live ESP32-CAM inference, including both successful results and observed limitations.

## System architecture

```text
OV2640 camera
    -> 96 x 96 RGB565 capture buffer
    -> Edge Impulse preprocessing
    -> 48 x 48 FOMO model input
    -> centroid/grid-cell detections
    -> serial diagnostics and CSV output
```

## Hardware and software

| Component | Configuration |
|---|---|
| Board | AI Thinker ESP32-CAM |
| Camera | OV2640 |
| ESP32 core | 3.3.10 |
| IDE | Arduino IDE 2.3.10 |
| Camera capture | 96 x 96 RGB565 |
| Model input | 48 x 48 RGB |
| Learning block | Edge Impulse FOMO |
| Classes | `keyfob`, `J2534VCI` |
| Typical inference time | Approximately 197 ms per frame |
| Serial baud | 115200 |

## Research questions

1. Can a compact FOMO model run reliably on an ESP32-CAM?
2. Can the model distinguish a key fob from a J2534 vehicle communication interface?
3. Can both classes be detected in the same frame?
4. Can reported FOMO width, height, or area be used as a practical distance proxy?

## Main findings

### Embedded deployment

- Firmware compiled and uploaded successfully to the ESP32-CAM.
- Changing frame checksums confirmed that fresh image data was being processed.
- Controlled J2534VCI testing produced 20 consecutive correct detections.
- Controlled key-fob testing produced sustained correct detections.
- Both classes were detected simultaneously in multiple frames.
- In Frame 1519, both `keyfob` and `J2534VCI` were reported at 0.97 confidence.

### Distance experiment

A centered key fob was evaluated at 22 cm, 33 cm, and 44 cm with 10 samples per distance.

| Distance | Correct detections | Detection rate | Average confidence | Reported size |
|---:|---:|---:|---:|---:|
| 22 cm | 9/10 | 90% | 0.939 | 8 x 8 |
| 33 cm | 10/10 | 100% | 0.911 | 8 x 8 |
| 44 cm | 10/10 | 100% | 0.908 | 8 x 8 |

The detector remained effective across the tested distances, but the reported width, height, and area stayed fixed at 8 x 8 and 64 model units. This shows that the deployed FOMO output is useful for classification and centroid localization, but it is not a calibrated continuous object-size measurement for range estimation.

## Repository structure

```text
.
├── README.md
├── CITATION.cff
├── CONTRIBUTING.md
├── .gitignore
├── src/
│   └── keyfob_distance_experiment/
│       └── ESP32CAM_FOMO_Keyfob_Distance_Experiment_22_33_44cm.ino
├── docs/
│   ├── methodology.md
│   ├── results.md
│   ├── troubleshooting.md
│   └── model-limitations.md
├── hardware/
│   └── esp32cam_configuration.md
├── model/
│   └── model_summary.md
├── data/
│   └── keyfob_distance_results.csv
└── evidence/
    ├── controlled_j2534vci_detection.txt
    ├── controlled_keyfob_detection.txt
    ├── controlled_two_class_detection.txt
    └── keyfob_distance_serial_log.txt
```

## Build and run

1. Install the Espressif ESP32 board package in Arduino IDE.
2. Install EloquentEsp32cam.
3. Export the trained Edge Impulse model as an Arduino library and install it locally.
4. Open the sketch in `src/keyfob_distance_experiment`.
5. Select `AI Thinker ESP32-CAM`.
6. Set CPU frequency to 240 MHz, PSRAM enabled, and Huge APP partition.
7. Compile and upload.
8. Open Serial Monitor at 115200 baud with `Newline` enabled.
9. Enter `C22`, `C33`, or `C44` to collect a 10-sample run.

The generated Edge Impulse inferencing library is not redistributed here. Install the library exported from your own Edge Impulse project and update the include name if necessary.

## Diagnostic design

The firmware records:

- distance command
- sample number
- frame number
- class status
- object count
- label
- centroid and grid-cell coordinates
- reported width and height
- calculated area
- confidence
- inference time
- framebuffer checksum

The checksum samples every eighth byte across the full framebuffer. It is intended as a lightweight freshness diagnostic, not a cryptographic hash.

## Limitations

- Results depend on object pose, scale, lighting, distance, and frame position.
- FOMO can produce duplicate grid activations around one physical object.
- A reported grid cell is not equivalent to a true pixel-accurate bounding box.
- The distance study used a small controlled sample and does not establish a general ranging model.
- The model is a research prototype, not a production-certified detection system.

## Future research

- Expand pose, background, and lighting diversity.
- Add calibrated camera geometry and a true size estimator.
- Compare FOMO centroid output with a bounding-box regression model.
- Evaluate temporal smoothing and duplicate-cell merging.
- Test additional automotive diagnostic tools and key-fob designs.
- Measure false-positive behavior in cluttered workshop environments.

## Skills demonstrated

Embedded C++, ESP32, Arduino, Edge Impulse, FOMO, object detection, model deployment, serial diagnostics, framebuffer analysis, performance evaluation, controlled testing, and technical documentation.

## Third-party components

This project uses or references Edge Impulse, Espressif ESP32 camera software, Arduino, and EloquentEsp32cam. Their respective licenses and documentation apply. No claim of authorship is made over third-party libraries.

## Project status

Experimental research prototype with successful controlled deployment and documented generalization limits.
