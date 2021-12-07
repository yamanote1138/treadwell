/*
  Motor.cpp - basic Library for controlling a dc motor connected to an h-bridge
  Created by Chad P Francis, November 23, 2021
  Released into the public domain.
*/

#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int pinPwm, int pinIn1, int pinIn2, char* name)
{
  pinMode(pinPwm, OUTPUT);
  pinMode(pinIn1, OUTPUT);
  pinMode(pinIn2, OUTPUT);
  _pinPwm = pinPwm;
  _pinIn1 = pinIn1;
  _pinIn2 = pinIn2;
  _name = name;
}

void Motor::stop()
{
  analogWrite(_pinPwm, 0);
  digitalWrite(_pinIn1, LOW);
  digitalWrite(_pinIn2, LOW);
}

void Motor::forward(int speed)
{
  digitalWrite(_pinIn1, HIGH);
  digitalWrite(_pinIn2, LOW);
  analogWrite(_pinPwm, speed);
}

void Motor::reverse(int speed)
{
  digitalWrite(_pinIn1, LOW);
  digitalWrite(_pinIn2, HIGH);
  analogWrite(_pinPwm, speed);
}