#ifndef pines_iluminacion_h
#define pines_iluminacion_h

// ********************************** Tiempos **********************************
#define TIMBRE_TIMER            20                                              // tiempo antes de poder volver a pulsar
#define TIMBRE_SONANDO          5                                               // tiempo sonando
#define TIMBRE_DELTA            (TIMBRE_TIMER - TIMBRE_SONANDO)

#define PASILLO_TIMER           30      //60                                    // 1 minuto (1*60)
#define DORM2_TIMER             10      //1800                                  // 30 minutos (30*60)
#define GARAGE_TIMER            10      //600                                   // 10 minutos (10*60)
//******************************************************************************

//************************** Pines de entrada/salida ***************************
#define PUL_TEMP_TIMBRE         2
#define SALIDA_TIMBRE           22

#define CONM_COCINA             3
#define SALIDA_COCINA           23

#define CONM1_COMEDOR           4
#define COMN2_COMEDOR           CONM1_COMEDOR
#define SALIDA_COMEDOR          24

#define CONM1_PASILLO           5
#define CONM2_PASILLO           CONM1_PASILLO
#define SALIDA_PASILLO          25

#define CONM1_DORMITORIO1       6
#define CONM2_DORMITORIO1       CONM1_DORMITORIO1
#define CONM3_DORMITORIO1       CONM1_DORMITORIO1
#define SALIDA_DORMITORIO1      26

#define CONM1_TEMP_DORMITORIO2  7
#define CONM2_TEMP_DORMITORIO2  CONM1_DORMITORIO2
#define SALIDA_DORMITORIO2      27

#define CONM1_TEMP_GARAGE       8
#define CONM2_TEMP_GARAGE       CONM1_TEMP_GARAGE
#define SALIDA_TEMP_GARAGE      28

#define CONM1_BANO1             9
#define CONM2_BANO1             CONM1_BANO1
#define SALIDA_BANO1            29

#define CONM_BANO2              10
#define SALIDA_BANO2            30
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
