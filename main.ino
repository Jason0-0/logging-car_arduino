//#include "chassis.h"
//#include "motor.h"
#include "Arm.h"
#include "Servo.h"

char ch = 0;
//const identify
const long arm_grab_angle = -10;
const long arm_place_angle = 59;
const long arm_smallRise_angle = 15; //微微抬起以避免夹爪舵机干涉
const long arm_camera_angle = 90;      //用到摄像头识别时抬起的角度
const long wrist_grab_angle = 59;
const long wrist_place_angle = 225;
const long wrist_camera_angle = 59;
const int delaytime = 1000;
const int servoDelay = 10;
const long wrist_hold_angle=140; //两次连续夹取之间腕部停驻的角度（避免打到地上的和车上的物料

Arm top;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  top.servoInit(4,6,7,10,11);
  delay(500);
  Serial.println("ready!");
  
}
void loop()
{
  
  switch (ch)
  {
  case 'a':
    Serial.println("第一次夹取...");
    firstGrab();
    Serial.println("Done 1 !");
    ch=0;
    break;

  case 'b':
    Serial.println("第二次夹取...");
    secondGrab();
    Serial.println("Done 2 !");
    ch=0;
    break;
  case 'c':
    Serial.println("第三次夹取...");
    thirdGrab();
    Serial.println("Done 3 !");
    ch=0;
    break;
  case 0:
    /*nothing*/
    break;
  
  default:
    ch=5;
    Serial.println("0. reset.");
    top.moveArm(90);
    top.moveWrist(TOP_ZERO_ANGLE);
    top.turnTo((char)1);
    top.loosenIt();
    delay(2000);
    break;
  }

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

void firstGrab()
{
  //任务编号判断（待写）

  //任务内容
  Serial.println("1. turn wrist.");
  top.moveWrist(wrist_grab_angle);
  delay(delaytime);
  Serial.println("2. move arms down.");
  Serial.println("moving...");

  //调速，todo:需要改起始角度的逻辑
  for (int i = ARM_MAX_ANGLE; i >= arm_grab_angle; i--)
  {
    //Serial.println(i);
    top.moveArm(i);
    delay(servoDelay);
  }
  //top.moveArm(-7);
  delay(delaytime);
  Serial.println("3. grab!"); //夹取
  top.grabIt();
  delay(delaytime);

  Serial.println("4. rise arms."); //提一个小角度便于越过干涉
  top.moveArm(arm_smallRise_angle);
  delay(delaytime);

  Serial.println("5. turn wrist."); //放置
  top.moveWrist(wrist_place_angle);
  delay(delaytime);
  Serial.println("6. down arms.");
  top.moveArm(arm_place_angle);
  delay(delaytime);
  Serial.println("7. place.");
  top.loosenIt();
  delay(delaytime);

  Serial.println("8. rise arms.");  //准备下一次夹取
  top.moveArm(arm_smallRise_angle);
  delay(delay);
  Serial.println("9. turn wrist."); 
  top.moveWrist(wrist_hold_angle);  //转到中间的角度
  
  //整点骚操作（伪时序配合
  delay(250);
  top.turnTo((char)2);    //转到2号盘

  //等底盘移到下一个位置
}

/**
 * @brief 夹取第二个物料
 * **********************************
 * 调用该函数时各部分的预期状态：
 * ** armAngle=arm_smallRise_angle;
 * ** wristAngle=wrist_hold_angle;
 * ** isGrab=false;
 * ** platformID=2
 * **********************************
 */
void secondGrab()
{
  //1.腕部转到夹取位置
  top.moveWrist(wrist_grab_angle);
  delay(delaytime);
  //2.手臂放下
  top.moveArm(arm_grab_angle);
  delay(delaytime);
  //3.夹取
  top.grabIt();
  delay(delaytime);
  //4.放
  top.moveArm(arm_smallRise_angle);
  delay(delaytime);
  top.moveWrist(wrist_place_angle);
  delay(delaytime);
  top.moveArm(arm_place_angle);
  delay(delaytime);
  top.loosenIt();
  delay(delaytime);

  top.moveArm(arm_smallRise_angle);
  delay(delaytime); 
  top.moveWrist(wrist_hold_angle);  //转到中间的角度
  
  //整点骚操作（伪时序配合
  delay(250);
  top.turnTo((char)0);    //转到中间

  //wait for next grab...
}

/**
 * @brief 第三次夹取，
 * **************************************
 * ******* 此时各部分的预期状态：**********
 * ** armAngle=arm_smallRise_angle;
 * ** wristAngle=wrist_hold_angle;
 * ** isGrab=false;
 * **platformID=0;
 * ***************************************
 */
void thirdGrab()
{
  //1.腕部转到夹取位置
  top.moveWrist(wrist_grab_angle);
  delay(delaytime);
  //2.手臂放下
  top.moveArm(arm_grab_angle);
  delay(delaytime);
  //3.夹取
  top.grabIt();
  delay(delaytime);
  
  //考虑就这样一直拿着。。。


}
