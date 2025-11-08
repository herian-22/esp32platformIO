#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include "myLib.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

const char *ssid = "Her.boy";
const char *password = "Herian68";

// ----- Ganti dengan info MQTT CloudAMQP -----
const char* mqtt_server = "mustang.rmq.cloudamqp.com";
const int mqtt_port = 1883;       // 1883 = MQTT biasa, 8883 = TLS
const char* mqtt_user = "eckzpuce:eckzpuce";
const char* mqtt_pass = "8SJhurANUpT8d4MgZ5aPDCNZe4wIUPqo";

// ===== Inisialisasi Web Server =====
WebServer server(80);

// ===== Klien WiFi & MQTT =====
WiFiClient espClient;
PubSubClient client(espClient);

// ===== Definisikan Topik MQTT =====
const char* led_command_topic = "esp32/led/command";
const char* status_topic = "esp32/status";
long lastMsg = 0;
char msg[50];
long lastReconnectAttempt = 0; // <-- Variabel untuk reconnect non-blocking

// ===== Handler untuk file web (Tidak berubah) =====
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

// ===== Fungsi Callback MQTT (Tidak berubah) =====
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (strcmp(topic, led_command_topic) == 0) {
    if (message == "ON") {
      toggleLed(true);
      Serial.println("LED dihidupkan via MQTT");
    } else if (message == "OFF") {
      toggleLed(false);
      Serial.println("LED dimatikan via MQTT");
    }
  }
}

// ===== Fungsi Cek & Reconnect MQTT (Non-Blocking) =====
// <-- INI ADALAH FUNGSI PENGGANTI RECONNECT() YANG LAMA -->
void checkMqttConnection() {
  if (!client.connected()) {
    long now = millis();
    // Coba sambung ulang hanya setiap 5 detik
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;

      Serial.print("Attempting MQTT connection...");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);

      if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
        Serial.println("connected");
        client.publish(status_topic, "ESP32 Terhubung");
        client.subscribe(led_command_topic);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
      }
    }
  }
}


// ===== Setup =====
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting...");

  // Mount SPIFFS
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
  Serial.println(WiFi.localIP());

  // Setup MQTT Server dan Callback
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Web route
  server.on("/", handleRoot);
  server.on("/style.css", handleStyle);
  server.on("/script.js", handleScript);

  server.on("/led/on", HTTP_GET, []() {
    toggleLed(true);
    server.send(200, "text/plain", "LED ON");
  });

  server.on("/led/off", HTTP_GET, []() {
    toggleLed(false);
    server.send(200, "text/plain", "LED OFF");
  });

  // Handler untuk request yang tidak dikenal
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });

  // Jalankan server
  server.begin();
  Serial.println("HTTP server started");

  // Inisialisasi lastReconnectAttempt
  lastReconnectAttempt = 0;
}

// ===== Loop =====
void loop() {
  // Tangani klien web server
  server.handleClient();

  // Cek koneksi MQTT (TIDAK AKAN MEM-BLOK)
  checkMqttConnection(); 

  // Biarkan klien MQTT memproses pesan masuk/keluar
  // Penting: client.loop() HARUS tetap dipanggil walau tidak terhubung
  client.loop(); 

  // Contoh: Kirim status "uptime" (hanya jika terhubung)
  if (client.connected()) {
    long now = millis();
    if (now - lastMsg > 10000) {
      lastMsg = now;
      snprintf(msg, 50, "Uptime: %ld detik", millis() / 1000);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(status_topic, msg);
    }
  }
}