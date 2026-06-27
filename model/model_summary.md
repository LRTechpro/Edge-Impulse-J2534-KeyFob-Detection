# Model Summary

## Configuration

| Property | Value |
|---|---|
| Platform | Edge Impulse |
| Learning block | FOMO object detection |
| Input | 48 x 48 RGB |
| Classes | `J2534VCI`, `keyfob` |
| Deployment | Arduino inferencing library |
| Embedded target | ESP32-CAM |

## Offline model testing

| Model | Accuracy | Precision | Recall | F1 |
|---|---:|---:|---:|---:|
| Float32 | 79.49% | 0.90 | 0.84 | 0.87 |
| Quantized int8 | 76.92% | 0.86 | 0.81 | 0.83 |

The quantized model was selected for embedded deployment because it reduced resource requirements while retaining similar performance.

## Deployment behavior

The model successfully detected both trained classes independently and simultaneously in controlled scenes. Performance remained sensitive to pose, scale, lighting, object spacing, and frame position.
