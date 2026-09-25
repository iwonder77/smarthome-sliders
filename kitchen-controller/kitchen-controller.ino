#include "src/Config.h"
#include "src/Slider.h"

Slider slider1(1, config::SLIDER1_PINS);
Slider slider2(2, config::SLIDER2_PINS);

bool stable = false;
bool last_raw = false;
uint32_t last_button_press_timestamp = 0;

bool buttonPressed(uint32_t now) {
  bool raw = digitalRead(config::BUTTON_PIN) == LOW;

  if (raw != last_raw) {
    last_raw = raw;
    last_button_press_timestamp = now;
    return false;
  }

  if (raw == stable) {
    return false;
  };
  if (now - last_button_press_timestamp < config::BUTTON_DEBOUNCE_MS) {
    return false;
  }

  stable = raw;
  return stable;
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // USB CDC needs time to enumerate

  pinMode(config::BUTTON_PIN, INPUT);
  slider1.init();
  slider2.init();
}

void loop() {
  uint32_t now = millis();
  slider1.update(now);
  slider2.update(now);
  if (buttonPressed(now)) {
    slider1.logSlider();
    slider2.logSlider();
  }
}
