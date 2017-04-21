// lectura teclado

#include "myleds.h"

#define OPEN_TIMER      5       // s
#define RESET_TIMER     5000    // ms
#define MAX_FAILS       3
#define WAITING_FAILS   30      // s
#define DIGITS_NUMBERS  4
#define ELECTROIMAN_PIN 13

char teclado[DIGITS_NUMBERS];
int clave = 6666;
int fails = 0;

void updatestatus(int a, int b)
{
	if (b) {
		switch (a) {
		case 1: setColor(0, 0, 200, 0b1000); break;
		case 2: setColor(0, 0, 200, 0b1100); break;
		case 3: setColor(0, 0, 200, 0b1110); break;
		case 4: setColor(0, 0, 200, 0b1111); break;
		}
	} else {
		switch (a) {
		case 1: setColor(200, 0, 0, 0b1000); break;
		case 2: setColor(200, 0, 0, 0b1100); break;
		case 3: setColor(200, 0, 0, 0b1110); break;
		case 4: setColor(200, 0, 0, 0b1111); break;
		}
	}
}

void setup()
{
	Serial.begin(38400);

	for (int i = 3; i <= 12; i++)
		pinMode(i, INPUT_PULLUP);

	pinMode(ELECTROIMAN_PIN, OUTPUT);
	digitalWrite(ELECTROIMAN_PIN, HIGH);

	initLeds(2500);
}

void loop()
{
	int c = 0;
	unsigned long tm = millis() + RESET_TIMER;

	while (c < DIGITS_NUMBERS && millis() < tm) {
		for (int i = 3; i <= 12; i++) {
			if (digitalRead(i) == LOW) {
				Serialprintf("%d -> %d\n", c, i - 3);
				teclado[c++] = i - 3;
				updatestatus(c, true);
				tm = millis() + RESET_TIMER;
				delay(1000);
			}
		}
	}

	if (c == DIGITS_NUMBERS) {
		Serial.print("checking code...");
		Serial.println(teclado[0] * 1000 + teclado[1] * 100 + teclado[2] * 10 + teclado[3]);
		if (teclado[0] * 1000 + teclado[1] * 100 + teclado[2] * 10 + teclado[3] == clave) {
			fails = 0;
			Serialprintf("valid code\nopening door ( %d seconds )\n", OPEN_TIMER);
			setColor(0, 200, 0, 0b1111);
			digitalWrite(ELECTROIMAN_PIN, LOW);
			delay(OPEN_TIMER * 1000);
			Serial.println("closing door");
			digitalWrite(ELECTROIMAN_PIN, HIGH);
		} else {
			Serialprintf("wrong code\nfails: %d\n", ++fails);
			updatestatus(fails, false);
		}
	} else if (c > 0) {
		Serial.println("reset time");
	}

	if (fails >= MAX_FAILS) {
		fails = 0;
		Serialprintf("Too many fails. Waiting %d s.\n", WAITING_FAILS);
		delay(WAITING_FAILS * 1000);
	}

	delay(1000);
	setColor(0, 0, 0, 0b1111);
}
