//#include "chassis.h"
//#include "motor.h"
#include "Arm.h"
#include "Servo.h"

//#define SERVO_MAX_ANGLE 270     //舵机最大转角
//#define ARM_MAX_ANGLE 90       //大臂最大转角（绝对角位置）
//#define ARM_MIN_ANGLE (-15)     //大臂最小转角（以底盘平面为0°）
//#define WRIST_MAX_ANGLE 270     //腕部最大转角（暂时是最大舵机转角，0°位置为手臂平面）
//
////#define ARM_BIAS_ANGLE 90
//#define WRIST_ZERO_ANGLE 0
//#define BL_BIAS_ANGLE (86)     //大臂舵机的中位角
//#define BR_BIAS_ANGLE (90)
//#define GRAB_ZERO_ANGLE 40       //夹爪爪
//#define TOP_ZERO_ANGLE 0
//#define PLATFORM_ZERO_ANGLE 45   //放置平台(ID1)
//#define PLATFORM_ID1_ANGLE 45
//#define PLATFORM_ID2_ANGLE 160
//#define GRAB_LOOSE_ANGLE 40      //夹爪抓取和放开对应的舵机转角
//#define GRAB_TIGHT_ANGLE 18
 
//chassis littleCar()
//class Arm
//{
//private:
//    Servo bottomL_s;    //底部大臂的两个舵机
//    Servo bottomR_s;
//    Servo grab_s;       //抓取机构
//    Servo top_s;        //腕部
//    Servo platform_s;   //平台
//    char platform_ID;   //标记当前位于可抓取位置的盘子ID
//    bool isGrab;        //用于标志当前夹爪状态
//    long armAngle;      //大臂相对底盘平面的角位置
//    //long current_armAngle;
//    long wristAngle;    //腕部相对大臂的角位置
//    //long current_wristAngle;
//    int currentPlatform;    //当前处于可被抓取位置的平台（待考虑
//    //bool platformReady;
//    inline long ANGLE(long realAngle);   //用于实际270°舵机与arduino Servo库默认的0~180°角度值映射转换
//public:
//    Arm(int blPin,int brPin,int grabPin,int topPin,int platformPin);   
//    void grabIt();  
//    void loosenIt();
//    void turnTo(char ID);
//    void moveArm(long angle);
//    void moveWrist(long angle);
//    void getCurrentPlace(); //获取当前平台、大臂、手腕的角位置（也有可能用不上
//    
//};

Arm top;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  top.servoInit(4,6,7,10,11);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  top.grabIt();
  Serial.println("grab!");
  delay(2000);
  top.loosenIt();
  Serial.println("loosen!");
  delay(2000);
  //Serial.println("eeee");

}


//
//
//inline Arm::Arm(int blPin,int brPin,int grabPin,int topPin,int platformPin)
//{
//
//  //Serial.println(blPin);
//  
//    //引脚定义
//    bottomL_s.attach(blPin);
//    bottomR_s.attach(brPin);
//    grab_s.attach(grabPin);
//    top_s.attach(topPin);
//    platform_s.attach(platformPin);
//    
//
//    //让舵机定位到初始位置
////    bottomL_s.write(ANGLE(BL_BIAS_ANGLE));
////    bottomR_s.write(ANGLE(BR_BIAS_ANGLE));
////    top_s.write(ANGLE(TOP_ZERO_ANGLE));
////    platform_s.write(ANGLE(PLATFORM_ZERO_ANGLE));
////    grab_s.write(ANGLE(GRAB_ZERO_ANGLE));       //40
////
////    armAngle=ARM_MAX_ANGLE;     //大臂举高到与底盘垂直（90°
////    wristAngle=WRIST_ZERO_ANGLE;    //腕部与大臂平行（0°
////    delay(5000);                     //等待舵机转到设定位置
////    isGrab=false;
//    
//}
//
////TODO:精确控制何时完成转动
// void Arm::grabIt()
//{
////  Serial.println("work");
////  delay(5);
//    grab_s.write(60);
//    delay(500);
//    isGrab=true;
//}
// void Arm::loosenIt()
//{
////    for (int i = grab_s.read(); i < GRAB_LOOSE_ANGLE; i=i+1)
////    {
////        grab_s.write(i/*(GRAB_LOOSE_ANGLE)*/);
////    }
//     grab_s.write(90);
//     delay(500);
//    
//    
//}
//
///**
// * @brief 大臂转动
// * 
// * @param angle 绝对角位置
// */
//inline void Arm::moveArm(long angle)
//{
//    //越界处理
//    if (angle>ARM_MAX_ANGLE)
//    {
//        armAngle=ARM_MAX_ANGLE;
//    }
//    else if(angle<ARM_MIN_ANGLE)
//    {
//        armAngle=ARM_MIN_ANGLE;
//    }
//    else
//    {
//        armAngle=angle;
//    }
//
//    bottomL_s.write(BL_BIAS_ANGLE+ANGLE((ARM_MAX_ANGLE-armAngle)));
//    bottomR_s.write(BR_BIAS_ANGLE-ANGLE((ARM_MAX_ANGLE-armAngle)));
//    //delay(10*)
//      
//}
//
//inline void Arm::moveWrist(long angle)
//{
//    wristAngle=angle;
//    top_s.write(wristAngle);
//}
//inline long Arm::ANGLE(long realAngle)
//{
//    //return SERVO_MAX_ANGLE/180*realAngle;
//    return map(realAngle,SERVO_MAX_ANGLE,0,180,0);
//}
//
//inline void Arm::getCurrentPlace()
//{
//    armAngle=map(bottomR_s.read(),180,0,SERVO_MAX_ANGLE,0)-(BR_BIAS_ANGLE-ARM_MAX_ANGLE);
//    wristAngle=map(top_s.read(),180,0,SERVO_MAX_ANGLE,0);
//    //留着写平台的
//}
//
//inline void Arm::turnTo(char ID)
//{
//    if (ID=1)
//    {
//        platform_s.write(PLATFORM_ID1_ANGLE); 
//        platform_ID=1;  
//    }
//    else if (ID=2)
//    {
//        platform_s.write(PLATFORM_ID2_ANGLE);
//        platform_ID=2;
//    }
//    
//}
