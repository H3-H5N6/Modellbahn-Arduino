#ifndef DCC_h
#define DCC_h

#include <NmraDcc.h>

NmraDcc Dcc;


#define DCC_INTERRUPT_NUMBER 0
#define DCC_IN_PIN           2
#define DCC_IN_ENABLE_PULLUP true
#define DCC_ACC_PIN          12

#define VERSION_ID 1

#define CV_7_VERSION 7
#define CV_8_MANUFACTURER 8
#define CV_29_DECODER_CONFIGURATION 29

#define CV_MAX 10

#define DEFAULT_LONG_ADDRESS 785

void initDcc() {
	Serial.print("Initialisiere DCC ... ");
	pinMode(DCC_ACC_PIN, OUTPUT);
	Dcc.pin(DCC_INTERRUPT_NUMBER, DCC_IN_PIN, DCC_IN_ENABLE_PULLUP);
	Dcc.init(Dcc.getCV(CV_8_MANUFACTURER), Dcc.getCV(CV_8_MANUFACTURER), Dcc.getCV(CV_29_DECODER_CONFIGURATION), 0);
	Serial.println("fertig");
}

uint8_t calcLSB(uint16_t longAddr) {
	return longAddr % 256;
}

uint8_t calcMSB(uint16_t longAddr) {
	return longAddr / 256;
}

/**
 *
 CV#1	Basisadresse Teil 1 Gruppe A
 CV#3	Timer für Ausgänge 1,2 Gruppe A
 CV#4	Timer für Ausgänge 3,4 Gruppe A
 CV#5	Timer für Ausgänge 5,6 Gruppe A
 CV#6	Timer für Ausgänge 7,8 Gruppe A
 CV#7	Versionsnummer
 CV#8	Herstelleridentnummer
 CV#9	Basisadresse Teil 2 Gruppe A // 	Adress-Berechnung: CV#9 * 64 + Wert in CV#1

 CV#17	POM Adresse, Teil 1
 CV#18	POM Adresse, Teil 2
 CV#28	RailCom-Einstellungen
 CV#29	Konfigurationsregister
 CV#35	Basisadresse Teil 1 Gruppe B
 CV#36	Basisadresse Teil 2 Gruppe B


 CV#57	Betriebsart Gruppe A
 CV#58	Betriebsart Gruppe B
 *
 */
void initDccConfig() {
	if (Dcc.getCV(CV_7_VERSION) != VERSION_ID || Dcc.getCV(CV_8_MANUFACTURER) != MAN_ID_DIY) {
		Serial.print("Schreibe Default-Werte ...");
		Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, calcLSB(DEFAULT_LONG_ADDRESS));
		Dcc.setCV(CV_7_VERSION, VERSION_ID);
		Dcc.setCV(CV_8_MANUFACTURER, MAN_ID_DIY);
		Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, calcMSB(DEFAULT_LONG_ADDRESS));
		Dcc.setCV(CV_29_DECODER_CONFIGURATION, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE);
		Serial.println("fertig");
	} else {
		Serial.println("Keine Änderungen an den CV");
	}
	for (int i = 1; i < 30; i++) {
		switch (i) {
		case CV_7_VERSION:
		case CV_8_MANUFACTURER:
		case CV_ACCESSORY_DECODER_ADDRESS_LSB:
		case CV_ACCESSORY_DECODER_ADDRESS_MSB:
		case CV_29_DECODER_CONFIGURATION:
			Serial.print("CV: ");
			Serial.print(i);
			Serial.print("[");
			Serial.print(Dcc.getCV(i));
			Serial.println("]");
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



// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState(uint16_t Addr, uint8_t State) {
	Serial.print("notifyDccSigOutputState: ");
	Serial.print(Addr, DEC);
	Serial.print(',');
	Serial.println(State, HEX);
}

#endif
