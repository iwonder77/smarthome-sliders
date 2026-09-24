#pragma once
/**
 * Slider.h
 *
 * Holds ownership of its 3 reed switch objects and tracks their state,
 * returns the reading of the slider
 */

#include <Arduino.h>

#include "Config.h"
#include "ReedSwitch.h"

enum class SliderReading : uint8_t {
  POSITION1,
  POSITION2,
  POSITION3,
  NONE_ACTIVE,
  MULTIPLE_ACTIVE
};

class Slider {
public:
  explicit Slider(const uint8_t (&pins)[config::NUM_REEDS_PER_SLIDER])
      : reeds_{ReedSwitch(pins[0]), ReedSwitch(pins[1]), ReedSwitch(pins[2])} {}

  void init();
  void update(uint32_t now);
  SliderReading reading() const;
  bool reedsJustChanged() const;
  uint8_t activeReedMask() const;

private:
  ReedSwitch reeds_[config::NUM_REEDS_PER_SLIDER];
};
