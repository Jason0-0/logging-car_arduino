/*  INCLUDE BEGIN */
#include "Arm.h"
#include "Servo.h"
#include "robot_movement.h"
#include <inttypes.h>
#include "chassis.h"
#include "motor.h"
#include <TimerFive.h>//【合代码时注意舵机接口45、46不能用】
/* INCLUDE END */

//机械臂
Arm top;
//底盘驱动部分
int chassis::kFB = 1;
int chassis::kLR = 1;
int chassis::kR = 1;
motor frontLeft_m(12, 34, 35, 18, 31); //要查查能不能用
motor frontRight_m(8, 37, 36, 19, 38);
motor backLeft_m(9, 43, 42, 3, 49);
motor backRight_m(5, 26, 27, 2, 23);
chassis Chassis_c(&frontLeft_m, &frontRight_m, &backLeft_m, &backRight_m);

//红外部分
const int sensorPin1 = 48; //传感器引脚
const int sensorPin2 = 47;
const int sensorPin3 = 40;
const int sensorPin4 = 41;
const int sensorPin5 = 33;
const int sensorPin6 = 32;
int data[6];

//oprnmv引脚
const int openmv = 30;

//扫码部分
char camBuf[4] = {0, 0, 0, 0};
int assignment = 0;
bool camFlag = false;
int flag_scan = 0; //触发扫码标志


//编码器部分
const int en[8] = {18, 31, 19, 38, 3, 49, 2, 23};
volatile long cnt[4] = {0, 0, 0, 0};//初始化
volatile double vx_old = 0;
volatile double vy_old = 0;
volatile double time_old = 0;
volatile double time_new = 0;
volatile  double v[4] = {0, 0, 0, 0};
volatile double vx = 0;
volatile double vy = 0;
volatile double dx = 0;
volatile double dy = 0;

const  double sample_time = 40; //每40ms检测一次
const double pulse_num = 11;//电机转一圈脉冲数
const double peri = 0.250;//周长，单位m
const double ratio = 18.75;//电机转一圈脉冲数
const double kx = 0.9; //x方向修正因数，表示给1m而实际走的距离，5cm内适用，【15cm待调】
const double ky = 0.7; //x方向修正因数，表示给1m而实际走的距离

void setup()
{
  Serial.begin(9600); //computer
  Serial2.begin(9600);  //camera

  //Pins Init
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  pinMode(sensorPin5, INPUT);
  pinMode(sensorPin6, INPUT);
  pinMode(openmv, OUTPUT);

  //encoder & timer Init
  for (int i = 0; i <= 3; i++)
  {
    pinMode(en[2 * i ], INPUT);
    pinMode(en[2 * i + 1], INPUT);
  }
  Timer5.initialize(sample_time * 1000);

  //Robot init
  top.servoInit(4,6,7,10,11);
  delay(500)

  //Serial test
  Serial.println("ready!");
}

inline void sensor_read()//获取传感器状态
{
  data[0] = digitalRead(sensorPin6); //读取各传感器状态 黑线为1
  data[1] = digitalRead(sensorPin5); //实际接线顺序是反的
  data[2] = digitalRead(sensorPin4);
  data[3] = digitalRead(sensorPin3);
  data[4] = digitalRead(sensorPin2);
  data[5] = digitalRead(sensorPin1);
}

void rotate90(int n, int dir) //旋转n*90度 dir小于零则左转 大于零右转【待调，过转问题，需要使用三传感器方案】
{
  int count1 = 0;//对应data0
  int temp1 = 1;
  int count2 = 0;//对应data4
  int temp2 = 1;
  justify();
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
  justify();
}

void tracing_f(int n)//向前循迹，到第n根黑线停下【待调，需要左右两边分别计数，且要处理十字起步问题】
{
  int count = 0;
  int temp = 1;
  while (count < n )
  {
    sensor_read();
    if (data[2] == 0 & data[3] == 0) //在直线内 直行
    {
      Chassis_c.Move(70, 0, 0);
      //Serial.println("foward");
    }
    else if (data[2] == 1  & data[3] == 1) //十字路口 直行
    {
      Chassis_c.Move(70, 0, 0);
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

void justify()//【待调，一次不到位问题】
{
  sensor_read();
  while (data[0] || data[1] || data[4] || data[5])
  { //先调前后 再调旋转
    if (data[0] && data[4])
    {
      while (data[0] && data[4])
      {
        Chassis_c.Move(0, 0, -40);
        sensor_read();
      }
    }

    else if (data[1] && data[5])
    {
      while (data[1] && data[5])
      {
        Chassis_c.Move(0, 0, 40);
        sensor_read();
      }
    }

    else if (data[0] && data[5])
    {
      while (data[0] && data[5])
      {
        Chassis_c.Move(-40, 0, 0);
        sensor_read();
      }
    }

    else if (data[1] && data[4])
    {
      while (data[1] && data[4])
      {
        Chassis_c.Move(40, 0, 0);
        sensor_read();
      }
    }

    else if (data[0])
    {
      while (data[0])
      {
        Chassis_c.Move(-20, 0, -20);
        sensor_read();
      }
    }

    else if (data[1])
    {
      while (data[1])
      {
        Chassis_c.Move(20, 0, 20);
        sensor_read();
      }
    }

    else if (data[4])
    {
      while (data[4])
      {
        Chassis_c.Move(20, 0, -20);
        sensor_read();
      }
    }

    else if (data[5])
    {
      while (data[5])
      {
        Chassis_c.Move(-20, 0, 20);
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

//电机计数
void motor_count(int n)
{
  if ((millis() - time_old) > 0.1)
  {
    //当编码器码盘的OUTA脉冲信号下跳沿每中断一次，
    if ((digitalRead(en[2 * n - 2]) == LOW) && (digitalRead(en[2 * n - 1]) == HIGH))
    {
      cnt[n - 1]++;
    }
    else
    {
      cnt[n - 1]--;
    }
  }
  time_old == millis();
}

void counter1()
{
  motor_count(1);
}
void counter2()
{
  motor_count(2);
}
void counter3()
{
  motor_count(3);
}
void counter4()
{
  motor_count(4);
}

void cal_v()      //计速，核心函数                                                 //定时器中断函数
{
  for (int i = 0; i <= 3; i++)
  {
    v[i] = (double)(cnt[i] * peri) / ( pulse_num * sample_time * ratio) * 1000;
  }
  vx = (double)(v[0] - v[1] + v[2] - v[3]) / 4;
  vy = (double)(v[0] + v[1] - v[2] - v[3]) / 4;

  dx = dx + kx * (double)(vx + vx_old) / 2 * sample_time / (double)1000;
  dy = dy + ky * (double)(vy + vy_old) / 2 * sample_time / (double)1000;

  memset(cnt, 0, sizeof(cnt));
  vx_old = vx;
  vy_old = vy;
}

void distance(int vx_in, int vy_in, int w_in, double xin, double yin) //给定速度和x、y方向移动距离，x或y方向距离先到达后停止。（需注意x和y不能给太小）
{ //x正方向为向前；y正方向为向右（与速度一致）
  memset(cnt, 0, sizeof(cnt));

  Timer5.attachInterrupt(cal_v);
  attachInterrupt(digitalPinToInterrupt(en[0]), counter1, FALLING);     //开启编码器中断
  attachInterrupt(digitalPinToInterrupt(en[2]), counter2, FALLING);
  attachInterrupt(digitalPinToInterrupt(en[4]), counter3, FALLING);
  attachInterrupt(digitalPinToInterrupt(en[6]), counter4, FALLING);
  if (xin >= 0 && yin >= 0)
  {
    while (dx < xin && dy < yin)
    {
      Chassis_c.Move(vx_in, vy_in, w_in);
    }
    Chassis_c.Stop();
  }
  else if (xin < 0 && yin < 0)
  {
    while (dx > xin && dy > yin)
    {
      Chassis_c.Move(vx_in, vy_in, w_in);
    }
    Chassis_c.Stop();
  }
  Timer5.detachInterrupt(); //  关闭定时器中断
  detachInterrupt(digitalPinToInterrupt(en[0])); //关闭编码器中断
  detachInterrupt(digitalPinToInterrupt(en[2]));
  detachInterrupt(digitalPinToInterrupt(en[4]));
  detachInterrupt(digitalPinToInterrupt(en[6]));
}


void loop()
{
  //--------------------------驱动底盘 起步到线上-------------------------------------------------
  sensor_read();
  while (!data[2])//【待调，出现倾斜问题】
  {
    Chassis_c.Move(0, -50, 0);
    sensor_read();
  }
  Chassis_c.Stop();

  //--------------------------驱动底盘 循迹到扫码区-------------------------------------------------
  tracing_f(2);//【如不需要扫码则合并走6条线】

  //--------------------------扫码------------------------------------------------
  //【扫码部分未完善】
  //rotate90(1, 1);
  //delay(2000);

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


  //--------------------------驱动底盘 循迹到原料区-------------------------------------------------
  tracing_f(4);
  rotate90(1, 1);
  delay(500);

  //--------------------------视觉识别及抓取-------------------------------------------------
  while (!CameraState()); //等待摄像头就位
  
  //【此处加入调整距离代码】
  for (int i = 0; i < 3; i++)//将数组获取字符转化为数字
  {
    assignment = assignment * 10 + (camBuf[i]-'0');
  }
  switch (assignment)//【只考虑了123情况】
  {
    case 123://物料红绿蓝，取料蓝绿红
      distance(0, -50, 0, -100, -0.15); //左移15cm
      firstGrab();
      distance(0, 50, 0, 100, 0.15); //右移15cm
      secondGrab();//【此处加入机械臂取料代码】
      distance(0, 50, 0, 100, 0.15); //右移15cm
      thirdGrab();//【此处加入机械臂取料代码】
      distance(0, -50, 0, -100, -0.15); //左移15cm
      break;
  }
  //【此处加入距离复位代码】
  while(!CameraState());  //大臂复位到视觉的位置
  //距离复位代码。。。
  //--------------------------驱动底盘 从原料区到粗加工区-------------------------------------------------
  rotate90(1, -1);
  tracing_f(1);
  rotate90(1, -1);
  tracing_f(3);
  rotate90(1, 1);

  //--------------------------视觉识别、放置及抓取-------------------------------------------------
  //【此处加入调整距离代码】
  switch (assignment)
  {
    case 123://物料红绿蓝，取料蓝绿红，放料红绿蓝
      Place(0);//放手上拿着的
      CameraState();
      distance(0, 50, 0, 100, 0.15); //右移15cm
      Place(1);//这个和下面的可以对换
      CameraState(); 
      distance(0, -50, 0, -100, -0.15); //左移15cm
      distance(0, -50, 0, -100, -0.15); //左移15cm
      Place(2);//
      CameraState();
      distance(0, 50, 0, 100, 0.15); //右移15cm，复位
      break;
  }
  //【再按原来顺序取料？未加入】
  //【此处加入复位距离代码】

  //--------------------------驱动底盘 粗加工区到半成品区-------------------------------------------------
  rotate90(1, -1);
  tracing_f(2);
  rotate90(1, -1);
  tracing_f(3);
  rotate90(1, 1);


  //--------------------------视觉识别及放置-------------------------------------------------
  //【此处加入调整距离代码】
  while(!CameraState());
  switch (assignment)
  {
    case 123://物料红绿蓝，取料蓝绿红，放料红绿蓝
      //【此处加入视觉和机械臂放料代码】
      Place(0);
      CameraState();
      distance(0, 50, 0, 100, 0.15); //右移15cm
      Place(1);//【此处加入视觉和机械臂放料代码】
      CameraState();
      distance(0, -50, 0, -100, -0.15); //左移15cm
      distance(0, -50, 0, -100, -0.15); //左移15cm
      Place(2);//【此处加入视觉和机械臂放料代码】
      CameraState();
      distance(0, 50, 0, 100, 0.15); //右移15cm，复位
      break;
  }
  //【此处加入复位距离代码】

  //--------------------------驱动底盘 从半成品区返回-------------------------------------------------
  rotate90(1, -1);
  tracing_f(2);
  rotate90(1, 1);
  tracing_f(1);
  rotate90(1, -1);
  tracing_f(1);
  //【此处加入走半个格代码】


  //--------------------------END-------------------------------------------------
  sensor_read();
  while (!data[0])
  {
    Chassis_c.Move(0, 50, 0);
    sensor_read();
  }
  Chassis_c.Stop();

  //-----------------------test zone------------------------------------------------

  //  Chassis_c.Move(0,50,0);
  //  delay(840);
  //  Chassis_c.Stop();
  //
  //  delay(500);
  //  Chassis_c.Move(0,-50,0);
  //  delay(1600);
  //  Chassis_c.Stop();
  //justify();
  //rotate90(1, 1);
  //delay(2000);
  rotate90(1, -1);
  delay(2000);
  while (1) //【测试用，记得删除】
  {
    ;
  }
}


//void serialEvent2()
//{
//  static int cnt=0;
//  while (Serial2.available()/*&&camFlag==false*/)
//  {
//    Serial.println("get!");
//
//    camBuf[cnt%3]=char(Serial2.read());
//    //test=int(Serial2.read()-'0'); //输出int类型
//    Serial.println(camBuf[cnt%3]);
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
//
//}
