#ifndef alarm_h
#define alarm_h

#include "alarmproperties.h"

using AlarmFunc = bool (*)(AlarmIO &aio);

enum Color { red, orange, green };

class Alarm {

public:
  // Default constructor and destructor for array creation in AlarmManager.
  Alarm() {}
  ~Alarm() {}

  // Populating constructor.
  Alarm(AlarmFunc condition, String text = "alarm")
      : condition(condition), text(text){};

  // Lambda function evaluating to true when alarm is triggered.
  AlarmFunc condition;

  String text;

  // Alarm state variables.
  /* bool prev_triggered; */
  int cycles_triggered = 0;
  /* int cycles_triggered; */
};

class AlarmManager {

public:
  AlarmManager(AlarmIO &aio, int num_alarms)
      : _aio(aio), _alarms_len(num_alarms), _prev_time(0) {
    _alarms = new Alarm[num_alarms];
  };

  void addAlarm(int idx, const Alarm &a);
  void addAlarm(int idx, AlarmFunc condition, String text);
  Alarm *evaluate();

private:
  // Ventilator IO reference.
  AlarmIO &_aio;

  // Array of alarms to evaluate.
  Alarm *_alarms;
  int _alarms_len;

  // Alarm Panel instance.
  /* AlarmPanel _alarm_panel; */

  // Number of mashine cycles alarm condition has to be valid to be
  // considered triggered.
  /* const int time_until_triggered = 100; */

  // Number of machine cycles which alarm has to be triggered before we
  // display alarm.
  /* const int cycles_until_display = 3; */

  // Number of miliseconds an alarm must be triggered before we display.
  const int _time_until_display = 5;

  // Previous time alarm manager was run.
  int _prev_time;
};

#endif
