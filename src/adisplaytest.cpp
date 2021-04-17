#ifdef ADT
#include "Wire.h"
#include "alarmdisplay.h"
#include "alarmproperties.h"
#include <Arduino.h>

AlarmIO aio();

AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);

Alarm *test = new Alarm(
    [](AlarmIO &aio) {
      Serial.println("Hi from alarm!");
      return true;
    },
    "dummy alarm");

void setup() {
  Serial.begin(9600);
  ad.start();
}

void loop() {

  ad.update(test, 3.4, 5.6,
            7.8); // pass in values for pip, peep, pp from algorithm
}

#endif
