#ifdef SBTST
#include "sensorbuffer.h" //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries.
#include <Arduino.h>

SensorBuffer sb(10);
unsigned long x;

void setup() {
  Serial.begin(9600);

  for (uint16_t i = 0; i < 10; i++) {
    sb.put_back(i);
  }

  Serial.println("Populated SensorBuffer...");
  Serial.println("Printing contents of SensorBuffer..");
  x = 100;
}

void loop() {

  for (uint16_t i = 0; i < 10; i++) {
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" sb: ");
    Serial.println(sb.get(i));
  }

  // Add a break in the serial monitor.
  Serial.println();

  x++;
  Serial.print("Adding new value: ");
  Serial.println(x);
  sb.put_back(x);

  // Add another break.
  Serial.println();

  delay(2 * 1000);
}

#endif
