# Smart_Helmet
This is my first IOT project. Project description in readme.

**Project Title: Smart Helmet for Rider Safety and Ignition Control**

**Project Description:**
The Smart Helmet project is an IoT-based safety system designed to enhance the protection and accountability of two-wheeler riders. This system integrates various sensors and communication modules to ensure the rider meets essential safety conditions before starting the vehicle. It also includes alert mechanisms in case of accidents or alcohol detection.

1. The helmet unit is equipped with an ESP32 microcontroller, which interfaces with the following components:
2. MQ3 Alcohol Sensor: Detects the presence of alcohol in the rider’s breath.
3. MPU6050 Accelerometer and Gyroscope: Monitors sudden movements or impacts to detect possible accidents.
4. Hall Effect Sensor: Verifies whether the helmet strap is securely fastened.

If the alcohol level is below a pre-defined threshold and the helmet strap is fastened, the ESP32 sends a boolean status to the bike-side ESP32 using the ESP-NOW communication protocol. Only when both safety conditions are satisfied, the bike ESP32 allows ignition by controlling a relay module.
In the event of a crash or alcohol detection above the safe threshold, the helmet ESP32 sends an HTTP request to a remote server, triggering an alert system. Additionally, the system uses MacroDroid on the mobile device to assist in emergency notifications and automation.
The Smart Helmet also supports bike indicators for left/right turns, managed via the ESP32 on the bike unit, which receives inputs wirelessly from the helmet.

**Key Features**
Helmet strap detection using a Hall sensor.
Alcohol level monitoring using an MQ3 sensor.
Accident detection with MPU6050.
ESP-NOW wireless communication between helmet and bike.
Ignition control via relay based on safety checks.
HTTP alerts are sent to a server in case of an emergency.
Mobile automation support via MacroDroid.
Indicator light control for rider signaling.

**Technologies Used:**
ESP32 microcontroller (on both helmet and bike side)
MQ3 Alcohol Sensor
MPU6050 IMU Sensor
Hall Effect Sensor
Relay Module
ESP-NOW Communication
HTTP Protocol for Web Alerts
MacroDroid Android App

