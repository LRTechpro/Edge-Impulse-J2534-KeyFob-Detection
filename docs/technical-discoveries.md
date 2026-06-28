# Technical Discoveries

This document captures the engineering findings that emerged during deployment and testing, including unexpected behavior and the reasoning used to interpret it.

## 1. The camera was producing fresh frames

The capture buffer was 96 x 96 RGB565:

```text
96 x 96 x 2 bytes = 18,432 bytes
```

A lightweight checksum sampled every eighth byte across the framebuffer before each inference. The checksum changed from frame to frame and changed substantially when the scene changed. That ruled out the original suspicion that the model was repeatedly processing one frozen frame.

**Discovery:** repeated classifications were caused by model behavior and scene presentation, not stale camera data.

## 2. The startup framebuffer warning was nonfatal

The ESP32-CAM repeatedly printed:

```text
cam_hal: FB-SIZE: 0 != 18432
```

The warning was followed by `Camera OK`, changing frame checksums, and valid inference results.

**Discovery:** in this configuration, the warning did not indicate a failed camera initialization. It should be documented, but it was not the root cause of detection errors.

## 3. Moving the framebuffer to DRAM improved diagnostic confidence

The camera configuration was changed to:

```cpp
config.fb_location = CAMERA_FB_IN_DRAM;
```

The 18,432-byte frame fit in DRAM. This change simplified the stale-frame investigation by avoiding uncertainty around PSRAM allocation and access.

**Discovery:** the DRAM change did not alter model weights or inference logic. It isolated camera-memory behavior from model behavior.

## 4. Quantization caused only a modest model-performance reduction

Offline model testing produced:

| Model | Accuracy | Precision | Recall | F1 |
|---|---:|---:|---:|---:|
| Float32 | 79.49% | 0.90 | 0.84 | 0.87 |
| Int8 | 76.92% | 0.86 | 0.81 | 0.83 |

**Discovery:** the int8 model retained most of the float32 model's performance and was appropriate for embedded deployment.

## 5. Pose mattered more than expected

Early J2534VCI tests showed misses and frequent `keyfob` confusion. When the same object was repositioned to resemble successful training examples, the model produced 20 consecutive correct detections with confidence between 0.86 and 1.00.

**Discovery:** the model learned a relatively narrow visual representation. Orientation, scale, lighting, and frame position had a stronger effect than the nominal class identity alone.

## 6. Simultaneous detection worked, but FOMO produced duplicate cells

In a controlled two-object sequence:

- Frames 1510 through 1519 contained both classes in 8 of 10 frames.
- Frames 1642 through 1649 contained both classes in 7 of 8 frames.
- Frame 1519 reported both `keyfob` and `J2534VCI` at 0.97 confidence.

Some frames produced two neighboring cells for one physical object.

**Discovery:** raw FOMO object count is not always the same as physical object count. Spatial merging or temporal filtering is needed before using counts operationally.

## 7. A fixed FOMO cell is not a true bounding box

The key-fob distance experiment used 22 cm, 33 cm, and 44 cm. Detection remained strong, but every successful sample reported:

```text
width = 8
height = 8
area = 64
```

**Discovery:** the Eloquent FOMO output represented a grid-cell activation, not a continuous object boundary. Therefore, width, height, and area did not shrink with distance and could not support direct geometric ranging.

## 8. Confidence was more informative than reported area in the distance study

Average confidence changed from 0.939 at 22 cm to 0.911 at 33 cm and 0.908 at 44 cm, while reported area remained constant.

**Discovery:** within this limited range, confidence reflected some scene variation, but it was not monotonic enough to serve as a calibrated distance estimator.

## 9. The J2534VCI and key-fob classes had asymmetric generalization

The J2534VCI distance trials produced all misses in the first three runs and then mostly `keyfob` classifications in a later 44 cm run. The key fob, by contrast, produced 29 correct classifications in 30 distance samples.

**Discovery:** class-level performance was asymmetric. Dataset diversity and pose coverage should be evaluated separately for each class rather than relying only on aggregate accuracy.

## 10. The Arduino wrapper required type-specific fixes

The Eloquent `bbox_t` type could not be initialized with an empty brace initializer, and `bbox.label` was an Arduino `String`, not a C string.

The firmware was corrected by:

- storing the best detection in primitive variables,
- comparing `bbox.label` directly to the target label,
- using `.c_str()` only when passing a label to a C-style function,
- using a standard callback with `fomo.forEach()`.

**Discovery:** wrapper types must be checked directly rather than assumed from generic C++ examples.

## Engineering conclusion

The project successfully demonstrated low-cost embedded two-class object detection. The strongest technical result was not only that detection worked, but that the experiments clearly separated three different issues:

1. camera-frame freshness,
2. model generalization,
3. FOMO grid-output limitations.

That distinction prevented camera and firmware behavior from being incorrectly blamed for model and representation limits.
