#pragma once
/**
 * Config.h
 *
 * Centralized configuration constants used by the sketch.
 * - make sure to `#include "Config.h"` then use config::XXXXX
 * - units are encoded in the name (e.g. _MS, _US)
 */

#include <Arduino.h>

namespace config {
constexpr uint8_t BUTTON_PIN = 17;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 20;

constexpr uint8_t NUM_SLIDERS = 2;
constexpr uint8_t NUM_REEDS_PER_SLIDER = 3;
constexpr uint8_t NUM_REEDS = NUM_SLIDERS * NUM_REEDS_PER_SLIDER;
constexpr uint8_t REED_PINS[NUM_SLIDERS][NUM_REEDS_PER_SLIDER] = {{42, 47, 48},
                                                                  {39, 40, 41}};
constexpr uint32_t REED_DEBOUNCE_MS = 40;
} // namespace config
