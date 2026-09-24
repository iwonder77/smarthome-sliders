#include "src/Config.h"
#include "src/Slider.h"

Slider slider1(1, config::SLIDER1_PINS);
Slider slider2(2, config::SLIDER2_PINS);

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
  if (slider1.reedsJustChanged()) {
    slider1.logSlider();
  }
  if (slider2.reedsJustChanged()) {
    slider2.logSlider();
  }
}
