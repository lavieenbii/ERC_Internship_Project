#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <Arduino.h>

#define LED_FRONT 7
#define LED_RIGHT 2
#define LED_LEFT 4
#define LED_BEHIND 8

LiquidCrystal_I2C lcd(0x27, 16, 2);
int AcX, AcY, AcZ, GyX, GyY, GyZ;
long int AcX_cal, AcY_cal, AcZ_cal, GyX_cal, GyY_cal, GyZ_cal;
const int MPU = 0x68;

void read_data()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

void setup()
{
  lcd.init();
  lcd.init();
  lcd.backlight();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("MAGANG ERC 2022");
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("FINDING ZERO VALUE");
  lcd.setCursor(0, 1);

  for (int i = 0; i < 2000; i++)
  {
    read_data();
    GyX_cal += GyX;
    GyY_cal += GyY;
    GyZ_cal += GyZ;
  }
  GyX_cal /= 2000;
  GyY_cal /= 2000;
  GyZ_cal /= 2000;
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("X = " + String(GyX_cal));
  lcd.setCursor(0, 1);
  lcd.print("Y = " + String(GyY_cal));
  delay(2000);
  lcd.clear();

  pinMode(LED_FRONT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_BEHIND, OUTPUT);
}

void loop()
{
  lcd.clear();
  read_data();

  if (GyX > (7000 + GyX_cal))
  {
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, HIGH);
    digitalWrite(LED_FRONT, LOW);
    digitalWrite(LED_BEHIND, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Arah : ");
    Serial.print("Kiri");
    lcd.setCursor(0, 1);
    lcd.print("Kiri");
  }
  else if (GyX < (-7000 + GyX_cal))
  {
    digitalWrite(LED_RIGHT, HIGH);
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_FRONT, LOW);
    digitalWrite(LED_BEHIND, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Arah : ");
    Serial.print("Kanan");
    lcd.setCursor(0, 1);
    lcd.print("Kanan");
  }
  else if (GyY > (7000 + GyY_cal))
  {
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_FRONT, LOW);
    digitalWrite(LED_BEHIND, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Arah : ");
    Serial.print("Belakang");
    lcd.setCursor(0, 1);
    lcd.print("Belakang");
  }
  else if (GyY < (-7000 + GyY_cal))
  {
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_FRONT, HIGH);
    digitalWrite(LED_BEHIND, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Arah : ");
    Serial.print("Depan");
    lcd.setCursor(0, 1);
    lcd.print("Depan");
  }
  else
  {
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_FRONT, LOW);
    digitalWrite(LED_BEHIND, LOW);
    lcd.setCursor(0, 0);
    lcd.print("X = " + String(GyX));
    lcd.setCursor(0, 1);
    lcd.print("Y = " + String(GyY));
  }

  delay(333);
}