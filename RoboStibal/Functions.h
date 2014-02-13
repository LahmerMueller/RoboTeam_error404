#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void rotateR()
{
    rotR = rotR + 1;
}

void rotateL()
{
    rotL = rotL + 1;
}

class Moto
{
public:
    void resetRot()
    {
        my_rotateL = rotL;
        my_rotateR = rotR;
    }

    void onFwdSync(int speed, bool direct)
    {

        digitalWrite(M1, direct);
        analogWrite(E1, speed);
        digitalWrite(M2, direct);
        analogWrite(E2, speed);
    }

private:
    int my_rotateR;
    int my_rotateL;
};

void followLine()
{
    while(!L5)
    {
        digitalWrite(M2, HIGH);
        analogWrite(E2, 255);

        while(L1 && L2 && L3 && L4)
        {
            digitalWrite(M1, LOW);
            analogWrite(E1, 255);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 255);
        }
    }

    speedR = 0;
    speedL = 0;
    for(int i = 0; i < 4; i++)
    {
        speedR = speedR + werte[0][i] * !digitalRead(i + 22);
        speedL = speedL + werte[1][i] * !digitalRead(i + 22);
    }

    if(!L1 || !L4)
    {
        drillPlus = 0;
    }

    if(speedR > speedL)
    {
        if(steerDirect != LEFT)
        {
            steerDirect = LEFT;
            drillPlus = 0;
        }
        speedR += drillPlus;
        drillPlus += 0.1;
    }
    else if(speedL > speedR)
    {
        if(steerDirect != RIGHT)
        {
            steerDirect = RIGHT;
            drillPlus = 0;
        }
        speedL -= drillPlus;
        drillPlus -= 0.1;
    }

    if(drillPlus > 99)
    {
        drillPlus = 99;
    }
    else if(drillPlus < -99)
    {
        drillPlus = -99;
    }

    if(speedR > 100)
    {
        speedR = 100;
    }
    else if(speedR < -100)
    {
        speedR = -100;
    }

    if(speedL > 100)
    {
        speedL = 100;
    }
    else if(speedL < -100)
    {
        speedL = -100;
    }

    /*if(speedL < 0 && speedR < 0)
    {*/
        Serial.print("SpeedR: ");
        Serial.println(speedR);

        Serial.print("SpeedL: ");
        Serial.println(speedL);

        Serial.print("drillPlus: ");
        Serial.println(drillPlus);
    //}
    /*else if(old_speedL != speedL)
    {

        Serial.print("SpeedR: ");
        Serial.println(speedR);

        Serial.print("SpeedL: ");
        Serial.println(speedL);
    }

    old_speedR = speedR;
    old_speedL = speedL;*/

    if(speedR > 0)
    {
        digitalWrite(M1, HIGH);
        analogWrite(E1, 155 + speedR);
    }
    else if(speedR < 0)
    {
        digitalWrite(M1, LOW);
        analogWrite(E1, 155 +  (-1) * speedR);
    }
    else
    {
        digitalWrite(M1, HIGH);
        analogWrite(E1, 155);
    }

    if(speedL > 0)
    {
        digitalWrite(M2, HIGH);
        analogWrite(E2, 155 + speedL);
    }
    else if(speedL < 0)
    {
        digitalWrite(M2, LOW);
        analogWrite(E2, 155 +  (-1) * speedL);
    }
    else
    {
        digitalWrite(M2, HIGH);
        analogWrite(E2, 155);
    }
}

void onTouch()
{
    if(T1 || T2)
    {
        while(true)
        {
        while(T1)
        {
            digitalWrite(M1, LOW);
            analogWrite(E1, 255);
            digitalWrite(M2, LOW);
            analogWrite(E2, 0);
        }
        while(!T1)
        {
            digitalWrite(M1, HIGH);
            analogWrite(E1, 255);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 225);
        }
        }
    }
}
#endif // FUNCTIONS_H_INCLUDED
