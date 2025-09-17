#include<SoftwareSerial.h>
//新建一个softSerial对象，rx:6,tx:5
SoftwareSerial softSerial1(7,8);
 
void setup() {
  // 初始化串口通信:
  Serial.begin(9600);
  //初始化软串口通信；
  softSerial1.begin(9600);
}
void likaiqidian(){     
  unsigned char li = 0;
unsigned char head[14];

head[0] = 0xFD;
head[1] = 0x00;
head[2] = 0x0B;
head[3] = 0x01;
head[4] = 0x00;
head[5] = 0xC0;
head[6] = 0xEB;
head[7] = 0xBF;
head[8] = 0xAA;
head[9] = 0xC6;
head[10] = 0xF0;
head[11] = 0xB5;
head[12] = 0xE3;
head[13] = 0xA9;

  for(li=0; li<14; li++){
    softSerial1.write(head[li]);
  }
}

void daodazhongdian(){
  unsigned char i = 0;
  unsigned char head[14];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x0B;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xB5;
  head[6] = 0xBD;
  head[7] = 0xB4;
  head[8] = 0xEF;
  head[9] = 0xD6;
  head[10] = 0xD5;
  head[11] = 0xB5;
  head[12] = 0xE3;
  head[13] = 0xF1;

  for(i=0; i<14; i++){
    softSerial1.write(head[i]);
  }
}
void sangzaiqiao(){
  unsigned char i = 0;
  unsigned char head[12];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x09;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xC9;
  head[6] = 0xCF;
  head[7] = 0xD5;
  head[8] = 0xAD;
  head[9] = 0xC7;
  head[10] = 0xC5;
  head[11] = 0x89;

  for(i=0; i<12; i++){
    softSerial1.write(head[i]);
  }
}
void likaizhaiqizo(){
  unsigned char i = 0;
  unsigned char head[14];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x0B;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xC0;
  head[6] = 0xEB;
  head[7] = 0xBF;
  head[8] = 0xAA;
  head[9] = 0xD5;
  head[10] = 0xAD;
  head[11] = 0xC7;
  head[12] = 0xC5;
  head[13] = 0xB3;

  for(i=0; i<14; i++){
    softSerial1.write(head[i]);
  }
}
void sahngjieti(){
  unsigned char i = 0;
  unsigned char head[12];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x09;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xC9;
  head[6] = 0xCF;
  head[7] = 0xBD;
  head[8] = 0xD7;
  head[9] = 0xCC;
  head[10] = 0xDD;
  head[11] = 0x88;

  for(i=0; i<12; i++){
    softSerial1.write(head[i]);
  }
}
void likaijieti(){
  unsigned char i = 0;
  unsigned char head[14];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x0B;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xC0;
  head[6] = 0xEB;
  head[7] = 0xBF;
  head[8] = 0xAA;
  head[9] = 0xBD;
  head[10] = 0xD7;
  head[11] = 0xCC;
  head[12] = 0xDD;
  head[13] = 0xB2;

  for(i=0; i<14; i++){
    softSerial1.write(head[i]);
  }
}
void zhuanwanqingzhuyi(){
  unsigned char i = 0;
  unsigned char head[16];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x0D;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xD7;
  head[6] = 0xAA;
  head[7] = 0xCD;
  head[8] = 0xE4;
  head[9] = 0xC7;
  head[10] = 0xEB;
  head[11] = 0xD7;
  head[12] = 0xA2;
  head[13] = 0xD2;
  head[14] = 0xE2;
  head[15] = 0xCC;

  for(i=0; i<16; i++){
    softSerial1.write(head[i]);
  }
}

void loop() {
likaiqidian();
delay(3000);
daodazhongdian();
delay(3000);
sangzaiqiao();
delay(3000);
likaizhaiqizo();
delay(3000);
sahngjieti();
delay(3000);
likaijieti();
delay(3000);
zhuanwanqingzhuyi();
delay(3000);


}