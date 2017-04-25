#define DEBUG 1
#include "myleds.h"
#include "pines_iluminacion.h"

enum zonas { TIMBRE, COCINA, COMEDOR, PASILLO, DORMITORIO1, DORMITORIO2, GARAGE, BANO1, BANO2, FINAL };

typedef struct {
	int	tiempo;         // tiempo para los temporizadores
	bool	flag1;          // flag para ejecutar cuando sea necesario
	bool	flag2;          // flag especial para el timbre
	bool	antes;          // estado (1) para detectar flancos
	bool	despues;        // estado (2) para detectar flancos
	bool	salida;         // estado de la salida
} Pines;

Pines pines[FINAL] = { 0, false, false, HIGH, HIGH, LOW };

void setup()
{
	Serial.begin(38400);
	for (int i = (TIMBRE + OFFSETENTRADAS); i < (FINAL + OFFSETENTRADAS); i++) {
		pinMode(i, INPUT_PULLUP);
		pinMode(i + 20, OUTPUT);
		digitalWrite(i + 20, LOW);
	}

	initLeds(2500);
}

void loop()
{
// ********************************** Timbre ***********************************
	LECTURAENTRADA(TIMBRE);
	if (FLANCO_BAJADA(TIMBRE) && (pines[TIMBRE].tiempo == 0)) {     // hemos pulsado el timbre y el temp=0?
		SALIDA_HIGH(TIMBRE);                                    // activamos el timbre
		ACTUALIZASALIDA(TIMBRE);                                // actualiza la salida
		Serialprintf("Temporizador Timbre: On\n");              // debug info
		pines[TIMBRE].flag1 = true;                             // flag para el timer (timbre)
		pines[TIMBRE].flag2 = true;                             // flag para el timbre (sonando)
		pines[TIMBRE].tiempo = TIMBRE_TIMER;                    // ponemos el timer
	}
	ACTUALIZAENTRADA(TIMBRE);

	if (pines[TIMBRE].flag2) {                                                      // ejecutamos este bloque? (al menos una vez,si pulsamos)
		if (pines[TIMBRE].tiempo > TIMBRE_DELTA) {                              // queda tiempo sonando el timbre?
			Serialprintf("Timbre sonando\n");                               // debug info (SI)
		} else {
			SALIDA_LOW(TIMBRE);                                             // desactivamos el timbre
			Serialprintf("Timbre parado\n");                                // debug info (NO)
			pines[TIMBRE].flag2 = false;                                    // no volvemos a ejecutar este bloque
		}
	}

	if (pines[TIMBRE].flag1) {                                                      // ejecutamos este bloque? (al menos una vez,si pulsamos)
		if (pines[TIMBRE].tiempo > 0) {                                         // queda tiempo del temporizador del timbre?
			Serialprintf("Temp. timbre %d\n", pines[TIMBRE].tiempo);        // debug info (tiempo timer)
			pines[TIMBRE].tiempo--;                                         // actualizamos el timer
		} else {
			Serialprintf("Temporizador Timbre: Off\n");                     // debug info (se acabo el timer,podemos volver a pulsar)
			pines[TIMBRE].flag1 = false;                                    // no volvemos a ejecutar este bloque
		}
	}

// ********************************** Cocina ***********************************
	LECTURAENTRADA(COCINA);
	if (FLANCO_BAJADA(COCINA)) {                                                    // hemos pulsado la cocina?
		SALIDA_CONMUTA(COCINA);                                                 // conmutamos la cocina
		ACTUALIZASALIDA(COCINA);
		Serialprintf("Cocina: %s\n", pines[COCINA].salida ? "On" : "Off");      // debug info
	}
	ACTUALIZAENTRADA(COCINA);

// ********************************** Comedor **********************************
	LECTURAENTRADA(COMEDOR);
	if (FLANCO_BAJADA(COMEDOR)) {                                                   // hemos pulsado el comedor?
		SALIDA_CONMUTA(COMEDOR);                                                // conmutamos el comedor
		ACTUALIZASALIDA(COMEDOR);
		Serialprintf("Comedor: %s\n", pines[COMEDOR].salida ? "On" : "Off");    // debug info
	}
	ACTUALIZAENTRADA(COMEDOR);

// ********************************** Pasillo **********************************
	LECTURAENTRADA(PASILLO);
	if (FLANCO_BAJADA(PASILLO)) {                                                   // hemos pulsado el pasillo?
		SALIDA_HIGH(PASILLO);                                                   // Encendemos el pasillo
		ACTUALIZASALIDA(PASILLO);
		Serialprintf("Pasillo: On\n");                                          // debug info
		Serialprintf("Temporizador Pasillo: On\n");                             // debug info
		pines[PASILLO].flag1 = true;                                            // flag para el timer (pasillo)
		pines[PASILLO].tiempo = PASILLO_TIMER;                                  // ponemos el timer
	}
	ACTUALIZAENTRADA(PASILLO);

	if (pines[PASILLO].flag1) {                                                     // ejecutamos este bloque? (al menos una vez,si pulsamos)
		if (pines[PASILLO].tiempo > 0) {                                        // queda tiempo del temporizador del pasillo?
			Serialprintf("Temp. pasillo %d\n", pines[PASILLO].tiempo);      // debug info (pasillo timer)
			pines[PASILLO].tiempo--;                                        // actualizamos el timer
		} else {
			SALIDA_LOW(PASILLO);                                            // apagamos el pasillo
			ACTUALIZASALIDA(PASILLO);
			Serialprintf("Pasillo: Off\n");                                 // debug info
			Serialprintf("Temporizador Pasillo: Off\n");                    // debug info
			pines[PASILLO].flag1 = false;                                   // no volvemos a ejecutar este bloque
		}
	}

// ******************************** Dormitorio1 ********************************
	LECTURAENTRADA(DORMITORIO1);
	if (FLANCO_BAJADA(DORMITORIO1)) {                                                       // hemos pulsado el Dormitorio1?
		SALIDA_CONMUTA(DORMITORIO1);                                                    // conmutamos el Dormitorio1
		ACTUALIZASALIDA(DORMITORIO1);
		Serialprintf("Dormitorio1: %s\n", pines[DORMITORIO1].salida ? "On" : "Off");    // debug info
	}
	ACTUALIZAENTRADA(DORMITORIO1);

// ******************************** Dormitorio2 ********************************
	LECTURAENTRADA(DORMITORIO2);
	if (FLANCO_BAJADA(DORMITORIO2)) {                                                                       // hemos pulsado el Dormitorio2?
		SALIDA_CONMUTA(DORMITORIO2);                                                                    // conmutamos el Dormitorio2
		ACTUALIZASALIDA(DORMITORIO2);                                                                   //
		Serialprintf("Dormitorio2: %s\n", pines[DORMITORIO2].salida ? "On" : "Off");                    // debug info
		Serialprintf("Temporizador Dormitorio2: %s\n", pines[DORMITORIO2].salida ? "On" : "Off");       // debug info
		pines[DORMITORIO2].flag1 = pines[DORMITORIO2].salida;                                           // flag para el timer (Dormitorio2)
		pines[DORMITORIO2].tiempo = DORM2_TIMER;                                                        // ponemos el timer
	}
	ACTUALIZAENTRADA(DORMITORIO2);

	if (pines[DORMITORIO2].flag1) {                                                         // ejecutamos este bloque? (al menos una vez,si)
		if (pines[DORMITORIO2].tiempo > 0) {                                            // queda tiempo del temporizador del Dormitorio2?
			Serialprintf("Temp. Dormitorio2 %d\n", pines[DORMITORIO2].tiempo);      // debug info (Dormitorio2 timer)
			pines[DORMITORIO2].tiempo--;                                            // actualizamos el timer
		} else {
			Serialprintf("Temporizador Dormitorio2: Off\n");                        // debug info (se acabo el tiempo)
			if (pines[DORMITORIO2].salida == HIGH) {                                // esta encendida la luz?
				SALIDA_LOW(DORMITORIO2);                                        // la apagamos en caso afirmativo
				ACTUALIZASALIDA(DORMITORIO2);
				Serialprintf("Dormitorio2: Off (Temporizador)\n");              // debug info
			}
			pines[DORMITORIO2].flag1 = false;                                       // no volvemos a ejecutar este bloque
		}
	}

// ********************************** Garage ***********************************
	LECTURAENTRADA(GARAGE);
	if (FLANCO_BAJADA(GARAGE)) {                                                                    // hemos pulsado el Garage?
		SALIDA_CONMUTA(GARAGE);                                                                 // conmutamos el Dormitorio2
		ACTUALIZASALIDA(GARAGE);                                                                //
		Serialprintf("Garage: %s\n", pines[GARAGE].salida ? "On" : "Off");                      // debug info
		Serialprintf("Temporizador Garage: %s\n", pines[GARAGE].salida ? "On" : "Off");         // debug info
		pines[GARAGE].flag1 = pines[GARAGE].salida;                                             // flag para el timer (Garage)
		pines[GARAGE].tiempo = GARAGE_TIMER;                                                    // ponemos el timer
	}
	ACTUALIZAENTRADA(GARAGE);

	if (pines[GARAGE].flag1) {                                                      // ejecutamos este bloque? (al menos una vez,si)
		if (pines[GARAGE].tiempo > 0) {                                         // queda tiempo del temporizador del Garage?
			Serialprintf("Temp. Garage %d\n", pines[GARAGE].tiempo);        // debug info (Garage timer)
			pines[GARAGE].tiempo--;                                         // actualizamos el timer
		} else {
			Serialprintf("Temporizador Garage: Off\n");                     // debug info (se acabo el tiempo)
			if (pines[GARAGE].salida == HIGH) {                             // esta encendida la luz?
				SALIDA_LOW(GARAGE);                                     // la apagamos en caso afirmativo
				ACTUALIZASALIDA(GARAGE);
				Serialprintf("Garage: Off (Temporizador)\n");           // debug info
			}
			pines[GARAGE].flag1 = false;                                    // no volvemos a ejecutar este bloque
		}
	}

// ******************************** Bano ********************************
	LECTURAENTRADA(BANO1);
	if (FLANCO_BAJADA(BANO1)) {                                                     // hemos pulsado el Bano1?
		SALIDA_CONMUTA(BANO1);                                                  // conmutamos el Bano1
		ACTUALIZASALIDA(BANO1);
		Serialprintf("Luz1 Bano: %s\n", pines[BANO1].salida ? "On" : "Off");    // debug info
	}
	ACTUALIZAENTRADA(BANO1);

// ******************************** Bano ********************************
	LECTURAENTRADA(BANO2);
	if (FLANCO_BAJADA(BANO2)) {                                                     // hemos pulsado el Bano2?
		SALIDA_CONMUTA(BANO2);                                                  // conmutamos el Bano2
		ACTUALIZASALIDA(BANO2);
		Serialprintf("Luz2 Bano: %s\n", pines[BANO2].salida ? "On" : "Off");    // debug info
	}
	ACTUALIZAENTRADA(BANO2);

	delay(1000);
}
