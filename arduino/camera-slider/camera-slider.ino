#include <DRV8825.h>
#include <SyncDriver.h>
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

const char KEY_1 PROGMEM = '1';
const char KEY_2 PROGMEM = '2';
const char KEY_3 PROGMEM = '3';
const char KEY_4 PROGMEM = '4';
const char KEY_5 PROGMEM = '5';
const char KEY_6 PROGMEM = '6';
const char KEY_7 PROGMEM = '7';
const char KEY_8 PROGMEM = '8';
const char KEY_9 PROGMEM = '9';
const char KEY_V PROGMEM = 'V';
const char KEY_0 PROGMEM = '0';
const char KEY_X PROGMEM = 'X';

const byte KEYPAD_ROWS PROGMEM = 4; //four rows
const byte KEYPAD_COLS PROGMEM = 3; //three columns
const char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {KEY_1, KEY_2, KEY_3},
    {KEY_4, KEY_5, KEY_6},
    {KEY_7, KEY_8, KEY_9},
    {KEY_V, KEY_0, KEY_X}};
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

const int MOTOR_MICROSTEPS PROGMEM = 16;     //32;
const int MOTOR_RPM_MAX PROGMEM = 600;       //600;
const int MOTOR_SLIDE_DISTANCE PROGMEM = 84; //84;
const int MOTOR_SLIDE_DISTANCE_HALF PROGMEM = MOTOR_SLIDE_DISTANCE / 2;
const int MOTOR_SLIDE_DISTANCE_HALF_PWR PROGMEM = MOTOR_SLIDE_DISTANCE_HALF * MOTOR_SLIDE_DISTANCE_HALF;
const int MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION PROGMEM = 4;
const int MOTOR_SLIDE_ROTATIONS PROGMEM = MOTOR_SLIDE_DISTANCE / MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION;
const long MOTOR_SLIDE_DEGREES_MOVE PROGMEM = (MOTOR_SLIDE_ROTATIONS * 360);
const long MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING PROGMEM = MOTOR_SLIDE_DEGREES_MOVE + 90;

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
DRV8825 motorSlide(MOTOR_STEPS, PIN_MOTOR_SLIDE_DIR, PIN_MOTOR_SLIDE_STEP, PIN_MOTOR_SLIDE_M0, PIN_MOTOR_SLIDE_M1, PIN_MOTOR_SLIDE_M2);

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
DRV8825 motorRotate(MOTOR_STEPS, PIN_MOTOR_ROTATE_DIR, PIN_MOTOR_ROTATE_STEP, PIN_MOTOR_ROTATE_M0, PIN_MOTOR_ROTATE_M1, PIN_MOTOR_ROTATE_M2);

// https://github.com/laurb9/StepperDriver/blob/master/src/SyncDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/SyncDriver.h
// https://github.com/laurb9/StepperDriver/blob/master/src/MultiDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/MultiDriver.h
SyncDriver motorSyncDriver(motorSlide, motorRotate);

//---------------------------------------------------
// STRINGS
//---------------------------------------------------

const char str_Main[] = "Main menu";
const char str_Duration[] = "Duration";
const char str_BounceMode[] = "Bounce mode";
const char str_Smoothing[] = "Smoothing";
const char str_Rotation[] = "Rotation";
const char str_RotationMode[] = "Rotation mode";
const char str_Separately[] = "Separately";
const char str_FocalDistance[] = "Focal distance";
const char str_LeftSide[] = "Left side";
const char str_RightSide[] = "Right side";
const char str_Start[] = "[START]";
const char str_Calibrating[] = "Calibrating";
const char str_PleaseWait[] = "Please wait ...";
const char str_Summary[] = "Summary";
const char str_Moving[] = "Moving";
const char str_On[] = "On";
const char str_Off[] = "Off";
const char str_Short[] = "Short";
const char str_Medium[] = "Medium";
const char str_Long[] = "Long";
const char str_suffix_second[] = " s";
const char str_suffix_cm[] = " cm";
const char str_suffix_degree[] = " deg";
const char str_Moving_bounceMode_left[] = "|>-";
const char str_Moving_bounceMode_right[] = "-<|";
const char str_Moving_once_left[] = "---";
const char str_Moving_once_right[] = "--|";
const char str_Moving_direction_left[] = "---<<<<---";
const char str_Moving_direction_right[] = "--->>>>---";
const char str_colon[] = ":";

const int strListSize_Main = 5;
const char *strList_Main[strListSize_Main] = {str_Duration, str_BounceMode, str_Smoothing, str_Rotation, str_Start};

const int strListSize_BounceMode = 2;
const char *strList_BounceMode[strListSize_BounceMode] = {str_Off, str_On};

const int strListSize_Smoothing = 4;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Short, str_Medium, str_Long};

const int strListSize_Rotation = 3;
const char *strList_Rotation[strListSize_Rotation] = {str_RotationMode, str_LeftSide, str_RightSide};

const int strListSize_RotationMode = 2;
const char *strList_RotationMode[strListSize_RotationMode] = {str_Separately, str_FocalDistance};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

const long DURATION_MIN PROGMEM = 1;            // 1 sec
const long DURATION_MAX PROGMEM = 60 * 60 * 24; // 1 day
const long FOCAL_DISTANCE_MIN PROGMEM = 10;     // 10 cm
const long FOCAL_DISTANCE_MAX PROGMEM = 10000;  // 100 m
const float ROTATION_DEGREES_MIN PROGMEM = 0;
const float ROTATION_DEGREES_MAX PROGMEM = 360;
const float ROTATION_DEGREES_AHEAD PROGMEM = 180;

enum MainState
{
  INITIAL,
  MOVE,
  MENU_MAIN,
  MENU_DURATION,
  MENU_BOUNCE_MODE,
  MENU_SMOOTHING,
  MENU_ROTATION,
  MENU_ROTATION_MODE,
  MENU_ROTATION_DEGREES_LEFT,
  MENU_ROTATION_DEGREES_RIGHT,
  MENU_START
};

enum Position
{
  UNKNOWN,
  LEFT,
  RIGHT,
  ROTATION_LEFT,
  ROTATION_RIGHT
};

enum Smoothing
{
  OFF,
  SHORT,
  MEDIUM,
  LONG
};

enum InputAction
{
  NOTHING,
  ACCEPT,
  CANCEL
};

MainState _mainState = INITIAL;
Position _positionCurrent = UNKNOWN;
Position _positionTarget = UNKNOWN;

bool _bounceMode;
long _duration;
BasicStepperDriver::Mode _smoothingMode = BasicStepperDriver::Mode::CONSTANT_SPEED;
short _smoothingValue = 1;
MainState _nextState;
MainState _backState;

bool _moveDirectionToRight;
long _moveDegreesSlide = 0;
float _moveDegreesRotate = 0;

long _configDuration = 20;                               //60;
bool _configBounceMode = false;                          //true;
Smoothing _configSmoothing = OFF;                        // OFF;
bool _configRotationByFocalDistance = false;             // true;
long _configFocalDistance = FOCAL_DISTANCE_MIN;          //FOCAL_DISTANCE_MIN;
float _configDegreesLeft = ROTATION_DEGREES_AHEAD - 90;  // ROTATION_DEGREES_AHEAD;
float _configDegreesRight = ROTATION_DEGREES_AHEAD + 90; // ROTATION_DEGREES_AHEAD;

int _inputMenuMainListIndex = 0;
int _inputMenuRotationListIndex = 0;
int _inputTmpListIndex = 0;
int _inputTmpListIndexLast = 0;
int _inputTmpValueInt = 0;
int _inputTmpValueIntLast = 0;
long _inputTmpValueLong = 0;
long _inputTmpValueLongLast = 0;
float _inputTmpValueFloat = 0;
float _inputTmpValueFloatLast = 0;

char _lcdLineA[17];
char _lcdLineB[17];
char _lcdValue[17];

//---------------------------------------------------
// MAIN
//---------------------------------------------------

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB

  pinMode(PIN_SWITCH, INPUT);

  motorSlide.begin(MOTOR_RPM_MAX, MOTOR_MICROSTEPS);
  motorRotate.begin(MOTOR_RPM_MAX, MOTOR_MICROSTEPS);

  lcd.begin();
  lcd.backlight();

  moveSetup(Position::LEFT, MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING, ROTATION_DEGREES_AHEAD, ROTATION_DEGREES_AHEAD, 1, Smoothing::OFF, false, MainState::MENU_MAIN, MainState::MENU_MAIN);
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
  case MainState::MENU_ROTATION_MODE:
    menuRotationModeLoop();
    break;
  case MainState::MENU_ROTATION_DEGREES_LEFT:
    menuRotationDegreesLeftLoop();
    break;
  case MainState::MENU_ROTATION_DEGREES_RIGHT:
    menuRotationDegreesRightLoop();
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
  // Serial.print("setState | old=");
  // Serial.print(_mainState);
  // Serial.print(", new=");
  // Serial.println(mainState);

  switch (mainState)
  {
  case MainState::MOVE:
    moveStart();
    break;
  case MainState::MENU_MAIN:
    _inputTmpListIndexLast = -1;
    _inputMenuRotationListIndex = 0;
    break;
  case MainState::MENU_DURATION:
    _inputTmpValueInt = -1;
    _inputTmpValueIntLast = -1;
    break;
  case MainState::MENU_BOUNCE_MODE:
    _inputTmpListIndex = _configBounceMode; // false => 0, true => 1
    _inputTmpListIndexLast = -1;
    break;
  case MainState::MENU_SMOOTHING:
    _inputTmpListIndex = _configSmoothing; // OFF => 0, SHORT => 1, MEDIUM => 2, LONG => 3
    _inputTmpListIndexLast = -1;
    break;
  case MainState::MENU_ROTATION:
    _inputTmpListIndexLast = -1;
    break;
  case MainState::MENU_ROTATION_MODE:
    _inputTmpListIndex = _configRotationByFocalDistance; // false => 0, true => 1
    _inputTmpListIndexLast = -1;
    break;
  case MainState::MENU_ROTATION_DEGREES_LEFT:
    _inputTmpValueLong = -1;
    _inputTmpValueLongLast = -1;
    _inputTmpValueFloat = -1;
    _inputTmpValueFloatLast = -1;
    break;
  case MainState::MENU_ROTATION_DEGREES_RIGHT:
    _inputTmpValueLong = -1;
    _inputTmpValueLongLast = -1;
    _inputTmpValueFloat = -1;
    _inputTmpValueFloatLast = -1;
    break;
  case MainState::MENU_START:
    lcdPrintMenuStart();
    break;
  }

  _mainState = mainState;
}

//---------------------------------------------------
// MAIN: STATE: MOVE
//---------------------------------------------------

void moveSetup(Position positionTarget, long degreesSlide, float degreesLeft, float degreesRight, long duration, Smoothing smoothing, bool bounceMode, MainState nextState, MainState backState)
{
  _positionTarget = positionTarget;
  _moveDegreesSlide = degreesSlide;

  if (_positionTarget == Position::LEFT)
  {
    _moveDegreesRotate = degreesLeft - degreesRight;
  }
  else
  {
    _moveDegreesRotate = degreesRight - degreesLeft;
  }

  if (_positionCurrent == _positionTarget && _moveDegreesSlide == 0 && _moveDegreesRotate != 0)
  {
    _positionCurrent = (_positionCurrent == Position::LEFT) ? Position::ROTATION_LEFT : Position::ROTATION_RIGHT;
  }

  _duration = duration;

  _smoothingMode = (smoothing == Smoothing::OFF) ? BasicStepperDriver::Mode::CONSTANT_SPEED : BasicStepperDriver::Mode::LINEAR_SPEED;
  switch (smoothing)
  {
  case Smoothing::OFF:
    _smoothingValue = 0;
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

  _bounceMode = bounceMode;
  _nextState = nextState;
  _backState = backState;

  // Serial.print("moveSetup | positionTarget=");
  // Serial.print(_positionTarget);
  // Serial.print(", _moveDegreesSlide=");
  // Serial.print(_moveDegreesSlide);
  // Serial.print(", _moveDegreesRotate=");
  // Serial.print(_moveDegreesRotate);
  // Serial.print(" | _positionCurrent=");
  // Serial.println(_positionCurrent);
}

void moveStart()
{
  // Serial.print("moveStart | _positionCurrent=");
  // Serial.print(_positionCurrent);
  // Serial.print(", _positionTarget=");
  // Serial.println(_positionTarget);

  if (_positionCurrent == _positionTarget)
  {
    if (_bounceMode)
    {
      Serial.println("moveStart | bounce");
      _positionTarget = (_positionTarget == Position::RIGHT) ? Position::LEFT : Position::RIGHT;
      _moveDirectionToRight = !_moveDirectionToRight;
      // _moveDegreesRotate = _moveDegreesRotate * -1;
      lcdPrintMove(_moveDirectionToRight, _bounceMode);
      moveMotorStart(_moveDirectionToRight, _moveDegreesSlide, _moveDegreesRotate, _duration, _smoothingMode, _smoothingValue);
    }
    else
    {
      Serial.println("moveStart | stop");
      moveStop(_nextState);
    }
  }
  else
  {
    switch (_positionCurrent)
    {
    case Position::LEFT:
    case Position::RIGHT:
    case Position::ROTATION_LEFT:
    case Position::ROTATION_RIGHT:
      Serial.println("moveStart | start");
      _moveDirectionToRight = (_positionCurrent == Position::LEFT || _positionCurrent == Position::ROTATION_LEFT);
      lcdPrintMove(_moveDirectionToRight, _bounceMode);
      moveMotorStart(_moveDirectionToRight, _moveDegreesSlide, _moveDegreesRotate, _duration, _smoothingMode, _smoothingValue);
      break;
    case Position::UNKNOWN:
      Serial.println("moveStart | calibrating");
      lcdPrint(str_Calibrating, str_PleaseWait);
      moveMotorStart(false, MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING, 0, 1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1);
      break;
    }
  }
}

void moveStop(MainState state)
{
  Serial.println("moveStop");
  motorSlide.stop();
  motorRotate.stop();
  motorSyncDriver.disable();
  setState(state);
}

// time: in seconds
void moveMotorStart(bool directionToRight, long degreesSlide, long degreesRotate, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue)
{
  time = time * 1000000;
  long motorSlideSteps = 0;
  long motorRotateSteps = 0;
  if (degreesSlide != 0)
  {
    motorSlide.enable();
    motorSlideSteps = motorSlideCalcSteps(directionToRight, degreesSlide, time, smoothingMode, smoothingValue);
    time = max(time, motorSlide.getTimeForMove(motorSlideSteps));
  }
  if (degreesRotate != 0)
  {
    motorRotate.enable();
    motorRotateSteps = motorRotateCalcSteps(degreesRotate, time, smoothingMode, smoothingValue);
  }
  _positionCurrent = Position::UNKNOWN;
  motorSyncDriver.startMove(motorSlideSteps, motorRotateSteps);
}

void moveLoop()
{
  if (motorSyncDriver.nextAction())
  {
    if (motorSlide.getDirection() == 1) //getDirection --> Get movement direction: forward +1, back -1
    {
      if (digitalRead(PIN_SWITCH) == HIGH)
      {
        // Serial.println("moveLoop | PIN_SWITCH == HIGH | moveStart");
        _positionCurrent = Position::LEFT;
        moveStart();
        return;
      }
    }
    if (keypad.getKey() == KEY_X)
    {
      // Serial.println("moveLoop | keypad X | moveStop");
      _positionCurrent = Position::UNKNOWN;
      moveStop(_backState);
      return;
    }
  }
  else
  {
    // Serial.println("moveLoop | motors stopped | moveStart");
    _positionCurrent = _positionTarget;
    moveStart();
    return;
  }
}

//---------------------------------------------------
// MAIN: STATE: MENU
//---------------------------------------------------

void menuMainLoop()
{
  InputAction inputAction = inputList(str_Main, strListSize_Main, strList_Main, &_inputMenuMainListIndex);
  if (inputAction == InputAction::ACCEPT)
  {
    switch (_inputMenuMainListIndex)
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
      moveSetup(Position::LEFT, MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING, _configDegreesLeft, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_START, MainState::MENU_MAIN);
      setState(MainState::MOVE);
      break;
    }
  }
}

void menuDurationLoop()
{
  InputAction inputAction = inputValueInt(str_Duration, &_inputTmpValueInt, _configDuration, str_suffix_second);
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
  InputAction inputAction = inputList(str_Rotation, strListSize_Rotation, strList_Rotation, &_inputMenuRotationListIndex);
  switch (inputAction)
  {
  case InputAction::ACCEPT:
    switch (_inputMenuRotationListIndex)
    {
    case 0:
      setState(MainState::MENU_ROTATION_MODE);
      break;
    case 1:
      moveSetup(Position::LEFT, MOTOR_SLIDE_DEGREES_MOVE, _configDegreesLeft, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_LEFT, MainState::MENU_ROTATION);
      setState(MainState::MOVE);
      break;
    case 2:
      moveSetup(Position::RIGHT, MOTOR_SLIDE_DEGREES_MOVE, _configDegreesLeft, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_RIGHT, MainState::MENU_ROTATION);
      setState(MainState::MOVE);
      break;
    }
    break;
  case InputAction::CANCEL:
    setState(MainState::MENU_MAIN);
    break;
  }
}

void menuRotationModeLoop()
{
  InputAction inputAction = inputList(str_RotationMode, strListSize_RotationMode, strList_RotationMode, &_inputTmpListIndex);
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    _configRotationByFocalDistance = _inputTmpListIndex; // 0 => false, 1 => true
    setState(MainState::MENU_ROTATION);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_ROTATION);
    break;
  }
}

void menuRotationDegreesLeftLoop()
{
  InputAction inputAction;
  if (_configRotationByFocalDistance)
  {
    inputAction = inputValueLong(str_FocalDistance, &_inputTmpValueLong, _configFocalDistance, str_suffix_cm);
  }
  else
  {
    inputAction = inputValueFloat(str_LeftSide, &_inputTmpValueFloat, _configDegreesLeft, str_suffix_degree);
  }
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
  {
    float degreesOld = _configDegreesLeft;
    if (_configRotationByFocalDistance)
    {
      _inputTmpValueLong = min(max(_inputTmpValueLong, FOCAL_DISTANCE_MIN), FOCAL_DISTANCE_MAX);
      _configFocalDistance = _inputTmpValueLong;
      setDegreesLeftRightByFocalDistance(_configFocalDistance);
    }
    else
    {
      _inputTmpValueFloat = min(max(_inputTmpValueFloat, ROTATION_DEGREES_MIN), ROTATION_DEGREES_MAX);
      _configDegreesLeft = _inputTmpValueFloat;
      _configFocalDistance = 0;
    }
    Serial.print("menuRotationDegreesLeftLoop | degreesOld=");
    Serial.print(degreesOld);
    Serial.print(", _configDegreesLeft=");
    Serial.println(_configDegreesLeft);
    moveSetup(Position::LEFT, 0, _configDegreesLeft, degreesOld, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_LEFT, MainState::MENU_ROTATION);
    setState(MainState::MOVE);
    break;
  }
  case (InputAction::CANCEL):
  {
    setState(MainState::MENU_ROTATION);
    break;
  }
  }
}

void menuRotationDegreesRightLoop()
{
  InputAction inputAction;
  if (_configRotationByFocalDistance)
  {
    inputAction = inputValueLong(str_FocalDistance, &_inputTmpValueLong, _configFocalDistance, str_suffix_cm);
  }
  else
  {
    inputAction = inputValueFloat(str_RightSide, &_inputTmpValueFloat, _configDegreesRight, str_suffix_degree);
  }
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
  {
    float degreesOld = _configDegreesRight;
    if (_configRotationByFocalDistance)
    {
      _inputTmpValueLong = min(max(_inputTmpValueLong, FOCAL_DISTANCE_MIN), FOCAL_DISTANCE_MAX);
      _configFocalDistance = _inputTmpValueLong;
      setDegreesLeftRightByFocalDistance(_configFocalDistance);
    }
    else
    {
      _inputTmpValueFloat = min(max(_inputTmpValueFloat, ROTATION_DEGREES_MIN), ROTATION_DEGREES_MAX);
      _configDegreesRight = _inputTmpValueFloat;
      _configFocalDistance = 0;
    }
    Serial.print("menuRotationDegreesRightLoop | degreesOld=");
    Serial.print(degreesOld);
    Serial.print(", _configDegreesRight=");
    Serial.println(_configDegreesRight);
    moveSetup(Position::RIGHT, 0, degreesOld, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_RIGHT, MainState::MENU_ROTATION);
    setState(MainState::MOVE);
    break;
  }
  case (InputAction::CANCEL):
  {
    setState(MainState::MENU_ROTATION);
    break;
  }
  }
}

void menuStartLoop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case KEY_V:
      moveSetup(Position::RIGHT, MOTOR_SLIDE_DEGREES_MOVE, _configDegreesLeft, _configDegreesRight, _configDuration, _configSmoothing, _configBounceMode, MainState::MENU_MAIN, MainState::MENU_MAIN);
      setState(MainState::MOVE);
      break;
    case KEY_X:
      setState(MainState::MENU_MAIN);
      break;
    }
  }
}

//---------------------------------------------------
// MAIN: HELPERS
//---------------------------------------------------

// time: in microseconds
// There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second.
long motorSlideCalcSteps(bool directionToRight, long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue) //, short microsteps)
{
  // degrees = min(degrees, MOTOR_SLIDE_DEGREES_MAX);

  if (directionToRight)
  {
    degrees *= -1;
  }

  float rotations = degrees / 360.0;
  float rpm = abs(rotations) / float(time / 1000000.0 / 60.0);
  float rpmNorm = min(rpm, float(MOTOR_RPM_MAX));

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setRPM(rpmNorm);
  motorSlide.setSpeedProfile(smoothingMode, smoothingValue, smoothingValue);
  long steps = motorSlide.calcStepsForRotation(degrees); // Calculate steps needed to rotate requested angle, given in degrees

  long timeBySteps = max(time, motorSlide.getTimeForMove(steps));

  Serial.print("mSliCalcSteps | deg=");
  Serial.print(degrees);
  Serial.print(", t=");
  Serial.print(time);
  Serial.print(", rot=");
  Serial.print(rotations);
  Serial.print(", rpm=");
  Serial.print(rpm);
  Serial.print(", rpmN=");
  Serial.print(rpmNorm);
  Serial.print(", st=");
  Serial.print(steps);
  Serial.print(", tBySt=");
  Serial.println(timeBySteps);

  return steps;
}

// time: in microseconds
// There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second.
long motorRotateCalcSteps(long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue) //, short microsteps)
{
  degrees = degrees * 3;

  float rotations = degrees / 360.0;
  float rpm = abs(rotations) / float(time / 1000000.0 / 60.0);
  float rpmNorm = min(rpm, float(MOTOR_RPM_MAX));

  // motorRotate.setMicrostep(microsteps);
  motorRotate.setRPM(rpmNorm);
  motorRotate.setSpeedProfile(smoothingMode, smoothingValue, smoothingValue);
  long steps = motorRotate.calcStepsForRotation(degrees); // Calculate steps needed to rotate requested angle, given in degrees

  long timeBySteps = max(time, motorRotate.getTimeForMove(steps));

  Serial.print("mRotCalcSteps | degrees=");
  Serial.print(degrees);
  Serial.print(", t=");
  Serial.print(time);
  Serial.print(", rot=");
  Serial.print(rotations);
  Serial.print(", rpm=");
  Serial.print(rpm);
  Serial.print(", rpmN=");
  Serial.print(rpmNorm);
  Serial.print(", st=");
  Serial.print(steps);
  Serial.print(", tBySt=");
  Serial.println(timeBySteps);

  return steps;
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
  strcat(_lcdLineB, itoa(_configDuration, _lcdValue, 10));
  strcat(_lcdLineB, str_suffix_second);

  lcdPrint(str_Summary, _lcdLineB);
}

void lcdPrintMove(bool directionToRight, bool bounceMode)
{
  strcpy(_lcdLineA, str_Moving);

  strcpy(_lcdLineB, (bounceMode) ? str_Moving_bounceMode_left : str_Moving_once_left);
  strcat(_lcdLineB, (directionToRight) ? str_Moving_direction_right : str_Moving_direction_left);
  strcat(_lcdLineB, (bounceMode) ? str_Moving_bounceMode_right : str_Moving_once_right);

  lcdPrint(_lcdLineA, _lcdLineB);
}

void lcdPrintInputListItem(char *header, char *listItem, int index)
{
  strcpy(_lcdLineA, header);
  strcat(_lcdLineA, str_colon);

  strcpy(_lcdLineB, "> ");
  strcat(_lcdLineB, listItem);

  lcdPrint(_lcdLineA, _lcdLineB);
}

void lcdPrintInputValueLong(char *header, long value, char *suffix)
{
  strcpy(_lcdLineA, header);
  strcat(_lcdLineA, str_colon);

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
    lcdPrintInputListItem(header, currentItem, *index);
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case KEY_V:
      return InputAction::ACCEPT;
    case KEY_X:
      return InputAction::CANCEL;
    case KEY_2:
    case KEY_4:
      (*index)--; // prev
      break;
    case KEY_6:
    case KEY_8:
      (*index)++; // next
      break;
    }
  }

  return InputAction::NOTHING;
}

InputAction inputValueInt(char *header, int *value, int defaultValue, char *suffix)
{
  if (*value == -1)
  {
    if (_inputTmpValueIntLast != defaultValue)
    {
      _inputTmpValueIntLast = defaultValue;
      lcdPrintInputValueLong(header, defaultValue, suffix);
    }
  }
  else
  {
    if (_inputTmpValueIntLast != *value)
    {
      _inputTmpValueIntLast = *value;
      lcdPrintInputValueLong(header, *value, suffix);
    }
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case KEY_V:
    {
      if (*value == -1)
      {
        *value = defaultValue;
      }
      return InputAction::ACCEPT;
    }
    case KEY_X:
      return InputAction::CANCEL;
    default:
    {
      if (*value == -1)
      {
        *value = (key - 48);
      }
      else
      {
        *value = ((*value) * 10) + (key - 48);
      }
      break;
    }
    }
  }

  return InputAction::NOTHING;
}

InputAction inputValueLong(char *header, long *value, long defaultValue, char *suffix)
{
  if (*value == -1)
  {
    if (_inputTmpValueLongLast != defaultValue)
    {
      _inputTmpValueLongLast = defaultValue;
      lcdPrintInputValueLong(header, defaultValue, suffix);
    }
  }
  else
  {
    if (_inputTmpValueLongLast != *value)
    {
      _inputTmpValueLongLast = *value;
      lcdPrintInputValueLong(header, *value, suffix);
    }
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case KEY_V:
    {
      if (*value == -1)
      {
        *value = defaultValue;
      }
      return InputAction::ACCEPT;
    }
    case KEY_X:
      return InputAction::CANCEL;
    default:
    {
      if (*value == -1)
      {
        *value = (key - 48);
      }
      else
      {
        *value = ((*value) * 10) + (key - 48);
      }
      break;
    }
    }
  }

  return InputAction::NOTHING;
}

InputAction inputValueFloat(char *header, float *value, float defaultValue, char *suffix)
{
  if (*value == -1)
  {
    if (_inputTmpValueFloatLast != defaultValue)
    {
      _inputTmpValueFloatLast = defaultValue;
      lcdPrintInputValueLong(header, defaultValue, suffix);
    }
  }
  else
  {
    if (_inputTmpValueFloatLast != *value)
    {
      _inputTmpValueFloatLast = *value;
      lcdPrintInputValueLong(header, *value, suffix);
    }
  }

  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
    case KEY_V:
    {
      if (*value == -1)
      {
        *value = defaultValue;
      }
      return InputAction::ACCEPT;
    }
    case KEY_X:
      return InputAction::CANCEL;
    default:
    {
      if (*value == -1)
      {
        *value = (key - 48);
      }
      else
      {
        *value = ((*value) * 10) + (key - 48);
      }
      break;
    }
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

void setDegreesLeftRightByFocalDistance(long focalDistance)
{
  // Serial.print("setDegreesLeftRightByFocalDistance | focalDistance=");
  // Serial.print(focalDistance);
  long focalDistancePwr = focalDistance * focalDistance;
  // Serial.print(", focalDistancePwr=");
  // Serial.print(focalDistancePwr);
  focalDistancePwr = focalDistancePwr + MOTOR_SLIDE_DISTANCE_HALF_PWR;
  // Serial.print(", focalDistancePwr=");
  // Serial.print(focalDistancePwr);
  float sideDistance = sqrt(focalDistancePwr);
  // Serial.print(", sideDistance=");
  // Serial.print(sideDistance);
  float sinAngle = focalDistance / sideDistance;
  // Serial.print(", sinAngle=");
  // Serial.print(sinAngle);
  float degreesBase = asin(sinAngle) * RAD_TO_DEG;
  // Serial.print(", degreesBase=");
  // Serial.print(degreesBase);
  _configDegreesLeft = 270 - degreesBase;
  _configDegreesRight = 90 + degreesBase;
  // Serial.print(" | _configDegreesLeft=");
  // Serial.print(_configDegreesLeft);
  // Serial.print(", _configDegreesRight=");
  // Serial.println(_configDegreesRight);
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
