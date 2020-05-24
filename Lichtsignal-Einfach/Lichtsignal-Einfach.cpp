#include <Arduino.h>

#define KEY_HP0 3
#define KEY_HP1 4
#define KEY_HP2 5
#define KEY_HP0_SH1 6

#define LED_RED_1 7
#define LED_RED_2 8
#define LED_YELLOW 9
#define LED_GREEN 10
#define LED_WHITE 11

#define ON 0
#define OFF 1

void hp0();

void initSerialForDebug() {
	Serial.begin(115200);
	Serial.println("Setup");
}

void initInput() {
	pinMode(KEY_HP0, INPUT_PULLUP);
	pinMode(KEY_HP1, INPUT_PULLUP);
	pinMode(KEY_HP2, INPUT_PULLUP);
	pinMode(KEY_HP0_SH1, INPUT_PULLUP);
}

void initOutput() {
	pinMode(LED_RED_1, OUTPUT);
	pinMode(LED_RED_2, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_WHITE, OUTPUT);

	digitalWrite(LED_RED_1, OFF);
	digitalWrite(LED_RED_2, OFF);
	digitalWrite(LED_YELLOW, OFF);
	digitalWrite(LED_GREEN, OFF);
	digitalWrite(LED_WHITE, OFF);
}

void setup() {
	initSerialForDebug();
	initInput();
	initOutput();
	hp0();
}

void off() {
	digitalWrite(LED_RED_1, OFF);
	digitalWrite(LED_RED_2, OFF);
	digitalWrite(LED_YELLOW, OFF);
	digitalWrite(LED_GREEN, OFF);
	digitalWrite(LED_WHITE, OFF);
}

void hp0() {
	off();
	digitalWrite(LED_RED_1, ON);
	digitalWrite(LED_RED_2, ON);
}

void hp1() {
	off();
	digitalWrite(LED_GREEN, ON);
}
void hp2() {
	off();
	digitalWrite(LED_GREEN, ON);
	digitalWrite(LED_YELLOW, ON);
}
void hp0_sh1() {
	off();
	digitalWrite(LED_RED_2, ON);
	digitalWrite(LED_WHITE, ON);
}

boolean keyIsPressed(uint8_t pin) {
	int input = digitalRead(pin);
	if (input == 0) {
		return true;
	}
	return false;
}

void loop() {
	boolean _hp0 = keyIsPressed(KEY_HP0);
	boolean _hp1 = keyIsPressed(KEY_HP1);
	boolean _hp2 = keyIsPressed(KEY_HP2);
	boolean _hp0_sh1 = keyIsPressed(KEY_HP0_SH1);

	if (_hp0) {
		hp0();
	}

	if (_hp1) {
		hp1();
	}

	if (_hp2) {
		hp2();
	}

	if (_hp0_sh1) {
		hp0_sh1();
	}
}
