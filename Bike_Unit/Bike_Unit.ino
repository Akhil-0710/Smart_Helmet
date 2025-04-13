#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"  // <-- Add this line


#define RELAY_PIN 27
#define LED_DRUNK_PIN 25
#define LED_OK_PIN 26

const char* ssid = "Akhi_Moto";
const char* password = "123456789";

struct HelmetData {
  bool helmetOn;
  bool alcoholSafe;
};

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi (same as sender's hotspot)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to hotspot");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Hotspot");

  // Print the Wi-Fi channel (to ensure match with sender)
  wifi_second_chan_t sc;
  uint8_t channel;
  esp_wifi_get_channel(&channel, &sc);
  Serial.print("Wi-Fi Channel: ");
  Serial.println(channel);

  // Initialize ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);

  // Setup pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_DRUNK_PIN, OUTPUT);
  pinMode(LED_OK_PIN, OUTPUT);

  // Default states
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_DRUNK_PIN, LOW);
  digitalWrite(LED_OK_PIN, LOW);
}

void loop() {
  // Nothing here, all handled in callback
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  HelmetData data;
  memcpy(&data, incomingData, sizeof(data));

  Serial.println("Data Received from Helmet:");
  Serial.print("Helmet On: ");
  Serial.println(data.helmetOn);
  Serial.print("Alcohol Safe: ");
  Serial.println(data.alcoholSafe);

  if (data.helmetOn && data.alcoholSafe) {
    digitalWrite(RELAY_PIN, HIGH);           // Enable ignition
    digitalWrite(LED_OK_PIN, HIGH);          // Sober = Green LED
    digitalWrite(LED_DRUNK_PIN, LOW);
    Serial.println("Ignition Allowed");
  } else {
    digitalWrite(RELAY_PIN, LOW);            // Disable ignition
    digitalWrite(LED_OK_PIN, LOW);
    digitalWrite(LED_DRUNK_PIN, HIGH);       // Drunk or Helmet off = Red LED
    Serial.println("Ignition Blocked");
  }
}