/*
  Motor.h - basic Library for controlling a dc motor connected to an h-bridge
  Created by Chad P Francis, November 23, 2021
  Released into the public domain.
*/
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor
{
  public:
    Motor();
    Motor(int pinPwm, int pinIn1, int pinIn2, char* name);
    void stop();
    void forward(int speed);
    void reverse(int speed);
  private:
    int _pinPwm;
    int _pinIn1;
    int _pinIn2;
    char* _name;
};

#endif