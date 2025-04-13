#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <esp_now.h>
#include <HTTPClient.h>

#define MQ3_PIN 32
#define HELMET_PIN 27
#define ACCIDENT_THRESHOLD 2.5

// Replace with the actual MAC address of your Bike ESP32
uint8_t bikeAddress[] = {0xF4, 0x65, 0x0B, 0x55, 0x8D, 0x64};  // Replace with your bike ESP32 MAC

const char* ssid = "Akhi_Moto";
const char* password = "123456789";
const char* emergencyURL = "http://192.168.116.73:8080/emergency";
const char* alcoholURL = "http://192.168.116.73:8080/alcohol";

MPU6050 mpu;

struct HelmetData {
  bool helmetOn;
  bool alcoholSafe;
};

void setup() {
  Serial.begin(115200);

  pinMode(MQ3_PIN, INPUT);
  pinMode(HELMET_PIN, INPUT_PULLUP);

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
  } else {
    Serial.println("MPU6050 connected.");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected to Mobile Hotspot");

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, bikeAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add ESP-NOW peer");
  } else {
    Serial.println("ESP-NOW peer added");
  }
}

void loop() {
  HelmetData data;
  //data.helmetOn = !digitalRead(HELMET_PIN);  // LOW = Strap worn
data.helmetOn = true;
  data.alcoholSafe = readMQ3();

  bool accident = checkAccident();

  esp_err_t result = esp_now_send(bikeAddress, (uint8_t*)&data, sizeof(data));
  Serial.print("Sending to Bike -> Helmet: ");
  Serial.print(data.helmetOn);
  Serial.print(", Alcohol Safe: ");
  Serial.print(data.alcoholSafe);
  Serial.print(", Status: ");
  Serial.println(result == ESP_OK ? "Success" : "Fail");

  if (accident) {
    sendEmergencyAlert();
  }

  if (!data.alcoholSafe) {
    sendAlcoholAlert();
  }

  delay(4000);
}

bool readMQ3() {
  int sensorValue = analogRead(MQ3_PIN);
  float voltage = sensorValue * (3.3 / 4095.0);
  Serial.print("MQ-3 Voltage: ");
  Serial.println(voltage);
  return voltage < 1.7;
}

bool checkAccident() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float acc = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2) + pow(az / 16384.0, 2));
  return acc > ACCIDENT_THRESHOLD;
}

void sendEmergencyAlert() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(emergencyURL);
    int code = http.GET();
    Serial.println(code > 0 ? "Emergency Alert Sent" : "Emergency Alert Failed");
    http.end();
  }
}

void sendAlcoholAlert() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(alcoholURL);
    int code = http.GET();
    Serial.println(code > 0 ? "Alcohol Alert Sent" : "Alcohol Alert Failed");
    http.end();
  }
}