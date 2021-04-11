#include <DRV8825.h>

// Pin Definitions
#define PIN_MOTOR_MOVE_DIR 10
#define PIN_MOTOR_MOVE_STEP 9
#define PIN_MOTOR_MOVE_M0 A2
#define PIN_MOTOR_MOVE_M1 A1
#define PIN_MOTOR_MOVE_M2 A0

// Global variables and defines
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
// Target RPM for cruise speed
#define RPM 120
// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 1000
#define MOTOR_DECEL 1000

// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MICROSTEPS 1

// Object initialization
DRV8825 stepper(MOTOR_STEPS, PIN_MOTOR_MOVE_DIR, PIN_MOTOR_MOVE_STEP, PIN_MOTOR_MOVE_M0, PIN_MOTOR_MOVE_M1, PIN_MOTOR_MOVE_M2);

float rpm;
float microsteps;
float degrees;
long duration;

void setup() {

//  Serial.begin(9600);
//  while (!Serial) ; // wait for serial port to connect. Needed for native USB

  rpm = 6;
  microsteps = 16;
  degrees = 360;
  stepper.begin(rpm, microsteps);
  stepper.setSpeedProfile(stepper.CONSTANT_SPEED, 1000, 1000);
//  stepper.startRotate(degrees);
  //  degrees = stepper.calcStepsForRotation(degrees);
  //  duration = 10;
  //  stepper.startMove(degrees, duration);
}

void loop() {
    delay(6000);
  
    stepper.setRPM(60);
    stepper.setMicrostep(16);  // Set microstep mode to 1:1
    stepper.rotate(360);     // forward revolution
    
//    delay(3000);
//
//    stepper.setRPM(120);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//
//    stepper.setRPM(180);
//    stepper.setMicrostep(16);  // Set microstep mode to 1:1
//    stepper.rotate(360);     // forward revolution
//
//    delay(3000);
//
//    stepper.setRPM(200);
//    stepper.setMicrostep(16);  // Set microstep mode to 1:1
//    stepper.rotate(360);     // forward revolution
//    
//    delay(3000);
//    
//    stepper.setRPM(240);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(300);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(360);
    stepper.setMicrostep(16);   // Set microstep mode to 1:8
    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(360);
    stepper.setMicrostep(32);   // Set microstep mode to 1:8
    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(420);
    stepper.setMicrostep(32);   // Set microstep mode to 1:8
    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(480);
    stepper.setMicrostep(32);   // Set microstep mode to 1:8
    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(540);
    stepper.setMicrostep(32);   // Set microstep mode to 1:8
    stepper.rotate(360);

    delay(3000);
    
    stepper.setRPM(600);
    stepper.setMicrostep(32);   // Set microstep mode to 1:8
    stepper.rotate(360);

    // NOT WORKING:

//    delay(3000);
//    
//    stepper.setRPM(360);
//    stepper.setMicrostep(8);   // Set microstep mode to 1:8
//    stepper.rotate(360);

//    delay(3000);
//    
//    stepper.setRPM(360);
//    stepper.setMicrostep(4);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(360);
//    stepper.setMicrostep(2);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(360);
//    stepper.setMicrostep(1);   // Set microstep mode to 1:8
//    stepper.rotate(360);

//    delay(3000);
//    
//    stepper.setRPM(420);
//    stepper.setMicrostep(8);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(420);
//    stepper.setMicrostep(4);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(420);
//    stepper.setMicrostep(2);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(420);
//    stepper.setMicrostep(1);   // Set microstep mode to 1:8
//    stepper.rotate(360);

//    delay(3000);
//    
//    stepper.setRPM(420);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(480);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);

//    delay(3000);
//    
//    stepper.setRPM(540);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);
//
//    delay(3000);
//    
//    stepper.setRPM(720);
//    stepper.setMicrostep(16);   // Set microstep mode to 1:8
//    stepper.rotate(360);
    
    delay(6000);

//  static float step = 0;
//  unsigned wait_time = stepper.nextAction();
//  if (wait_time) {
//    Serial.print("  step="); Serial.print(step++);
//    Serial.print("  angle="); Serial.print(float(step / microsteps / 200.0) * degrees);
//    Serial.print("  dt="); Serial.print(wait_time);
//    Serial.print("  rpm="); Serial.print(stepper.getCurrentRPM());
//    Serial.print("  state="); Serial.print(stepper.getCurrentState());
//    Serial.println();
//  } else {
//    stepper.disable();
//    Serial.println("END");
//    delay(3600000);
//  }
}
