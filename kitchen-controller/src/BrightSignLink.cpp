#include <esp_mac.h>

#include "BrightSignLink.h"
#include "Config.h"

bool BrightSignLink::init() {
  SPI.begin(config::W5500_SCK, config::W5500_MISO, config::W5500_MOSI,
            config::W5500_CS);

  resetW5500();

  uint8_t mac[6];
  Ethernet.init(config::W5500_CS);
  if (!readEthernetMac(mac)) {
    return false;
  }
  Ethernet.begin(mac, config::LOCAL_IP);
  Ethernet.setRetransmissionTimeout(config::ETH_RETRANSMIT_TIMEOUT_MS);
  Ethernet.setRetransmissionCount(config::ETH_RETRANSMIT_COUNT);
  hardware_found_ = (Ethernet.hardwareStatus() == EthernetW5500);
  if (!hardware_found_) {
    Serial.println("[ETH] ERROR: W5500 not found");
    return false;
  }

  udp_.begin(config::UDP_PORT);

  Serial.print("[ETH] W5500 OK, IP ");
  Serial.print(config::LOCAL_IP.toString());
  Serial.println(", link pending");
  return true;
}

void BrightSignLink::update(uint32_t now) {
  if (now - last_link_poll_ms_ >= config::LINK_POLL_INTERVAL_MS) {
    last_link_poll_ms_ = now;
    EthernetLinkStatus status = Ethernet.linkStatus();
    if (last_link_status_ != status) {
      last_link_status_ = status;
      Serial.print("[ETH] link status:");
      switch (status) {
      case Unknown:
        Serial.println(" UNKNOWN");
        break;
      case LinkON:
        Serial.println(" UP");
        break;
      case LinkOFF:
        Serial.println(" DOWN");
        break;
      }
    }
  }
}

bool BrightSignLink::sendCommand(const char *command) {
  // nullptr = no command configured (e.g. CMD_INVALID_FEEDBACK before the
  // feedback video exists). Deliberate, not a network failure: no count, no log
  if (command == nullptr) {
    return false;
  }

  // counters are per PACKET, so at all times:
  //   packets_sent_ + send_failures_ == commands attempted * UDP_SEND_REPEATS
  if (!isReady()) {
    send_failures_ += config::UDP_SEND_REPEATS;
    Serial.printf("[ETH] not ready (no W5500 or link down), dropped '%s'\n",
                  command);
    return false;
  }

  const size_t length = strlen(command);
  uint8_t sent = 0;

  // blocks ~(UDP_SEND_REPEATS - 1) * UDP_REPEAT_GAP_MS by design: short,
  // bounded, and only right before the lockout. delay() yields to other tasks
  for (uint8_t i = 0; i < config::UDP_SEND_REPEATS; i++) {
    if (i > 0) {
      delay(
          config::UDP_REPEAT_GAP_MS); // gap between copies, not after the last
    }

    if (!udp_.beginPacket(config::BRIGHTSIGN_IP, config::UDP_PORT)) {
      continue; // could not start a packet: counted as failed below
    }
    const bool wrote_all =
        udp_.write(reinterpret_cast<const uint8_t *>(command), length) ==
        length;
    // always close a started packet. endPacket() success means the packet left
    // the W5500, NOT that the BrightSign received it (UDP has no delivery ack)
    if (udp_.endPacket() && wrote_all) {
      sent++;
    }
  }

  packets_sent_ += sent;
  send_failures_ += config::UDP_SEND_REPEATS - sent;

  Serial.printf("[ETH] sent '%s' %u/%u\n", command, unsigned(sent),
                unsigned(config::UDP_SEND_REPEATS));
  return sent > 0;
}

bool BrightSignLink::isReady() const {
  return hardware_found_ && last_link_status_ == LinkON;
}

void BrightSignLink::resetW5500() {
  pinMode(config::W5500_RST, OUTPUT);
  digitalWrite(config::W5500_RST, LOW);
  delay(100);
  digitalWrite(config::W5500_RST, HIGH);
  delay(200);
}

bool BrightSignLink::readEthernetMac(uint8_t (&mac)[6]) {
  if (esp_read_mac(mac, ESP_MAC_ETH) != ESP_OK) {
    Serial.println("[ETH] ERROR: could not read factory MAC");
    return false;
  }
  Serial.printf("[ETH] MAC %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1],
                mac[2], mac[3], mac[4], mac[5]);
  return true;
}
