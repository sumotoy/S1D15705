#include <S1D15705.h>

// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
S1D15705 lcd = S1D15705(8,9,10,11,4,5,6,7,14,15,17,18,16);

void setup() {
  Serial.begin(38400);
  lcd.begin();
}

void loop() {
  //lcd.setPixel(random(0,128),random(0,64),random(0,1));
  lcd.test();
  delay(10);
}