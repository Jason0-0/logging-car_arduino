#include "motor.h"

#ifndef CHASSIS_H_
#define CHASSIS_H_

/* defines start */

/* defines end  */

class chassis
{
private:
    static int kFB;
    static int kLR;
    static int kR;
    
    motor* frontLeft_m;
    motor* frontRight_m;
    motor* backLeft_m;
    motor* backRight_m;

    int vFB;int vLR;
    int rotateSpeed;

    /* 还有编码器等反馈数值 */
public:
    chassis(motor*,motor*,motor*,motor*,int vfb=0,int vlr=0,int omega=0);
    inline void Move(int fbSpeed,int lrSpeed,int omega);
    inline void Stop();
    //未完待续

    //~chassis();
};

chassis::chassis(motor* fl, motor* fr, motor* bl, motor* br, int vfb = 0, int vlr = 0, int omega = 0)
{
  kFB = 1;
  kLR = 1;
  kR = 1;
  frontLeft_m = fl;
  frontRight_m = fr;
  backLeft_m = bl;
  backRight_m = br;

  vFB = vfb; vLR = vlr;
  rotateSpeed = omega;
}

 void chassis::Move(int fbSpeed, int lrSpeed, int omega)
{
  vFB = fbSpeed; vLR = lrSpeed; rotateSpeed = omega;
  frontLeft_m->SetSpeed(vFB * kFB + vLR * kLR + kR * rotateSpeed);
  frontRight_m->SetSpeed(-vFB * kFB + vLR * kLR + kR * rotateSpeed);
  backLeft_m->SetSpeed(vFB * kFB - vLR * kLR + kR * rotateSpeed);
  backRight_m->SetSpeed(-vFB * kFB - vLR * kLR + kR * rotateSpeed);
}

 void chassis::Stop()
 {
    frontLeft_m->SetSpeed(0);
  frontRight_m->SetSpeed(0);
  backLeft_m->SetSpeed(0);
  backRight_m->SetSpeed(0);
 }


#endif // !CHASSIS_H_
