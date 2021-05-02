#ifdef ADT

#include <Arduino.h>
#include "Wire.h"
#include "alarmdisplay.h"
#include "alarmproperties.h"    //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries. 

// Pins for MC connections
#define F2_TO_F1 2
#define F1_TO_F2 13

unsigned long iholdInitTime;
const uint8_t pulseDuration = 100;
bool signalSent;

AlarmIO aio;

SensorParameters sp = {35, 0, 0, 0, 0}; //initialize all parameters
AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);
AlarmManager am(aio, 3);          //passing reference to aio.
PressureManager pm(PS_Vout);      //initialize with pressure pin input



void setup() {
  Serial.begin(9600);
  
  // Congifure pinMode for communicating F2 <-> F1;
  pinMode(F2_TO_F1, OUTPUT);
  pinMode(F1_TO_F2, INPUT);

  signalSent = false;
  
  ad.warning();
  ad.start();

  am.addAlarm(0, [](AlarmIO &aio) {
        //change this "condition" to say flow less than 0 when flow sensor is included on the inspiratory circuit. 
        sp.inspiratoryVolume < 0; //if peak inspiratory pressure is less than 0
        return true;
      },
      "Alert: Awake"); 

  am.addAlarm(1, [](AlarmIO &aio) {
        sp.pip > sp.max_pip;
        return true;
      },
      "Alert: High Insp P"); 

  am.addAlarm(2, [](AlarmIO &aio) {
        //function to get expiratory pressure reading
        return false;
      },
      "Alert: Low Exp P");

  aio.calibrate();
}

void loop() {
  // Poll I-Hold button state AND pressure sensor
  aio.poll();

  if (aio.ihold_button.getButtonState() == true) {
    iholdInitTime = millis(); //read the time once then each iteration check the time and digitalWrite high
    digitalWrite(F2_TO_F1, HIGH);
    signalSent = true;
    // ad.IHoldMessage();
    //write method to clear display without using clear display? maybe just used _lcd->start() after certain amt of time.
  }
  else {
    if (signalSent)  {
      if (millis() - iholdInitTime > pulseDuration) {
        signalSent = false;
        digitalWrite(F2_TO_F1, LOW);
      }
    }
    else {
      digitalWrite(F2_TO_F1, LOW);
      Serial.println("eh");
    }
  }

  // sp.pip = aio.ipProcess();
  // sp.peep = aio.epProcess();
  
  // if (digitalRead(F1_TO_F2) == HIGH) {
  //   ad.startIHold();
  //   sp.pp = aio.ppProcess();
  // }
  // else {
  //   // when the signal turns off, replace with this panel
  //   ad.updateIHold(sp.pip, sp.peep, sp.pp);
  // }
 
  // Alarm *triggered_alarm = am.evaluate(); 
  
  
  // ad.update(triggered_alarm, sp.pip, sp.peep, sp.pp);
}

#endif
