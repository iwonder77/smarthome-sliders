#include "src/Config.h"
#include "src/Slider.h"
#include "src/BrightSignLink.h"

BrightSignLink brightsign;

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

  brightsign.init();

  pinMode(config::BUTTON_PIN, INPUT);
  slider1.init();
  slider2.init();
}

void loop() {
  uint32_t now = millis();
  brightsign.update(now);
  slider1.update(now);
  slider2.update(now);
  if (buttonPressed(now)) {
    // first check readings of sliders
    SliderReading r1 = slider1.reading();
    SliderReading r2 = slider2.reading();
    if (r1 == SliderReading::NONE_ACTIVE || r2 == SliderReading::NONE_ACTIVE) {
      Serial.println("Could not construct valid position combination");
      return;
    }
    if (r1 == SliderReading::MULTIPLE_ACTIVE || r2 == SliderReading::MULTIPLE_ACTIVE) {
      Serial.println("Invalid position combination, more than one reed per slider active");
      return;
    }
    // else we have valid position readings, build command to send then send it
    brightsign.sendCommand(config::CMD_TABLE[static_cast<uint8_t>(r1)][static_cast<uint8_t>(r2)]);
    slider1.logSlider();
    slider2.logSlider();
  }
}
