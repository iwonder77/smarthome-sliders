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

  /**
   * @brief initialization function, called in setup() and configures
   * the pinMode
   */
  void init();

  /**
   * @brief monitors reed switch state and contains debounce logic
   *
   * @param now timestamp of when this function was called
   */
  void update(uint32_t now);

  /**
   * @brief getter for reed switch stable state
   *
   * @return stable_ which is 1 (active) or 0 (inactive)
   */
  bool isActive() const;

  /**
   * @brief getter for the diagnostic flag telling us if the reed state
   * flipped, must be called right after update() in the same loop pass
   *
   * @return just_changed_ which is 1 (just flipped state) or 0 (maintained
   * state)
   */
  bool justChanged() const;

private:
  const uint8_t input_pin_;
  bool readRaw() const;
  bool just_changed_ = false; // flag toggled when reed switch state transitions
  bool last_raw_ = false;     // the reading from the previous pass
  bool stable_ = false;       // the value we've committed to and reported
  uint32_t last_change_timestamp_ = 0; // when the pin started reading last_raw
};
