/*
  Tank.cpp - control two dc motors in concert via an h-bridge
  Created by Chad P Francis, November 23, 2021
  Released into the public domain.
*/

#include "Arduino.h"
#include "Tank.h"

Tank::Tank(int pinPwmA, int pinAIn1, int pinAIn2, int pinStandby, int pinPwmB, int pinBIn1, int pinBIn2)
{
  _pinStandby = pinStandby;
  pinMode(_pinStandby, OUTPUT);
  digitalWrite(_pinStandby, LOW);

  _leftMotor = new Motor(pinPwmA, pinAIn1, pinAIn2, "left");
  _rightMotor = new Motor(pinPwmB, pinBIn1, pinBIn2, "right");
  this->brake();
  _leftMotor->stop();
  _rightMotor->stop();
}

void Tank::forward(int speed, int duration)
{
  this->brake();
  _leftMotor->forward(speed);
  _rightMotor->forward(speed);
  this->unbrake();
  delay(duration);
  this->brake();
}

void Tank::reverse(int speed, int duration)
{
  this->brake();
  _leftMotor->reverse(speed);
  _rightMotor->reverse(speed);
  this->unbrake();
  delay(duration);
  this->brake();
}

void Tank::left(int speed, int duration)
{
  this->brake();
  _leftMotor->reverse(speed);
  _rightMotor->forward(speed);
  this->unbrake();
  delay(duration);
  this->brake();
}

void Tank::right(int speed, int duration)
{
  this->brake();
  _leftMotor->forward(speed);
  _rightMotor->reverse(speed);
  this->unbrake();
  delay(duration);
  this->brake();
}

void Tank::stop()
{
  this->brake();
  _leftMotor->stop();
  _rightMotor->stop();
}

void Tank::brake()
{
  digitalWrite(_pinStandby, LOW);
}

void Tank::unbrake()
{
  digitalWrite(_pinStandby, HIGH);
}