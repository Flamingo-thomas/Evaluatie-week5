#include <Adafruit_BMP280.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define BLYNK_TEMPLATE_ID           "TMPL26jVAd1r"
#define BLYNK_DEVICE_NAME           "IoT Thermostaat"
#define BLYNK_AUTH_TOKEN            "eT4_4E4-DVyWmdjm297XZv0s5QG5iMYu"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


Adafruit_BMP280 bmp; // I2C Interface

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD display

uint8_t led = D8;
uint8_t pot = A0;

char ssid[] = "Bram Van Eekert";
char pass[] = "015314421";
char auth[] = BLYNK_AUTH_TOKEN;

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  Blynk.begin(auth, ssid, pass);

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

    Blynk.virtualWrite(V1, potValue);
    Blynk.virtualWrite(V2, temperature);
    int wantedTemperature = BLYNK_READ(V3);

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
    Serial.print("Blynk temperature: ");
    Serial.println(wantedTemperature);
    Serial.println(" ");
}
