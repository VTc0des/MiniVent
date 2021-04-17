#ifdef ADT
#include <Arduino.h>
#include "Wire.h"
#include "alarmproperties.h"

AlarmDisplay ad(RS, EN, D4, D5, D6, D7, RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT);

void setup ()    
{
    Serial.begin(9600);
    ad.start();
}

void loop ()
{
    ad.update(3.4, 5.6, 7.8); //pass in values for pip, peep, pp from algorithm
}


#endif