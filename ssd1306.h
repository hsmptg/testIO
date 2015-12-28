/*
 * ssd1306.h
 *
 *  Created on: 26/09/2015
 *      Author: Helio
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "Arduino.h"
#include "SPI.h"
#include "font.h"

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

class ssd1306 {
    public:
		uint8_t buf[8*126];
		ssd1306();
		void begin();
        void clear();
        void display(boolean on);
        void invert(boolean inv);
        void printBuf();
        void printAt(int row, int col, char *msg);
        void print2At(int row, int col, char *msg);
        void printAtBuf(int row, int col, char *msg);
        void print2AtBuf(int row, int col, char *msg);
    private:
        void sendCmd(uint8_t cmd);
};

#endif /* SSD1306_H_ */
