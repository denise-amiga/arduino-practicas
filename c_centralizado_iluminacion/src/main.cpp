// control centralizado de iluminacion

#include "myleds.h"

int pin4_status;
int tiempo;

void setup()
{
	Serial.begin(38400);

	for (size_t i = 4; i <= 9; i++) {
		pinMode(i, INPUT_PULLUP);
		pinMode(i + 6, OUTPUT);
		digitalWrite(i + 6, LOW);
	}
//    for (size_t i = 10; i <= 15; i++) {
//        pinMode(i, OUTPUT);
//        digitalWrite(i,LOW);
//    }
	initLeds();
}

void loop()
{
	if ((digitalRead(4) == LOW) && (pin4_status == HIGH)) {
		digitalWrite(10, !digitalRead(10));
		Serial.println(digitalRead(10) ? "On" : "Off");
	}
	if (digitalRead(5) == LOW)
		tiempo = 20;
	if (tiempo > 0) {
		digitalWrite(11, HIGH);
		Serial.println(tiempo);
		tiempo--;
	} else {
		digitalWrite(11, LOW);
	}
	pin4_status = digitalRead(4);
	delay(1000);
}
