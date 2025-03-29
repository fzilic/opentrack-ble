#include "LedRgb.h"

#include <Arduino.h>

void LedRgb::begin()
{
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void LedRgb::set(LedRgbColor color)
{
  switch (color)
  {
  case LRC_WHITE:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    break;
  case LRC_RED:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    break;
  case LRC_GREEN:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    break;
  case LRC_BLUE:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
    break;
  case LRC_PURPLE:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
    break;
  case LRC_YELLOW:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    break;
  case LRC_CYAN:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    break;
  case LRC_OFF:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    break;
  }
}

void LedRgb::white()
{
  this->set(LRC_WHITE);
}

void LedRgb::red()
{
  this->set(LRC_RED);
}

void LedRgb::green()
{
  this->set(LRC_GREEN);
}

void LedRgb::blue()
{
  this->set(LRC_BLUE);
}

void LedRgb::purple()
{
  this->set(LRC_PURPLE);
}

void LedRgb::yellow()
{
  this->set(LRC_YELLOW);
}

void LedRgb::cyan() 
{
  this->set(LRC_CYAN);
}

void LedRgb::off()
{
  this->set(LRC_OFF);
}