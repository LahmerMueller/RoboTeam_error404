#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void serialWrite(int msg)
{
    Serial.write((byte)(msg >> 8));
    Serial.write(msg);
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

bool anyBlackV2()
{
    if(!L1 || !L2 || !L3 || !L4 || !L5 /*|| !L6*/)
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
    //int motoGrad = grad;
    int my_rotR;
    int my_rotL;

    if(withInter)
    {
        my_rotR = rotR;
        my_rotL = rotL;
        while(((rotR - my_rotR) < motoGrad || (rotL - my_rotL) < motoGrad) && !anySensor())
        {
            onFwd(RIGHT, speed * ((rotR - my_rotR) < motoGrad), !turnDirect);
            onFwd(LEFT, speed * ((rotL - my_rotL) < motoGrad), turnDirect);
        }
    }
    else if(!withInter)
    {
        my_rotR = rotR;
        my_rotL = rotL;
        while((rotR - my_rotR) < motoGrad || (rotL - my_rotL) < motoGrad)
        {
            onFwd(RIGHT, speed * ((rotR - my_rotR) < motoGrad), !turnDirect);
            onFwd(LEFT, speed * ((rotL - my_rotL) < motoGrad), turnDirect);
        }
    }
}

int sharp(int direct)
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
    else if(direct == MIDDLE)
    {
        val = analogRead(A13);
        return val;
    }

    return val;
}

void doseFinden()
{
    dosDirect = RIGHT;
    bool raumDirect = dosDirect;

    while(T1 || T2)
    {
        //onFwd(RIGHT, 100 * (T1 ? 1 : 0), HIGH);
        //onFwd(LEFT, 100 * (T2 ? 1 : 0), HIGH);
        onFwd(STRAIGHT, 25, HIGH);
    }
    fahreCm(STRAIGHT, 50, LOW, 5);
    turn(50, dosDirect, 90);
    onFwd(STRAIGHT, 50, LOW);
    delay(5000);

    while(sharp(dosDirect) < DISTANCE)
    {
        onFwd(LEFT, 35, HIGH);
        onFwd(RIGHT, 65, HIGH);

        if(!T1 && !T2)
        {
            fahreCm(STRAIGHT, 50, LOW, 6);
            turn(50, dosDirect, 90);
            fahreCm(STRAIGHT, 50, HIGH, 15);
            turn(50, !dosDirect, 90);
            while(T1 || T2)
            {
                onFwd(STRAIGHT, 50, HIGH);
            }
            fahreCm(STRAIGHT, 50, LOW, 6);
            turn(50, dosDirect, 180);
            onFwd(STRAIGHT, 50, LOW);
            delay(3000);
            dosDirect = !dosDirect;
        }
    }

    fahreCm(STRAIGHT, 50, LOW, 3);
    /*else if(sharp() >= DISTANCE)
    {*/
    turn(50, !dosDirect, 60);

    onFwd(STRAIGHT, 0, HIGH);

    /*int my_rotL = rotL;
    while((rotL - my_rotL) < 360)
    {
        onFwd(RIGHT, 20, LOW);
        onFwd(LEFT, 50, LOW);
    }
    onFwd(STRAIGHT, 0, HIGH);*/
    //int my_grad = 0;
    while(sharp(MIDDLE) < DISTANCE)
    {
        onFwd(!dosDirect, 1, LOW);
        onFwd(dosDirect, 1, HIGH);
        //my_grad++;
    }

    //turn(50, dosDirect, 5);

    fahreCm(STRAIGHT, 50, LOW, 10);

    while(sharp(MIDDLE) < DISTANCE)
    {
        onFwd(!dosDirect, 1, LOW);
        onFwd(dosDirect, 1, HIGH);
        //my_grad++;
    }

    //turn(50, dosDirect, 5);

    fahreCm(STRAIGHT, 50, LOW, 10);

    while(sharp(MIDDLE) < DISTANCE)
    {
        onFwd(!dosDirect, 1, LOW);
        onFwd(dosDirect, 1, HIGH);
        //my_grad++;
    }

    onFwd(STRAIGHT, 0, LOW);
    myservo.write(160);
    delay(2000);

    //turn(50, LEFT, 120 - my_grad);
    while(T1 || T2)
    {
        onFwd(STRAIGHT, 50, HIGH);
    }
    fahreCm(STRAIGHT, 50, LOW, 6);
    turn(50, raumDirect, 86);

    while(!anyBlack())
    {
        onFwd(STRAIGHT, 50, HIGH);
        if(!T1 && !T2)
        {
            fahreCm(STRAIGHT, 50, LOW, 6);
            turn(50, raumDirect, 90);
        }
    }

    /*while(L1)
    {
        onFwd(RIGHT, 50, HIGH);
        onFwd(LEFT, 0, LOW);
    }*/
    /*while(raumDirect ? L6 : L1)
    {
        onFwd(!raumDirect, 0, LOW);
        onFwd(raumDirect, 50, HIGH);
    }*/

    turn(50, !raumDirect, 60);
    fahreCm(STRAIGHT, 50, LOW, 3);
    //fahreCm(!raumDirect, 50, LOW, 10);

    fahreCm(STRAIGHT, 50, LOW, 10);
    turn(50, !raumDirect, 180);
    onFwd(STRAIGHT, 0, LOW);

    /*turn(100, LEFT, 90);
    while(T1 || T2)
    {
        onFwd(STRAIGHT, 50, HIGH);
    }

    fahreCm(STRAIGHT, 50, LOW, 4);
    turn(50, LEFT, 100);
    onFwd(STRAIGHT, 0, LOW);*/

    myservo.write(20);
    delay(2000);

    fahreCm(STRAIGHT, 50, LOW, 30);
    fahreCm(STRAIGHT, 50, HIGH, 20);
    onFwd(STRAIGHT, 0, LOW);

    delay(8000);
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
    if(!L2 || !L3 || !L4 || !L5)
    {
        lastLight = ONLINE;
    }
    else if(L1 && L2 && L3 && L4 && L5 && L6 && lastLight == ONLINE)
    {
        lastLight = OFFLINE;
        rotSeek = rotR;
    }

    if(lastLight == OFFLINE && (rotR - rotSeek) > 825)
    {
        /*int gotLine = 0;
        turn(50, LEFT, 45, true);
        gotLine += anyBlack() ? 1 : 0;
        turn(50, RIGHT, 90, true);
        gotLine += anyBlack() ? 1 : 0;
        turn(50, LEFT, 45, true);
        gotLine += anyBlack() ? 1 : 0;
        if(gotLine == 0)
        {*/
        digitalWrite(13, HIGH);
            doseFinden();
        //}
    }
    else if((lastLight == RIGHT || lastLight == LEFT) && (rotR - rotSeek) > 10)
    {
        if(lastLight == RIGHT)
        {
            lastLight = OFFLINE;
            rotSeek = rotL;
            inter = anyBlack();
            while((rotL - rotSeek) < 150 && !inter)
            {
                /*rechts(((rotL - rotSeek) / 6) - 155, HIGH);
                links(100, HIGH);
                inter = anyBlack();*/

                onFwd(RIGHT, 50, LOW);
                onFwd(LEFT, 50, HIGH);
                inter = anyBlack();
            }
            rotSeek = rotL;
            while((rotL - rotSeek) < 150 && !inter)
            {
                /*rechts((50 - ((rotL - rotSeek) / 6)) - 155, LOW);
                links(50, LOW);
                inter = anyBlack();*/

                onFwd(RIGHT, 50, HIGH);
                onFwd(LEFT, 50, LOW);
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
                /*rechts(100, HIGH);
                links(((rotR - rotSeek) / 6) - 155, HIGH);
                inter = anyBlack();*/

                onFwd(RIGHT, 50, HIGH);
                onFwd(LEFT, 50, LOW);
                inter = anyBlack();
            }
            rotSeek = rotR;
            while((rotR - rotSeek) < 150 && !inter)
            {
                /*rechts(50, LOW);
                links((50 - ((rotR - rotSeek) / 6)) - 155, LOW);
                inter = anyBlack();*/

                onFwd(RIGHT, 50, LOW);
                onFwd(LEFT, 50, HIGH);
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
        if(!L5)
        {
            digitalWrite(M1, HIGH);
            analogWrite(E1, 255);
            digitalWrite(M2, LOW);
            analogWrite(E2, 155);
        }
        else
        {
            digitalWrite(M1, HIGH);
            analogWrite(E1, 255);
            digitalWrite(M2, LOW);
            analogWrite(E2, 255);
        }

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
            /*if(straightFwd != ONSTRAIGHT)
            {
                rotRotR = rotR;
                rotRotL = rotL;
                my_time = millis();

                straightFwd = ONSTRAIGHT;
            }

            if(millis() - my_time > 100)
            {
                if(rotL - rotRotL < 10 || rotR - rotRotR < 10)
                {
                    digitalWrite(13, HIGH);
                    //fahreCm(STRAIGHT, 100, HIGH, 2);
                    onFwd(STRAIGHT, 100, HIGH);
                    delay(200);
                }
                else
                {
                    digitalWrite(13, LOW);
                }

                rotRotR = rotR;
                rotRotL = rotL;
                my_time = millis();
                LEDStatus = !LEDStatus;

                straightFwd = OFFSTRAIGHT;
            }
            else
            {*/
                digitalWrite(M1, HIGH);
                analogWrite(E1, 180);
                digitalWrite(M2, HIGH);
                analogWrite(E2, 180);
            //}
        }
        else
        {
            //straightFwd = OFFSTRAIGHT;

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
                analogWrite(E1, 100);
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
                analogWrite(E2, 100);
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
            bool flaschDirect = RIGHT;

            /**if(ultraschall(TRIG1, PWM1, validRead, true) > ultraschall(TRIG2, PWM2, validRead, true))
            {
                flaschDirect = RIGHT;
            }*/

            fahreCm(STRAIGHT, 50, LOW, 4);
            turn(50, flaschDirect, 55);
            int my_distance;
            int my_rotR = rotR;
            int my_rotL = rotL;

            while((flaschDirect ? (rotR - my_rotR) : (rotL - my_rotL)) < 180 || !anyBlackV2())
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
                    onFwd(!flaschDirect, 100, LOW);
                    onFwd(flaschDirect, 0, HIGH);
                }
            }
            onFwd(STRAIGHT, 0, LOW);
            fahreCm(STRAIGHT, 50 , HIGH, 10);
            turn(50, flaschDirect, 30);
        }
    }
}

void silberStreifen()
{
    if(!S1)
    {
        fahreCm(STRAIGHT, 25, HIGH, 6);
        //digitalWrite(13, LEDStatus);
        //LEDStatus = !LEDStatus;

        lastLight = OFFLINE;
    }
}

void loadingStatus()
{
    digitalWrite(14, (analogRead(A0) < 694));
}

#endif // FUNCTIONS_H_INCLUDED
