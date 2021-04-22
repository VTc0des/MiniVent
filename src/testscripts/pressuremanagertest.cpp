#ifdef SBTST
#include "sensorbuffer.h" //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries.
#include <Arduino.h>
#include "alarmproperties.h"

AlarmIO aio;

//Init pressure manager reading with 0 value and the pin for the pressure sensor
PressureManager pm(0,PS_Vout);

void setup() {
  Serial.begin(9600);

}

void loop() {

  
}

#endif
