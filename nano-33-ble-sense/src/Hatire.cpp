#include <Arduino.h>
#include "Hatire.h"
#include "GyroData.h"

void Hatire::send(GyroData data)
{
  this->data.gyro[0] = data.yaw;
  this->data.gyro[1] = data.pitch;
  this->data.gyro[2] = data.roll;

#ifndef _DEBUG_
  Serial.write((byte *)&this->data, sizeof(this->data));
#endif

  this->data.Cpt += 1;

  if (this->data.Cpt > 999)
  {
    this->data.Cpt = 0;
  }
}