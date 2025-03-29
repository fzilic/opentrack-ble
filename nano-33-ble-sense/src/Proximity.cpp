#include "Proximity.h"

#include <Arduino.h>
#include <Arduino_APDS9960.h>

#include "LedRgb.h"

ProximityState Proximity::readProximity()
{
  if (APDS.proximityAvailable())
  {
    int proximity = APDS.readProximity();
    bool proximityState = proximity >= 0 && proximity < this->proximityThreshold;

    bool pending = this->pending.debounce(proximityState);
    this->active.debounce(pending);
  }

  if (this->active.getState())
  {
    return PS_ON;
  }
  else if (this->pending.getState())
  {
    return PS_TRANSITION;
  }
  else
  {
    return PS_OFF;
  }
}