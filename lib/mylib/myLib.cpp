#include "myLib.h"

const int LED_PIN = 2;

void toggleLed(bool state) {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}
