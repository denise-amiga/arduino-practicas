#ifndef pines_iluminacion_h
#define pines_iluminacion_h

// ********************************** Tiempos **********************************
#if DEBUG
#define TIMBRE_TIMER            15                                              // tiempo antes de poder volver a pulsar
#define TIMBRE_SONANDO          5                                               // tiempo sonando
#define TIMBRE_DELTA            (TIMBRE_TIMER - TIMBRE_SONANDO)

#define PASILLO_TIMER           15                                              // 15 segundos
#define DORM2_TIMER             10                                              // 30 segundos
#define GARAJE_TIMER            10                                              // 10 segundos
#else
#define TIMBRE_TIMER            30                                              // tiempo antes de poder volver a pulsar
#define TIMBRE_SONANDO          5                                               // tiempo sonando
#define TIMBRE_DELTA            (TIMBRE_TIMER - TIMBRE_SONANDO)

#define PASILLO_TIMER           60                                              // 1 minuto (1*60)
#define DORM2_TIMER             1800                                            // 30 minutos (30*60)
#define GARAJE_TIMER            600                                             // 10 minutos (10*60)
#endif
//******************************************************************************

//******************************* Macros de ayuda ******************************
#define OFFSETENTRADAS          2
#define OFFSETSALIDAS           22
#define LECTURAENTRADA(x)       (pines[x].antes = digitalRead((x + OFFSETENTRADAS)))
#define ACTUALIZAENTRADA(x)     (pines[x].despues = pines[x].antes)
#define ACTUALIZASALIDA(x)      (pines[x].salida = digitalRead((x + OFFSETSALIDAS)))
#define FLANCO_BAJADA(x)        ((pines[x].antes == LOW) && (pines[x].despues == HIGH))
#define FLANCO_SUBIDA(x)        ((pines[x].antes == HIGH) && (pines[x].despues == LOW))
#define SALIDA_LOW(x)           (digitalWrite((x + OFFSETSALIDAS), LOW))
#define SALIDA_HIGH(x)          (digitalWrite((x + OFFSETSALIDAS), HIGH))
#define SALIDA_CONMUTA(x)       (digitalWrite((x + OFFSETSALIDAS), !digitalRead((x + OFFSETSALIDAS))))
//******************************************************************************

#endif
