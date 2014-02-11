#ifndef MOTORCLASS_H_INCLUDED
#define MOTORCLASS_H_INCLUDED

class Motor
{
public:
    Motor(uint8_t my_E1, uint8_t my_M1, uint8_t my_E2, uint8_t my_M2)
    {
        E1 = my_E1;
        M1 = my_M1;
        E2 = my_E2;
        M2 = my_M2;
    }

    void onFwdSync(int my_speed)
    {

    }

private:
    uint8_t E1, M1, E2, M2;
    int speedR;
    int speedL;
    int* degreeR, degreeL;
};

#endif // MOTORCLASS_H_INCLUDED
