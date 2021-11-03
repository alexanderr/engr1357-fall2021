#include <Arduino.h>
#include <arduino-timer.h>
#include <Keypad.h>
#include "RobotConstants.h"
#include "RobotUtil.h"
#include <LCD.h> //I2C library
#include <LiquidCrystal_I2C.h>
#include "Actions.h"
#include <Motor.h>


int requested_motor_state = -1;
int motor_state = MS_STATIONARY; // Don't modify this directly, use requested_motor_state
unsigned int robot_state = 0; // Robot state

Action** current_action_list = nullptr;
int action_index = -1;
int blocking_event = Events::NONE;
unsigned long blocking_duration = 0;
unsigned long last_action_time = 0;

String lcd_message = "test";


float distanceF = -1;
float distanceL = -1;
float distanceR = -1;

Timer<8, millis> timer;

Motor motors[4] = {
    Motor(Pins::M_FRONTLEFT),
    Motor(Pins::M_FRONTRIGHT),
    Motor(Pins::M_BACKLEFT),
    Motor(Pins::M_BACKRIGHT),
};

Keypad keypad = Keypad(makeKeymap(KeypadConstants::KEYS), 
                       KeypadConstants::ROW_PINS, KeypadConstants::COL_PINS, 
                       KEYPAD_ROWS, KEYPAD_COLS);


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void fire_event(int event) {
    if(blocking_event == event)
        blocking_event = Events::NONE;
}

bool motor_loop(void*) {
    motor_state = MS_STATIONARY;

    switch (requested_motor_state)
    {
        case -1:
            return true; // no request is made.
        case MS_STATIONARY:
            robot_state &= ~RobotState::ENABLE_MOTORS_MASK;
            break;
        case MS_FORWARD:
            robot_state |= RobotState::ENABLE_MOTORS_MASK;
            robot_state &= ~RobotState::REVERSE_MOTORS_MASK;
            break;
        case MS_TURN_LEFT:
            robot_state |= RobotState::ENABLE_MOTORS_MASK;
            break;
        case MS_REVERSE_TURN_LEFT:
            robot_state |= (RobotState::MOTOR_FL_ON | RobotState::MOTOR_BL_ON); // enable left motors.
            robot_state &= ~(RobotState::MOTOR_FR_ON | RobotState::MOTOR_BR_ON); // Turn off right motors.
            break;
        case MS_REVERSE_TURN_RIGHT:
            robot_state |= (RobotState::MOTOR_FR_ON | RobotState::MOTOR_BR_ON); // enable right motors.
            robot_state &= ~(RobotState::MOTOR_FL_ON | RobotState::MOTOR_BL_ON); // Turn off left motors.
            break;
        default:
            break;
    }

    motor_state = requested_motor_state;
    requested_motor_state = -1;

    if((robot_state & RobotState::ENABLE_MOTORS_MASK) == RobotState::ENABLE_MOTORS_MASK) {
        // Enable all
        Motor::enable_many(motors[FL], motors[FR], motors[BL], motors[BR]);
    }
    else if(!(robot_state & RobotState::ENABLE_MOTORS_MASK)) {
        // Disable all
        Motor::disable_many(motors[FL], motors[FR], motors[BL], motors[BR]);
    }
    else {
        // Set each motor individually.
        motors[FL].set_active(robot_state & RobotState::MOTOR_FL_ON);
        motors[FR].set_active(robot_state & RobotState::MOTOR_FR_ON);
        motors[BL].set_active(robot_state & RobotState::MOTOR_BL_ON);
        motors[BR].set_active(robot_state & RobotState::MOTOR_BR_ON);
    }


    motors[FL].set_speed((robot_state & RobotState::MOTOR_FL_REV) ? Speeds::LEFT_REVERSE : Speeds::LEFT_FORWARD);
    motors[FR].set_speed((robot_state & RobotState::MOTOR_FR_REV) ? Speeds::RIGHT_REVERSE : Speeds::RIGHT_FORWARD);
    motors[BL].set_speed((robot_state & RobotState::MOTOR_BR_REV) ? Speeds::LEFT_REVERSE : Speeds::LEFT_FORWARD);
    motors[BR].set_speed((robot_state & RobotState::MOTOR_BR_REV) ? Speeds::RIGHT_REVERSE : Speeds::RIGHT_FORWARD);


    return true;

}

bool ping_loop(void*) {
    static int i = 0;
    static int next;

    next = i & 3;

    switch(next) {
        case 0:
            distanceF = Ping::get_ping(Pins::PINGF_TRIG, Pins::PINGF_ECHO); 

            if(distanceF < DEFAULT_COLLISION_THRESHOLD) robot_state |= RobotState::FRONT_COLL;
            else robot_state &= ~RobotState::FRONT_COLL;

            break;
        case 1:
            distanceL = Ping::get_ping(Pins::PINGL_TRIG, Pins::PINGL_ECHO); 

            if(distanceL < DEFAULT_COLLISION_THRESHOLD) robot_state |= RobotState::LEFT_COLL;
            else robot_state &= ~RobotState::LEFT_COLL;

            break;
        case 2:
            distanceR = Ping::get_ping(Pins::PINGR_TRIG, Pins::PINGR_ECHO); 

            if(distanceR < DEFAULT_COLLISION_THRESHOLD) robot_state |= RobotState::RIGHT_COLL;
            else robot_state &= ~RobotState::RIGHT_COLL;

            break;
    }

    if(robot_state & RobotState::LEFT_COLL)
        fire_event(Events::LEFT_COLLISION);
    
    if(robot_state & RobotState::RIGHT_COLL)
        fire_event(Events::RIGHT_COLLISION);
    
    if(robot_state & RobotState::FRONT_COLL)
        fire_event(Events::FRONT_COLLISION);

    ++i;
    return true;
}

bool action_loop(void*) {
    if(current_action_list == nullptr)
        return true;
    
    if(!blocking_event && !blocking_duration) {
        ++action_index;
        Action& action = *(current_action_list[action_index]);
        Actions::FunctionTable[action.action]();
        blocking_event = action.endEvent;
        blocking_duration = action.duration;
        last_action_time = millis();
    }
    else if(blocking_duration) {
        if((millis() - last_action_time) > blocking_duration){ // has the action surpassed its duration?
            blocking_duration = 0;
            action_loop(nullptr);
        }
    }

    return true;
}

bool inclinometer_loop(void*) {
    static int reading = 0;
    // TODO: take reading
    return true;
}

void on_key_pressed(char key) {
    switch(key) {
        case 'A': break;
        default: break;
    }
}

bool keypad_loop(void*) {
    if(!keypad.getKeys())
        return true; // No key states changed.
    
    for(int i = 0; i < LIST_MAX; ++i) {
        if(!keypad.key[i].stateChanged)
            continue;
        
        switch(keypad.key[i].kstate) {
            case PRESSED: on_key_pressed(keypad.key[i].kchar); break;
            case HOLD: break;
            case RELEASED: break;
            case IDLE: break;
        }
    }

    return true;
}

bool lcd_loop(void*) {
    lcd.clear();
    lcd.home();
    lcd.println(lcd_message);
    return true;
}

void setup() {
    lcd.begin(16, 2);
    lcd.backlight();


    timer.every(20, action_loop);
    timer.every(40, motor_loop);
    timer.every(60, ping_loop);
    timer.every(80, keypad_loop);
    timer.every(200, lcd_loop);
}


void loop() {
    timer.tick<void>();
}
