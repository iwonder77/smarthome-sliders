#pragma once

/**
 * DebouncedInput.h
 *
 * initializes GPIO pin to track reed switch state, debounces
 * reed switch, and exposes state of corresponding reed switch
 */
#include <Arduino.h>

class DebouncedInput {
public:
  explicit DebouncedInput(uint8_t pin) : input_pin_(pin) {}
  void init();
  bool readRaw() const;
  void update(uint32_t now);
  bool isActive() const;
  bool isSettled() const;

private:
  uint8_t input_pin_;
  bool last_raw_ = 0; // the reading from the previous pass
  bool stable_ = 0;   // the value we've committed to and reported
  uint32_t last_change_timestamp_ = 0; // when the pin started reading last_raw
};
