#include "alarmdisplay.h"

AlarmDisplay::AlarmDisplay(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1,
                           uint8_t d2, uint8_t d3, uint8_t red, uint8_t green, uint8_t blue)
    : _red_pin(red), _green_pin(green), _blue_pin(blue), _pip(0), _peep(0),
      _pp(0) {
  _lcd = new LiquidCrystal(rs, enable, d0, d1, d2, d3);
}

void AlarmDisplay::warning() {
  _lcd->begin(_cols, _rows); // pointer will populate memory address of _lcd
  setBacklight(0, 128, 128); // set blue-green color for display

  _lcd->setCursor(6, 1);
  _lcd->print(_warning_lines[0]);

  delay(2000);
  _lcd->clear();

  _lcd->setCursor(6,0);
  _lcd->print(_warning_lines[1]);

  _lcd->setCursor(3, 1);
  _lcd->print(_warning_lines[2]);

  _lcd->setCursor(3, 2);
  _lcd->print(_warning_lines[3]);

  delay(2000);
}

void AlarmDisplay::start() {
  _lcd->clear();
  // print header and units
  _lcd->setCursor(0, 1);
  _lcd->print(_display_lines[0]);

  _lcd->setCursor(15, 1);
  _lcd->print(_display_lines[3]);

  _lcd->setCursor(0, 2);
  _lcd->print(_display_lines[1]);

  _lcd->setCursor(15, 2);
  _lcd->print(_display_lines[3]);

  _lcd->setCursor(0, 3);
  _lcd->print(_display_lines[2]);

  _lcd->setCursor(15, 3);
  _lcd->print(_display_lines[3]);

}

void AlarmDisplay::update(Alarm *alarm, float pip, float peep, float pp) {
  _pip = pip;
  _peep = peep;
  _pp = pp;
    
  if (alarm != 0) {
    setBacklight(255, 0, 0);  //set backlight to red
    _lcd->setCursor(0, 0);
    _lcd->print(alarm->text); //print top line with text specified in src file
  }
  else {
    setBacklight(0, 255, 0);  //set backlight to green
    _lcd->setCursor(0, 0);
    _lcd->print("Alert: NONE         ");
  }

  _lcd->setCursor(8, 1);
  _lcd->print(_pip);

  _lcd->setCursor(8, 2);
  _lcd->print(_peep);

  _lcd->setCursor(8, 3);
  _lcd->print(_pp);
}

void AlarmDisplay::IHoldMessage() {
  _lcd->setCursor(0, 0);
  _lcd->print(_ihold_lines[0]);
}

void AlarmDisplay::updateIHold (float pip, float peep, float pp) {
  _lcd->setCursor(0, 0);
  _lcd->print(_ihold_lines[1]);

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

// try not to use this function of _lcd->clear() as it causes the screen to flicker upon update
void AlarmDisplay::clearOneLine(uint8_t row_pos) {
  // set cursor at beginning of row. 
  _lcd->setCursor(0,row_pos);
  // print a row of spaces
  for (uint8_t i=0; i<20; i++) {
    _lcd->print(" ");
  }
}