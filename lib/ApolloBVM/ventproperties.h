#ifndef ventsettings_h
#define ventsettings_h

#include "Encoder.h"
#include "buttonmanager.h"
#include "buzzermanager.h"
#include "encodermanager.h"
#include "nhd_0420d3z.h"
#include "pins.h"
#include <LiquidCrystal.h>

typedef struct {

  // Current slave mode.
  char mode;

  // Ventilation settings.
  int tidal_volume;
  int respiration_rate;
  int inhale;
  int exhale;
  // units are in seconds not milliseconds.
  int hold_seconds;
  int hold_decimals;
  int ihold_seconds; // 0.5 seconds is 0,50 for ihold_seconds, ihold_decimals.
  int ihold_decimals;

  // Time between trajectory points.
  int delta_time;

  // Current ventilation time.
  int hours;
  int minute;
  int seconds;

  // Alarm status.
  bool alarm;

  // Boolean to indicate to send settings.
  bool send;

} VentSettings;

typedef struct {

  // Limits for ventilation settings.
  int min_tidal_volume = 300;
  int max_tidal_volume = 700;
  int delta_tidal_volume = 50;

  int min_respiration_rate = 10;
  int max_respiration_rate = 30;
  int delta_respiration_rate = 1;

  int min_exhale = 1;
  int max_exhale = 5;
  int delta_exhale = 1;

} VentLimits;

class VentIO {

public:
  VentIO()
      : disp{DISPLAY_PIN}, enc{ENC_DT_PIN, ENC_CLK_PIN},
        enc_button{ENC_BUTTON_PIN, true}, stop_button{STOP_BUTTON_PIN, false},
        ihold_button{IHOLD_BUTTON_PIN, false},
        limit_right{LIMIT_RIGHT_PIN, true}, limit_left{LIMIT_LEFT_PIN, true},
        buzzer(BUZZER_PIN) {}

  NhdDisplay disp;
  Encoder enc;
  ButtonManager enc_button;
  ButtonManager stop_button;
  ButtonManager ihold_button;
  ButtonManager limit_right;
  ButtonManager limit_left;

  BuzzerManager buzzer;

  void poll() {
    enc_button.poll();
    stop_button.poll();
    ihold_button.poll();
    limit_right.poll();
    limit_left.poll();
    buzzer.poll();
    // poll pressure sensor value?
  }

  void LED() {
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(GREEN_LIGHT, OUTPUT);
    pinMode(BLUE_LIGHT, OUTPUT);
  }
};

#endif
