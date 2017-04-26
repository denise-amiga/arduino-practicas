////
////  Access Control v1.0 by Denise
////
#include "Arduino.h"
#include <time.h>
//#include <SoftwareSerial.h>
//SoftwareSerial BT(11, 12);

////
////  *1b0063652439# //sample data
////

////
////  change for
////  real or testing
////
//#define STX 0x02
//#define ETX 0x03
#define STX '*'
#define ETX '#'

#define NUSERS 5

typedef struct {
	bool			acceso;
	const char *		nombre;
	const char *		tratamiento;
	unsigned long long	tarjeta;
	char			tiempo;
} Ficha;

Ficha ficha[NUSERS] =
{
	{ false, "Desconocido", "Sr.",	 0x00000000, 0	},
	{ true,	 "Nicolas",	"Sr.",	 0x00636521, 5	},
	{ true,	 "Alberto",	"Sr.",	 0x00636522, 10 },
	{ true,	 "Maria",	"Sra.",	 0x00636523, 5	},
	{ false, "Denise",	"Srta.", 0x00636524, 10 }
};

////
////  message1
////   * (negative)
////   * (positive)
////
const char *m1[2] =
{
	"Lo siento ",
	"Bienvenido/a "
};

////
////  message2
////   * (negative)
////   * (positive)
////
const char *m2[2] =
{
	" no tiene permitido el acceso",
	" su acceso esta permitido"
};

char *pstx, *petx;
char rawData[200];
char valorleido[15];
bool lecturaok = false;

//#define CrPin     5
//#define CgPin     6
//#define CbPin     7
#define ArPin     52
#define AgPin     46
#define AbPin     44
#define led4      53
#define led3      51
#define led2      49
#define led1      47
#define fadespeed 250
#define initspeed 4

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

void fadeAccess(bool a)
{
	setColor(0, 0, 200, 0b1000);
	delay(fadespeed);
	setColor(0, 0, 200, 0b0100);
	delay(fadespeed);
	setColor(0, 0, 200, 0b0010);
	delay(fadespeed);
	//  setColor(0, 0, 200, 0b0001);
	//  delay(fadespeed);
	////
	////  access?
	////
	if (a) {
		////
		////  yes (green)
		////
		setColor(0, 200, 0, 0b0001);
	} else {
		////
		////  no (red)
		////
		setColor(200, 0, 0, 0b0001);
	}
	//delay(5000);
	//setColor(0, 0, 0, 0b1111);
}

void initLeds()
{
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

int read2bytes(char *txt, int pos)
{
	int r, p;

	p = *(txt + pos);
	switch (p) {
	case '0' ... '9': p -= 48; break;       //'0'
	case 'A' ... 'F': p -= 55; break;       //'A'+10
	case 'a' ... 'f': p -= 87; break;       //'a'+10
	}
	r = p * 16;
	p = *(txt + pos + 1);
	switch (p) {
	case '0' ... '9': p -= 48; break;       //'0'
	case 'A' ... 'F': p -= 55; break;       //'A'+10
	case 'a' ... 'f': p -= 87; break;       //'a'+10
	}
	r += p;
	return r;
}

int findcard(unsigned long long t)
{
	////
	////  find in "db"
	////
	for (int j = 0; j < NUSERS; j++) {
		////
		////  exits?
		////
		if (t == ficha[j].tarjeta) {
			////
			////  yes
			////  return pos
			////
			return j;
		}
	}
	////
	//// no
	//// return 0
	////
	return 0;
}

void door(char pin, bool pol, Ficha f)
{
	if (f.acceso == true) {
		//digitalWrite(pin,pol);
		Serialprintf("abriendo puerta. (%i segundos)", f.tiempo);
		delay(f.tiempo * 1000);
		//digitalWrite(pin,!pol);
		Serial.println(" cerrando puerta");
		setColor(0, 0, 0, 0b1111);
	}
}

void setup()
{
	//Serial setup
	Serial.begin(38400);
	Serial.println("-= Access Control 1.0 =-");
	Serial1.begin(38400);
	Serial1.println("-= Access Control 1.0 =-");
	Serial.flush();
	Serial1.flush();

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

	initLeds();
}
void loop()
{
	//Serial.print(now());
	int j;

	if (Serial1.available()) {
		int i = Serial1.readBytes(rawData, Serial1.available());

		if (i < 12) return;

		lecturaok = false;
		valorleido[0] = 0;
		pstx = petx = rawData;

		////
		////  find '0x02' -> STX -> Start message
		////
		while ((*pstx != STX) && ((pstx - rawData) < i)) pstx++;
		if ((pstx - rawData) == i) {
			Serial.println("Error: STX no encontrado");
			return;
		}
		petx = pstx++;

		////
		////  find '0x03' -> ETX -> End message
		////
		while ((*petx != ETX) && ((petx - rawData) < i)) petx++;
		if ((petx - rawData) == i) {
			Serial.println("Error: ETX no encontrado");
			return;
		}
		//petx--;

		////
		////  Correct length?
		////
		if ((petx - pstx) == 12) {
			i = 0;
			lecturaok = true;

			////
			////  process all digits
			////
			while ((i < 12) && (lecturaok == true)) {
				char *c;
				c = (pstx + i);

				////
				////  is valid?
				////
				if ((*c >= '0' && *c <= '9') || (*c >= 'A' && *c <= 'F') || (*c >= 'a' && *c <= 'f'))
					lecturaok = true;
				else
					lecturaok = false;
				//printf("%i %c\n", lecturaok, *c);

				////
				////  store it
				////
				valorleido[i++] = *c;
			}

			////
			////  finish string '\0'
			////
			valorleido[i] = 0;

			////
			////  check checksum
			////
			if (lecturaok) {
				int fc, uc1, uc2, uc3, uc4, ck;
				fc = read2bytes(valorleido, 0);
				uc1 = read2bytes(valorleido, 2);
				uc2 = read2bytes(valorleido, 4);
				uc3 = read2bytes(valorleido, 6);
				uc4 = read2bytes(valorleido, 8);
				ck = read2bytes(valorleido, 10);
				//Serialprintf("%i %i %i %i %i %i\n",fc,uc1,uc2,uc3,uc4,ck);

				////
				////  is ok?
				////
				if ((fc ^ uc1 ^ uc2 ^ uc3 ^ uc4) == ck) {
					Serialprintf("Ok\n%.2x %.2x%.2x%.2x%.2x %.2x\n", fc, uc1, uc2, uc3, uc4, ck);

					//          String r = Serial1.readString();
					//          Serial.println(r);

					j = findcard(uc1 * 16777216 + uc2 * 65536 + uc3 * 256 + uc4);
					Serialprintf("%s%s %s%s\n", m1[ficha[j].acceso], ficha[j].tratamiento, ficha[j].nombre, m2[ficha[j].acceso]);

					////
					////  is the card in "db"?
					////
					if (0 != j) {
						////
						////  has access?
						////
						if (ficha[j].acceso) {
							////
							////  db (yes)
							////  access (yes)
							////
							Serial1.write(1);
							fadeAccess(true);
							//Serial1.println("ok");
						} else {
							////
							////  db (yes)
							////  access (no)
							////
							Serial1.write(0);
							fadeAccess(false);
							//Serial1.println("ko");
						}
					} else {
						////
						////  db (no)
						////
						Serial1.write(0);
						fadeAccess(false);
						//Serial1.println("ko");
					}
					door(13, true, ficha[j]);
					setColor(0, 0, 0, 0b1111);
				} else {
					Serialprintf("Error: Checksum incorrecto\n%.4x %.4x\n", (fc ^ uc1 ^ uc2 ^ uc3 ^ uc4), ck);
				}
			} else {
				Serialprintf("Error: Digito invalido\n%s\n", valorleido);
			}
		} else {
			//printf("Error lectura incompleta\n%I64d\n", (petx - pstx));
			Serial.print("Error: De lectura ");
			if ((petx - pstx) < 12)
				Serial.println("(faltan caracteres)");
			else
				Serial.println("(sobran caracteres)");
		}
		//Serial.println("bye");
		return;
	}
	delay(250);
	if (Serial.available())
		Serial1.write(Serial.read());
}
