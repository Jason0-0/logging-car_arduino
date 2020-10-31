#define ROBOT_MOVEMENT_H_

#ifndef ROBOT_MOVEMENT_H_
#define ROBOT_MOVEMENT_H_

#include "Arm.h"

//const identify
extern const long arm_grab_angle = -10;
extern const long arm_place_angle = 59;
extern const long arm_smallRise_angle = 15; //微微抬起以避免夹爪舵机干涉
extern const long arm_camera_angle = 90;      //用到摄像头识别时抬起的角度
extern const long wrist_grab_angle = 59;
extern const long wrist_place_angle = 225;
extern const long wrist_camera_angle = 59;
extern const int delaytime = 500;
extern const int servoDelay = 10;
extern const long wrist_hold_angle=140; //两次连续夹取之间腕部停驻的角度（避免打到地上的和车上的物料

extern Arm top;

void firstGrab();
void secondGrab();
void thirdGrab();

void Place(char ID);

#endif // !ROBOT_MOVEMENT_H_
