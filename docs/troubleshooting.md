# Troubleshooting Notes

## Camera initialization warning

The serial stream displayed:

```text
cam_hal: FB-SIZE: 0 != 18432
```

The warning was nonfatal in this configuration because it was followed by `Camera OK`, changing checksums, and successful inference.

## Framebuffer location

The Eloquent camera configuration was adjusted to use:

```cpp
config.fb_location = CAMERA_FB_IN_DRAM;
```

A 96 x 96 RGB565 frame requires 18,432 bytes, which fit in DRAM. The change was used while investigating repeated-frame behavior.

## FQBN error

A `Missing FQBN` compile error means that Arduino IDE does not have a board profile selected. Select `AI Thinker ESP32-CAM` and the correct COM port before compiling.

## Bounding-box type errors

The Eloquent `bbox_t` type used Arduino `String` for labels and could not be initialized with an empty brace initializer. The sketch was corrected by storing the best detection in primitive variables, comparing the label as an Arduino `String`, using `.c_str()` only when a C string was required, and passing a standard callback to `fomo.forEach()`.

## Class confusion

Initial J2534VCI distance trials produced misses or `keyfob` labels. The firmware and camera were still working because checksums changed and inference completed. The issue was model generalization under that physical presentation, not a frozen frame or failed upload.
