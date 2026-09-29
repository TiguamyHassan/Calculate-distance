#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int Redled = 2;
int Yellowled = 3;
int Greenled = 4;
int buzzer = 5;
int trig = 9;
int echo = 8;

void setup()
{
  lcd.init();
  lcd.backlight();

  pinMode(Redled, OUTPUT);
  pinMode(Yellowled, OUTPUT);
  pinMode(Greenled, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

}

void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2;

  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm  ");

  if (distance < 4)
  {
    digitalWrite(Greenled, LOW);
    digitalWrite(Yellowled, LOW);
    digitalWrite(Redled, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);
    delay(150);
  }
  else if (distance < 10)
  {
    digitalWrite(Greenled, LOW);
    digitalWrite(Redled, LOW);    
    digitalWrite(Yellowled, HIGH); 
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(300);
  }
  else
  {
    digitalWrite(Greenled, HIGH);
    digitalWrite(Yellowled, LOW);
    digitalWrite(Redled, LOW);
   
    digitalWrite(buzzer, LOW);
  
  }

  delay(50);
}
