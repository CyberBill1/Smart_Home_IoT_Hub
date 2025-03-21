# Smart Home IoT Hub with Multi-Protocol Integration

An ESP32-based IoT hub integrating MQTT (AWS IoT Core), Zigbee (via Zigbee2MQTT), and Bluetooth, with a Node.js web interface for control.

## Prerequisites
- ESP32 DevKit
- Zigbee dongle (e.g., CC2531 with Zigbee2MQTT)
- Bluetooth device (e.g., HC-05)
- Arduino IDE, `PubSubClient`, `BluetoothSerial`
- Node.js, `npm install express mqtt`
- AWS IoT Core setup (certs, endpoint)

## Setup
1. **ESP32**: Flash `esp32_code/esp32_code.ino` via Arduino IDE. Update WiFi/AWS details.
2. **Zigbee**: Install Zigbee2MQTT (`npm install zigbee2mqtt`), configure `config.yaml`, run `npm start`.
3. **Bluetooth**: Pair HC-05 (TX: GPIO 17, RX: GPIO 16).
4. **Web Interface**: In `web_interface/`, run `npm install && node server.js`.
5. **Test**: Open `http://localhost:3000`, toggle devices, check AWS IoT Core (`smarthome/data`).

## Demo
- Controls a Zigbee light and reads Bluetooth data via AWS.
- [Demo Video](demo.mp4) <!-- Add after testing -->
