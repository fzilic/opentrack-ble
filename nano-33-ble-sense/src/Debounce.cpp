#include "Debouce.h"

#include <Arduino.h>

Debounce::Debounce(unsigned long debounce)
{
  this->raising = debounce;
  this->falling = debounce;
}

Debounce::Debounce(unsigned long raising, unsigned long falling)
{
  this->raising = raising;
  this->falling = falling;
}

bool Debounce::debounce(bool state)
{
  unsigned long now = millis();
  if (state != this->lastState)
  {
    this->time = now;
  }

  // state change
  if (state != this->state &&
      // raising
      ((this->state == false && this->raising <= now - this->time) ||
       // falling
       (this->state == true && this->falling <= now - this->time)))
  {
    this->state = state;
  }

  this->lastState = state;

  return this->state;
}

bool Debounce::getState()
{
  return this->state;
}