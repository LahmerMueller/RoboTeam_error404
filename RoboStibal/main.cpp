#include <Wire.h>
#include <WProgram.h> //import main Arduino header file
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
    pinMode(13, OUTPUT);

    pinMode(22, INPUT);
    pinMode(23, INPUT);
    pinMode(24, INPUT);
    pinMode(25, INPUT);
    pinMode(26, INPUT);
    pinMode(27, INPUT);

    attachInterrupt(3, rotateR, FALLING);
    attachInterrupt(2, rotateL, FALLING);

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
    int old_rotL = 0;
    int old_rotR = 0;

    while(true)
    {
        /*readFrom(ADXL345, accelVal);
        sprintf(str, "%d %d %d", accelVal[0] - calbAccelVal[0], accelVal[1] - calbAccelVal[1], accelVal[2] - calbAccelVal[2]);
        Serial.println(str);

        delay(500);*/

        serialWrite(312);
        delay(1000);

        //followLine();
    }

    return 0;
}
