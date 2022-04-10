#include <DRV8825.h>
#include <MultiDriver.h>
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

const short MOTOR_MICROSTEPS PROGMEM = 32;     //32;
const float MOTOR_RPM_MAX PROGMEM = 120;       //200 (1-200 is a reasonable range)
const int MOTOR_SLIDE_DISTANCE PROGMEM = 84; //84;
const int MOTOR_SLIDE_DISTANCE_HALF PROGMEM = MOTOR_SLIDE_DISTANCE / 2; // center
const int MOTOR_SLIDE_DISTANCE_HALF_PWR PROGMEM = MOTOR_SLIDE_DISTANCE_HALF * MOTOR_SLIDE_DISTANCE_HALF;
const int MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION PROGMEM = 4;
const int MOTOR_SLIDE_ROTATIONS PROGMEM = MOTOR_SLIDE_DISTANCE / MOTOR_SLIDE_DISTANCE_IN_ONE_ROTATION;
const float MOTOR_SLIDE_DEGREES_MOVE PROGMEM = (MOTOR_SLIDE_ROTATIONS * 360);
const float MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING PROGMEM = MOTOR_SLIDE_DEGREES_MOVE + 90;

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
DRV8825 motorSlide(MOTOR_STEPS, PIN_MOTOR_SLIDE_DIR, PIN_MOTOR_SLIDE_STEP, PIN_MOTOR_SLIDE_M0, PIN_MOTOR_SLIDE_M1, PIN_MOTOR_SLIDE_M2);

// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
DRV8825 motorRotate(MOTOR_STEPS, PIN_MOTOR_ROTATE_DIR, PIN_MOTOR_ROTATE_STEP, PIN_MOTOR_ROTATE_M0, PIN_MOTOR_ROTATE_M1, PIN_MOTOR_ROTATE_M2);

// https://github.com/laurb9/StepperDriver/blob/master/src/SyncDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/SyncDriver.h
// SyncDriver motorSyncDriver(motorSlide, motorRotate);
// https://github.com/laurb9/StepperDriver/blob/master/src/MultiDriver.cpp
// https://github.com/laurb9/StepperDriver/blob/master/src/MultiDriver.h
MultiDriver motorMultiDriver(motorSlide, motorRotate);

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
const char str_Minimum[] = "Minimum";
const char str_Short[] = "Short";
const char str_Medium[] = "Medium";
const char str_Long[] = "Long";
const char str_Maximum[] = "Maximum";
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

const int strListSize_Smoothing = 6;
const char *strList_Smoothing[strListSize_Smoothing] = {str_Off, str_Minimum, str_Short, str_Medium, str_Long, str_Maximum};

const int strListSize_Rotation = 3;
const char *strList_Rotation[strListSize_Rotation] = {str_RotationMode, str_LeftSide, str_RightSide};

const int strListSize_RotationMode = 2;
const char *strList_RotationMode[strListSize_RotationMode] = {str_Separately, str_FocalDistance};

//---------------------------------------------------
// MAIN
//---------------------------------------------------

const long DURATION_MIN PROGMEM = 1;                       // 16 sec, in seconds
const long DURATION_MAX PROGMEM = 86400;                    // 1 day, in seconds
const long DURATION_CORRECTION_RANGE_MIN PROGMEM = DURATION_MIN * 1000;   // 16 sec, in milliseconds
const long DURATION_CORRECTION_RANGE_MAX PROGMEM = 3600000; // 1 hour, in milliseconds
const long DURATION_CORRECTION PROGMEM = 5300;              // 5.3 sec, in milliseconds
const long DURATION_CALIBRATING PROGMEM = 10625;            // minimal time from logs, in seconds
const long FOCAL_DISTANCE_MIN PROGMEM = 10;                 // 10 cm
const long FOCAL_DISTANCE_MAX PROGMEM = 10000;              // 100 m
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
  MINIMUM,
  SHORT,
  MEDIUM,
  LONG,
  MAXIMUM
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
float _moveDegreesSlide = 0;
float _moveDegreesRotate = 0;

long _configDuration = 16;                          // 60;
bool _configBounceMode = false;                     // false;
Smoothing _configSmoothing = Smoothing::OFF;    // MINIMUM;
bool _configRotationByFocalDistance = true;         // true;
long _configFocalDistance = FOCAL_DISTANCE_MIN;     // FOCAL_DISTANCE_MIN;
float _configDegreesLeft = 90;  // ROTATION_DEGREES_AHEAD;
float _configDegreesRight = 270; // ROTATION_DEGREES_AHEAD;

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

bool _debugMSStop = false;
bool _debugMRStop = false;

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
    _inputTmpListIndex = _configSmoothing; // 0 => OFF, 1 => MINIMUM, 2 => SHORT, 3 => MEDIUM, 4 => LONG, 5 => MAXIMUM
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

// duration in seconds
void moveSetup(Position positionTarget, float degreesSlide, float degreesLeft, float degreesRight, long duration, Smoothing smoothing, bool bounceMode, MainState nextState, MainState backState)
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

  // TODO duration as float without converting to milliseconds or miniseconds
  _duration = duration * 1000; // convert to milliseconds
  Serial.print("moveSetup | d=");
  Serial.println(_duration);
  // if (_duration >= DURATION_CORRECTION_RANGE_MIN && _duration <= DURATION_CORRECTION_RANGE_MAX)
  // {
  //   float ratio = 1.0 - ((float)_duration / (float)DURATION_CORRECTION_RANGE_MAX);
  //   long factor = (DURATION_CORRECTION * ratio);
  //   // _duration -= factor;
  //   Serial.print("^ r=");
  //   Serial.print(ratio);
  //   Serial.print(", f=");
  //   Serial.print(factor);
  //   Serial.print(", d=");
  //   Serial.println(_duration);
  // }

  _smoothingMode = (smoothing == Smoothing::OFF) ? BasicStepperDriver::Mode::CONSTANT_SPEED : BasicStepperDriver::Mode::LINEAR_SPEED;
  switch (smoothing)
  {
  case Smoothing::OFF:
    _smoothingValue = 10000;
    break;
  case Smoothing::MINIMUM:
    _smoothingValue = 256;
    break;
  case Smoothing::SHORT:
    _smoothingValue = 128;
    break;
  case Smoothing::MEDIUM:
    _smoothingValue = 32;
    break;
  case Smoothing::LONG:
    _smoothingValue = 8;
    break;
  case Smoothing::MAXIMUM:
    _smoothingValue = 1;
    break;
  }

  _bounceMode = bounceMode;
  _nextState = nextState;
  _backState = backState;

  // Serial.print("moveSetup | posT=");
  // Serial.print(_positionTarget);
  // Serial.print(", degSli=");
  // Serial.print(_moveDegreesSlide);
  // Serial.print(", degRot=");
  // Serial.print(_moveDegreesRotate);
  // Serial.print(" | posCur=");
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
      _moveDegreesRotate = _moveDegreesRotate * -1;
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
      moveMotorStart(false, MOTOR_SLIDE_DEGREES_MOVE_CALIBRATING, 0, DURATION_CALIBRATING, BasicStepperDriver::Mode::CONSTANT_SPEED, 1);
      break;
    }
  }
}

void moveStop(MainState state)
{
  // Serial.println("moveStop");
  motorSlide.stop();
  motorRotate.stop();
  motorMultiDriver.disable();
  setState(state);
}

// time: in milliseconds
void moveMotorStart(bool directionToRight, long degreesSlide, float degreesRotate, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue)
{
  time = time * 1000; // convert to microseconds
  float timeSlideRatio = 1.0;
  long timeSlide =  time * timeSlideRatio;
  float timeRotateRatio = 1.0;
  long timeRotate =  time * timeRotateRatio;
  
  long motorSlideSteps = 0;
  long motorSlideTime = 0;
  long motorRotateSteps = 0;
  long motorRotateTime = 0;
  if (degreesSlide != 0)
  {
    motorSlide.enable();
    motorSlideSteps = motorSlideCalcSteps(directionToRight, degreesSlide, timeSlide, smoothingMode, smoothingValue);
    motorSlideTime = abs(motorSlide.getTimeForMove(motorSlideSteps));
  }
  if (degreesRotate != 0)
  {
    motorRotate.enable();
    motorRotateSteps = motorRotateCalcSteps(degreesRotate, timeRotate, smoothingMode, smoothingValue);
    motorRotateTime = abs(motorRotate.getTimeForMove(motorRotateSteps));
  }

  Serial.print("mMStart | t=");
  Serial.print(time);
  Serial.print(", tSr=");
  Serial.print(timeSlideRatio);
  Serial.print(", tS=");
  Serial.print(timeSlide);
  Serial.print(", tRr=");
  Serial.print(timeRotateRatio);
  Serial.print(", tR=");
  Serial.print(timeRotate);
  // Serial.print(", mSSteps=");
  // Serial.print(motorSlideSteps);
  Serial.print(", mSTime=");
  Serial.print(motorSlideTime);
  // Serial.print(", mRSteps=");
  // Serial.print(motorRotateSteps);
  Serial.print(", mRTime=");
  Serial.println(motorRotateTime);

  _debugMSStop = false;
  _debugMRStop = false;
  Serial.print("START ");
  Serial.println(micros());

  _positionCurrent = Position::UNKNOWN;
  motorMultiDriver.startMove(motorSlideSteps, motorRotateSteps);
}

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
  float rpmNorm = min(rpm, MOTOR_RPM_MAX);

  // motorSlide.setMicrostep(microsteps);
  motorSlide.setRPM(rpmNorm);
  motorSlide.setSpeedProfile(smoothingMode, smoothingValue, smoothingValue);
  long steps = motorSlide.calcStepsForRotation(degrees); // Calculate steps needed to rotate requested angle, given in degrees

  long timeBySteps = motorSlide.getTimeForMove(steps);

  // Serial.print("mSliCalcSteps | deg=");
  // Serial.print(degrees);
  // Serial.print(", t=");
  // Serial.print(time);
  // Serial.print(", rot=");
  // Serial.print(rotations);
  // Serial.print(", rpm=");
  // Serial.print(rpm);
  // Serial.print(", rpmN=");
  // Serial.print(rpmNorm);
  // Serial.print(", st=");
  // Serial.print(steps);
  // Serial.print(", tBySt=");
  // Serial.println(timeBySteps);

  return steps;
}

// time: in microseconds
// There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second.
long motorRotateCalcSteps(float degrees, long time, BasicStepperDriver::Mode smoothingMode, short smoothingValue) //, short microsteps)
{
  degrees = (degrees * 3.0);

  float rotations = degrees / 360.0;
  float rpm = abs(rotations) / float(time / 1000000.0 / 60.0);
  float rpmNorm = min(rpm, MOTOR_RPM_MAX);

  // motorRotate.setMicrostep(microsteps);
  motorRotate.setRPM(rpmNorm);
  motorRotate.setSpeedProfile(smoothingMode, smoothingValue, smoothingValue);
  long steps = motorRotate.calcStepsForRotation(degrees); // Calculate steps needed to rotate requested angle, given in degrees

  long timeBySteps = motorRotate.getTimeForMove(steps);

  // Serial.print("mRotCalcSteps | deg=");
  // Serial.print(degrees);
  // Serial.print(", t=");
  // Serial.print(time);
  // Serial.print(", rot=");
  // Serial.print(rotations);
  // Serial.print(", rpm=");
  // Serial.print(rpm);
  // Serial.print(", rpmN=");
  // Serial.print(rpmNorm);
  // Serial.print(", st=");
  // Serial.print(steps);
  // Serial.print(", tBySt=");
  // Serial.println(timeBySteps);

  return steps;
}

void moveLoop()
{
  unsigned long nextAction = motorMultiDriver.nextAction();
  if (nextAction)
  {
    if (!_debugMSStop && motorSlide.getCurrentState() == Motor::STOPPED) {
      _debugMSStop = true;
      Serial.print("STOP S ");
      Serial.println(micros());
    }
    if (!_debugMRStop && motorRotate.getCurrentState() == Motor::STOPPED) {
      _debugMRStop = true;
      Serial.print("STOP R ");
      Serial.println(micros());
    }
    if (motorSlide.getCurrentState() != Motor::STOPPED && motorSlide.getDirection() == 1) //getDirection --> Get movement direction: forward +1, back -1
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
      moveSetup(Position::LEFT, MOTOR_SLIDE_DEGREES_MOVE, _configDegreesLeft, _configDegreesRight, 1, Smoothing::OFF, false, MainState::MENU_START, MainState::MENU_MAIN);
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
    _configSmoothing = _inputTmpListIndex; // 0 => OFF, 1 => MINIMUM, 2 => SHORT, 3 => MEDIUM, 4 => LONG, 5 => MAXIMUM
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
