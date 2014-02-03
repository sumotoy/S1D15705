
#include "S1D15705.h"

S1D15705::S1D15705(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,uint8_t d4,uint8_t d5,uint8_t d6,uint8_t d7,uint8_t cs,uint8_t a0,uint8_t wr,uint8_t rd,uint8_t rst){
	_dataPins[0] = d0;
	_dataPins[1] = d1;
	_dataPins[2] = d2;
	_dataPins[3] = d3;
	_dataPins[4] = d4;
	_dataPins[5] = d5;
	_dataPins[6] = d6;
	_dataPins[7] = d7;
	_cs1 = cs;
	_a0 = a0;
	_wr = wr;
	_rd = rd;
	_res = rst;
}

void S1D15705::begin(void){
	uint8_t i, j;
	for (i=0;i<8;i++){
		pinMode(_dataPins[i],OUTPUT);
		digitalWrite(_dataPins[i],HIGH);
	}
	pinMode(_cs1,OUTPUT);
	pinMode(_a0,OUTPUT);
	pinMode(_wr,OUTPUT);
	pinMode(_rd,OUTPUT);
	pinMode(_res,OUTPUT);
	const uint8_t initCommands[16]={0xE1,0xAB,0xA3,0xA0,0xC8,0x20,0x2B,0x26,0x81,0x13,0xAF,0xA7,0x42,0xB0,0x10,0x03};
	//init gpio;
	//sendData(0xFF);//write();
	digitalWrite(_res,LOW);
	digitalWrite(_res,HIGH);
	for(i=0;i<16;i++) {
		command_write(initCommands[i]);
	}
	for(j=0xB0;j<0xB9;j++){
		 command_write(j);
		 command_write(0x10);
		 command_write(0x03);
		 for(i=0;i<162;i++){
			data_write(0x00);
		 }
	}
	
	command_write(0x40);	//start line = 0 

	command_write(0xA2);	//0b10100010 BIAS 1/9
	command_write(0x81);	//0b10000001 ELECTRIC CONTRAST
	command_write(0x12);	//0b00010010 data for EC
	command_write(0xA3);	//0b10100011 BIAS 1/7
	Serial.println("inited");
}

void S1D15705::LCD_reset(void){
	digitalWrite(_rd,HIGH);
	digitalWrite(_wr,LOW);
	sendData(0xE2);
	digitalWrite(_res,LOW);
	digitalWrite(_cs1,LOW);
	delay(5);
	digitalWrite(_cs1,HIGH);
	digitalWrite(_res,HIGH);
}



void S1D15705::command_write(byte cmd){
	digitalWrite(_res,LOW);
	digitalWrite(_rd,HIGH);
	digitalWrite(_wr,LOW);
	sendData(cmd);
	digitalWrite(_cs1,LOW);
	digitalWrite(_cs1,HIGH);
	digitalWrite(_wr,HIGH);
	sendData(0xFF);
}

void S1D15705::sendData(byte data){
	for (uint8_t i=0;i<8;i++){
		if (bitRead(data,i) == 1){
			digitalWrite(_dataPins[i],HIGH);
		} else {
			digitalWrite(_dataPins[i],LOW);
		}
	}
}


void S1D15705::lcdWait(void){
	//read command here(todo)
	digitalWrite(_res,LOW);
	digitalWrite(_wr,HIGH);
	digitalWrite(_rd,LOW);
	/*
	do {
		digitalWrite(_cs1,LOW);
		delay(2);
		digitalWrite(_cs1,HIGH);
	} while((PD7==1));
	*/
	//write;
	digitalWrite(_res,HIGH);
	digitalWrite(_wr,HIGH);
}

void S1D15705::data_write(byte dat){
	lcdWait();
	digitalWrite(_res,HIGH);
	digitalWrite(_rd,HIGH);
	digitalWrite(_wr,LOW);
	sendData(dat);
	digitalWrite(_cs1,LOW);
	digitalWrite(_cs1,HIGH);
	digitalWrite(_wr,HIGH);
	sendData(0xFF);
}


byte S1D15705::data_read(void){
	byte dat = 0;
  
	digitalWrite(_res,HIGH);
	digitalWrite(_wr,HIGH);

	digitalWrite(_cs1,LOW);
	digitalWrite(_cs1,HIGH);

	sendData(0xAA);
	//read command here(todo)

	digitalWrite(_rd,LOW);
  
	digitalWrite(_cs1,LOW);
	digitalWrite(_cs1,HIGH);

	digitalWrite(_cs1,LOW);
	delay(1);
	//data get here (todo)
	digitalWrite(_cs1,HIGH);
	digitalWrite(_rd,HIGH);
	//write
	return(dat);
}

void S1D15705::test(){
		delay(250);
		command_write(0xa6);
		delay(250);
		command_write(0xa7);
}

void S1D15705::setPixel(uint8_t x,uint8_t y,uint8_t mode){
	uint8_t x_h, x_l;
	uint8_t tmp;
	uint8_t page;

	x+=3;
	x_h = (x >> 4);
	x_l = x & 0x0F;
	page = y / 8; 
    tmp = y - (page * 8);  
    y = 0x01 << tmp;
    if (mode == 0) y = ~y;
    command_write(0x40);			
    command_write(0xB0 | page);		
    command_write(x_h|0x10);	
    command_write(x_l);

    tmp = data_read();		

    if (mode == 0){	
      y&=tmp;
    } else {
      y|=tmp;		
	}
    command_write(0xB0 | page);	
    command_write(x_h | 0x10);
    command_write(x_l);
    data_write(y);
}

void S1D15705::drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t mode){
	uint8_t x, y, dx, dy;
	float x_0, y_0, x_1, y_1, x_, y_;

	dx = _abs(x0-x1);
	dy = _abs(y0-y1);
	x_0 = x0;
	y_0 = y0;
	x_1 = x1;
	y_1 = y1;
	if (dx >= dy){
		for(x=x0;x<=x1;x++){
			x_ = x;
			y = ((x_ - x_0)/(x_1 - x_0)) * (y_1 - y_0) + y_0;
			setPixel(x,y,mode);
		}
	} else {
		for(y=y0;y<=y1;y++) {
			y_ = y;
			x = ((y_-y_0)/(y_1-y_0))*(x_1-x_0)+x_0;
			setPixel(x,y,mode);
		}
	}
}