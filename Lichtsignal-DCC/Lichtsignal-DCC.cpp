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

#include <OneButton.h>
#include <NmraDcc.h>

NmraDcc Dcc;

#define DCC_INTERRUPT_NUMBER 0
#define DCC_IN_PIN           2
#define DCC_IN_ENABLE_PULLUP true
#define DCC_ACC_PIN          12

#define VERSION_ID 1

struct CVPair {
	uint16_t CV;
	uint8_t Value;
};

typedef enum {
	HP0, HP1, HP2, HP0_SH1, UNDEF
} SIGNAL_ASPECT;

SIGNAL_ASPECT signalAspect = HP0;

CVPair FactoryDefaultCVs[] = { { CV_ACCESSORY_DECODER_ADDRESS_LSB, 17 }, // 17 - 3
		{ CV_ACCESSORY_DECODER_ADDRESS_MSB, 3 }, //256 * MSB + LSB : LSB: 0..63, MSB  0 .. 7 ((785))
		};

uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);

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

OneButton hp0_button(BUTTON_HP0_PIN, true, true);
OneButton hp1_button(BUTTON_HP1_PIN, true, true);
OneButton hp2_button(BUTTON_HP2_PIN, true, true);
OneButton hp0_sh1_button(BUTTON_HP0_SH1_PIN, true, true);

void hp0();
void hp1();
void hp2();
void hp0_sh1();

void initSerialForDebug() {
	Serial.begin(115200);
	Serial.println("Setup");
}

void initInput() {
	hp0_button.attachClick(hp0);
	hp1_button.attachClick(hp1);
	hp2_button.attachClick(hp2);
	hp0_sh1_button.attachClick(hp0_sh1);
}

void initOutput() {
	pinMode(LED_RED_1_PIN, OUTPUT);
	pinMode(LED_RED_2_PIN, OUTPUT);
	pinMode(LED_YELLOW_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(LED_WHITE_PIN, OUTPUT);
}

void initDcc() {
	pinMode(DCC_ACC_PIN, OUTPUT);
	Dcc.pin(DCC_INTERRUPT_NUMBER, DCC_IN_PIN, DCC_IN_ENABLE_PULLUP);
	Dcc.init(MAN_ID_DIY, VERSION_ID, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

	if (FactoryDefaultCVIndex && Dcc.isSetCVReady()) {
		for (int i = 0; i < FactoryDefaultCVIndex; i++) {
			Serial.print("CV:");
			Serial.print(FactoryDefaultCVs[i].CV);
			Serial.print("=>");
			Serial.println(FactoryDefaultCVs[i].Value);

			Dcc.setCV(FactoryDefaultCVs[i].CV, FactoryDefaultCVs[i].Value);
		}
	}
}

void notifyCVAck(void) {
	Serial.println("notifyCVAck");

	digitalWrite(DCC_ACC_PIN, HIGH);
	delay(6);
	digitalWrite(DCC_ACC_PIN, LOW);
}

// This function is called whenever a normal DCC Turnout Packet is received and we're in Board Addressing Mode
void notifyDccAccTurnoutBoard(uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower) {
	Serial.print("notifyDccAccTurnoutBoard: ");
	Serial.print(BoardAddr, DEC);
	Serial.print(',');
	Serial.print(OutputPair, DEC);
	Serial.print(',');
	Serial.print(Direction, DEC);
	Serial.print(',');
	Serial.println(OutputPower, HEX);
}

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower) {

	uint16_t addr2 = Dcc.getAddr();
	Serial.print("Ddc.getAddr: ");
	Serial.println(addr2);
	Serial.print("notifyDccAccTurnoutOutput: ");
	Serial.print("Adresse: ");
	Serial.print(Addr, DEC);
	Serial.print(' ');
	if (Direction == 0) {
		Serial.print("Abzeig");
	} else {
		Serial.print("Gerade");
	}
	Serial.print(" [");
	Serial.print(OutputPower, HEX);
	Serial.print(']');
	Serial.println();

	if (Addr == 785) {
		if (Direction == 0) {
			signalAspect = HP0;
		}
		if (Direction == 1) {
			signalAspect = HP1;
		}
	}

	if (Addr == 786) {
		if (Direction == 0) {
			signalAspect = HP0_SH1;
		}
		if (Direction == 1) {
			signalAspect = HP2;
		}
	}

}

// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState(uint16_t Addr, uint8_t State) {
	Serial.print("notifyDccSigOutputState: ");
	Serial.print(Addr, DEC);
	Serial.print(',');
	Serial.println(State, HEX);
}

void setup() {
	initSerialForDebug();
	initInput();
	initOutput();

	initDcc();

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
	hp0_button.tick();
	hp1_button.tick();
	hp2_button.tick();
	hp0_sh1_button.tick();
	Dcc.process();

	switch (signalAspect) {
	case HP0:
		hp0();
		signalAspect=UNDEF;
		break;
	case HP1:
		hp1();
		signalAspect=UNDEF;
		break;
	case HP2:
		hp2();
		signalAspect=UNDEF;
		break;
	case HP0_SH1:
		hp0_sh1();
		signalAspect=UNDEF;
		break;
	}
}
