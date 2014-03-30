#include <WProgram.h> //import main Arduino header file
#include <Wire.h>
#include <Servo.h>
#include "Accel_ADXL345.h"
#include "Defs.h"
#include "Functions.h"

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    myservo.attach(9);
    myservo.write(20);

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


    /*int ground = 0;
    int ramp = 0;

    while(T1 && T2);
    readFrom(ADXL345, accelVal);
    ground = accelVal[2];
    while(!T1 || !T2);
    delay(2000);
    while(T1 && T2);
    readFrom(ADXL345, accelVal);
    ramp = accelVal[2];
    while(!T1 || !T2);

    Serial.println((ground + ramp)/2);*/
    unsigned long my_time;

    my_time = millis();
    while(true)
    {
        /*readFrom(ADXL345, accelVal);
        serialWrite(accelVal[2]);*/
        //Serial.println(sizeof(int));
        /*serialWrite(accelVal[2]);
        serialWrite(accelVal[3]);*/

        /*if((millis() - my_time) > 100)
        {
            char str[50];
            readFrom(ADXL345, accelVal);
            sprintf(str, "%d %d %d", accelVal[0], accelVal[1], accelVal[2]);
            Serial.println(str);
            my_time = millis();
        }*/
        //delay(500);

        //serialWrite(-36);
        //Serial.write(-127);

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

        //doseFinden();

        //Serial.println(sharp(MIDDLE));

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
    /*char str[20];
    int msg = 20;
    int result;
    byte test1;
    byte test2;
    sprintf(str, "%x", msg);
    Serial.println(msg);

    test1 = (byte)(msg >> 8);
    //Serial.println(test1);
    test2 = (byte) msg;
    //Serial.println(test2);

    result = (int) test1;
    Serial.println(result);
    result = (result << 8) | test2;
    Serial.println(result);*/

    return 0;
}
