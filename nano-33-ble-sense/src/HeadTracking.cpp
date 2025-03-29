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
#define _HATIRE_SERIAL_

#define SERIAL_SPEED 115200
// #define SERIAL_SPEED 9600

// Gyro gyro; // default
// Gyro gyro(GA_X, GA_Y, GA_Z); // default but explicit
Gyro gyro(GA_Y, GA_Z_I, GA_X_I);
Hatire hatire;
Proximity proximity;
LedRgb led;

BLEService service("FC7C1FD2-0C82-11F0-BBE4-00155DD2D8C6");
BLEStringCharacteristic notification("7E13A434-0C83-11F0-9DE9-00155DD2D8C6", BLENotify, 64);

unsigned long uptime = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  led.begin();

  Serial.begin(SERIAL_SPEED);
  // while (!Serial)
  //   ;

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

  if (!BLE.begin())
  {
#ifdef _DEBUG_
    Serial.println("starting Bluetooth® Low Energy failed!");
#endif
    while (1)
      ;
  }

  BLE.setLocalName("HT N33SBLER2");
  BLE.setAdvertisedService(service);
  service.addCharacteristic(notification);
  BLE.addService(service);
  notification.writeValue("");
  BLE.advertise();
}

GyroData process();

void loop()
{
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if (central)
  {
#ifdef _DEBUG_
    Serial << "Connected to central: " << central.address() << "\n";
#endif

    digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection
    while (central.connected())
    {
      auto data = process();
      if (data.hasData)
      {

#ifdef _DEBUG_
        char buffer[64];
        sprintf(buffer, "R:%f,P:%f,Y:%f", data.roll, data.pitch, data.yaw);
        Serial << buffer << "\n";
#endif
        float roll = data.roll;
        float pitch = data.pitch;
        float yaw = data.yaw;

        char data[sizeof(float) * 3];
        memcpy(data, &roll, sizeof roll);
        memcpy(&data[sizeof(float)], &pitch, sizeof pitch);
        memcpy(&data[sizeof(float) * 2], &yaw, sizeof yaw);

        notification.writeValue(data);
      }
    }
  }
  else
  {
    process();
    digitalWrite(LED_BUILTIN, LOW); // turn on the LED to indicate the connection
  }
}

GyroData process()
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
#ifdef _HATIRE_SERIAL_
    hatire.send(data);
#endif
#endif
  }

  return data;
}