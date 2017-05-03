#include "Arduino.h"
#include "DMXSerial.h"

void setup() {
  Serial1.begin(38400);
  DMXSerial.maxChannel(16);
  DMXSerial.init(DMXController, 8); //1
}

void loop() {
  unsigned long tiempo = millis();
  DMXSerial.write(2, 0);
  DMXSerial.write(3, 0);
  for (int i = 0; i <= 255; i++) {
    DMXSerial.write(1, i);
    delay(10);
  }
  DMXSerial.write(1, 0);
  DMXSerial.write(3, 0);
  for (int i = 0; i <= 255; i++) {
    DMXSerial.write(2, i);
    delay(10);
  }
  DMXSerial.write(1, 0);
  DMXSerial.write(2, 0);
  for (int i = 0; i <= 255; i++) {
    DMXSerial.write(3, i);
    delay(10);
  }
  Serial1.print("Tiempo invertido");
  Serial1.println((millis() - tiempo) / 1000.0, 3);
}
