#ifndef _GYRO_DATA_
#define _GYRO_DATA_

class GyroData
{
public:
  GyroData();
  GyroData(float, float, float);

  void print();

  bool hasData;
  float roll, pitch, yaw;
};

#endif