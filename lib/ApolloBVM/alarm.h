#include "ventproperties.h"

using AlarmFunc = bool (*)(VentIO &vio);

enum Color { red, orange, green };

class Alarm {

public:
  // Default constructor and destructor for array creation in AlarmManager.
  Alarm() {}
  ~Alarm() {}

  // Populating constructor.
  Alarm(AlarmFunc condition, bool blink_led = true, Color led_color = red,
        bool stop_running = true, bool sound_buzzer = true,
        int display_time = -1, bool cancelable = false, String top_text = "top",
        String bottom_text = "bottom")
      : condition(condition), blink_led(blink_led), led_color(led_color),
        stop_running(stop_running), sound_buzzer(sound_buzzer),
        display_time(display_time), cancelable(cancelable), top_text(top_text),
        bottom_text(bottom_text) {}

  // Lambda function evaluating to true when alarm is triggered.
  AlarmFunc condition;

  // Alarm actions to take.
  bool blink_led;
  Color led_color;
  bool stop_running;
  bool sound_buzzer;

  // Alarm display parameters.
  int display_time;
  bool cancelable;
  String top_text;
  String bottom_text;

  // Alarm state variables.
  /* bool prev_triggered; */
  int time_triggered = 0;
  /* int cycles_triggered; */
};

class AlarmManager {

public:
  AlarmManager(VentIO &vio, int num_alarms)
      : _vio(vio), _alarms_len(num_alarms), _prev_time(0) {
    _alarms = new Alarm[num_alarms];
  }

  void addAlarm(int idx, const Alarm &a);
  void addAlarm(int idx, AlarmFunc condition, bool blink_led, Color led_color,
                bool stop_running, bool sound_buzzer, int display_time,
                bool cancelable, String top_text, String bottom_text);
  bool evaluate();

private:
  // Ventilator IO reference.
  VentIO &_vio;

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
