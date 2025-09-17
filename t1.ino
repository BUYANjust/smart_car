#include "MPU9250.h"
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include "MPU9250.h"
#define Trig A3 //引脚Tring 连接 IO D2
#define Echo 4 //引脚Echo 连接 IO D3 
float distance; //距离变量
float temp; //存储回波

MPU9250 mpu;
int gyro[16];
int i=0;
int step1=0;
int step2=1;
int step3=0;
//新建一个softSerial对象，rx:6,tx:5
SoftwareSerial softSerial1(7,8);
SoftwareSerial softSerial2(A6,9);
float Roll = 0;//陀螺仪数据暂存
 
int SENSOR1=12; //传感器
int SENSOR2=13;
int SENSOR3=10;
int SENSOR4=A0;
int SENSOR5=A1;
int SENSOR6=A2;

int sen1 = 0;  // 读取红外传感器数值
int sen2 = 0; 
int sen3 = 0;  
int sen4 = 0;  
int sen5 = 0;  
int sen6 = 0;  

// 定义直流电机引脚
#define MOTOR1_PIN1 3
#define MOTOR1_PIN2 5
#define MOTOR2_PIN1 11
#define MOTOR2_PIN2 6



LiquidCrystal_I2C lcd(0x27,16,2); //屏幕配置
int lcdtime = 0;

int turn_adjust_ok=0;

int Line_Holdtime = 20; //巡线超时时间      毫秒
int Line_Delaytime = 1000; //巡到线后等待时间  毫秒
int Count_Holdtime = 20; //数线消抖时间     毫秒
unsigned char Line_Count_start =0; //数线开关
unsigned char Line_Count_Busy = 0; //数线程序忙碌状态
unsigned char Linetracker_Right_State = 0;
unsigned char Linetracker_Left_State = 0;
int Line_Count_Time = 0; 
int Line_Count =0; //数线结果
int left_time;
int left_time_busy = 0;
int left_count=0;

int delay_busy = 0;
int delay_time_temporary = 0;

// int rise_number;
// int descend_number;
// int Pitch_change;
// int last_Pitch;
// int Pitch_count;
float Pitch;

int A_time;
int B_time;
int C_time;
int D_time;

void setup() {
    // 初始化串口通信:
  //Serial.begin(9600);
  //初始化软串口通信；
  Wire.begin();
  Serial.begin(115200);
  softSerial1.begin(9600);
  softSerial2.begin(9600);
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);
  pinMode(SENSOR6, INPUT_PULLUP);
  
  pinMode(2, INPUT_PULLUP); //启动1按钮

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  
  mpu.setup(0x68);

  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮
  lcd.setCursor(0,0);                //设置显示指针
  lcd.print("Carprogram");
 

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(8,OUTPUT);
}

void loop() {
  // button();
  digitalWrite(Trig, LOW); //给Trig发送一个低电平（初始化）
  delayMicroseconds(2);    //等待 2微妙
  digitalWrite(Trig,HIGH); //给Trig发送一个高电平
  delayMicroseconds(10);    //等待 10微妙
  digitalWrite(Trig, LOW); //给Trig发送一个低电平
  
  temp = float(pulseIn(Echo, HIGH)); //存储回波等待时间,
  //pulseIn函数会等待引脚变为HIGH,开始计算时间,再等待变为LOW并停止计时
  //返回脉冲的长度
  
  //声速是:340m/1s 换算成 34000cm / 1000000μs = 34 / 1000
  //因为发送到接收，是一来一回，我们只要单程就好了，所以➗2
  //距离(厘米)  =  (回波时间 * (34 / 1000)) / 2
  //简化后的计算公式为 (回波时间 * 17)/ 1000
  
  distance = (temp * 17 )/1000; //把回波时间换算成cm
  
  if (mpu.update()) {
      static uint32_t prev_ms = millis();
      if (millis()-prev_ms >  20 ) {
          prev_ms = millis();
  //         Roll_count++;
  //     if(Roll_count == 1)
  //     {last_Roll = mpu.getRoll();}
  //     else if(Roll_count==4){
        Roll = mpu.getRoll();           
  //       change=Roll-last_Roll;
  //       Roll_count=0;
  //       }     
  //     }      
  //     if(millis()-prev_ms > 25 ){
  //         prev_ms = millis();
  //         Pitch_count++;
  //     if(Pitch_count==1)
  //     {last_Pitch = mpu.getPitch();}
  //     if(Pitch_count==20){
        Pitch = mpu.getPitch();
  //       Pitch_change=Pitch-last_Pitch;
  //       Pitch_count=0;
          }
      }      
  //   }
  if (millis()-left_time>20) {
          left_time = millis();
          if((sen1== 1 && sen2 ==0 && sen3 == 0 && sen5 == 0 && sen6 == 0)
          or(sen1== 1 && sen2 ==1 && sen3 == 0 && sen5 == 0 && sen6 == 0)
          or(sen1== 1 && sen2 ==1 && sen3 == 1 && sen5 == 0 && sen6 == 0)
          or(sen1== 0 && sen2 ==1 && sen3 == 1 && sen5 == 0 && sen6 == 0))
          //or(sen1== 0 && sen2 ==1 && sen3 == 0 && sen5 == 0 && sen6 == 0))
        {
          left_count++;
          }

      }  

  if (millis()-lcdtime >= 200) //屏幕刷新程序
    {
      lcd.setCursor(0,0);  //设置显示指针
      //lcd.print("                ");//清空第一行   
      lcd.setCursor(0,0);
      lcd.print("sen");     //输出字符到LCD1602上
      lcd.print(sen1);   
      lcd.print(sen2); 
      lcd.print(sen3); 
      lcd.print(sen4);  
      lcd.print(sen5);   
      lcd.print(sen6);
      lcd.print("L=");        
      lcd.print(Line_Count);
      lcd.print("S=");        
      lcd.print(step3);

      lcd.setCursor(0,1);                //设置显示指针
      lcd.print("R");
      lcd.print(Roll);     //输出字符到LCD1602上
      //lcd.print(mpu.getRoll());
      lcd.print("P="); 
      lcd.print(Pitch);
      lcd.print("D=");
      lcd.print(distance);
      
      
      lcd.print("     ");//清空第二行
      lcdtime = millis();
    }
    

    // if(step1==1){
    //   xunxian();
    //   time_delay(800);
    //   if(delay_busy==0)
    //   { step1=2; 
    //       likaiqidian();
    //   }
    //   }

    // // if(step1==1){
    // //   xunxian();
    // //   Line_Count_start=1;
    // //   shuxian();
    // //   if(Line_Count==1)  
    // //   {
    // //     likaiqidian();
    // //     Line_Count=0;
    // //     left_count=0;
    // //     step1=2;
    // //   }
    // // }
    // else if(step1==2){
    //   xunxian();
    //   if(left_count>=2)  
    //   {
    //    zhuanwanqingzhuyi();
    //    left_count=0;
    //    step2=3;
    //   }
    // }
    // else if(step1==3){
    //   xunxian();
    //   if(left_count>=2)  
    //   {
    //    zhuanwanqingzhuyi();
    //    left_count=0;
    //    step2=4;
    //   }
    // }
    // else if(step1==4){
    //   xunxian();
    //   if(left_count>=2)  
    //   {
    //    zhuanwanqingzhuyi();
    //    left_count=0;
    //    step2=5;
    //   }
    // }
    // else if(step1==4){
    //   xunxian();
    //   if(left_count>=2)  
    //   {
    //    zhuanwanqingzhuyi();
    //    left_count=0;
    //    Line_Count=0;
    //    step2=5;
    //   }
    // }
    // else if(step1==5){
    //   xunxian();
    //   Line_Count_start=1;
    //   shuxian();
    //   if(Line_Count==1)  
    //   {
    //     daodazhongdian();
    //     Line_Count=0;
    //     left_count=0;
    //     analogWrite(MOTOR1_PIN1, 0);
    //     analogWrite(MOTOR1_PIN2, 0);
    //     analogWrite(MOTOR2_PIN1, 0);
    //     analogWrite(MOTOR2_PIN2, 0);
    //     step1=6;
    //   }
    // }
    if(step2==1){
      xunxian();
      time_delay(800);
      if(delay_busy==0)
      { step2=2; 
          likaiqidian();
      }
      }
    if(step2==2)
    {
    time_delay(500);
    xunxian();
      if(delay_busy==0)
      {step2=2.1; 
      left_count=0;}

    }
    if(step2==2.1){
      xunxian();
      if(left_count >= 2)  
      {
        zhuanwanqingzhuyi();
        D_time=millis();
        softSerial2.println("D_time:");
        softSerial2.print(D_time/10);
          softSerial2.println("s");
        step2=3;
      }      
    }
    if(step2==3){
      xunxian();
//  if (Roll != Last_Roll)
// {
  if(Roll>14){
  sangzaiqiao();    
    step2=4;
  
}
}
    if(step2==4){
     mpu_run_rise();
  if(Roll<=-5 ){
    likaizhaiqiao();    
    step2=5;
  }
    }

  if(step2==5){
  xunxian_zaiqiao();
  time_delay(3000);
  if(delay_busy==0)
  {left_count=0;
  step2=6;}
  }  

  if (step2==6)
  {
    xunxian_zaiqiao();
    if(left_count==2){
    zhuanwanqingzhuyi();
    A_time=millis()-D_time;
    softSerial2.print("A_time:");
    softSerial2.println(A_time/1000);
    softSerial2.println("s");
    step2=8;
  }
  }

//   if(step2==6){
//     car_motor(0,255,255);
//     time_delay(1500);
//     if(delay_busy==0){
//       step2=7;
//     }
// } 
//   if(step2==7){
//     car_motor(200,0,200);
//     time_delay(500);
//     if(delay_busy==0){
//       step2=8;
//     }
// }
if(step2==8){
 xunxian();
 if(Roll>14){
   sangjieti();
   step2=9;
 }
} 
if(step2==9){
 xunxian();
 if(Roll<-4){
   likaijieti();
   left_count=0;
   step2=10;
 }
} 
if(step2==10){
 xunxian_new();
 if(left_count==2){
    zhuanwanqingzhuyi();
    B_time=millis()-A_time;
        softSerial2.println("B_time:");
        softSerial2.print(B_time/1000);
          softSerial2.println("s");
     left_count = 0;
     step2=11;
 }
  } 
 
if(step2==11){
xunxian_new();
time_delay(8000);
if(delay_busy==0){
  left_count = 0;
  step2=12;
    }
}

if(step2==12){
    xunxian();
  if( left_count==2){
    zhuanwanqingzhuyi();
    C_time=millis()-B_time;
        softSerial2.println("C_time:");
        softSerial2.print(C_time/1000);
        softSerial2.println("s");
    Line_Count = 0;
    step2=13;
    
}
}
if(step2==13){
      Line_Count_start=1;
      shuxian();
      xunxian();
       if(Line_Count==1)  {
         daodazhongdian();
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 0);
        softSerial2.println("total time:");
        softSerial2.print(millis()/1000);
        softSerial2.println("s");

      step2=14;
       }

    
}
if(step3==1){
xunxian();  
  if(distance<=27){
    step3=2;}
}
if(step3==2){
  car_motor_new(0,30,130,0);
   time_delay(800);
  if(delay_busy==0){
  step3=3;
  }
  }

  if(step3==3){
  car_motor_new(130,0,130,0);
   time_delay(3000);
  if(delay_busy==0){
  step3=4;
  }
  }
  if(step3==4){
    car_motor_new(30,0,0,130);
    time_delay(2500);
    if(delay_busy==0){
    step3=5;
  }
  }
  if (step3==5){
xunxian();
}

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
void likaizhaiqiao(){
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
void sangjieti(){
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

void time_delay(int delay_time)// 延迟函数
{
  if (millis()-delay_time_temporary >= delay_time) 
      {
        if (delay_busy == 0)
        {
        delay_time_temporary=millis();
        delay_busy = 1;
        }
        else if(delay_busy == 1)
        {
        delay_busy = 0;
        delay_time_temporary=0;
        }
      }
}

void shuxian()
{
  if(digitalRead(SENSOR1)==1 and digitalRead(SENSOR2)==1 and digitalRead(SENSOR5)==1 and digitalRead(SENSOR6)==1 and Line_Count_Busy==0)
  {Line_Count_Busy=1;
  Line_Count_Time = millis();
  Line_Count++;
  }
  if(Line_Count_Busy ==1 and millis()-Line_Count_Time>= Line_Delaytime)
  {Line_Count_Busy=0;}
  //SENSOR1 左 SENSOR6 右
//  if(Line_Count_Busy == 0 and Line_Count_start == 1)
//   {
//     if(digitalRead(SENSOR1)==1 and digitalRead(SENSOR2)==1)
//     {
//       Linetracker_Left_State = 1;
//       Line_Count_Busy = 1; 
//       Line_Count_Time = millis();
//     }
//     if(digitalRead(SENSOR1)==1 and digitalRead(SENSOR2)==1 and Line_Count_Busy == 1) //若同时识别到线
//     {
//       Linetracker_Right_State = 1;
//     }
//     else if(digitalRead(SENSOR5)==1 and digitalRead(SENSOR6)==1 and Line_Count_Busy == 0) //若左侧没识别到线，右侧识别到
//     {
//       Linetracker_Right_State = 1;
//       Line_Count_Busy = 1; 
//       Line_Count_Time = millis();
//     }
//   }
//   else if (Line_Count_Busy == 1 and Line_Count_start == 1)
//   {
//     if(millis()-Line_Count_Time >= Line_Delaytime and Linetracker_Right_State == 1 and Linetracker_Left_State == 1) 
//     {
//       Line_Count++;
//       Line_Count_Busy = 2;
//     }
//     else if (millis()-Line_Count_Time >= Count_Holdtime and (Linetracker_Right_State == 0 or Linetracker_Left_State == 0))
//     {
//       Line_Count_Busy = 0;
//       Linetracker_Left_State = 0;
//       Linetracker_Right_State = 0;
//     }
//   }
//   if (Line_Count_Busy == 2 and (digitalRead(SENSOR1)==0 or digitalRead(SENSOR6)==0))
//   {
//     Line_Count_Busy = 0;
//     Linetracker_Left_State = 0;
//     Linetracker_Right_State = 0;
//   }
}
void xunxian() //巡线函数
{
  sen1 = digitalRead(SENSOR1);  // 读取红外传感器数值
  sen2 = digitalRead(SENSOR2);  // 读取红外传感器数值
  sen3 = digitalRead(SENSOR3);  // 读取红外传感器数值
  sen4 = digitalRead(SENSOR4);  // 读取红外传感器数值
  sen5 = digitalRead(SENSOR5);  // 读取红外传感器数值
  sen6 = digitalRead(SENSOR6);  // 读取红外传感器数值
  // 如果左侧传感器检测到黑线，右侧传感器检测到白线，向左转
  if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 0 && sen6== 0 ) //直走
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==1 && sen3 == 1 && sen4 ==1 && sen5 == 0 && sen6== 0) //小左
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 20);
    analogWrite(MOTOR2_PIN2, 0);
  }
    else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 1 && sen5 == 0 && sen6== 0) //中左0
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 70);
  }
    else if (sen1== 0 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左0
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 70);
  }
  else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左1
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 120);
  }
  else if (sen1== 1 && sen2 ==1 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左2
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 220);
  }
  else if (sen1== 1 && sen2 ==0 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //大左
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 255);
    // zhuanwanqingzhuyi();
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 0) //小右
  {
    analogWrite(MOTOR1_PIN1, 120);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 1) //中右
  {
    analogWrite(MOTOR1_PIN1, 80);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==1 && sen5 == 1 && sen6== 1)//中右1
 {
    analogWrite(MOTOR1_PIN1,30);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 1 && sen6== 1)//中右2
 {
    analogWrite(MOTOR1_PIN1, 10);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 0 && sen6== 1) //大右
  {
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
    //zhuanwanqingzhuyi();
  }
  else { //没有检测到黑线
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
}

void xunxian_zaiqiao() //巡线函数
{
  sen1 = digitalRead(SENSOR1);  // 读取红外传感器数值
  sen2 = digitalRead(SENSOR2);  // 读取红外传感器数值
  sen3 = digitalRead(SENSOR3);  // 读取红外传感器数值
  sen4 = digitalRead(SENSOR4);  // 读取红外传感器数值
  sen5 = digitalRead(SENSOR5);  // 读取红外传感器数值
  sen6 = digitalRead(SENSOR6);  // 读取红外传感器数值
  // 如果左侧传感器检测到黑线，右侧传感器检测到白线，向左转
  if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 0 && sen6== 0 ) //直走
  {
    analogWrite(MOTOR1_PIN1, 150);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 150);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==1 && sen3 == 1 && sen4 ==1 && sen5 == 0 && sen6== 0) //小左
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 50);
  }
    else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 1 && sen5 == 0 && sen6== 0) //中左0
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 80);
  }
    else if (sen1== 0 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左0
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 80);
  }
  else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左1
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 120);
  }
  else if (sen1== 1 && sen2 ==1 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左2
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 230);
  }
  else if (sen1== 1 && sen2 ==0 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //大左
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 255);
    // zhuanwanqingzhuyi();
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 0) //小右
  {
    analogWrite(MOTOR1_PIN1, 120);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 1) //中右
  {
    analogWrite(MOTOR1_PIN1, 80);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==1 && sen5 == 1 && sen6== 1)//中右1
 {
    analogWrite(MOTOR1_PIN1,30);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 1 && sen6== 1)//中右2
 {
    analogWrite(MOTOR1_PIN1, 10);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 0 && sen6== 1) //大右
  {
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
    //zhuanwanqingzhuyi();
  }
    else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 1 && sen5 == 1 && sen6== 1) //全黑
  {
    analogWrite(MOTOR1_PIN1, 120);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 120);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else { //没有检测到黑线
    analogWrite(MOTOR1_PIN1, 120);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 120);
    analogWrite(MOTOR2_PIN2, 0);
  }
}
void xunxian_new() //巡线函数
{
  sen1 = digitalRead(SENSOR1);  // 读取红外传感器数值
  sen2 = digitalRead(SENSOR2);  // 读取红外传感器数值
  sen3 = digitalRead(SENSOR3);  // 读取红外传感器数值
  sen4 = digitalRead(SENSOR4);  // 读取红外传感器数值
  sen5 = digitalRead(SENSOR5);  // 读取红外传感器数值
  sen6 = digitalRead(SENSOR6);  // 读取红外传感器数值
  // 如果左侧传感器检测到黑线，右侧传感器检测到白线，向左转
  if (sen1== 1 & sen2 ==1 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 1) //直走
  {
    analogWrite(MOTOR1_PIN1, 75);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 75);
    analogWrite(MOTOR2_PIN2, 0);
  }
 else if (sen1== 0 or sen2 ==0 or sen3 == 0 or sen4 ==0 or sen5 == 0 or sen6== 0) //小左
  {
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 0);
  }
//     else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 1 && sen5 == 0 && sen6== 0) //中左0
//   {
//     analogWrite(MOTOR1_PIN1, 255);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//     else if (sen1== 0 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左0
//   {
//     analogWrite(MOTOR1_PIN1, 255);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左1
//   {
//     analogWrite(MOTOR1_PIN1, 255);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 1 && sen2 ==1 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //中左2
//   {
//     analogWrite(MOTOR1_PIN1, 255);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 1 && sen2 ==0 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0) //大左
//   {
//     analogWrite(MOTOR1_PIN1, 255);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//     // zhuanwanqingzhuyi();
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 0) //小右
//   {
//     analogWrite(MOTOR1_PIN1, 80);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 120);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1 && sen6== 1) //中右
//   {
//     analogWrite(MOTOR1_PIN1, 60);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 100);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==1 && sen5 == 1 && sen6== 1)//中右1
//  {
//     analogWrite(MOTOR1_PIN1,20);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 100);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 1 && sen6== 1)//中右2
//  {
//     analogWrite(MOTOR1_PIN1, 10);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 100);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 0 && sen6== 1) //大右
//   {
//     analogWrite(MOTOR1_PIN1, 0);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 100);
//     analogWrite(MOTOR2_PIN2, 0);
//     //zhuanwanqingzhuyi();
//   }
//     else if (sen1== 1 && sen2 ==1 && sen3 == 1 && sen4 == 1 && sen5 == 1 && sen6== 1) //全黑
//   {
//     analogWrite(MOTOR1_PIN1, 75);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 75);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen1== 0 && sen2 ==0 && sen3 == 0 && sen4 == 0 && sen5 == 0 && sen6== 0)
//   { //没有检测到黑线
//     analogWrite(MOTOR1_PIN1, 150);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else
//   {
//     analogWrite(MOTOR1_PIN1, 150);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
}

// void turn_adjust() //旋转调整函数
// {
//   int sen1 = digitalRead(SENSOR1);  // 读取红外传感器数值
//   int sen2 = digitalRead(SENSOR2);  // 读取红外传感器数值
//   int sen3 = digitalRead(SENSOR3);  // 读取红外传感器数值
//   int sen4 = digitalRead(SENSOR4);  // 读取红外传感器数值
//   int sen5 = digitalRead(SENSOR5);  // 读取红外传感器数值
//   int sen6 = digitalRead(SENSOR6);  // 读取红外传感器数值
//   if ((sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 0) or (sen2 ==0 && sen3 == 1 && sen4 ==1 && sen5 == 1))
//   //or (sen2 ==1 && sen3 == 1 && sen4 ==1 && sen5 == 0) )//判断可用
  
//   {
//   turn_adjust_ok=1;    
//     analogWrite(MOTOR1_PIN1, 0);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen2 ==0 && sen3 == 1 && sen4 ==0 && sen5 == 0) //判断可用X 向左转
//   {
//   turn_adjust_ok=1;    
//     analogWrite(MOTOR1_PIN1, 0);
//     analogWrite(MOTOR1_PIN2, 50);
//     analogWrite(MOTOR2_PIN1, 50);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen2 ==1 && sen3 == 1 && sen4 ==0 && sen5 == 0) //向左转
//   { 
//     analogWrite(MOTOR1_PIN1, 0);
//     analogWrite(MOTOR1_PIN2, 50);
//     analogWrite(MOTOR2_PIN1, 50);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen2 ==1 && sen3 == 0 && sen4 ==0 && sen5 == 0) //向左转
//   { 
//     analogWrite(MOTOR1_PIN1, 0);
//     analogWrite(MOTOR1_PIN2, 50);
//     analogWrite(MOTOR2_PIN1, 50);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//   else if (sen2 ==0 && sen3 == 0 && sen4 ==1 && sen5 == 0) //判断可用X 向右转
//   { turn_adjust_ok=1; 
//     analogWrite(MOTOR1_PIN1, 50);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 50);
//   }
//   else if (sen2 ==0 && sen3 == 0 && sen4 ==1 && sen5 == 1 )//向右转
//  {
//     analogWrite(MOTOR1_PIN1, 50);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 50);
//   }
//   else if (sen2 ==0 && sen3 == 0 && sen4 ==0 && sen5 == 1 ) //向右转
//   {
//     analogWrite(MOTOR1_PIN1, 50);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 0);
//     analogWrite(MOTOR2_PIN2, 50);
//   }
// }

void car_motor(int left_speed1,int left_speed2,int right_speed){
    analogWrite(MOTOR1_PIN1, right_speed);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, left_speed1);
    analogWrite(MOTOR2_PIN2, left_speed2);
}

void mpu_run_rise(){
  if (Pitch<-6){
    analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255*0.85);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else if (Pitch>-3){
    analogWrite(MOTOR1_PIN1, 255*0.85);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);
  }
  else
  { analogWrite(MOTOR1_PIN1, 255);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 255);
    analogWrite(MOTOR2_PIN2, 0);}
}

void car_motor_new(int left_speed1,int left_speed2,int right_speed1,int right_speed2){
    analogWrite(MOTOR1_PIN1, right_speed1);
    analogWrite(MOTOR1_PIN2, right_speed2);
    analogWrite(MOTOR2_PIN1, left_speed1);
    analogWrite(MOTOR2_PIN2, left_speed2);
}
// void mpu_run_descend(){
//   if (Pitch_change>1.5){
//     analogWrite(MOTOR1_PIN1, 120);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 120*0.2);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
//     if (Pitch_change<-2){
//     analogWrite(MOTOR1_PIN1, 120*0.2);
//     analogWrite(MOTOR1_PIN2, 0);
//     analogWrite(MOTOR2_PIN1, 120);
//     analogWrite(MOTOR2_PIN2, 0);
//   }
// }
void button(){
 int n = digitalRead(1);
 if (step1 == 0 and n == 0)
 {
  step1=1;
  delay(1500);
 }

else if (step2 == 0 and step1 == 6 and n == 0)
 {
  step2=1;
  delay(1500);
 }
}