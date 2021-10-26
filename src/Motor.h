#ifndef ENGR_MOTOR_H
#define ENGR_MOTOR_H
#include <Servo.h>
#include "Arduino.h"


class Motor {
public:
  byte m_pin;
  byte m_speed;
  bool m_active;
  Servo _controller;

public:
  Motor() = default;
  Motor(byte pin, byte mspeed = 255): m_pin(pin), m_speed(mspeed), m_active(false) {
    
  }
  
  void toggle() {
    m_active = not m_active;
    if(m_active)
      enable();
    else
      disable();
  }

  void enable() {
    m_active = true;
    _controller.attach(m_pin);
    _controller.write(m_speed);
  }

  void disable() {
    m_active = false;
    _controller.write(0);
    _controller.detach();
  }

  void setSpeed(byte mspeed){
    m_speed = mspeed;
    _controller.write(m_active ? m_speed : 0);
  }


  // Variadic helper functions
  
  static void multiEnable(Motor& m){
    Serial.println("enable:" + String(m.m_pin));
    m.enable();
  }

  template<typename... Args>
  static void multiEnable(Motor& first, Args... args){
    Serial.println("enable:" + String(first.m_pin));
    first.enable();
    multiEnable(args...);
  }

  static void multiDisable(Motor& m){
    m.disable();
  }

  template<typename... Args>
  static void multiDisable(Motor& first, Args... args){
    first.disable();
    multiDisable(args...);
  }

  static void multiSetSpeed(byte mspeed, Motor& m){
    m.setSpeed(mspeed);
  }

  template<typename... Args>
  static void multiSetSpeed(byte mspeed, Motor& first, Args... args){
    first.setSpeed(mspeed);
    multiSetSpeed(mspeed, args...);
  }

  
};

#endif
