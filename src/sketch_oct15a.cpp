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
#include <Chomper.h>

int requested_motor_state = -1;

int motor_state = MS_STATIONARY; // Don't modify this directly, use requested_motor_state
int chomp_state = false;
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

Chomper chomper = Chomper(Pins::M_CHOMP);

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

bool chomp_loop(void*) {
    chomper.chomp(millis());
    return true;
}

// Loop that controls the state of the motors.
bool motor_loop(void *)
{
    motor_state = MS_STATIONARY;

    // if (distanceR < 10 && distanceR > 0) {
    //     requested_motor_state = MS_TURN_LEFT;
    // } else if (distanceR > 10 && distanceR < 40) {
    //      requested_motor_state = MS_FORWARD;
    // }

    // if (distanceL < 10 && distanceL > 0) {
    //     requested_motor_state = MS_TURN_RIGHT;
    // } else if (distanceR > 10 && distanceR < 40) {
    //      requested_motor_state = MS_FORWARD;
    // }

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

bool ping(int trig, int echo, int coll_flag, float collision_threshold, float &distance, int on_event, int off_event)
{
    static const int PING_BUFFER = 5;
    static float buffer[PING_BUFFER];
    for (int i = 0; i < PING_BUFFER; ++i)
    {
        buffer[i] = Ping::get_ping(trig, echo);
    }
    RobotUtil::insertionSort(buffer, PING_BUFFER);
    distance = buffer[PING_BUFFER / 2];

    if (distance > 0)
    {
        if (distance < collision_threshold)
            robot_state |= coll_flag;
        else
            robot_state &= ~coll_flag;
        fire_event((robot_state & coll_flag) ? on_event : off_event);
    }
    return distance;
}

// Loop that controls the ping sensors. Only 1 ping sensor is updated every tick.
bool ping_loop(void *)
{
    static int i = 0;
    static int next;

    next = i % 3;
    switch (next)
    {
    case 0:
        ping(Pins::PINGF_TRIG, Pins::PINGF_ECHO, RobotState::FRONT_COLL, DEFAULT_COLLISION_THRESHOLD, distanceF,
             Events::FRONT_COLLISION, Events::NO_FRONT_COLLISION);
        break;
    case 1:
        ping(Pins::PINGL_TRIG, Pins::PINGL_ECHO, RobotState::LEFT_COLL, LEFT_COLLISION_THRESHOLD, distanceL,
             Events::LEFT_COLLISION, Events::NO_LEFT_COLLISION);
        break;
    case 2:
        ping(Pins::PINGR_TRIG, Pins::PINGR_ECHO, RobotState::RIGHT_COLL, RIGHT_COLLISION_THRESHOLD, distanceR,
             Events::RIGHT_COLLISION, Events::NO_RIGHT_COLLISION);
        break;
    }

    ++i;
    return true;
}

// Loop that controls the actions that are queued up in the actionList.
bool action_loop(void *)
{
    if (current_action_list == nullptr)
        return true;

    if ((action_index >= 0) && ((current_action_list[action_index].action == Actions::TERMINATE)))
        return true;

    if (!blocking_event && !blocking_duration)
    {
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
    // analog pin 7

    return true;
}

// Callback for keypad
void on_key_pressed(char key)
{
    Serial.println(String("key_pressed") + key);

    switch (key)
    {
    case '1':
        requested_motor_state = MS_STATIONARY;
        break;
    case '2':
        requested_motor_state = MS_FORWARD;
        break;
    case '3':
        requested_motor_state = MS_REVERSE;
        break;
    case '4':
        Speeds::LEFT_FORWARD += 1;
        break;
    case '5':
        Speeds::LEFT_FORWARD -= 1;
        break;
    case '7':
        Speeds::RIGHT_FORWARD += 1;
        break;
    case '8':
        Speeds::RIGHT_FORWARD -= 1;
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
    static int i = 0;

    // lcd.clear();
    // lcd.home();

    // static char row1[16]{};
    // static char row2[16]{};

    // strcpy(row1, "F: ");
    // dtostrf(distanceF, 11, 4, row1 + 2);

    // if(i % 2) {
    //     strcpy(row2, "L: ");
    //     dtostrf(distanceL, 11, 4, row2 + 3);
    // }
    // else {
    //     strcpy(row2, "R: ");
    //     dtostrf(distanceR, 11, 4, row2 + 3);
    // }

    // lcd.println(row1);
    // lcd.setCursor(0, 1);
    // lcd.println(row2);

    lcd.println(String("LFT SPD:") + Speeds::LEFT_FORWARD);
    lcd.setCursor(0, 1);
    lcd.println(String("RGT SPD:") + Speeds::RIGHT_FORWARD);

    ++i;
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
    timer.every(75, ping_loop);
    timer.every(150, keypad_loop);
    timer.every(250, lcd_loop);
}

void loop()
{
    timer.tick<void>();
}
