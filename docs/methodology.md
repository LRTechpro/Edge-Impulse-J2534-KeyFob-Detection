# Methodology

## Model deployment workflow

The project used a custom Edge Impulse FOMO model with two labels: `keyfob` and `J2534VCI`. The model was exported as an Arduino inferencing library and integrated with EloquentEsp32cam on an AI Thinker ESP32-CAM.

The camera captured 96 x 96 RGB565 frames. Edge Impulse processed those frames for a 48 x 48 model input. Inference output was printed over serial at 115200 baud.

## Live-frame validation

A rolling frame counter and a lightweight checksum were added before inference. The checksum sampled every eighth byte across the framebuffer. Changing checksum values were used to verify that the camera was providing new data rather than repeatedly processing a frozen frame.

## Controlled object validation

Three controlled modes were used:

1. J2534VCI only
2. Key fob only
3. Both objects in the same frame

The objects were repositioned until both classes were presented in orientations similar to the training data. Results were reviewed by label, confidence, coordinates, and grid-cell dimensions.

## Distance experiment

The key fob was centered at 22 cm, 33 cm, and 44 cm. Ten inference samples were recorded at each distance. The farthest distance was exactly twice the nearest distance.

For each sample, the sketch recorded distance, sample and frame number, detection status, object count, label, coordinates, reported width and height, calculated area, confidence, inference time, and checksum.
