#include "Arduino.h"
#include "DMXSerial.h"

byte nivel[5]={0};

void setup() {
    DMXSerial.maxChannel(5);
    DMXSerial.init(DMXController, 11);

}

void loop() {

}
