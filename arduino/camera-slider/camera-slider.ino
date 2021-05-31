#include <DRV8825.h>
#include <LCD_I2C.h>
#include <Keypad.h>
// #include <arduino.h>

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

const byte KEYPAD_ROWS = 4; //four rows
const byte KEYPAD_COLS = 3; //three columns
const char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'V', '0', 'X'}};
const byte keypadRowPins[KEYPAD_ROWS] = {2, 3, 4, 5};
const byte keypadColPins[KEYPAD_COLS] = {6, 7, 8};

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

const int MOTOR_MOVE_DISTANCE PROGMEM = 100;
const int MOTOR_MOVE_DISTANCE_HALF PROGMEM = MOTOR_MOVE_DISTANCE / 2;
const int MOTOR_MOVE_DISTANCE_HALF_PWR PROGMEM = MOTOR_MOVE_DISTANCE_HALF * MOTOR_MOVE_DISTANCE_HALF;
const int MOTOR_MOVE_DISTANCE_IN_ONE_ROTATION PROGMEM = 4;
const int MOTOR_MOVE_ROTATIONS_MAX PROGMEM = MOTOR_MOVE_DISTANCE / MOTOR_MOVE_DISTANCE_IN_ONE_ROTATION;
const long MOTOR_MOVE_DEGREES_MAX PROGMEM = MOTOR_MOVE_ROTATIONS_MAX * 360;
const long MOTOR_MOVE_DEGREES_WORK_MARGIN PROGMEM = 180;
const long MOTOR_MOVE_DEGREES_WORK_RIDE PROGMEM = MOTOR_MOVE_DEGREES_MAX - (2 * MOTOR_MOVE_DEGREES_WORK_MARGIN);
const long MOTOR_MOVE_DURATION_MIN PROGMEM = 1;
const long MOTOR_MOVE_DURATION_MAX PROGMEM = 60 * 60 * 24;
const int MOTOR_MOVE_RPM_MAX PROGMEM = 600;

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
const char str_Rotation[] = "Rotation";
const char str_TargetDistance[] = "Target distance";
const char str_AngleLeft[] = "Angle (LEFT)";
const char str_AngleRight[] = "Angle (RIGHT)";
const char str_Start[] = "[START]";
const char str_Calibrating[] = "Calibrating";
const char str_PleaseWait[] = "Please wait ...";
const char str_Summary[] = "Summary";
const char str_Working[] = "Working";
const char str_On[] = "On";
const char str_Off[] = "Off";
const char str_Short[] = "Short";
const char str_Medium[] = "Medium";
const char str_Long[] = "Long";
const char str_suffix_s[] = " s";
const char str_suffix_cm[] = " cm";
const char str_suffix_degree[] = " °";
const char str_direction_forward[] = "   >>>";
const char str_direction_backward[] = "   <<<";
const char str_Stopped[] = "Stopped";
const char str_Accelerating[] = "Accelerating";
const char str_Cruising[] = "Cruising";
const char str_Decelerating[] = "Decelerating";

const int strListSize_Main = 6;
const char *strList_Main[strListSize_Main] = {str_Calibrate, str_Duration, str_BounceMode, str_Smoothing, str_Rotation, str_Start};

const int strListSize_BounceMode = 2;
const char *strList_BounceMode[strListSize_BounceMode] = {str_Off, str_On};

const int strListSize_Smoothing = 4;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Short, str_Medium, str_Long};

const int strListSize_Rotation = 3;
const char *strList_Rotation[strListSize_Rotation] = {str_TargetDistance, str_AngleLeft, str_AngleRight};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

const int TARGET_DISTANCE_MIN PROGMEM = 10; // 10 cm
const int TARGET_DISTANCE_MAX PROGMEM = 100 * 1000 * 10; // 10km
const int ANGLE_MIN PROGMEM = 0;
const int ANGLE_MAX PROGMEM = 360;

enum MainState
{
  WORK_SUMMARY,
  WORK,
  WORK_RIDE,
  CALIBRATE,
  SETTINGS,
  SETTINGS_DURATION,
  SETTINGS_BOUNCE_MODE,
  SETTINGS_SMOOTHING,
  SETTINGS_ROTATION,
  SETTINGS_ROTATION_TARGET_DISTANCE,
  SETTINGS_ROTATION_ANGLE_LEFT,
  SETTINGS_ROTATION_ANGLE_RIGHT
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

long duration = 60;
bool bounceMode = true;
Smoothing smoothing = OFF;
BasicStepperDriver::Mode smoothingMode = BasicStepperDriver::Mode::CONSTANT_SPEED;
short smoothingValue = 1;
int targetDistance = 10;
double angleLeft = 180.0;
double angleRight = 180.0;

bool isCalibrated = false;
bool directionIsForward = true;

char lcdLineA[16];
char lcdLineB[16];
char lcdValue[16];

int inputSettingsListIndex = 0;
int inputSettingsRotationListIndex = 0;
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

  motorMove.begin(1, 32);

  lcd.begin();
  lcd.backlight();

  setupSmoothing();

  setState(MainState::SETTINGS);
}

//---------------------------------------------------
//---------------------------------------------------

void loop()
{
  switch (mainState)
  {
  case MainState::WORK_RIDE:
    workRide();
    break;
  case MainState::CALIBRATE:
    calibrate();
    break;
  case MainState::WORK:
    work();
    break;
  case MainState::WORK_SUMMARY:
    workSummary();
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
  case MainState::SETTINGS_ROTATION:
    settingsRotation();
    break;
  case MainState::SETTINGS_ROTATION_TARGET_DISTANCE:
    settingsRotationTargetDistance();
    break;
  case MainState::SETTINGS_ROTATION_ANGLE_LEFT:
    settingsRotationAngleLeft();
    break;
  case MainState::SETTINGS_ROTATION_ANGLE_RIGHT:
    settingsRotationAngleRight();
    break;
  }
}

//---------------------------------------------------
// MAIN: SET STATE
//---------------------------------------------------

void setState(MainState state)
{
  // Serial.print("setState | mainState=");
  // Serial.print(mainState);
  // Serial.print(", state=");
  // Serial.println(state);

  switch (state)
  {
  case MainState::WORK_SUMMARY:
    lcdPrintWorkSummary();
    break;
  case MainState::WORK:
    if (isCalibrated)
    {
      lcdClear();
      isCalibrated = false;
      directionIsForward = true;
      motorMoveStart(MOTOR_MOVE_DEGREES_WORK_MARGIN, 1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1, 1);
    }
    else
    {
      setState(MainState::CALIBRATE);
      return;
    }
    break;
  case MainState::WORK_RIDE:
    motorMoveStart(MOTOR_MOVE_DEGREES_WORK_RIDE, duration, smoothingMode, smoothingValue, smoothingValue);
    lcdPrintWorkRide(directionIsForward, motorMove.getCurrentState());
    break;
  case MainState::CALIBRATE:
    lcdPrint(str_Calibrating, str_PleaseWait);
    directionIsForward = false;
    motorMoveStart(MOTOR_MOVE_DEGREES_MAX, 1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1, 1);
    break;
  case MainState::SETTINGS:
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
  case MainState::SETTINGS_ROTATION:
    break;
  case MainState::SETTINGS_ROTATION_TARGET_DISTANCE:
    inputTmpValueIntLast = -1;
    inputTmpValueInt = 0;
    break;
  case MainState::SETTINGS_ROTATION_ANGLE_LEFT:
    inputTmpValueIntLast = -1;
    inputTmpValueInt = 0;
    break;
  case MainState::SETTINGS_ROTATION_ANGLE_RIGHT:
    inputTmpValueIntLast = -1;
    inputTmpValueInt = 0;
    break;
  }

  mainState = state;
}

//---------------------------------------------------
// MAIN: STATE: WORK
//---------------------------------------------------

void workSummary()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case 'V':
      setState(MainState::WORK);
      break;
    case 'X':
      setState(MainState::SETTINGS);
      break;
    }
  }
}

void work()
{
  long waitTime = motorMove.nextAction();
  if (!waitTime)
  {
    delay(1000);
    setState(MainState::WORK_RIDE);
  }
}

void workRide()
{
  long waitTime = motorMove.nextAction();
  if (waitTime)
  {
    if (!directionIsForward)
    {
      bool switchValue = digitalRead(PIN_SWITCH);
      if (switchValue == HIGH)
      {
        workRideEnd();
        return;
      }
    }
    char key = keypad.getKey();
    if (key == 'X')
    {
      workEnd();
      return;
    }
  }
  else
  {
    workRideEnd();
  }
}

void workRideEnd()
{
  if (bounceMode)
  {
    directionIsForward = !directionIsForward;
    motorMoveStart(MOTOR_MOVE_DEGREES_WORK_RIDE, duration, smoothingMode, smoothingValue, smoothingValue);
    lcdPrintWorkRide(directionIsForward, motorMove.getCurrentState());
  }
  else
  {
    workEnd();
  }
}

void workEnd()
{
  motorMoveStop();
  setState(MainState::SETTINGS);
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
      isCalibrated = true;
      calibrateEnd();
      return;
    }
    char key = keypad.getKey();
    if (key == 'X')
    {
      calibrateEnd();
    }
  }
  else
  {
    motorMoveStart(360, 1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1, 1);
  }
}

void calibrateEnd()
{
  motorMoveStop();
  setState(MainState::SETTINGS);
}

//---------------------------------------------------
// MAIN: STATE: SETTINGS
//---------------------------------------------------

void settings()
{
  InputAction inputAction = inputList(str_Main, strListSize_Main, strList_Main, &inputSettingsListIndex);
  if (inputAction == InputAction::ACCEPT)
  {
    switch (inputSettingsListIndex)
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
      setState(MainState::SETTINGS_ROTATION);
      break;
    case 5:
      setState(MainState::WORK_SUMMARY);
      break;
    }
  }
}

void settingsDuration()
{
  InputAction inputAction = inputValueInt(str_Duration, &inputTmpValueInt, duration, str_suffix_s);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    inputTmpValueInt = min(max(inputTmpValueInt, MOTOR_MOVE_DURATION_MIN), MOTOR_MOVE_DURATION_MAX);
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
  InputAction inputAction = inputList(str_BounceMode, strListSize_BounceMode, strList_BounceMode, &inputTmpListIndex);
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
    setupSmoothing();
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS);
    break;
  }
}

void settingsRotation()
{
  InputAction inputAction = inputList(str_Rotation, strListSize_Rotation, strList_Rotation, &inputSettingsRotationListIndex);
  switch (inputAction)
  {
  case InputAction::ACCEPT:
    switch (inputSettingsRotationListIndex)
    {
    case 0:
      setState(MainState::SETTINGS_ROTATION_TARGET_DISTANCE);
      break;
    case 1:
      setState(MainState::SETTINGS_ROTATION_ANGLE_LEFT);
      break;
    case 2:
      setState(MainState::SETTINGS_ROTATION_ANGLE_RIGHT);
      break;
    }
    break;
  case InputAction::CANCEL:
    setState(MainState::SETTINGS);
    break;
  }
}

void settingsRotationTargetDistance()
{
  InputAction inputAction = inputValueInt(str_TargetDistance, &inputTmpValueInt, targetDistance, str_suffix_cm);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    inputTmpValueInt = min(max(inputTmpValueInt, TARGET_DISTANCE_MIN), TARGET_DISTANCE_MAX);
    targetDistance = inputTmpValueInt;
    double angleBase = calculateAngleBase();
    angleLeft = 270.0 - angleBase;
    angleRight = 90.0 + angleBase;
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS_ROTATION);
    break;
  }
}

void settingsRotationAngleLeft()
{
  InputAction inputAction = inputValueInt(str_AngleLeft, &inputTmpValueInt, angleLeft, str_suffix_degree);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    inputTmpValueInt = min(max(inputTmpValueInt, ANGLE_MIN), ANGLE_MAX);
    angleLeft = inputTmpValueInt;
    targetDistance = 0;
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS_ROTATION);
    break;
  }
}

void settingsRotationAngleRight()
{
  InputAction inputAction = inputValueInt(str_AngleLeft, &inputTmpValueInt, angleRight, str_suffix_degree);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    inputTmpValueInt = min(max(inputTmpValueInt, ANGLE_MIN), ANGLE_MAX);
    angleRight = inputTmpValueInt;
    targetDistance = 0;
    setState(MainState::SETTINGS);
    break;
  case (InputAction::CANCEL):
    setState(MainState::SETTINGS_ROTATION);
    break;
  }
}

//---------------------------------------------------
// MAIN: HELPERS
//---------------------------------------------------

void motorMoveStart(long degrees, long time, BasicStepperDriver::Mode mode, short accel, short decel) //, short microsteps)
{
  Serial.print("motorMoveStart | input | degrees=");
  Serial.print(degrees);
  Serial.print(", time=");
  Serial.println(time);

  degrees = min(degrees, MOTOR_MOVE_DEGREES_MAX);
  time = min(max(time, MOTOR_MOVE_DURATION_MIN), MOTOR_MOVE_DURATION_MAX);

  Serial.print("motorMoveStart | min,max | degrees=");
  Serial.print(degrees);
  Serial.print(", time=");
  Serial.println(time);

  float rotations = degrees / 360.0;
  float rpm = rotations / (float(time) / 60.0);
  rpm = min(rpm, float(MOTOR_MOVE_RPM_MAX));
  motorMove.setRPM(rpm);

  long timeByRpm = long((rotations / rpm) * 60.0);

  Serial.print("motorMoveStart | rpm | rotations=");
  Serial.print(rotations);
  Serial.print(", rpm=");
  Serial.print(rpm);
  Serial.print(", timeByRpm=");
  Serial.println(timeByRpm);

  // motorMove.setMicrostep(microsteps);
  motorMove.setSpeedProfile(mode, accel, decel);

  if (directionIsForward)
  {
    degrees *= -1;
  }
  long steps = motorMove.calcStepsForRotation(degrees);

  Serial.print("motorMoveStart | start | degrees=");
  Serial.print(degrees);
  Serial.print(", steps=");
  Serial.println(steps);

  motorMove.startMove(steps, timeByRpm);
}

void motorMoveStop()
{
  motorMove.stop();
}

//---------------------------------------------------

void setupSmoothing()
{
  smoothingMode = (smoothing == Smoothing::OFF) ? BasicStepperDriver::Mode::CONSTANT_SPEED : BasicStepperDriver::Mode::LINEAR_SPEED;
  switch (smoothing)
  {
  case Smoothing::OFF:
    smoothingValue = 1;
    break;
  case Smoothing::SHORT:
    smoothingValue = 1000;
    break;
  case Smoothing::MEDIUM:
    smoothingValue = 2000;
    break;
  case Smoothing::LONG:
    smoothingValue = 3000;
    break;
  }
}

//---------------------------------------------------

void lcdClear()
{
  lcd.clear();
}

void lcdPrint(char *lineA, char *lineB)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lineA);
  lcd.setCursor(0, 1);
  lcd.print(lineB);
}

void lcdPrintWorkSummary()
{
  strcpy(lcdLineB, "D: ");
  strcat(lcdLineB, itoa(duration, lcdValue, 10));
  strcat(lcdLineB, str_suffix_s);

  lcdPrint(str_Summary, lcdLineB);
}

void lcdPrintWorkRide(bool direction, BasicStepperDriver::State state)
{

  strcpy(lcdLineA, str_Working);
  strcat(lcdLineA, (direction) ? str_direction_forward : str_direction_backward);

  switch (state)
  {
  case BasicStepperDriver::State::STOPPED:
    strcpy(lcdLineB, str_Stopped);
    break;
  case BasicStepperDriver::State::ACCELERATING:
    strcpy(lcdLineB, str_Accelerating);
    break;
  case BasicStepperDriver::State::CRUISING:
    strcpy(lcdLineB, str_Cruising);
    break;
  case BasicStepperDriver::State::DECELERATING:
    strcpy(lcdLineB, str_Decelerating);
    break;
  }

  lcdPrint(lcdLineA, lcdLineB);
}

void lcdPrintInputListItem(char *header, char *listItem)
{
  strcpy(lcdLineA, header);
  strcat(lcdLineA, ":");

  strcpy(lcdLineB, "> ");
  strcat(lcdLineB, listItem);

  lcdPrint(lcdLineA, lcdLineB);
}

void lcdPrintInputValueInt(char *header, int value, char *suffix)
{
  strcpy(lcdLineA, header);
  strcat(lcdLineA, ":");

  itoa(value, lcdLineB, 10);
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
      lcdPrintInputValueInt(header, defaultValue, suffix);
    }
  }
  else
  {
    if (inputTmpValueIntLast != *value)
    {
      inputTmpValueIntLast = *value;
      lcdPrintInputValueInt(header, *value, suffix);
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

/*
let TD = 10;
const L = 100;

const halfL = L / 2;
console.log('halfL=' + halfL);

const halfLPwr = halfL * halfL;
console.log('halfLPwr=' + halfLPwr);

const SD = Math.sqrt(TD * TD + halfLPwr);
console.log('SD=' + SD);

const sinA = TD / SD;
console.log('sinA=' + sinA);
const A = Math.asin(sinA) * 180/Math.PI
console.log('A=' + A);
*/
double calculateAngleBase()
{
  double sideDistance = sqrt(targetDistance * targetDistance + MOTOR_MOVE_DISTANCE_HALF_PWR);
  double sinAngle = targetDistance / sideDistance;
  double angleBase = asin(sinAngle) * RAD_TO_DEG;
  return angleBase;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
