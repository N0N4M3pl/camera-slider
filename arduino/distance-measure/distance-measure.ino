// Libraries
#include <NewPing.h>

// Pin Definitions
#define DIST_PIN_TRIG 2
#define DIST_PIN_ECHO 3

// Global variables and defines

// Object initialization
NewPing distance(DIST_PIN_TRIG, DIST_PIN_ECHO);

void setup() {
  // Setup Serial which is useful for debugging
  // Use the Serial Monitor to view printed messages
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");
}

void loop() {
  // simple
  //  int distanceCm = distance.ping_cm();

  // median
  long distanceMs = distance.ping_median(10);
  int distanceCm = distance.convert_cm(distanceMs);

  Serial.print(F("Distance: ")); Serial.print(distanceCm); Serial.println(F("[cm]"));
  delay(100);
}
