#ifndef _LED_RGB_
#define _LED_RGB_

enum LedRgbColor
{
  LRC_WHITE,
  LRC_RED,
  LRC_GREEN,
  LRC_BLUE,
  LRC_PURPLE,
  LRC_YELLOW,
  LRC_CYAN,
  LRC_OFF
};

class LedRgb
{
public:
  void begin();
  void set(LedRgbColor);

  void white();
  void red();
  void green();
  void blue();
  void purple();
  void yellow();
  void cyan();
  void off();
};

#endif
