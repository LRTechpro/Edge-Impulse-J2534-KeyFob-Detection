# Results

## Controlled single-object validation

The J2534VCI sequence produced 20 consecutive correct detections with confidence values ranging from 0.86 to 1.00. The key-fob sequence produced sustained correct detections, commonly between 0.85 and 0.98.

## Simultaneous-object validation

Two controlled simultaneous-object sequences were reviewed:

- Frames 1510 through 1519: both classes appeared together in 8 of 10 frames.
- Frames 1642 through 1649: both classes appeared together in 7 of 8 frames.

Frame 1519 reported:

- `keyfob`: 0.97
- `J2534VCI`: 0.97

Some frames showed duplicate FOMO grid activations around one physical object.

## Distance experiment summary

| Distance | Correct detections | Detection rate | Average confidence | Average width | Average height | Average area |
|---:|---:|---:|---:|---:|---:|---:|
| 22 cm | 9 | 90% | 0.939 | 8 | 8 | 64 |
| 33 cm | 10 | 100% | 0.911 | 8 | 8 | 64 |
| 44 cm | 10 | 100% | 0.908 | 8 | 8 | 64 |

Average inference time remained approximately 197 ms per frame.

## Interpretation

Detection reliability remained high across the three tested distances. Confidence changed modestly, but the reported dimensions stayed fixed at one 8 x 8 grid cell.

No linear, inverse-distance, or inverse-square relationship could be estimated from the reported FOMO cell size. The deployed output is appropriate for centroid localization and class presence, but not for direct calibrated range estimation.
