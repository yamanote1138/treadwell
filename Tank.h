/*
  Tank.h - control two dc motors in concert via an h-bridge
  Created by Chad P Francis, November 23, 2021
  Released into the public domain.
*/
#ifndef Tank_h
#define Tank_h

#include "Arduino.h"
#include "Motor.h"

class Tank
{
  public:
    Tank(int pinPwmA, int pinAIn1, int pinAIn2, int pinStandby, int pinPwmB, int pinBIn1, int pinBIn2);
    void stop();
    void forward(int speed, int duration);
    void reverse(int speed, int duration);
    void left(int speed, int duration);
    void right(int speed, int duration);
    void brake();
    void unbrake();
  private:
    int _pinStandby;
    Motor *_leftMotor;
    Motor *_rightMotor;
};

#endif
