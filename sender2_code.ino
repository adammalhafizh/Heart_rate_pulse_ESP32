#include <esp_now.h>
#include <WiFi.h>

#define PULSE_SENSOR_PIN 34
#define CHANNEL 1
#define DATA_SIZE 4

uint8_t macPenerima1[] = { 0xB4, 0x8A, 0x0A, 0x46, 0x57, 0x00 };
//uint8_t macPenerima2[] = { 0xB4, 0x8A, 0x0A, 0x46, 0x36, 0x7C };  // Replace with the receiver's MAC address
uint8_t data[DATA_SIZE] = { 0x01, 0x02, 0x03, 0x04 };             // Replace with your data
char data_esp[50];
bool data_sudah_diterima = false;
typedef struct {
  char message[50];
} Data_diterima;

// void OnDataRecv(const uint8_t *macPenerima2, const uint8_t *incomingData, int len) {
//   Data_diterima data_diterima;
//   memcpy(&data_diterima, incomingData, sizeof(data_diterima));
//   Serial.println(data_diterima.message);
//   data_sudah_diterima = true;
// }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, macPenerima1, sizeof(macPenerima2));
  peerInfo.channel = CHANNEL;
  peerInfo.encrypt = false;

  // esp_now_peer_info_t peerInfo2;
  // memset(&peerInfo2, 0, sizeof(peerInfo2));
  // memcpy(peerInfo.peer_addr, macPenerima2, sizeof(macPenerima2));
  // peerInfo.channel = CHANNEL;
  // peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  //esp_now_register_recv_cb(OnDataRecv);
  // WiFi.setSleep(true);
  // WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
}

void loop() {
  int data_ESP1 = 5;
  if (data_sudah_diterima) {
    esp_now_send(macPenerima1, (uint8_t *)&data_ESP1, sizeof(data_ESP1)) != ESP_OK;
    //esp_now_send(macPenerima2, (uint8_t *)&data_ESP1, sizeof(data_ESP1)) != ESP_OK;
    return;
  }
  delay(2000);
}