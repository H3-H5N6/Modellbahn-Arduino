/**
 * Ansteuerung eines Ausfahrtsignal mit den Signalbegriffen Hp 0, Hp 1, Hp 2 und Hp 0 + Sh1.
 * Eine Beschreibung der Signalbilder kann man z.B. unter
 *   https://de.wikipedia.org/wiki/H/V-Signalsystem
 * finden.
 *
 * Die LED werden direkt an den Arduino über eine Vorwiderstand angeschlossen.
 * Die Anode der LEDs werden an +V angeschlossen.
 *
 * Die Makros LED_ON und LED_OFF dienen der besseren Lesbarkeit.
 * Eine 0 schaltet die LED an und eine 1 die LED aus.
 *
 * Getest wurde die Software auf einem Arduino Nano 3.1
 */
#include <Arduino.h>

#define BUTTON_HP0_PIN 3
#define BUTTON_HP1_PIN 4
#define BUTTON_HP2_PIN 5
#define BUTTON_HP0_SH1_PIN 6

#define LED_RED_1_PIN 7
#define LED_RED_2_PIN 8
#define LED_YELLOW_PIN 9
#define LED_GREEN_PIN 10
#define LED_WHITE_PIN 11

#define LED_ON 0
#define LED_OFF 1

void hp0();

void initSerialForDebug() {
	Serial.begin(115200);
	Serial.println("Setup");
}

void initInput() {
	pinMode(BUTTON_HP0_PIN, INPUT_PULLUP);
	pinMode(BUTTON_HP1_PIN, INPUT_PULLUP);
	pinMode(BUTTON_HP2_PIN, INPUT_PULLUP);
	pinMode(BUTTON_HP0_SH1_PIN, INPUT_PULLUP);
}

void initOutput() {
	pinMode(LED_RED_1_PIN, OUTPUT);
	pinMode(LED_RED_2_PIN, OUTPUT);
	pinMode(LED_YELLOW_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(LED_WHITE_PIN, OUTPUT);
}

void setup() {
	initSerialForDebug();
	initInput();
	initOutput();

	hp0();
}

void blank() {
	digitalWrite(LED_RED_1_PIN, LED_OFF);
	digitalWrite(LED_RED_2_PIN, LED_OFF);
	digitalWrite(LED_YELLOW_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_WHITE_PIN, LED_OFF);
}

void hp0() {
	blank();
	digitalWrite(LED_RED_1_PIN, LED_ON);
	digitalWrite(LED_RED_2_PIN, LED_ON);
}

void hp1() {
	blank();
	digitalWrite(LED_GREEN_PIN, LED_ON);
}
void hp2() {
	blank();
	digitalWrite(LED_GREEN_PIN, LED_ON);
	digitalWrite(LED_YELLOW_PIN, LED_ON);
}
void hp0_sh1() {
	blank();
	digitalWrite(LED_RED_2_PIN, LED_ON);
	digitalWrite(LED_WHITE_PIN, LED_ON);
}

boolean keyIsPressed(uint8_t pin) {
	int input = digitalRead(pin);
	if (input == 0) {
		return true;
	}
	return false;
}

void loop() {
	boolean _hp0 = keyIsPressed(BUTTON_HP0_PIN);
	boolean _hp1 = keyIsPressed(BUTTON_HP1_PIN);
	boolean _hp2 = keyIsPressed(BUTTON_HP2_PIN);
	boolean _hp0_sh1 = keyIsPressed(BUTTON_HP0_SH1_PIN);

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
