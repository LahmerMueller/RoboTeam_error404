#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define RIGHT 1
#define LEFT 0

#define E1 4
#define M1 5
#define E2 6
#define M2 7

#define L1 digitalRead(22)
#define L2 digitalRead(23)
#define L3 digitalRead(24)
#define L4 digitalRead(25)
#define L5 digitalRead(51)

#define T1 digitalRead(26)
#define T2 digitalRead(27)

const int werte[2][4] = {{100, 30, -1, -100},
                         {-100, -1, 30, 100}};

int speedR, speedL;
int old_speedR;
int old_speedL;

double drillPlus = 0;
bool steerDirect;
bool greenPoint = false;

#endif // DEFS_H_INCLUDED
