#include "src/Config.h"
#include "src/Slider.h"

Slider slider1(config::SLIDER1_PINS);
Slider slider2(config::SLIDER2_PINS);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // USB CDC needs time to enumerate

  slider1.init();
  slider2.init();
}

void loop() {
  uint32_t now = millis();
  slider1.update(now);
  slider2.update(now);
}
