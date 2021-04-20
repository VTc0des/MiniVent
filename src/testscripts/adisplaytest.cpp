#ifdef TST/
#include "Wire.h"
#include "alarmdisplay.h"
#include "alarmproperties.h"    //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries. 
#include <Arduino.h>

AlarmIO aio;

AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);
AlarmManager am(aio, 3); //passing reference to aio.


void setup() {
  Serial.begin(9600);
  ad.start();

  am.addAlarm (0,[](AlarmIO &aio) {
      Serial.println("Hi from alarm!");
      //function to get inspiratory pressure reading
      return false;
    },
    "Alert: High Insp P"); //idx, condition, string text

      am.addAlarm (1,[](AlarmIO &aio) {
      Serial.println("Hi from alarm!");
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

  if (aio.ihold.getButtonState() == true) {
    digitalWrite(F2_to_F1, HIGH);
    ad.startIHold();
  }
  else { 
    digitalWrite(F2_to_F1, LOW);
  }

  if (digitalRead(F1_to_F2)) {
    // identify plateau pressure specifically.
    Serial.println("I-Hold Started!");
  }
  else {
    // when the signal turns off, replace with this panel
    ad.updateIHold(3.4, 5.6, 7.8);
  }
}

#endif
