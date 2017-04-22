#include "myleds.h"

#define FLANCO_BAJADA(x) ((x ## _antes == LOW) && (x ## _despues == HIGH))
#define FLANCO_SUBIDA(x) ((x ## _antes == HIGH) && (x ## _despues == LOW))

int pin4_antes = HIGH, pin4_despues = HIGH;
int pin5_antes = HIGH, pin5_despues = HIGH;
int tiempo4, tiempo5;

void setup()
{
	Serial.begin(38400);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	initLeds(2500);
}

void loop()
{
	pin4_antes = digitalRead(4);
	if (FLANCO_BAJADA(pin4))
		tiempo4 = 10;
	pin4_despues = pin4_antes;
	if (tiempo4 > 0) {
		Serialprintf("Tiempo4 %d\n", tiempo4);
		tiempo4--;
	}
	pin5_antes = digitalRead(5);
	if (FLANCO_BAJADA(pin5) && (tiempo5 == 0))
		tiempo5 = 10;
	pin5_despues = pin5_antes;
	if (tiempo5 > 0) {
		Serialprintf("Tiempo5 %d\n", tiempo5);
		tiempo5--;
	}
	delay(1000);
}
