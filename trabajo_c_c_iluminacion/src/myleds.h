#ifndef myLeds_h
#define myLeds_h

#include "Arduino.h"

#define ArPin                   52
#define AgPin                   46
#define AbPin                   44
#define led4                    53
#define led3                    51
#define led2                    49
#define led1                    47
#define fadespeed               250
#define initspeed               4

void setColor(byte r, byte g, byte b, byte mask);
void initLeds(int initdelay);
void Serialprintf(const char *txt, ...);

#endif
