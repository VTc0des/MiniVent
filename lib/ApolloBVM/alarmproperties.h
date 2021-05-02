#ifndef alarmproperties_h
#define alarmproperties_h

#include "buttonmanager.h"
#include "constants.h"
#include "pins.h"
#include "pressuremanager.h"

typedef struct {
  // variables for pressure sensor on expiratory limb
  float max_pip; //35 cmH2O
  float pip;
  float peep;
  float pp;

  // variables for flow sensor on inspiratory limb
  float inspiratoryVolume;
} SensorParameters;

class AlarmIO {

public:
  AlarmIO() : ihold_button{IHOLD_BUTTON_PIN, false}, pressure_input{PS_Vout} {}

  ButtonManager ihold_button;
  PressureManager pressure_input;

  void calibrate() {
    pressure_input.calibrate();
    //add calibration methods for other sensors
  }

  void poll() {
    ihold_button.poll();
    pressure_input.poll();
  }

  //process inspiratory pressure
  float ipProcess() {
    return pressure_input.ipProcess();
  }

  //process plateau pressure
  float ppProcess() {
    return pressure_input.ppProcess();
  }

  //process end expiratory pressure
  float epProcess() {
    return pressure_input.epProcess();
  }
};

#endif
