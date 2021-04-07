#include <DRV8825.h>
#include <LCD_I2C.h>
//#include <MD_UISwitch.h>
//#include <MD_Menu.h>
#include <Keypad.h>

//---------------------------------------------------
// PINS
//---------------------------------------------------

#define PIN_SWITCH 13
#define PIN_LCD_ADDRESS 0x27
#define PIN_MOTOR_MOVE_DIR 10
#define PIN_MOTOR_MOVE_STEP 9
#define PIN_MOTOR_MOVE_M0 A2
#define PIN_MOTOR_MOVE_M1 A1
#define PIN_MOTOR_MOVE_M2 A0
#define PIN_MOTOR_ROTATE_DIR 12
#define PIN_MOTOR_ROTATE_STEP 11
#define PIN_MOTOR_ROTATE_M0 A7
#define PIN_MOTOR_ROTATE_M1 A6
#define PIN_MOTOR_ROTATE_M2 A3

//---------------------------------------------------
// KEYPAD
//---------------------------------------------------

//const uint8_t KeypadRowPins[] PROGMEM = { 2, 3, 4, 5 };     // connected to keypad row pinouts
//const uint8_t KeypadColPins[] PROGMEM = { 6, 7, 8, 11 };   // connected to the keypad column pinouts
//const char KeypadKeyTable[16 + 1] PROGMEM = "123A456B789C*0#D"; //define the symbols for the keypad
const byte KEYPAD_ROWS PROGMEM = 4; //four rows
const byte KEYPAD_COLS PROGMEM = 3; //three columns
const char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] PROGMEM = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'#', '0', '*'}
};
const byte keypadRowPins[KEYPAD_ROWS] PROGMEM = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
const byte keypadColPins[KEYPAD_COLS] PROGMEM = {6, 7, 8}; //connect to the column pinouts of the keypad

// https://majicdesigns.github.io/MD_UISwitch/class_m_d___u_i_switch___matrix.html
//MD_UISwitch_Matrix keypad(4, 4, KeypadRowPins, KeypadColPins, KeypadKeyTable);

// https://playground.arduino.cc/Code/Keypad/#Example
Keypad keypad = Keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS );

//---------------------------------------------------
// SWITCH
//---------------------------------------------------

//---------------------------------------------------
// LCD
//---------------------------------------------------

// https://github.com/blackhack/LCD_I2C/blob/master/src/LCD_I2C.cpp
LCD_I2C lcd(PIN_LCD_ADDRESS);

//---------------------------------------------------
// MOTOR
//---------------------------------------------------

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
// Target RPM for cruise speed
#define MOTOR_RPM 120
// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MOTOR_MICROSTEPS 16
#define MOTOR_RPM_MAX 600

/**
   distance per 1 rotation = 4 cm
   max distance = 100 cm
   max amount of rotations = 100 / 4 = 20 rotations
   max angle = 20 * 360 = 7200 degree
*/
const int MOTOR_MOVE_ANGLE_MAX PROGMEM = 7200;

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
DRV8825 motorMove(MOTOR_STEPS, PIN_MOTOR_MOVE_DIR, PIN_MOTOR_MOVE_STEP, PIN_MOTOR_MOVE_M0, PIN_MOTOR_MOVE_M1, PIN_MOTOR_MOVE_M2);
DRV8825 motorRotate(MOTOR_STEPS, PIN_MOTOR_ROTATE_DIR, PIN_MOTOR_ROTATE_STEP, PIN_MOTOR_ROTATE_M0, PIN_MOTOR_ROTATE_M1, PIN_MOTOR_ROTATE_M2);

//---------------------------------------------------
// MENU
//---------------------------------------------------

const char str_Calibrate[] PROGMEM = "Calibrate";
const char str_DurationTime[] PROGMEM = "Duration time";
const char str_BounceMode[] PROGMEM = "Bounce mode";
const char str_Smoothing[] PROGMEM = "Smoothing";
const char str_Start[] PROGMEM = "START";
const char str_Calibrating[] PROGMEM = "Calibrating";
const char str_Working[] PROGMEM = "Working";

const char *menuMain[] = {str_Calibrate, str_DurationTime, str_BounceMode, str_Smoothing, str_Start };

//---------------------------------------------------
// MAIN
//---------------------------------------------------

enum MainState { WORK,
                 WORK_OPERATING,
                 CALIBRATE,
                 CALIBRATE_OPERATING,
                 CONFIGURATION,
                 CONFIGURATION_MAIN,
                 CONFIGURATION_SET_DURATION,
                 CONFIGURATION_SET_BOUNCE_MODE,
                 CONFIGURATION_SET_SMOOTHING,
                 CONFIGURATION_SUMMARY
               };

enum Smoothing { DISABLED, SMALL, NORMAL, LARGE };

// main
MainState mainState = CALIBRATE;

long duration = 15;
bool bounceMode = true;
Smoothing smoothing = DISABLED;

bool directionIsForward = true;

//---------------------------------------------------
// MAIN: SETUP
//---------------------------------------------------

void setup() {
  //  Serial.begin(9600);
  //  while (!Serial) ; // wait for serial port to connect. Needed for native USB

  // DRV8825 automatically setup pinModes at begin
  pinMode(PIN_SWITCH, INPUT);

  motorMove.begin(120, 1);
}

//---------------------------------------------------
// MAIN: LOOP
//---------------------------------------------------

void loop() {
  switch (mainState) {
    case WORK:
      work();
      break;
    case WORK_OPERATING:
      workOperating();
      break;
    case CALIBRATE:
      calibrate();
      break;
    case CALIBRATE_OPERATING:
      calibrateOperating();
      break;
    case CONFIGURATION:
      break;
    case CONFIGURATION_MAIN:
      break;
    case CONFIGURATION_SET_DURATION:
      break;
    case CONFIGURATION_SET_BOUNCE_MODE:
      break;
    case CONFIGURATION_SET_SMOOTHING:
      break;
    case CONFIGURATION_SUMMARY:
      break;

  }
}

//---------------------------------------------------
// MAIN: STATE: WORK
//---------------------------------------------------

void work() {
  directionIsForward = true;
  motorMoveSetup(MOTOR_RPM_MAX, 1, motorMove.CONSTANT_SPEED, 0, 0);
  motorMoveStart(MOTOR_MOVE_ANGLE_MAX, duration);
  mainState = WORK_OPERATING;
}

void workOperating() {
  lcdPrint(str_Working, str_Working);

  long waitTime = motorMove.nextAction();
  if (!waitTime) {
    if (bounceMode) {
      directionIsForward = !directionIsForward;
      motorMoveStart(MOTOR_MOVE_ANGLE_MAX, duration);
    } else {
      motorMove.stop();
      mainState = CONFIGURATION;
    }
  }
}

//---------------------------------------------------
// MAIN: STATE: CALIBRATE
//---------------------------------------------------

void calibrate() {
  directionIsForward = false;
  motorMoveSetup(MOTOR_RPM_MAX, 1, motorMove.CONSTANT_SPEED, 0, 0);
  motorMoveStart(MOTOR_MOVE_ANGLE_MAX, 10);
  mainState = CALIBRATE_OPERATING;
}

void calibrateOperating() {
  lcdPrint(str_Calibrating, str_Calibrating);

  long waitTime = motorMove.nextAction();
  if (waitTime) {
    bool switchValue = digitalRead(PIN_SWITCH);
    if (switchValue == HIGH) {
      motorMove.stop();
      mainState = CONFIGURATION;
    }
  } else {
    motorMove.startRotate(360);
  }
}

//---------------------------------------------------
// MAIN: STATE: CONFIGURATION
//---------------------------------------------------

void configuration() {

}

void configurationMain() {

}

void configurationSetDuration() {

}

void configurationSetBounceMode() {

}

void configurationSetSmoothing() {

}

void configurationSummary() {

}

//---------------------------------------------------
// MAIN: HELPERS
//---------------------------------------------------

void lcdPrint(char *lineA, char *lineB) {
  lcd.setCursor(0, 0);
  lcd.print(lineA);
  lcd.setCursor(0, 1);
  lcd.print(lineB);
}

void motorMoveSetup(float rpm, short microsteps, BasicStepperDriver::Mode mode, short accel, short decel) {
  motorMove.setRPM(rpm);
  motorMove.setMicrostep(microsteps);
  motorMove.setSpeedProfile(mode, accel, decel);
}

void motorMoveStart(long degrees, long duration) {
  if (!directionIsForward) {
    degrees *= -1;
  }
  degrees = motorMove.calcStepsForRotation(degrees);
  motorMove.startMove(degrees, duration);
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
