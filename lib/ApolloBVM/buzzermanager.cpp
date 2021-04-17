#include "buzzermanager.h"

void BuzzerManager::startBuzzer() {
  _play = true;
  _state = true;
}

void BuzzerManager::stopBuzzer() {
  _play = false;
  _state = false;
}

void BuzzerManager::poll() {
  if (_state) {

    if (_play) {
      tone(_buzzer_pin, _freq);
    } else {
      noTone(_buzzer_pin);
    }

    if (!(millis() % _duration)) {
      // Every _duration miliseconds, flip to buzzer to play or not.
      _play = !_play;
    }
  }
}
