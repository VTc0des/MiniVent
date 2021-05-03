#ifndef alarmdisplay_h
#define alarmdisplay_h

#include "alarm.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

class AlarmDisplay {
public:
  AlarmDisplay(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2,
               uint8_t d3, uint8_t red, uint8_t green, uint8_t blue);

  void warning();
  void start();
  void update(Alarm *alarm, float pip, float peep, float pp);
  void IHoldMessage();
  void updateIHold(float pip, float peep, float pp);
  void setBacklight(uint8_t r, uint8_t g, uint8_t b);
  void clearOneLine(uint8_t row_pos);

private:
  const String _warning_lines[4] = {"MINIVENT", "WARNING:", "USE PEDI SIZED", "BAG VALVE MASK"};
  const String _display_lines[4] = {"PIP  = ", "PEEP = ", "PP   = ", "cmH2O"};
  const String _ihold_lines[2] = {"I-HOLD INITIATED    ", "I-HOLD PARAMETERS   "};
  const uint8_t _cols = 20;
  const uint8_t _rows = 4;
  const uint8_t _brightness = 255;

  LiquidCrystal *_lcd; // create pointer to LCD display, initialize later

  uint8_t _red_pin;
  uint8_t _green_pin;
  uint8_t _blue_pin;

  // variables to hold values that are outputted to the display
  //put these floats in pressure manager, then pass them to the update functions
  float _pip;
  float _peep;
  float _pp;
};

#endif
