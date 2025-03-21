#include <WiFi.h>
#include <PubSubClient.h>
#include <BluetoothSerial.h>

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// AWS IoT Core
const char* mqtt_server = "your-iot-endpoint.iot.region.amazonaws.com";
const int mqtt_port = 8883;
const char* mqtt_topic_pub = "smarthome/data";
const char* mqtt_topic_sub = "smarthome/commands";

// Certificates (replace with your AWS IoT Core certs)
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n...AmazonRootCA1.pem...\n-----END CERTIFICATE-----";
const char* client_cert = "-----BEGIN CERTIFICATE-----\n...your-cert.pem.crt...\n-----END CERTIFICATE-----";
const char* client_key = "-----BEGIN RSA PRIVATE KEY-----\n...your-private.pem.key...\n-----END RSA PRIVATE KEY-----";

WiFiClientSecure espClient;
PubSubClient client(espClient);
BluetoothSerial SerialBT;

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Received: " + message);
  if (message == "ON") digitalWrite(2, HIGH);  // Example: Turn on LED
  if (message == "OFF") digitalWrite(2, LOW);  // Example: Turn off LED
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");
    if (client.connect("SmartHomeHub")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Hub");  // Bluetooth name
  pinMode(2, OUTPUT);  // Example LED pin

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Configure MQTT with TLS
  espClient.setCACert(ca_cert);
  espClient.setCertificate(client_cert);
  espClient.setPrivateKey(client_key);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Bluetooth data (e.g., from a sensor)
  if (SerialBT.available()) {
    String bt_data = SerialBT.readString();
    client.publish(mqtt_topic_pub, ("BT: " + bt_data).c_str());
  }

  // Zigbee data (via Zigbee2MQTT bridge, assumed on local MQTT)
  // Publish example data every 10s
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 10000) {
    lastMsg = millis();
    String zigbee_data = "Zigbee: Temp=25C";  // Placeholder
    client.publish(mqtt_topic_pub, zigbee_data.c_str());
  }

  delay(100);
}
