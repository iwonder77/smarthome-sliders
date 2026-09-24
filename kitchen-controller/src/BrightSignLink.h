#pragma once
/**
 * BrightSignLink.h
 *
 * Sends command strings to the BrightSign via UDP over the ESP32-S3-ETH's
 * onboard W5500 Ethernet chip. Owns W5500 bring-up, link monitoring, and
 * sending. All settings (pins, IPs, port, timing) come from Config.h.
 *
 * - init() does NOT wait for the link; update() picks up a late cable
 * - each command is sent config::UDP_SEND_REPEATS times; this is safe because
 *   the BrightSign ignores commands while a video is playing
 * - create exactly ONE instance: the Ethernet library is a global singleton
 */

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "Config.h"

class BrightSignLink {
public:
  /**
   * @brief resets the W5500, starts SPI, Ethernet (factory MAC, static IP), and
   * UDP; bounds retransmission time. Call once in setup().
   *
   * @return true if a W5500 was detected, false otherwise
   */
  bool init();

  /**
   * @brief polls link status every config::LINK_POLL_INTERVAL_MS and logs
   * up/down transitions. Call every loop() pass.
   *
   * @param now timestamp for this loop pass
   */
  void update(uint32_t now);

  /**
   * @brief sends the command config::UDP_SEND_REPEATS times, spaced by
   * config::UDP_REPEAT_GAP_MS. Blocks briefly (~tens of ms) by design.
   *
   * @param command null-terminated command string, e.g. config::CMD_CORRECT
   * @return true if at least one packet left the W5500; false if not ready
   * or every send failed. NOTE: UDP has no delivery confirmation.
   */
  bool sendCommand(const char *command);

  /**
   * @return true if the W5500 was detected AND the link is currently up
   */
  bool isReady() const;

  /** @return packets successfully handed off by the W5500 since boot */
  uint32_t packetsSent() const;

  /** @return packets that failed to send since boot */
  uint32_t sendFailures() const;

private:
  void resetW5500();                 // pulse the RST pin
  void readEthernetMac(uint8_t *mac); // factory MAC reserved for Ethernet

  EthernetUDP udp_;
  bool hardware_found_ = false; // result of the hardwareStatus() check in init()
  EthernetLinkStatus last_link_status_ = Unknown; // to detect link transitions
  uint32_t last_link_poll_ms_ = 0; // rate-limits SPI link polling
  uint32_t packets_sent_ = 0;      // diagnostics
  uint32_t send_failures_ = 0;     // diagnostics
};
