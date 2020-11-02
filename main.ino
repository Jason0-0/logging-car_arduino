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

/* private function declear begin */
void sensor_read();
void rotate90(int n, int dir);
void tracing_f(int n);
void justify();
void rotate_temp();
/* private function declear end */

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
 //--------------------------驱动底盘 起步到线上-------------------------------------------------
    sensor_read();
   while(!data[2])
   {
      Chassis_c.Move(0, -40, 0);
      sensor_read();
   }
Chassis_c.Stop();

  //--------------------------驱动底盘 循迹到扫码区-------------------------------------------------
tracing_f(2);
Chassis_c.Move(40,0,0);
delay(500);
Chassis_c.Stop();




//--------------------------扫码------------------------------------------------
rotate90(1,1);
delay(2000);

//Serial.println("get!");
//digitalWrite(openmv,HIGH);
//Serial.println("get!");
//  static int cnt=0;
//  while (Serial2.available() && camFlag==false)
//  {
//    Serial.println("get!");
//    
//    camBuf[cnt%3]=char(Serial2.read());
//    //test=int(Serial2.read()-'0'); //输出int类型
//    //Serial.println(camBuf[cnt%3]); 
//    cnt++;
//    //Serial.print(cnt);Serial.print("get ");
//    delay(2);
//    if (cnt>30-1) 
//    {
//      cnt=0;
//      //Serial.println();
//      camFlag=true;
//    }
//  }
//  digitalWrite(openmv,LOW);
//  Serial.println(camBuf);
////delay(2000);


rotate_temp();
delay(1000);

//--------------------------驱动底盘 循迹到原料区-------------------------------------------------


tracing_f(4);
delay(500);
rotate90(1,1);
delay(500);

justify();





  
  //--------------------------视觉识别及抓取-------------------------------------------------


 Chassis_c.Move(0,40, 0);
  delay(1200);
  Chassis_c.Stop();

  delay(500);
  Chassis_c.Move(0,-40,0);
  delay(2400);
  Chassis_c.Stop();

delay(1200);

 Chassis_c.Move(0,40, 0);
  delay(1200);
  Chassis_c.Stop();

while(1)//【测试用，记得删除】
{;}
  //--------------------------驱动底盘 从原料区到粗加工区-------------------------------------------------

//delay(2000);
//rotate90(1,-1);
//delay(500);
//tracing_f(1);
//delay(500);
//rotate90(1,-1);
//delay(500);
//tracing_f(3);
//delay(500);
//rotate90(1,1);  

  //--------------------------视觉识别、放置及抓取-------------------------------------------------


  //--------------------------驱动底盘 粗加工区到半成品区-------------------------------------------------


  //--------------------------视觉识别及放置-------------------------------------------------


  //--------------------------驱动底盘 从半成品区返回-------------------------------------------------



  //--------------------------END-------------------------------------------------
 
  

}

/* interrupt callback funtion begin */
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

/* interrupt callback function end */

/* private function define begin */

//获取传感器状态
inline void sensor_read()
{
  data[0] = digitalRead(sensorPin6); //读取各传感器状态 黑线为1
  data[1] = digitalRead(sensorPin5); //实际接线顺序是反的
  data[2] = digitalRead(sensorPin4);
  data[3] = digitalRead(sensorPin3);
  data[4] = digitalRead(sensorPin2);
  data[5] = digitalRead(sensorPin1);
}

//旋转n*90度 dir小于零则左转 大于零右转
void rotate90(int n, int dir) 
{
  int count1 = 0;//对应data0
  int temp1 = 1;
  int count2 = 0;//对应data4
  int temp2 = 1;

  if (dir <= 0)
  {
    while (count1 < n || count2 < n) //左转n*90度
    {
      sensor_read();
      Chassis_c.Move(0, 0, -40);

      if (data[0] & !temp1)//从空地碰到黑线才计数
      {
        count1++;
        temp1 = 1;
      }
      else if (!data[0])
      {
        temp1 = 0;
      }

      if (data[4] & !temp2)
      {
        count2++;
        temp2 = 1;
      }
      else if (!data[4])
      {
        temp2 = 0;
      }
    }
    Chassis_c.Stop();
  }

  else 
  {
    while (count1 < n || count2 < n) //右转n*90度
    {
      sensor_read();
      Chassis_c.Move(0, 0, 40);

      if (data[1] & !temp1)//从空地碰到黑线才计数
      {
        count1++;
        temp1 = 1;
      }
      else if (!data[1])
      {
        temp1 = 0;
      }

      if (data[5] & !temp2)
      {
        count2++;
        temp2 = 1;
      }
      else if (!data[5])
      {
        temp2 = 0;
      }
    }
    Chassis_c.Stop();
  }
}

//向前循迹，到第n根黑线停下
void tracing_f(int n)
  {
    int count = 0;
    int temp = 1;
    while (count < n )
    {
      sensor_read();
      if (data[2] == 0 & data[3] == 0) //在直线内 直行
      {
        Chassis_c.Move(40, 0, 0);
        //Serial.println("foward");
      }
      else if (data[2] == 1  & data[3] == 1) //十字路口 直行
      {
        Chassis_c.Move(40, 0, 0);
        //Serial.println("foward");
      }
      else if (data[2] == 1  & data[3] == 0) //右偏 原地左转
      {
        delay(50);
        sensor_read();
        if (data[2] == 1  & data[3] == 0)//延时检测，避免误差
        {
          Chassis_c.Move(0, 0, -40);
          delay(50);
        }
      }

      else if (data[2] == 0  & data[3] == 1) //左偏 原地右转
      {
        delay(50);
        sensor_read();
        if (data[2] == 0  & data[3] == 1)
        {
          Chassis_c.Move(0, 0, 40);
          delay(50);
        }
      }

      if (data[1] & data[4] & !temp ) //翻转计数
      {
        count++;
        temp = 1;
      }

      else if (data[1] == 0 & data[4] == 0)
      {
        temp = 0;
      }
    }
    Chassis_c.Stop();
  }

void justify()
{
  sensor_read();
  while(data[0]||data[1]||data[4]||data[5])
  {//先调前后 再调旋转
    if(data[0] && data[4])
    {
      while(data[0] && data[4])
      {
         Chassis_c.Move(0,0,-40);   
        sensor_read();
      }
    }
    
    else if(data[1] && data[5])
    {
      while(data[1] && data[5])
      {
        Chassis_c.Move(0,0,40);   
        sensor_read();
        }
    }

    else if(data[0] && data[5]) 
    {
      while(data[0] && data[5])
      {
        Chassis_c.Move(-40,0,0);   
        sensor_read();
        }
    }

    else if(data[1] && data[4])
    {
      while(data[1] && data[4])
      {
        Chassis_c.Move(40,0,0);   
        sensor_read();
        }
    }

    else if(data[0])
    {
      while(data[0])
      {
        Chassis_c.Move(-20,0,-20);   
        sensor_read();
        }
    }

    else if(data[1])
    {
      while(data[1])
      {
        Chassis_c.Move(20,0,20);   
        sensor_read();
        }
    }

    else if(data[4])
    {
      while(data[4])
      {
        Chassis_c.Move(20,0,-20);   
        sensor_read();
        }
    }

    else if(data[5])
    {
      while(data[5])
      {
        Chassis_c.Move(-20,0,20);   
        sensor_read();
        }
    }

    else
    {
      Chassis_c.Stop();
    }
 
  }
  Chassis_c.Stop(); 
 }

void rotate_temp() //【到二维码中间区，需改进】
{
  Chassis_c.Move(0,0,-40);
  delay(200);
  while(!data[2])
  {
      Chassis_c.Move(0,0,-40);
  }
  Chassis_c.Stop();
}

/* private function define end */

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
