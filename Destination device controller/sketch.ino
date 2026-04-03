#define POT_PIN 14
#define M1 21
#define M2 19

int pot_value;

void setup() {
  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);

  // New ESP32 PWM style
  ledcAttach(M1, 1000, 8);
  ledcAttach(M2, 1000, 8);
}

void loop() {
  pot_value = analogRead(POT_PIN);
  Serial.println(pot_value);

  int speed1 = 0;
  int speed2 = 0;

  if (pot_value > 2000) {

    if (pot_value < 2500) {
      speed1 = 50;
      speed2 = 50;
      Serial.println("Forward");

    } else if (pot_value < 3000) {
      speed1 = 200;
      speed2 = 0;
      Serial.println("Left");

    } else if (pot_value < 3500) {
      speed1 = 0;
      speed2 = 200;
      Serial.println("Right");

    } else {
      speed1 = 0;
      speed2 = 0;
      Serial.println("Stop");
    }
  }

  ledcWrite(M1, speed1);
  ledcWrite(M2, speed2);

  delay(5);
}