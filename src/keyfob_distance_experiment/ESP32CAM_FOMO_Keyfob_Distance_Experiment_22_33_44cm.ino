/**
 * ESP32-CAM FOMO Metric Distance Experiment
 *
 * Purpose:
 * Collect exactly 10 inference samples at each camera-to-object distance.
 * Enter a distance in centimeters in the Arduino Serial Monitor.
 *
 * Recommended commands:
 *   C22
 *   C33
 *   C44
 *
 * Use Newline as the Serial Monitor line ending.
 *
 * Target object: keyfob
 * Board: AI Thinker ESP32-CAM
 * Baud: 115200
 */

#include <HaroldLRWatkins-project-1_inferencing.h>
#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/edgeimpulse/fomo.h>
#include <cstring>

using eloq::camera;
using eloq::ei::fomo;

static const char *TARGET_LABEL = "keyfob";
static const uint16_t SAMPLES_PER_DISTANCE = 10;

static uint32_t frameCount = 0;
static uint16_t sampleCount = 0;
static float currentDistanceCm = 0.0f;
static bool collectionActive = false;

static bool targetFound = false;
static float bestTargetProbability = -1.0f;
static int bestTargetX = 0;
static int bestTargetY = 0;
static int bestTargetWidth = 0;
static int bestTargetHeight = 0;


/**
 * Inspect one FOMO bounding box and preserve the highest-confidence
 * keyfob detection for the current frame.
 */
void collectBestTarget(int index, bbox_t bbox) {
    (void)index;

    if (
        bbox.label == TARGET_LABEL &&
        (!targetFound || bbox.proba > bestTargetProbability)
    ) {
        targetFound = true;
        bestTargetProbability = bbox.proba;
        bestTargetX = bbox.x;
        bestTargetY = bbox.y;
        bestTargetWidth = bbox.width;
        bestTargetHeight = bbox.height;
    }
}


/**
 * Read a distance command from Serial Monitor.
 *
 * Accepted examples:
 *   C22
 *   C33
 *   C44
 *
 * A plain number such as 22 or 44 is also accepted.
 */
void readDistanceCommand() {
    if (!Serial.available())
        return;

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() == 0)
        return;

    if (
        command[0] == 'C' || command[0] == 'c' ||
        command[0] == 'D' || command[0] == 'd'
    ) {
        command.remove(0, 1);
    }

    float requestedDistance = command.toFloat();

    if (requestedDistance <= 0.0f) {
        Serial.println("INVALID_COMMAND");
        Serial.println("Enter C22, C33, or C44 and press Send.");
        return;
    }

    currentDistanceCm = requestedDistance;
    sampleCount = 0;
    collectionActive = true;

    Serial.printf(
        "COLLECTION_STARTED,distance_cm=%.1f,samples=%u,target=%s\n",
        currentDistanceCm,
        SAMPLES_PER_DISTANCE,
        TARGET_LABEL
    );
}


/**
 * Print one CSV-compatible result row.
 */
void printCsvRow(
    const char *status,
    const char *label,
    int objectCount,
    int x,
    int y,
    int width,
    int height,
    int area,
    float confidence,
    int inferenceMs,
    uint32_t checksum
) {
    Serial.printf(
        "CSV,%.1f,%u,%lu,%s,%d,%s,%d,%d,%d,%d,%d,%.2f,%d,0x%08lX\n",
        currentDistanceCm,
        sampleCount,
        (unsigned long)frameCount,
        status,
        objectCount,
        label,
        x,
        y,
        width,
        height,
        area,
        confidence,
        inferenceMs,
        (unsigned long)checksum
    );
}


void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.setTimeout(100);

    Serial.println("ESP32-CAM FOMO METRIC DISTANCE EXPERIMENT");
    Serial.printf("Project: %s\n", EI_CLASSIFIER_PROJECT_NAME);
    Serial.printf(
        "Model input: %d x %d\n",
        EI_CLASSIFIER_INPUT_WIDTH,
        EI_CLASSIFIER_INPUT_HEIGHT
    );
    Serial.printf("Target label: %s\n", TARGET_LABEL);
    Serial.printf("Samples per distance: %u\n", SAMPLES_PER_DISTANCE);

    camera.pinout.aithinker();
    camera.brownout.disable();
    camera.resolution.yolo();
    camera.pixformat.rgb565();

    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());

    Serial.println("Camera OK");
    Serial.println("Serial Monitor line ending: Newline");
    Serial.println("Place the key fob in the center of the frame.");
    Serial.println("Enter C22, C33, or C44 to start a 10-sample collection.");
    Serial.println(
        "CSV_HEADER,distance_cm,sample,frame,status,object_count,label,"
        "x,y,width,height,area,confidence,inference_ms,checksum"
    );
}


void loop() {
    readDistanceCommand();

    if (!collectionActive) {
        delay(50);
        return;
    }

    if (!camera.capture().isOk()) {
        Serial.println(camera.exception.toString());
        return;
    }

    frameCount++;
    sampleCount++;

    uint8_t *buf = camera.frame->buf;
    uint32_t len = (uint32_t)camera.frame->len;
    uint32_t checksum = 0;

    // Sample every eighth byte across the full framebuffer.
    for (uint32_t i = 0; i < len; i += 8)
        checksum += buf[i];

    if (!fomo.run().isOk()) {
        Serial.println(fomo.exception.toString());
        return;
    }

    int inferenceMs = fomo.benchmark.millis();
    int objectCount = fomo.count();

    // Reset the preserved target values before reviewing this frame.
    targetFound = false;
    bestTargetProbability = -1.0f;
    bestTargetX = 0;
    bestTargetY = 0;
    bestTargetWidth = 0;
    bestTargetHeight = 0;

    if (fomo.foundAnyObject()) {
        // Review every FOMO result and keep the highest-confidence target box.
        fomo.forEach(collectBestTarget);
    }

    if (targetFound) {
        int area = bestTargetWidth * bestTargetHeight;

        printCsvRow(
            "DETECTED",
            TARGET_LABEL,
            objectCount,
            bestTargetX,
            bestTargetY,
            bestTargetWidth,
            bestTargetHeight,
            area,
            bestTargetProbability,
            inferenceMs,
            checksum
        );
    }
    else if (fomo.foundAnyObject()) {
        int area = fomo.first.width * fomo.first.height;

        printCsvRow(
            "WRONG_LABEL",
            fomo.first.label.c_str(),
            objectCount,
            fomo.first.x,
            fomo.first.y,
            fomo.first.width,
            fomo.first.height,
            area,
            fomo.first.proba,
            inferenceMs,
            checksum
        );
    }
    else {
        printCsvRow(
            "MISSED",
            "NONE",
            0,
            0,
            0,
            0,
            0,
            0,
            0.0f,
            inferenceMs,
            checksum
        );
    }

    if (sampleCount >= SAMPLES_PER_DISTANCE) {
        collectionActive = false;
        Serial.printf(
            "COLLECTION_COMPLETE,distance_cm=%.1f,samples=%u\n",
            currentDistanceCm,
            sampleCount
        );
        Serial.println("Move the object to the next metric distance and enter the new C command.");
    }

    delay(300);
}
