# ESP32 Web-Based LED Controller

A simple yet robust project to control an LED connected to an ESP32 board via a web interface. The project serves a clean, modern web page from the ESP32's internal SPIFFS (SPI Flash File System) and provides simple API endpoints to turn an LED on and off.

## Features

- **Web Interface:** Control the LED from any device with a web browser on the same network.
- **Responsive UI:** The web interface is designed to be clean and responsive, working well on both desktop and mobile browsers.
- **SPIFFS File Serving:** Serves HTML, CSS, and JavaScript files directly from the ESP32's flash memory.
- **Simple API:** Easy-to-understand GET requests for controlling the LED (`/led/on`, `/led/off`).
- **Extensible:** The web interface includes a placeholder for a video stream, making it a great starting point for a camera project.

## Hardware Requirements

- An ESP32 development board (e.g., ESP32-DevKitC).
- An LED.
- A current-limiting resistor (e.g., 220Ω or 330Ω).
- Jumper wires and a breadboard.

## Software & Dependencies

This project is built using PlatformIO with the Arduino framework.

### Libraries
The project configuration in `platformio.ini` includes the following libraries:

- `ESPAsyncWebServer`: For handling HTTP requests asynchronously.
- `AsyncTCP`: A required dependency for `ESPAsyncWebServer` on the ESP32.
- `PubSubClient`: For MQTT communication (not used in the current version but available for future expansion).

> **Note:** The current implementation in `src/main.cpp` uses the standard synchronous `WebServer.h`. While this works well for this simple application, migrating to `ESPAsyncWebServer` is recommended for better performance, especially if you plan to implement the video streaming feature.

## Project Structure

```
esp32-project/
├── data/
│   ├── index.html       # Main web page (Not provided, but required)
│   ├── style.css        # Stylesheet for the web page
│   └── script.js        # JavaScript for button interaction (Not provided, but required)
├── include/
│   └── myLib.h          # Custom library for LED control functions
├── lib/
├── src/
│   └── main.cpp         # Main application code
└── platformio.ini       # Project configuration
```

## Setup and Installation

1.  **Clone the Repository:**
    ```bash
    git clone <your-repository-url>
    cd <your-repository-name>
    ```

2.  **Configure Wi-Fi Credentials:**
    Open `src/main.cpp` and update the following lines with your Wi-Fi network's SSID and password:
    ```cpp
    const char *ssid = "Your_WiFi_SSID";
    const char *password = "Your_WiFi_Password";
    ```

3.  **Define LED Pin:**
    Make sure your `myLib.h` (or `main.cpp` if you prefer) correctly defines the GPIO pin connected to your LED. For example:
    ```cpp
    // In myLib.h or at the top of main.cpp
    #define LED_PIN 2 // Use the onboard LED for many ESP32 boards
    ```

4.  **Upload Files to SPIFFS:**
    The web interface files (`index.html`, `style.css`, `script.js`) are stored in the `data` directory. You need to upload them to the ESP32's SPIFFS. In PlatformIO, you can do this by running the "Upload Filesystem Image" task.

5.  **Build and Upload:**
    Build the project and upload it to your ESP32 board using PlatformIO.

## How to Use

1.  **Power On:** Power up your ESP32 board.

2.  **Find the IP Address:** Open the Serial Monitor (baud rate `115200`). The ESP32 will print its IP address once it connects to your Wi-Fi network.
    ```
    Connecting to WiFi...
    Connected! IP address: 192.168.1.123
    HTTP server started
    ```

3.  **Access the Web Interface:**
    Open a web browser on a device connected to the same Wi-Fi network and navigate to the IP address you noted (e.g., `http://192.168.1.123`).

4.  **Control the LED:**
    You will see a simple web page with "ON" and "OFF" buttons. Click them to control the LED.

## Future Improvements

- **Implement Asynchronous Web Server:** Refactor `main.cpp` to use `ESPAsyncWebServer` for non-blocking request handling.
- **Add Video Streaming:** Integrate an ESP32-CAM or an external camera and stream the video to the `#videoStream` element on the web page.
- **Dynamic Wi-Fi Configuration:** Use a captive portal (WiFiManager) to configure Wi-Fi credentials without hardcoding them.
- **MQTT Integration:** Use the `PubSubClient` library to connect the device to an MQTT broker for IoT integration.