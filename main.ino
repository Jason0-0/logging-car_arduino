/*  INCLUDE BEGIN */
#include "Arm.h"
#include "Servo.h"
#include "robot_movement.h"
#include <inttypes.h>
#include "chassis.h"
#include "motor.h"
/* INCLUDE END */

/* DEFINES BEGIN */

/* DEFINES END */

/* --------------static begin------------------------------------ */
char ch = 0;
//chassis
int chassis::kFB = 1;
int chassis::kLR = 1;
int chassis::kR = 1;
int flag_scan=0;//触发扫码标志
int data[5];

//camera
char camBuf[4]={0,0,0};
bool camFlag=false;

/* --------------------static end ------------------------------- */

/* -------------------const begin ------------------------------*/
//robot
const long arm_grab_angle = -10;
const long arm_place_angle = 3;
const long arm_smallRise_angle = 15; //微微抬起以避免夹爪舵机干涉
const long arm_camera_angle = 90;      //用到摄像头识别时抬起的角度
const long wrist_grab_angle = 59;
const long wrist_place_angle = 225;
const long wrist_camera_angle = 59;
const int delaytime = 300;
const int servoDelay = 10;
const long wrist_hold_angle=140; //两次连续夹取之间腕部停驻的角度（避免打到地上的和车上的物料
//chassis
const int sensorPin1 = 48; //传感器引脚
const int sensorPin2 = 47;
const int sensorPin3 = 40;
const int sensorPin4 = 41;
const int sensorPin5 = 33;
const int sensorPin6 = 32;
//camera
const int openmv=30;
/* ---------------------const end ---------------------------*/

/* ** class instance begin ** */

Arm top;

motor frontLeft_m(12, 34, 35, 18, 31); //要查查能不能用
motor frontRight_m(8, 37, 36, 19, 38);
motor backLeft_m(9, 43, 42, 3, 49);
motor backRight_m(5, 26, 27, 2, 23);

chassis Chassis_c(&frontLeft_m, &frontRight_m, &backLeft_m, &backRight_m);

/* ** class instance end ** */

void setup() {
  // Serial setup
  Serial.begin(9600);    //computer
  Serial2.begin(9600);  //camera

  //Pins Init
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  pinMode(sensorPin5, INPUT);
  pinMode(sensorPin6, INPUT);
  pinMode(openmv,OUTPUT);

  //Robot Init
  top.servoInit(4,6,7,10,11);
  delay(500); //wait for servos to reach setting places.

  //Serial test
  Serial.println("ready!");
  
}
void loop()
{
  
  

}


void serialEvent()
{
  
  //  int buf=0;
  //  while(Serial.available())
  //  {
  //    buf=buf*10+((char)Serial.read()-'0');
  //    delay(2);
  //  }
  //  setPos=buf;
  //Serial.println(buf);
  while (Serial.available())
  {
    ch = (char)Serial.read();
  }
  // if (ch == 'b')
  //   flag = true;
  // else if (ch == 's')
  //   flag = false;
}

// void firstGrab()
// {
//   //任务编号判断（待写）

//   //任务内容
//   Serial.println("1. turn wrist.");
//   top.moveWrist(wrist_grab_angle);
//   delay(delaytime);
//   Serial.println("2. move arms down.");
//   Serial.println("moving...");

//   //调速，todo:需要改起始角度的逻辑
//   for (int i = ARM_MAX_ANGLE; i >= arm_grab_angle; i--)
//   {
//     //Serial.println(i);
//     top.moveArm(i);
//     delay(servoDelay);
//   }
//   //top.moveArm(-7);
//   delay(delaytime);
//   Serial.println("3. grab!"); //夹取
//   top.grabIt();
//   delay(delaytime);

//   Serial.println("4. rise arms."); //提一个小角度便于越过干涉
//   top.moveArm(arm_smallRise_angle);
//   delay(delaytime);

//   Serial.println("5. turn wrist."); //放置
//   top.moveWrist(wrist_place_angle);
//   delay(delaytime);
//   Serial.println("6. down arms.");
//   top.moveArm(arm_place_angle);
//   delay(delaytime);
//   Serial.println("7. place.");
//   top.loosenIt();
//   delay(delaytime);

//   Serial.println("8. rise arms.");  //准备下一次夹取
//   top.moveArm(arm_smallRise_angle);
//   delay(delay);
//   Serial.println("9. turn wrist."); 
//   top.moveWrist(wrist_hold_angle);  //转到中间的角度
  
//   //整点骚操作（伪时序配合
//   delay(250);
//   top.turnTo((char)2);    //转到2号盘

//   //等底盘移到下一个位置
// }

// /**
//  * @brief 夹取第二个物料
//  * **********************************
//  * 调用该函数时各部分的预期状态：
//  * ** armAngle=arm_smallRise_angle;
//  * ** wristAngle=wrist_hold_angle;
//  * ** isGrab=false;
//  * ** platformID=2
//  * **********************************
//  */
// void secondGrab()
// {
//   //1.腕部转到夹取位置
//   top.moveWrist(wrist_grab_angle);
//   delay(delaytime);
//   //2.手臂放下
//   top.moveArm(arm_grab_angle);
//   delay(delaytime);
//   //3.夹取
//   top.grabIt();
//   delay(delaytime);
//   //4.放
//   top.moveArm(arm_smallRise_angle);
//   delay(delaytime);
//   top.moveWrist(wrist_place_angle);
//   delay(delaytime);
//   top.moveArm(arm_place_angle);
//   delay(delaytime);
//   top.loosenIt();
//   delay(delaytime);

//   top.moveArm(arm_smallRise_angle);
//   delay(delaytime); 
//   top.moveWrist(wrist_hold_angle);  //转到中间的角度
  
//   //整点骚操作（伪时序配合
//   delay(250);
//   top.turnTo((char)0);    //转到中间

//   //wait for next grab...
// }

// /**
//  * @brief 第三次夹取，
//  * **************************************
//  * ******* 此时各部分的预期状态：**********
//  * ** armAngle=arm_smallRise_angle;
//  * ** wristAngle=wrist_hold_angle;
//  * ** isGrab=false;
//  * **platformID=0;
//  * ***************************************
//  */
// void thirdGrab()
// {
//   //1.腕部转到夹取位置
//   top.moveWrist(wrist_grab_angle);
//   delay(delaytime);
//   //2.手臂放下
//   top.moveArm(arm_grab_angle);
//   delay(delaytime);
//   //3.夹取
//   top.grabIt();
//   delay(delaytime);
//   top.moveArm(arm_smallRise_angle);
//   delay(delaytime);
  
//   //考虑就这样一直拿着。。。


// }
