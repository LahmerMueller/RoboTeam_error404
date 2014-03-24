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

bool anySensor()
{
    if(!L1 || !L2 || !L3 || !L4 || !L5 || !L6 || !T1 || !T2)
    {
        return true;
    }
    return false;
}

bool anyBlack()
{
    if(!L1 || !L2 || !L3 || !L4 || !L5 || !L6)
    {
        return true;
    }
    return false;
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

void onFwd(int moto, int speed, bool direct)
{
    if(moto == 0)
    {
        links(speed, direct);
    }
    else if(moto == 1)
    {
        rechts(speed, direct);
    }
    else if(moto == 2)
    {
        rechts(speed, direct);
        links(speed, direct);
    }
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

void rotateMoto(int moto, int speed, int direct, int grad, bool withInter = false)
{
    int my_rotR;
    if(withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < grad && !anySensor())
        {
            onFwd(moto, speed, direct);
        }
    }
    else if(!withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < grad)
        {
            onFwd(moto, speed, direct);
        }
    }
}

void fahreCm(int moto, int speed, bool direct, int cm, bool withInter = false)
{
    int grad = cm * DPROP;
    int my_rotR;

    if(withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < grad && !anySensor())
        {
            onFwd(moto, speed, direct);
        }
    }
    else if(!withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < grad)
        {
            onFwd(moto, speed, direct);
        }
    }
}

void turn(int speed, bool turnDirect, int grad, bool withInter = false)
{
    int motoGrad = grad * TPROP;
    int my_rotR;

    if(withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < motoGrad && !anySensor())
        {
            onFwd(RIGHT, speed, !turnDirect);
            onFwd(LEFT, speed, turnDirect);
        }
    }
    else if(!withInter)
    {
        my_rotR = rotR;
        while((rotR - my_rotR) < motoGrad)
        {
            onFwd(RIGHT, speed, !turnDirect);
            onFwd(LEFT, speed, turnDirect);
        }
    }
}

int sharp(bool direct)
{
    int val = 0;
    if(direct == RIGHT)
    {
        val = analogRead(A14);
        return val;
    }
    else if(direct == LEFT)
    {
        val = analogRead(A15);
        return val;
    }
    return val;
}

void doseFinden()
{
    dosDirect = LEFT;

    while(T1 || T2)
    {
        onFwd(STRAIGHT, 100, HIGH);
    }
    fahreCm(STRAIGHT, 50, LOW, 5);
    turn(50, LEFT, 90);
    onFwd(STRAIGHT, 50, LOW);
    delay(2000);

    while(sharp(dosDirect) < DISTANCE)
    {
        onFwd(LEFT, 35, HIGH);
        onFwd(RIGHT, 65, HIGH);

        if(!T1 && !T2)
        {
            fahreCm(STRAIGHT, 50, LOW, 5);
            turn(50, dosDirect, 90);
            fahreCm(STRAIGHT, 50, HIGH, 10);
            turn(50, !dosDirect, 60);
            while(T1 || T2)
            {
                onFwd(STRAIGHT, 50, HIGH);
            }
            fahreCm(STRAIGHT, 50, LOW, 5);
            turn(50, dosDirect, 180);
            onFwd(STRAIGHT, 50, LOW);
            delay(3000);
            dosDirect = !dosDirect;
        }
    }
    /*else if(sharp() >= DISTANCE)
    {*/
    turn(50, !dosDirect, 60);


    /*onFwd(STRAIGHT, 50, LOW);
    delay(10000);
    while(T1 || T2)
    {
        onFwd(STRAIGHT, 50, HIGH);
    }
    fahreCm(STRAIGHT, 50, HIGH, 10);
    turn(50, LEFT, 90);
    fahreCm(STRAIGHT, 50, HIGH, 20);
    turn(50, RIGHT, 90);
    while(T1 || T2)
    {
        onFwd(STRAIGHT, 50, HIGH);
    }
    delay(2000);
    fahreCm(STRAIGHT, 50, LOW, 2);
    turn(50, RIGHT, 90);

    while(!anyBlack())
    {
        onFwd(STRAIGHT, 50, HIGH);
    }

    fahreCm(STRAIGHT, 50, HIGH, 5);

    fahreCm(STRAIGHT, 100, LOW, 30);*/
    //}
    onFwd(STRAIGHT, 0, LOW);
    delay(9000);
}

void followLine()
{
    Serial.print(lastLight);
    Serial.println(rotR - rotSeek);
    if(!L2 || !L3 || !L4 || !L5)
    {
        lastLight = ONLINE;
    }
    else if(L1 && L2 && L3 && L4 && L5 && L6 && lastLight == ONLINE)
    {
        lastLight = OFFLINE;
        rotSeek = rotR;
    }

    if(lastLight == OFFLINE && (rotR - rotSeek) > 1000)
    {
        doseFinden();
    }
    else if((lastLight == RIGHT || lastLight == LEFT) && (rotR - rotSeek) > 25)
    {
        if(lastLight == RIGHT)
        {
            lastLight = OFFLINE;
            rotSeek = rotR;
            inter = anyBlack();
            while((rotR - rotSeek) < 150 && !inter)
            {
                rechts(50, LOW);
                links(50, HIGH);
                inter = anyBlack();
            }
            rotSeek = rotR;
            while((rotR - rotSeek) < 150 && !inter)
            {
                rechts(50, HIGH);
                links(50, LOW);
                inter = anyBlack();
            }
        }
        else if(lastLight == LEFT)
        {
            lastLight = OFFLINE;
            rotSeek = rotR;
            inter = anyBlack();
            while((rotR - rotSeek) < 150 && !inter)
            {
                rechts(50, HIGH);
                links(50, LOW);
                inter = anyBlack();
            }
            rotSeek = rotR;
            while((rotR - rotSeek) < 150 && !inter)
            {
                rechts(50, LOW);
                links(50, HIGH);
                inter = anyBlack();
            }
        }
        rotSeek = rotR;
    }

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

        lastLight = RIGHT;
        rotSeek = rotR;
    }
    else if(!L1)
    {
        digitalWrite(M1, HIGH);
        analogWrite(E1, 255);
        digitalWrite(M2, LOW);
        analogWrite(E2, 255);

        lastLight = LEFT;
        rotSeek = rotR;
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

void onTouch()
{
    if(!T1 || !T2)
    {
        if(!T1 && !T2)
        {
            bool flaschDirect = LEFT;
            onFwd(STRAIGHT, 0, HIGH);

            /*if(ultraschall(TRIG1, PWM1, validRead) > 25 && ultraschall(TRIG2, PWM2, validRead) > 25)
            {
                flaschDirect = RIGHT;
            }
            else
            {
                flaschDirect = LEFT;
            }*/

            fahreCm(STRAIGHT, 75, LOW, 10);
            turn(75, flaschDirect, 90);
            fahreCm(STRAIGHT, 75, HIGH, 25);
            turn(75, !flaschDirect, 90);
            fahreCm(STRAIGHT, 75, HIGH, 40);
            turn(75, !flaschDirect, 80);

            if(flaschDirect == RIGHT)
            {
                while(!anyBlack())
                {
                    onFwd(STRAIGHT, 50, HIGH);
                }
                if(!L1 || !L2 || !L3)
                {
                    fahreCm(STRAIGHT, 50, HIGH, 10);
                    turn(75, flaschDirect, 90);
                    //turn(75, flaschDirect, 50, true);
                    onFwd(STRAIGHT, 0, LOW);
                    turn(75, !flaschDirect, 100, true);
                    onFwd(STRAIGHT, 0, LOW);
                }
                else if(!L4 || !L5 || !L6)
                {
                    turn(75, flaschDirect, 60);
                }
            }
            else
            {
                while(!anyBlack())
                {
                    onFwd(STRAIGHT, 50, HIGH);
                }
                if(!L4 || !L5 || !L6)
                {
                    fahreCm(STRAIGHT, 50, HIGH, 10);
                    turn(75, flaschDirect, 90);
                    //turn(75, flaschDirect, 50, true);
                    onFwd(STRAIGHT, 0, LOW);
                    turn(75, !flaschDirect, 100, true);
                    onFwd(STRAIGHT, 0, LOW);
                }
                else if(!L1 || !L2 || !L3)
                {
                    turn(75, flaschDirect, 60);
                }
            }
        }
        else if(!T1 && T2 && anyBlack() && lastLight != RIGHT && lastLight != LEFT)
        {
            fahreCm(STRAIGHT, 50, LOW, 4);
            turn(100, RIGHT, 5);
            fahreCm(RIGHT, 50, HIGH, 6);
        }
        else if(T1 && !T2 && anyBlack() && lastLight != RIGHT && lastLight != LEFT)
        {
            fahreCm(STRAIGHT, 50, LOW, 4);
            turn(100, LEFT, 5);
            fahreCm(LEFT, 50, HIGH, 6);
        }
    }
}

void onTouchV2()
{
    if(!T1 || !T2)
    {
        onFwd(STRAIGHT, 50, HIGH);
        delay(150);

        if(!T1 && T2)
        {
            fahreCm(STRAIGHT, 50, LOW, 10);
            turn(50, LEFT, 20, true);

            rotSeek = rotSeek + 760;
        }
        else if(T1 && !T2)
        {
            fahreCm(STRAIGHT, 50, LOW, 10);
            turn(50, RIGHT, 20, true);

            rotSeek = rotSeek + 760;
        }
        else if(!T1 && !T2)
        {
            onFwd(STRAIGHT, 0, LOW);
            bool flaschDirect = LEFT;

            if(ultraschall(TRIG1, PWM1, validRead, true) > ultraschall(TRIG2, PWM2, validRead, true))
            {
                flaschDirect = RIGHT;
            }

            fahreCm(STRAIGHT, 50, LOW, 4);
            turn(50, flaschDirect, 55);
            int my_distance;
            int my_rotR = rotR;
            int my_rotL = rotL;

            while((flaschDirect ? (rotR - my_rotR) : (rotL - my_rotL)) < 180 || !anyBlack())
            {
                if(!T1 || !T2)
                {
                    fahreCm(STRAIGHT, 50, LOW, 2);
                    turn(50, flaschDirect, 7);

                    flaschDirect ? my_rotR = my_rotR + 160 : my_rotL = my_rotL + 160;

                    if((flaschDirect ? (rotR - my_rotR) : (rotL - my_rotL)) < 0)
                    {
                        flaschDirect ? my_rotR = rotR : my_rotL = rotL;
                    }
                }

                my_distance = sharp(!flaschDirect);
                if(my_distance == DISTANCE)
                {
                    onFwd(!flaschDirect, 75, LOW);
                    onFwd(flaschDirect, 100, HIGH);
                }
                if(sharp(!flaschDirect) < DISTANCE)
                {
                    onFwd(STRAIGHT, 50, HIGH);
                }
                else
                {
                    onFwd(!flaschDirect, 50, LOW);
                    onFwd(flaschDirect, 100, HIGH);
                }
            }
            onFwd(STRAIGHT, 0, LOW);
            fahreCm(STRAIGHT, 50 , HIGH, 10);
            turn(50, flaschDirect, 30);
        }
    }
}

#endif // FUNCTIONS_H_INCLUDED
