#include <LCD_I2C.h>

LCD_I2C lcd(0x27); // Default address of most PCF8574 modules, change according

void setup()
{
    lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                 // this stop the library(LCD_I2C) from calling Wire.begin()
    lcd.backlight();
}

void loop()
{
    lcd.print("     Hello"); // You can make spaces using well... spaces
    lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd.print("World!");
    delay(500);

    // Flashing the backlight
    for (int i = 0; i < 5; ++i)
    {
        lcd.backlight();
        delay(50);
        lcd.noBacklight();
        delay(50);
    }

    lcd.backlight();
    lcd.clear();
    delay(500);
}
