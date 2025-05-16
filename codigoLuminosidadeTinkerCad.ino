#include <LiquidCrystal_I2C.h>
#include <math.h>

#define COLUMNS 16
#define LINES 2
#define I2C_ADDRESS 0x27 

#define LDR_PIN 2

#define BUZZER_PIN 13

#define LED_GREEN_PIN 12
#define LED_YELLOW_PIN 11
#define LED_RED_PIN 10

#define LDR_ANALOG_PORT A0
#define HUMIDITY_ANALOG_PORT A1
#define TEMPERATURE_ANALOG_PORT A2

const float LOW_LUX = 88;
const float MEDIUM_LUX = 176;

const float LOW_TEMPERATURE = 10;
const float MEDIUM_TEMPERATURE = 15;

const float LOW_HUMIDITY = 50;
const float MEDIUM_HUMIDITY = 70;

const long buzzerIntervalDuration = 3000;
const long displayIntervalDuration = 5000;

const long applicationDelay = 1000;

bool changeOk = true;
bool isBuzzerActive = false;
long buzzerTime = 0;
long displayTime = 0;

int currentIndex = 0;

float* luxHistory;
float* temperatureHistory;
float* humidityHistory;

int historySize;

LiquidCrystal_I2C lcd(I2C_ADDRESS, COLUMNS, LINES);

void setup() {
	pinMode(LDR_PIN, INPUT);
	pinMode(HUMIDITY_ANALOG_PORT, INPUT);

	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(LED_YELLOW_PIN, OUTPUT);
	pinMode(LED_RED_PIN, OUTPUT);

	historySize = (displayIntervalDuration/applicationDelay);

	luxHistory = (float*)malloc(historySize * sizeof(float));
	temperatureHistory = (float*)malloc(historySize * sizeof(float));
	humidityHistory = (float*)malloc(historySize * sizeof(float));
	
	Serial.begin(9600);

	if (!luxHistory || !temperatureHistory || !humidityHistory) {
		Serial.print("Error allocating memory.");
		while (true);
	}
	
	lcdInit();
}

void loop() {
    unsigned long currentTime = millis();

	Serial.println("==================");
    float lux = getLuxValue();
	long humidity = getHumidityPercentage();
	double temperature = getTemperatureCelsius();
	Serial.println("==================");

	bool isLuxOk = (lux < LOW_LUX);
	bool isHumidityOk = (humidity >= LOW_HUMIDITY && humidity <= MEDIUM_HUMIDITY);
	bool isTemperatureOk = (temperature >= LOW_TEMPERATURE && temperature <= MEDIUM_TEMPERATURE);

    if ( (isLuxOk) && (isHumidityOk) && (isTemperatureOk) ) {
        turnLedPins(HIGH, LOW, LOW); // Verde
        deactivateBuzzer();
    }
    else {
		bool isLuxMedium = (lux < MEDIUM_LUX && lux > LOW_LUX);
		bool isLuxAlert = (!isLuxOk && !isLuxMedium);

		if (isLuxAlert || !isHumidityOk) {
			turnLedPins(LOW, LOW, HIGH); // Vermelho
		}
		else if (isLuxMedium || !isTemperatureOk) {
			turnLedPins(LOW, HIGH, LOW); // Amarelo
		}
		
		if (isLuxMedium) {
			deactivateBuzzer();
		}
		else {
			toggleBuzzerByInterval(currentTime);
        }
    }

	luxHistory[currentIndex] = lux;
	humidityHistory[currentIndex] = humidity;
	temperatureHistory[currentIndex] = temperature;

	currentIndex = (currentIndex + 1);

	if ( (currentTime - displayTime >= displayIntervalDuration) ) {
		lcdPrintStatus();
		displayTime = currentTime;
	}
  
    delay(applicationDelay);
}

float avarage(float* array) {
	float sum = 0;
	for (int i = 0; i < historySize; i++) {
		sum += array[i];
	}
	return (sum / historySize);
}

void lcdPrintStatus() {
	float luxAvarage = avarage(luxHistory);
	float humidityAvarage = avarage(humidityHistory);
	float temperatureAvarage = avarage(temperatureHistory);

	Serial.println("*******************");
	Serial.print("Lux Avarage: ");
	Serial.println(luxAvarage, 0);
	Serial.print("Humidity Avarage: ");
	Serial.println(humidityAvarage, 0);
	Serial.print("Temperature Avarage: ");
	Serial.println(temperatureAvarage, 1);
	Serial.println("*******************");

	bool isLuxAvarageHigh = (luxAvarage > MEDIUM_LUX);
	bool isHumidityAvarageHigh = (humidityAvarage > MEDIUM_HUMIDITY);
	bool isTemperatureAvarageHigh = (temperatureAvarage > MEDIUM_TEMPERATURE);

	bool isLuxAvarageMedium = (luxAvarage < MEDIUM_LUX && luxAvarage >= LOW_LUX);
	bool isHumidityAvarageLow = (humidityAvarage < LOW_HUMIDITY);
	bool isTemperatureAvarageLow = (temperatureAvarage < LOW_TEMPERATURE);

	bool isHumidityOk = (!isHumidityAvarageLow && !isHumidityAvarageHigh) && (changeOk);
	bool isTemperatureOk = (!isTemperatureAvarageLow && !isTemperatureAvarageHigh);

	lcd.clear();
	if (isLuxAvarageHigh) {
		lcdPrintText("Ambiente muito", 0 ,0);
		lcdPrintText("CLARO", 0, 1);
	}
	else if (isHumidityAvarageHigh) {
		lcdPrintStatusText("Umidade ALTA", "Umidade = ", humidityAvarage, 0, "%");
	}
	else if (isTemperatureAvarageHigh) {
		lcdPrintStatusWithIcon("Temp. ALTA", "Temp. = ", temperatureAvarage, 1, 15);
	}
	else if (isLuxAvarageMedium) {
		lcdPrintText("Ambiente a meia", 0 ,0);
		lcdPrintText("luz", 0, 1);
	}
	else if (isHumidityAvarageLow) {
		lcdPrintStatusText("Umidade BAIXA", "Umidade = ", humidityAvarage, 0, "%");
	}
	else if (isTemperatureAvarageLow) {
		lcdPrintStatusWithIcon("Temp. BAIXA", "Temp. = ", temperatureAvarage, 1, 15);
	}
	else if (isHumidityOk) {
		lcdPrintStatusText("Umidade OK", "Umidade = ", humidityAvarage, 0, "%");
		changeOk = false;
	}
	else if (isTemperatureOk) {
		lcdPrintStatusWithIcon("Temp. OK", "Temp. = ", temperatureAvarage, 1, 15);
		changeOk = true;
	}
	else {
		lcdPrintText("Ambiente OK", 0 ,0);
		lcdPrintText("Normal", 0, 1);
	}

	currentIndex = 0;
}

void lcdPrintStatusText(const char* text, const char* prefix, float value, int decimalPlaces, const char* sufix) {
	lcd.clear();

	lcdPrintText(text, 0, 0);

	lcd.setCursor(0, 1);
	lcd.print(prefix);
	lcd.print(value, decimalPlaces);
	lcd.print(sufix);
}

void lcdPrintStatusWithIcon(const char* text, const char* prefix, float value, int decimalPlaces, int emote) {
	lcd.clear();

	lcdPrintText(text, 0, 0);

	lcd.setCursor(0, 1);
	lcd.print(prefix);
	lcd.print(value, decimalPlaces);
	lcdWriteIcon(emote);
}

void lcdPrintText(const char* text, int column, int line ) {
	lcd.setCursor(column, line);
	lcd.print(text);
}

void lcdWriteIcon(int emote) {
    lcd.write(byte(emote));
}

void lcdInit() {
	lcd.init();
	lcd.backlight();
	lcd.clear();

	byte shit[8] = {
		B00000,
		B10001,
		B00000,
		B00000,
		B01110,
		B10001,
		B00000,
	};

	byte smile[8] = {
		B00000,
		B10001,
		B00000,
		B00000,
		B10001,
		B01110,
		B00000,
	};

	byte serious[8] = {
		B00000,
		B10001,
		B00000,
		B00000,
		B11111,
		B00000,
		B00000,
	};

	byte wow[8] = {
		B00000,
		B10001,
		B00000,
		B01110,
		B10001,
		B10001,
		B01110,
	};

	byte lowLight[8] = {
		B00100,
		B01110,
		B00000,
		B00100,
		B01110,
		B00000,
		B00000,
		B00000,
	};

	byte mediumLight[8] = {
		B00100,
		B01110,
		B00000,
		B00100,
		B01110,
		B11111,
		B11111,
		B00000,
	};

	byte fullLight[8] = {
		B00100,
		B01110,
		B00000,
		B00100,
		B01110,
		B11111,
		B11111,
		B11111,
	};

	byte lowTemp[8] = {
		B00100,
		B00100,
		B00100,
		B00100,
		B00100,
		B00000,
		B00100,
		B00000
	};

	byte mediumTemp[8] = {
		B00100,
		B00100,
		B00100,
		B00100,
		B00100,
		B00000,
		B01110,
		B00000
	};

	byte highTemp[8] = {
		B00100,
		B00100,
		B00100,
		B00100,
		B00100,
		B00000,
		B11111,
		B00000
	};

	byte lowHumidity[8] = {
		B00100,
		B00100,
		B00000,
		B00000,
		B00000,
		B00000,
		B00100,
		B00000
	};

	byte mediumHumidity[8] = {
		B00100,
		B01110,
		B00100,
		B00000,
		B00000,
		B00000,
		B01110,
		B00000
	};

	byte highHumidity[8] = {
		B00100,
		B01110,
		B11111,
		B00000,
		B00000,
		B00000,
		B11111,
		B00000
	};

	byte celsiusOne[8] = {
		B01000,
		B10100,
		B01010,
		B00101,
		B00100,
		B00100,
		B00101,
		B00010
	};

	byte celsiusTwo[8] = {
		B10000,
		B00110,
		B01001,
		B01000,
		B01000,
		B01000,
		B01001,
		B00110
	};

	byte celsiusThree[8] = {
		B10000,
		B01110,
		B10001,
		B10000,
		B10000,
		B10001,
		B01110,
		B00000
	};

	lcd.createChar(0, shit);
	lcd.createChar(1, smile);
	lcd.createChar(3, serious);
	lcd.createChar(4, wow);

	lcd.createChar(5, lowLight);
	lcd.createChar(6, mediumLight);
	lcd.createChar(7, fullLight);

	lcd.createChar(8, lowTemp);
	lcd.createChar(9, mediumTemp);
	lcd.createChar(10, highTemp);

	lcd.createChar(11, lowHumidity);
	lcd.createChar(12, mediumHumidity);
	lcd.createChar(13, highHumidity);

	lcd.createChar(14, celsiusTwo);
	lcd.createChar(15, celsiusThree);
}

void turnLedPins(int green, int yellow, int red) {
    digitalWrite(LED_GREEN_PIN, green);
    digitalWrite(LED_YELLOW_PIN, yellow);
    digitalWrite(LED_RED_PIN, red);
}

void activateBuzzer() {
	digitalWrite(BUZZER_PIN, HIGH);
	isBuzzerActive = true;
}

void deactivateBuzzer() {
	digitalWrite(BUZZER_PIN, LOW);
	isBuzzerActive = false;
}

void toggleBuzzerByInterval(unsigned long currentTime) {
	bool isBuzzerIntervalPassed = (currentTime - buzzerTime >= buzzerIntervalDuration);

	if (isBuzzerActive && isBuzzerIntervalPassed) {
		buzzerTime = currentTime;
		deactivateBuzzer();
	}
	else if (isBuzzerIntervalPassed) {
		buzzerTime = currentTime;
		activateBuzzer();
	}
}

float getLuxValue() {
    const float GAMMA = 0.7;
    const float RL10 = 50;
    float voltage = analogRead(LDR_ANALOG_PORT) / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

	Serial.print("Lux: ");
  	Serial.println(lux);

    return lux;
}

long getHumidityPercentage() {
  	int humiditySensorOutput = analogRead(HUMIDITY_ANALOG_PORT);
	long humidityPercentage = map(humiditySensorOutput, 0, 1023, 0, 100);

	Serial.print("Humidade: ");
  	Serial.println(humidityPercentage);

	return humidityPercentage;
}

double getTemperatureCelsius() {
	int rawValue = analogRead(TEMPERATURE_ANALOG_PORT);
    double voltage = (rawValue / 1023.0) * 5000.0;
    double tempCelsius = (voltage - 500) * 0.1;

	Serial.print("Temperatura: ");
  	Serial.println(tempCelsius);

  	return tempCelsius;
}