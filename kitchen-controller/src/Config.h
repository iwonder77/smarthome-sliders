#pragma once
/**
 * Config.h
 *
 * Centralized configuration constants used by the sketch.
 * - make sure to `#include "Config.h"` then use config::XXXXX
 * - units are encoded in the name (e.g. _MS, _US)
 */

#include "IPAddress.h"
#include <Arduino.h>

namespace config {
// ===== BUTTON CONFIG =====
constexpr uint8_t BUTTON_PIN = 17;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 20;

// ===== REED & SLIDERS CONFIG =====
constexpr uint8_t NUM_SLIDERS = 2;
constexpr uint8_t NUM_REEDS_PER_SLIDER = 3;
constexpr uint8_t SLIDER1_PINS[NUM_REEDS_PER_SLIDER] = {42, 47, 48};
constexpr uint8_t SLIDER2_PINS[NUM_REEDS_PER_SLIDER] = {39, 40, 41};
constexpr uint32_t REED_DEBOUNCE_MS = 40;

// ===== ETHERNET HARDWARE CONFIG =====
// ESP32S3-ETH module wiki specifies the following pinout for onboard WZ5500 use
// wiki link: https://www.waveshare.com/wiki/ESP32-S3-ETH#ETH_DHCP
constexpr uint8_t W5500_CS = 14;    // Chip Select (CS)
constexpr uint8_t W5500_RST = 9;    // Reset (RST)
constexpr uint8_t W5500_INT = 10;   // Interrupt (INT) - optional (unused)
constexpr uint8_t W5500_MISO = 12;  // MISO
constexpr uint8_t W5500_MOSI = 11;  // MOSI
constexpr uint8_t W5500_SCK = 13;   // SPI Clock (SCK)
constexpr uint16_t UDP_PORT = 5000; // Port BrightSign will listen on
// this station's static IP (unique per station)
const IPAddress LOCAL_IP(192, 168, 50, 2);
// BrightSign IP (or Mac running `nc -ul 5000` for testing)
const IPAddress BRIGHTSIGN_IP(192, 168, 50, 10);
constexpr uint32_t LINK_POLL_INTERVAL_MS = 500;

// ===== COMMANDS TO SEND =====
constexpr const char *CMD_CORRECT = "bread_correct";
constexpr const char *CMD_INCORRECT1 = "bread_incorrect1";
constexpr const char *CMD_INCORRECT2 = "bread_incorrect2";
constexpr const char *CMD_INCORRECT3 = "bread_incorrect3";
constexpr const char *CMD_INVALID_FEEDBACK = nullptr;

// dimenstions of this table are positions/reeds per slider
constexpr const char *CMD_TABLE[NUM_REEDS_PER_SLIDER][NUM_REEDS_PER_SLIDER] = {
    // rows: slider 1 position, columns: slider 2 position
    // S2: P1               P2              P3
    {CMD_INCORRECT3, CMD_INCORRECT1, CMD_INCORRECT1}, // S1:  P1
    {CMD_INCORRECT3, CMD_CORRECT, CMD_INCORRECT2},    //      P2
    {CMD_INCORRECT3, CMD_INCORRECT2, CMD_INCORRECT2}, //      P3
};

constexpr uint8_t UDP_SEND_REPEATS = 3;    // BrightSign ignores repeats
constexpr uint32_t UDP_REPEAT_GAP_MS = 15; // spread repeat packets in time
constexpr uint16_t ETH_RETRANSMIT_TIMEOUT_MS =
    50; // bounds how long a failed send can block
constexpr uint8_t ETH_RETRANSMIT_COUNT = 2;

} // namespace config
