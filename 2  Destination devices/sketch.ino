#include <WiFi.h>
#include <PubSubClient.h>

#define LED1 21
#define LED2 19

#define RELAY1 42
#define RELAY2 41

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  int value = message.toInt();

  Serial.print("Received: ");
  Serial.println(value);

  // 🚗 Wheelchair control
  if (String(topic) == "esp32/wheelchair") {

    if (value < 2000) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      Serial.println("STOP");

    } else if (value < 2500) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      Serial.println("FORWARD");

    } else if (value < 3000) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      Serial.println("LEFT");

    } else {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      Serial.println("RIGHT");
    }
  }

  // 💡 IoT Device Control
  if (String(topic) == "esp32/iot") {

    if (value > 1500 && value < 2000) {
      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, LOW);
      Serial.println("Light ON");

    } else if (value > 1000 && value < 1500) {
      digitalWrite(RELAY2, HIGH);
      digitalWrite(RELAY1, LOW);
      Serial.println("Fan ON");

    } else {
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      Serial.println("All OFF");
    }
  }
}

void setup_wifi() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32_CONTROLLER")) {
      Serial.println("Connected");

      client.subscribe("esp32/wheelchair");
      client.subscribe("esp32/iot");

    } else {
      Serial.println("Retrying...");
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}