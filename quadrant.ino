#include <Joystick.h>

const bool DEBUG = true;

Joystick_ Joystick(
    0x03, // hid id
    JOYSTICK_TYPE_JOYSTICK, // type
    1, //button count
    0, // hat switches
    false, // XAxis
    false, // YAxis
    false, // ZAxis
    false, // RxAxis
    false, // RyAxis
    false, // RzAxis
    false, // Rudder
    true,  // Throttle
    false, // Accelerator
    false, // Breaks -- to change
    false  // Steering
);

// LEFT THRUST LEVER
const int L_THRUST_PIN = A0;
const int L_THRUST_REV_SW = 0;
const int L_THRUST_REV_VAL = 523;
bool      L_REV_ON = false; // virtual button to indicate left reverse
int       L_RAW_THRUST = 0;
int       L_CPT_THRUST = 0;
int       L_REV_THRUST = 0;

// GLOBALS
const     int MIN_THROTTLE = L_THRUST_REV_VAL;
constexpr int MAX_THROTTLE = 1023;

void setup()
{
  if (DEBUG) { Serial.begin(9600); }
  Joystick.begin();
  Joystick.setThrottleRange(MIN_THROTTLE, MAX_THROTTLE);
  Joystick.setButton(L_THRUST_REV_SW, 0);
  pinMode(9, INPUT);
}

int last = L_REV_ON;

void loop()
{
  L_RAW_THRUST  = analogRead(L_THRUST_PIN);
  L_CPT_THRUST  = L_REV_ON ? (MAX_THROTTLE - L_RAW_THRUST) : L_RAW_THRUST;
  L_REV_ON = L_RAW_THRUST  < L_THRUST_REV_VAL ? true : false;
  if (DEBUG) { Serial.print(L_RAW_THRUST); Serial.print(" "); Serial.print(L_REV_ON); Serial.println(); }
  if (L_REV_ON != last) {Joystick.setButton(L_THRUST_REV_SW, 1); delay(100); Joystick.setButton(L_THRUST_REV_SW, 0); last = L_REV_ON; }
  Joystick.setThrottle(L_CPT_THRUST);
}
