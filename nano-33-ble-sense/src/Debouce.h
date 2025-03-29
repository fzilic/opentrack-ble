#ifndef _DEBOUNCE_
#define _DEBOUNCE_

class Debounce
{
public:
  Debounce() {}
  Debounce(unsigned long);
  Debounce(unsigned long, unsigned long);

  bool debounce(bool);
  bool getState();

private:
  bool state = false;
  bool lastState = false;
  unsigned long time = 0;
  unsigned long raising = 100;
  unsigned long falling = 100;
};

#endif