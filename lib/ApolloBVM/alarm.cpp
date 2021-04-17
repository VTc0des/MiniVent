#include "alarm.h"

void AlarmManager::addAlarm(int idx, const Alarm &a) { _alarms[idx] = a; }

void AlarmManager::addAlarm(int idx, AlarmFunc condition, String text) {
  _alarms[idx] = Alarm(condition, text);
}

Alarm &AlarmManager::evaluate() {

  /* int delta_time = millis() - _prev_time; */
  /* _prev_time = millis(); */

  for (int i = 0; i < _alarms_len; i++) {
    // Check if each alarm has been triggered.
    if (_alarms[i].condition(_aio)) {

      // Increment the number of cycles this alarm has been triggered.
      _alarms[i].cycles_triggered++;

      if (_alarms[i].cycles_triggered > _time_until_display) {
        return &_alarms[i];
      }
    } else {

      // If we've seen that this alarm has been triggered many times in the
      // past, reset.
      /* TODO:  <17-04-21, Therring> Add logic for counting cycles where alarm
       * is false before resetting.*/

      _alarms[i].cycles_triggered = 0;
    }
  }

  // No alarm should be displayed.
  return 0;
}
