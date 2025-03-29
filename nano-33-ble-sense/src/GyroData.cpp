#include <Arduino.h>
#include "GyroData.h"

GyroData::GyroData()
{
  this->hasData = false;
}

GyroData::GyroData(float roll, float pitch, float yaw)
{
  this->hasData = true;
  this->roll = roll;
  this->pitch = pitch;
  this->yaw = yaw;
}

void GyroData::print()
{
  Serial.print("R:");
  Serial.print(this->roll);
  Serial.print(",");
  Serial.print("P:");
  Serial.print(this->pitch);
  Serial.print(",");
  Serial.print("Y:");
  Serial.print(this->yaw);
  Serial.println();
}
