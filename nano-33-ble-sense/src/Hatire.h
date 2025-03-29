#ifndef _HATIRE_
#define _HATIRE_

#include "GyroData.h"

struct HatireData
{
  int16_t Begin; // Start marker
  uint16_t Cpt;  // Counter
  float gyro[3]; // Gyroscope data (Pitch, Roll, Yaw)
  float acc[3];  // Accelerometer data
  int16_t End;   // End marker
};

class Hatire
{
private:
  HatireData data = {
      .Begin = (int16_t)0xAAAA, // Explicit cast to int16_t
      .Cpt = 0,
      .gyro = {0.0f, 0.0f, 0.0f},
      .acc = {0.0f, 0.0f, 0.0f},
      .End = (int16_t)0x5555 // Explicit cast to int16_t
  };

public:
  void send(GyroData);
};

#endif