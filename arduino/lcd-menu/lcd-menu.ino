#include <LCD_I2C.h>

// Pin Definitions

// Global variables and defines
#define LCD_ADDRESS 0x27

// Object initialization
LCD_I2C lcd(LCD_ADDRESS);

void setup() {
  // Setup Serial which is useful for debugging
  // Use the Serial Monitor to view printed messages
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");

  lcd.begin();
//  lcd.setBacklight(BACKLIGHT);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" camera-slider  ");
  lcd.setCursor(0, 1);
  lcd.print("     Rocks!     ");

}

void loop() {

}
