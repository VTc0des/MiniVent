#include "alarmdisplay.h"

AlarmDisplay::AlarmDisplay(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1,
                           uint8_t d2, uint8_t d3, int red, int green, int blue)
    : _red_pin(red), _green_pin(green), _blue_pin(blue), _pip(0), _peep(0),
      _pp(0) {
  _lcd = new LiquidCrystal(rs, enable, d0, d1, d2, d3);
}

void AlarmDisplay::start() {
  _lcd->begin(_cols, _rows); // pointer will populate memory address of _lcd
  setBacklight(0, 128, 128); // set blue-green color for display

  _lcd->setCursor(0, 1);
  _lcd->print("PIP =  ");

  _lcd->setCursor(0, 2);
  _lcd->print("PEEP = ");

  _lcd->setCursor(0, 3);
  _lcd->print("PP =   ");
}

void AlarmDisplay::update(Alarm *alarm, float pip, float peep, float pp) {
  _pip = pip;
  _peep = peep;
  _pp = pp;
  // use update to write new values while headers remain the same.
  //_lcd->clear();

  // for (int i = 0; i < 4; i++)
  // {
  //     _lcd->setCursor(8, i);
  //     _lcd->print();
  // }

  if (alarm != 0) {
    _lcd->setCursor(4, 0);
    _lcd->print(alarm->text);
  }

  //_lcd->clear();
  _lcd->setCursor(8, 1);
  _lcd->print(_pip);

  _lcd->setCursor(8, 2);
  _lcd->print(_peep);

  _lcd->setCursor(8, 3);
  _lcd->print(_pp);
}

void AlarmDisplay::setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);

  r = map(r, 0, 255, 0, _brightness);
  g = map(g, 0, 255, 0, _brightness);
  b = map(b, 0, 255, 0, _brightness);

  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);

  analogWrite(_red_pin, r);
  analogWrite(_green_pin, g);
  analogWrite(_blue_pin, b);
}
