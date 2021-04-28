#ifdef ADT

#include <Arduino.h>
#include "Wire.h"
#include "alarmdisplay.h"
#include "alarmproperties.h"    //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries. 

// Pins for MC connections
#define F2_TO_F1 2
#define F1_TO_F2 13

AlarmIO aio;

AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);
AlarmManager am(aio, 3); //passing reference to aio.


void setup() {
  Serial.begin(9600);
  
  // Congifure pinMode for communicating F2 <-> F1;
  pinMode(F2_TO_F1, OUTPUT);
  pinMode(F1_TO_F2, INPUT);
  
  ad.start();

  am.addAlarm (0,[](AlarmIO &aio) {
      //function to get inspiratory pressure reading
      return false;
    },
    "Alert: High Insp P"); //idx, condition, string text

      am.addAlarm (1,[](AlarmIO &aio) {
      //function to get expiratory pressure reading
      return false;
    },
    "Alert: Low Exp P"); //idx, condition, string text

      am.addAlarm (2,[](AlarmIO &aio) {
      //function is inspiration is detected. 
      return false;
    },
    "Alert: Independent P"); //idx, condition, string text
}

void loop() {
  // Poll I-Hold button state AND pressure sensor
  aio.poll();
  
  // cyclically evaluate high pressure, low end expiratory pressure, and negative pressure in order of importance. 
  Alarm *triggered_alarm = am.evaluate(); //create a pointer variable to stroe the value of the pointer
  ad.update(triggered_alarm, 3.4, 5.6, 7.8); // pass in values for pip, peep, pp from algorithm, don't put * dont want to dereference

  if (aio.ihold_button.getButtonState() == true) {
    digitalWrite(F2_TO_F1, HIGH);
    Serial.println("Button Input detected");
  }
  else { 
    digitalWrite(F2_TO_F1, LOW);
  }

  if (digitalRead(F1_TO_F2) == HIGH) {
    // identify plateau pressure specifically.
    Serial.println("I-Hold Started!");
    ad.startIHold();
  }
  else {
    // when the signal turns off, replace with this panel
    ad.updateIHold(9.4, 10.6, 5.8);
    Serial.println("I-Hold Complete!");
  }
}

#endif
