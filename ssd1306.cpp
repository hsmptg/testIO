#include "ssd1306.h"

const int ssPin = 10;
const int rstPin = 14;
const int dcPin = 15;

ssd1306::ssd1306() {
	uint8_t *p;
	p = buf;
	for (int i=0; i<8*126; i++)
		*p++ = 0x00;
}

void ssd1306::display(boolean on) {
	if (on)
		sendCmd(SSD1306_DISPLAYON);
	else
		sendCmd(SSD1306_DISPLAYOFF);
}

void ssd1306::invert(boolean inv) {
	if (inv)
		sendCmd(SSD1306_INVERTDISPLAY);
	else
		sendCmd(SSD1306_NORMALDISPLAY);
}

void ssd1306::clear() {
	sendCmd(SSD1306_COLUMNADDR);
	sendCmd(0);
	sendCmd(127);
	sendCmd(SSD1306_PAGEADDR);
	sendCmd(0);
	sendCmd(7);

	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, LOW);
	for (int p=0; p<8; p++)
	    for (int i=0; i<128; i++)
	    	SPI.transfer(0x00);
	digitalWrite(ssPin, HIGH);
}

void ssd1306::printBuf() {
	sendCmd(SSD1306_COLUMNADDR);
	sendCmd(1);
	sendCmd(126);
	sendCmd(SSD1306_PAGEADDR);
	sendCmd(0);
	sendCmd(7);

	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, LOW);
	for (int p=0; p<8; p++)
	    for (int i=0; i<126; i++)
	    	SPI.transfer(buf[p*126+i]);
	digitalWrite(ssPin, HIGH);
}

void ssd1306::printAtBuf(int row, int col, char* msg) {
	int b, f;

    while (*msg) {
        b = 6*(row*21 + col);
        f = 5*(*msg-32);
    	for (int i=0; i<5; i++)
    		buf[b++] = font[f++];
    	buf[b] = 0x00;
    	col++;
		msg++;
    }
}

void ssd1306::printAt(int row, int col, char* msg) {
	sendCmd(SSD1306_COLUMNADDR);
	sendCmd(1 + 6*col);
	sendCmd(126);
	sendCmd(SSD1306_PAGEADDR);
	sendCmd(row);
	sendCmd(row);

	int f;

	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, LOW);
    while (*msg) {
        f = 5*(*msg-32);
    	for (int i=0; i<5; i++)
    		SPI.transfer(font[f++]);
		SPI.transfer(0x00);
		msg++;
    }
	digitalWrite(ssPin, HIGH);
}

void ssd1306::print2AtBuf(int row, int col, char* msg) {
	int b, f;

	while (*msg) {
		b = 6*(row*21 + col);
		f = 20*(*msg-32);
		for (int i=0; i<10; i++)
			buf[b++] = font2[f++];
		buf[b++] = 0x00;
		buf[b++] = 0x00;
		b = 6*(row*21 + col) + 126;
		f = 20*(*msg-32) + 10;
		for (int i=0; i<10; i++)
			buf[b++] = font2[f++];
		buf[b++] = 0x00;
		buf[b++] = 0x00;
		col += 2;
		msg++;
	}
}

void ssd1306::print2At(int row, int col, char* msg) {
	int f;
	char* str;
	str = msg;

	sendCmd(SSD1306_COLUMNADDR);
	sendCmd(1 + 6*col);
	sendCmd(126);
	sendCmd(SSD1306_PAGEADDR);
	sendCmd(row);
	sendCmd(row);

	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, LOW);
    while (*msg) {
        f = 20*(*msg-32);
    	for (int i=0; i<10; i++)
    		SPI.transfer(pgm_read_byte_near(font2 + f++));
		SPI.transfer(0x00);
		SPI.transfer(0x00);
		msg++;
    }
	digitalWrite(ssPin, HIGH);

	msg = str;

	sendCmd(SSD1306_COLUMNADDR);
	sendCmd(1 + 6*col);
	sendCmd(126);
	sendCmd(SSD1306_PAGEADDR);
	sendCmd(row+1);
	sendCmd(row+1);

	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, LOW);
    while (*msg) {
        f = 20*(*msg-32)+10;
    	for (int i=0; i<10; i++)
    		SPI.transfer(pgm_read_byte_near(font2 + f++));
		SPI.transfer(0x00);
		SPI.transfer(0x00);
		msg++;
    }
	digitalWrite(ssPin, HIGH);
}

void ssd1306::sendCmd(uint8_t cmd) {
	digitalWrite(ssPin, LOW);
	digitalWrite(dcPin, LOW);
	SPI.transfer(cmd);
	digitalWrite(dcPin, HIGH);
	digitalWrite(ssPin, HIGH);
}

void ssd1306::begin() {
	pinMode (ssPin, OUTPUT);
	digitalWrite(ssPin, HIGH);

	pinMode (rstPin, OUTPUT);
	digitalWrite(rstPin, HIGH);
	delay(1);
	digitalWrite(rstPin, LOW);
	delay(1);
	digitalWrite(rstPin, HIGH);

	pinMode (dcPin, OUTPUT);
	digitalWrite(dcPin, HIGH);

	SPI.begin();

	sendCmd(SSD1306_DISPLAYOFF);                    // 0xAE
    sendCmd(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    sendCmd(0x80);                                  // the suggested ratio 0x80
    sendCmd(SSD1306_SETMULTIPLEX);                  // 0xA8
    sendCmd(0x3F);
    sendCmd(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    sendCmd(0x0);                                   // no offset
    sendCmd(SSD1306_SETSTARTLINE | 0x0);            // line #0
    sendCmd(SSD1306_CHARGEPUMP);                    // 0x8D
    sendCmd(0x14);
    sendCmd(SSD1306_MEMORYMODE);                    // 0x20
    sendCmd(0x00);                                  // 0x0 act like ks0108
    sendCmd(SSD1306_SEGREMAP | 0x1);
    sendCmd(SSD1306_COMSCANDEC);
    sendCmd(SSD1306_SETCOMPINS);                    // 0xDA
    sendCmd(0x12);
    sendCmd(SSD1306_SETCONTRAST);                   // 0x81
    sendCmd(0xCF);
    sendCmd(SSD1306_SETPRECHARGE);                  // 0xd9
    sendCmd(0xF1);
    sendCmd(SSD1306_SETVCOMDETECT);                 // 0xDB
    sendCmd(0x40);
    sendCmd(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    sendCmd(SSD1306_NORMALDISPLAY);                 // 0xA6
}
