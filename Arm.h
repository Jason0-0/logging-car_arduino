/**
 * @file Arm.h
 * @author Jason Yan
 * @brief 小车车上部机械臂夹取及储物平台运动的类 Arm定义 
 * @version 0.1
 * @date 2020-10-19
 * 
 * @todo 考虑引入基于舵机返回的当前角度信息的成员变量 x_currentAngle并以此来实现对舵机转动过程的准确控制
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef ARM_H_
#define ARM_H_

#define SERVO_MAX_ANGLE 270     //舵机最大转角
#define ARM_MAX_ANGLE 90       //大臂最大转角（绝对角位置）
#define ARM_MIN_ANGLE (-15)     //大臂最小转角（以底盘平面为0°）
#define WRIST_MAX_ANGLE 270     //腕部最大转角（暂时是最大舵机转角，0°位置为手臂平面）

//#define ARM_BIAS_ANGLE 90
#define WRIST_ZERO_ANGLE 0
#define BL_BIAS_ANGLE (86)     //大臂舵机的中位角
#define BR_BIAS_ANGLE (90)
#define GRAB_ZERO_ANGLE 40       //夹爪爪
#define TOP_ZERO_ANGLE 0
#define PLATFORM_ZERO_ANGLE 45   //放置平台(ID1)
#define PLATFORM_ID1_ANGLE 45
#define PLATFORM_ID2_ANGLE 160
#define GRAB_LOOSE_ANGLE 40      //夹爪抓取和放开对应的舵机转角
#define GRAB_TIGHT_ANGLE 18


#include <Servo.h>

class Arm
{
private:
    Servo bottomL_s;    //底部大臂的两个舵机
    Servo bottomR_s;
    Servo grab_s;       //抓取机构
    Servo top_s;        //腕部
    Servo platform_s;   //平台
    char platform_ID;   //标记当前位于可抓取位置的盘子ID
    bool isGrab;        //用于标志当前夹爪状态
    long armAngle;      //大臂相对底盘平面的角位置
    //long current_armAngle;
    long wristAngle;    //腕部相对大臂的角位置
    //long current_wristAngle;
    int currentPlatform;    //当前处于可被抓取位置的平台（待考虑
    //bool platformReady;
    inline long ANGLE(long realAngle);   //用于实际270°舵机与arduino Servo库默认的0~180°角度值映射转换
public:
    Arm();   
    void grabIt();  
    void loosenIt();
    void turnTo(char ID);
    void moveArm(long angle);
    void moveWrist(long angle);
    void getCurrentPlace(); //获取当前平台、大臂、手腕的角位置（也有可能用不上
    void servoInit(int blPin,int brPin,int grabPin,int topPin,int platformPin);
    
};

inline Arm::Arm()
{
    
    armAngle=ARM_MAX_ANGLE;     //大臂举高到与底盘垂直（90°
    wristAngle=WRIST_ZERO_ANGLE;    //腕部与大臂平行（0°
    //
    isGrab=false;
    
    
}

/**
 * @brief 舵机初始化函数，包括pin口挂载和初始角度设定的功能。
 * @brief 在setup()中调用！！！
 * 
 * @param blPin 大臂左侧电机
 * @param brPin 大臂右侧电机
 * @param grabPin 夹爪
 * @param topPin 小臂电机
 * @param platformPin 置物平台
 */
inline void Arm::servoInit(int blPin,int brPin,int grabPin,int topPin,int platformPin)
{
    //引脚定义和挂载
    bottomL_s.attach(blPin);
    bottomR_s.attach(brPin);
    grab_s.attach(grabPin);
    top_s.attach(topPin);
    platform_s.attach(platformPin);

    //让舵机定位到初始位置
    bottomL_s.write(ANGLE(BL_BIAS_ANGLE));
    bottomR_s.write(ANGLE(BR_BIAS_ANGLE));
    top_s.write(ANGLE(TOP_ZERO_ANGLE));
    platform_s.write(ANGLE(PLATFORM_ZERO_ANGLE));
    grab_s.write(ANGLE(GRAB_ZERO_ANGLE));       //40
    delay(500);                     //等待舵机转到设定位置
    
}

//TODO:精确控制何时完成转动
inline void Arm::grabIt()
{
    /** experiment code begin
     * 
     * while(grab_s.read()!=ANGLE(GRAB_TIGHT_ANGLE))
     * {
     *     grab_s.write(ANGLE(GRAB_TIGHT_ANGLE));
     * }
     * 
     *   experiment code end
     **/
    grab_s.write(GRAB_TIGHT_ANGLE);
    //delay(200);
    isGrab=true;
}
inline void Arm::loosenIt()
{
//    for (int i = grab_s.read(); i < GRAB_LOOSE_ANGLE; i=i+1)
//    {
//        /* code */
//    }
    
    grab_s.write((GRAB_LOOSE_ANGLE));
}

/**
 * @brief 大臂转动
 * 
 * @param angle 绝对角位置
 */
inline void Arm::moveArm(long angle)
{
    //越界处理
    if (angle>ARM_MAX_ANGLE)
    {
        armAngle=ARM_MAX_ANGLE;
    }
    else if(angle<ARM_MIN_ANGLE)
    {
        armAngle=ARM_MIN_ANGLE;
    }
    else
    {
        armAngle=angle;
    }

    bottomL_s.write(BL_BIAS_ANGLE+ANGLE((ARM_MAX_ANGLE-armAngle)));
    bottomR_s.write(BR_BIAS_ANGLE-ANGLE((ARM_MAX_ANGLE-armAngle)));
    //delay(10*)
      
}

inline void Arm::moveWrist(long angle)
{
    wristAngle=angle;
    top_s.write(wristAngle);
}
inline long Arm::ANGLE(long realAngle)
{
    //return SERVO_MAX_ANGLE/180*realAngle;
    return map(realAngle,SERVO_MAX_ANGLE,0,180,0);
}

inline void Arm::getCurrentPlace()
{
    armAngle=map(bottomR_s.read(),180,0,SERVO_MAX_ANGLE,0)-(BR_BIAS_ANGLE-ARM_MAX_ANGLE);
    wristAngle=map(top_s.read(),180,0,SERVO_MAX_ANGLE,0);
    //留着写平台的
}

/**
 * @brief 置物平台切换
 * 
 * @param ID 标志当前需要转到夹爪可抓取位置的盘子ID号（1或2）
 */
inline void Arm::turnTo(char ID)
{
    if (ID=1)
    {
        platform_s.write(PLATFORM_ID1_ANGLE); 
        platform_ID=1;  
    }
    else if (ID=2)
    {
        platform_s.write(PLATFORM_ID2_ANGLE);
        platform_ID=2;
    }
    
}



#endif // !ARM_H_
    
