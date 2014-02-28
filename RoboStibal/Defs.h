#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define ONLINE 3
#define RIGHT 1
#define LEFT 0

#define E1 4
#define M1 5
#define E2 6
#define M2 7

#define L1 digitalRead(53)
#define L2 digitalRead(22)
#define L3 digitalRead(23)
#define L4 digitalRead(24)
#define L5 digitalRead(25)
#define L6 digitalRead(52)

#define T1 digitalRead(26)
#define T2 digitalRead(27)

#define TRIG1 37
#define PWM1 38
#define TRIG2 39
#define PWM2 40

const int werte[2][4] = {{100, 50, 0, -100},
                         {-100, 0, 50, 100}};

int speedR, speedL;

volatile int rotR = 0, rotL = 0;

bool *validRead;

char str[512];
int accelVal[3];
int calbAccelVal[3];

/// 3 = Online, 1 = Right, 0 = Left
int lastLight = 3;
bool inter = false;
int rotSeek;

#endif // DEFS_H_INCLUDED
