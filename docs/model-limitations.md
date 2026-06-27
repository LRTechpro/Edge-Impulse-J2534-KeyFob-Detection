# Model Limitations

## Pose and presentation sensitivity

The model performed best when objects were presented at orientations similar to the training images. Changes in angle, apparent size, background, and frame position could cause misses or class confusion.

## FOMO grid quantization

FOMO is centroid based. In this deployment, the wrapper reported quantized grid-cell dimensions such as 8 x 8 or 8 x 16. These values should not be interpreted as precise physical object boundaries.

## Duplicate activations

One physical object can activate more than one neighboring FOMO grid cell. Raw object counts can therefore overstate the number of physical objects unless detections are merged spatially.

## Distance-estimation limitation

The key-fob distance experiment returned the same 8 x 8 size at 22 cm, 33 cm, and 44 cm. A useful geometric range estimator would require calibrated camera intrinsics, a true bounding-box or segmentation output, known object dimensions, temporal filtering, or a learned regression model.

## Dataset limitations

Future training should add more lighting conditions, wider pose and rotation coverage, multiple distances, cluttered workshop backgrounds, additional negative examples, and more examples near frame edges.
