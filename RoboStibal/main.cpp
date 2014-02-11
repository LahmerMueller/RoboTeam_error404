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
}

int main()
{
    init(); //Don't forget this!

    setup();

    while(true)
    {
        followLine();
    }

    return 0;
}
