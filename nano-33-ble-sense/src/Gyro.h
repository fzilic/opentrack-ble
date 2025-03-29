#ifndef _GYRO_
#define _GYRO_

#include <Arduino_BMI270_BMM150.h>
#include "GyroData.h"

enum GyroAxis
{
  GA_X,
  GA_X_I,
  GA_Y,
  GA_Y_I,
  GA_Z,
  GA_Z_I
};

class Gyro
{
public:
  Gyro() {}
  Gyro(GyroAxis, GyroAxis, GyroAxis);

  void zero();

  void calibrate(int samples = 5000);
  void printCalibrationResults();
  GyroData read();

  void setPitchAxis(GyroAxis);
  void setRollAxis(GyroAxis);
  void setYawAxis(GyroAxis);

private:
  float readAxis(GyroAxis, float, float, float, float);

  GyroAxis axisRoll = GA_X, axisPitch = GA_Y, axisYaw = GA_Z;
  float roll = 0.0, pitch = 0.0, yaw = 0.0;
  unsigned long lastTime = 0;

  // calibration data
  int samples = 0;
  float mid_x = 0.0, mid_y = 0.0, mid_z = 0.0;
};

#endif