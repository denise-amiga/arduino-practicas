#include "myleds.h"

#define FLANCO_BAJADA(x) ((x ## _antes == LOW) && (x ## _despues == HIGH))
#define FLANCO_SUBIDA(x) ((x ## _antes == HIGH) && (x ## _despues == LOW))

#define t4_tiempo       15
#define t5_tiempo       15                              // tiempo antes de poder volver a pulsar
#define t5_sonando       3                              // tiempo sonando
#define t5_delta        t5_tiempo - t5_sonando          // calculo intermedio necesario

bool bt4_t_once = false;
bool bt5_t_once = false;
bool bt5_s_once = false;

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
	if (FLANCO_BAJADA(pin4)) {
		Serialprintf("T4: On\n");
		bt4_t_once = true;
		tiempo4 = t4_tiempo;
	}
	pin4_despues = pin4_antes;
	if (bt4_t_once) {
		if (tiempo4 > 0) {
			Serialprintf("T4 %d\n", tiempo4);
			tiempo4--;
		} else {
			Serialprintf("T4: Off\n");
			bt4_t_once = false;
		}
	}

	pin5_antes = digitalRead(5);
	if (FLANCO_BAJADA(pin5) && (tiempo5 == 0)) {
		Serialprintf("Tiempo5: On\n");
		bt5_t_once = true;
		bt5_s_once = true;
		tiempo5 = t5_tiempo;
	}
	pin5_despues = pin5_antes;

	if (bt5_s_once) {
		if (tiempo5 > t5_delta) {
			Serialprintf("Sonando5: on\n");
		} else {
			Serialprintf("Sonando5: off\n");
			bt5_s_once = false;
		}
	}
	if (bt5_t_once) {
		if (tiempo5 > 0) {
			Serialprintf("T5 %d\n", tiempo5);
			tiempo5--;
		} else {
			Serialprintf("T5: Off\n");
			bt5_t_once = false;
		}
	}
	delay(1000);
}
