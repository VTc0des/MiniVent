#include "alarmdisplay.h"

AlarmDisplay::AlarmDisplay(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1,
                           uint8_t d2, uint8_t d3, uint8_t red, uint8_t green, uint8_t blue)
    : _red_pin(red), _green_pin(green), _blue_pin(blue), _pip(0), _peep(0),
      _pp(0) {
  _lcd = new LiquidCrystal(rs, enable, d0, d1, d2, d3);
}

void AlarmDisplay::start() {
  _lcd->begin(_cols, _rows); // pointer will populate memory address of _lcd
  setBacklight(0, 128, 128); // set blue-green color for display

  _lcd->setCursor(0, 1);
  _lcd->print(_display_lines[0]);

  _lcd->setCursor(0, 2);
  _lcd->print(_display_lines[1]);

  _lcd->setCursor(0, 3);
  _lcd->print(_display_lines[2]);
}

void AlarmDisplay::update(Alarm *alarm, float pip, float peep, float pp) {
  // use update to write new values while headers remain the same.

  _pip = pip;
  _peep = peep;
  _pp = pp;
    
  if (alarm != 0) {
    _lcd->setCursor(0, 0);
    _lcd->print(alarm->text);
  }

  _lcd->setCursor(8, 1);
  _lcd->print(_pip);

  _lcd->setCursor(8, 2);
  _lcd->print(_peep);

  _lcd->setCursor(8, 3);
  _lcd->print(_pp);
}

void AlarmDisplay::IHoldMessage() {
  _lcd->clear();
  setBacklight(0, 128, 128); // set blue-green color for display

  _lcd->setCursor(0, 1);
  _lcd->print(_ihold_lines[0]);

  _lcd->setCursor(0,2);
  _lcd->print(_ihold_lines[1]);
}

void AlarmDisplay::startIHold() {
  _lcd->clear();
  setBacklight(0, 128, 128); // set blue-green color for display

  _lcd->setCursor(0,0);
  _lcd->print(_ihold_lines[2]);

  _lcd->setCursor(0, 1);
  _lcd->print(_ihold_lines[3]);

  _lcd->setCursor(0, 2);
  _lcd->print(_ihold_lines[4]);

  _lcd->setCursor(0, 3);
  _lcd->print(_ihold_lines[5]);
}

void AlarmDisplay::updateIHold (float pip, float peep, float pp) {
  _lcd->setCursor(8, 1);
  _lcd->print(_pip);

  _lcd->setCursor(8, 2);
  _lcd->print(_peep);

  _lcd->setCursor(8, 3);
  _lcd->print(_pp);
}

void AlarmDisplay::setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  // r = map(r, 0, 255, 0, 100);
  // g = map(g, 0, 255, 0, 150);

  r = map(r, 0, 255, 0, 255);
  g = map(g, 0, 255, 0, 255);

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
