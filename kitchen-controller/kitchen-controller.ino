#include "src/Config.h"
#include "src/ReedSwitch.h"

ReedSwitch reeds[config::NUM_REEDS] = {
  ReedSwitch(config::REED_PINS[0][0]),
  ReedSwitch(config::REED_PINS[0][1]),
  ReedSwitch(config::REED_PINS[0][2]),
  ReedSwitch(config::REED_PINS[1][0]),
  ReedSwitch(config::REED_PINS[1][1]),
  ReedSwitch(config::REED_PINS[1][2])
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
