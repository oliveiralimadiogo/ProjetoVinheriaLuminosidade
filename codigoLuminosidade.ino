#define LDR_PIN 2

#define BUZZER_PIN 13

#define LED_GREEN_PIN 12
#define LED_YELLOW_PIN 11
#define LED_RED_PIN 10

#define LDR_ANALOG_PORT A0

bool isBuzzerActive = false;
long buzzerTime = 0;

const float OK = 88;
const float WARNING = 176;

const long buzzerIntervalDuration = 3000;

void setup() {
  pinMode(LDR_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
    unsigned long currentTime = millis(); 
    float lux = getLuxValue(analogRead(LDR_ANALOG_PORT));

    Serial.println(lux);

    if (lux < OK) {
        turnPins(HIGH, LOW, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        isBuzzerActive = false;
    }
    else if (lux < WARNING) {
        turnPins(LOW, HIGH, LOW);

        if (isBuzzerActive && (currentTime - buzzerTime >= buzzerIntervalDuration)) {
            buzzerTime = currentTime;
            digitalWrite(BUZZER_PIN, LOW);
            isBuzzerActive = false;
        }
        else if (currentTime - buzzerTime >= buzzerIntervalDuration) {
            buzzerTime = currentTime;
            digitalWrite(BUZZER_PIN, HIGH);
            isBuzzerActive = true;
        }
    }
    else {
        turnPins(LOW, LOW, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
      isBuzzerActive = false;
    }

    delay(100);
}

void turnPins(int green, int yellow, int red) {
    digitalWrite(LED_GREEN_PIN, green);
    digitalWrite(LED_YELLOW_PIN, yellow);
    digitalWrite(LED_RED_PIN, red);
}

float getLuxValue(int analogValue) {
    const float GAMMA = 0.7;
    const float RL10 = 50;
    float voltage = analogValue / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    return lux;
}