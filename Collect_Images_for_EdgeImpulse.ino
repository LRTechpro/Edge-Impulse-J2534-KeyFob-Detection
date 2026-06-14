/**
 * Collect images for Edge Impulse object detection
 * MS579 Computer Vision — Assignment 4.1: Collecting and Labeling Data
 *
 * Hardware: ESP32-CAM (AI Thinker module, OV2640 camera)
 * Library:  EloquentEsp32cam (eloquent_esp32cam)
 *
 * Setup:
 *   1. Install the Espressif ESP32 board package in Arduino IDE.
 *   2. Install EloquentEsp32cam via the Arduino Library Manager.
 *   3. Set Tools > Board = "AI Thinker ESP32-CAM"
 *   4. Set Tools > Core Debug Level = "Info" (enables debug messages)
 *   5. Replace YOUR_WIFI_SSID and YOUR_WIFI_PASSWORD below.
 *   6. Flash to the ESP32-CAM using an FTDI programmer.
 *   7. Open Serial Monitor at 115200 baud to get the server URL.
 *   8. Navigate to the URL in a browser to begin capturing images.
 *
 * SECURITY NOTE:
 *   Never commit real Wi-Fi credentials to a public repository.
 *   Replace the placeholders below with your actual SSID and password,
 *   but keep the file with real credentials local only.
 */

// Wi-Fi credentials — replace before flashing
#define WIFI_SSID  "YOUR_WIFI_SSID"
#define WIFI_PASS  "YOUR_WIFI_PASSWORD"

// mDNS hostname — access the camera at http://AI_THINKER.local
#define HOSTNAME   "AI_THINKER"

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>

using eloq::camera;
using eloq::wifi;
using eloq::viz::collectionServer;


void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("___IMAGE COLLECTION SERVER___");

    // -----------------------------------------------------------------
    // Camera configuration
    // -----------------------------------------------------------------

    // AI Thinker pinout — maps GPIO lines for this specific module
    camera.pinout.aithinker();

    // Disable brownout detector to prevent resets under USB power
    camera.brownout.disable();

    // Face resolution = 240×240 pixels (square, required for FOMO input)
    // Edge Impulse FOMO expects a square image; this avoids distortion
    // when downsampling to the 48×48 model input size.
    camera.resolution.face();

    // High JPEG quality — preserves edge detail for bounding-box annotation
    camera.quality.high();

    // -----------------------------------------------------------------
    // Initialize camera
    // -----------------------------------------------------------------
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());

    // -----------------------------------------------------------------
    // Connect to Wi-Fi
    // -----------------------------------------------------------------
    while (!wifi.connect().isOk())
        Serial.println(wifi.exception.toString());

    // -----------------------------------------------------------------
    // Start HTTP image collection server
    // The server runs in a dedicated FreeRTOS task; the main loop is idle.
    // -----------------------------------------------------------------
    while (!collectionServer.begin().isOk())
        Serial.println(collectionServer.exception.toString());

    // Print status to Serial Monitor
    Serial.println("Camera OK");
    Serial.println("WiFi OK");
    Serial.println("Image Collection Server OK");
    Serial.println(collectionServer.address());
    // Open the printed URL in a browser to access the capture interface.
}


void loop() {
    // HTTP server runs in a separate FreeRTOS task.
    // No action needed in the main loop — intentionally left empty.
    //
    // Capture workflow (via browser):
    //   1. Navigate to the URL printed in setup().
    //   2. Position the target object in front of the camera.
    //   3. Click "Capture" to save the current frame.
    //   4. Reposition and repeat for each training image.
    //
    // Labeling workflow (Edge Impulse):
    //   5. Upload images to Edge Impulse (Data Acquisition → Upload).
    //   6. Draw tight bounding boxes using the Labeling Queue.
    //   7. Use exact label strings: "keyfob" and "J2534VCI"
}
