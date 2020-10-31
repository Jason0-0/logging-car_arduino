#ifndef MOTOR_H_
#define MOTOR_H_
#include "inttypes.h"
#include "arduino.h"

class motor
{
private:
    uint8_t sPin;
    uint8_t d1Pin;
    uint8_t d2Pin;
    uint8_t EnAPin;
    uint8_t EnBPin;
    char direction;
    int speed;
public:
    motor(uint8_t speedPin,uint8_t dir1Pin,uint8_t dir2Pin,uint8_t EncoderAPin,uint8_t EncoderBPin,int speed=0,char dir='s');
     inline void SetSpeed(int speed);
     inline void GetEncoderData();
     inline void motorStop();
};

motor::motor(uint8_t speedPin,uint8_t dir1Pin,uint8_t dir2Pin,uint8_t EncoderAPin,uint8_t EncoderBPin,int speed=0,char dir='s')
{
    sPin=speedPin;d1Pin=dir1Pin;d2Pin=dir2Pin;
    EnAPin=EncoderAPin;EnBPin=EncoderBPin;
    this->speed=speed;
    direction=dir;

    //initial
    pinMode(d1Pin,OUTPUT);
    pinMode(d2Pin,OUTPUT);
    pinMode(sPin,OUTPUT);
}

inline void motor::SetSpeed(int myspeed)
{
    //static unsigned char dir='s';
    if (myspeed>0)
    {
        speed=myspeed;
        analogWrite(sPin,speed);
        
        //direction change
        direction='f';
        digitalWrite(d1Pin,LOW);
        digitalWrite(d2Pin,HIGH);
    }
    else if (myspeed<0)
    {
        speed=-myspeed;
        analogWrite(sPin,speed);

        direction='b';
        digitalWrite(d1Pin,HIGH);
        digitalWrite(d2Pin,LOW);
    }
    else    //speed=0 ,no brake
    {
        analogWrite(sPin,speed); //speed should be 0
        digitalWrite(d1Pin,LOW);
        digitalWrite(d2Pin,LOW);
        direction=direction;
    }
}
    //刹车，但保留以前的速度
    inline void motor::motorStop()
    {
        //speed=0; //或者是仅刹车但保留之前设置的速度？
        //analogWrite(sPin,0);
        digitalWrite(d1Pin,HIGH);
        digitalWrite(d2Pin,HIGH);
        direction='s';
    }

#endif // !MOTOR_H_
