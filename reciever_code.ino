#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1
#define DATA_SIZE 4

uint8_t macSender1[] = { 0x40, 0x22, 0xD8, 0xE8, 0x72, 0xA0 };
uint8_t macSender2[] = { 0xB4, 0x8A, 0x0A, 0x46, 0x36, 0x7C };

int iterationCount = 0; // Counter for iterations

void onDataReceived(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  if (memcmp(macAddr, macSender1, sizeof(macSender1)) == 0) {
    int data_ESP1 = *((int *)incomingData);
    Serial.printf("Menghitung denyut jantung...");
    Serial.printf("\n");
  } else if (memcmp(macAddr, macSender2, sizeof(macSender2)) == 0) {
    int bpm = *((int *)incomingData);
    Serial.printf("BPM: ");
    Serial.print(bpm);
    Serial.printf("\n");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peer1;
  memset(&peer1, 0, sizeof(peer1));
  memcpy(peer1.peer_addr, macSender1, sizeof(macSender1));
  peer1.channel = 0;
  peer1.encrypt = false;
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("Gagal menambahkan peer 1");
    return;
  }

  esp_now_peer_info_t peer2;
  memset(&peer2, 0, sizeof(peer2));
  memcpy(peer2.peer_addr, macSender2, sizeof(macSender2));
  peer2.channel = 0;
  peer2.encrypt = false;
  if (esp_now_add_peer(&peer2) != ESP_OK) {
    Serial.println("Gagal menambahkan peer 2");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  Serial.println("ESP32 siap menerima data dari ESP-NOW");
}

void loop() {
  
}