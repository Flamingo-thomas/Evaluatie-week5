#include <Adafruit_BMP280.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Adafruit_BMP280 bmp; // I2C Interface

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD display

uint8_t led = D8;
uint8_t pot = A0;


void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  pinMode(led, OUTPUT);
  pinMode(pot, INPUT);

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
    int potValue = analogRead(pot);
    float temperature = bmp.readTemperature();

    potValue = map(potValue, 0,1024, 0, 30);

    lcd.setCursor(0, 0);
    lcd.print("TempGewenst: ");
    lcd.print(potValue);
    lcd.setCursor(0, 12);
    lcd.print("TempHuidig: ");
    lcd.print(temperature);

    if(temperature < potValue){
      digitalWrite(led, HIGH);
    }
    else{
      digitalWrite(led, LOW);
    }
    
    Serial.print(F("Temperature = "));
    Serial.print(temperature);
    Serial.println(" *C");
    Serial.print("Potentiometer: ");
    Serial.println(potValue);
    Serial.println(" ");
}
