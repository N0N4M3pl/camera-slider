#include <DRV8825.h>
#include <LCD_I2C.h>
#include <Keypad.h>
// #include <arduino.h>

//---------------------------------------------------
// PINS
//---------------------------------------------------

#define PIN_SWITCH 13
#define PIN_MOTOR_SLIDE_DIR 10
#define PIN_MOTOR_SLIDE_STEP 9
#define PIN_MOTOR_SLIDE_M0 A2
#define PIN_MOTOR_SLIDE_M1 A1
#define PIN_MOTOR_SLIDE_M2 A0
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

const int MOTOR_RPM_MAX PROGMEM = 600;
const int MOTOR_SLIDE_DISTANCE PROGMEM = 100;
const int MOTOR_SLIDE_DISTANCE_HALF PROGMEM = MOTOR_SLIDE_DISTANCE / 2;
const int MOTOR_SLIDE_DISTANCE_HALF_PWR PROGMEM = MOTOR_SLIDE_DISTANCE_HALF * MOTOR_SLIDE_DISTANCE_HALF;
const int MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION PROGMEM = 4;
const int MOTOR_SLIDE_ROTATIONS PROGMEM = MOTOR_SLIDE_DISTANCE / MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION;
const long MOTOR_SLIDE_DEGREES_MAX PROGMEM = (MOTOR_SLIDE_ROTATIONS * 360);
const long MOTOR_SLIDE_DEGREES_MOVE_MARGIN PROGMEM = 180;
const long MOTOR_SLIDE_DEGREES_MOVE PROGMEM = MOTOR_SLIDE_DEGREES_MAX - MOTOR_SLIDE_DEGREES_MOVE_MARGIN;
const long MOTOR_SLIDE_DEGREES_MOVE_HALF PROGMEM = MOTOR_SLIDE_DEGREES_MOVE / 2;

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
DRV8825 motorSlide(MOTOR_STEPS, PIN_MOTOR_SLIDE_DIR, PIN_MOTOR_SLIDE_STEP, PIN_MOTOR_SLIDE_M0, PIN_MOTOR_SLIDE_M1, PIN_MOTOR_SLIDE_M2);
DRV8825 motorRotate(MOTOR_STEPS, PIN_MOTOR_ROTATE_DIR, PIN_MOTOR_ROTATE_STEP, PIN_MOTOR_ROTATE_M0, PIN_MOTOR_ROTATE_M1, PIN_MOTOR_ROTATE_M2);

//---------------------------------------------------
// STRINGS
//---------------------------------------------------

const char str_Main[] = "Main menu";
const char str_Duration[] = "Duration";
const char str_BounceMode[] = "Bounce mode";
const char str_Smoothing[] = "Smoothing";
const char str_Rotation[] = "Rotation";
const char str_FocalDistance[] = "Focal distance";
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

const int strListSize_Main = 5;
const char *strList_Main[strListSize_Main] = {str_Duration, str_BounceMode, str_Smoothing, str_Rotation, str_Start};

const int strListSize_BounceMode = 2;
const char *strList_BounceMode[strListSize_BounceMode] = {str_Off, str_On};

const int strListSize_Smoothing = 4;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Short, str_Medium, str_Long};

const int strListSize_Rotation = 3;
const char *strList_Rotation[strListSize_Rotation] = {str_FocalDistance, str_AngleLeft, str_AngleRight};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

const long DURATION_MIN PROGMEM = 1;                    // 1 sec
const long DURATION_MAX PROGMEM = 60 * 60 * 24;         // 1 day
const int FOCAL_DISTANCE_MIN PROGMEM = 10;              // 10 cm
const int FOCAL_DISTANCE_MAX PROGMEM = 100 * 1000 * 10; // 10 km
const double ANGLE_MIN PROGMEM = 0.0;
const double ANGLE_MAX PROGMEM = 360.0;
const double ANGLE_AHEAD PROGMEM = 180.0;

enum MainState
{
  // WORK_SUMMARY,
  // WORK,
  // WORK_RIDE,
  // CALIBRATE,

  MOVE,

  MENU_MAIN,
  MENU_DURATION,
  MENU_BOUNCE_MODE,
  MENU_SMOOTHING,
  MENU_ROTATION,
  MENU_ROTATION_FOCAL_DISTANCE,
  MENU_ROTATION_ANGLE_LEFT,
  MENU_ROTATION_ANGLE_RIGHT,
  MENU_START
};

enum Position
{
  UNKNOWN,
  LEFT,
  CENTER,
  RIGHT
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

MainState _mainState;
Position _positionCurrent = UNKNOWN;
Position _positionTarget = UNKNOWN;

bool _bounceMode;
long _duration;
BasicStepperDriver::Mode _smoothingMode = BasicStepperDriver::Mode::CONSTANT_SPEED;
short _smoothingValue = 1;
MainState _nextState;

bool _moveDirectionToRight;
double _moveDegreesSlide = 0;
double _moveDegreesRotate = 0;

long _configDuration = 60;
bool _configBounceMode = true;
Smoothing _configSmoothing = OFF;
int _configFocalDistance = FOCAL_DISTANCE_MIN;
double _configAngleLeft = ANGLE_AHEAD;
double _configAngleRight = ANGLE_AHEAD;

int _inputSettingsListIndex = 0;
int _inputSettingsRotationListIndex = 0;
int _inputTmpListIndex = 0;
int _inputTmpListIndexLast = 0;
int _inputTmpValueInt = 0;
int _inputTmpValueIntLast = 0;

char _lcdLineA[16];
char _lcdLineB[16];
char _lcdValue[16];

//---------------------------------------------------
// MAIN
//---------------------------------------------------

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB

  pinMode(PIN_SWITCH, INPUT);

  motorSlide.begin(1, 32);

  lcd.begin();
  lcd.backlight();

  moveSetup(Position::LEFT, false, 1, Smoothing::OFF, MainState::MENU_MAIN));
  setState(MainState::MOVE);
}

//---------------------------------------------------
//---------------------------------------------------

void loop()
{
  switch (_mainState)
  {
  case MainState::MOVE:
    moveLoop();
    break;
  case MainState::MENU_MAIN:
    menuMainLoop();
    break;
  case MainState::MENU_DURATION:
    menuDurationLoop();
    break;
  case MainState::MENU_BOUNCE_MODE:
    menuBounceModeLoop();
    break;
  case MainState::MENU_SMOOTHING:
    menuSmoothingLoop();
    break;
  case MainState::MENU_ROTATION:
    menuRotationLoop();
    break;
  case MainState::MENU_ROTATION_FOCAL_DISTANCE:
    menuRotationFocalDistanceLoop();
    break;
  case MainState::MENU_ROTATION_ANGLE_LEFT:
    menuRotationAngleLeftLoop();
    break;
  case MainState::MENU_ROTATION_ANGLE_RIGHT:
    menuRotationAngleRightLoop();
    break;
  case MainState::MENU_START:
    menuStartLoop();
    break;
  }
}

//---------------------------------------------------
// MAIN: SET STATE
//---------------------------------------------------

void setState(MainState mainState)
{
  _mainState = mainState;

  switch (_mainState)
  {
  case MainState::MOVE:
    moveStart();
    break;
  case MainState::MENU_MAIN:
    break;
  case MainState::MENU_DURATION:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = 0;
    break;
  case MainState::MENU_BOUNCE_MODE:
    _inputTmpListIndexLast = -1;
    _inputTmpListIndex = _bounceMode; // false => 0, true => 1
    break;
  case MainState::MENU_SMOOTHING:
    _inputTmpListIndexLast = -1;
    _inputTmpListIndex = _configSmoothing; // OFF => 0, SHORT => 1, MEDIUM => 2, LONG => 3
    break;
  case MainState::MENU_ROTATION:
    break;
  case MainState::MENU_ROTATION_FOCAL_DISTANCE:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = FOCAL_DISTANCE_MIN;
    break;
  case MainState::MENU_ROTATION_ANGLE_LEFT:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = ANGLE_AHEAD;
    break;
  case MainState::MENU_ROTATION_ANGLE_RIGHT:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = ANGLE_AHEAD;
    break;
  case MainState::MENU_START:
    lcdPrintMenuStart();
    break;
  }
}

//---------------------------------------------------
// MAIN: STATE: MOVE
//---------------------------------------------------

void moveSetup(Position positionTarget, bool bounceMode, long duration, Smoothing smoothing, MainState nextState)
{
  _positionTarget = positionTarget;
  _bounceMode = bounceMode;
  _duration = duration;

  _smoothingMode = (smoothing == Smoothing::OFF) ? BasicStepperDriver::Mode::CONSTANT_SPEED : BasicStepperDriver::Mode::LINEAR_SPEED;
  switch (smoothing)
  {
  case Smoothing::OFF:
    _smoothingValue = 1;
    break;
  case Smoothing::SHORT:
    _smoothingValue = 1000;
    break;
  case Smoothing::MEDIUM:
    _smoothingValue = 2000;
    break;
  case Smoothing::LONG:
    _smoothingValue = 3000;
    break;
  }

  _nextState = nextState;
}

void moveStart()
{
  if (_positionCurrent == Position::UNKNOWN)
  {
    lcdPrint(str_Calibrating, str_PleaseWait);
    _moveDirectionToRight = false;
    _moveDegreesSlide = MOTOR_SLIDE_DEGREES_MAX;
    moveMotorStart(1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1);
  }
  else if (_positionCurrent == _positionTarget)
  {
    if (_bounceMode)
    {
      _positionTarget = (_positionTarget == Position::RIGHT) ? Position::LEFT : Position::RIGHT;
      _moveDirectionToRight = !_moveDirectionToRight;
      lcdPrintMove(_moveDirectionToRight, motorSlide.getCurrentState());
      moveMotorStart(_duration, _smoothingMode, _smoothingValue);
    }
    else
    {
      moveStop();
    }
  }
  else
  {
    switch (_positionCurrent)
    {
    case Position::LEFT:
    case Position::RIGHT:
      _moveDirectionToRight = (_positionCurrent == Position::LEFT);
      _moveDegreesSlide = (_positionTarget == Position::CENTER) ? MOTOR_SLIDE_DEGREES_MOVE_HALF : MOTOR_SLIDE_DEGREES_MOVE;
      break;
    case Position::CENTER:
      _moveDirectionToRight = (_positionTarget == Position::RIGHT);
      _moveDegreesSlide = MOTOR_SLIDE_DEGREES_MOVE_HALF;
      break;
    }

    lcdPrintMove(_moveDirectionToRight, motorSlide.getCurrentState());
    moveMotorStart(_duration, _smoothingMode, _smoothingValue);
  }
}

void moveStop()
{
  motorSlideStop();
  motorRotateStop();
  setState(_nextState);
}

void moveMotorStart(long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue)
{
  motorSlideStart(_moveDegreesSlide, time, smoothingMode, smoothingValue, smoothingValue);
  if (_moveDegreesRotate > 0)
  {
    // motorRotationStart
  }
}

void moveLoop()
{
  long waitTime = motorSlide.nextAction();
  if (waitTime)
  {
    if (!_moveDirectionToRight)
    {
      bool switchValue = digitalRead(PIN_SWITCH);
      if (switchValue == HIGH)
      {
        _positionCurrent = Position::LEFT;
        moveStart();
        return;
      }
    }
    char key = keypad.getKey();
    if (key == 'X')
    {
      _positionCurrent = Position::UNKNOWN;
      moveStop();
      return;
    }
  }
  else
  {
    _positionCurrent = _positionTarget;
    moveStart();
  }
}

//---------------------------------------------------
// MAIN: STATE: MENU
//---------------------------------------------------

void menuMainLoop()
{
  InputAction inputAction = inputList(str_Main, strListSize_Main, strList_Main, &_inputSettingsListIndex);
  if (inputAction == InputAction::ACCEPT)
  {
    switch (_inputSettingsListIndex)
    {
    case 0:
      setState(MainState::MENU_DURATION);
      break;
    case 1:
      setState(MainState::MENU_BOUNCE_MODE);
      break;
    case 2:
      setState(MainState::MENU_SMOOTHING);
      break;
    case 3:
      setState(MainState::MENU_ROTATION);
      break;
    case 4:
      setState(MainState::MENU_START);
      break;
    }
  }
}

void menuDurationLoop()
{
  InputAction inputAction = inputValueInt(str_Duration, &_inputTmpValueInt, _configDuration, str_suffix_s);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _inputTmpValueInt = min(max(_inputTmpValueInt, DURATION_MIN), DURATION_MAX);
    _configDuration = _inputTmpValueInt;
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_MAIN);
    break;
  }
}

void menuBounceModeLoop()
{
  InputAction inputAction = inputList(str_BounceMode, strListSize_BounceMode, strList_BounceMode, &_inputTmpListIndex);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _configBounceMode = _inputTmpListIndex; // 0 => false, 1 => true
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_MAIN);
    break;
  }
}

void menuSmoothingLoop()
{
  InputAction inputAction = inputList(str_Smoothing, strListSize_Smoothing, strList_Smoothing, &_inputTmpListIndex);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _configSmoothing = _inputTmpListIndex; // 0 => OFF, 1 => SHORT, 2 => MEDIUM, 3 => LONG
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_MAIN);
    break;
  }
}

void menuRotationLoop()
{
  InputAction inputAction = inputList(str_Rotation, strListSize_Rotation, strList_Rotation, &_inputSettingsRotationListIndex);
  switch (inputAction)
  {
  case InputAction::ACCEPT:
    switch (_inputSettingsRotationListIndex)
    {
    case 0:
      setState(MainState::MENU_ROTATION_FOCAL_DISTANCE);
      break;
    case 1:
      setState(MainState::MENU_ROTATION_ANGLE_LEFT);
      break;
    case 2:
      setState(MainState::MENU_ROTATION_ANGLE_RIGHT);
      break;
    }
    break;
  case InputAction::CANCEL:
    setState(MainState::MENU_MAIN);
    break;
  }
}

void menuRotationFocalDistanceLoop()
{
  InputAction inputAction = inputValueInt(str_FocalDistance, &_inputTmpValueInt, _configFocalDistance, str_suffix_cm);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _inputTmpValueInt = min(max(_inputTmpValueInt, FOCAL_DISTANCE_MIN), FOCAL_DISTANCE_MAX);
    _configFocalDistance = _inputTmpValueInt;
    double angleBase = calculateAngleBase();
    _configAngleLeft = 270.0 - angleBase;
    _configAngleRight = 90.0 + angleBase;
    _moveDegreesRotate = _configAngleRight - _configAngleLeft;
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_ROTATION);
    break;
  }
}

void menuRotationAngleLeftLoop()
{
  InputAction inputAction = inputValueInt(str_AngleLeft, &_inputTmpValueInt, _configAngleLeft, str_suffix_degree);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _inputTmpValueInt = min(max(_inputTmpValueInt, ANGLE_MIN), ANGLE_MAX);
    _configAngleLeft = _inputTmpValueInt;
    _configFocalDistance = 0;
    // rotationIsEnabled = (_configAngleLeft != _configAngleRight);
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_ROTATION);
    break;
  }
}

void menuRotationAngleRightLoop()
{
  InputAction inputAction = inputValueInt(str_AngleLeft, &_inputTmpValueInt, _configAngleRight, str_suffix_degree);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _inputTmpValueInt = min(max(_inputTmpValueInt, ANGLE_MIN), ANGLE_MAX);
    _configAngleRight = _inputTmpValueInt;
    _configFocalDistance = 0;
    // rotationIsEnabled = (_configAngleLeft != _configAngleRight);
    setState(MainState::MENU_MAIN);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_ROTATION);
    break;
  }
}

void menuStartLoop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case 'V':
      setState(MainState::MOVE);
      break;
    case 'X':
      setState(MainState::MENU_MAIN);
      break;
    }
  }
}

//---------------------------------------------------
// MAIN: HELPERS
//---------------------------------------------------

void motorSlideStart(long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingAccel, short smoothingDecel) //, short microsteps)
{
  // degrees = min(degrees, MOTOR_SLIDE_DEGREES_MAX);

  Serial.print("motorSlideStart | input | degrees=");
  Serial.print(degrees);
  Serial.print(", time=");
  Serial.println(time);

  float rotations = degrees / 360.0;
  float rpm = rotations / (float(time) / 60.0);
  rpm = min(rpm, float(MOTOR_RPM_MAX));
  motorSlide.setRPM(rpm);

  long timeByRpm = long((rotations / rpm) * 60.0);

  Serial.print("motorSlideStart | rpm | rotations=");
  Serial.print(rotations);
  Serial.print(", rpm=");
  Serial.print(rpm);
  Serial.print(", timeByRpm=");
  Serial.println(timeByRpm);

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setSpeedProfile(smoothingMode, smoothingAccel, smoothingDecel);

  if (_moveDirectionToRight)
  {
    degrees *= -1;
  }
  long steps = motorSlide.calcStepsForRotation(degrees);

  Serial.print("motorSlideStart | start | degrees=");
  Serial.print(degrees);
  Serial.print(", steps=");
  Serial.println(steps);

  motorSlide.startMove(steps, timeByRpm);
}

void motorSlideStop()
{
  motorSlide.stop();
}

void motorRotateStart(long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingAccel, short smoothingDecel) //, short microsteps)
{
  if (degrees <= 0)
  {
    return;
  }

  Serial.print("motorRotateStart | input | degrees=");
  Serial.print(degrees);
  Serial.print(", time=");
  Serial.println(time);

  float rotations = degrees / 360.0;
  float rpm = rotations / (float(time) / 60.0);
  rpm = min(rpm, float(MOTOR_RPM_MAX));
  motorSlide.setRPM(rpm);

  long timeByRpm = long((rotations / rpm) * 60.0);

  Serial.print("motorRotateStart | rpm | rotations=");
  Serial.print(rotations);
  Serial.print(", rpm=");
  Serial.print(rpm);
  Serial.print(", timeByRpm=");
  Serial.println(timeByRpm);

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setSpeedProfile(smoothingMode, smoothingAccel, smoothingDecel);

  if (_moveDirectionToRight)
  {
    degrees *= -1;
  }
  long steps = motorSlide.calcStepsForRotation(degrees);

  Serial.print("motorRotateStart | start | degrees=");
  Serial.print(degrees);
  Serial.print(", steps=");
  Serial.println(steps);

  motorSlide.startMove(steps, timeByRpm);
}

void motorRotateStop()
{
  motorSlide.stop();
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

void lcdPrintMenuStart()
{
  strcpy(_lcdLineB, "D: ");
  strcat(_lcdLineB, itoa(_duration, _lcdValue, 10));
  strcat(_lcdLineB, str_suffix_s);

  lcdPrint(str_Summary, _lcdLineB);
}

void lcdPrintMove(bool direction, BasicStepperDriver::State state)
{

  strcpy(_lcdLineA, str_Working);
  strcat(_lcdLineA, (direction) ? str_direction_forward : str_direction_backward);

  switch (state)
  {
  case BasicStepperDriver::State::STOPPED:
    strcpy(_lcdLineB, str_Stopped);
    break;
  case BasicStepperDriver::State::ACCELERATING:
    strcpy(_lcdLineB, str_Accelerating);
    break;
  case BasicStepperDriver::State::CRUISING:
    strcpy(_lcdLineB, str_Cruising);
    break;
  case BasicStepperDriver::State::DECELERATING:
    strcpy(_lcdLineB, str_Decelerating);
    break;
  }

  lcdPrint(_lcdLineA, _lcdLineB);
}

void lcdPrintInputListItem(char *header, char *listItem)
{
  strcpy(_lcdLineA, header);
  strcat(_lcdLineA, ":");

  strcpy(_lcdLineB, "> ");
  strcat(_lcdLineB, listItem);

  lcdPrint(_lcdLineA, _lcdLineB);
}

void lcdPrintInputValueInt(char *header, int value, char *suffix)
{
  strcpy(_lcdLineA, header);
  strcat(_lcdLineA, ":");

  itoa(value, _lcdLineB, 10);
  strcat(_lcdLineB, suffix);

  lcdPrint(_lcdLineA, _lcdLineB);
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

  if (_inputTmpListIndexLast != *index)
  {
    _inputTmpListIndexLast = *index;
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
    if (_inputTmpValueIntLast != defaultValue)
    {
      _inputTmpValueIntLast = defaultValue;
      lcdPrintInputValueInt(header, defaultValue, suffix);
    }
  }
  else
  {
    if (_inputTmpValueIntLast != *value)
    {
      _inputTmpValueIntLast = *value;
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
  double sideDistance = sqrt(targetDistance * targetDistance + MOTOR_SLIDE_DISTANCE_HALF_PWR);
  double sinAngle = targetDistance / sideDistance;
  double angleBase = asin(sinAngle) * RAD_TO_DEG;
  return angleBase;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------