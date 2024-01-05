#include <esp_now.h>
#include <WiFi.h>

#define PULSE_SENSOR_PIN 34
#define CHANNEL 1
#define DATA_SIZE 4

uint8_t macPenerima[] = { 0xB4, 0x8A, 0x0A, 0x46, 0x57, 0x00 }; 
uint8_t macSender2[] = { 0x40, 0x22, 0xD8, 0xE8, 0x72, 0xA0 };
uint8_t data[DATA_SIZE] = { 0x01, 0x02, 0x03, 0x04 };  
bool data_diterima = false;
bool data_terkirim = false;
bool esp32modemsleep = false;

void OnDataSent(const uint8_t *macPenerima, esp_now_send_status_t status) {
  Serial.print("Packet sent to: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(macPenerima[i], HEX);
    if (i < 5) Serial.print(':');
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(PULSE_SENSOR_PIN, INPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, macPenerima, sizeof(macPenerima));
  peerInfo.channel = CHANNEL;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer 1");
    return;
  } 
  WiFi.setSleep(true);
  WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
}

void loop() {
  int sensorValue = analogRead(PULSE_SENSOR_PIN);
  int bpm = 0.024 * sensorValue;
  //esp_now_send(macSender2, (uint8_t *)&bpm, sizeof(bpm)) != ESP_OK;
  if (bpm == 42 || bpm == 43 || bpm < 42 || bpm == 44) {
    Serial.print("\nESP32 masih dalam mode Modem sleep");
    WiFi.setSleep(true);
    WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
  } else {
    if (esp_now_send(macPenerima, (uint8_t *)&bpm, sizeof(bpm)) != ESP_OK) {
      Serial.println("Failed to send data");
    } else {
      Serial.println("\nData sent successfully");
    }
    delay(3000);
  }
}
