# Experiment Notebook

## Phase 1: Model validation

- Confirmed the Edge Impulse project contained two classes: `J2534VCI` and `keyfob`.
- Verified 48 x 48 model input and FOMO object detection.
- Compared float32 and int8 model-test results.
- Noted that failed J2534VCI examples were generally smaller, vertical, edge-positioned, or less visually similar to the successful training examples.

## Phase 2: ESP32-CAM deployment

- Compiled for AI Thinker ESP32-CAM using ESP32 core 3.3.10.
- Uploaded over COM4.
- Verified flash write, hash verification, and automatic RTS reset.
- Observed typical inference time near 197 ms per frame.

## Phase 3: Camera freshness investigation

**Question:** Was the ESP32-CAM repeatedly returning one stale frame?

**Action:** Added a frame counter, framebuffer address, length, dimensions, timestamp, and checksum before inference.

**Observation:** frame checksums changed continuously and changed when the scene changed.

**Conclusion:** the camera stream was live. Repeated labels were not evidence of a frozen framebuffer.

## Phase 4: Controlled single-object tests

### J2534VCI

- Repositioned the object to a favorable diagonal pose with the distinctive face visible.
- Preserved 20 consecutive correct detections.
- Confidence ranged from 0.86 to 1.00.

### Key fob

- Preserved a sustained sequence of correct detections.
- Confidence commonly remained above 0.90.

**Conclusion:** both classes were deployable, but performance depended strongly on presentation.

## Phase 5: Controlled simultaneous-object tests

- Separated the objects spatially.
- Positioned the J2534VCI and key fob in different regions of the frame.
- Preserved two simultaneous-detection sequences.

### Sequence A

- Frames 1510 through 1519
- Both classes present in 8 of 10 frames
- Frame 1519: both classes at 0.97 confidence

### Sequence B

- Frames 1642 through 1649
- Both classes present in 7 of 8 frames

**Observation:** neighboring FOMO cells sometimes activated around one physical object.

**Conclusion:** simultaneous detection worked, but raw object counts require spatial post-processing.

## Phase 6: Distance pilot with J2534VCI

### Distances

- 22 cm
- 33 cm
- 44 cm

### Initial result

- 30 of 30 frames missed the target.
- A later 44 cm run produced 9 `keyfob` classifications and 1 miss.

**Conclusion:** the camera and inference pipeline were active, but the J2534VCI presentation generalized poorly in the distance setup.

## Phase 7: Distance experiment with key fob

The target class was changed to `keyfob` while preserving the same 22 cm, 33 cm, and 44 cm distances.

### Results

| Distance | Correct | Total | Detection rate | Average confidence |
|---:|---:|---:|---:|---:|
| 22 cm | 9 | 10 | 90% | 0.939 |
| 33 cm | 10 | 10 | 100% | 0.911 |
| 44 cm | 10 | 10 | 100% | 0.908 |

All successful detections reported an 8 x 8 cell and area 64.

**Conclusion:** detection remained reliable, but grid-cell area could not be used as a continuous distance measurement.

## Phase 8: Current research direction

The next useful experiments are:

1. collect a larger multi-distance dataset,
2. add a true bounding-box model for comparison,
3. calibrate camera intrinsics,
4. merge adjacent FOMO activations,
5. add temporal smoothing,
6. repeat the distance study with controlled lighting and multiple object orientations.
