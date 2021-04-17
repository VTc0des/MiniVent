#include "alarm.h"

void AlarmManager::addAlarm(int idx, const Alarm &a) { _alarms[idx] = a; }

void AlarmManager::addAlarm(int idx, AlarmFunc condition, bool blink_led,
                            Color led_color, bool stop_running,
                            bool sound_buzzer, int display_time,
                            bool cancelable, String top_text,
                            String bottom_text) {
  _alarms[idx] =
      Alarm(condition, blink_led, led_color, stop_running, sound_buzzer,
            display_time, cancelable, top_text, bottom_text);
}

bool AlarmManager::evaluate() {

  /* int delta_time = millis() - _prev_time; */
  /* _prev_time = millis(); */

  for (int i = 0; i < _alarms_len; i++) {
    // Check if each alarm has been triggered.
    if (_alarms[i].condition(_vio)) {
      // Add the delta time to the total time this alarm has been triggered.
      /* cur.time_triggered += delta_time; */
      _alarms[i].time_triggered++;
      Serial.println(_alarms[i].time_triggered);

      if (_alarms[i].time_triggered > _time_until_display) {

        /*   // TODO: Change the Alarm Panel to display this alarm. */

        return true;
      }
    } else {
      /* // TODO: Add time that alarm hasn't been triggered for to know when to
       */
      /* // reset the time_triggered counter. */
    }
  }

  // No alarm should be displayed.
  return false;
}
