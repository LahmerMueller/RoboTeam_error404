/**
 *  @version 17.2.2014
 *
 *  needs Wire.h
 */

#ifndef ACCEL_ADXL345_H_INCLUDED
#define ACCEL_ADXL345_H_INCLUDED

#define ADXL345 0x53

void writeTo(int device, byte val)
{
    Wire.beginTransmission(device);
    Wire.write(0x2D);
    Wire.write(val);
    Wire.endTransmission();
}

void readFrom(int device, int buff[])
{
    byte rawBuff[6];
    Wire.beginTransmission(device);
    Wire.write(0x32);
    Wire.endTransmission();

    Wire.beginTransmission(device);
    Wire.requestFrom(device, 6);

    int i = 0;
    while(Wire.available())
    {
        rawBuff[i] = Wire.read();
        i++;
    }
    Wire.endTransmission();

    buff[0] = (((int) rawBuff[1]) << 8) | rawBuff[0];
    buff[1] = (((int) rawBuff[3]) << 8) | rawBuff[2];
    buff[2] = (((int) rawBuff[5]) << 8) | rawBuff[4];
}

#endif // ACCEL_ADXL345_H_INCLUDED
