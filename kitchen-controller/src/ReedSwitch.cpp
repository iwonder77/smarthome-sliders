#include "ReedSwitch.h"
#include "Config.h"

void ReedSwitch::init() {
  pinMode(input_pin_, INPUT);

  // seed from the real pin state so boot doesn't look like a transition
  last_raw_ = stable_ = readRaw();
  last_change_timestamp_ = millis();
}

void ReedSwitch::update(uint32_t now) {
  // reset every call, we will check if the reading has changed subsequently
  // and assign to true when it has
  just_changed_ = false;
  bool raw = readRaw();

  if (raw != last_raw_) {
    // new edge detected - record the new state and restart the clock
    last_raw_ = raw;
    last_change_timestamp_ = now;
    return;
  }
  if (last_raw_ == stable_)
    return; // nothing new to report
  if (now - last_change_timestamp_ < config::REED_DEBOUNCE_MS)
    return; // hasn't held long enough

  // held past the debounce window - promote this reed's stable state
  // and update the just_changed_flag
  stable_ = last_raw_;
  just_changed_ = true;
}

bool ReedSwitch::isActive() const { return stable_; }

bool ReedSwitch::justChanged() const { return just_changed_; }

// states are 1 (ACTIVE) and 0 (INACTIVE)
bool ReedSwitch::readRaw() const { return digitalRead(input_pin_) == LOW; }
