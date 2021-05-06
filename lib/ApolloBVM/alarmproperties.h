#ifndef alarmproperties_h
#define alarmproperties_h

#include "buttonmanager.h"
#include "constants.h"
#include "pins.h"
#include "pressuremanager.h"

typedef struct {
  // variables for pressure sensor on expiratory limb
  double max_pip;  //35 cmH2O
  double min_peep; //3 cmH2O OR 5 cmH2O
  double pip;
  double peep;
  double pp;

  // variables for flow sensor on inspiratory limb
  double inspiratoryVolume;
} SensorParameters;

typedef struct {
  const uint8_t signalDuration;
  unsigned long iholdInitTime;
  bool signalSent;

  bool iHmessageDisplayed;

  const uint8_t displayDuration;  //display for 2000 ms. 
  unsigned long iholdMsgStart;
  bool iHoldDisplay;
} IHoldParameters;

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

  void iholdPoll() {
    pressure_input.iholdPoll();
  }

  //process inspiratory pressure
  double ipProcess() {
    return pressure_input.ipProcess();
  }
  
  //process end expiratory pressure
  double epProcess() {
    return pressure_input.epProcess();
  }

  //process plateau pressure
  double ppProcess() {
    return pressure_input.ppProcess();
  }
};

#endif
