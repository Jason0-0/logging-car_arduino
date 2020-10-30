//#include "chassis.h"
//#include "motor.h"
#include "Arm.h"
#include "Servo.h"

int setPos=45;
int nowPos=45;
bool flag=false;
Arm top;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  top.servoInit(4,6,7,10,11);
  delay(500);
  
}


void loop() {

  if(flag==true)
  {
    Serial.println("1. turn wrist.");
    top.moveWrist(59);
    delay(1000);
    Serial.println("2. move arms down.");
    Serial.println("moving...");
     for(int i=90;i>=-10;i--)
    {
      Serial.println(i);
      top.moveArm(i);
      delay(10);
    }
    //top.moveArm(-7);
    delay(3000);
    Serial.println("3. grab!");
    top.grabIt();
    delay(1000);
    Serial.println("4. rise arms.");
    top.moveArm(13);
    delay(1000);
    Serial.println("5. turn wrist.");
    top.moveWrist(225);
    delay(1000);
    Serial.println("6. down arms.");
    top.moveArm(3);
    delay(1000);
    Serial.println("7. place.");
    top.loosenIt();
    delay(2000);
    Serial.println("8. rise arms.");
    top.moveArm(13);
    delay(1000);
    Serial.println("9. turn wrist.");
    top.moveWrist(59);
    delay(1000);
    Serial.println("10. turn platform and down arms.");
    top.turnTo((char)2);
    top.moveArm(-12);
    delay(500);
    Serial.println("3. grab!");
    top.grabIt();
    delay(1000);
    Serial.println("4. rise arms.");
    top.moveArm(13);
    delay(1000);
    Serial.println("5. turn wrist.");
    top.moveWrist(225);
    delay(1000);
    Serial.println("6. down arms.");
    top.moveArm(3);
    delay(1000);
    Serial.println("7. place.");
    top.loosenIt();
    delay(2000);
  }
  
  Serial.println("0. reset.");
  top.moveArm(90);
  top.moveWrist(TOP_ZERO_ANGLE);
  top.turnTo((char)1);
  top.loosenIt();
  delay(2000);
  flag=false;
//top.loosenIt();

//if(setPos==666)
//{
//  Serial.println("moving");
//   for(int i=90;i>=5;i--)
//  {
//    Serial.println(i);
//    top.moveArm(i);
//    delay(5);
//  }
//  setPos=nowPos;
//  flag=false;
//}
//
//if(setPos==999)
//{
//  top.grabIt();
//  flag=false;
//}
//else if(setPos==996)
//{
//  top.loosenIt();
//  flag=false;
//}
//
//if(setPos>=300&&setPos<=600)
//{
//  top.moveArm(setPos-315);
//  Serial.print("arm: ");
//  Serial.println(setPos-315);
//  flag=false;
//}
//
////top.moveWrist(45);
//if(flag)
//{
//  nowPos=setPos;
//  Serial.println(nowPos);
//  flag=false;
//}
//  //top.moveArm(nowPos);
//  top.moveWrist(nowPos);


}

void serialEvent()
{
  int ch=0;
//  int buf=0;
//  while(Serial.available())
//  {
//    buf=buf*10+((char)Serial.read()-'0');
//    delay(2);
//  }
//  setPos=buf;
  //Serial.println(buf);
  while(Serial.available())
  {
    ch=(char)Serial.read();
  }
  if(ch=='b')
    flag=true;
  else if(ch=='s')
    flag=false;
  
}
