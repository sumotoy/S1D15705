#ifndef S1D15705__H
#define S1D15705__H


#include "Arduino.h"

#define _abs(x) ((x<0)?-x:x)

class S1D15705{
	public:
	//8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
		S1D15705(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,uint8_t d4,uint8_t d5,uint8_t d6,uint8_t d7,uint8_t cs,uint8_t a0,uint8_t wr,uint8_t rd,uint8_t rst);
		void 		begin(void);
		void 		setPixel(uint8_t x,uint8_t y,uint8_t mode);
		void 		drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t mode);
		void		test();
	private:
		void		LCD_reset(void);
		void 		command_write(byte cmd);
		void 		lcdWait(void);
		void 		data_write(byte dat);
		byte		data_read(void);
		void 		sendData(byte data);
		uint8_t _res;
		uint8_t _wr;
		uint8_t _rd;
		uint8_t _a0;
		uint8_t _cs1;
		uint8_t _dataPins[8];
};

#endif
