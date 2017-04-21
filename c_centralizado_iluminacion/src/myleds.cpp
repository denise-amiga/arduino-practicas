#include "myleds.h"

void initLeds(int initdelay)
{
	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	digitalWrite(led3, LOW);
	digitalWrite(led4, LOW);

	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);
	pinMode(led4, OUTPUT);

	//pinMode(CrPin, OUTPUT);
	//pinMode(CgPin, OUTPUT);
	//pinMode(CbPin, OUTPUT);
	pinMode(ArPin, OUTPUT);
	pinMode(AgPin, OUTPUT);
	pinMode(AbPin, OUTPUT);

	if (initdelay)
		delay(initdelay);            //skip reset by serial init

	////
	////  red fade
	////
	for (int x = 0; x < 255; x++) {
		setColor(x, 0, 0, 0b1111);
		delay(initspeed);
	}
	////
	////  green fade
	////
	for (int x = 0; x < 255; x++) {
		setColor(0, x, 0, 0b1111);
		delay(initspeed);
	}
	////
	////  blue fade
	////
	for (int x = 0; x < 255; x++) {
		setColor(0, 0, x, 0b1111);
		delay(initspeed);
	}
	////
	////  lights off
	////
	setColor(0, 0, 0, 0b0000);
}

void setColor(byte r, byte g, byte b, byte mask)
{
	//if ((mask & 0b10000) == 0b10000){
	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	digitalWrite(led3, LOW);
	digitalWrite(led4, LOW);
	//}
	////
	////  common catodo led
	////
	//analogWrite(CrPin, r);
	//analogWrite(CgPin, g);
	//analogWrite(CbPin, b);
	////
	////  common anodo led
	////
	analogWrite(ArPin, 255 - r);
	analogWrite(AgPin, 255 - g);
	analogWrite(AbPin, 255 - b);
	////
	////  turn on led (1,2,3,4)
	////
	if ((mask & 0b0001) == 0b0001) digitalWrite(led1, HIGH);
	if ((mask & 0b0010) == 0b0010) digitalWrite(led2, HIGH);
	if ((mask & 0b0100) == 0b0100) digitalWrite(led3, HIGH);
	if ((mask & 0b1000) == 0b1000) digitalWrite(led4, HIGH);
}

void Serialprintf(const char *format, ...)
{
	char buff[1024];
	va_list args;

	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	buff[sizeof(buff) / sizeof(buff[0]) - 1] = '\0';
	Serial.print(buff);
}
