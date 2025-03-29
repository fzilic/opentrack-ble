#include <Arduino.h>

#include <Arduino_APDS9960.h>
#include <Arduino_BMI270_BMM150.h>
#include <ArduinoBLE.h>

#include <Streaming.h>

#include "LedRgb.h"
#include "Gyro.h"
#include "Hatire.h"
#include "Proximity.h"

// #define _DEBUG_

#define SERIAL_SPEED 115200
// #define SERIAL_SPEED 9600

// Gyro gyro; // default
// Gyro gyro(GA_X, GA_Y, GA_Z); // default but explicit
Gyro gyro(GA_Y, GA_Z_I, GA_X_I);
Hatire hatire;
Proximity proximity;
LedRgb led;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  led.begin();

  Serial.begin(SERIAL_SPEED);
  while (!Serial)
    ;

  if (!APDS.begin())
  {
#ifdef _DEBUG_
    Serial.println("Error initializing APDS-9960 sensor!");
#endif
  }

  if (!IMU.begin())
  {
#ifdef _DEBUG_
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
#endif
  }

  led.yellow();
  delay(2000);

  led.red();
  gyro.calibrate(500);
  led.green();
}

void loop()
{

  if (APDS.gestureAvailable())
  {
    if (APDS.readGesture() != GESTURE_NONE)
    {
#ifdef _DEBUG_
      Serial << "D:Zero\n";
#endif
      gyro.zero();
    }
  }

  auto state = proximity.readProximity();
  if (state == PS_ON)
  {
#ifdef _DEBUG_
    Serial << "D:Calibrate\n";
#endif
    led.red();
    gyro.calibrate(1000);
    proximity = Proximity();
  }
  else if (state == PS_TRANSITION)
  {
    led.yellow();
  }
  else
  {
    led.green();
  }

  GyroData data = gyro.read();
  if (data.hasData)
  {
#ifdef _DEBUG_
    data.print();
#else
    hatire.send(data);
#endif
  }
}
