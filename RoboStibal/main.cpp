#include <WProgram.h> //import main Arduino header file
#include <Wire.h>
#include "Defs.h"
#include "Functions.h"

void setup()
{
    Serial.begin(9600);

    pinMode(E1, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(E2, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(13, OUTPUT);

    pinMode(22, INPUT);
    pinMode(23, INPUT);
    pinMode(24, INPUT);
    pinMode(25, INPUT);
    pinMode(26, INPUT);
    pinMode(27, INPUT);

    attachInterrupt(3, rotateR, FALLING);
    attachInterrupt(2, rotateL, FALLING);
}

int main()
{
    init(); //Don't forget this!

    setup();

    int old_rotL = 0;
    int old_rotR = 0;

    while(true)
    {
            /*digitalWrite(M1, HIGH);
            analogWrite(E1, 255);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 255);*/

            Serial.print("rechts: ");
            Serial.println(T1);
            Serial.print("links: ");
            Serial.println(T1);

            //onTouch();

        //followLine();
    }

    return 0;
}
