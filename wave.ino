#include "MPU9250.h"

MPU9250 mpu;
int gyro[16];
int i=0;
int average1;
int average2;
int average3;
int average4;
int wave;
int tolerance;
int threshold;
void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
}

void get_wave(){
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25 && i< 21) {
            gyro[i]=mpu.getRoll();
            prev_ms = millis();
            i++;
            if(i==4){
            average1=(gyro[0]+gyro[1]+gyro[2]+gyro[3])/4;
            }
            if(i==8){
            average2=(gyro[4]+gyro[5]+gyro[6]+gyro[7])/4;
            }
            if(i==12){
            average3=(gyro[8]+gyro[9]+gyro[10]+gyro[11])/4;
            }
            if(i==16){
            average4=(gyro[12]+gyro[13]+gyro[14]+gyro[15])/4;
            }
            if(i==20){
              average_new=
             average1=average2;
             average2=average3;
             average3=average4;
             average4=average_new;
             i=14;
            }
        }
        if
    }
}

void judge_wave(){
if((average3-average2)>(average2-average1)+tolerance&&(average3-average2)>(average4-average3)+tolerance){
wave=2;
}
if((average3-average2)==(average2-average1)+tolerance&&(average3-average2)==(average4-average3)+tolerance){
wave=1;
}
}
void voice(){//有判断波形的判断（不全）
  if(wave==1){

  }
  if(wave==2){
    
  }
  if(wave==2 && shuxian==1){
    
  }
  if(wave==0 && shuxian==1){
    
  }
}

void judge(){//排除判断波形的判断（不全）
  if(average4-average1==threshold || average4-average1==-threshold){
    voice();
    step++;
  }
}
void loop(){

}





void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}


