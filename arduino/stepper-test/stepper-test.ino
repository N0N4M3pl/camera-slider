#include <DRV8825.h>

// Pin Definitions
#define MOTOR_M0_PIN 2
#define MOTOR_M1_PIN 3
#define MOTOR_M2_PIN 4
#define MOTOR_STEP_PIN 5
#define MOTOR_DIR_PIN 6

// Global variables and defines
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
// Target RPM for cruise speed
#define RPM 120
// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000

// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MICROSTEPS 16

// Object initialization
DRV8825 stepper(MOTOR_STEPS, MOTOR_DIR_PIN, MOTOR_STEP_PIN, MOTOR_M0_PIN, MOTOR_M1_PIN, MOTOR_M2_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB

  stepper.begin(RPM, MICROSTEPS);
  stepper.enable();
  stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  stepper.startRotate(360);
}

void loop() {
  //  delay(1000);
  //  stepper.setMicrostep(1);  // Set microstep mode to 1:1
  //  stepper.rotate(360);     // forward revolution
  //  stepper.setMicrostep(8);   // Set microstep mode to 1:8
  //  stepper.rotate(360);
  //  delay(1000);

  static int step = 0;
  unsigned wait_time = stepper.nextAction();
  if (wait_time) {
    Serial.print("  step="); Serial.print(step++);
    Serial.print("  angle="); Serial.print(step/MICROSTEPS/200 * 360);
    Serial.print("  dt="); Serial.print(wait_time);
    Serial.print("  rpm="); Serial.print(stepper.getCurrentRPM());
    Serial.print("  state="); Serial.print(stepper.getCurrentState());
    Serial.println();
  } else {
    stepper.disable();
    Serial.println("END");
    delay(3600000);
  }
}
