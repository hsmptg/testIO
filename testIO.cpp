#include "testIO.h"
#include "ssd1306.h"

ssd1306 lcd;

// https://www.pjrc.com/teensy/td_uart.html
// https://www.pjrc.com/teensy/td_timing_IntervalTimer.html
IntervalTimer myTimer;

#define pinOUT 20
#define pinIN 16

int ledState = LOW;

void ping(void) {
	static int c = 0;
	static int dim = 0;
	static int cnt = 0;
	static char str[20];

	if (++c == 10) {
		c = 0;

		analogWrite(pinOUT, dim);
		dim = (dim + 5) % 256;

		if (ledState == LOW) {
			ledState = HIGH;
		} else {
			ledState = LOW;
		}
		digitalWrite(pinOUT, ledState);

		if (!digitalRead(pinIN)) {
			sprintf(str, "cnt = %d", cnt++);
			lcd.printAt(1, 1, str);
		}
	}
}

void setup() {
	pinMode(pinOUT, OUTPUT);
	pinMode(pinIN, INPUT);

//	https://www.pjrc.com/teensy/td_pulse.html

	lcd.begin();
	lcd.clear();
	lcd.display(true);
//	lcd.invert(true);

	lcd.printAt(0, 0, "IP: 192.168.102.123");

//	lcd.print2At(2, 2, "X: 123");
	lcd.print2At(2, 2, "X: ABC");
	lcd.print2At(4, 2, "Y:  34");
	lcd.print2At(6, 2, "Z: 240");

	myTimer.begin(ping, 1000);
}

void loop() {
	delay(100);
/*
	if (digitalRead(pinIN)) {
		digitalWrite(pinOUT, HIGH);
		delay(100);
		digitalWrite(pinOUT, LOW);
		delay(100);
		digitalWrite(pinOUT, HIGH);
		delay(100);
		digitalWrite(pinOUT, LOW);
		delay(700);
	}
 */
}
