#include <Arduino.h>
#include <LiquidCrystal.h>

#define LEDGREEN 2    //defines leds
#define LEDYELLOW 3
#define LEDRED 4
#define SENSOR A0   //defines temp sensor
#define INPUT1 6    //defines inputs for H-bridge
#define INPUT2 5
#define BUZZER 7

LiquidCrystal lcd (8, 9, 10, 11, 12, 13); //object for liquid crystal, with the used ports

void TurnOnFan()
{
    digitalWrite(INPUT1, LOW);
    digitalWrite(INPUT2, HIGH);
}

void TurnOffFan()
{
    digitalWrite(INPUT1, LOW);
    digitalWrite(INPUT2, LOW);
}

void setup()
{
    lcd.begin(16, 2);
    pinMode(LEDGREEN, OUTPUT);   //leds
    pinMode(LEDYELLOW, OUTPUT);
    pinMode(LEDRED, OUTPUT);

    pinMode(INPUT1, OUTPUT);    //inputs for h-bridge
    pinMode(INPUT2, OUTPUT);

    pinMode(SENSOR, INPUT);

    pinMode(BUZZER, OUTPUT);
}

void loop()
{
    int sensorVal = analogRead(SENSOR);

    sensorVal = map(sensorVal, 20, 358, -40, 125);

    if(sensorVal <= 20){
        digitalWrite(LEDGREEN, HIGH);
        digitalWrite(LEDYELLOW, LOW);
        digitalWrite(LEDRED, LOW);
        noTone(BUZZER);
    }

    if(sensorVal > 20 && sensorVal <= 50){
        digitalWrite(LEDGREEN, LOW);
        digitalWrite(LEDYELLOW, HIGH);
        digitalWrite(LEDRED, LOW);
        noTone(BUZZER);
    }

    if(sensorVal > 50){
        digitalWrite(LEDGREEN, LOW);
        digitalWrite(LEDYELLOW, LOW);
        digitalWrite(LEDRED, HIGH);
        tone(BUZZER, 1200);
    }else{
        noTone(BUZZER);
    }

    if(sensorVal >= 25){
        TurnOnFan();
    }else{
        TurnOffFan();
    }

    lcd.setCursor(3, 0);
    lcd.print("Temp: ");
    lcd.print(sensorVal);
    lcd.print(" C");
    delay(500);
    lcd.clear();
}
