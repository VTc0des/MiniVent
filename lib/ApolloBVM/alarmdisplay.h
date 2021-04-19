#ifndef alarmdisplay_h
#define alarmdisplay_h

#include "alarm.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

class AlarmDisplay {
public:
  AlarmDisplay(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2,
               uint8_t d3, int red, int green, int blue);

  void start();
  void update(Alarm *alarm, float pip, float peep, float pp);
  void startIHold();
  void updateIHold(float pip, float peep, float pp);
  void setBacklight(uint8_t r, uint8_t g, uint8_t b);

private:
  const String _display_lines[3] = {"PIP = ", "PEEP = ", "PP = "};
  const String _ihold_lines[6] = {"I-HOLD INITIATED", "Wait for Parameters", "  IHold Parameters",
                                  "PIP = ", "PEEP = ", "PP = "};
  const uint8_t _cols = 20;
  const uint8_t _rows = 4;
  const uint8_t _brightness = 255;

  LiquidCrystal *_lcd; // create pointer to LCD display, initialize later

  uint8_t _red_pin;
  uint8_t _green_pin;
  uint8_t _blue_pin;

  // variables to hold values that are outputted to the display
  float _pip;
  float _peep;
  float _pp;
};

#endif
