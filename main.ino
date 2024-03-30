#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int mq7 = A0;
const int mq4 = A1;   
const int mq135 = A3;
const int buzzerPin = 8;  // Change this to the pin connected to the buzzer

float m = -0.6527; 
float b = 1.30; 
float R0 = 21.91;

float threshold_mq4 = 2.50;
float threshold_mq135 = 1.50;
float threshold_mq7 = 1.00;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("Atmosphere");
  lcd.setCursor(0,3);
  lcd.print("Gas Analyser");
  delay (3000);
  lcd.clear();

  lcd.print("Team");
  lcd.setCursor(0,3);
  lcd.print("Sensor Squad");
  delay (3000);
  lcd.clear();

  lcd.print("Please Wait till");
  lcd.setCursor(0,3);
  lcd.print("15 sec...");
  delay (2000);

  pinMode(mq4, INPUT); 
  pinMode(mq135, INPUT); 
  pinMode(mq7, INPUT);
  pinMode(buzzerPin, OUTPUT);

  delay(15000);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  double mq4_value = analogToPPM(analogRead(mq4));
  double mq135_value = analogToPPM(analogRead(mq135));
  double mq7_value = analogToPPM(analogRead(mq7));

  Serial.println("************");
  Serial.print("MQ-4 Value: "); Serial.print(mq4_value, DEC); Serial.println(" PPM");
  Serial.print("MQ-135 Value: "); Serial.print(mq135_value, DEC); Serial.println(" PPM");
  Serial.print("MQ-7 Value: "); Serial.print(mq7_value, DEC); Serial.println(" PPM");
  Serial.println("************\n\n");

  lcd.setCursor(0,0);
  lcd.print("CNG Value");
  lcd.setCursor(0,3);
  lcd.println(mq4_value);
  lcd.setCursor(7,7);
  lcd.print("PPM");
  checkThresholdAndAlert(mq4_value, threshold_mq4, "CNG");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("AQI Value");
  lcd.setCursor(0,3);
  lcd.println(mq135_value);
  lcd.setCursor(7,7);
  lcd.print("PPM");
  checkThresholdAndAlert(mq135_value, threshold_mq135, "AQI");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("CO Value");
  lcd.setCursor(0,3);
  lcd.println(mq7_value);
  lcd.setCursor(7,7);
  lcd.print("PPM");
  checkThresholdAndAlert(mq7_value, threshold_mq7, "CO");
  delay(2000);
  lcd.clear();
}

double analogToPPM(int aValue) {
  float sensor_volt;
  float RS_gas; 
  float ratio;
  int sensorValue = aValue;

  sensor_volt = sensorValue*(5.0/1023.0); 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0; 
  ratio = RS_gas/R0; 
  double ppm_log = (log10(ratio)-b)/m;
  return ppm_log;
}

void checkThresholdAndAlert(double sensorValue, float threshold, const char* gasType) {
  if (sensorValue > threshold) {
    lcd.setCursor(0, 0);
    lcd.print("ALERT! High ");
    lcd.print(gasType);
    lcd.setCursor(0, 1);
    lcd.print("level: ");
    lcd.println(sensorValue);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);  // Buzzer on for 1 second
    digitalWrite(buzzerPin, LOW);
    delay(1000);  // Delay before clearing LCD and turning off buzzer
    lcd.clear();
  }
}