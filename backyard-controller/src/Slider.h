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
  // valid positions are values in the CMD_TABLE indexes, must stay 0..2 in
  // physical order
  POSITION1 = 0,
  POSITION2 = 1,
  POSITION3 = 2,
  // invalid readings: never used as an index in CMD_TABLE
  NONE_ACTIVE,
  MULTIPLE_ACTIVE
};

const char *toString(SliderReading reading);

class Slider {
public:
  explicit Slider(uint8_t id,
                  const uint8_t (&pins)[config::NUM_REEDS_PER_SLIDER])
      : id_(id),
        reeds_{ReedSwitch(pins[0]), ReedSwitch(pins[1]), ReedSwitch(pins[2])} {}

  void init();
  void update(uint32_t now);
  SliderReading reading() const;
  bool reedsJustChanged() const;
  void logSlider() const;

private:
  uint8_t activeReedMask() const;

  const uint8_t id_;
  ReedSwitch reeds_[config::NUM_REEDS_PER_SLIDER];
};
