#ifndef _PROX_
#define _PROX_

#include "Debouce.h"

enum ProximityState
{
  PS_OFF,
  PS_TRANSITION,
  PS_ON
};

class Proximity
{
public:
  int proximityThreshold = 100;
  ProximityState readProximity();

private:
  Debounce pending = Debounce();
  Debounce active = Debounce(5000, 100);
};
#endif