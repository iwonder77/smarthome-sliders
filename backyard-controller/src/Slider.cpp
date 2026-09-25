#include "Slider.h"
#include "Config.h"

void Slider::init() {
  for (int i = 0; i < config::NUM_REEDS_PER_SLIDER; i++) {
    reeds_[i].init();
  }
}

void Slider::update(uint32_t now) {
  for (int i = 0; i < config::NUM_REEDS_PER_SLIDER; i++) {
    reeds_[i].update(now);
  }
}

SliderReading Slider::reading() const {
  uint8_t active_mask = activeReedMask();
  switch (active_mask) {
  case 0b000:
    return SliderReading::NONE_ACTIVE;
  case 0b001:
    return SliderReading::POSITION1;
  case 0b010:
    return SliderReading::POSITION2;
  case 0b100:
    return SliderReading::POSITION3;
  default:
    return SliderReading::MULTIPLE_ACTIVE;
  }
}

bool Slider::reedsJustChanged() const {
  for (int i = 0; i < config::NUM_REEDS_PER_SLIDER; i++) {
    if (reeds_[i].justChanged()) {
      return true;
    }
  }
  return false;
}

void Slider::logSlider() const {
  SliderReading current_reading = reading();
  uint8_t active_mask = activeReedMask();
  Serial.print("Slider ");
  Serial.print(id_);
  Serial.print(" reading: ");
  for (int i = 0; i < config::NUM_REEDS_PER_SLIDER; i++) {
    Serial.print((active_mask >> i) & 1);
  }
  Serial.print(" --> ");
  Serial.println(toString(current_reading));
}

uint8_t Slider::activeReedMask() const {
  uint8_t mask = 0b000;
  for (int i = 0; i < config::NUM_REEDS_PER_SLIDER; i++) {
    if (reeds_[i].isActive()) {
      // lsb corresponds to first reed
      mask |= (1 << i);
    }
  }
  return mask;
}

const char *toString(SliderReading reading) {
  switch (reading) {
  case SliderReading::NONE_ACTIVE:
    return "None Active";
  case SliderReading::POSITION1:
    return "Position 1";
  case SliderReading::POSITION2:
    return "Position 2";
  case SliderReading::POSITION3:
    return "Position 3";
  case SliderReading::MULTIPLE_ACTIVE:
    return "Multiple Active";
  }
  return "UNKNOWN";
}
