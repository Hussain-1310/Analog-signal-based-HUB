#include <WiFi.h>
#include <PubSubClient.h>

#define POT 14   // Use GPIO34 in Wokwi for analog

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// Filter function
int filterdata() {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(POT);
    delay(2);
  }
  return sum / 5;
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_HUB")) {
      Serial.println("HUB Connected");
    } else {
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(POT, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int value = filterdata();

  String msg = String(value);
  client.publish("esp32/wheelchair", msg.c_str());
  client.publish("esp32/iot",msg.c_str());

  Serial.println("Sent: " + msg);

  delay(100);
}