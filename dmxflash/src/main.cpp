#include "Arduino.h"
#include "DMXSerial.h"

unsigned long sindatos;
int listadepines[5] = { 2, 4, 6, 56, 45 };

void setup()
{
	Serial.begin(38400);
	DMXSerial.maxChannel(5);
	DMXSerial.init(DMXReceiver);
	for (int i = 0; i < 6; i++)
		pinMode(listadepines[i], OUTPUT);
}

void loop()
{
	sindatos = DMXSerial.noDataSince();

	if (sindatos > 5000) {
		Serial.println("Revise la conexion");
		for (int i = 0; i < 6; i++)
			analogWrite(listadepines[i], 0x7f);
	} else {
		for (int i = 0; i < 6; i++)
			analogWrite(listadepines[i], DMXSerial.read(30 + i));
	}
}
