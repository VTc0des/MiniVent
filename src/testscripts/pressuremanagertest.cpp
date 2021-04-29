#ifdef SBTST
#include <Arduino.h>
#include "alarmproperties.h"


AlarmIO aio;

//Init pressure manager reading with 0 value and the pin for the pressure sensor
PressureManager pm(PS_Vout);

void setup() {
  Serial.begin(9600);

  pinMode(F2_TO_F1, OUTPUT);
  pinMode(F1_TO_F2, INPUT);

  aio.calibrate();
}

void loop() {
  aio.poll(); 
  pip = aio.process();
}

#endif
