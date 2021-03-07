#include <Joystick.h>

// ________ DEBUG MODE SEMAPHORE
const bool DEBUG = true;

// ________ JOYSTICK DECLARATION
Joystick_ Joystick(
    0x03,                   // hid id
    JOYSTICK_TYPE_JOYSTICK, // type
    3,                      // button count
    0,                      // hat switches
    false,                  // XAxis
    false,                  // YAxis
    false,                  // ZAxis
    false,                  // RxAxis
    false,                  // RyAxis
    false,                  // RzAxis
    false,                  // Rudder
    true,                   // Throttle
    false,                  // Accelerator
    false,                  // Breaks -- to change
    false                   // Steering
);
// ________ END JOYSTICK DECLARATION

// ________ LEFT THRUST LEVER
const int L_THRUST_PIN = A0;
const int L_THRUST_REV_SW = 0;
const int L_THRUST_REV_VAL = 523;

bool L_REV_ON = false; // semaphore to indicate left reverse
int L_RAW_THRUST = 0; // raw analog data from left potentiometer
int L_CPT_THRUST = 0; // computed data from left potentiometer
int L_REV_CURR_STATE = L_REV_ON; // save current reverse state to variable for further computation
// ________ END LEFT THRUST LEVER

// ________ RIGHT THRUST LEVER
const int R_THRUST_PIN = A1;
const int R_THRUST_REV_SW = 0;
const int R_THRUST_REV_VAL = 523;

bool R_REV_ON = false; // semaphore to indicate right reverse
int R_RAW_THRUST = 0; // raw analog data from right potentiometer
int R_CPT_THRUST = 0; // computed data from right potentiometer
int R_REV_CURR_STATE = R_REV_ON; // save current reverse state to variable for further computation
// ________ END RIGHT THRUST LEVER

// ________ ENGINE SWITCHES
const int L_NGN_SWITCH_PIN = 1;
const int R_NGN_SWITCH_PIN = 2;

const int L_NGN_SWITCH_VAL = 0;
const int R_NGN_SWITCH_VAL = 0;
// ________ END ENGINE SWITCHES

// ________ GLOBALS
const int MIN_THROTTLE = L_THRUST_REV_VAL == R_THRUST_REV_VAL ? L_THRUST_REV_VAL : 0; // TODO: it may be splitted in 2 separate minimums
constexpr int MAX_THROTTLE = 1023;
// ________ END GLOBALS

// ________ SETUP
void setup()
{
  if (DEBUG)
  {
    Serial.begin(9600);
  }
  Joystick.begin();
  Joystick.setThrottleRange(MIN_THROTTLE, MAX_THROTTLE);
  
  pinMode(L_NGN_SWITCH_PIN, INPUT_PULLUP);
  pinMode(R_NGN_SWITCH_PIN, INPUT_PULLUP);
}
// ________ END SETUP

void loop()
{
  // ________ LEFT THRUST
  L_RAW_THRUST = analogRead(L_THRUST_PIN);
  L_CPT_THRUST = L_REV_ON ? (MAX_THROTTLE - L_RAW_THRUST) : L_RAW_THRUST;
  L_REV_ON = L_RAW_THRUST < L_THRUST_REV_VAL ? true : false;
  if (DEBUG)
  {
    Serial.print(L_RAW_THRUST);
    Serial.print(" ");
    Serial.print(L_REV_ON);
    Serial.println();
  }
  if (L_REV_ON != L_REV_CURR_STATE)
  {
    Joystick.setButton(L_THRUST_REV_SW, 1);
    delay(100);
    Joystick.setButton(L_THRUST_REV_SW, 0);
    L_REV_CURR_STATE = L_REV_ON;
  }
  Joystick.setThrottle(L_CPT_THRUST);
  // ________ END LEFT THRUST
  
  // ________ ENGINES
  if (digitalRead(L_NGN_SWITCH_PIN) == LOW)
  {
    Joystick.setButton(L_NGN_SWITCH_PIN, 1);
  }
  else
  {
    Joystick.setButton(L_NGN_SWITCH_PIN, 0);
  }
  if (digitalRead(R_NGN_SWITCH_PIN) == LOW)
  {
    Joystick.setButton(R_NGN_SWITCH_PIN, 1);
  }
  else
  {
    Joystick.setButton(R_NGN_SWITCH_PIN, 0);
  }
  // ________ END ENGINES
}
