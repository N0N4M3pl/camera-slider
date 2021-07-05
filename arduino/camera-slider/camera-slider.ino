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

const int MOTOR_RPM_MAX PROGMEM = 600;
const int MOTOR_SLIDE_DISTANCE PROGMEM = 30; //84;
const int MOTOR_SLIDE_DISTANCE_HALF PROGMEM = MOTOR_SLIDE_DISTANCE / 2;
const int MOTOR_SLIDE_DISTANCE_HALF_PWR PROGMEM = MOTOR_SLIDE_DISTANCE_HALF * MOTOR_SLIDE_DISTANCE_HALF;
const int MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION PROGMEM = 4;
const int MOTOR_SLIDE_ROTATIONS PROGMEM = MOTOR_SLIDE_DISTANCE / MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION;
const long MOTOR_SLIDE_DEGREES_MOVE PROGMEM = (MOTOR_SLIDE_ROTATIONS * 360);
const long MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING PROGMEM = MOTOR_SLIDE_DEGREES_MOVE + 360;

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
const char str_RotationMode[] = "Rotation mode";
const char str_FocalDistance[] = "Focal distance";
const char str_Separately[] = "Separately";
const char str_DegreesLeft[] = "Degrees - left";
const char str_DegreesRight[] = "Degrees - right";
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
const char str_suffix_s[] = " s";
const char str_suffix_cm[] = " cm";
const char str_suffix_degree[] = " °";
const char str_Moving_bounceMode_left[] = "|>-";
const char str_Moving_bounceMode_right[] = "-<|";
const char str_Moving_once_left[] = "---";
const char str_Moving_once_right[] = "--|";
const char str_Moving_direction_left[] = "--->>>>---";
const char str_Moving_direction_right[] = "---<<<<---";
const char str_Stopped[] = "Stopped";
const char str_Accelerating[] = "Accelerating";
const char str_Cruising[] = "Cruising";
const char str_Decelerating[] = "Decelerating";
const char str_colon[] = ":";

const int strListSize_Main = 5;
const char *strList_Main[strListSize_Main] = {str_Duration, str_BounceMode, str_Smoothing, str_Rotation, str_Start};

const int strListSize_BounceMode = 2;
const char *strList_BounceMode[strListSize_BounceMode] = {str_Off, str_On};

const int strListSize_Smoothing = 4;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Short, str_Medium, str_Long};

const int strListSize_Rotation = 3;
const char *strList_Rotation[strListSize_Rotation] = {str_RotationMode, str_DegreesLeft, str_DegreesRight};

const int strListSize_RotationMode = 2;
const char *strList_RotationMode[strListSize_RotationMode] = {str_FocalDistance, str_Separately};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

const long DURATION_MIN PROGMEM = 1;                    // 1 sec
const long DURATION_MAX PROGMEM = 60 * 60 * 24;         // 1 day
const int FOCAL_DISTANCE_MIN PROGMEM = 10;              // 10 cm
const int FOCAL_DISTANCE_MAX PROGMEM = 100 * 1000 * 10; // 10 km
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
  RIGHT
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

long _configDuration = 60;
bool _configBounceMode = true;
Smoothing _configSmoothing = OFF;
bool _configRotationByFocalDistance = true;
int _configFocalDistance = FOCAL_DISTANCE_MIN;
float _configDegreesLeft = ROTATION_DEGREES_AHEAD;
float _configDegreesRight = ROTATION_DEGREES_AHEAD;

int _inputMenuMainListIndex = 0;
int _inputMenuRotationListIndex = 0;
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
    break;
  case MainState::MENU_DURATION:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = 0;
    break;
  case MainState::MENU_BOUNCE_MODE:
    _inputTmpListIndexLast = -1;
    _inputTmpListIndex = _configBounceMode; // false => 0, true => 1
    break;
  case MainState::MENU_SMOOTHING:
    _inputTmpListIndexLast = -1;
    _inputTmpListIndex = _configSmoothing; // OFF => 0, SHORT => 1, MEDIUM => 2, LONG => 3
    break;
  case MainState::MENU_ROTATION:
    break;
  case MainState::MENU_ROTATION_MODE:
    _inputTmpListIndexLast = -1;
    _inputTmpListIndex = _configRotationByFocalDistance; // false => 0, true => 1
    break;
  case MainState::MENU_ROTATION_DEGREES_LEFT:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = (_configRotationByFocalDistance) ? _configFocalDistance : _configDegreesLeft;
    break;
  case MainState::MENU_ROTATION_DEGREES_RIGHT:
    _inputTmpValueIntLast = -1;
    _inputTmpValueInt = (_configRotationByFocalDistance) ? _configFocalDistance : _configDegreesRight;
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
  // Serial.print("moveSetup | positionTarget=");
  // Serial.print(positionTarget);
  // Serial.print(", bounceMode=");
  // Serial.print(bounceMode);
  // Serial.print(", duration=");
  // Serial.print(duration);
  // Serial.print(", smoothing=");
  // Serial.print(smoothing);
  // Serial.print(", nextState=");
  // Serial.print(nextState);
  // Serial.print(", backState=");
  // Serial.println(backState);

  _positionTarget = positionTarget;
  _moveDegreesSlide = degreesSlide;

  if (positionTarget == Position::LEFT)
  {
    _moveDegreesRotate = degreesLeft - degreesRight;
  }
  else
  {
    _moveDegreesRotate = degreesRight - degreesLeft;
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
      // Serial.println("moveStart | bounce");
      _positionTarget = (_positionTarget == Position::RIGHT) ? Position::LEFT : Position::RIGHT;
      _moveDirectionToRight = !_moveDirectionToRight;
      // _moveDegreesRotate = _moveDegreesRotate * -1;
      lcdPrintMove(_moveDirectionToRight, _bounceMode);
      moveMotorStart(_moveDirectionToRight, _moveDegreesSlide, _moveDegreesRotate, _duration, _smoothingMode, _smoothingValue);
    }
    else
    {
      // Serial.println("moveStart | stop");
      moveStop(_nextState);
    }
  }
  else
  {
    switch (_positionCurrent)
    {
    case Position::LEFT:
    case Position::RIGHT:
      // Serial.println("moveStart | start");
      _moveDirectionToRight = (_positionCurrent == Position::LEFT);
      lcdPrintMove(_moveDirectionToRight, _bounceMode);
      moveMotorStart(_moveDirectionToRight, _moveDegreesSlide, _moveDegreesRotate, _duration, _smoothingMode, _smoothingValue);
      break;
    case Position::UNKNOWN:
      // Serial.println("moveStart | calibrating");
      lcdPrint(str_Calibrating, str_PleaseWait);
      moveMotorStart(false, MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING, 0, 1, BasicStepperDriver::Mode::CONSTANT_SPEED, 1);
      break;
    }
  }
}

void moveStop(MainState state)
{
  motorSlideStop();
  motorRotateStop();
  setState(state);
}

void moveMotorStart(bool directionToRight, long degreesSlide, long degreesRotate, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue)
{
  _positionCurrent = Position::UNKNOWN;
  if (degreesSlide > 0)
  {
    motorSlideStart(directionToRight, degreesSlide, time, smoothingMode, smoothingValue, smoothingValue);
  }
  if (degreesRotate > 0)
  {
    motorRotateStart(directionToRight, degreesRotate, time, smoothingMode, smoothingValue, smoothingValue);
  }
}

void moveLoop()
{
  long motorSlideWaitTime = motorSlide.nextAction();
  if (motorSlideWaitTime)
  {
    if (motorSlide.getDirection() == 1) //getDirection --> Get movement direction: forward +1, back -1
    {
      bool switchValue = digitalRead(PIN_SWITCH);
      if (switchValue == HIGH)
      {
        // Serial.println("moveLoop | switch == HIGH | moveStart");
        _positionCurrent = Position::LEFT;
        moveStart();
        return;
      }
    }
    char key = keypad.getKey();
    if (key == KEY_X)
    {
      // Serial.println("moveLoop | X | moveStop");
      _positionCurrent = Position::UNKNOWN;
      moveStop(_backState);
      return;
    }
  }
  else
  {
    // Serial.println("moveLoop | motorSlideWaitTime == 0 | moveStart");
    _positionCurrent = _positionTarget;
    moveStart();
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
    inputAction = inputValueInt(str_FocalDistance, &_inputTmpValueInt, _configFocalDistance, str_suffix_cm);
  }
  else
  {
    inputAction = inputValueInt(str_DegreesLeft, &_inputTmpValueInt, _configDegreesLeft, str_suffix_degree);
  }
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    float degreesOld = _configDegreesLeft;
    if (_configRotationByFocalDistance)
    {
      _inputTmpValueInt = min(max(_inputTmpValueInt, FOCAL_DISTANCE_MIN), FOCAL_DISTANCE_MAX);
      _configFocalDistance = _inputTmpValueInt;
      setDegreesLeftRightByFocalDistance(_configFocalDistance);
    }
    else
    {
      _inputTmpValueInt = min(max(_inputTmpValueInt, ROTATION_DEGREES_MIN), ROTATION_DEGREES_MAX);
      _configDegreesLeft = _inputTmpValueInt;
      _configFocalDistance = 0;
    }
    moveSetup(Position::LEFT, 0, _configDegreesLeft, degreesOld, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_LEFT, MainState::MENU_ROTATION);
    setState(MainState::MOVE);
    break;
  case (InputAction::CANCEL):
    setState(MainState::MENU_ROTATION);
    break;
  }
}

void menuRotationDegreesRightLoop()
{
  InputAction inputAction;
  if (_configRotationByFocalDistance)
  {
    inputAction = inputValueInt(str_FocalDistance, &_inputTmpValueInt, _configFocalDistance, str_suffix_cm);
  }
  else
  {
    inputAction = inputValueInt(str_DegreesRight, &_inputTmpValueInt, _configDegreesRight, str_suffix_degree);
  }
  switch (inputAction)
  {
  case (InputAction::ACCEPT):
    float degreesOld = _configDegreesRight;
    if (_configRotationByFocalDistance)
    {
      _inputTmpValueInt = min(max(_inputTmpValueInt, FOCAL_DISTANCE_MIN), FOCAL_DISTANCE_MAX);
      _configFocalDistance = _inputTmpValueInt;
      setDegreesLeftRightByFocalDistance(_configFocalDistance);
    }
    else
    {
      _inputTmpValueInt = min(max(_inputTmpValueInt, ROTATION_DEGREES_MIN), ROTATION_DEGREES_MAX);
      _configDegreesRight = _inputTmpValueInt;
      _configFocalDistance = 0;
    }
    moveSetup(Position::RIGHT, 0, degreesOld, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_ROTATION_DEGREES_RIGHT, MainState::MENU_ROTATION);
    setState(MainState::MOVE);
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

void motorSlideStart(bool directionToRight, long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingAccel, short smoothingDecel) //, short microsteps)
{
  // degrees = min(degrees, MOTOR_SLIDE_DEGREES_MAX);

  // Serial.print("motorSlideStart | input | degrees=");
  // Serial.print(degrees);
  // Serial.print(", time=");
  // Serial.println(time);

  float rotations = degrees / 360.0;
  float rpm = rotations / (float(time) / 60.0);
  rpm = min(rpm, float(MOTOR_RPM_MAX));
  motorSlide.setRPM(rpm);

  long timeByRpm = long((rotations / rpm) * 60.0);

  // Serial.print("motorSlideStart | rpm | rotations=");
  // Serial.print(rotations);
  // Serial.print(", rpm=");
  // Serial.print(rpm);
  // Serial.print(", timeByRpm=");
  // Serial.println(timeByRpm);

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setSpeedProfile(smoothingMode, smoothingAccel, smoothingDecel);

  if (directionToRight)
  {
    degrees *= -1;
  }
  long steps = motorSlide.calcStepsForRotation(degrees);

  // Serial.print("motorSlideStart | start | degrees=");
  // Serial.print(degrees);
  // Serial.print(", steps=");
  // Serial.println(steps);

  motorSlide.startMove(steps, timeByRpm);
}

void motorSlideStop()
{
  motorSlide.stop();
}

void motorRotateStart(bool directionToRight, long degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingAccel, short smoothingDecel) //, short microsteps)
{
  // Serial.print("motorRotateStart | input | degrees=");
  // Serial.print(degrees);
  // Serial.print(", time=");
  // Serial.println(time);

  float rotations = degrees / 360.0;
  float rpm = rotations / (float(time) / 60.0);
  rpm = min(rpm, float(MOTOR_RPM_MAX));
  motorSlide.setRPM(rpm);

  long timeByRpm = long((rotations / rpm) * 60.0);

  // Serial.print("motorRotateStart | rpm | rotations=");
  // Serial.print(rotations);
  // Serial.print(", rpm=");
  // Serial.print(rpm);
  // Serial.print(", timeByRpm=");
  // Serial.println(timeByRpm);

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setSpeedProfile(smoothingMode, smoothingAccel, smoothingDecel);

  if (!directionToRight)
  {
    degrees *= -1;
  }
  long steps = motorSlide.calcStepsForRotation(degrees);

  // Serial.print("motorRotateStart | start | degrees=");
  // Serial.print(degrees);
  // Serial.print(", steps=");
  // Serial.println(steps);

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
  strcat(_lcdLineB, itoa(_configDuration, _lcdValue, 10));
  strcat(_lcdLineB, str_suffix_s);

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

void lcdPrintInputValueInt(char *header, int value, char *suffix)
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
    case KEY_V:
      return InputAction::ACCEPT;
    case KEY_X:
      return InputAction::CANCEL;
    default:
      *value = ((*value) * 10) + (key - 48);
      break;
    }
  }

  return InputAction::NOTHING;
}

//---------------------------------------------------

void setDegreesLeftRightByFocalDistance(int focalDistance)
{
  float degreeBase = sqrt(focalDistance * focalDistance + MOTOR_SLIDE_DISTANCE_HALF_PWR);
  degreeBase = focalDistance / degreeBase;
  degreeBase = asin(degreeBase) * RAD_TO_DEG;
  _configDegreesLeft = 270 - degreeBase;
  _configDegreesRight = 90 + degreeBase;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
