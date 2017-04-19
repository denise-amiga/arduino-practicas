// control centralizado de iluminacion

#include "myleds.h"

#define pulsador_conmutador                     4
#define pulsador_temporizado            5
#define pulsador_zumbador                       6
#define salida_conmutada                        10
#define salida_temporizada                      11
#define salida_zumbador                         12

#define tiempo_temporizador                     20
#define tiempo_zumbador1                        5
#define tiempo_zumbador2                        15

int pin4_status;
int tiempo1;
int tiempo2;
int tiempo3;
boolean zumbador_status = false;

void setup()
{
	Serial.begin(38400);

	for (int i = 4; i <= 9; i++) {
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
	// pulsador_conmutador
	if ((digitalRead(pulsador_conmutador) == LOW) && (pin4_status == HIGH)) {
		digitalWrite(salida_conmutada, !digitalRead(salida_conmutada));
		Serialprintf("Conmutador: %s\n", digitalRead(salida_conmutada) ? "On" : "Off");
	}

	pin4_status = digitalRead(pulsador_conmutador);

	// pulsador_temporizado
	if (digitalRead(pulsador_temporizado) == LOW) {
		digitalWrite(salida_temporizada, HIGH);
		tiempo1 = tiempo_temporizador;
	}
	if (tiempo1 > 0) {
		Serialprintf("Temporizador: %d\n", tiempo1);
		tiempo1--;
	} else {
		digitalWrite(salida_temporizada, LOW);
	}

	// pulsador_zumbador
	if ((digitalRead(pulsador_zumbador) == LOW) && (digitalRead(salida_zumbador) == LOW) && (zumbador_status == false)) {
		tiempo2 = tiempo_zumbador1;     // tiempo sonando
		tiempo3 = tiempo_zumbador2;     // tiempo para poder volver a pulsar
		digitalWrite(salida_zumbador, HIGH);
		zumbador_status = true;
	}
	if (tiempo2 > 0) {
		Serialprintf("Zumbador Sonando: %d\n", tiempo2);
		tiempo2--;
	} else {
		digitalWrite(salida_zumbador, LOW);
	}
	if (tiempo3 > 0) {
		Serialprintf("Temporizador->Zumbador: %d\n", tiempo3);
		tiempo3--;
	} else {
		zumbador_status = false;
	}
	delay(1000);
}
