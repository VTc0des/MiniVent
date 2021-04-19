#ifndef alarmproperties_h
#define alarmproperties_h

/* #include "alarmdisplay.h" */
#include "buttonmanager.h"
#include "constants.h"
#include "pins.h"

typedef struct {
  bool ihold;        // stores ihold button state on master controller
  bool holdComplete; // is to trigger resending values to slave controller.
} InspiratoryHold;

typedef struct {
  // boolean flags for various alarm statuses
  bool highP; // if breathing in pressure is too high.
  bool lowP;  // if auto-peep is occuring
  bool insp;  // if patient is breathing in

  // pressure values
  int high_pressure;
  int low_peep;
  int inspiratory_pressure;
} AlarmSettings;

typedef struct {
  // variables for pressure sensor algorithm
  int inspiratoryPressure;
  int expiratoryPressure;
  int plateauPressure;
} SensorParameters;

typedef struct {
  // all time constants are in milliseconds
  // for setup display duration
  unsigned long prevDispTime;
  unsigned long currDispTime;
  // for display update duration
  unsigned long prevUpdate;
  unsigned long currUpdate;
  // for pressures sensor measurement
  unsigned long prevPressureRead;
  unsigned long currPresssureRead;
  // for ihold measurement
  unsigned long previhold;
  unsigned long currihold;

  const long interval;    // read pressure data for 10 seconds (~2/3 cycles).
                          // change based on insp and exp cycle.
  const long inspHold;    // recommended time for inspiratory hold (0.5 seconds)
  const long displayTime; // time to display the intro message
} TimeConsts;

class AlarmIO {

public:
  AlarmIO() : ihold_button{IHOLD_BUTTON_PIN, false} {};

  ButtonManager ihold_button;

  void poll() {
    ihold_button.poll();
    // poll pressure sensor value?
  }

  void LED() {
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(GREEN_LIGHT, OUTPUT);
    pinMode(BLUE_LIGHT, OUTPUT);
  }
};

#endif
