#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTTYPE DHT11

const int DHTPIN = 6, RS = 7, EN = 8, D4 = 9, D5 = 10, D6 = 11, D7 = 12;
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup()
{
  Serial.begin(9600); 
  // inialize the LCD
  lcd.begin(16, 2); 
  lcd.print("Initializing....");
  // initialize DHT11 sensor   
  dht.begin();    
}

void loop()
{
  
  // read humidity
  float humidity = dht.readHumidity();
  // read temperature as Celcium
  float temperature = dht.readTemperature();

  // check for fail reading
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity!");
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }
  if (isnan(temperature)){
    Serial.println("Failed to read temperature!");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Temp:     ");
    lcd.print(temperature);
    lcd.print("C");
  }
  
  // delay for the next iteration
  delay(2000);
}