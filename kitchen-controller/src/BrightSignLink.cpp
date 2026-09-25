#include <esp_mac.h>

#include "BrightSignLink.h"

bool BrightSignLink::init() {
  SPI.begin(config::W5500_SCK, config::W5500_MISO, config::W5500_MOSI,
            config::W5500_CS);

  pinMode(config::W5500_RST, OUTPUT);
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
    Serial.println("W5500 not found!");
    return false;
  } else {
    Serial.println("W5500 found!");
  }

  udp_.begin(config::UDP_PORT);

  Serial.print("[ETH] W5500 OK, IP ");
  Serial.print(config::LOCAL_IP.toString());
  Serial.println(", link pending");
  return true;
}

void BrightSignLink::update(uint32_t now) {
  if (now - last_link_poll_ms_ >= config::LINK_POLL_INTERVAL_MS) {
  }
}

void BrightSignLink::resetW5500() {
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
