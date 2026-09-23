#pragma once

/**
 * ReedSwitch.h
 *
 * initializes GPIO pin to track reed switch state, debounces
 * reed switch, and exposes state of corresponding reed switch
 */
#include <Arduino.h>

class ReedSwitch {
public:
  explicit ReedSwitch(uint8_t pin) : input_pin_(pin) {}
  void init();
  void update(uint32_t now);
  bool isActive() const;
  bool justChanged() const;

private:
  const uint8_t input_pin_;
  bool readRaw() const;
  bool just_changed_ = false;
  bool last_raw_ = false; // the reading from the previous pass
  bool stable_ = false;   // the value we've committed to and reported
  uint32_t last_change_timestamp_ = 0; // when the pin started reading last_raw
};
