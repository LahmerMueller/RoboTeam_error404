#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void serialWrite(int msg)
{
    Serial.write((byte)(msg >> 8));
    Serial.write((byte) msg);
    Serial.println();
}

void rotateR()
{
    rotR = rotR + 1;
}

void rotateL()
{
    rotL = rotL + 1;
}

void rechts(int speed, bool direct)
{
    if(speed == 0)
    {
        digitalWrite(M1, direct);
        analogWrite(E1, 0);
    }
    else
    {
        digitalWrite(M1, direct);
        analogWrite(E1, speed + 155);
    }
}

void links(int speed, bool direct)
{
    if(speed == 0)
    {
        digitalWrite(M2, direct);
        analogWrite(E2, 0);
    }
    else
    {
        digitalWrite(M2, direct);
        analogWrite(E2, speed + 155);
    }
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
    if(!L6)
    {
        if(!L1 || !L2)
        {
            digitalWrite(M1, LOW);
            analogWrite(E1, 155);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 255);
        }
        else
        {
            digitalWrite(M1, LOW);
            analogWrite(E1, 255);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 255);
        }
    }
    else if(!L1)
    {
        digitalWrite(M1, HIGH);
        analogWrite(E1, 255);
        digitalWrite(M2, LOW);
        analogWrite(E2, 255);
    }
    else
    {
        speedR = 0;
        speedL = 0;
        for(int i = 0; i < 4; i++)
        {
            speedR = speedR + werte[0][i] * !digitalRead(i + 22);
            speedL = speedL + werte[1][i] * !digitalRead(i + 22);
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

        if(speedR == speedL)
        {
            digitalWrite(M1, HIGH);
            analogWrite(E1, 155);
            digitalWrite(M2, HIGH);
            analogWrite(E2, 155);
        }
        else
        {
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
                digitalWrite(M1, (speedL < 0));
                analogWrite(E1, 10);
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
                digitalWrite(M2, (speedR < 0));
                analogWrite(E2, 10);
            }
        }
    }
}

void followLineV2()
{
    if(!L5)
    {
        rechts(0, LOW);
        links(100, HIGH);
    }
    else if(L1 && L2 && L3 && L4 && L5)
    {
        rechts(100, HIGH);
        links(100, HIGH);
    }
    else if(!L4)
    {
        rechts(100, LOW);
        links(100, HIGH);
    }
    else if(!L1)
    {
        rechts(100, HIGH);
        links(100, LOW);
    }
    else if(!L3)
    {
        rechts(0, LOW);
        links(100, HIGH);
    }
    else if(!L2)
    {
        rechts(100, HIGH);
        links(0, LOW);
    }
}

void onTouch()
{

        while(!T2)
        {
            rechts(100, LOW);
            links(-55, HIGH);
        }
        /*while(T1)
        {*/
            rechts(100, HIGH);
            links(-55, LOW);
        //}

}

unsigned int ultraschall(byte trig, byte pwm, bool *validRead, boolean raw = false)
{
    unsigned int pulse = 0;

    digitalWrite(trig, LOW);
    digitalWrite(trig, HIGH);

    pulse = pulseIn(pwm, LOW);

    *validRead = true;
    if(pulse > 50000 || pulse < 0)
    {
        *validRead = false;
    }

    if(raw)
    {
        return pulse;
    }
    else
    {
        return pulse / 50;
    }
}

#endif // FUNCTIONS_H_INCLUDED
