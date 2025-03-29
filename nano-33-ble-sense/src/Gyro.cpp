#include <Arduino.h>
#include "Gyro.h"

Gyro::Gyro(GyroAxis roll, GyroAxis pitch, GyroAxis yaw)
{
  this->axisRoll = roll;
  this->axisPitch = pitch;
  this->axisYaw = yaw;
}

void Gyro::zero()
{
  this->roll = 0.0;
  this->pitch = 0.0;
  this->yaw = 0.0;
}

void Gyro::calibrate(int samples)
{
  this->samples = 0;

  this->mid_x = 0.0;
  this->mid_y = 0.0;
  this->mid_z = 0.0;

#ifdef _DEBUG_
  unsigned long time = millis();
  unsigned long readTime = 0;
#endif

  float sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;

  while (this->samples < samples)
  {
#ifdef _DEBUG_
    if (millis() - time > 1000)
    {
      time = millis();
      Serial.print("CS:");
      Serial.println(this->samples);
    }
#endif

    if (IMU.gyroscopeAvailable())
    {
      float x, y, z;
      IMU.readGyroscope(x, y, z);

      sum_x += x;
      sum_y += y;
      sum_z += z;

      this->samples += 1;
    }
  }

  this->mid_x = sum_x / samples;
  this->mid_y = sum_y / samples;
  this->mid_z = sum_z / samples;

  this->zero();
}

void Gyro::printCalibrationResults()
{
  Serial.print("ZRO_X:");
  Serial.print(this->mid_x, 10);
  Serial.print(",");
  Serial.print("ZRO_Y:");
  Serial.print(this->mid_y, 10);
  Serial.print(",");
  Serial.print("ZRO_Z:");
  Serial.print(this->mid_z, 10);
  Serial.println();
  delay(5000);
}

GyroData Gyro::read()
{
  if (IMU.gyroscopeAvailable())
  {
    float x, y, z;
    IMU.readGyroscope(x, y, z);
    unsigned long now = millis();

    if (this->lastTime > 0)
    {
      float delta = (now - this->lastTime) / 1000.0;
      this->lastTime = now;

      this->roll += this->readAxis(this->axisRoll, delta, x, y, z);
      this->pitch += this->readAxis(this->axisPitch, delta, x, y, z);
      this->yaw += this->readAxis(this->axisYaw, delta, x, y, z);

      return GyroData(
          this->roll,
          this->pitch,
          this->yaw);
    }
    else
    {
      this->lastTime = now;
    }
  }

  return GyroData();
}

void Gyro::setPitchAxis(GyroAxis axis)
{
  this->axisPitch = axis;
}

void Gyro::setRollAxis(GyroAxis axis)
{
  this->axisRoll = axis;
}

void Gyro::setYawAxis(GyroAxis axis)
{
  this->axisYaw = axis;
}

float Gyro::readAxis(GyroAxis axis, float delta, float x, float y, float z)
{
  float value = 0.0;
  float mid = 0.0;

  switch (axis)
  {
  case GA_X:
  case GA_X_I:
    value = x;
    mid = this->mid_x;
    break;
  case GA_Y:
  case GA_Y_I:
    value = y;
    mid = this->mid_y;
    break;
  case GA_Z:
  case GA_Z_I:
    value = z;
    mid = this->mid_z;
    break;
  }

  value -= mid;
  value *= delta;

  if (axis == GA_X_I || axis == GA_Y_I || axis == GA_Z_I)
  {
    value *= -1;
  }

  return value;
}
