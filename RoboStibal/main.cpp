#include <WProgram.h> //import main Arduino header file
#include <Wire.h>
//#include <Servo.h>
#include "Defs.h"
#include "Functions.h"
#include "Accel_ADXL345.h"

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(E1, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(E2, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(PWM1, INPUT);
    pinMode(TRIG1, OUTPUT);
    pinMode(PWM2, INPUT);
    pinMode(TRIG2, OUTPUT);
    pinMode(13, OUTPUT);

    pinMode(22, INPUT);
    pinMode(23, INPUT);
    pinMode(24, INPUT);
    pinMode(25, INPUT);
    pinMode(26, INPUT_PULLUP);
    pinMode(27, INPUT_PULLUP);
    pinMode(28, INPUT_PULLUP);

    attachInterrupt(5, rotateR, FALLING);
    attachInterrupt(4, rotateL, FALLING);

    writeTo(ADXL345, 0);
    writeTo(ADXL345, 16);
    writeTo(ADXL345, 8);

    readFrom(ADXL345, calbAccelVal);
}

int main()
{
    init(); //Don't forget this!

    setup();
    Serial.print(1);

    while(true)
    {
        /*readFrom(ADXL345, accelVal);
        sprintf(str, "%d %d %d", accelVal[0] - calbAccelVal[0], accelVal[1] - calbAccelVal[1], accelVal[2] - calbAccelVal[2]);
        Serial.println(str);

        delay(500);*/

        /*serialWrite(312);
        delay(1000);*/
        /*Serial.print("rechts: ");
        Serial.print(T1);
        Serial.print(" links: ");
        Serial.println(T2);

        Serial.println();
        delay(100);*/

        followLine();
        onTouchV2();

        /*if(!T3)
        {
            Serial.println(sharp());
            Serial.println();
            Serial.println(sharp());
        }
        while(!T3){};*/

        //Serial.println(sharp());

        /*while(rotR < 75)
        {
            rechts(1, HIGH);
            links(1, HIGH);
        }
        rechts(0, LOW);
        links(0, LOW);

        Serial.print("rechts: ");
        Serial.print(rotR);
        Serial.print(" links: ");
        Serial.println(rotL);*/

        /*rechts(100, HIGH);
        links(100, HIGH);*/
    }
    /*int myrotR = rotR;
    while((rotR - myrotR) < 360)
    {
        rechts(50, HIGH);
        links(50, HIGH);
    }
    rechts(-135, LOW);
    links(-135, LOW);*/

    /*fahreCm(STRAIGHT, 50, HIGH, 10);
    rechts(-135, LOW);
    links(-135, LOW);
    delay(200);
    onFwd(STRAIGHT, 0, LOW);*/
    return 0;
}
