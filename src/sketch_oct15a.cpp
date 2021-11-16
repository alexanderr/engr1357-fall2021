#include <Arduino.h>
#include <arduino-timer.h>
#include <Keypad.h>
#include "RobotConstants.h"
#include "RobotUtil.h"
#include <LCD.h> //I2C library
#include <LiquidCrystal_I2C.h>
#include "Actions.h"
#include "ActionLists.h"
#include <Motor.h>

int requested_motor_state = -1;
int tick = 0;
int motor_state = MS_STATIONARY; // Don't modify this directly, use requested_motor_state
unsigned int robot_state = 0;    // Robot state

Action *current_action_list = maze_mode;
int action_index = -1;
int blocking_event = Events::NONE;
unsigned long blocking_duration = 0;
unsigned long last_action_time = 0;

String lcd_message = "test";

float distanceF = -1;
float distanceL = -1;
float distanceR = -1;

Timer<8, millis> timer;

Motor motors[2] = {
    // Motor(Pins::M_FRONTLEFT),
    // Motor(Pins::M_FRONTRIGHT),
    Motor(Pins::M_BACKLEFT),
    Motor(Pins::M_BACKRIGHT),
};

Keypad keypad = Keypad(makeKeymap(KeypadConstants::KEYS),
                       KeypadConstants::ROW_PINS, KeypadConstants::COL_PINS,
                       KEYPAD_ROWS, KEYPAD_COLS);

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Fires an event, if the current action is currently listening to this event,
// it will complete it and start the next action.
void fire_event(int event)
{
    if (blocking_event == event)
        blocking_event = Events::NONE;
}

// Loop that controls the state of the motors.
bool motor_loop(void *)
{
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
        robot_state |= RobotState::MOTOR_BL_REV;
        robot_state &= ~RobotState::MOTOR_BR_REV;
        break;
    case MS_TURN_RIGHT:
        robot_state |= RobotState::ENABLE_MOTORS_MASK;
        robot_state |= RobotState::MOTOR_BR_REV;
        robot_state &= ~RobotState::MOTOR_BL_REV;
        break;
    case MS_REVERSE_TURN_LEFT:
        robot_state |= (RobotState::MOTOR_FL_ON | RobotState::MOTOR_BL_ON);  // enable left motors.
        robot_state &= ~(RobotState::MOTOR_FR_ON | RobotState::MOTOR_BR_ON); // Turn off right motors.
        break;
    case MS_REVERSE_TURN_RIGHT:
        robot_state |= (RobotState::MOTOR_FR_ON | RobotState::MOTOR_BR_ON);  // enable right motors.
        robot_state &= ~(RobotState::MOTOR_FL_ON | RobotState::MOTOR_BL_ON); // Turn off left motors.
        break;
    default:
        break;
    }

    motor_state = requested_motor_state;
    requested_motor_state = -1;

    motors[BL].set_active(robot_state & RobotState::MOTOR_BL_ON);
    motors[BR].set_active(robot_state & RobotState::MOTOR_BR_ON);

    motors[BL].set_speed((robot_state & RobotState::MOTOR_BL_REV) ? Speeds::LEFT_REVERSE : Speeds::LEFT_FORWARD);
    motors[BR].set_speed((robot_state & RobotState::MOTOR_BR_REV) ? Speeds::RIGHT_REVERSE : Speeds::RIGHT_FORWARD);

    return true;
}

// Loop that controls the ping sensors. Only 1 ping sensor is updated every tick.
bool ping_loop(void *)
{
    static int next;

    next = tick % 3;
    switch (next)
    {
    case 0:
        distanceF = Ping::get_ping(Pins::PINGF_TRIG, Pins::PINGF_ECHO);
        Serial.println("FRONT: " + String(distanceF));
        if (distanceF < DEFAULT_COLLISION_THRESHOLD && distanceF != 0 )
            robot_state |= RobotState::FRONT_COLL;
        else
            robot_state &= ~RobotState::FRONT_COLL;

        break;
    case 1:
        distanceL = Ping::get_ping(Pins::PINGL_TRIG, Pins::PINGL_ECHO);
        Serial.println("LEFT: " + String(distanceL));

        if (distanceL < LEFT_COLLISION_THRESHOLD  && distanceL != 0 )
            robot_state |= RobotState::LEFT_COLL;
        else
            robot_state &= ~RobotState::LEFT_COLL;

        break;
    case 2:
        distanceR = Ping::get_ping(Pins::PINGR_TRIG, Pins::PINGR_ECHO);
        Serial.println("RIGHT: " + String(distanceR));

        if (distanceR < RIGHT_COLLISION_THRESHOLD  && distanceR != 0 )
            robot_state |= RobotState::RIGHT_COLL;
        else
            robot_state &= ~RobotState::RIGHT_COLL;

        break;
    }

    if (robot_state & RobotState::LEFT_COLL)
    {
        fire_event(Events::LEFT_COLLISION);
    }
    else
    {
        fire_event(Events::NO_LEFT_COLLISION);
    }
    if (robot_state & RobotState::RIGHT_COLL)
    {
        fire_event(Events::RIGHT_COLLISION);
    }
    else
    {
        fire_event(Events::NO_RIGHT_COLLISION);
    }
    if (robot_state & RobotState::FRONT_COLL)
    {
        fire_event(Events::FRONT_COLLISION);
    }

    ++tick;
    return true;
}

// Loop that controls the actions that are queued up in the actionList.
bool action_loop(void *)
{
    if (current_action_list == nullptr)
        return true;

    if(current_action_list[action_index].action == Actions::TERMINATE)
        return true;

    if (!blocking_event && !blocking_duration)
    {   
        Serial.println(String("action index:") + action_index);
        ++action_index;
        Actions::FunctionTable[current_action_list[action_index].action]();
        blocking_event = current_action_list[action_index].endEvent;
        blocking_duration = current_action_list[action_index].duration;
        last_action_time = millis();
    }
    else if (blocking_duration)
    {
        if ((millis() - last_action_time) > blocking_duration)
        { // has the action surpassed its duration?
            blocking_duration = 0;
            action_loop(nullptr);
        }
    }

    return true;
}

// Inclinometer event loop that should take a reading and fire an event if there is a change in inclination.
bool inclinometer_loop(void *)
{
    static int reading = 0;
    // TODO: take reading
    return true;
}

// Callback for keypad
void on_key_pressed(char key)
{
    Serial.println(String("key_pressed") + key);

    switch (key)
    {
    case 'A':
        requested_motor_state = MS_STATIONARY;
        break;
    case 'B':
        requested_motor_state = MS_FORWARD;
        break;
    case 'C':
        requested_motor_state = MS_REVERSE;
        break;
    default:
        break;
    }
}

// Keypad loop that checks for input and calls any callback functions per key.
bool keypad_loop(void *)
{
    if (!keypad.getKeys())
        return true; // No key states changed.

    for (int i = 0; i < LIST_MAX; ++i)
    {
        if (!keypad.key[i].stateChanged)
            continue;

        switch (keypad.key[i].kstate)
        {
        case PRESSED:
            on_key_pressed(keypad.key[i].kchar);
            break;
        case HOLD:
            break;
        case RELEASED:
            break;
        case IDLE:
            break;
        }
    }

    return true;
}

bool lcd_loop(void *)
{
    lcd.clear();
    lcd.home();
    lcd.println(lcd_message);
    return true;
}

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
    pinMode(Pins::PINGF_TRIG, OUTPUT);
    pinMode(Pins::PINGF_ECHO, INPUT);
    pinMode(Pins::PINGR_TRIG, OUTPUT);
    pinMode(Pins::PINGR_ECHO, INPUT);
    pinMode(Pins::PINGL_TRIG, OUTPUT);
    pinMode(Pins::PINGL_ECHO, INPUT);

    timer.every(20, action_loop);
    timer.every(20, motor_loop);
    timer.every(60, ping_loop);
    //timer.every(80, keypad_loop);
    //timer.every(200, lcd_loop);
    requested_motor_state = MS_FORWARD;
    delay(2000);
}

void loop()
{
    timer.tick<void>();
}
