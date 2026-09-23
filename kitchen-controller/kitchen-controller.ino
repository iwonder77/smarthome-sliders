#include "src/Config.h"
#include "src/ReedSwitch.h"

ReedSwitch reeds[config::NUM_REEDS] = {
  ReedSwitch(config::SLIDER1_REED_INPUT_PINS[0]),
  ReedSwitch(config::SLIDER1_REED_INPUT_PINS[1]),
  ReedSwitch(config::SLIDER1_REED_INPUT_PINS[2]),
  ReedSwitch(config::SLIDER2_REED_INPUT_PINS[0]),
  ReedSwitch(config::SLIDER2_REED_INPUT_PINS[1]),
  ReedSwitch(config::SLIDER2_REED_INPUT_PINS[2]),
};

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // USB CDC needs time to enumerate

  for (int i = 0; i < config::NUM_REEDS; i++) reeds[i].init();
  Serial.println("READING REED SWITCHES");
}

void loop() {
  uint32_t now = millis();

  for (int i = 0; i < config::NUM_REEDS; i++) {
    reeds[i].update(now);
    if (reeds[i].isTriggered()) {
    }
  }
}
