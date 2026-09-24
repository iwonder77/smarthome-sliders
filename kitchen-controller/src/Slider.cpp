#include "Slider.h"

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
