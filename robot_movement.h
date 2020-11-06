//#define ROBOT_MOVEMENT_H_

#ifndef ROBOT_MOVEMENT_H_
#define ROBOT_MOVEMENT_H_

#include "Arm.h"

//const identify

extern const long arm_grab_angle ;
extern const long arm_place_angle ;
extern const long arm_smallRise_angle ; //微微抬起以避免夹爪舵机干涉
extern const long arm_camera_angle ;      //用到摄像头识别时抬起的角度
extern const long wrist_grab_angle ;
extern const long wrist_place_angle ;
extern const long wrist_camera_angle ;
extern const int delaytime ;
extern const int servoDelay ;
extern const long wrist_hold_angle; //两次连续夹取之间腕部停驻的角度（避免打到地上的和车上的物料
extern const long arm_release_angle;
extern const long arm_bigRise_angle;
extern const long wrist_release_angle;
extern const long arm_moving_angle;  //行走过程中为了平衡重心...
extern const long wrist_moving_angle;


extern Arm top;


// void firstGrab();
// void secondGrab();
// void thirdGrab();
void Grab(char ID);
void Place(char ID);
bool CameraState();
bool ArmWait();
bool MovingState();

#endif // !ROBOT_MOVEMENT_H_
