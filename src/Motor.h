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

  void set_speed(byte speed){
    m_speed = speed;
    _controller.write(m_active ? m_speed : 0);
  }

  void set_active(bool active) {
      if(active)
        enable();
      else
        disable();
  }


  // Variadic helper functions
  
  static void enable_many(Motor& m){
    Serial.println("enable:" + String(m.m_pin));
    m.enable();
  }

  template<typename... Args>
  static void enable_many(Motor& first, Args... args){
    Serial.println("enable:" + String(first.m_pin));
    first.enable();
    enable_many(args...);
  }

  static void disable_many(Motor& m){
    m.disable();
  }

  template<typename... Args>
  static void disable_many(Motor& first, Args... args){
    first.disable();
    disable_many(args...);
  }

  static void set_speed_many(byte mspeed, Motor& m){
    m.set_speed(mspeed);
  }

  template<typename... Args>
  static void set_speed_many(byte mspeed, Motor& first, Args... args){
    first.set_speed(mspeed);
    set_speed_many(mspeed, args...);
  }

  static void set_active_many(bool active, Motor& m){
    m.set_active(active);
  }

  template<typename... Args>
  static void set_active_many(bool active, Motor& first, Args... args){
    first.set_active(active);
    set_active_many(active, args...);
  }


};

#endif
