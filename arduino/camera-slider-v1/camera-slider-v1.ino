#include <DRV8825.h>
#include <LCD_I2C.h>
//#include <MD_UISwitch.h>
//#include <MD_Menu.h>
#include <Keypad.h>

//---------------------------------------------------
// PINS
//---------------------------------------------------

#define PIN_SWITCH 13
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
#define LCD_ADDRESS 0x27

//---------------------------------------------------
// KEYPAD
//---------------------------------------------------

//const uint8_t KeypadRowPins[] PROGMEM = { 2, 3, 4, 5 };     // connected to keypad row pinouts
//const uint8_t KeypadColPins[] PROGMEM = { 6, 7, 8, 11 };   // connected to the keypad column pinouts
//const char KeypadKeyTable[16 + 1] PROGMEM = "123A456B789C*0#D"; //define the symbols for the keypad
const byte KEYPAD_ROWS = 4; //four rows
const byte KEYPAD_COLS = 3; //three columns
const char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'V', '0', 'X'}};
const byte keypadRowPins[KEYPAD_ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
const byte keypadColPins[KEYPAD_COLS] = {6, 7, 8};    //connect to the column pinouts of the keypad

// https://majicdesigns.github.io/MD_UISwitch/class_m_d___u_i_switch___matrix.html
//MD_UISwitch_Matrix keypad(4, 4, KeypadRowPins, KeypadColPins, KeypadKeyTable);

// https://playground.arduino.cc/Code/Keypad/#Example
Keypad keypad = Keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS);

//---------------------------------------------------
// SWITCH
//---------------------------------------------------

//---------------------------------------------------
// LCD
//---------------------------------------------------

// https://github.com/blackhack/LCD_I2C/blob/master/src/LCD_I2C.cpp
LCD_I2C lcd(LCD_ADDRESS);

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
#define MOTOR_RPM_MAX 120

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
// STRINGS
//---------------------------------------------------

const char str_Main[] = "Main menu";
const char str_Calibrate[] = "Calibrate";
const char str_Duration[] = "Duration";
const char str_BounceMode[] = "Bounce mode";
const char str_Smoothing[] = "Smoothing";
const char str_Start[] = "START";
const char str_Calibrating[] = "Calibrating";
const char str_Working[] = "Working";
const char str_On[] = "On";
const char str_Off[] = "Off";
const char str_Short[] = "Short";
const char str_Medium[] = "Medium";
const char str_Long[] = "Long";
const char str_s[] = " s";

const int strListSize_Main = 5;
const char *strList_Main[strListSize_Main] = {str_Calibrate, str_Duration, str_BounceMode, str_Smoothing, str_Start};

const int strListSize_OnOff = 2;
const char *strList_OnOff[strListSize_OnOff] = {str_Off, str_On};

const int strListSize_Smoothing = 4;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Short, str_Medium, str_Long};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

enum MainState
{
  WORK,
  CALIBRATE,
  SETTINGS,
  SETTINGS_DURATION,
  SETTINGS_BOUNCE_MODE,
  SETTINGS_SMOOTHING,
  SETTINGS_START
};

enum InputAction
{
  NOTHING,
  ACCEPT,
  CANCEL
};

enum Smoothing
{
  OFF,
  SHORT,
  MEDIUM,
  LONG
};

MainState mainState;

bool directionIsForward = true;
bool isCalibrated = false;

long duration = 30 * 1000000;
bool bounceMode = true;
Smoothing smoothing = OFF;

char lcdLineA[16];
char lcdLineB[16];

int inputMainListIndex = 0;
int inputTmpListIndex = 0;
int inputTmpListIndexLast = 0;
int inputTmpValueInt = 0;
int inputTmpValueIntLast = 0;

//---------------------------------------------------
// MAIN
//---------------------------------------------------

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB

  pinMode(PIN_SWITCH, INPUT);

  // DRV8825 automatically setup pinModes at begin
  motorMove.begin(120, 1);

  lcd.begin();
  lcd.backlight();


  setState(MainState::SETTINGS);
}

//---------------------------------------------------
//---------------------------------------------------

void loop()
{
  switch (mainState)
  {
  case MainState::WORK:
    work();
    break;
  case MainState::CALIBRATE:
    calibrate();
    break;
  case MainState::SETTINGS:
    settings();
    break;
  case MainState::SETTINGS_DURATION:
    settingsDuration();
    break;
  case MainState::SETTINGS_BOUNCE_MODE:
    settingsBounceMode();
    break;
  case MainState::SETTINGS_SMOOTHING:
    settingsSmoothing();
    break;
  case MainState::SETTINGS_START:
    settingsStart();
    break;
  }
}

//---------------------------------------------------
// MAIN: SET STATE
//---------------------------------------------------

void setState(MainState state)
{
  Serial.print("setState | mainState=");
  Serial.print(mainState);
  Serial.print(", state=");
  Serial.println(state);

  switch (state)
  {
  case MainState::WORK:
    directionIsForward = true;
    motorMoveSetup(MOTOR_RPM_MAX, 1, motorMove.CONSTANT_SPEED, 1000, 1000);
    motorMoveStart(MOTOR_MOVE_ANGLE_MAX, duration);
    lcdPrint(str_Working, str_Working);
    break;
  case MainState::CALIBRATE:
    directionIsForward = false;
    motorMoveSetup(MOTOR_RPM_MAX, 1, motorMove.CONSTANT_SPEED, 1000, 1000);
    motorMoveStart(MOTOR_MOVE_ANGLE_MAX, 10);
    lcdPrint(str_Calibrating, str_Calibrating);
    break;
  case MainState::SETTINGS:
    inputTmpListIndexLast = -1;
    break;
  case MainState::SETTINGS_DURATION:
    inputTmpValueIntLast = -1;
    inputTmpValueInt = 0;
    break;
  case MainState::SETTINGS_BOUNCE_MODE:
    inputTmpListIndexLast = -1;
    inputTmpListIndex = bounceMode; // false => 0, true => 1
    break;
  case MainState::SETTINGS_SMOOTHING:
    inputTmpListIndexLast = -1;
    inputTmpListIndex = smoothing; // OFF => 0, SHORT => 1, MEDIUM => 2, LONG => 3
    break;
  case MainState::SETTINGS_START:
    lcdPrint(str_Start, str_Start);
    break;
  }

  mainState = state;
}

//---------------------------------------------------
// MAIN: STATE: WORK
//---------------------------------------------------

void work()
{
  long waitTime = motorMove.nextAction();
  if (!waitTime)
  {
    if (bounceMode)
    {
      directionIsForward = !directionIsForward;
      motorMoveStart(MOTOR_MOVE_ANGLE_MAX, duration);
    }
    else
    {
      motorMove.stop();
      setState(MainState::SETTINGS);
    }
  }
}

//---------------------------------------------------
// MAIN: STATE: CALIBRATE
//---------------------------------------------------

void calibrate()
{
  long waitTime = motorMove.nextAction();
  if (waitTime)
  {
    bool switchValue = digitalRead(PIN_SWITCH);
    if (switchValue == HIGH)
    {
      motorMove.stop();
      isCalibrated = true;
      setState(MainState::SETTINGS);
    }
  }
  else
  {
    motorMove.startRotate(360);
  }
}

//---------------------------------------------------
// MAIN: STATE: SETTINGS
//---------------------------------------------------

void settings()
{
  InputAction inputAction = inputList(str_Main, strListSize_Main, strList_Main, &inputMainListIndex);
  if (inputAction == InputAction::ACCEPT)
  {
    switch (inputMainListIndex)
    {
    case 0:
      setState(MainState::CALIBRATE);
      break;
    case 1:
      setState(MainState::SETTINGS_DURATION);
      break;
    case 2:
      setState(MainState::SETTINGS_BOUNCE_MODE);
      break;
    case 3:
      setState(MainState::SETTINGS_SMOOTHING);
      break;
    case 4:
      setState(MainState::SETTINGS_START);
      break;
    }
  }
}

void settingsDuration()
{
  InputAction inputAction = inputValueInt(str_Duration, &inputTmpValueInt, duration, str_s);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    duration = inputTmpValueInt;
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS);
    break;
  }
}

void settingsBounceMode()
{
  InputAction inputAction = inputList(str_BounceMode, strListSize_OnOff, strList_OnOff, &inputTmpListIndex);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    bounceMode = inputTmpListIndex; // 0 => false, 1 => true
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS);
    break;
  }
}

void settingsSmoothing()
{
  InputAction inputAction = inputList(str_Smoothing, strListSize_Smoothing, strList_Smoothing, &inputTmpListIndex);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    smoothing = inputTmpListIndex; // 0 => OFF, 1 => SHORT, 2 => MEDIUM, 3 => LONG
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS);
    break;
  }
}

void settingsStart()
{
}

//---------------------------------------------------

//---------------------------------------------------
// MAIN: HELPERS
//---------------------------------------------------

void motorMoveSetup(float rpm, short microsteps, BasicStepperDriver::Mode mode, short accel, short decel)
{
  motorMove.setRPM(rpm);
  motorMove.setMicrostep(microsteps);
  motorMove.setSpeedProfile(mode, accel, decel);
}

void motorMoveStart(long degrees, long duration)
{
  if (!directionIsForward)
  {
    degrees *= -1;
  }
  degrees = motorMove.calcStepsForRotation(degrees);
  motorMove.startMove(degrees, duration);
}

//---------------------------------------------------

void lcdPrint(char *lineA, char *lineB)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lineA);
  lcd.setCursor(0, 1);
  lcd.print(lineB);
}

void lcdPrintInputListItem(char *header, char *listItem)
{
  strcpy(lcdLineA, header);
  strcat(lcdLineA, ":");

  strcpy(lcdLineB, "> ");
  strcat(lcdLineB, listItem);

  lcdPrint(lcdLineA, lcdLineB);
}

void lcdPrintInputValueInt(char *header, int *value, char *suffix)
{
  strcpy(lcdLineA, header);
  strcat(lcdLineA, ":");

  itoa(*value, lcdLineB, 10);
  strcat(lcdLineB, suffix);

  lcdPrint(lcdLineA, lcdLineB);
}

//---------------------------------------------------

InputAction inputList(char *header, int listSize, char **list, int *index)
{
  if (*index >= listSize)
  {
    *index = 0;
  }
  else if (*index < 0)
  {
    *index = listSize - 1;
  }

  if (inputTmpListIndexLast != *index)
  {
    inputTmpListIndexLast = *index;
    char *currentItem = list[*index];
    lcdPrintInputListItem(header, currentItem);
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case 'V':
      return InputAction::ACCEPT;
    case 'X':
      return InputAction::CANCEL;
    case '2':
    case '4':
      (*index)--; // prev
      break;
    case '6':
    case '8':
      (*index)++; // next
      break;
    }
  }

  return InputAction::NOTHING;
}

InputAction inputValueInt(char *header, int *value, int defaultValue, char *suffix)
{
  if (*value == 0)
  {
    if (inputTmpValueIntLast != defaultValue)
    {
      inputTmpValueIntLast = defaultValue;
      lcdPrintInputValueInt(header, &defaultValue, suffix);
    }
  }
  else
  {
    if (inputTmpValueIntLast != *value)
    {
      inputTmpValueIntLast = *value;
      lcdPrintInputValueInt(header, value, suffix);
    }
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case 'V':
      return InputAction::ACCEPT;
    case 'X':
      return InputAction::CANCEL;
    default:
      *value = ((*value) * 10) + (key - 48);
      break;
    }
  }

  return InputAction::NOTHING;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
