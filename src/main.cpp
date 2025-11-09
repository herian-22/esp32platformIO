#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include "myLib.h" // Library Anda untuk LED

// --- Info WiFi Anda ---
const char *ssid = "Her.boy";
const char *password = "Herian68";

WebServer server(80);


// ===== Handler untuk file web =====
void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleStyle() {
  File file = SPIFFS.open("/style.css", "r");
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "text/css");
  file.close();
}

void handleScript() {
  File file = SPIFFS.open("/script.js", "r");
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "application/javascript");
  file.close();
}


// ===== Setup =====
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting...");

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP()); // <-- CATAT IP INI NANTI


  // === WEB ROUTE (HANDLER) ===
  
  server.on("/", handleRoot);
  server.on("/style.css", handleStyle);
  server.on("/script.js", handleScript);

  server.on("/led/on", HTTP_GET, []() {
    toggleLed(true); // Panggil fungsi dari myLib
    server.send(200, "text/plain", "LED ON");
  });

  server.on("/led/off", HTTP_GET, []() {
    toggleLed(false); // Panggil fungsi dari myLib
    server.send(200, "text/plain", "LED OFF");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

// ===== Loop =====
void loop() {
  server.handleClient();
}