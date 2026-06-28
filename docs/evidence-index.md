# Evidence Index

## Existing setup screenshots

### ESP32 board package installed

![ESP32 board package installed](../screenshots/ESP32_Espressif_Board_Package_Installed.png)

### AI Thinker ESP32-CAM selected with EloquentEsp32cam

![AI Thinker ESP32-CAM selected](../screenshots/ESP32CAM_Board_Selected_EloquentEsp32cam_Installed.png)

## Dataset contact sheets

### Key-fob samples

![Key-fob contact sheet](../contact-sheets/keyFob_selected_contact_sheet.jpg)

### J2534VCI samples

![J2534VCI contact sheet](../contact-sheets/j2534VCI_selected_contact_sheet.jpg)

### Negative/background samples

![Background contact sheet](../contact-sheets/background_selected_contact_sheet.jpg)

## Technical figures

### Embedded inference pipeline

![Embedded inference pipeline](figures/system-pipeline.svg)

### Simultaneous-detection evidence

![Frame 1519 evidence](figures/frame-1519.svg)

### Distance-experiment result

![Distance experiment result](figures/distance-results.svg)

## Raw evidence files

- [Controlled J2534VCI detections](../evidence/controlled_j2534vci_detection.txt)
- [Controlled key-fob detections](../evidence/controlled_keyfob_detection.txt)
- [Controlled simultaneous two-class detections](../evidence/controlled_two_class_detection.txt)
- [Key-fob distance serial log](../evidence/keyfob_distance_serial_log.txt)
- [Key-fob distance CSV dataset](../data/keyfob_distance_results.csv)

## Interpretation notes

- [Technical discoveries](technical-discoveries.md)
- [Experiment notebook](experiment-notebook.md)
- [Results](results.md)
- [Troubleshooting](troubleshooting.md)
- [Model limitations](model-limitations.md)
