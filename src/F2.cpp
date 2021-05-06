#ifdef ALM

#include <Arduino.h>
#include "Wire.h"
#include "alarmdisplay.h"
#include "alarmproperties.h"    //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries. 

AlarmIO aio;

//initialize all parameters.
SensorParameters sp = {35, 3, 0, 0, 0, 0}; 
IHoldParameters ip = {100, 0, false, false, 5, 0, false};
 
AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);
AlarmManager am(aio, 3);          //passing reference to aio.
PressureManager pm(PS_Vout);      //initialize with pressure pin input

void setup() {
  Serial.begin(9600);
  
  // Congifure pinMode for communicating F2 <-> F1;
  pinMode(F2_TO_F1, OUTPUT);
  pinMode(F1_TO_F2, INPUT);
  
  ad.warning();
  ad.start();

  am.addAlarm(0, [](AlarmIO &aio) {
        //change this "condition" to say flow less than 0 when flow sensor is included on the inspiratory circuit. 
        return sp.inspiratoryVolume < 0; //if peak inspiratory pressure is less than 0
      },
      "Alert: Awake        "); 

  am.addAlarm(1, [](AlarmIO &aio) {
        return sp.pip > sp.max_pip;
      },
      "Alert: High Insp P  "); 

  am.addAlarm(2, [](AlarmIO &aio) {
        //function to get expiratory pressure reading
        return sp.peep < sp.min_peep;
      },
      "Alert: Low Exp P    ");

  aio.calibrate();
}

void loop() {
  // Poll I-Hold button state AND pressure sensor
  aio.poll();

  //SIGNAL TO F1
  if (aio.ihold_button.getButtonState() == true) {
    ip.iholdInitTime = millis(); 
    digitalWrite(F2_TO_F1, HIGH);
    ip.signalSent = true;
    ip.iHmessageDisplayed = true;
    ad.IHoldMessage();
  }
  else {
    if (ip.signalSent) {
      //only when I-Hold is initiated, monitor time to send long signal
      if (millis() - ip.iholdInitTime > ip.signalDuration)
      {
        ip.signalSent = false;
        digitalWrite(F2_TO_F1, LOW);
      }
    }
    else {
      digitalWrite(F2_TO_F1, LOW);
    }
  }

  //PROCESS PRESSURE SENSOR DATA
  sp.pip = aio.ipProcess();
  sp.peep = aio.epProcess();
  //READ SIGNAL FROM F1
  if (digitalRead(F1_TO_F2) == HIGH) {
    aio.iholdPoll();

    //these variables will constantly be updated while the signal is high. the last state that it saves is the moment before the signal writes low. 
    ip.iHmessageDisplayed = false;
    ip.iHoldDisplay = true;
    ip.iholdMsgStart = millis();
  }

  //UPDATE DISPLAY
  if (ip.iHoldDisplay) {
    //if ihold maneuver completed, the calculate plateau pressure
    if (digitalRead(F1_TO_F2) == LOW) {
      sp.pp = aio.ppProcess();
    }

    ad.updateIHold(sp.pip, sp.peep, sp.pp);

    if (millis() - ip.iholdMsgStart > (ip.displayDuration*100)) {
      ip.iHoldDisplay = false;
    }
  }
  else if (ip.iHmessageDisplayed) {
    ad.IHoldMessage();
  }
  else {
      //evaluate alarms each cycle
      Alarm *triggered_alarm = am.evaluate();
      //update display with alarm status and patient parameters
      ad.update(triggered_alarm, sp.pip, sp.peep, sp.pp);
  }
} //end of loop

#endif
